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

#include "globals.hpp"
#include "Variables.hpp"

CMDriver::~CMDriver() {}
void CMDriver::setup() {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Global variables used for obtaining flux and concentration data //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    theVars->PS_PR_OLDTIME = 0;
    theVars->PS_PR_TIME_N = 1;
    theVars->PS_OLD_TIME = 0;
    theVars->PS_TIME_N = 0;
    theVars->PR_OLD_TIME = 0;
    theVars->PR_TIME_N = 1;

    ////////////////////////////////////////////////
    //   Initialation step //
    ////////////////////////////////////////////////
    IniModelCom(theVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.
    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    theVars->PR_PS_com = true;
    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

    CMCon CM_con = CM_Ini();
    constraints = CM_con.toArray();
    ////////////////////////////////////////////////
    //   Calculation  step //
    ////////////////////////////////////////////////

    theVars->SUCS_Param[0] = 1;
    theVars->SUCS_Param[1] = 1;

    theVars->PS_PR_Param = 0;
}

void CMDriver::getResults() {
    //////////////////////////////////////////////
    //   output  step      //
    //////////////////////////////////////////////
    CMCon CM_int_con(intermediateRes);

    arr temp = CM_Mb(time, CM_int_con, theVars);
    // Reinitialize some values of global variables.
    theVars->PSPR_SUCS_com = false;
    IniModelCom(theVars);

    const double CO2AR = TargetFunVal(theVars);


    results = {CO2AR};
}
