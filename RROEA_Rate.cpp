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



arr RROEA_Rate(double t, arr &RROEA_Con, arr &RROEA_Param, varptr &myVars) {
    
    arr RROEA_Vel = zeros(11);
    
    //global RROEA_RC;
    
    const double light = RROEA_Param[0];
    
    const double ke2GAPDH = myVars.RROEA_RC[0];	//	The rate constant of electron transfer to GAPDH. From literature.
    const double ke2MDH = myVars.RROEA_RC[1];		//	The rate constant of electront transfer to MDH, this rate is totally ASSUMED.
    const double ke2FBPase = myVars.RROEA_RC[2];	//	The rate constant of electron transfer from thioredoxin to FBPase.
    const double ke2SBPase = myVars.RROEA_RC[3];	//	The rate constant of electron tranfer from thioredoxin to SBPase
    const double ke2PRK = myVars.RROEA_RC[4];	    //	The rate constant of electron transfer from thioredoxin to PRK, Phosphoribulase kinase
    const double ke2RuACT = myVars.RROEA_RC[5];	//	The rate constant of electron transfer from thioredoxin to Rubisco activase
    const double ke2Fd = myVars.RROEA_RC[6];	    //	The rate constant of electron transfer to fe
    const double keFd2Thio = myVars.RROEA_RC[7];	//	The rate constant of electron transfer from fd to thio
    const double keFd2Calvin = myVars.RROEA_RC[8];	    //	The rate constant of electron transfer from fd to Calvin cycle
    const double ke2ATPGPP = myVars.RROEA_RC[9];	    //	The rate constant of electron transfer from fd to ATPGPP
    
    
    //global RROEA_KE;
    
    const double KEe2FBPase = myVars.RROEA_KE[0];
    const double KEe2SBPase = myVars.RROEA_KE[1];
    const double KEe2PRK = myVars.RROEA_KE[2];
    double KEe2ATPase = myVars.RROEA_KE[3];
    const double KEe2RuACT = myVars.RROEA_KE[4];
    const double KEe2GAPDH = myVars.RROEA_KE[5];
    const double KEe2MDH = myVars.RROEA_KE[6];
    const double KEe2ATPGPP = myVars.RROEA_KE[7];
    const double KEeFd2Thio = myVars.RROEA_KE[8];
    
    const double GAPDH = RROEA_Con[0];	//	The  concentration of active GAPDH
    const double FBPase = RROEA_Con[1];	//	The  concentration of active FBPase
    const double SBPase = RROEA_Con[2];	//	The  concentration of active SBPase
    const double PRK = RROEA_Con[3];	//	The  concentration of actove PRK
    const double ATPase = RROEA_Con[4];	//	The  concentration of actove ATPase
    const double ATPGPP = RROEA_Con[5];	//	The  concentration of actove ATPGPP
    const double MDH = RROEA_Con[6];	//	The  concentration of actove MDH
    const double Thio = RROEA_Con[7];                  //   The  concentration of
    const double Fd = RROEA_Con[8];	//	The  concentration of reduced ferrodoxin
    const double RuACT = RROEA_Con[9];               // The concentration of Rubisco activase
    
    
    //global RROEA_Pool;
    
    const double GAPDHT = myVars.RROEA_Pool[0];
    const double FBPaseT = myVars.RROEA_Pool[1];
    const double SBPaseT = myVars.RROEA_Pool[2];
    const double PRKT = myVars.RROEA_Pool[3];
    const double ATPaseT = myVars.RROEA_Pool[4];
    const double ATPGPPT = myVars.RROEA_Pool[5];
    const double MDHT = myVars.RROEA_Pool[6];
    const double ThioT = myVars.RROEA_Pool[7];
    const double FdT = myVars.RROEA_Pool[8];
    const double RuACTT = myVars.RROEA_Pool[9];
    
    
    const double GAPDHo = GAPDHT - GAPDH;
    const double FBPaseo = FBPaseT - FBPase;
    const double SBPaseo = SBPaseT - SBPase;
    const double PRKo = PRKT - PRK;
    const double ATPaseo = ATPaseT - ATPase;
    const double ATPGPPo = ATPGPPT - ATPGPP;
    const double MDHo = MDHT - MDH;
    const double Thioo = ThioT - Thio;
    const double Fdo = FdT - Fd;
    const double RuACTo = RuACTT - RuACT;
    
    const double ve2GAPDH = ke2GAPDH * (Thio * GAPDHo - Thioo * GAPDH / KEe2GAPDH);
    const double ve2FBPase = ke2FBPase * (Thio * FBPaseo - Thioo * FBPase / KEe2FBPase);
    const double ve2SBPase = ke2SBPase * (Thio * SBPaseo - Thioo * SBPase / KEe2SBPase);
    const double ve2PRK = ke2PRK * (Thio * PRKo - Thioo * PRK / KEe2PRK);
    
    KEe2ATPase = 1;
    const double ke2ATPase = 1;
    
    const double ve2ATPase = ke2ATPase * (Thio * ATPaseo - Thioo * ATPase / KEe2ATPase);
    const double ve2ATPGPP = ke2ATPGPP * (Thio * ATPGPPo - Thioo * ATPGPP / KEe2ATPGPP);
    const double ve2MDH = ke2MDH * (Thio * MDHo - Thioo * MDH / KEe2MDH) - MDH;
    
    double ve2Fd;
    if (light > 500) {
        ve2Fd = ke2Fd * Fdo;
    } else {
        ve2Fd = ke2Fd * light / 500 * Fdo;
    }
    
    
    const double veFd2Thio = keFd2Thio * (Fd * Thioo - Thio * Fdo / KEeFd2Thio);
    
    const double veFd2Calvin = Fd * keFd2Calvin * (FBPase / FBPaseT);
    
    const double ve2RuACT = ke2RuACT * (Thio * RuACTo - Thioo * RuACT / KEe2RuACT);
    
    //global RROEA_OLD_TIME;
    //global RROEA_TIME_N;
    //global RROEA_VEL;
    //global RROEA_CON;
    
    if (myVars.RROEA_TIME_N == 0)
        myVars.RROEA_TIME_N = 1;
    
    
    if (t > myVars.RROEA_OLD_TIME) {
        myVars.RROEA_TIME_N = myVars.RROEA_TIME_N + 1;
        myVars.RROEA_OLD_TIME = t;
    }
    
    if (myVars.RROEA_VEL.shape()[0] < myVars.RROEA_TIME_N) {
        myVars.RROEA_VEL.resize(boost::extents[myVars.RROEA_TIME_N][RROEA_VEL_SIZE]);
        myVars.RROEA_CON.resize(boost::extents[myVars.RROEA_TIME_N][RROEA_CON_SIZE]);
    }

    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][0] = t;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][1] = ve2GAPDH;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][2] = ve2FBPase;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][3] = ve2SBPase;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][4] = ve2PRK;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][5] = ve2ATPase;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][6] = ve2ATPGPP;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][7] = ve2MDH;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][8] = ve2Fd;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][9] = veFd2Thio;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][10] = veFd2Calvin;
    myVars.RROEA_VEL[myVars.RROEA_TIME_N - 1][11] = ve2RuACT;
    
    //myVars.RROEA_CON[myVars.RROEA_TIME_N - 1][0] = t;  // --unused
    //myVars.RROEA_CON[myVars.RROEA_TIME_N - 1][1] = Thioo;  // --unused
    
    
    
    RROEA_Vel[0] = ve2GAPDH;
    RROEA_Vel[1] = ve2FBPase;
    RROEA_Vel[2] = ve2SBPase;
    RROEA_Vel[3] = ve2PRK;
    RROEA_Vel[4] = ve2ATPase;
    RROEA_Vel[5] = ve2ATPGPP;
    RROEA_Vel[6] = ve2MDH;
    RROEA_Vel[7] = ve2Fd;
    RROEA_Vel[8] = veFd2Thio;
    RROEA_Vel[9] = veFd2Calvin;
    RROEA_Vel[10] = ve2RuACT;
    
    //global RROEA2PS_GAPDH;
    //global RROEA2PS_FBPase;
    //global RROEA2PS_SBPase;
    //global RROEA2PS_PRK;
    //global RROEA2PS_ATPase;
    //global RROEA2PS_ATPGPP;
    
    myVars.RROEA2PS_GAPDH = GAPDH;
    //myVars.RROEA2PS_FBPase = FBPase;  // --unused
    myVars.RROEA2PS_SBPase = SBPase;
    myVars.RROEA2PS_PRK = PRK;
    myVars.RROEA2PS_ATPase = ATPase;
    myVars.RROEA2PS_ATPGPP = ATPGPP;
    
    //global RROEA2RuACT_RuAC;
    myVars.RROEA2RuACT_RuAC = RuACT;
    
    //global RROEA2trDynaPS_ve2Fd;
    myVars.RROEA2trDynaPS_ve2Fd = ve2Fd;
    
    //global RROEA2trDynaPS_veFd2Calvin;
    myVars.RROEA2trDynaPS_veFd2Calvin = veFd2Calvin;
    return RROEA_Vel;
}
