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
#include "modules/RROEA.hpp"

#define KEe2ATPase 1.
#define ke2ATPase 1.

void RROEA::_Rate(const double t, const RROEACondition* RROEA_con, Variables *theVars) {

    const double GAPDHo = theVars->RROEA_Pool.GAPDH - RROEA_con->GAPDH;
    const double FBPaseo = theVars->RROEA_Pool.FBPase - RROEA_con->FBPase;
    const double SBPaseo = theVars->RROEA_Pool.SBPase - RROEA_con->SBPase;
    const double PRKo = theVars->RROEA_Pool.PRK - RROEA_con->PRK;
    const double ATPaseo = theVars->RROEA_Pool.ATPase - RROEA_con->ATPase;
    const double ATPGPPo = theVars->RROEA_Pool.ATPGPP - RROEA_con->ATPGPP;
    const double MDHo = theVars->RROEA_Pool.MDH - RROEA_con->MDH;
    const double Thioo = theVars->RROEA_Pool.ThioT - RROEA_con->Thio;
    const double Fdo = theVars->RROEA_Pool.FdT - RROEA_con->Fd;
    const double RuACTo = theVars->RROEA_Pool.RuACTT - RROEA_con->RuACT;

    if (theVars-> RROEA_Param[0] > 500) {
        theVars->RROEA_Vel.ve2Fd = theVars->RROEA_RC.ke2Fd * Fdo;
    } else {
        theVars->RROEA_Vel.ve2Fd = theVars->RROEA_RC.ke2Fd * theVars-> RROEA_Param[0] / 500 * Fdo;
    }

    if (t > theVars->RROEA_OLD_TIME) {
            theVars->RROEA_TIME_N = theVars->RROEA_TIME_N + 1;
            theVars->RROEA_OLD_TIME = t;
        }
    theVars->RROEA_Vel.ve2GAPDH = theVars->RROEA_RC.ke2GAPDH * (RROEA_con->Thio * GAPDHo - Thioo * RROEA_con->GAPDH / theVars->RROEA_KE.KEe2GAPDH);
    theVars->RROEA_Vel.ve2FBPase = theVars->RROEA_RC.ke2FBPase * (RROEA_con->Thio * FBPaseo - Thioo * RROEA_con->FBPase / theVars->RROEA_KE.KEe2FBPase);
    theVars->RROEA_Vel.ve2SBPase = theVars->RROEA_RC.ke2SBPase * (RROEA_con->Thio * SBPaseo - Thioo * RROEA_con->SBPase / theVars->RROEA_KE.KEe2SBPase);
    theVars->RROEA_Vel.ve2PRK = theVars->RROEA_RC.ke2PRK * (RROEA_con->Thio * PRKo - Thioo * RROEA_con->PRK / theVars->RROEA_KE.KEe2PRK);
    theVars->RROEA_Vel.ve2ATPase = ke2ATPase * (RROEA_con->Thio * ATPaseo - Thioo * RROEA_con->ATPase / KEe2ATPase);
    theVars->RROEA_Vel.ve2ATPGPP = theVars->RROEA_RC.ke2ATPGPP * (RROEA_con->Thio * ATPGPPo - Thioo * RROEA_con->ATPGPP / theVars->RROEA_KE.KEe2ATPGPP);
    theVars->RROEA_Vel.ve2MDH = theVars->RROEA_RC.ke2MDH * (RROEA_con->Thio * MDHo - Thioo * RROEA_con->MDH / theVars->RROEA_KE.KEe2MDH) - RROEA_con->MDH;
    theVars->RROEA_Vel.veFd2Thio = theVars->RROEA_RC.keFd2Thio * (RROEA_con->Fd * Thioo - RROEA_con->Thio * Fdo / theVars->RROEA_KE.KEeFd2Thio);
    theVars->RROEA_Vel.veFd2Calvin = RROEA_con->Fd * theVars->RROEA_RC.keFd2Calvin * (RROEA_con->FBPase / theVars->RROEA_Pool.FBPase);
    theVars->RROEA_Vel.ve2RuACT = theVars->RROEA_RC.ke2RubACT * (RROEA_con->Thio * RuACTo - Thioo * RROEA_con->RuACT / theVars->RROEA_KE.KEe2RuACT);

    DEBUG_INTERNAL(theVars->RROEA_Vel)
    if (theVars->record) {
        theVars->RROEA_VEL.insert(theVars->RROEA_TIME_N - 1, t, theVars->RROEA_Vel);
    }
}
