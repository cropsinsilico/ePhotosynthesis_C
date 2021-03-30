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

#define KI124 2        // This KI is one guessed.
#define KM1312 0.075   // Michaelis constant for NAD;
#define KI1312 0.015   // Inhibition constant for NADH;    Since in the current program, we assume that P protein limit the
                       // rate of the overall glycin decarboxylase; the KI1312 and
                       // KM1312 were not used.

double PR::KC = 0.;
double PR::KE113 = 0.;
double PR::KE122 = 0.;
double PR::KE123 = 0.;
double PR::KE124 = 0.;
double PR::KGc = 0.;
double PR::KI1011 = 0.;
double PR::KI1012 = 0.;
double PR::KI1121 = 0.;
double PR::KI1122 = 0.;
double PR::KI113 = 0.;
double PR::KI1221 = 0.;
double PR::KI123 = 0.;
double PR::KI1311 = 0.;
double PR::KM1011 = 0.;
double PR::KM1012 = 0.;
double PR::KM112 = 0.;
double PR::KM1131 = 0.;
double PR::KM1132 = 0.;
double PR::KM121 = 0.;
double PR::KM1221 = 0.;
double PR::KM1222 = 0.;
double PR::KM123 = 0.;
double PR::KM1241 = 0.;
double PR::KM1242 = 0.;
double PR::KM1311 = 0.;
double PR::KO = 0.;
double PR::KR = 0.;
double PR::NADHc = 0.;
double PR::NADc = 0.;
double PR::PR_ADP = 0.;
double PR::PR_ATP = 0.;
double PR::V111 = 0.;
double PR::V112 = 0.;
double PR::V113 = 0.;
double PR::V121 = 0.;
double PR::V122 = 0.;
double PR::V123 = 0.;
double PR::V124 = 0.;
double PR::V131 = 0.;
double PR::V2T = 0.;
double PR::Vfactor112 = 0;
double PR::Vfactor113 = 0;
double PR::Vfactor121 = 0;
double PR::Vfactor122 = 0;
double PR::Vfactor123 = 0;
double PR::Vfactor124 = 0;
double PR::Vfactor131 = 0;


PRCon PR::PR_Ini(Variables *theVars) {
    if (theVars->useC3) {
        NADHc = 0.47;
        NADc = 0.4;
    } else {
        NADHc = 0.47 * theVars->PRRatio[8];
        NADc = 0.4 * theVars->PRRatio[9];
    }

    if (theVars->useC3) {
        theVars->GLUc = 24;
        KGc = 0.4;
    } else {
        theVars->GLUc = 24 * theVars->PRRatio[12];
        KGc = 0.4 * theVars->PRRatio[13];

        PR_ADP = 0.82 * theVars->PRRatio[14];
        PR_ATP = 0.68 * theVars->PRRatio[15];
    }

    PRCon PR_con;

    PR_con.GCEA = 0.1812;   // Glycerate in chloroplast; derived based on V113
    PR_con.GCA = 0.36;      // Derived from radioactive labelling experiment; assuem equal concenatration
                            // inside and outshide chloroplast
    if (theVars->useC3) {
        PR_con.PGA = 2.4;       // PGA in chloroplast;
    } else {
        PR_con.PGA = 4.3;       // PGA in chloroplast;4.3 is the original value;
    }
    PR_con.PGCA = 0.0029;   // Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;

    PR_con.GCAc = 0.36;     // See the note for GCA.
    PR_con.GOAc = 0.028;    // Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
    PR_con.SERc = 7.5;      // Serine in cytosol; 7.5 original value
    PR_con.GLYc = 1.8;      // Glycine in cytosol; 1.8 original vlaue
    PR_con.HPRc = 0.0035;   // HydroxylPyruvate; derived from equation 123;
    PR_con.GCEAc = 0.1812;  // Glycerate in cytosol; assume at equilibrium with GCEA initially.
    PR_con.RUBP = 2.;       // RuBP concentration
    PR_con.CO2 = 0.012;     // CO2 concentration(mM)
    PR_con.O2 = 0.264;      // O2 concentration(mM)


    if (theVars->useC3) {
        theVars->V1T = 5.;
        Vfactor112 = 1;
        Vfactor113 = 1;
        Vfactor121 = 1;
        Vfactor122 = 1;
        Vfactor123 = 1;
        Vfactor124 = 1;
        Vfactor131 = 1;
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0) {
            Vfactor112 = theVars->VfactorCp[25];
            Vfactor113 = theVars->VfactorCp[26];
            Vfactor121 = theVars->VfactorCp[28];
            Vfactor122 = theVars->VfactorCp[29];
            Vfactor123 = theVars->VfactorCp[30];
            Vfactor131 = theVars->VfactorCp[32];
            Vfactor124 = theVars->VfactorCp[11];
        }
        // The constant for calculating the glycolate uptake
        V2T = 6;      // The original value is 0.32.
        // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
        KO = PS::KM12;           // Michaelis constant for O2
        KC = PS::KM11;          // Michaelis constant for CO2
        KR = 0.02;           // Michaelis constant for RUBP

        // Reaction: 112: PGlycolate-->Pi+Glycolate;
        KM112 = 0.026;   // Km112 for PGlycolate;
        KI1122 = 94;     // Inhibition constant for Glycolate;
        KI1121 = 2.55;   // The competitive Pi inhibition for PGlycolate

        // Reaction 113  : Gcea+ATP<-->ADP + PGA
        KM1131 = 0.21;  // Km for ATP;
        KM1132 = 0.25;  // Km for Gcea;
        KI113 = 0.36;   // Ki for ATP BY pga;  %%%%%%%%%%%%%%%%%%%%%%%%% Competitive inhibition for ATP; in original paper it is 0.36;
        KE113 = 300;     // New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics

        // To set global information for different reactions
        KM121 = 0.1;//Glycolate +O2<-->H2O2+Glyoxylate

        KM1221 = 0.15; // Michaelis constant for glyoxylate;
        KM1222 = 2.7;  // Michaelis constant for serinie;
        KI1221 = 33;   // Inhibition constant for Glycine;
        KE122 = 0.24;  //  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.

        KM123 = 0.09;       //   Michaelis constant for hydroxylpyruvate;
        KI123 = 12;          // Inhibition constant for hydroxypyruvate;
        KE123 = 1/(4*pow(10, -6));  // Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 1/(4*10^(-6);

        KM1241 = 0.15; // Michaelis constant for glyoxylate
        KM1242 = 1.7;  // Michaelis constant for Glu
        KE124 = 607;   // New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K 607.

        KM1311 = 6;// Michaelis constant for Glycine;
        KI1311 = 4; // Inhibition constant for Serine



        KM1011 = 0.39;
        KI1011 = 0.28;

        KM1012 = 0.2;
        KI1012 = 0.22;

        //C = CO2_cond;
        //O = O2_cond;

    } else {
        // To set global information for different reactions
        // Reaction: 110: RuBP + CO2 <--> 2PGA

        const double CE = 1; // This is the coefficient for calibrating the volume effect // Default is 4.

        // Reaction: 111: RUBP+O2<-->PGlycolate + PGA

        KO = 0.222 * theVars->PRRatio[16];  // Michaelis constant for O2
        KC = 0.0115 * theVars->PRRatio[17]; // Michaelis constant for CO2

        if (theVars->PR_PS_com) {
            KC = theVars->KM11;
            KO = theVars->KM12;
        }

        KR = 0.02 * theVars->PRRatio[18]; // Michaelis constant for RUBP

        // Reaction: 112: PGlycolate-->Pi+Glycolate;

        KM112 = 0.026 * theVars->PRRatio[19]; // Km112 for PGlycolate;
        KI1122 = 94 * theVars->PRRatio[20];   // Inhibition constant for Glycolate;
        KI1121 = 2.55 * theVars->PRRatio[21]; // The competitive Pi inhibition for PGlycolate


        // Reaction 113  : Gcea+ATP<-->ADP + PGA
        KM1131 = 0.21 * theVars->PRRatio[22]; // Km for ATP;
        KM1132 = 0.25 * theVars->PRRatio[23]; // Km for Gcea;
        KI113 = 0.36 * theVars->PRRatio[24];  // Competitive inhibition for ATP; in original paper it is 0.36;
        KE113 = 300 * theVars->PRRatio[25];   // New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics  300, as default


        // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
        KM121 = 0.1 * theVars->PRRatio[26];

        // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
        KM1221 = 0.15 * theVars->PRRatio[27]; // Michaelis constant for glyoxylate;
        KM1222 = 2.7 * theVars->PRRatio[28];  // Michaelis constant for serinie;
        KI1221 = 33 * theVars->PRRatio[29];   // Inhibition constant for Glycine;
        KE122 = 0.24 * theVars->PRRatio[30];  //  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.

        // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate

        KM123 = 0.09 * theVars->PRRatio[31];  //   Michaelis constant for hydroxylpyruvate;
        KI123 = 12 * theVars->PRRatio[32];    // Inhibition constant for hydroxypyruvate;
        KE123 = 1 / (4 * pow(10, (-6))) * theVars->PRRatio[33];

        // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
        KM1241 = 0.15 * theVars->PRRatio[34]; // Michaelis constant for glyoxylate
        KM1242 = 1.7 * theVars->PRRatio[35];  // Michaelis constant for Glu
        KE124 = 607 * theVars->PRRatio[37];   // New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K' 607.

        // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
        KM1311 = 6 * theVars->PRRatio[38];  // Michaelis constant for Glycine;
        KI1311 = 4 * theVars->PRRatio[39];  // Inhibition constant for Serine

        // The consant for calculating the glycerate uptake.
        theVars->V1T = 0.25 * CE * 20 * theVars->PRRatio[42];
        KM1011 = 0.39 * theVars->PRRatio[43];
        KI1011 = 0.28 * theVars->PRRatio[44];

        // The constant for calculating the glycolate output
        V2T = 0.32 * CE * 10 * 2 * theVars->PRRatio[45];
        KM1012 = 0.2 * theVars->PRRatio[46];
        KI1012 = 0.22 * theVars->PRRatio[47];
    }
    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
    // Reaction: 112: PGlycolate-->Pi+Glycolate;
    // Reaction 113  : Gcea+ATP<-->ADP + PGA
    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3

    if (theVars->GP == 0) {
        if (theVars->useC3) {
            V111 = theVars->V1 * 0.22;
            V112 = theVars->EnzymeAct.at("V112");
            V113 = theVars->EnzymeAct.at("V113");
            V121 = theVars->EnzymeAct.at("V121");
            V122 = theVars->EnzymeAct.at("V122");
            V123 = theVars->EnzymeAct.at("V123");
            V124 = theVars->EnzymeAct.at("V124");
            V131 = theVars->EnzymeAct.at("V131");

        } else {
            if (theVars->PR_PS_com) {
                V111 = 0.24 * theVars->V1 * theVars->PRRatio[0];
            } else {
                V111 = 3.7 * 0.24 * 1;
            }
            V112 = 52.41992121 * theVars->PRRatio[1];
            V113 = 5.715787563 * theVars->PRRatio[2];
            V121 = 1.456108923 * theVars->PRRatio[3];
            V122 = 3.306190845 * 3 * theVars->PRRatio[4];
            V123 = 10.00978112 * theVars->PRRatio[5];
            V124 = 2.745819515 * theVars->PRRatio[6];
            V131 = 2.494745448 * theVars->PRRatio[7];
        }
    }

    return PR_con;
}
