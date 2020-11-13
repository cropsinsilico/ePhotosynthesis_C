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



arr RuACT_Mb(double t, arr &RuACT_Con, varptr *myVars) {
    
    //global GLight;
    Condition(t, myVars);
    const double light = myVars->GLight;
    
    myVars->RuACT_Param[0] = light;
    
    RuACT_Rate(t, RuACT_Con, myVars);
    
    const double v1 = myVars->RuACT_Vel[0];	//	v1	The rate of ER activation due to Rubisco activase
    const double vn1 = myVars->RuACT_Vel[1];	//	vn1	The rate of E inactiavtion due to binding of RuBP
    const double v7 = myVars->RuACT_Vel[2];	//	v7	The rate of formation of ECMR from ECM by binding of RuBP
    const double vn7 = myVars->RuACT_Vel[3];	//	vn7	The rate of actiavtion of ECMR by Rubisco activase
    const double v6_1 = myVars->RuACT_Vel[4];	//	v6_1	The rate of RuBP carboxylation
    const double v6_2 = myVars->RuACT_Vel[5];	//	v6_2	The rate of RuBP oxygenation
    
    
    arr RuACT_mb = zeros(4);
    RuACT_mb[0] = vn1 - v1;//	ER
    RuACT_mb[1] = v1 - v7 + vn7 + v6_1 + v6_2 - vn1;//	EAF
    RuACT_mb[2] = v7 - vn7 - v6_1 - v6_2;//	ECMR
    RuACT_mb[3] = v6_1 + v6_2 + v1 - vn1 + vn7 - v7;//	RuBP
    return RuACT_mb;
}
