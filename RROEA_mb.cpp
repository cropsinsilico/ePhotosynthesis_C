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



arr RROEA_Mb(double t, arr &RROEA_Con, varptr *myVars) {
    
    //global GLight;
    Condition(t, myVars);
    const double light = myVars->GLight;
    
    myVars->RROEA_Param[0] = light;
    
    arr RROEA_Vel = zeros(11);
    RROEA_Vel = RROEA_Rate(t, RROEA_Con, myVars);
    
    const double ve2GAPDH = RROEA_Vel[0];
    const double ve2FBPase = RROEA_Vel[1];
    const double ve2SBPase = RROEA_Vel[2];
    const double ve2PRK = RROEA_Vel[3];
    const double ve2ATPase = RROEA_Vel[4];
    const double ve2ATPGPP = RROEA_Vel[5];
    const double ve2MDH = RROEA_Vel[6];
    const double ve2Fd = RROEA_Vel[7];
    const double veFd2Thio = RROEA_Vel[8];
    const double veFd2Calvin = RROEA_Vel[9];
    const double ve2RuACT = RROEA_Vel[10];
    
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
