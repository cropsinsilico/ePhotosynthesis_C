/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/
\
#include "Variables.hpp"
#include "globals.hpp"

arr PR::PRmb(double t, PRCon &PR_con, Variables *myVars) {
    Condition(t, myVars);

    PRrate(t, PR_con, myVars);

    arr PRdydt = zeros(13);
    PRdydt[0] = myVars->PR_Vel.v1in - myVars->PR_Vel.v113;
    PRdydt[1] = myVars->PR_Vel.v112 - myVars->PR_Vel.v2out;
    PRdydt[2] = myVars->PR_Vel.v111 + myVars->PR_Vel.v113 - 0.5;
    PRdydt[2] = 0;
    PRdydt[3] = myVars->PR_Vel.v111 - myVars->PR_Vel.v112;
    PRdydt[4] = myVars->PR_Vel.v2out - myVars->PR_Vel.v121;
    PRdydt[5] = myVars->PR_Vel.v121 - myVars->PR_Vel.v122 - myVars->PR_Vel.v124;
    PRdydt[6] = myVars->PR_Vel.v131 - myVars->PR_Vel.v122;
    PRdydt[7] = myVars->PR_Vel.v122 + myVars->PR_Vel.v124 - 2 * myVars->PR_Vel.v131;
    PRdydt[8] = myVars->PR_Vel.v122 - myVars->PR_Vel.v123;
    PRdydt[9] = myVars->PR_Vel.v123 - myVars->PR_Vel.v1in;
    PRdydt[10] = 0.3 - myVars->PR_Vel.v111;
    PRdydt[11] = 0;
    PRdydt[12] = 0;

    return PRdydt;
}
