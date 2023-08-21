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

#include <math.h>
#include "Variables.hpp"
#include "globals.hpp"
#include "drivers/EPS_Driver.hpp"
#include "modules/EPS.hpp"
#include "modules/PS.hpp"
//#include <iostream>
//#include <fstream>

//using namespace std;

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

EPSDriver::~EPSDriver() {
    modules::EPS::_reset();
}

void EPSDriver::setup() {
    //Ca = theVars->TestCa;
    inputVars->TestLi /= 30.;
    //Li = theVars->TestLi;
    AtpCost = inputVars->TestATPCost;
    IniModelCom(inputVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    SYSInitial(inputVars);
    //time = tglobal;
    inputVars->Tp = this->Tp;
    inputVars->alfa = 0.85;
    PS::setJmax(inputVars->EnzymeAct.at("Jmax"));
    inputVars->fc = 0.15;

    double Tp = inputVars->Tp;
    double Theta = 0.76 + 0.01713 * Tp - 3.75 * pow(Tp,2.0) / 10000.0;//Yufeng: match Farquhar Matlab
    PS::setTheta(Theta);
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
    results = zeros(3);
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
