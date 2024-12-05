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
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

DynaPSDriver::~DynaPSDriver() {
    modules::DynaPS::_reset();
}

void DynaPSDriver::setup() {
    // This part include the function to begin the simulation.

    // The time information is set in a global variable called tglobal in SYSInitial.
    if (ParaNum <= 103) {
        inputVars->PSRatio[ParaNum] = Ratio;
    } else if (ParaNum <= 169) {
        inputVars->SUCRatio[ParaNum - 103] = Ratio;
    } else if (ParaNum <= 217) {
        inputVars->PRRatio[ParaNum - 169] = Ratio;
    } else if (ParaNum <= 233) {
        inputVars->RacRatio[ParaNum - 217] = Ratio;
    } else if (ParaNum <= 256) {
        inputVars->FIRatio[ParaNum - 233] = Ratio;
    } else if (ParaNum <= 305) {
        inputVars->BFRatio[ParaNum - 256] = Ratio;
    } else if (ParaNum <= 309) {
        inputVars->XanRatio[ParaNum - 305] = Ratio;
    }

    // This part include the function to begin the simulation.
    // The time information is set in a global variable called tglobal in SYSInitial.

    SYSInitial(inputVars);

    ////////////////////////////////////////////////
    //   Calculation  step //
    ////////////////////////////////////////////////

    IniModelCom(inputVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    inputVars->BF_FI_com = true;

    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    inputVars->PR_PS_com = true;

    // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    inputVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    // langmm: This is true in the MATLAB code, but was false in the
    //   original translation to C++
    inputVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    inputVars->RedoxReg_RA_com = false;        // This means that the connection is not provided there.

    inputVars->XanCycle_BF_com = true;

    inputVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    inputVars->PSPR_SUCS_com = true;

    inputVars->SUCS_Param = zeros(2);

    // Next is to initialize the vector.

    DynaPSCondition* DynaPS_con = DynaPS_Ini();

    const double va1 = 0;
    // The ratio of the PSI unit to the PSII unit
    inputVars->BF_Param[0] = va1;
    inputVars->BF_Param[1] = inputVars->PS12ratio;

    inputVars->FI_Param[0] = va1;
    inputVars->FI_Param[1] = inputVars->PS12ratio;

    inputVars->PS_PR_Param = 0.;

    inputVars->RuACT_Param[0] = va1;
    inputVars->RuACT_Param[1] = inputVars->PS12ratio;

    inputVars->XanCycle_Param[0] = va1;
    inputVars->XanCycle_Param[1] = inputVars->PS12ratio;

    constraints = DynaPS_con->toArray();
    delete DynaPS_con;
}

void DynaPSDriver::getResults() {

    DynaPSCondition* dyna_int_con = new DynaPSCondition(intermediateRes);

    arr temp = DynaPS::MB(time, dyna_int_con, inputVars);

    double PSIIabs = inputVars->FI_Vel.vP680_d;
    double PSIabs = inputVars->BF_Vel.Vbf11;
    double CarbonRate = inputVars->RuACT_Vel.v6_1 * inputVars->AVR;
    double VPR = inputVars->RuACT_Vel.v6_2 * inputVars->AVR;
    double Vpgasink = inputVars->SUCS_Vel.vpga_use * inputVars->AVR;
    double VStarch = (inputVars->PS_Vel.v23 - inputVars->PS_Vel.v25) * inputVars->AVR;
    double Vsucrose = inputVars->SUCS_Vel.vdhap_in * inputVars->AVR;

    results = zeros(7);

    results[0] = PSIIabs;
    results[1] = PSIabs;
    //Resulta(3)=PSIabs2(row);
    results[2] = CarbonRate;
    results[3] = VPR;
    results[4] = Vpgasink;
    results[5] = Vsucrose;
    results[6] = VStarch;

    if(inputVars->record) {
        makeFluxTR(inputVars);
    }
    // This is to set the regualtions to be as beginning.
    //theVars->ATPActive = 0;
    inputVars->BF_FI_com = false;
    inputVars->PR_PS_com = false;
    inputVars->FIBF_PSPR_com = false;
    inputVars->RuACT_EPS_com = false;
    inputVars->RedoxReg_RA_com = false;
    inputVars->XanCycle_BF_com = false;

    delete dyna_int_con;
    IniModelCom(inputVars);
    //save FDC2
}

DynaPSCondition* DynaPSDriver::DynaPS_Ini() {
    return DynaPS::init(inputVars);
}

// DynaPS_mb.m  This model includes the mass balance equations for the full model of photosynthesis.

arr DynaPSDriver::MB(realtype t, N_Vector u) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    // Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.
    realtype *x = N_VGetArrayPointer(u);

    DynaPSCondition* DynaPS_con = new DynaPSCondition(x);

    arr dxdt = DynaPS::MB(t, DynaPS_con, inputVars);

    delete DynaPS_con;
    return dxdt;
}
