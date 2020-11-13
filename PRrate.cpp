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


arr PRrate(double t, arr &PrS, arr &PR_Param, varptr &myVars) {
    
    //global NADHc;
    //global NADc;
    //global ADPc;
    //global ATPc;
    //global GLUc;
    //global KGc;
    
    
    //global PR_OLD_TIME;
    //global PR_TIME_N;
    //global PR_VEL;
    //global PR_CON;
    
    const double Gcea = PrS[0];
    const double Gca = PrS[1];
    const double Pga = PrS[2];
    const double Pgca = PrS[3];
    const double Gcac = PrS[4];
    const double Goac = PrS[5];
    const double Serc = PrS[6];
    const double Glyc = PrS[7];
    const double Hprc = PrS[8];
    const double Gceac = PrS[9];
    const double Rubp = PrS[10];
    double C = PrS[11];
    double O = PrS[12];
    
    // To set global information for different reactions
    
    
    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
    //global V111;
    //global KO;
    //global KC;
    //global KR;
    
    const double PrV111 = myVars.V111;
    const double PrKO = myVars.KO;
    const double PrKC = myVars.KC;
    const double PrKR = myVars.KR;
    
    // Reaction: 112: PGlycolate-->Pi+Glycolate;
    //global V112;
    //global KM112;       // Km112 for PGlycolate;
    //global KI1122;       // Inhibition constant for glycolate;
    //global KI1121;      // Inhibition constnat for Pi ( Competitive with PGlycolate)
    const double PrV112 = myVars.V112;
    const double PrKM112 = myVars.KM112;
    const double PrKI1122 = myVars.KI1122;
    const double PrKI1121 = myVars.KI1121;
    
    // Reaction 113  : Gla+ATP<-->ADP + PGA
    //global V113;
    //global KM1131;  // Km for ATP;
    //global KM1132;  // Km for Gla;
    //global KI113;   // Ki for PGA;
    //global KE113;
    
    const double PrV113 = myVars.V113;
    const double PrKM1131 = myVars.KM1131;
    const double PrKM1132 = myVars.KM1132;
    const double PrKI113 = myVars.KI113;
    const double PrKE113 = myVars.KE113;
    
    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
    //global V121;
    //global KM121;
    const double PrV121 = myVars.V121;
    const double PrKM121 = myVars.KM121;
    
    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    //global V122;
    //global KM1221;      // Michaelis constant for glyoxylate;
    //global KM1222;      // Michaelis constant for serinie;
    //global KI1221;      // Inhibition constant for Glycine;
    //global KE122;
    
    const double PrV122 = myVars.V122;
    const double PrKM1221 = myVars.KM1221;
    const double PrKM1222 = myVars.KM1222;
    const double PrKI1221 = myVars.KI1221;
    const double PrKE122 = myVars.KE122;
    
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    //global V123;
    //global KM123;       // Michaelis constant for hydroxylpyruvate;
    //global KI123;
    //global KE123;
    
    const double PrV123 = myVars.V123;
    const double PrKM123 = myVars.KM123;
    const double PrKI123 = myVars.KI123;
    const double PrKE123 = myVars.KE123;
    
    
    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    //global V124;
    //global KM1241;  // Michaelis constant for glyoxylate
    //global KM1242;  // Michaelis constant for Glu
    //global KI124;   // This is a guessed value
    //global KE124;
    
    const double PrV124 = myVars.V124;
    const double PrKM1241 = myVars.KM1241;
    const double PrKM1242 = myVars.KM1242;
    const double PrKI124 = myVars.KI124;
    const double PrKE124 = myVars.KE124;
    
    // Reaction 131: LS2+Glycine <--> CO2+ AMDHL
    //global V131;
    //global KM1311;   // Michaelis constant for Glycine;
    //global KM1312;   // Michaelis constant for NAD;
    //global KI1311;   // Inibition constant for Serine;
    //global KI1312;   // Inhibition constant for NADH;
    
    const double PrV131 = myVars.V131;
    const double PrKM1311 = myVars.KM1311;
    const double PrKM1312 = myVars.KM1312;
    const double PrKI1311 = myVars.KI1311;
    const double PrKI1312 = myVars.KI1312;
    
    // Reaction 132; AMDHL + THF <--> MTHF+DHLA + NH3;
    //global V132;
    //global KM1321;  // Michaelis constant for AMDHL;
    //global KM1322;  // Michaelis constant for THF;
    const double PrV132 = myVars.V132;
    const double PrKM1321 = myVars.KM1321;
    const double PrKM1322 = myVars.KM1322;
    
    // Reaction 133: DHLA + MAD <--> NADH + LS2;
    //global V133;
    //global KM1331;  // Michaelis constant for NAD;
    //global KM1332;  // Michaelis constant for DHLA;
    //global KI1331;	// Inhibition constant for NADH;
    //global KI1332;	// Inhibition constant for LS2;
    
    const double PrV133 = myVars.V133;
    const double PrKM1331 = myVars.KM1331;
    const double PrKM1332 = myVars.KM1332;
    const double PrKI1331 = myVars.KI1331;
    const double PrKI1332 = myVars.KI1332;
    
    // The consant for calculating the glycerate uptake.
    //global V1T;
    //global KM1011;
    //global KI1011;
    const double PrKM1011 = myVars.KM1011;
    const double PrKI1011 = myVars.KI1011;
    const double PrV1T = myVars.V1T;
    
    // The constant for calculating the glycolate uptake
    //global V2T;
    //global KM1012;
    //global KI1012;
    const double PrKM1012 = myVars.KM1012;
    const double PrKI1012 = myVars.KI1012;
    const double PrV2T = myVars.V2T;
    
    // Calculate the reactino inside chloroplast;
    double Pi = PR_Param[1];    // Value from spinach. Assume constant currently.
    
    // Information from PS cycle is transfered back to the PR cycle for calcualtion.
    //global PR_PS_com;
    
    //global PS2PR_Pi;
    //global CO2_cond;
    //global O2_cond;
    
    //global PR_ADP;         // The chloroplast ADP concentration
    //global PR_ATP;         // The chloroplast ATP concentration
    //global PS2PR_ATP;
    //global PS2PR_ADP;
    //global PS2PR_Pi;
    
    C = myVars.CO2_cond;
    O = myVars.O2_cond;
    
    double ATP;
    double ADP;
    if (myVars.PR_PS_com) {
        ADP = myVars.PS2PR_ADP;
        ATP = myVars.PS2PR_ATP;
        Pi = myVars.PS2PR_Pi;
        
        //global myVars.StomCond_TrDynaPS_com;
        if (myVars.StomCond_TrDynaPS_com) {
            //global myVars.PS2PRC;
            //global myVars.PS2PRO;
            C = myVars.PS2PRC;
            O = myVars.PS2PRO;
        }
    } else {
        ADP = myVars.PR_ADP;
        ATP = myVars.PR_ATP;
    }
    
    //global V1Reg;       // This is a parameter generated from PSRate routine.
    //global RUBISCOMETHOD;
    //global RUBISCOTOTAL;
    
    //global FIBF_PSPR_com;
    //global EPS_ATP_Rate;
    //global ATPActive;
    //global PsV1;
    
    //if (myVars.FIBF_PSPR_com) {
    //    if (myVars.ATPActive == 0)
    //        PrV111 = PrV111;
    //    
    //}
    //fprintf("PSPR   //f  //f\n",RUBISCOMETHOD,  RUBISCOTOTAL);
    double v111;
    double PrV111t;
    PrV111t;
    if (myVars.RUBISCOMETHOD == 2) {
        //fprintf("PRPS   //f\n", myVars.PR_PS_com);
        if (myVars.PR_PS_com) {
            PrV111t = PrV111 * Rubp / (Rubp + PrKR * myVars.V1Reg);
        } else {
            PrV111t = PrV111 * Rubp / (Rubp + PrKR);
        }
        v111 = PrV111t * O / (O + PrKO * (1 + C / PrKC));
        
        if (Rubp < myVars.PsV1 / 2.5)
            v111 = v111 * (2.5 * Rubp / PrV111t);
        
        
    } else if (myVars.RUBISCOMETHOD == 1){
        v111 = PrV111 * O / (O + PrKO * (1 + C / PrKC));
        if (Rubp < myVars.RUBISCOTOTAL)
            v111 = v111 * Rubp / myVars.RUBISCOTOTAL;
        
    }
    //global PRRatio;
    //fprintf("XX //f  //f  //f  //f  //f  //f  //f  //f  //f\n", PrV111, Rubp, PrKR, V1Reg, O, PrKO, C, PrKC, PR_PS_com);
    const double v112 = PrV112 * Pgca / (Pgca + PrKM112 * (1 + Gca / PrKI1122) * (1 + Pi / PrKI1121));
    
    double v113;
    if (myVars.PR_PS_com) {
        v113 = PrV113 * (ATP * Gcea - ADP * Pga / PrKE113) / ((ATP + PrKM1131 * (1 + Pga / PrKI113)) * (Gcea + PrKM1132));// This is the old version.
        
    } else {
        v113 = PrV113 * (ATP * Gcea - ADP * Pga / PrKE113) / ((ATP + PrKM1131 * (1 + 2.5 / PrKI113)) * (Gcea + PrKM1132));// This is the old version.
    }
    
    // Ks = 0.4;       // From Ferjancic-Biagini et al 1998// --unused
    // Ki = 5;         // From Ferjancic-Biagini et al 1998// --unused
    // Kip = 14;       // From Ferjancic-Biagini et al 1998// --unused
    const double v121 = PrV121 * Gcac / (Gcac + PrKM121);
    
    // The reaction: Goac + Serine --> HPR + Gly
    
    const double v122 = PrV122 * (Goac * Serc - Hprc * Glyc / PrKE122) / ((Goac + PrKM1221) * (Serc + PrKM1222 * (1 + Glyc / PrKI1221)));
    
    const double PrKM1232 = 0.5;
    
    const double v123 = PrV123 * (Hprc * myVars.NADHc - Gceac * myVars.NADc / PrKE123) / ((Hprc + PrKM123 * (1 + Hprc / PrKI123)) * (myVars.NADHc + PrKM1232));
    
    const double v124 = PrV124 * (Goac * myVars.GLUc - myVars.KGc * Glyc / PrKE124) / ((Goac + PrKM1241) * (myVars.GLUc + PrKM1242 * (1 + Glyc / PrKI1221)));
    
    const double v131 = PrV131 * Glyc / (Glyc + PrKM1311 * (1 + Serc / PrKI1311));
    
    //fid = fopen('v131.txt', 'a+');
    //fprintf(fid, '//f     //f     //f\n', t, Rubp, v131);
    //fclose(fid);
    const double v2out = PrV2T * (Gca / (Gca + PrKM1012 * (1 + Gcea / PrKI1012)) - Gcac / (Gcac + PrKM1012 * (1 + Gceac / PrKI1012)));// Competive inhibition
    
    const double v1in = PrV1T * (Gceac / (Gceac + PrKM1011 * (1 + Gcac / PrKI1011)) - Gcea / (Gcea + PrKM1011 * (1 + Gca / PrKI1011)));// Competive inhibition
    
    
    if (myVars.PR_TIME_N == 0)
        myVars.PR_TIME_N = 1;
    
    
    if (t > myVars.PR_OLD_TIME) {
        myVars.PR_TIME_N = myVars.PR_TIME_N + 1;
        myVars.PR_OLD_TIME = t;
    }
    
    if (myVars.PR_VEL.shape()[0] < myVars.PR_TIME_N) {
        myVars.PR_VEL.resize(boost::extents[myVars.PR_TIME_N][PR_VEL_SIZE]);
    }

    myVars.PR_VEL[myVars.PR_TIME_N - 1][0] = t;
    
    myVars.PR_VEL[myVars.PR_TIME_N - 1][1] = v111;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][2] = v112;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][3] = v113;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][4] = v121;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][5] = v122;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][6] = v123;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][7] = v124;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][8] = v131;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][9] = v1in;
    myVars.PR_VEL[myVars.PR_TIME_N - 1][10] = v2out;
    
    // The following is used to take the information back to the PRmb routine.
    
    arr Velocity = zeros(10);
    
    Velocity[0] = v111;
    Velocity[1] = v112;
    Velocity[2] = v113;
    Velocity[3] = v121;
    Velocity[4] = v122;
    Velocity[5] = v123;
    Velocity[6] = v124;
    Velocity[7] = v131;
    Velocity[8] = v1in;
    Velocity[9] = v2out;
    
    
    // here is some parameters we need to output
    //global PR2OUT;
    // PR2OUT = zeros(5, 1);
    
    myVars.PR2OUT[0] = Gcea;
    myVars.PR2OUT[1] = Gca;
    myVars.PR2OUT[2] = Pga;
    myVars.PR2OUT[3] = Pgca;
    myVars.PR2OUT[4] = Gcac;
    myVars.PR2OUT[5] = Goac;
    myVars.PR2OUT[6] = Serc;
    myVars.PR2OUT[7] = Glyc;
    myVars.PR2OUT[8] = Hprc;
    myVars.PR2OUT[9] = Gceac;
    myVars.PR2OUT[10] = Rubp;
    myVars.PR2OUT[11] = v131;
    return Velocity;
}
