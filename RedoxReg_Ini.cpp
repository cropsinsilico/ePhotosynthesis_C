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



arr RedoxReg_Ini(varptr *myVars) {
    
    // BEGIN = 1;// --unused
    
    //global RedoxReg_OLD_TIME;
    //global RedoxReg_TIME_N;
    //global RedoxReg_VEL;
    //global RedoxReg_CON;
    
    myVars->RedoxReg_OLD_TIME = 0;
    myVars->RedoxReg_TIME_N = 1;
    // RedoxReg_VEL = zeros(1, 3);
    // RedoxReg_CON = zeros(3, 1);
    
    
    arr RA_Con = RA_Ini(myVars);
    arr RedoxReg_Con = zeros(93);
    
    for (int m = 0; m < 92; m++)
        RedoxReg_Con[m] = RA_Con[m];
    
    
    const double Thion = 0.25;     // This is a wild guess
    RedoxReg_Con[92] = Thion;             //
    
    
    //global RedoxReg_VMAX6;
    //global RedoxReg_VMAX9;
    //global RedoxReg_VMAX13;
    //global RedoxReg_VMAX16;
    
    //global V6;
    //global V9;
    //global V13;
    //global V16;
    
    myVars->RedoxReg_VMAX6 = myVars->V6;
    myVars->RedoxReg_VMAX9 = myVars->V9;
    myVars->RedoxReg_VMAX13 = myVars->V13;
    myVars->RedoxReg_VMAX16 = myVars->V16;
    
    //global RedoxReg_MP;
    for (int i = 0; i < 5; i++)
        myVars->RedoxReg_MP.push_back(zeros(3));
    myVars->RedoxReg_MP[0][0] = 1000;
    myVars->RedoxReg_MP[0][1] = - 0.3;
    myVars->RedoxReg_MP[0][2] = 0.5;
    
    myVars->RedoxReg_MP[1][0] = 6;             // FBPase
    myVars->RedoxReg_MP[1][1] = - 0.305;
    myVars->RedoxReg_MP[1][2] = 0.5;
    
    myVars->RedoxReg_MP[2][0] = 9;             // SBPase
    myVars->RedoxReg_MP[2][1] = - 0.3;
    myVars->RedoxReg_MP[2][2] = 0.5;
    
    myVars->RedoxReg_MP[3][0] = 13;            // PRK
    myVars->RedoxReg_MP[3][1] = - 0.295;
    myVars->RedoxReg_MP[3][2] = 0.5;
    
    myVars->RedoxReg_MP[4][0] = 16;            // ATPase
    myVars->RedoxReg_MP[4][1] = - 0.28;
    myVars->RedoxReg_MP[4][2] = 0.5;
    
    
    //global Thio_Oxidation;
    //global Fd_Thio_ET;
    
    //myVars->Thio_Oxidation = 0.1;   // constant set in globals.hpp
    //myVars->Fd_Thio_ET = 500;   // constant set in globals.hpp
    
    
    //global ThioT;
    //myVars->ThioT = 0.5;   // constant set in globals.hpp
    
    //global BF_Pool;
    //global BF2RedoxReg_Fdt;
    myVars->BF2RedoxReg_Fdt = myVars->BF_Pool[5];
    return RedoxReg_Con;
}
