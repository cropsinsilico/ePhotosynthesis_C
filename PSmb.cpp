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

#include "Variables.hpp"
#include "globals.hpp"

arr PS::PSmb(double t, PSCon &PS_con, arr &Param, Variables *myVars) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifying KM, KI, KE VMAX for different reactions as the regulation//
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Regulations first.
    Condition(t, myVars);

    // Get the rate for the reactions in the photosynthesis sytem
    PSRate(t, PS_con, Param, myVars);

    // Get the rate

    // Implement the mass balance equations

    arr PSdydt = zeros(15);
    PSdydt[0] = myVars->PS_Vel.v13 - myVars->PS_Vel.v1;
    PSdydt[1] = 2 * myVars->PS_Vel.v1 - myVars->PS_Vel.v2 - myVars->PS_Vel.v32;
    PSdydt[2] = myVars->PS_Vel.v2 - myVars->PS_Vel.v3;
    PSdydt[3] = myVars->PS_Vel.v3 - 2 * myVars->PS_Vel.v5 - myVars->PS_Vel.v7 - myVars->PS_Vel.v8 - myVars->PS_Vel.v10 - myVars->PS_Vel.v31 - myVars->PS_Vel.v33;
    PSdydt[4] = myVars->PS_Vel.v23 - myVars->PS_Vel.v24;
    PSdydt[5] = myVars->PS_Vel.v5 - myVars->PS_Vel.v6;
    PSdydt[6] = myVars->PS_Vel.v7 - myVars->PS_Vel.v8;
    PSdydt[7] = myVars->PS_Vel.v9 - myVars->PS_Vel.v10;
    PSdydt[8] = myVars->PS_Vel.v8 - myVars->PS_Vel.v9;
    PSdydt[9] = myVars->PS_Vel.v16 - myVars->PS_Vel.v2 - myVars->PS_Vel.v23 - myVars->PS_Vel.v13 - myVars->PS_Vel.v25;
    PSdydt[10] = 0;
    PSdydt[11] = 0;
    PSdydt[12] = 0;
    PSdydt[13] = myVars->PS_Vel.v6 - myVars->PS_Vel.v7 - myVars->PS_Vel.v23 + myVars->PS_Vel.v25;
    PSdydt[14] = myVars->PS_Vel.v7 + myVars->PS_Vel.v10 * 2 - myVars->PS_Vel.v13;

    return PSdydt;
}
