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

void RROEA_Rate(double t, RROEACon &RROEA_con, Variables *myVars) {

    const double light =myVars-> RROEA_Param[0];

    const double ke2GAPDH = myVars->RROEA_RC.ke2GAPDH;	//	The rate constant of electron transfer to GAPDH. From literature.
    const double ke2MDH = myVars->RROEA_RC.ke2MDH;		//	The rate constant of electront transfer to MDH, this rate is totally ASSUMED.
    const double ke2FBPase = myVars->RROEA_RC.ke2FBPase;	//	The rate constant of electron transfer from thioredoxin to FBPase.
    const double ke2SBPase = myVars->RROEA_RC.ke2SBPase;	//	The rate constant of electron tranfer from thioredoxin to SBPase
    const double ke2PRK = myVars->RROEA_RC.ke2PRK;	    //	The rate constant of electron transfer from thioredoxin to PRK, Phosphoribulase kinase
    const double ke2RuACT = myVars->RROEA_RC.ke2RubACT;	//	The rate constant of electron transfer from thioredoxin to Rubisco activase
    const double ke2Fd = myVars->RROEA_RC.ke2Fd;	    //	The rate constant of electron transfer to fe
    const double keFd2Thio = myVars->RROEA_RC.keFd2Thio;	//	The rate constant of electron transfer from fd to thio
    const double keFd2Calvin = myVars->RROEA_RC.keFd2Calvin;	    //	The rate constant of electron transfer from fd to Calvin cycle
    const double ke2ATPGPP = myVars->RROEA_RC.ke2ATPGPP;	    //	The rate constant of electron transfer from fd to ATPGPP

    const double KEe2FBPase = myVars->RROEA_KE.KEe2FBPase;
    const double KEe2SBPase = myVars->RROEA_KE.KEe2SBPase;
    const double KEe2PRK = myVars->RROEA_KE.KEe2PRK;
    double KEe2ATPase = myVars->RROEA_KE.KEe2ATPase;
    const double KEe2RuACT = myVars->RROEA_KE.KEe2RuACT;
    const double KEe2GAPDH = myVars->RROEA_KE.KEe2GAPDH;
    const double KEe2MDH = myVars->RROEA_KE.KEe2MDH;
    const double KEe2ATPGPP = myVars->RROEA_KE.KEe2ATPGPP;
    const double KEeFd2Thio = myVars->RROEA_KE.KEeFd2Thio;

    const double GAPDH = RROEA_con.GAPDH;	//	The  concentration of active GAPDH
    const double FBPase = RROEA_con.FBPase;	//	The  concentration of active FBPase
    const double SBPase = RROEA_con.SBPase;	//	The  concentration of active SBPase
    const double PRK = RROEA_con.PRK;	//	The  concentration of actove PRK
    const double ATPase = RROEA_con.ATPase;	//	The  concentration of actove ATPase
    const double ATPGPP = RROEA_con.ATPGPP;	//	The  concentration of actove ATPGPP
    const double MDH = RROEA_con.MDH;	//	The  concentration of actove MDH
    const double Thio = RROEA_con.Thio;                  //   The  concentration of
    const double Fd = RROEA_con.Fd;	//	The  concentration of reduced ferrodoxin
    const double RuACT = RROEA_con.RuACT;               // The concentration of Rubisco activase

    const double GAPDHT = myVars->RROEA_Pool.GAPDH;
    const double FBPaseT = myVars->RROEA_Pool.FBPase;
    const double SBPaseT = myVars->RROEA_Pool.SBPase;
    const double PRKT = myVars->RROEA_Pool.PRK;
    const double ATPaseT = myVars->RROEA_Pool.ATPase;
    const double ATPGPPT = myVars->RROEA_Pool.ATPGPP;
    const double MDHT = myVars->RROEA_Pool.MDH;
    const double ThioT = myVars->RROEA_Pool.ThioT;
    const double FdT = myVars->RROEA_Pool.FdT;
    const double RuACTT = myVars->RROEA_Pool.RuACTT;

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

    if (t > myVars->RROEA_OLD_TIME) {
            myVars->RROEA_TIME_N = myVars->RROEA_TIME_N + 1;
            myVars->RROEA_OLD_TIME = t;
        }
    myVars->RROEA_Vel.ve2GAPDH = ve2GAPDH;
    myVars->RROEA_Vel.ve2FBPase = ve2FBPase;
    myVars->RROEA_Vel.ve2SBPase = ve2SBPase;
    myVars->RROEA_Vel.ve2PRK = ve2PRK;
    myVars->RROEA_Vel.ve2ATPase = ve2ATPase;
    myVars->RROEA_Vel.ve2ATPGPP = ve2ATPGPP;
    myVars->RROEA_Vel.ve2MDH = ve2MDH;
    myVars->RROEA_Vel.ve2Fd = ve2Fd;
    myVars->RROEA_Vel.veFd2Thio = veFd2Thio;
    myVars->RROEA_Vel.veFd2Calvin = veFd2Calvin;
    myVars->RROEA_Vel.ve2RuACT = ve2RuACT;

    if (myVars->record) {
        myVars->RROEA_VEL.insert(myVars->RROEA_TIME_N - 1, t, myVars->RROEA_Vel);
    }
    myVars->RROEA2PS_GAPDH = GAPDH;
    myVars->RROEA2PS_SBPase = SBPase;
    myVars->RROEA2PS_PRK = PRK;
    myVars->RROEA2PS_ATPase = ATPase;
    myVars->RROEA2PS_ATPGPP = ATPGPP;

    myVars->RROEA2RuACT_RuAC = RuACT;

    myVars->RROEA2trDynaPS_ve2Fd = ve2Fd;

    myVars->RROEA2trDynaPS_veFd2Calvin = veFd2Calvin;
}
