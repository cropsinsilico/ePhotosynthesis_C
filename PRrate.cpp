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
#include <stdexcept>
#include "Variables.hpp"

void PRrate(double t, PRCon &PR_con, Variables *myVars) {
    // To set global information for different reactions

    // Reaction: 111: RUBP+O2<-->PGlycolate + PGA

    // Reaction: 112: PGlycolate-->Pi+Glycolate;

    // Reaction 113  : Gla+ATP<-->ADP + PGA

    // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate

    // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate

    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;

    // Reaction 131: LS2+Glycine <--> CO2+ AMDHL

    // Calculate the reactino inside chloroplast;
    double Pi = myVars->PR_Param[1];    // Value from spinach. Assume constant currently.

    // Information from PS cycle is transfered back to the PR cycle for calcualtion.

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

    double PrV111t;
    if (myVars->RUBISCOMETHOD == 2) {
        if (myVars->PR_PS_com) {
            PrV111t = myVars->V111 * PR_con.RUBP / (PR_con.RUBP + myVars->KR * myVars->V1Reg);
        } else {
            PrV111t = myVars->V111 * PR_con.RUBP / (PR_con.RUBP + myVars->KR);
        }
        myVars->PR_Vel.v111 = PrV111t * myVars->O2_cond / (myVars->O2_cond + myVars->KO * (1 + myVars->CO2_cond / myVars->KC));

        if (PR_con.RUBP < myVars->PsV1 / 2.5)
            myVars->PR_Vel.v111 = myVars->PR_Vel.v111 * (2.5 * PR_con.RUBP / PrV111t);


    } else if (myVars->RUBISCOMETHOD == 1){
        myVars->PR_Vel.v111 = myVars->V111 * myVars->O2_cond / (myVars->O2_cond + myVars->KO * (1 + myVars->CO2_cond / myVars->KC));
        if (PR_con.RUBP < myVars->RUBISCOTOTAL)
            myVars->PR_Vel.v111 = myVars->PR_Vel.v111 * PR_con.RUBP / myVars->RUBISCOTOTAL;

    } else {
        throw std::invalid_argument("Invalid value of RUBISCOMETHOD used");
    }

    //double v113;
    if (myVars->PR_PS_com) {
        myVars->PR_Vel.v113 = myVars->V113 * (ATP * PR_con.GCEA - ADP * PR_con.PGA / myVars->KE113) / ((ATP + myVars->KM1131 * (1 + PR_con.PGA / myVars->KI113)) * (PR_con.GCEA + myVars->KM1132));// This is the old version.

    } else {
        myVars->PR_Vel.v113 = myVars->V113 * (ATP * PR_con.GCEA - ADP * PR_con.PGA / myVars->KE113) / ((ATP + myVars->KM1131 * (1 + 2.5 / myVars->KI113)) * (PR_con.GCEA + myVars->KM1132));// This is the old version.
    }


    // The reaction: PR_con.GOAc + Serine --> HPR + Gly

    const double PrKM1232 = 0.5;

    if (t > myVars->PR_OLD_TIME) {
            myVars->PR_TIME_N = myVars->PR_TIME_N + 1;
            myVars->PR_OLD_TIME = t;
        }
    // The following is used to take the information back to the PRmb routine.
    myVars->PR_Vel.v112 = myVars->V112 * PR_con.PGCA / (PR_con.PGCA + myVars->KM112 * (1 + PR_con.GCA / myVars->KI1122) * (1 + Pi / myVars->KI1121));
    myVars->PR_Vel.v121 = myVars->V121 * PR_con.GCAc / (PR_con.GCAc + myVars->KM121);
    myVars->PR_Vel.v122 = myVars->V122 * (PR_con.GOAc * PR_con.SERc - PR_con.HPRc * PR_con.GLYc / myVars->KE122) / ((PR_con.GOAc + myVars->KM1221) * (PR_con.SERc + myVars->KM1222 * (1 + PR_con.GLYc / myVars->KI1221)));
    myVars->PR_Vel.v123 = myVars->V123 * (PR_con.HPRc * myVars->NADHc - PR_con.GCEAc * myVars->NADc / myVars->KE123) / ((PR_con.HPRc + myVars->KM123 * (1 + PR_con.HPRc / myVars->KI123)) * (myVars->NADHc + PrKM1232));
    myVars->PR_Vel.v124 = myVars->V124 * (PR_con.GOAc * myVars->GLUc - myVars->KGc * PR_con.GLYc / myVars->KE124) / ((PR_con.GOAc + myVars->KM1241) * (myVars->GLUc + myVars->KM1242 * (1 + PR_con.GLYc / myVars->KI1221)));
    myVars->PR_Vel.v131 = myVars->V131 * PR_con.GLYc / (PR_con.GLYc + myVars->KM1311 * (1 + PR_con.SERc / myVars->KI1311));
    myVars->PR_Vel.v1in = myVars->V1T * (PR_con.GCEAc / (PR_con.GCEAc + myVars->KM1011 * (1 + PR_con.GCAc / myVars->KI1011)) - PR_con.GCEA / (PR_con.GCEA + myVars->KM1011 * (1 + PR_con.GCA / myVars->KI1011)));// Competive inhibition
    myVars->PR_Vel.v2out = myVars->V2T * (PR_con.GCA / (PR_con.GCA + myVars->KM1012 * (1 + PR_con.GCEA / myVars->KI1012)) - PR_con.GCAc / (PR_con.GCAc + myVars->KM1012 * (1 + PR_con.GCEAc / myVars->KI1012)));// Competive inhibition

    if (myVars->record) {
        myVars->PR_VEL.insert(myVars->PR_TIME_N - 1, t, myVars->PR_Vel);

        // here is some parameters we need to output

        myVars->PR2OUT[0] = PR_con.GCEA;
        myVars->PR2OUT[1] = PR_con.GCA;
        myVars->PR2OUT[2] = PR_con.PGA;
        myVars->PR2OUT[3] = PR_con.PGCA;
        myVars->PR2OUT[4] = PR_con.GCAc;
        myVars->PR2OUT[5] = PR_con.GOAc;
        myVars->PR2OUT[6] = PR_con.SERc;
        myVars->PR2OUT[7] = PR_con.GLYc;
        myVars->PR2OUT[8] = PR_con.HPRc;
        myVars->PR2OUT[9] = PR_con.GCEAc;
        myVars->PR2OUT[10] = PR_con.RUBP;
        myVars->PR2OUT[11] = myVars->PR_Vel.v131;
    }
}
