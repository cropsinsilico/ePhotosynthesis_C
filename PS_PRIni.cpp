#include "globals.hpp"
#include "PS_PR.hpp"
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


PS_PRCon PS_PRIni(varptr *myVars) {
    
    //arr PSs = zeros(5);
    //PSs = PSInitial(myVars);
    //arr PrS = PRinitial(myVars);
    PSCon PS_con = PSInitial(myVars);
    PRCon PR_con = PRinitial(myVars);
    arr PrS = PR_con.toArray();
    arr PSs = PS_con.toArray();
    PS_PRCon PS_PR_con(PS_con, PR_con);
    /*
    arr PS_PRs = zeros(24);
    
    for (int m = 0; m < 4; m++)
        PS_PRs[m] = PSs[m];
    
    
    for (int m = 4; m < 14; m++)
        PS_PRs[m] = PSs[m + 1];
    
    
    for (int m = 14; m < 16; m++)
        PS_PRs[m] = PrS[m - 14];
    
    
    for (int m = 16; m < 23; m++)
        PS_PRs[m] = PrS[m - 13];
    
    
    PS_PRs[23] = PSs[4];
     */
    std::cout << "PSS 4   " << PSs[4] << std::endl;
    return PS_PR_con;
}
