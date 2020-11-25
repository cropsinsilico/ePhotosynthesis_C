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

    const double GAPDHo = myVars->RROEA_Pool.GAPDH - RROEA_con.GAPDH;
    const double FBPaseo = myVars->RROEA_Pool.FBPase - RROEA_con.FBPase;
    const double SBPaseo = myVars->RROEA_Pool.SBPase - RROEA_con.SBPase;
    const double PRKo = myVars->RROEA_Pool.PRK - RROEA_con.PRK;
    const double ATPaseo = myVars->RROEA_Pool.ATPase - RROEA_con.ATPase;
    const double ATPGPPo = myVars->RROEA_Pool.ATPGPP - RROEA_con.ATPGPP;
    const double MDHo = myVars->RROEA_Pool.MDH - RROEA_con.MDH;
    const double Thioo = myVars->RROEA_Pool.ThioT - RROEA_con.Thio;
    const double Fdo = myVars->RROEA_Pool.FdT - RROEA_con.Fd;
    const double RuACTo = myVars->RROEA_Pool.RuACTT - RROEA_con.RuACT;


    const double KEe2ATPase = 1;
    const double ke2ATPase = 1;

    if (myVars-> RROEA_Param[0] > 500) {
        myVars->RROEA_Vel.ve2Fd = myVars->RROEA_RC.ke2Fd * Fdo;
    } else {
        myVars->RROEA_Vel.ve2Fd = myVars->RROEA_RC.ke2Fd * myVars-> RROEA_Param[0] / 500 * Fdo;
    }

    if (t > myVars->RROEA_OLD_TIME) {
            myVars->RROEA_TIME_N = myVars->RROEA_TIME_N + 1;
            myVars->RROEA_OLD_TIME = t;
        }
    myVars->RROEA_Vel.ve2GAPDH = myVars->RROEA_RC.ke2GAPDH * (RROEA_con.Thio * GAPDHo - Thioo * RROEA_con.GAPDH / myVars->RROEA_KE.KEe2GAPDH);
    myVars->RROEA_Vel.ve2FBPase = myVars->RROEA_RC.ke2FBPase * (RROEA_con.Thio * FBPaseo - Thioo * RROEA_con.FBPase / myVars->RROEA_KE.KEe2FBPase);
    myVars->RROEA_Vel.ve2SBPase = myVars->RROEA_RC.ke2SBPase * (RROEA_con.Thio * SBPaseo - Thioo * RROEA_con.SBPase / myVars->RROEA_KE.KEe2SBPase);
    myVars->RROEA_Vel.ve2PRK = myVars->RROEA_RC.ke2PRK * (RROEA_con.Thio * PRKo - Thioo * RROEA_con.PRK / myVars->RROEA_KE.KEe2PRK);
    myVars->RROEA_Vel.ve2ATPase = ke2ATPase * (RROEA_con.Thio * ATPaseo - Thioo * RROEA_con.ATPase / KEe2ATPase);
    myVars->RROEA_Vel.ve2ATPGPP = myVars->RROEA_RC.ke2ATPGPP * (RROEA_con.Thio * ATPGPPo - Thioo * RROEA_con.ATPGPP / myVars->RROEA_KE.KEe2ATPGPP);
    myVars->RROEA_Vel.ve2MDH = myVars->RROEA_RC.ke2MDH * (RROEA_con.Thio * MDHo - Thioo * RROEA_con.MDH / myVars->RROEA_KE.KEe2MDH) - RROEA_con.MDH;
    myVars->RROEA_Vel.veFd2Thio = myVars->RROEA_RC.keFd2Thio * (RROEA_con.Fd * Thioo - RROEA_con.Thio * Fdo / myVars->RROEA_KE.KEeFd2Thio);
    myVars->RROEA_Vel.veFd2Calvin = RROEA_con.Fd * myVars->RROEA_RC.keFd2Calvin * (RROEA_con.FBPase / myVars->RROEA_Pool.FBPase);
    myVars->RROEA_Vel.ve2RuACT = myVars->RROEA_RC.ke2RubACT * (RROEA_con.Thio * RuACTo - Thioo * RROEA_con.RuACT / myVars->RROEA_KE.KEe2RuACT);

    if (myVars->record) {
        myVars->RROEA_VEL.insert(myVars->RROEA_TIME_N - 1, t, myVars->RROEA_Vel);
    }
    myVars->RROEA2PS_GAPDH = RROEA_con.GAPDH;
    myVars->RROEA2PS_SBPase = RROEA_con.SBPase;
    myVars->RROEA2PS_PRK = RROEA_con.PRK;
    myVars->RROEA2PS_ATPase = RROEA_con.ATPase;
    myVars->RROEA2PS_ATPGPP = RROEA_con.ATPGPP;

    myVars->RROEA2RuACT_RuAC = RROEA_con.RuACT;
}
