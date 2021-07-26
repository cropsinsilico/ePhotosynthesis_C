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

EPSDriver::~EPSDriver() {}

void EPSDriver::setup() {
    //Ca = theVars->TestCa;
    theVars->TestLi /= 30.;
    //Li = theVars->TestLi;
    AtpCost = theVars->TestATPCost;
    SucPath = theVars->TestSucPath;
    IniModelCom(theVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    SYSInitial(theVars);
    //time = tglobal;
    theVars->Tp = this->Tp;
    theVars->alfa = 0.85;
    theVars->Jmax = theVars->EnzymeAct.at("Jmax");
    theVars->fc = 0.15;
    theVars->Theta = 0.7;
    theVars->beta = 0.7519;
    theVars->BF_FI_com = true;

    theVars->PR_PS_com = true;     // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.


    theVars->FIBF_PSPR_com = true;     //true means that the overall EPS model is used. false means partial model is used.

    /////theVars->ATPActive = 0;

    theVars->EPS_ATP_Rate = 0;   // Indicate in the beginning there is no ATP synthesis activity.

    theVars->EPS_SUCS_com = true;

    theVars->PSPR_SUCS_com = true;   // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.

    /////////////////////////
    //   Calculation  step //
    /////////////////////////
    EPSCondition* EPS_Con = EPS_Init();

    int va1 = 0;
    theVars->BF_Param[0] = va1;
    theVars->BF_Param[1] = theVars->PS12ratio;

    theVars->FI_Param[0] = va1;
    theVars->FI_Param[1] = theVars->PS12ratio;

    theVars->PS_PR_Param = 0;
///////theVars->Sucs_Param = 0;
    // Get the initial variables for SUCS.



    //////////////////////////////////////////////////////////////////////////
    //                      END                                              /
    //////////////////////////////////////////////////////////////////////////

    // This is a variable to indicate whether BF is run by itself or together.


    constraints = zeros(87);
    arr temp = EPS_Con->toArray();
    for (size_t i = 0; i < constraints.size(); i++)
        constraints[i] = temp[i];
    delete EPS_Con;
}

void EPSDriver::getResults() {
    uint adjust = 0;
    if (theVars->useC3)
        adjust = 1;
    EPSCondition* eps_int_con = new EPSCondition(intermediateRes, adjust);
    arr temp = EPS::MB(time, eps_int_con, theVars);
    results = zeros(1);
    const double Arate = (theVars->PS_Vel.v1 - theVars->PR_Vel.v131) * theVars->AVR;
    results[0] = Arate;
}

EPSCondition* EPSDriver::EPS_Init() {
    return EPS::init(theVars);
}

arr EPSDriver::MB(realtype t, N_Vector u) {
    //// Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.
    realtype *x = N_VGetArrayPointer(u);

    uint adjust = 0;
    if (theVars->useC3) {
        adjust = 1;
    }
    EPSCondition* EPS_con = new EPSCondition(x, adjust);

    arr dxdt = EPS::MB(t, EPS_con, theVars);
    delete EPS_con;

    return dxdt;
}
