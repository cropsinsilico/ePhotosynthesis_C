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




arr PRmb(double t, arr &PrS, varptr *myVars) {
    
    Condition(t, myVars);
    
    arr Velocity = zeros(10);
    Velocity = PRrate(t, PrS, myVars);
    
    const double v111 = Velocity[0];
    const double v112 = Velocity[1];
    const double v113 = Velocity[2];
    const double v121 = Velocity[3];
    const double v122 = Velocity[4];
    const double v123 = Velocity[5];
    const double v124 = Velocity[6];
    const double v131 = Velocity[7];
    const double v1in = Velocity[8];
    const double v2out = Velocity[9];
    
    
    arr tmp = zeros(13);
    
    
    tmp[0] = v1in - v113;
    
    
    tmp[1] = v112 - v2out;
    
    
    tmp[2] = v111 + v113 - 0.5;
    
    tmp[2] = 0;
    
    
    tmp[3] = v111 - v112;
    
    
    tmp[4] = v2out - v121;
    
    
    
    tmp[5] = v121 - v122 - v124;
    
    
    tmp[6] = v131 - v122;
    
    
    tmp[7] = v122 + v124 - 2 * v131;
    
    
    tmp[8] = v122 - v123;
    
    
    tmp[9] = v123 - v1in;
    
    
    tmp[10] = 0.3 - v111;
    
    
    tmp[11] = 0;
    
    
    tmp[12] = 0;
    
    
    arr PR_DYDT = tmp;
    return PR_DYDT;
}
