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

double TargetFunVal(varptr *myVars) {
    
    //global PS_VEL;
    //global PR_VEL;
    
    const double PSVCOEFF = 30;

    for (int i = 1; i < myVars->PS_VEL.shape()[0]; i++)    
        for (int y = 0; y < PS_VEL_SIZE; y++)
            myVars->PS_VEL[i][y] *= PSVCOEFF;
    
    // PSVEL = PS_VEL';// --unused
    
    //global VolRatioStCyto
    double ratio;
    if (myVars->VolRatioStCyto == 1) {
        ratio = PSVCOEFF;
    } else {
        ratio = PSVCOEFF * 4 / 9;
    }
    for (int i = 0; i < myVars->PR_VEL.shape()[0]; i++)
        for (int j = 0; j < PR_VEL_SIZE; j++)
            myVars->PR_VEL[i][j] *= ratio;
    
    
    
    const size_t* n = myVars->PS_VEL.shape();
    const double a = myVars->PS_VEL[1][n[1]];
    const double b = myVars->PR_VEL[n[1]][8];
    
    const double CO2AR = a - b;
    
    return CO2AR;
}
