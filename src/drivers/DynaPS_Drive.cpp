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

DynaPSDriver::~DynaPSDriver() {}

void DynaPSDriver::setup() {
    // This part include the function to begin the simulation.

    // The time information is set in a global variable called tglobal in SYSInitial.
    if (ParaNum <= 103) {
        theVars->PSRatio[ParaNum] = Ratio;
    } else if (ParaNum <= 169) {
        theVars->SUCRatio[ParaNum - 103] = Ratio;
    } else if (ParaNum <= 217) {
        theVars->PRRatio[ParaNum - 169] = Ratio;
    } else if (ParaNum <= 233) {
        theVars->RacRatio[ParaNum - 217] = Ratio;
    } else if (ParaNum <= 256) {
        theVars->FIRatio[ParaNum - 233] = Ratio;
    } else if (ParaNum <= 305) {
        theVars->BFRatio[ParaNum - 256] = Ratio;
    } else if (ParaNum <= 309) {
        theVars->XanRatio[ParaNum - 305] = Ratio;
    }

    // This part include the function to begin the simulation.
    // The time information is set in a global variable called tglobal in SYSInitial.

    SYSInitial(theVars);

    ////////////////////////////////////////////////
    //   Calculation  step //
    ////////////////////////////////////////////////

    IniModelCom(theVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    theVars->BF_FI_com = true;

    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    theVars->PR_PS_com = true;

    // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    theVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    theVars->RuACT_EPS_com = false;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    theVars->RedoxReg_RA_com = false;        // This means that the connection is not provided there.

    theVars->XanCycle_BF_com = true;

    theVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

    theVars->SUCS_Param = zeros(2);

    // Next is to initialize the vector.

    DynaPSCondition* DynaPS_con = DynaPS_Ini();

    const double va1 = 0;
    // The ratio of the PSI unit to the PSII unit
    theVars->BF_Param[0] = va1;
    theVars->BF_Param[1] = theVars->PS12ratio;

    theVars->FI_Param[0] = va1;
    theVars->FI_Param[1] = theVars->PS12ratio;

    theVars->RuACT_Param[0] = va1;
    theVars->RuACT_Param[1] = theVars->PS12ratio;

    theVars->XanCycle_Param[0] = va1;
    theVars->XanCycle_Param[1] = theVars->PS12ratio;

    constraints = DynaPS_con->toArray();
    delete DynaPS_con;
}

void DynaPSDriver::getResults() {

    DynaPSCondition* dyna_int_con = new DynaPSCondition(intermediateRes);

    arr temp = DynaPS::MB(time, dyna_int_con, theVars);

    double PSIIabs = theVars->FI_Vel.vP680_d;
    double PSIabs = theVars->BF_Vel.Vbf11;
    double CarbonRate = theVars->RuACT_Vel.v6_1 * theVars->AVR;
    double VPR = theVars->RuACT_Vel.v6_2 * theVars->AVR;
    double Vpgasink = theVars->SUCS_Vel.vpga_use * theVars->AVR;
    double VStarch = (theVars->PS_Vel.v23 - theVars->PS_Vel.v25) * theVars->AVR;
    double Vsucrose = theVars->SUCS_Vel.vdhap_in * theVars->AVR;

    results = zeros(7);

    results[0] = PSIIabs;
    results[1] = PSIabs;
    //Resulta(3)=PSIabs2(row);
    results[2] = CarbonRate;
    results[3] = VPR;
    results[4] = Vpgasink;
    results[5] = Vsucrose;
    results[6] = VStarch;

    if(theVars->record) {
        makeFluxTR(theVars);
    }
    // This is to set the regualtions to be as beginning.
    //theVars->ATPActive = 0;
    theVars->BF_FI_com = false;
    theVars->PR_PS_com = false;
    theVars->FIBF_PSPR_com = false;
    theVars->RuACT_EPS_com = false;
    theVars->RedoxReg_RA_com = false;
    theVars->XanCycle_BF_com = false;

    IniModelCom(theVars);
    //save FDC2
}

DynaPSCondition* DynaPSDriver::DynaPS_Ini() {
    return DynaPS::init(theVars);
}

// DynaPS_mb.m  This model includes the mass balance equations for the full model of photosynthesis.

arr DynaPSDriver::MB(realtype t, N_Vector u) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    //// Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.
    realtype *x = N_VGetArrayPointer(u);

    DynaPSCondition* DynaPS_con = new DynaPSCondition(x);

    arr dxdt = DynaPS::MB(t, DynaPS_con, theVars);

    delete DynaPS_con;
    return dxdt;
}
