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
    Li = theVars->TestLi;
    theVars->useC3 = true;
    init(theVars->useC3);
    if (outputVars.empty()) {
      outputVars.push_back("CO2AR");
    }
    if (inputVars->useC3 && inputVars->EnzymeAct.empty())
        throw std::runtime_error("EnzymeAct must be set if useC3 is True (automatically set for EPS driver)");
}
void EPSDriver::setup_connections(Variables* theVars) {
  
    theVars->BF_FI_com = true;

    theVars->PR_PS_com = true;     // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.

  
    theVars->FIBF_PSPR_com = true;     //true means that the overall EPS model is used. false means partial model is used.

    theVars->EPS_SUCS_com = true;

    theVars->PSPR_SUCS_com = true;   // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
}
void EPSDriver::setup_variables(Variables* theVars) {

    //Ca = theVars->TestCa;
    if (theVars->inputUpdated("ALL::VARS::TestLi")) {
        Li = theVars->TestLi;
    }
    theVars->TestLi = Li / 30.;
    AtpCost = theVars->TestATPCost;

    SYSInitial(theVars);
    //time = tglobal;
    theVars->alfa = 0.85;
    PS::setJmax(theVars->EnzymeAct.at("Jmax"));
    theVars->fc = 0.15;
    PS::setTheta(0.7);
    PS::setbeta(0.7519);
    // theVars->EnzymeAct.at("V1") *= theVars->alpha1;
    // theVars->EnzymeAct.at("V2") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V3") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V5") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V6") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V7") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V8") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V9") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V10") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V13") *= theVars->alpha2;
    // theVars->EnzymeAct.at("V23") *= theVars->alpha2;
    
    //theVars->ATPActive = 0;

}

void EPSDriver::setup_param(Variables* theVars) {

    int va1 = 0;
    theVars->BF_Param[0] = va1;
    theVars->BF_Param[1] = theVars->PS12ratio;

    theVars->FI_Param[0] = va1;
    theVars->FI_Param[1] = theVars->PS12ratio;

    theVars->PS_PR_Param = 0;
///////theVars->Sucs_Param = 0;

}

void EPSDriver::getResults(Variables* theVars) {

    if (!theVars) theVars = currentVariables();

    results = zeros(1);
    const double Arate = TargetFunVal(theVars);
    results[0] = Arate;

}
