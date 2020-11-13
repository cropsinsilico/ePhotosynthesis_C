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





arr RedoxReg_mb(double t, arr &RedoxReg_Con, varptr *myVars) {
    
    
    //global trDynaPS2RedReg_cal
    myVars->trDynaPS2RedReg_cal = 1;
    
    arr RA_Con = zeros(92);
    for (int m = 0; m < 92; m++)
        RA_Con[m] = RedoxReg_Con[m];
    
    
    // ThioRe = RedoxReg_Con[92];// --unused
    
    arr RedoxReg_Vel = zeros(2);
    RedoxReg_Vel = RedoxReg_Rate(t, RedoxReg_Con, myVars);
    
    arr RA_DYDT = RA_mb(t, RA_Con, myVars);
    
    arr RedoxReg_DYDT = zeros(93);
    
    for (int index = 0; index < 92; index++)
        RedoxReg_DYDT[index] = RA_DYDT[index];
    
    
    const double vred = RedoxReg_Vel[0];
    const double vox = RedoxReg_Vel[1];
    
    RedoxReg_DYDT[92] = vred - vox;
    RedoxReg_DYDT[92] = 0;
    
    
    const double Temp = RedoxReg_DYDT[23];
    RedoxReg_DYDT[23] = Temp;
    return RedoxReg_DYDT;
}
