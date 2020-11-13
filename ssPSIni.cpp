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

void ssPSIni(double t, varptr *myVars) {
    
    //global kmCO2;
    //global kmO2;
    //global O2;
    //global GammaStar;
    //global Rd;
    
    const double RT = 0.0083 * (273 + t);
    
    //myVars->Rd = exp(18.72 - 46.39 / RT);  // --unused
    myVars->GammaStar = exp(19.02 - 37.83 / RT);
    myVars->kmCO2 = exp(38.05 - 79.43 / RT);
    myVars->kmO2 = exp(20.30 - 36.38 / RT);
    //myVars->O2 = 210;   // constant set in globals.hpp
    
    //const double Done = 1;
    //return Done;
}
