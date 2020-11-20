#include "globals.hpp"
#include "PS_PR.hpp"
#include "SUCS.hpp"
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



arr CM::CM_Ini() {
    
    arr PS_PRs = PS_PRIni(myVars);
    //PS_PRCon PS_PR_con = PS_PRIni(myVars);
    //arr PS_PRs = PS_PR_con.toArray();
    arr CMs = zeros(36);
    
    for (int m = 0; m < 23; m++)
        CMs[m] = PS_PRs[m];
    
    
    SUCSCon SUCS_Con = SUCS_Ini(myVars);
    arr SUCSc = SUCS_Con.toArray();
    // The gap left is for later use.
    
    for (int m = 0; m < 12; m++)
        CMs[23 + m] = SUCSc[m];
    
    
    CMs[35] = PS_PRs[23];
    return CMs;
}
