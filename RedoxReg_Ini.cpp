#include "Variables.hpp"
#include "RedoxReg.hpp"
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

RedoxRegCon RedoxReg_Ini(Variables *myVars) {
    myVars->RedoxReg_OLD_TIME = 0;
    myVars->RedoxReg_TIME_N = 1;
    RACon RA_con = RA_Ini(myVars);

    const double Thion = 0.25;     // This is a wild guess
    RedoxRegCon RedoxReg_con(RA_con, Thion);

    myVars->RedoxReg_VMAX6 = myVars->V6;
    myVars->RedoxReg_VMAX9 = myVars->V9;
    myVars->RedoxReg_VMAX13 = myVars->V13;
    myVars->RedoxReg_VMAX16 = myVars->V16;

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

    myVars->BF2RedoxReg_Fdt = myVars->BF_Pool.kU_f;
    return RedoxReg_con;
}
