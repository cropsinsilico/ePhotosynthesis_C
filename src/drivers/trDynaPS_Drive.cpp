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
void trDynaPSDriver::setup() {

    if (ParaNum <= 103) {
        inputVars->PSRatio[ParaNum] = Ratio;
    } else if (ParaNum <= 169) {
        inputVars->SUCSRatio[ParaNum - 103] = Ratio;
    } else if (ParaNum <= 217) {
        inputVars->PRRatio[ParaNum - 169] = Ratio;
    } else if (ParaNum <= 233) {
        inputVars->RuACTRatio[ParaNum - 217] = Ratio;
    } else if (ParaNum <= 256) {
        inputVars->FIRatio[ParaNum - 233] = Ratio;
    } else if (ParaNum <= 305) {
        inputVars->BFRatio[ParaNum - 256] = Ratio;
    } else if (ParaNum <= 309) {
        inputVars->XanCycleRatio[ParaNum - 305] = Ratio;
    }
    SYSInitial(inputVars);

    IniModelCom(inputVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    inputVars->BF_FI_com = true;

    // true means that the overall EPS model is used. false means partial model of FIBF is used.
    inputVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    inputVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    inputVars->RedoxReg_RA_com = false;        // This means that the connection is there.

    inputVars->XanCycle_BF_com = true;

    inputVars->RROEA_EPS_com = true;

    inputVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    inputVars->PSPR_SUCS_com = true;

    // Next is to initialize the vector.
    trDynaPSCondition* trDynaPS_con = trDynaPS_Ini();
    ParamSet(inputVars);
    constraints = trDynaPS_con->toArray();
    delete trDynaPS_con;

}

void trDynaPSDriver::getResults() {
    // call the functions one last time to get the correct values we need
    trDynaPSCondition* trDynaPS_res = new trDynaPSCondition(intermediateRes);
    arr temp = trDynaPS::MB(time, trDynaPS_res, inputVars);

    double CarbonRate = inputVars->RuACT_Vel.v6_1 * inputVars->AVR;
    double VPR = inputVars->RuACT_Vel.v6_2 * inputVars->AVR;
    double Vpgasink = inputVars->SUCS_Vel.vpga_use * inputVars->AVR;
    double VStarch = (inputVars->PS_Vel.v23 - inputVars->PS_Vel.v25) * inputVars->AVR;
    double Vt3p = (inputVars->PS_Vel.v31 + inputVars->PS_Vel.v33) * inputVars->AVR;
    const double CO2AR = TargetFunVal(inputVars);
    results = zeros(8);
    results[0] = CarbonRate; //Vc
    results[1] = VPR;        //Vo
    results[2] = Vpgasink;   //PGA
    results[3] = Vt3p;       //VT3P
    results[4] = VStarch;    //Vstarch
    results[5] = inputVars->PR_Vel.v1in * inputVars->AVR;  //Vt_glycerate
    results[6] = inputVars->PR_Vel.v2out * inputVars->AVR; //Vt_glycolate
    results[7] = CO2AR;

    if(inputVars->record) {
        makeFluxTR(inputVars);
    }
    delete trDynaPS_res;
    IniModelCom(inputVars);
}
