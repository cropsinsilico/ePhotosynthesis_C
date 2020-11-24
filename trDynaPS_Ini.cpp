#include "globals.hpp"
#include "trDynaPS.hpp"
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



trDynaPSCon trDynaPS::trDynaPS_Ini() {
    
    // BEGIN = 1;// --unused
    
    //global trDynaPS_OLD_TIME;
    //global trDynaPS_TIME_N;
    //global trDynaPS_VEL;
    //global trDynaPS_CON;
    
    //myVars.trDynaPS_OLD_TIME = 0;  // --unused
    //myVars.trDynaPS_TIME_N = 1;  // --unused
    // trDynaPS_VEL = zeros(1, 3);    // Clean memory
    // trDynaPS_CON = zeros(3, 1);    // Clean memory
    
    // Now get the combined total concentration of different concentration variables.
    //DynaPS dps = DynaPS(myVars);
    DynaPSCon DynaPS_con = DynaPS_Init(myVars);
    //arr DynaPS_Con = dps.DynaPS_Ini();
    //arr temp = DynaPS_con.toArray();
    //DynaPS_Con.reserve(120);
    //arr DynaPS_Con = zeros(120);
    
    //for (int m = 0; m < 96; m++)
    //    DynaPS_Con[m] = temp[m];
    
    
    RROEACon RROEA_con = RROEA_Ini(myVars);
    //arr RROEA_Con = RROEA_con.toArray();
    //for (int m = 0; m < 10; m++)
    //    DynaPS_Con[m + 110] = RROEA_Con[m];
    trDynaPSCon trDynaPS_Con(DynaPS_con, RROEA_con);
    return trDynaPS_Con;
}
