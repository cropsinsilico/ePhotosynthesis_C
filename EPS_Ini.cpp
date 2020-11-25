#include "Variables.hpp"
#include "EPS.hpp"
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

EPSCon EPS_Ini(Variables *myVars) {
    FIBF_Ini(myVars);
    FICon FI_Con = FI_Ini(myVars);
    BFCon BF_con = BF_Ini(myVars);
    FIBFCon FIBF_con(BF_con, FI_Con);
    //// Step 1 Get the initialization of the variables for BF

    myVars->BF_OLD_TIME = 0;
    myVars->BF_TIME_N = 1;

    //// Get the initialization of the variables for FI

    myVars->FI_OLD_TIME = 0;
    myVars->FI_TIME_N = 1;
    myVars->FI_Pool.PQT = myVars->FIBF_Pool.PQT;
    myVars->BF_Pool.k_r1 = myVars->FIBF_Pool.PQT;

    // Second, try to get the iniitalzation files for the PSPR model

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Global variables used for obtaining flux and concentration data //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    myVars->PS_OLD_TIME = 0;
    myVars->PS_TIME_N = 1;
    myVars->PR_OLD_TIME = 0;
    myVars->PR_TIME_N = 1;

    ////////////////////////////////////////////////
    //   Initialation step //
    ////////////////////////////////////////////////

    CMCon CM_con = CMInit(myVars);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Use the initialized variables to construct variables that will be transfered to the Drive file. ////////////
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    EPSCon EPS_con(FIBF_con, CM_con);

    return EPS_con;
}
