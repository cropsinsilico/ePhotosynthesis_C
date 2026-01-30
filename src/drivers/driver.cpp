/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include "drivers/driver.hpp"
#include "Variables.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <sunnonlinsol/sunnonlinsol_newton.h>
#include "drivers/CVodeMem.hpp"
#include <cstdio>

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

#define INITIALIZE_VARS_AND_CONDITIONS                  \
  bool con_created = false;                             \
  if (!theVars0) theVars0 = currentVariables();         \
  Variables* cpyVars = nullptr;                         \
  const Variables* theVars = theVars0;                  \
  if (!con) {                                           \
    con = currentConditions();                          \
    con_created = true;                                 \
  }                                                     \
  std::map<MODULE, const ValueSet_t*> conditions;       \
  if (con) {                                            \
    cpyVars = theVars0->deepcopy();                     \
    cpyVars->setRecord(con, conditions);                \
    theVars = cpyVars;                                  \
  }

#define FINALIZE_VARS_AND_CONDITIONS            \
  if (cpyVars) {                                \
    delete cpyVars;                             \
  }                                             \
  if (con_created && con) {                     \
    delete con;                                 \
  }

bool DriverParam::showWarnings = false;

Driver::Driver(Variables *theVars, const double startTime,
	       const double stepSize, const double endTime,
	       const int maxSubsteps,
	       const double atol, const double rtol,
	       const std::size_t para, const double ratio,
	       const bool showWarn,
	       const std::vector<std::string>& outVars) :
  DriverParam(startTime, stepSize, endTime, maxSubsteps,
	      atol, rtol, para, ratio, showWarn, outVars) {
#ifdef SUNDIALS_CONTEXT_REQUIRED
  this->_context = theVars->context_ptr();
#endif // SUNDIALS_CONTEXT_REQUIRED
  this->inputVars = theVars;
  theVars->finalizeInputs();
  initialStep = stepSize;
  _dumpInterval = stepSize;
  maxStep = 20. * step;
  data = nullptr;
  currentVars = nullptr;
  intermediateRes = nullptr;
  _lastStep = false;
  _dumpStep = true; // Output the first step
  _lastDumpTime = 0.0;
  if (inputVars->useC3 && inputVars->EnzymeAct.empty())
      throw std::runtime_error("EnzymeAct must be set if useC3 is True (automatically set for EPS driver)");
}

const Variables* Driver::currentVariables() const {
  if (currentVars) return currentVars;
  return inputVars;
}

Variables* Driver::currentVariables() {
  if (currentVars) return currentVars;
  return inputVars;
}

void Driver::setup(Variables* theVars, const bool continuingRun) {
  if (!theVars) {
    if (currentVars) {
      delete currentVars;
      currentVars = nullptr;
    }
    currentVars = new Variables(inputVars);
    theVars = currentVars;
  }
  bool dumpStep = _dumpStep;
  if (continuingRun)
    _dumpStep = false;
  else
    _lastDumpTime = 0.0;
  theVars->finalizeInputs(true);
  // Initialize the structure of the model, i.e. Is this model separate
  // or combined with others.
  IniModelCom(theVars);
  setup_connections(theVars);
  // The time information is set in a global variable called tglobal in SYSInitial.
  SYSInitial(theVars);
  setup_variables(theVars);
  arr prev_constraints = constraints;
  if (!continuingRun) {
    setup_param(theVars);
  }
  setup_constraints(theVars);
  if (continuingRun) {
    constraints = prev_constraints;
  }
  theVars->inputsFinalized = true;
  theVars->inputsUpdated.clear();
  if (continuingRun)
    _dumpStep = dumpStep;
}

void Driver::teardown(const realtype& t, const N_Vector& y,
                      Variables* theVars) {
  if (!theVars) theVars = currentVariables();
  intermediateRes = N_VGetArrayPointer(y);
  for (size_t i = 0; i < constraints.size(); i++)
    constraints[i] = intermediateRes[i];
  _lastStep = true;
  _dumpStep = true;
  time = t;
  getOutputVars(theVars);
  MB(t, y);
  getResults(theVars);
  
  if (theVars != inputVars)
    inputVars->deepcopy(theVars);
  IniModelCom(inputVars); // Reset connections
}

arr Driver::continue_run(const double endTime) {
    this->start = this->endtime;
    this->endtime = endTime;
    step = initialStep;
    _lastStep = false;
    _clear_cvode_mem();
    return run(true);
}

arr Driver::run(const bool continuingRun) {

    int error = CV_SUCCESS;
    uint count = 0;
    uint max_count = 10;
    realtype t0 = start;
    realtype t = start;
    realtype tout = start + step;
    realtype hlast = 0;
    _firstPass = true;
    _dumpedStepFiles.clear();

    while (count < max_count) {
        maxStep = 20. * step;

        _cleanup_dumped_files(); 
        setup(nullptr, continuingRun);
        t = t0;
        tout = t + step;

        sunindextype N =  static_cast<long>(constraints.size());
        N_Vector y;
#ifdef SUNDIALS_CONTEXT_REQUIRED
        y = N_VNew_Serial(N, context());
#else // SUNDIALS_CONTEXT_REQUIRED
        y = N_VNew_Serial(N);
#endif // SUNDIALS_CONTEXT_REQUIRED
	sunrealtype* y_ptr = N_VGetArrayPointer(y);

        for (std::size_t i = 0; i < constraints.size(); i++)
            y_ptr[i] = constraints[i];

        CVodeMem *cmem = nullptr;
        try {
            cmem = &CVodeMem::create();
            cmem->cvode_mem_init(this, t0, y);
        } catch(...) {
            std::exception_ptr eptr = std::current_exception();
            N_VDestroy(y);
            std::rethrow_exception(eptr);
        }

        data->drv = this;

#ifdef SUNDIALS_CONTEXT_REQUIRED
        SUNMatrix A = SUNDenseMatrix(N, N, context());
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y, context());
        SUNLinearSolver LS = SUNLinSol_Dense(y, A, context());
#else // SUNDIALS_CONTEXT_REQUIRED
        SUNMatrix A = SUNDenseMatrix(N, N);
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y);
        SUNLinearSolver LS = SUNLinSol_Dense(y, A);
#endif // SUNDIALS_CONTEXT_REQUIRED

        try {
            if (CVodeSetNonlinearSolver(cvode_mem, NLS) != CV_SUCCESS) {
                std::cout << "CVodeSetNonlinearSolver failed" << std::endl;
                throw std::runtime_error("CVodeSetNonlinearSolver failed");
            }
            if (CVodeSetLinearSolver(cvode_mem, LS, A) != CV_SUCCESS) {
                std::cout << "CVodeSetLinearSolver failed" << std::endl;
                throw std::runtime_error("CVodeSetLinearSolver failed");
            }
        } catch(...) {
            std::exception_ptr eptr = std::current_exception();
            SUNNonlinSolFree(NLS);
            SUNLinSolFree(LS);
            SUNMatDestroy(A);
            N_VDestroy(y);
            std::rethrow_exception(eptr);
        }

        bool runOK = true;
        while (t < endtime) {
            if (tout > endtime)
              tout = endtime;
            error = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
            CVodeGetLastStep(cvode_mem, &hlast);
            if (error != CV_SUCCESS) {
                std::cout << "CVode failed to solve for t=" << tout <<
                  " (at t=" << t << " from t0=" << t0 <<
                  " with step=" << step << " and " <<
                  "internal_step=" << hlast << ")" <<
                  std::endl;
                runOK = false;
                break;
            }
            tout += step;
            if (tout > endtime)
                tout = endtime;
        }
        if (runOK) {
            teardown(t, y);
        }

        SUNNonlinSolFree(NLS);
        SUNLinSolFree(LS);
        SUNMatDestroy(A);
        N_VDestroy(y);
        intermediateRes = nullptr;
        if (currentVars) {
            delete currentVars;
            currentVars = nullptr;
        }
        if (runOK)
            return results;

        count++;
        step = initialStep / pow(2, count + 1);
        _firstPass = false;
        std::cout << "[t=" << t << "] Retrying with smaller step size: " << step <<
          " (INTERNAL STEP = " << hlast << ")" << std::endl;
    }
    throw std::runtime_error("No valid solution found");
}

void Driver::outputParam(const std::string& fname_init,
			 const std::string& fname_last,
                         const std::string& fname_step,
                         const std::vector<std::string>& vars) {
    fname_vars_init = fname_init;
    fname_vars_last = fname_last;
    fname_vars_step = fname_step;
    param_vars = vars;
}
void Driver::outputParam(const OutputFreq& frequency,
                         const std::string& fname_base,
                         const std::vector<std::string>& vars) {
    std::string fname_base_cpy = fname_base;
    if (fname_base_cpy.empty()) {
	fname_base_cpy = "NOMODULE_";
    }
    std::string finit, flast, fstep;
    if (frequency >= OUTPUT_FREQ_FIRST)
        finit = fname_base_cpy + "init.txt";
    if (frequency >= OUTPUT_FREQ_FIRST_AND_LAST)
        flast = fname_base_cpy + "last.txt";
    if (frequency >= OUTPUT_FREQ_STEPS)
        fstep = fname_base_cpy + "step";
    outputParam(finit, flast, fstep, vars);
}

void Driver::getOutputVars(const Variables* theVars0,
                           const ValueSet_t* con) {
    INITIALIZE_VARS_AND_CONDITIONS;
    output.clear();
    for (typename std::vector<std::string>::const_iterator it = outputVars.begin();
	 it != outputVars.end(); it++) {
        output[*it] = getVar(*it, theVars, conditions);
    }
    FINALIZE_VARS_AND_CONDITIONS;
}

void Driver::setOutputVars(const std::vector<std::string>& newVars) {
  outputVars.clear();
  outputVars = newVars;
}

void Driver::writeOutputTable(std::ostream& s) const {
    typename std::vector<std::string>::const_iterator it = outputVars.begin();
    if (output.size() > 1) {
	s << *it;
	it++;
	for (; it != outputVars.end(); it++)
	    s << "," << *it;
	s << std::endl;
	it = outputVars.begin();
    }
    s << output.find(*it)->second;
    it++;
    for (; it != outputVars.end(); it++)
	s << "," << output.find(*it)->second;
    s << std::endl;
}

double Driver::getVar(const std::string& k, const Variables* theVars0,
                      const ValueSet_t* con) const {
    INITIALIZE_VARS_AND_CONDITIONS;
    double out = theVars->getVar(k, false, conditions);
    FINALIZE_VARS_AND_CONDITIONS;
    return out;
}
double Driver::getVar(const std::string& k, const Variables* theVars,
                      const std::map<MODULE, const ValueSet_t*>& conditions) const {
    return theVars->getVar(k, false, conditions);
}

void Driver::dump(const std::string& filename, const Variables* theVars0,
		  const ValueSet_t* con, const bool is_init) {
    INITIALIZE_VARS_AND_CONDITIONS;
    bool skipCalculated = false;
#ifdef MAKE_EQUIVALENT_TO_MATLAB
    skipCalculated = true;  // Not output by MATLAB
    // Matlab uses different names for the pool variables
    static std::map<std::string, std::string> key_aliases = {};
    static std::vector<std::string> skip_keys = {
        // Not used by either model
	"ALL::VARS::GLight",
	// Not named variables in Matlab
	"BF::RC::Em_IPS",
	"BF::RC::Em_Cytf",
	"BF::RC::Em_PG",
	"PS::MOD::KE1Ratio",
	"PS::MOD::KE2Ratio",
	"SUCS::MOD::KE5Ratio",
        // Add for explicit definition of Zhu 2012 parameters
        "FIBF::RC::RC0",
        "FIBF::RC::RC",
        "BF::MOD::F",
        "PS::MOD::F",
        "FIBF::RC::kdm0",
        "RuACT::RC::factor_n7",
        "RuACT::RC::kn7",
        "RuACT::RC::RCA"
    };
    // Only used if useC3 false which is incompatible with C3 matlab
    if (theVars->useC3) {
	skip_keys.push_back("PS::MOD::KE57");
	skip_keys.push_back("PS::COND::ADPG");
	skip_keys.push_back("SUCS::MOD::KI583");
	skip_keys.push_back("SUCS::MOD::SC");
	skip_keys.push_back("SUCS::MOD::SC1");
    }
#else // MAKE_EQUIVALENT_TO_MATLAB
    static std::map<std::string, std::string> key_aliases = {};
    static std::vector<std::string> skip_keys = {};
#endif // MAKE_EQUIVALENT_TO_MATLAB
    std::vector<PARAM_TYPE> skip_param_types;
    if (is_init)
	skip_param_types.push_back(PARAM_TYPE_VEL);
    theVars->dump(filename, true, {}, skip_param_types,
                  skip_keys, key_aliases, conditions, param_vars,
                  {}, skipCalculated);
    FINALIZE_VARS_AND_CONDITIONS;
}

Driver::~Driver() {
    if (currentVars != nullptr)
        delete currentVars;
    _clear_cvode_mem();
#ifdef SUNDIALS_CONTEXT_REQUIRED
    if (_context.use_count() == 1)
        SUNContext_Free(_context.get());
    _context.reset();
#endif // SUNDIALS_CONTEXT_REQUIRED
}

void Driver::_clear_cvode_mem() {
    CVodeMem *cmem = nullptr;
    cmem = &CVodeMem::create();
    cmem->cvode_mem_free();
    cvode_mem = nullptr;
    data = nullptr;
}

void Driver::_dump(realtype t, ValueSet_t* con) {
    if (!_dumpStep) return;
    if (t == 0 && !fname_vars_init.empty()) {
      dump(fname_vars_init, nullptr, con, true);
    } else if (_lastStep && !fname_vars_last.empty()) {
      dump(fname_vars_last, nullptr, con, false);
    }
    if (!fname_vars_step.empty()) {
      if (t >= endtime) return;
      if (_lastDumpTime > 0 && _dumpInterval > 0 &&
          ((t - _lastDumpTime) < _dumpInterval))
        return;
      _lastDumpTime = t;
      std::ostringstream tss;
      tss.precision(8);
      tss << std::fixed << t;
      std::string tstr = tss.str();
      tstr = std::string(15 - tstr.length(), '0') + tstr;
      std::string ifile = fname_vars_step + tstr + ".txt";
      dump(ifile, nullptr, con, false);
      _dumpedStepFiles.push_back(ifile);
    }
    if (fname_vars_step.empty())
        _dumpStep = false;
}

void Driver::_cleanup_dumped_files() {

    for (typename std::vector<std::string>::const_iterator it = _dumpedStepFiles.begin();
         it != _dumpedStepFiles.end(); it++) {
      remove(it->c_str());
    }
    _dumpedStepFiles.clear();
}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    CalcData *data = static_cast<CalcData*>(user_data);
    arr ddxdt = data->drv->MB(t, u);
    for (std::size_t index = 0; index < ddxdt.size(); index++)
        dxdt[index] = ddxdt[index];
    return 0;
}

const std::map<std::string, double>& Driver::getOutput() const {
    return output;
}

#undef INITIALIZE_VARS_AND_CONDITIONS
#undef FINALIZE_VARS_AND_CONDITIONS
