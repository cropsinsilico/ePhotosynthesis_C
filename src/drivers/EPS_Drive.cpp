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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

DEFINE_DRIVER(EPS);

EPSDriver::EPSDriver(Variables *theVars, const double startTime,
		     const double stepSize, const double endTime,
		     const int maxSubsteps,
		     const double atol, const double rtol,
		     const std::size_t para, const double ratio,
		     const bool showWarn,
		     const std::vector<std::string>& outVars) :
    DriverBase(theVars, startTime, stepSize, endTime, maxSubsteps,
	       atol, rtol, para, ratio, showWarn, outVars) {
#ifdef INCDEBUG
    ePhotosynthesis::conditions::EPSCondition::setTop();
#endif
    init(theVars->useC3);
    if (outputVars.empty()) {
      outputVars.push_back("CO2AR");
    }
}
void EPSDriver::setup() {
    //Ca = theVars->TestCa;
    inputVars->TestLi /= 30.;
    //Li = theVars->TestLi;
    AtpCost = inputVars->TestATPCost;
    IniModelCom(inputVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    SYSInitial(inputVars);
    //time = tglobal;
    inputVars->alfa = 0.85;
    PS::setJmax(inputVars->EnzymeAct.at("Jmax"));
    inputVars->fc = 0.15;
    PS::setTheta(0.7);
    PS::setbeta(0.7519);
    inputVars->BF_FI_com = true;
    inputVars->EnzymeAct.at("V1") *= inputVars->alpha1;
    inputVars->EnzymeAct.at("V2") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V3") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V5") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V6") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V7") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V8") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V9") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V10") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V13") *= inputVars->alpha2;
    inputVars->EnzymeAct.at("V23") *= inputVars->alpha2;

    inputVars->PR_PS_com = true;     // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.


    inputVars->FIBF_PSPR_com = true;     //true means that the overall EPS model is used. false means partial model is used.

    //theVars->ATPActive = 0;

    inputVars->EPS_SUCS_com = true;

    inputVars->PSPR_SUCS_com = true;   // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.

    /////////////////////////
    //   Calculation  step //
    /////////////////////////
    EPSCondition* EPS_Con = EPS_Ini();

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
    results = zeros(1);
    const double Arate = TargetFunVal(inputVars);
    delete eps_int_con;
    results[0] = Arate;
}
