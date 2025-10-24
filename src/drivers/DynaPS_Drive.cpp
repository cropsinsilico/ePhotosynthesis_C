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
#include "modules/DynaPS.hpp"
#include "drivers/DynaPS_Driver.hpp"
#include "conditions/DynaPSCondition.hpp"
#include "globals.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DEFINE_DRIVER(DynaPS);

DynaPSDriver::DynaPSDriver(Variables *theVars, const double startTime,
			   const double stepSize, const double endTime,
			   const int maxSubsteps,
			   const double atol, const double rtol,
			   const std::size_t para,
			   const double ratio, const bool showWarn,
			   const std::vector<std::string>& outVars) :
    DriverBase(theVars, startTime, stepSize, endTime, maxSubsteps,
	       atol, rtol, para, ratio, showWarn, outVars) {
#ifdef INCDEBUG
    ePhotosynthesis::conditions::DynaPSCondition::setTop();
#endif
    init(theVars->useC3);
    if (outputVars.empty()) {
      outputVars.push_back("Light intensity");
      outputVars.push_back("PSIIabs");
      outputVars.push_back("PSIabs");
      outputVars.push_back("Vc");
      outputVars.push_back("Vo");
      outputVars.push_back("VPGA");
      outputVars.push_back("Vsucrose");
      outputVars.push_back("Vstarch");
      outputVars.push_back("CO2AR");
    }
}
void DynaPSDriver::setup_connections(Variables* theVars) {

    // The combination of BF and FI model
    theVars->BF_FI_com = true;

    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    theVars->PR_PS_com = true;

    // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    theVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    // langmm: This is true in the MATLAB code, but was false in the
    //   original translation to C++
    theVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    theVars->RedoxReg_RA_com = false;        // This means that the connection is not provided there.

    theVars->XanCycle_BF_com = true;

    theVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

}
void DynaPSDriver::setup_variables(Variables* theVars) {

    if (ParaNum <= 103) {
        theVars->PSRatio[ParaNum] = Ratio;
    } else if (ParaNum <= 169) {
        theVars->SUCSRatio[ParaNum - 103] = Ratio;
    } else if (ParaNum <= 217) {
        theVars->PRRatio[ParaNum - 169] = Ratio;
    } else if (ParaNum <= 233) {
        theVars->RuACTRatio[ParaNum - 217] = Ratio;
    } else if (ParaNum <= 256) {
        theVars->FIRatio[ParaNum - 233] = Ratio;
    } else if (ParaNum <= 305) {
        theVars->BFRatio[ParaNum - 256] = Ratio;
    } else if (ParaNum <= 309) {
        theVars->XanCycleRatio[ParaNum - 305] = Ratio;
    }

    theVars->alpha1 = 1.0;
    theVars->alpha2 = 1.0;

}

void DynaPSDriver::setup_param(Variables* theVars) {

    theVars->SUCS_Param = zeros(2);

    const double va1 = 0;
    // The ratio of the PSI unit to the PSII unit
    theVars->BF_Param[0] = va1;
    theVars->BF_Param[1] = theVars->PS12ratio;

    theVars->FI_Param[0] = va1;
    theVars->FI_Param[1] = theVars->PS12ratio;

    theVars->PS_PR_Param = 0.;

    theVars->RuACT_Param[0] = va1;
    theVars->RuACT_Param[1] = theVars->PS12ratio;

    theVars->XanCycle_Param[0] = va1;
    theVars->XanCycle_Param[1] = theVars->PS12ratio;

}

void DynaPSDriver::getResults(Variables* theVars) {

    if (!theVars) theVars = currentVariables();

    double PSIIabs = theVars->FI_Vel.vP680_d;
    double PSIabs = theVars->BF_Vel.Vbf11;
    double CarbonRate = theVars->RuACT_Vel.v6_1 * theVars->AVR;
    double VPR = theVars->RuACT_Vel.v6_2 * theVars->AVR;
    double Vpgasink = theVars->SUCS_Vel.vpga_use * theVars->AVR;
    double VStarch = (theVars->PS_Vel.v23 - theVars->PS_Vel.v25) * theVars->AVR;
    double Vsucrose = theVars->SUCS_Vel.vdhap_in * theVars->AVR;
    const double CO2AR = TargetFunVal(theVars);

    results = zeros(8);

    results[0] = PSIIabs;
    results[1] = PSIabs;
    //Resulta(3)=PSIabs2(row);
    results[2] = CarbonRate;
    results[3] = VPR;
    results[4] = Vpgasink;
    results[5] = Vsucrose;
    results[6] = VStarch;
    results[7] = CO2AR;

    if(theVars->record) {
        makeFluxTR(theVars);
    }
    
}

