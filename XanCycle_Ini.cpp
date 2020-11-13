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

arr XanCycle_Ini(varptr *myVars) {
    //global XanRatio;
    //global XanCycle_kva;
    //global XanCycle_kaz;
    //global XanCycle_kza;
    //global XanCycle_kav;
    //global XanCycle_kvf;
    //global XanCycle_kv2ABA;
    //global XanCycle_kABAdg;
    
    myVars->XanCycle_kva = 0.163 / 60 * myVars->XanRatio[0];// Ruth Frommolt et a; 2001; Planta
    myVars->XanCycle_kaz = 0.691 / 60 * myVars->XanRatio[1];// Ruth Frommolt et a; 2001; Planta
    myVars->XanCycle_kza = 0.119 / 60 * myVars->XanRatio[2];// Ruth Frommolt et a; 2001; Planta
    myVars->XanCycle_kav = 0.119 / 60 * myVars->XanRatio[3];// Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
    
    //myVars->XanCycle_kvf = 0;            // This is the rate of formation of v from its precursors, reprenting the net generation of new V.  // --unused
    //myVars->XanCycle_kv2ABA = 0;         // This represent the rate constant of conversion from v to ABA. This is just a guess.  // --unused
    //myVars->XanCycle_kABAdg = 0;         // This represent the rate constant of conversion ABA degradation  // --unused
    
    const double Vx = 160;
    const double Ax = 10;
    const double Zx = 5;
    const double ABA = 1;
    arr XanCycle_Con = zeros(4);
    // Coeff = 0.37;// --unused
    
    XanCycle_Con[0] = Vx * 0.37;
    XanCycle_Con[1] = Ax * 0.37;
    XanCycle_Con[2] = Zx * 0.37;
    XanCycle_Con[3] = ABA;
    
    //global XanCycle_OLD_TIME;
    //global XanCycle_TIME_N;
    //global XanCycle_VEL;
    //global XanCycle_CON;
    
    myVars->XanCycle_OLD_TIME = 0;
    myVars->XanCycle_TIME_N = 1;
    
    // XanCycle_VEL = zeros(1, 4);    // Clean memory
    // XanCycle_CON = zeros(1, 4);    // Clean memory
    
    
    //global XanCycle2FIBF_Xstate;
    myVars->XanCycle2FIBF_Xstate = Zx / (Ax + Vx + Zx);
    
    return XanCycle_Con;
}
