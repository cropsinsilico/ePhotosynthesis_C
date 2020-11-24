#include "globals.hpp"
#include "XanCycle.hpp"
#include "RA.hpp"
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

arr DynaPS::DynaPS_Ini() {
    
    // BEGIN = 1;// --unused
    
    //////////////////////////////////////////////////////////////////////////////////
    //   Clear up memory for simulation       //
    //////////////////////////////////////////////////////////////////////////////////
    
    //global DynaPS_OLD_TIME;
    //global DynaPS_TIME_N;
    //global DynaPS_VEL;
    //global DynaPS_CON;
    
    //myVars.DynaPS_OLD_TIME = 0;  // --unused
    //myVars.DynaPS_TIME_N = 1;  // --unused
    // DynaPS_VEL = zeros(1, 3);    // Clean memory
    // DynaPS_CON = zeros(3, 1);    // Clean memory
    
    
    RACon RA_con = RA_Ini(myVars);
    arr DynaPS_Con = zeros(120);
    arr RA_Con = RA_con.toArray();
    for (int m = 0; m < 92; m++)
        DynaPS_Con[m] = RA_Con[m];
    
    
    
    XanCycleCon XanCycle_Con = XanCycle_Ini(myVars);
    arr XanCycle_con = XanCycle_Con.toArray();
    for (int m = 0; m < 4; m++)
        DynaPS_Con[m + 92] = XanCycle_con[m];
    
    return DynaPS_Con;
}
