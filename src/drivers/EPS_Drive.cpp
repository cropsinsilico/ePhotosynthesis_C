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

#include "Variables.hpp"
#include "globals.hpp"
#include "drivers/EPS_Driver.hpp"
#include "modules/EPS.hpp"
#include "modules/PS.hpp"
#include "modules/SUCS.hpp"
#include <cvode/cvode.h>
#include <algorithm>
#include <cmath>
#include <type_traits>

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

EPSDriver::~EPSDriver() {
    modules::EPS::_reset();
}

void EPSDriver::setup() {
    //the conversion factor comes from Zhu et al (2007). They assume
    //1 g chlorophyll/m²,30 μL stroma/mg chlorophyll =  30 mL stroma/m², or 0.03 L
    inputVars->TestLi /= 30.;
    //Li = theVars->TestLi;
    AtpCost = inputVars->TestATPCost;
    IniModelCom(inputVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    SYSInitial(inputVars);
    //time = tglobal;
    inputVars->Tp = this->Tp;
    inputVars->Phi_max = 0.69;//similar to PhiPSII in BIoCro, but this represents the maximum light efficiency for both PSI and PSII. So it can be adjusted/optimized as needed.
    PS::setJmax(inputVars->EnzymeAct.at("Jmax"));
    inputVars->fc = 0.15; // equals leaf_reflectance + leaf_transmittance in BioCro

    double Tp = inputVars->Tp;
    double Theta_base = 0.76; //0.76 is the default of Theta used in BioCro 
    double Theta = Theta_base + 0.01713 * Tp - 3.75 * pow(Tp,2.0) / 10000.0;//Yufeng: match Farquhar Matlab
    PS::setTheta(Theta);
    PS::setbeta(0.7519);//Used when coupled with FIBF. ATP production is replaced by BF's ATP
    inputVars->BF_FI_com = true;
    //inputVars->EnzymeAct.at("V1") *= inputVars->alpha1;
    //inputVars->EnzymeAct.at("V2") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V3") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V5") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V6") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V7") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V8") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V9") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V10") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V13") *= inputVars->alpha2;
    //inputVars->EnzymeAct.at("V23") *= inputVars->alpha2;

    //we scale up some enzymes 
    inputVars->EnzymeAct.at("V2") *= inputVars->sensitivity_sf;
    inputVars->EnzymeAct.at("V13") *= inputVars->sensitivity_sf;
    //

    inputVars->PR_PS_com = true;     // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.


    inputVars->FIBF_PSPR_com = true;     //true means that the overall EPS model is used. false means partial model is used.

    //theVars->ATPActive = 0;

    inputVars->EPS_SUCS_com = true;

    inputVars->PSPR_SUCS_com = true;   // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.

    /////////////////////////
    //   Calculation  step //
    /////////////////////////
    EPSCondition* EPS_Con = EPS_Init();

    int va1 = 0;
    inputVars->BF_Param[0] = va1;
    inputVars->BF_Param[1] = inputVars->PS12ratio;

    inputVars->FI_Param[0] = va1;
    inputVars->FI_Param[1] = inputVars->PS12ratio;

    inputVars->PS_PR_Param = 0;
///////theVars->Sucs_Param = 0;
    // Get the initial variables for SUCS.



    //////////////////////////////////////////////////////////////////////////
    //                      END                                              /
    //////////////////////////////////////////////////////////////////////////

    // This is a variable to indicate whether BF is run by itself or together.


    constraints = EPS_Con->toArray();
    delete EPS_Con;
}

void EPSDriver::getResults() {
    EPSCondition* eps_int_con = new EPSCondition(intermediateRes);
    arr temp = EPS::MB(time, eps_int_con, inputVars);
    results = zeros(4);
    const double Arate = (inputVars->PS_Vel.v1 - inputVars->PR_Vel.v131) * inputVars->AVR;
    delete eps_int_con;

//    ofstream myfile;
//    myfile.open ("hello.txt");
//    myfile << "Writing this to a file.\n";
//    myfile.close();
    results[0] = Arate;
    results[1] = inputVars->PS_Vel.v1 * inputVars->AVR; //carboxylation
    results[2] = inputVars->PR_Vel.v131 * inputVars->AVR; //photorespiration 
}

EPSCondition* EPSDriver::EPS_Init() {
    return EPS::init(inputVars);
}

arr EPSDriver::MB(realtype t, N_Vector u) {
    // Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.
    realtype *x = N_VGetArrayPointer(u);

    EPSCondition* EPS_con = new EPSCondition(x);

    arr dxdt = EPS::MB(t, EPS_con, inputVars);
    delete EPS_con;

    return dxdt;
}

// EPS uses populations down to approximately 1e-12 alongside millimolar pools.
// Respect stricter caller tolerances, but cap the relative error and set
// separate absolute error limits for pools and low-abundance excitation states.
void EPSDriver::configureSolver(N_Vector initial) {
    nonnegativeMask.clear();
    solverAbsoluteTolerances.clear();
    stateNames.clear();
    if (!inputVars->useC3) return;
    if (abstol <= 0.0) throw std::invalid_argument("C3 EPS protection requires a positive absolute tolerance");

    EPSCondition state(N_VGetArrayPointer(initial));
    const std::size_t n = static_cast<std::size_t>(NV_LENGTH_S(initial));
    const std::size_t bfSize = state.FIBF_con->BF_con->size();
    const std::size_t fiSize = state.FIBF_con->FI_con->size();
    psOffset = state.FIBF_con->size();
    prOffset = psOffset + state.CM_con->PS_PR_con->PS_con->size();
    sucsOffset = psOffset + state.CM_con->PS_PR_con->size();
    nadphIndex = bfSize - 1;
    if (prOffset - psOffset != 11 || sucsOffset - prOffset != 9 || n - sucsOffset != 8) {
        throw std::runtime_error("unsupported C3 EPS state layout for solver protection");
    }
    nonnegativeMask.assign(n, 0.0);
    solverAbsoluteTolerances.assign(n, std::min<double>(abstol, 1e-8));
    stateNames.assign(n, "negative excitation population");
    // Protect the actual PS/PR/SUCS concentrations. Do not blanket-constrain
    // light-reaction coordinates or BF ADP bookkeeping states: their startup
    // trial behavior differs from the connected carbon/metabolite pools.
    for (std::size_t i = psOffset; i < n; ++i) nonnegativeMask[i] = 1.0;
    nonnegativeMask[nadphIndex] = 1.0;
    stateNames[nadphIndex] = "negative NADPH";
    const char* psNames[] = {"RuBP", "PGA", "DPGA", "T3P", "FBP", "E4P", "S7P", "SBP", "ATP", "HexP", "PenP"};
    const char* prNames[] = {"GCEA", "GCA", "PGCA", "GCAc", "GOAc", "SERc", "GLYc", "HPRc", "GCEAc"};
    const char* sucsNames[] = {"T3Pc", "FBPc", "HexPc", "F26BPc", "UDPGc", "SUCP", "SUC", "PGAc"};
    for (std::size_t i = 0; i < 11; ++i) stateNames[psOffset + i] = std::string("negative PS.") + psNames[i];
    for (std::size_t i = 0; i < 9; ++i) stateNames[prOffset + i] = std::string("negative PR.") + prNames[i];
    for (std::size_t i = 0; i < 8; ++i) stateNames[sucsOffset + i] = std::string("negative SUCS.") + sucsNames[i];
    for (std::size_t i = bfSize; i < bfSize + std::min<std::size_t>(fiSize, 6); ++i) {
        solverAbsoluteTolerances[i] = std::min<double>(abstol, 1e-12);
    }
    // The final BF fields are Aip,U,An,[Fdn],BFHs,BFHl,PHs,PHl,NADPH.
    const std::size_t bfTail = BFCondition::getRROEA_connect() ? 8 : 9;
    for (std::size_t i = bfSize - bfTail; i < bfSize - bfTail + 2; ++i) {
        solverAbsoluteTolerances[i] = std::min<double>(abstol, 1e-12);
        nonnegativeMask[i] = 1.0;
        stateNames[i] = "negative BF excitation population";
    }

    using OwnedVector = std::unique_ptr<std::remove_pointer<N_Vector>::type, decltype(&N_VDestroy)>;
    OwnedVector bounds(N_VClone(initial), N_VDestroy);
    OwnedVector tolerance(N_VClone(initial), N_VDestroy);
    if (!bounds || !tolerance) throw std::runtime_error("solver protection vector allocation failed");
    std::copy(nonnegativeMask.begin(), nonnegativeMask.end(), N_VGetArrayPointer(bounds.get()));
    std::copy(solverAbsoluteTolerances.begin(), solverAbsoluteTolerances.end(), N_VGetArrayPointer(tolerance.get()));
    if (CVodeSetConstraints(cvode_mem, bounds.get()) != CV_SUCCESS) {
        throw std::runtime_error("CVodeSetConstraints failed");
    }
    if (CVodeSVtolerances(cvode_mem, std::min<double>(reltol, 1e-7), tolerance.get()) != CV_SUCCESS) {
        throw std::runtime_error("CVodeSVtolerances failed");
    }
    // CVODE copies both vectors; their temporary storage can be released here.
}

const char* EPSDriver::stateError(N_Vector state) const {
    if (const char* error = Driver::stateError(state)) return error;
    if (nonnegativeMask.empty()) return nullptr;
    if (static_cast<std::size_t>(NV_LENGTH_S(state)) != nonnegativeMask.size()) return "EPS state size mismatch";
    const auto y = N_VGetArrayPointer(state);
    for (std::size_t i = 0; i < nonnegativeMask.size(); ++i) {
        // Allow only a small error-scale margin for Newton/Jacobian trials and
        // interpolation. The solver's nonnegativity constraints remain active.
        if (nonnegativeMask[i] && y[i] < -10.0 * solverAbsoluteTolerances[i]) return stateNames[i].c_str();
    }
    const double margin = 10.0 * std::min<double>(abstol, 1e-8);
    const double adp = PS::getPS_C_CA() - y[psOffset + 8];
    if (adp < -margin) return "ATP exceeds adenylate pool";
    // KE1Ratio and KE2Ratio partition PenP/HexP but preserve their total
    // phosphate. This is the same phosphate balance as PS_Rate, without
    // unnecessary subpool divisions and cancellation.
    const double weights[] = {2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1};
    double pi = PS::getPS_C_CP() - y[prOffset + 2];
    for (std::size_t i = 0; i < 11; ++i) pi -= weights[i] * y[psOffset + i];
    if (pi < -margin || pi > PS::getPS_C_CP() + margin) return "stromal phosphate outside pool";
    const double pic = inputVars->SUCS_Pool.PTc - 2 * (y[sucsOffset + 1] + y[sucsOffset + 3])
        - (y[sucsOffset + 7] + y[sucsOffset] + y[sucsOffset + 2] + y[sucsOffset + 5]
           + SUCS::getUTPc() + SUCS::getATPc());
    if (pic < -margin || pic > inputVars->SUCS_Pool.PTc + margin) return "cytosolic phosphate outside pool";
    if (y[nadphIndex] > inputVars->BF_Pool.k30 + margin) return "NADPH exceeds NADP pool";
    return nullptr;
}

void EPSDriver::writeState(std::ostream& out, N_Vector state) const {
    if (!nonnegativeMask.empty() && static_cast<std::size_t>(NV_LENGTH_S(state)) == nonnegativeMask.size()) {
        EPSCondition condition(N_VGetArrayPointer(state));
        out << "total_phosphate=" << PS::getPS_C_CP()
            << " total_adenylate=" << PS::getPS_C_CA() << '\n' << condition;
    } else {
        Driver::writeState(out, state);
    }
}
