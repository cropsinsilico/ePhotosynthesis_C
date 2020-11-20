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

arr PSmb(double t, arr &PSs, arr &Param, varptr *myVars) {
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifying KM, KI, KE VMAX for different reactions as the regulation//
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    // Regulations first.
    Condition(t, myVars);
    
    // Get the rate for the reactions in the photosynthesis sytem
    
    //arr myVars->PS_Vel = zeros(18);
    PSRate(t, PSs, Param, myVars);
    
    // Get the rate
    
    const double v1 = myVars->PS_Vel.v1;
    const double v2 = myVars->PS_Vel.v2;
    const double v3 = myVars->PS_Vel.v3;
    // NONE = myVars->PS_Vel.v4;// --unused
    const double v5 = myVars->PS_Vel.v5;
    const double v6 = myVars->PS_Vel.v6;
    const double v7 = myVars->PS_Vel.v7;
    const double v8 = myVars->PS_Vel.v8;
    const double v9 = myVars->PS_Vel.v9;
    const double v10 = myVars->PS_Vel.v10;
    const double v13 = myVars->PS_Vel.v13;
    const double v16 = myVars->PS_Vel.v16;
    const double v23 = myVars->PS_Vel.v23;
    const double v31 = myVars->PS_Vel.v31;
    const double v32 = myVars->PS_Vel.v32;
    const double v33 = myVars->PS_Vel.v33;
    const double v24 = myVars->PS_Vel.v24;
    const double v25 = myVars->PS_Vel.v25;
    
    // Implement the mass balance equations
    
    arr tmp = zeros(15);
    
    
    tmp[0] = v13 - v1;
    
    
    tmp[1] = 2 * v1 - v2 - v32;
    
    
    tmp[2] = v2 - v3;
    
    
    tmp[3] = v3 - 2 * v5 - v7 - v8 - v10 - v31 - v33;
    
    
    tmp[4] = v23 - v24;
    
    
    tmp[5] = v5 - v6;
    
    
    tmp[6] = v7 - v8;
    
    
    tmp[7] = v9 - v10;
    
    
    tmp[8] = v8 - v9;
    
    
    tmp[9] = v16 - v2 - v23 - v13 - v25;
    
    //global PS2EPS_NADPH;
    //global PS_C_CN;
    
    tmp[10] = 0;
    
    
    
    tmp[11] = 0;
    
    
    tmp[12] = 0;
    
    
    tmp[13] = v6 - v7 - v23 + v25;
    
    
    tmp[14] = v7 + v10 * 2 - v13;
    
    arr PSdydt = tmp;
    return PSdydt;
}
