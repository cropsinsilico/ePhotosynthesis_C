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

arr RROEA_Mb(double t, RROEACon &RROEA_Con, Variables *myVars) {
    Condition(t, myVars);
    const double light = myVars->GLight;

    myVars->RROEA_Param[0] = light;

    RROEA_Rate(t, RROEA_Con, myVars);

    const double ve2GAPDH = myVars->RROEA_Vel.ve2GAPDH;
    const double ve2FBPase = myVars->RROEA_Vel.ve2FBPase;
    const double ve2SBPase = myVars->RROEA_Vel.ve2SBPase;
    const double ve2PRK = myVars->RROEA_Vel.ve2PRK;
    const double ve2ATPase = myVars->RROEA_Vel.ve2ATPase;
    const double ve2ATPGPP = myVars->RROEA_Vel.ve2ATPGPP;
    const double ve2MDH = myVars->RROEA_Vel.ve2MDH;
    const double ve2Fd = myVars->RROEA_Vel.ve2Fd;
    const double veFd2Thio = myVars->RROEA_Vel.veFd2Thio;
    const double veFd2Calvin = myVars->RROEA_Vel.veFd2Calvin;
    const double ve2RuACT = myVars->RROEA_Vel.ve2RuACT;

    arr RROEA_mb = zeros(10);
    RROEA_mb[0] = ve2GAPDH;	//	GAPDH
    RROEA_mb[1] = ve2FBPase;	//	FBPase
    RROEA_mb[2] = ve2SBPase;	//	SBPase
    RROEA_mb[3] = ve2PRK;	    //	PRK
    RROEA_mb[4] = ve2ATPase;	//	ATPase
    RROEA_mb[5] = ve2ATPGPP;	//	ATPGPP
    RROEA_mb[6] = ve2MDH;	    //	MDH
    RROEA_mb[7] = veFd2Thio - ve2GAPDH - ve2FBPase - ve2SBPase - ve2PRK - ve2ATPGPP - ve2RuACT;//	Thio
    RROEA_mb[8] = ve2Fd - veFd2Thio - veFd2Calvin;//	Fd
    RROEA_mb[9] = ve2RuACT;   // RuACT;
    return RROEA_mb;
}
