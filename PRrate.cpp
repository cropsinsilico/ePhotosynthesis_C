#include "Variables.hpp"
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


void PRrate(double t, PRCon &PR_con, Variables *myVars) {
    const double Gcea = PR_con.GCEA;
    const double Gca = PR_con.GCA;
    const double Pga = PR_con.PGA;
    const double Pgca = PR_con.PGCA;
    const double Gcac = PR_con.GCAc;
    const double Goac = PR_con.GOAc;
    const double Serc = PR_con.SERc;
    const double Glyc = PR_con.GLYc;
    const double Hprc = PR_con.HPRc;
    const double Gceac = PR_con.GCEAc;
    const double Rubp = PR_con.RUBP;
    double C = PR_con.CO2;
    double O = PR_con.O2;

    // To set global information for different reactions

    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
    const double PrV111 = myVars->V111;
    const double PrKO = myVars->KO;
    const double PrKC = myVars->KC;
    const double PrKR = myVars->KR;

    // Reaction: 112: PGlycolate-->Pi+Glycolate;
    const double PrV112 = myVars->V112;
    const double PrKM112 = myVars->KM112;  // Km112 for PGlycolate;
    const double PrKI1122 = myVars->KI1122;   // Inhibition constant for glycolate;
    const double PrKI1121 = myVars->KI1121;   // Inhibition constnat for Pi ( Competitive with PGlycolate)

    // Reaction 113  : Gla+ATP<-->ADP + PGA
    const double PrV113 = myVars->V113;
    const double PrKM1131 = myVars->KM1131;// Km for ATP;
    const double PrKM1132 = myVars->KM1132; // Km for Gla;
    const double PrKI113 = myVars->KI113; // Ki for PGA;
    const double PrKE113 = myVars->KE113;

    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
    const double PrV121 = myVars->V121;
    const double PrKM121 = myVars->KM121;

    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    const double PrV122 = myVars->V122;
    const double PrKM1221 = myVars->KM1221;  // Michaelis constant for glyoxylate;
    const double PrKM1222 = myVars->KM1222;    // Michaelis constant for serinie;
    const double PrKI1221 = myVars->KI1221;   // Inhibition constant for Glycine;
    const double PrKE122 = myVars->KE122;

    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    const double PrV123 = myVars->V123;
    const double PrKM123 = myVars->KM123;  // Michaelis constant for hydroxylpyruvate;
    const double PrKI123 = myVars->KI123;
    const double PrKE123 = myVars->KE123;


    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    const double PrV124 = myVars->V124;
    const double PrKM1241 = myVars->KM1241;  // Michaelis constant for glyoxylate
    const double PrKM1242 = myVars->KM1242;  // Michaelis constant for Glu
    const double PrKE124 = myVars->KE124;

    // Reaction 131: LS2+Glycine <--> CO2+ AMDHL
    const double PrV131 = myVars->V131;
    const double PrKM1311 = myVars->KM1311;// Michaelis constant for Glycine;
    const double PrKI1311 = myVars->KI1311; // Inibition constant for Serine;

    // The consant for calculating the glycerate uptake.
    const double PrKM1011 = myVars->KM1011;
    const double PrKI1011 = myVars->KI1011;
    const double PrV1T = myVars->V1T;

    // The constant for calculating the glycolate uptake
    const double PrKM1012 = myVars->KM1012;
    const double PrKI1012 = myVars->KI1012;
    const double PrV2T = myVars->V2T;

    // Calculate the reactino inside chloroplast;
    double Pi = myVars->PR_Param[1];    // Value from spinach. Assume constant currently.

    // Information from PS cycle is transfered back to the PR cycle for calcualtion.
    C = myVars->CO2_cond;
    O = myVars->O2_cond;

    double ATP;
    double ADP;
    if (myVars->PR_PS_com) {
        ADP = myVars->PS2PR_ADP;
        ATP = myVars->PS2PR_ATP;
        Pi = myVars->PS2PR_Pi;
    } else {
        ADP = myVars->PR_ADP;
        ATP = myVars->PR_ATP;
    }

    double v111;
    double PrV111t;
    if (myVars->RUBISCOMETHOD == 2) {
        if (myVars->PR_PS_com) {
            PrV111t = PrV111 * Rubp / (Rubp + PrKR * myVars->V1Reg);
        } else {
            PrV111t = PrV111 * Rubp / (Rubp + PrKR);
        }
        v111 = PrV111t * O / (O + PrKO * (1 + C / PrKC));

        if (Rubp < myVars->PsV1 / 2.5)
            v111 = v111 * (2.5 * Rubp / PrV111t);


    } else if (myVars->RUBISCOMETHOD == 1){
        v111 = PrV111 * O / (O + PrKO * (1 + C / PrKC));
        if (Rubp < myVars->RUBISCOTOTAL)
            v111 = v111 * Rubp / myVars->RUBISCOTOTAL;

    }
    const double v112 = PrV112 * Pgca / (Pgca + PrKM112 * (1 + Gca / PrKI1122) * (1 + Pi / PrKI1121));

    double v113;
    if (myVars->PR_PS_com) {
        v113 = PrV113 * (ATP * Gcea - ADP * Pga / PrKE113) / ((ATP + PrKM1131 * (1 + Pga / PrKI113)) * (Gcea + PrKM1132));// This is the old version.

    } else {
        v113 = PrV113 * (ATP * Gcea - ADP * Pga / PrKE113) / ((ATP + PrKM1131 * (1 + 2.5 / PrKI113)) * (Gcea + PrKM1132));// This is the old version.
    }

    const double v121 = PrV121 * Gcac / (Gcac + PrKM121);

    // The reaction: Goac + Serine --> HPR + Gly

    const double v122 = PrV122 * (Goac * Serc - Hprc * Glyc / PrKE122) / ((Goac + PrKM1221) * (Serc + PrKM1222 * (1 + Glyc / PrKI1221)));

    const double PrKM1232 = 0.5;

    const double v123 = PrV123 * (Hprc * myVars->NADHc - Gceac * myVars->NADc / PrKE123) / ((Hprc + PrKM123 * (1 + Hprc / PrKI123)) * (myVars->NADHc + PrKM1232));

    const double v124 = PrV124 * (Goac * myVars->GLUc - myVars->KGc * Glyc / PrKE124) / ((Goac + PrKM1241) * (myVars->GLUc + PrKM1242 * (1 + Glyc / PrKI1221)));

    const double v131 = PrV131 * Glyc / (Glyc + PrKM1311 * (1 + Serc / PrKI1311));

    const double v2out = PrV2T * (Gca / (Gca + PrKM1012 * (1 + Gcea / PrKI1012)) - Gcac / (Gcac + PrKM1012 * (1 + Gceac / PrKI1012)));// Competive inhibition

    const double v1in = PrV1T * (Gceac / (Gceac + PrKM1011 * (1 + Gcac / PrKI1011)) - Gcea / (Gcea + PrKM1011 * (1 + Gca / PrKI1011)));// Competive inhibition

    if (t > myVars->PR_OLD_TIME) {
            myVars->PR_TIME_N = myVars->PR_TIME_N + 1;
            myVars->PR_OLD_TIME = t;
        }
    // The following is used to take the information back to the PRmb routine.
    myVars->PR_Vel.v111 = v111;
    myVars->PR_Vel.v112 = v112;
    myVars->PR_Vel.v113 = v113;
    myVars->PR_Vel.v121 = v121;
    myVars->PR_Vel.v122 = v122;
    myVars->PR_Vel.v123 = v123;
    myVars->PR_Vel.v124 = v124;
    myVars->PR_Vel.v131 = v131;
    myVars->PR_Vel.v1in = v1in;
    myVars->PR_Vel.v2out = v2out;

    if (myVars->record) {
        myVars->PR_VEL.insert(myVars->PR_TIME_N - 1, t, myVars->PR_Vel);

        // here is some parameters we need to output

        myVars->PR2OUT[0] = Gcea;
        myVars->PR2OUT[1] = Gca;
        myVars->PR2OUT[2] = Pga;
        myVars->PR2OUT[3] = Pgca;
        myVars->PR2OUT[4] = Gcac;
        myVars->PR2OUT[5] = Goac;
        myVars->PR2OUT[6] = Serc;
        myVars->PR2OUT[7] = Glyc;
        myVars->PR2OUT[8] = Hprc;
        myVars->PR2OUT[9] = Gceac;
        myVars->PR2OUT[10] = Rubp;
        myVars->PR2OUT[11] = v131;
    }
}
