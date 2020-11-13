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



arr PRinitial(varptr &myVars) {
    //global PRRatio;
    //global NADHc;
    //global NADc;
    //global ADPc;
    //global ATPc;
    //global GLUc;
    //global KGc;
    //global PR_ADP;
    //global PR_ATP;
    
    myVars.NADHc = 0.47 * myVars.PRRatio[8];
    myVars.NADc = 0.4 * myVars.PRRatio[9];
    
    myVars.ADPc = 0.64 * myVars.PRRatio[10];
    myVars.ATPc = 0.35 * myVars.PRRatio[11];
    myVars.GLUc = 24 * myVars.PRRatio[12];
    myVars.KGc = 0.4 * myVars.PRRatio[13];
    
    myVars.PR_ADP = 0.82 * myVars.PRRatio[14];
    myVars.PR_ATP = 0.68 * myVars.PRRatio[15];
    
    const double SERc = 7.5;                  // Serine in cytosol; 7.5 original value
    const double GLYc = 1.8;                 // Glycine in cytosol; 1.8 original vlaue
    const double PGA = 4.3;                  // PGA in chloroplast;4.3 is the original value;
    
    const double GOAc = 0.028;              // Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
    
    const double GCAc = 0.36;                   // See the note for GCA.
    const double GCA = 0.36;                    // Derived from radioactive labelling experiment; assuem equal concenatration
    // inside and outshide chloroplast
    
    const double PGCA = 0.0029;                // Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;
    const double GCEA = 0.1812;                  // Glycerate in chloroplast; derived based on V113
    const double GCEAc = 0.1812;                 // Glycerate in cytosol; assume at equilibrium with GCEA initially.
    const double HPRc = 0.0035;                // HydroxylPyruvate; derived from equation 123;
    const double RUBP = 2;                   // RuBP concentration
    
    const double CO2 = 0.012;                 // CO2 concentration(mM)
    const double O2 = 0.264;                  // O2 concentration(mM)
    
    arr PrS = zeros(13);
    
    PrS[0] = GCEA;
    PrS[1] = GCA;
    PrS[2] = PGA;
    PrS[3] = PGCA;
    
    PrS[4] = GCAc;
    PrS[5] = GOAc;
    PrS[6] = SERc;
    PrS[7] = GLYc;
    PrS[8] = HPRc;
    PrS[9] = GCEAc;
    PrS[10] = RUBP;
    PrS[11] = CO2;
    PrS[12] = O2;
    
    // To set global information for different reactions
    // Reaction: 110: RuBP + CO2 <--> 2PGA
    
    const double CE = 1; // This is the coefficient for calibrating the volume effect // Default is 4.
    // CEV111 = 1;    // 1.72 was used to// --unused
    // CE122 = 1;// --unused
    
    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
    //global V111;
    //global KO;
    //global KC;
    //global KR;
    //global PR_PS_com;
    //global PS2PR_V1;
    
    
    myVars.KO = 0.222 * myVars.PRRatio[16];// Michaelis constant for O2
    myVars.KC = 0.0115 * myVars.PRRatio[17];// Michaelis constant for CO2
    
    if (myVars.PR_PS_com) {
        //global	myVars.KM11;
        //global	myVars.KM12;
        myVars.KC = myVars.KM11;
        myVars.KO = myVars.KM12;
    }
    
    myVars.KR = 0.02 * myVars.PRRatio[18];// Michaelis constant for RUBP
    
    // Reaction: 112: PGlycolate-->Pi+Glycolate;
    //global V112;
    //global KM112;       // Km112 for PGlycolate;
    //global KI1122;      // Inhibition constant for Glycolate;
    //global KI1121;      // The competitive Pi inhibition for PGlycolate
    
    myVars.KM112 = 0.026 * myVars.PRRatio[19];
    myVars.KI1122 = 94 * myVars.PRRatio[20];
    myVars.KI1121 = 2.55 * myVars.PRRatio[21];
    
    
    // Reaction 113  : Gcea+ATP<-->ADP + PGA
    //global V113;
    //global KM1131;  // Km for ATP;
    //global KM1132;  // Km for Gcea;
    //global KI113;   // Ki for ATP BY pga;
    //global  KE113;  // New
    myVars.KM1131 = 0.21 * myVars.PRRatio[22];
    myVars.KM1132 = 0.25 * myVars.PRRatio[23];
    myVars.KI113 = 0.36 * myVars.PRRatio[24];////////////////////////////////////////////////// Competitive inhibition for ATP; in original paper it is 0.36;
    myVars.KE113 = 300 * myVars.PRRatio[25];// New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics  300, as default
    
    
    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
    //global V121;
    //global KM121;
    
    myVars.KM121 = 0.1 * myVars.PRRatio[26];
    
    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    //global V122;
    //global KM1221;      // Michaelis constant for glyoxylate;
    //global KM1222;      // Michaelis constant for serinie;
    //global KI1221;      // Inhibition constant for Glycine;
    //global KE122;       // New
    myVars.KM1221 = 0.15 * myVars.PRRatio[27];
    myVars.KM1222 = 2.7 * myVars.PRRatio[28];
    myVars.KI1221 = 33 * myVars.PRRatio[29];
    myVars.KE122 = 0.24 * myVars.PRRatio[30];//  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.
    
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    //global V123;
    //global KM123;           //   Michaelis constant for hydroxylpyruvate;
    //global KI123;
    //global KE123;   // New
    
    myVars.KM123 = 0.09 * myVars.PRRatio[31];
    myVars.KI123 = 12 * myVars.PRRatio[32];// Inhibition constant for hydroxypyruvate;
    myVars.KE123 = 1 / (4 * pow(10, (-6))) * myVars.PRRatio[33];
    
    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    //global V124;
    //global KM1241;  // Michaelis constant for glyoxylate
    //global KM1242;  // Michaelis constant for Glu
    //global KI124;   // This KI is one guessed.
    //global KE124;   // New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K' 607.
    
    
    myVars.KM1241 = 0.15 * myVars.PRRatio[34];
    myVars.KM1242 = 1.7 * myVars.PRRatio[35];
    myVars.KI124 = 2 * myVars.PRRatio[36];// This is a guessed vlaue        ???????????????? To be calibrated.
    myVars.KE124 = 607 * myVars.PRRatio[37];
    
    // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
    //global V131;
    //global KM1311;   // Michaelis constant for Glycine;
    //global KI1311;   // Inhibition constant for Serine
    
    myVars.KM1311 = 6 * myVars.PRRatio[38];
    myVars.KI1311 = 4 * myVars.PRRatio[39];
    
    //global KI1312;   // Inhibition constant for NADH;    Since in the current program, we assume that P protein limit the
    // rate of the overall glycin decarboxylase; the KI1312 and KM1312 were not used.
    //global KM1312;   // Michaelis constant for NAD;
    myVars.KM1312 = 0.075 * myVars.PRRatio[40];
    myVars.KI1312 = 0.015 * myVars.PRRatio[41];
    
    // The consant for calculating the glycerate uptake.
    //global V1T;
    //global KM1011;
    //global KI1011;
    
    myVars.V1T = 0.25 * CE * 20 * myVars.PRRatio[42];
    myVars.KM1011 = 0.39 * myVars.PRRatio[43];
    myVars.KI1011 = 0.28 * myVars.PRRatio[44];
    
    // The constant for calculating the glycolate output
    //global V2T;
    //global KM1012;
    //global KI1012;
    myVars.V2T = 0.32 * CE * 10 * 2 * myVars.PRRatio[45];
    myVars.KM1012 = 0.2 * myVars.PRRatio[46];
    myVars.KI1012 = 0.22 * myVars.PRRatio[47];
    
    
    //global gp2V111;
    myVars.V111 = myVars.gp2V111;
    
    //global GP;
    if (myVars.GP == 0) {
        if (myVars.PR_PS_com) {
            //fprintf("Y\n");
            myVars.V111 = 0.24 * myVars.PS2PR_V1 * myVars.PRRatio[0];
            
    } else {
            //fprintf("Z\n");
            myVars.V111 = 3.7 * 0.24 * 1;
        }
        myVars.V112 = 52.41992121 * myVars.PRRatio[1];
        myVars.V113 = 5.715787563 * myVars.PRRatio[2];
        myVars.V121 = 1.456108923 * myVars.PRRatio[3];
        myVars.V122 = 3.306190845 * 3 * myVars.PRRatio[4];
        myVars.V123 = 10.00978112 * myVars.PRRatio[5];
        myVars.V124 = 2.745819515 * myVars.PRRatio[6];
        myVars.V131 = 2.494745448 * myVars.PRRatio[7];
    }
    
    return PrS;
}
