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




arr PS_PRmb(double t, arr &PS_PRs, double PS_PR_Param, varptr &myVars) {
    //global TestATPCost;
    //global AVR;
    
    //global CO2_cond;
    //global O2_cond;
    //global PS_C_CP;
    
    const double vATPcost = myVars.TestATPCost / myVars.AVR;
    
    arr PSs = zeros(15);
    arr PrS = zeros(13);
    
    for (int m = 0; m < 4; m++)
        PSs[m] = PS_PRs[m];
    
    
    for (int m = 4; m < 14; m++)
        PSs[m + 1] = PS_PRs[m];
    
    
    PSs[4] = PS_PRs[23];
    
    for (int m = 14; m < 16; m++)
        PrS[m - 14] = PS_PRs[m];
    
    PrS[2] = PS_PRs[1];
    
    for (int m = 16; m < 23; m++)
        PrS[m - 13] = PS_PRs[m];
    
    
    
    PrS[10] = PS_PRs[0];            // RUBP
    PrS[11] = PS_PRs[10];           // CO2
    PrS[12] = PS_PRs[11];           // O2
    
    const double PR2PS_Pgca = PrS[3];            // FOr transfering information between PR to PS.
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 2. Add exprimental conditions here; Conditions like light, temperature, CO2, O2 concentration should be added here //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    const double fini = Condition(t, myVars);
    
    
    arr PR_v = zeros(10);
    arr PS_v = zeros(18);
    
    arr PS_Param = zeros(2);
    PS_Param[0] = PS_PR_Param;
    PS_Param[1] = PR2PS_Pgca;
    
    PS_v = PSRate(t, PSs, PS_Param, myVars);
    
    arr PR_Param = zeros(2);
    PR_Param[0] = PS_PR_Param;                  // To indicate that the calcualtion is using the combined model
    // for the PS-PR combined model. 0: Combined model; 1: Separate model
    //global PS2PR_Pi;
    PR_Param[1] = myVars.PS2PR_Pi;
    
    PR_v = PRrate(t, PrS, PR_Param, myVars);
    //throw(MException(""));
    //global PR2OUT;
    //global mRuBP;
    //fid = fopen('v1.txt','a+');
    //fprintf(fid, '//f    //f     //f\n', t, mRuBP, (PS_v(1)-PR2OUT(12))*30.0);
    //fclose(fid);
    // Assign the rate of reaction that is calculated from the photosynthesis and photorespiration routine.
    
    const double v1 = PS_v[0];
    const double v2 = PS_v[1];
    const double v3 = PS_v[2];
    // v4 = PS_v[3];// --unused
    const double v5 = PS_v[4];
    const double v6 = PS_v[5];
    const double v7 = PS_v[6];
    const double v8 = PS_v[7];
    const double v9 = PS_v[8];
    const double v10 = PS_v[9];
    const double v13 = PS_v[10];
    const double v16 = PS_v[11];
    const double v23 = PS_v[12];
    const double v31 = PS_v[13];
    const double v32 = PS_v[14];
    const double v33 = PS_v[15];
    const double v24 = PS_v[16];
    const double v25 = PS_v[17];
    
    const double v111 = PR_v[0];
    const double v112 = PR_v[1];
    const double v113 = PR_v[2];
    const double v121 = PR_v[3];
    const double v122 = PR_v[4];
    const double v123 = PR_v[5];
    const double v124 = PR_v[6];
    const double v131 = PR_v[7];
    const double v1in = PR_v[8];
    const double v2out = PR_v[9];
    
    //fid = fopen('Mymatrix3.txt','a');
    //fprintf(fid, '//f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f   //f\n',t, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v13, v16, v23, v31, v32, v33, v24, v25, v111, v112, v113, v121, v122, v123, v124, v131, v1in, v2out);
    //fprintf(fid, '\n');
    //fclose(fid);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 5. This part exchange informations from two systems.//
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //global PSPR2RA_v1;
    //global PSPR2RA_v13;
    //global PSPR2RA_v111;
    //global PRGlu;
    myVars.PSPR2RA_v1 = v1;
    myVars.PSPR2RA_v13 = v13;
    myVars.PSPR2RA_v111 = v111;
    myVars.PRGlu = v124;
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
