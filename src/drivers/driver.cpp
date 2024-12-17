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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

bool Driver::showWarnings = false;

Driver::Driver(Variables *theVars, const double startTime, const double stepSize, const double endTime,
	       const int maxSubsteps, const double atol, const double rtol,
	       const bool showWarn) {
#ifdef SUNDIALS_CONTEXT_REQUIRED
  _context = theVars->context_ptr();
#endif // SUNDIALS_CONTEXT_REQUIRED
  inputVars = theVars;
  start = startTime;
  step = stepSize;
  initialStep = stepSize;
  endtime = endTime;
  maxSubSteps = maxSubsteps;
  showWarnings = showWarn;
  abstol = atol;
  reltol = rtol;
  maxStep = 20. * step;
  data = nullptr;
  origVars = nullptr;
  intermediateRes = nullptr;
}

arr Driver::run() {
    origVars = new Variables(inputVars);
    uint count = 0;

    while (count < 10){
        maxStep = 20. * step;

        setup();

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
        realtype t0 = start;

        CVodeMem *cmem;
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

        realtype t = 0;
        bool runOK = true;
        realtype tout = start + step;
        while (t <= endtime) {
            if (CVode(cvode_mem, tout, y, &t, CV_NORMAL) != CV_SUCCESS) {
                std::cout << "CVode failed at t=" << tout << "  " << t << std::endl;
                runOK = false;
                break;
            }
            tout += step;
        }
        if (runOK) {
            intermediateRes = N_VGetArrayPointer(y);
            time = t;
            getResults();
        }

        SUNNonlinSolFree(NLS);
        SUNLinSolFree(LS);
        SUNMatDestroy(A);
        N_VDestroy(y);
        if (runOK)
            return results;

        inputVars = origVars;

        count++;
        step = initialStep / (count + 1);
        std::cout << "Retrying with smaller step size: " << step << std::endl;
    }
    throw std::runtime_error("No valid solution found");
}

Driver::~Driver() {
    if (origVars != nullptr)
        delete origVars;
}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    CalcData *data = static_cast<CalcData*>(user_data);
    arr ddxdt = data->drv->MB(t, u);
    for (std::size_t index = 0; index < ddxdt.size(); index++)
        dxdt[index] = ddxdt[index];
    return 0;
}
