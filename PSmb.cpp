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
    
    arr PSr = zeros(18);
    PSr = PSRate(t, PSs, Param, myVars);
    
    // Get the rate
    
    const double v1 = PSr[0];
    const double v2 = PSr[1];
    const double v3 = PSr[2];
    // NONE = PSr[3];// --unused
    const double v5 = PSr[4];
    const double v6 = PSr[5];
    const double v7 = PSr[6];
    const double v8 = PSr[7];
    const double v9 = PSr[8];
    const double v10 = PSr[9];
    const double v13 = PSr[10];
    const double v16 = PSr[11];
    const double v23 = PSr[12];
    const double v31 = PSr[13];
    const double v32 = PSr[14];
    const double v33 = PSr[15];
    const double v24 = PSr[16];
    const double v25 = PSr[17];
    
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
