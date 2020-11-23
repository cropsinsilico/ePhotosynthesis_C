#include "globals.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arr EPS_Ini(varptr *myVars) {
    
    // Begin = 1;// --unused
    
    FIBF_Ini(myVars);
    FI_Ini(myVars);
    BF_Ini(myVars);
    
    //// Step 1 Get the initialization of the variables for BF
    
    //global BF_OLD_TIME;
    //global BF_TIME_N;
    //global BF_VEL;
    //global BF_CON;
    
    myVars->BF_OLD_TIME = 0;
    myVars->BF_TIME_N = 1;
    
    // BF_VEL = zeros(1, 5);    // Clean memory
    // BF_CON = zeros(1, 5);    // Clean memory
    
    //// Get the initialization of the variables for FI
    
    //global FI_OLD_TIME;
    //global FI_TIME_N;
    //global FI_VEL;
    //global FI_CON;
    
    myVars->FI_OLD_TIME = 0;
    myVars->FI_TIME_N = 1;
    
    // FI_VEL = zeros(1, 5);    // Clean memory
    // FI_CON = zeros(5, 1);    // Clean memory
    
    
    //global FIBF_Pool;
    //global FI_Pool;
    //global BF_Pool;
    myVars->FI_Pool.PQT = myVars->FIBF_Pool.PQT;
    myVars->BF_Pool.k_r1 = myVars->FIBF_Pool.PQT;
    
    // Initial concentration for FIBF_Con
    arr FIBF_Con = zeros(52);
    //FIBF_Con;
    for (int m = 0; m < 29; m++)
        FIBF_Con[m] = myVars->BF_con[m];
    
    arr FI_con = myVars->FI_Con.toArray();
    for (int m = 0; m < 22; m++)
        FIBF_Con[m + 29] = FI_con[m];
    
    
    FIBF_Con[51] = pow(10, 8) * 0.5;
    
    // Second, try to get the iniitalzation files for the PSPR model
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Global variables used for obtaining flux and concentration data //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //global PS_PR_OLDTIME;
    //global PS_PR_TIME_N;
    //global PS_PR_VEL;
    
    //myVars->PS_PR_OLDTIME = 0;  // --unused
    //myVars->PS_PR_TIME_N = 1;  // --unused
    // PS_PR_VEL = zeros(27, 1);        // Store the flux value
    
    //global PS_OLD_TIME;
    //global PS_TIME_N;
    //global PS_VEL;
    myVars->PS_OLD_TIME = 0;
    myVars->PS_TIME_N = 1;
    // PS_VEL = zeros(1, 1);
    
    //global PR_OLD_TIME;
    //global PR_TIME_N;
    //global PR_VEL;
    myVars->PR_OLD_TIME = 0;
    myVars->PR_TIME_N = 1;
    // PR_VEL = zeros(1, 1);
    
    ////////////////////////////////////////////////
    //   Initialation step //
    ////////////////////////////////////////////////
    
    //Begin = 1;  // --unused
    CM temp = CM(myVars);
    arr CMs = temp.CM_Ini();
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Use the initialized variables to construct variables that will be transfered to the Drive file. ////////////
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    arr EPS_Con = zeros(52 + 36);
    for (int m = 0; m < 52; m++)
        EPS_Con[m] = FIBF_Con[m];
    
    
    for (int m = 0; m < 36; m++)
        EPS_Con[m + 52] = CMs[m];
    
    
    return EPS_Con;
}
