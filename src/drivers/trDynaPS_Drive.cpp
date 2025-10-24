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
#include "drivers/trDynaPS_Driver.hpp"
#include "modules/trDynaPS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

Variables* Driver::inputVars = nullptr;

DEFINE_DRIVER(trDynaPS);

trDynaPSDriver::trDynaPSDriver(Variables *theVars, const double startTime,
			       const double stepSize, const double endTime,
			       const int maxSubsteps,
			       const double atol, const double rtol,
			       const std::size_t para,
			       const double ratio, const bool showWarn,
			       const std::vector<std::string>& outVars) :
    DriverBase(theVars, startTime, stepSize, endTime, maxSubsteps,
	       atol, rtol, para, ratio, showWarn, outVars) {
    init(theVars->useC3);
    if (outputVars.empty()) {
      outputVars.push_back("Light intensity");
      outputVars.push_back("Vc");
      outputVars.push_back("Vo");
      outputVars.push_back("VPGA");
      outputVars.push_back("VT3P");
      outputVars.push_back("Vstarch");
      outputVars.push_back("Vt_glycerate");
      outputVars.push_back("Vt_glycolate");
      outputVars.push_back("CO2AR");
    }
}
void trDynaPSDriver::setup_connections(Variables* theVars) {
    // The combination of BF and FI model
    theVars->BF_FI_com = true;

    // true means that the overall EPS model is used. false means partial model of FIBF is used.
    theVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    theVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    // langmm: I believe this should be true, but the RedoxReg module
    //   solver problem does not appear to be well solved and is always
    //   solved by the initial guess.
    theVars->RedoxReg_RA_com = false;        // This means that the connection is there.

    theVars->XanCycle_BF_com = true;
  
    theVars->RROEA_EPS_com = true;

    theVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

}
void trDynaPSDriver::setup_variables(Variables* theVars) {

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

void trDynaPSDriver::setup_param(Variables* theVars) {

    ParamSet(theVars);

}

void trDynaPSDriver::getResults(Variables* theVars) {

    if (!theVars) theVars = currentVariables();

    double CarbonRate = theVars->RuACT_Vel.v6_1 * theVars->AVR;
    double VPR = theVars->RuACT_Vel.v6_2 * theVars->AVR;
    double Vpgasink = theVars->SUCS_Vel.vpga_use * theVars->AVR;
    double VStarch = (theVars->PS_Vel.v23 - theVars->PS_Vel.v25) * theVars->AVR;
    double Vt3p = (theVars->PS_Vel.v31 + theVars->PS_Vel.v33) * theVars->AVR;
    const double CO2AR = TargetFunVal(theVars);
    results = zeros(8);
    results[0] = CarbonRate; //Vc
    results[1] = VPR;        //Vo
    results[2] = Vpgasink;   //PGA
    results[3] = Vt3p;       //VT3P
    results[4] = VStarch;    //Vstarch
    results[5] = theVars->PR_Vel.v1in * theVars->AVR;  //Vt_glycerate
    results[6] = theVars->PR_Vel.v2out * theVars->AVR; //Vt_glycolate
    results[7] = CO2AR;

    if(theVars->record) {
        makeFluxTR(theVars);
    }

}
