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
#include "PS_PR.hpp"
#include "globals.hpp"

arr PS_PRmb(double t, PS_PRCon &PS_PR_con, Variables *myVars) {

    const double vATPcost = myVars->TestATPCost / myVars->AVR;

    PRCon PR_con = PS_PR_con.PR_con;
    PSCon PS_con = PS_PR_con.PS_con;
    PR_con.PGA = PS_con.PGA;

    PR_con.RUBP = PS_con.RuBP;
    PR_con.CO2 = PS_con.CO2;
    PR_con.O2 = PS_con.O2;

    const double PR2PS_Pgca = PS_PR_con.PR_con.PGCA; //PrS[3];            // FOr transfering information between PR to PS.

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 2. Add exprimental conditions here; Conditions like light, temperature, CO2, O2 concentration should be added here //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Condition(t, myVars);

    arr PS_Param = zeros(2);
    PS_Param[0] = myVars->PS_PR_Param;
    PS_Param[1] = PR2PS_Pgca;

    PS::PSRate(t, PS_con, PS_Param, myVars);

    arr PR_Param = zeros(2);
    PR_Param[0] = myVars->PS_PR_Param;                  // To indicate that the calcualtion is using the combined model
    // for the PS-PR combined model. 0: Combined model; 1: Separate model
    PR_Param[1] = myVars->PS2PR_Pi;

    PR::PRrate(t, PR_con, myVars);

    // Assign the rate of reaction that is calculated from the photosynthesis and photorespiration routine.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 5.  Calculation of the mass balance equations ////
    //////////////////////////////////////////////////////////////////////////////////////////////$

    arr PS_PR_DYDT = zeros(24);
    PS_PR_DYDT[0] = myVars->PS_Vel.v13 - myVars->PS_Vel.v1 - myVars->PR_Vel.v111;
    PS_PR_DYDT[1] = 2 * myVars->PS_Vel.v1 - myVars->PS_Vel.v2 - myVars->PS_Vel.v32 + myVars->PR_Vel.v113 + myVars->PR_Vel.v111;
    PS_PR_DYDT[2] = myVars->PS_Vel.v2 - myVars->PS_Vel.v3;
    PS_PR_DYDT[3] = myVars->PS_Vel.v3 - 2 * myVars->PS_Vel.v5 - myVars->PS_Vel.v7 - myVars->PS_Vel.v8 - myVars->PS_Vel.v10 - myVars->PS_Vel.v31 - myVars->PS_Vel.v33;
    PS_PR_DYDT[4] = myVars->PS_Vel.v5 - myVars->PS_Vel.v6;
    PS_PR_DYDT[5] = myVars->PS_Vel.v7 - myVars->PS_Vel.v8;
    PS_PR_DYDT[6] = myVars->PS_Vel.v9 - myVars->PS_Vel.v10;
    PS_PR_DYDT[7] = myVars->PS_Vel.v8 - myVars->PS_Vel.v9;
    PS_PR_DYDT[8] = myVars->PS_Vel.v16 - myVars->PS_Vel.v2 - myVars->PS_Vel.v23 - myVars->PS_Vel.v13 - myVars->PR_Vel.v113 - myVars->PS_Vel.v25 - myVars->PR_Vel.v131 - vATPcost;//WY202003 extra ATP cost
    //PS_PR_DYDT(9) = myVars->PS_Vel.v16 - myVars->PS_Vel.v2 - myVars->PS_Vel.v23 - myVars->PS_Vel.v13- myVars->PR_Vel.v113 - myVars->PS_Vel.v25 - myVars->PR_Vel.v124;    //AWY201804
    PS_PR_DYDT[9] = 0;
    PS_PR_DYDT[10] = 0;
    PS_PR_DYDT[11] = 0;
    PS_PR_DYDT[12] = myVars->PS_Vel.v6 - myVars->PS_Vel.v7 - myVars->PS_Vel.v23 + myVars->PS_Vel.v25;
    PS_PR_DYDT[13] = myVars->PS_Vel.v7 + myVars->PS_Vel.v10 * 2 - myVars->PS_Vel.v13;
    PS_PR_DYDT[14] = myVars->PR_Vel.v1in - myVars->PR_Vel.v113;
    PS_PR_DYDT[15] = myVars->PR_Vel.v112 - myVars->PR_Vel.v2out;
    PS_PR_DYDT[16] = myVars->PR_Vel.v111 - myVars->PR_Vel.v112;
    PS_PR_DYDT[17] = myVars->PR_Vel.v2out - myVars->PR_Vel.v121;
    PS_PR_DYDT[18] = myVars->PR_Vel.v121 - myVars->PR_Vel.v122 - myVars->PR_Vel.v124;
    PS_PR_DYDT[19] = myVars->PR_Vel.v131 - myVars->PR_Vel.v122;
    PS_PR_DYDT[20] = myVars->PR_Vel.v122 + myVars->PR_Vel.v124 - 2 * myVars->PR_Vel.v131;
    PS_PR_DYDT[21] = myVars->PR_Vel.v122 - myVars->PR_Vel.v123;
    PS_PR_DYDT[22] = myVars->PR_Vel.v123 - myVars->PR_Vel.v1in;
    PS_PR_DYDT[23] = myVars->PS_Vel.v23 - myVars->PS_Vel.v24;

    return PS_PR_DYDT;
}
