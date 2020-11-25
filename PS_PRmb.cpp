#include "Variables.hpp"
#include "PS_PR.hpp"
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

    PSRate(t, PS_con, PS_Param, myVars);

    arr PR_Param = zeros(2);
    PR_Param[0] = myVars->PS_PR_Param;                  // To indicate that the calcualtion is using the combined model
    // for the PS-PR combined model. 0: Combined model; 1: Separate model
    PR_Param[1] = myVars->PS2PR_Pi;

    PRrate(t, PR_con, myVars);

    // Assign the rate of reaction that is calculated from the photosynthesis and photorespiration routine.
    const double v1 = myVars->PS_Vel.v1;
    const double v2 = myVars->PS_Vel.v2;
    const double v3 = myVars->PS_Vel.v3;
    const double v5 = myVars->PS_Vel.v5;
    const double v6 = myVars->PS_Vel.v6;
    const double v7 = myVars->PS_Vel.v7;
    const double v8 = myVars->PS_Vel.v8;
    const double v9 = myVars->PS_Vel.v9;
    const double v10 = myVars->PS_Vel.v10;
    const double v13 = myVars->PS_Vel.v13;
    const double v16 = myVars->PS_Vel.v16;
    const double v23 = myVars->PS_Vel.v23;
    const double v31 = myVars->PS_Vel.v31;
    const double v32 = myVars->PS_Vel.v32;
    const double v33 = myVars->PS_Vel.v33;
    const double v24 = myVars->PS_Vel.v24;
    const double v25 = myVars->PS_Vel.v25;

    const double v111 = myVars->PR_Vel.v111;
    const double v112 = myVars->PR_Vel.v112;
    const double v113 = myVars->PR_Vel.v113;
    const double v121 = myVars->PR_Vel.v121;
    const double v122 = myVars->PR_Vel.v122;
    const double v123 = myVars->PR_Vel.v123;
    const double v124 = myVars->PR_Vel.v124;
    const double v131 = myVars->PR_Vel.v131;
    const double v1in = myVars->PR_Vel.v1in;
    const double v2out = myVars->PR_Vel.v2out;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 5. This part exchange informations from two systems.//
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    myVars->PSPR2RA_v1 = v1;
    myVars->PSPR2RA_v13 = v13;
    myVars->PSPR2RA_v111 = v111;
    myVars->PRGlu = v124;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 6.  Calculation of the mass balance equations ////
    //////////////////////////////////////////////////////////////////////////////////////////////$

    arr tmp = zeros(24);


    tmp[0] = v13 - v1 - v111;


    tmp[1] = 2 * v1 - v2 - v32 + v113 + v111;


    tmp[2] = v2 - v3;


    tmp[3] = v3 - 2 * v5 - v7 - v8 - v10 - v31 - v33;


    tmp[4] = v5 - v6;


    tmp[5] = v7 - v8;


    tmp[6] = v9 - v10;


    tmp[7] = v8 - v9;

    tmp[8] = v16 - v2 - v23 - v13 - v113 - v25 - v131 - vATPcost;//WY202003 extra ATP cost
    //tmp(9) = v16 - v2 - v23 - v13- v113 - v25 - v124;    //AWY201804


    tmp[9] = 0;


    tmp[10] = 0;


    tmp[11] = 0;


    tmp[12] = v6 - v7 - v23 + v25;


    tmp[13] = v7 + v10 * 2 - v13;


    tmp[14] = v1in - v113;


    tmp[15] = v112 - v2out;


    tmp[16] = v111 - v112;


    tmp[17] = v2out - v121;


    tmp[18] = v121 - v122 - v124;


    tmp[19] = v131 - v122;


    tmp[20] = v122 + v124 - 2 * v131;

    tmp[21] = v122 - v123;
    tmp[22] = v123 - v1in;

    tmp[23] = v23 - v24;


    arr PS_PR_DYDT = tmp;
    return PS_PR_DYDT;
}
