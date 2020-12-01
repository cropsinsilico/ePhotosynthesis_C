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

PRCon PR::PRinitial(Variables *myVars) {
    NADHc = 0.47 * myVars->PRRatio[8];
    NADc = 0.4 * myVars->PRRatio[9];

    myVars->GLUc = 24 * myVars->PRRatio[12];
    KGc = 0.4 * myVars->PRRatio[13];

    PR_ADP = 0.82 * myVars->PRRatio[14];
    PR_ATP = 0.68 * myVars->PRRatio[15];

    PRCon PR_con;

    PR_con.GCEA = 0.1812;       // Glycerate in chloroplast; derived based on V113
    PR_con.GCA = 0.36;       // Derived from radioactive labelling experiment; assuem equal concenatration
                             // inside and outshide chloroplast
    PR_con.PGA = 4.3;     // PGA in chloroplast;4.3 is the original value;
    PR_con.PGCA = 0.0029;       // Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;

    PR_con.GCAc = 0.36;      // See the note for GCA.
    PR_con.GOAc = 0.028;     // Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
    PR_con.SERc = 7.5;             // Serine in cytosol; 7.5 original value
    PR_con.GLYc = 1.8;           // Glycine in cytosol; 1.8 original vlaue
    PR_con.HPRc = 0.0035;       // HydroxylPyruvate; derived from equation 123;
    PR_con.GCEAc = 0.1812;      // Glycerate in cytosol; assume at equilibrium with GCEA initially.
    PR_con.RUBP = 2.;       // RuBP concentration
    PR_con.CO2 = 0.012;         // CO2 concentration(mM)
    PR_con.O2 = 0.264;         // O2 concentration(mM)



    // To set global information for different reactions
    // Reaction: 110: RuBP + CO2 <--> 2PGA

    const double CE = 1; // This is the coefficient for calibrating the volume effect // Default is 4.

    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA

    KO = 0.222 * myVars->PRRatio[16];// Michaelis constant for O2
    KC = 0.0115 * myVars->PRRatio[17];// Michaelis constant for CO2

    if (myVars->PR_PS_com) {
        KC = myVars->KM11;
        KO = myVars->KM12;
    }

    KR = 0.02 * myVars->PRRatio[18];// Michaelis constant for RUBP

    // Reaction: 112: PGlycolate-->Pi+Glycolate;

    KM112 = 0.026 * myVars->PRRatio[19];// Km112 for PGlycolate;
    KI1122 = 94 * myVars->PRRatio[20];// Inhibition constant for Glycolate;
    KI1121 = 2.55 * myVars->PRRatio[21];// The competitive Pi inhibition for PGlycolate


    // Reaction 113  : Gcea+ATP<-->ADP + PGA
    KM1131 = 0.21 * myVars->PRRatio[22];// Km for ATP;
    KM1132 = 0.25 * myVars->PRRatio[23];// Km for Gcea;
    KI113 = 0.36 * myVars->PRRatio[24];////////////////////////////////////////////////// Competitive inhibition for ATP; in original paper it is 0.36;
    KE113 = 300 * myVars->PRRatio[25];// New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics  300, as default


    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
    KM121 = 0.1 * myVars->PRRatio[26];

    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    KM1221 = 0.15 * myVars->PRRatio[27]; // Michaelis constant for glyoxylate;
    KM1222 = 2.7 * myVars->PRRatio[28];// Michaelis constant for serinie;
    KI1221 = 33 * myVars->PRRatio[29]; // Inhibition constant for Glycine;
    KE122 = 0.24 * myVars->PRRatio[30];//  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.

    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate

    KM123 = 0.09 * myVars->PRRatio[31];//   Michaelis constant for hydroxylpyruvate;
    KI123 = 12 * myVars->PRRatio[32];// Inhibition constant for hydroxypyruvate;
    KE123 = 1 / (4 * pow(10, (-6))) * myVars->PRRatio[33];

    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    KM1241 = 0.15 * myVars->PRRatio[34]; // Michaelis constant for glyoxylate
    KM1242 = 1.7 * myVars->PRRatio[35];// Michaelis constant for Glu
    KE124 = 607 * myVars->PRRatio[37];// New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K' 607.

    // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
    KM1311 = 6 * myVars->PRRatio[38]; // Michaelis constant for Glycine;
    KI1311 = 4 * myVars->PRRatio[39];  // Inhibition constant for Serine

    // The consant for calculating the glycerate uptake.
    myVars->V1T = 0.25 * CE * 20 * myVars->PRRatio[42];
    KM1011 = 0.39 * myVars->PRRatio[43];
    KI1011 = 0.28 * myVars->PRRatio[44];

    // The constant for calculating the glycolate output
    V2T = 0.32 * CE * 10 * 2 * myVars->PRRatio[45];
    KM1012 = 0.2 * myVars->PRRatio[46];
    KI1012 = 0.22 * myVars->PRRatio[47];

    if (myVars->GP == 0) {
        if (myVars->PR_PS_com) {
            V111 = 0.24 * myVars->V1 * myVars->PRRatio[0];

    } else {
            V111 = 3.7 * 0.24 * 1;
        }
        V112 = 52.41992121 * myVars->PRRatio[1];
        V113 = 5.715787563 * myVars->PRRatio[2];
        V121 = 1.456108923 * myVars->PRRatio[3];
        V122 = 3.306190845 * 3 * myVars->PRRatio[4];
        V123 = 10.00978112 * myVars->PRRatio[5];
        V124 = 2.745819515 * myVars->PRRatio[6];
        V131 = 2.494745448 * myVars->PRRatio[7];
    }

    return PR_con;
}
