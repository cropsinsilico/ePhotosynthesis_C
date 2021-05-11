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
#define KI124 2
void PR::PR_Rate(const double t, const PRCon* PR_con, Variables *theVars) {
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

    if (theVars->useC3) {

        const double PrV112 = V112 * Vfactor112 * Vf_T112 * pow(Q10_112, (theVars->Tp - 25) / 10);
        const double PrV113 = V113 * Vfactor113 * Vf_T113 * pow(Q10_113, (theVars->Tp - 25) / 10);
        const double PrV121 = V121 * Vfactor121 * Vf_T121 * pow(Q10_121, (theVars->Tp - 25) / 10);
        const double PrV122 = V122 * Vfactor122 * Vf_T122 * pow(Q10_122, (theVars->Tp - 25) / 10);
        const double PrV123 = V123 * Vfactor123 * Vf_T123 * pow(Q10_123, (theVars->Tp - 25) / 10);
        const double PrV124 = V124 * Vfactor124 * pow(Q10_124, (theVars->Tp - 25) / 10);
        const double PrV131 = V131 * Vfactor131 * Vf_T131 * pow(Q10_131, (theVars->Tp - 25) / 10);
        const double PrV111 = PS::PsV1 * 0.24;

       if (theVars->PR_PS_com == 1) {
            KC= PS::KM11;
            KO= PS::KM12;
        }

        if (theVars->RUBISCOMETHOD == 2) {            // Using michelies and enzyme information
            double PrV111t;
            if (theVars->PR_PS_com == 1) {       // FOr the combined PS-PR model
                PrV111t = PrV111*PR_con->RuBP / (PR_con->RuBP + KR * theVars->V1Reg);
            } else {                    // For the PR model
                PrV111t = PrV111*PR_con->RuBP / (PR_con->RuBP + KR);
            }
            theVars->PR_Vel.v111 = PrV111t * theVars->O2_cond / (theVars->O2_cond + KO * (1 + theVars->CO2_cond / KC));

        } else if (theVars->RUBISCOMETHOD == 1) {
            theVars->PR_Vel.v111 = PrV111 * theVars->O2_cond / (theVars->O2_cond + KO * (1 + theVars->CO2_cond / KC));
            if (PR_con->RuBP < theVars->RUBISCOTOTAL)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * PR_con->RuBP / theVars->RUBISCOTOTAL;

        }

        theVars->PR_Vel.v112 = PrV112 * PR_con->PGCA / (PR_con->PGCA + KM112 * (1 + PR_con->GCA / KI1122) * (1 + theVars->Pi / KI1121));

         if (theVars->PR_PS_com == 1) {            // For the combined PS-PR MODEL
             theVars->PR_Vel.v113 = PrV113 * (PR_con->parent->PS_con->ATP * PR_con->GCEA - theVars->ADP * PR_con->PGA / KE113) / ((PR_con->parent->PS_con->ATP + KM1131 * (1 + PR_con->PGA / KI113)) * (PR_con->GCEA + KM1132));
         } else {
             theVars->PR_Vel.v113 = PrV113 * (PR_con->parent->PS_con->ATP * PR_con->GCEA - theVars->ADP * PR_con->PGA / KE113) / ((PR_con->parent->PS_con->ATP + KM1131 * (1 + 2.5 / KI113)) * (PR_con->GCEA + KM1132));
         }

        theVars->PR_Vel.v121 = PrV121 * PR_con->GCAc / (PR_con->GCAc + KM121);

        theVars->PR_Vel.v122 = PrV122 * (PR_con->GOAc * PR_con->SERc - PR_con->HPRc * PR_con->GLYc / KE122) / ((PR_con->GOAc + KM1221) * (PR_con->SERc + KM1222 * (1 + PR_con->GLYc / KI1221)));
        theVars->PR_Vel.v123 = PrV123 * (PR_con->HPRc * NADHc - PR_con->GCEA * NADc / KE123) / (PR_con->HPRc + KM123 * (1 + PR_con->HPRc / KI123));
        theVars->PR_Vel.v124 = PrV124 * (PR_con->GOAc * theVars->GLUc-KGc * PR_con->GLYc/KE124)/((PR_con->GOAc + KM1241)*(theVars->GLUc + KM1242 * (1 + PR_con->GLYc / KI124)));

        theVars->PR_Vel.v131 = PrV131 * PR_con->GLYc/(PR_con->GLYc + KM1311*(1 + PR_con->SERc / KI1311));
        theVars->PR_Vel.v2out = V2T * (PR_con->GCA/(PR_con->GCA + KM1012*(1 + PR_con->GCEA / KI1012)) - PR_con->GCAc /(PR_con->GCAc + KM1012 * (1 + PR_con->GCEAc / KI1012)));   // Competive inhibition
        theVars->PR_Vel.v1in = theVars->V1T *(PR_con->GCEAc / (PR_con->GCEAc + KM1011 * (1 + PR_con->GCAc / KI1011)) -PR_con->GCEA /(PR_con->GCEA + KM1011 * (1 + PR_con->GCA / KI1011)));  // Competive inhibition


    } else {
        theVars->Pi = theVars->PR_Param[1];    // Value from spinach. Assume constant currently.

        // Information from PS cycle is transfered back to the PR cycle for calcualtion.

        double ATP;
        double ADP;
        if (theVars->PR_PS_com) {
            ADP = theVars->ADP;
            ATP = PR_con->parent->PS_con->ATP;
        } else {
            ADP = PR_ADP;
            ATP = PR_ATP;
        }

        double PrV111t;
        if (theVars->RUBISCOMETHOD == 2) {
            if (theVars->PR_PS_com) {
                PrV111t = V111 * PR_con->RuBP / (PR_con->RuBP + KR * theVars->V1Reg);
            } else {
                PrV111t = V111 * PR_con->RuBP / (PR_con->RuBP + KR);
            }
            theVars->PR_Vel.v111 = PrV111t * theVars->O2_cond / (theVars->O2_cond + KO * (1 + theVars->CO2_cond / KC));

            if (PR_con->RuBP < PS::PsV1 / 2.5)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * (2.5 * PR_con->RuBP / PrV111t);


        } else if (theVars->RUBISCOMETHOD == 1){
            theVars->PR_Vel.v111 = V111 * theVars->O2_cond / (theVars->O2_cond + KO * (1 + theVars->CO2_cond / KC));
            if (PR_con->RuBP < theVars->RUBISCOTOTAL)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * PR_con->RuBP / theVars->RUBISCOTOTAL;

        } else {
            throw std::invalid_argument("Invalid value of RUBISCOMETHOD used");
        }

        //double v113;
        if (theVars->PR_PS_com) {
            theVars->PR_Vel.v113 = V113 * (ATP * PR_con->GCEA - ADP * PR_con->PGA / KE113) / ((ATP + KM1131 * (1 + PR_con->PGA / KI113)) * (PR_con->GCEA + KM1132));// This is the old version.

        } else {
            theVars->PR_Vel.v113 = V113 * (ATP * PR_con->GCEA - ADP * PR_con->PGA / KE113) / ((ATP + KM1131 * (1 + 2.5 / KI113)) * (PR_con->GCEA + KM1132));// This is the old version.
        }


        // The reaction: PR_con->GOAc + Serine --> HPR + Gly

        const double PrKM1232 = 0.5;

        if (t > theVars->PR_OLD_TIME) {
            theVars->PR_TIME_N = theVars->PR_TIME_N + 1;
            theVars->PR_OLD_TIME = t;
        }
        // The following is used to take the information back to the PRmb routine.
        theVars->PR_Vel.v112 = V112 * PR_con->PGCA / (PR_con->PGCA + KM112 * (1 + PR_con->GCA / KI1122) * (1 + theVars->Pi / KI1121));
        theVars->PR_Vel.v121 = V121 * PR_con->GCAc / (PR_con->GCAc + KM121);
        theVars->PR_Vel.v122 = V122 * (PR_con->GOAc * PR_con->SERc - PR_con->HPRc * PR_con->GLYc / KE122) / ((PR_con->GOAc + KM1221) * (PR_con->SERc + KM1222 * (1 + PR_con->GLYc / KI1221)));
        theVars->PR_Vel.v123 = V123 * (PR_con->HPRc * NADHc - PR_con->GCEAc * NADc / KE123) / ((PR_con->HPRc + KM123 * (1 + PR_con->HPRc / KI123)) * (NADHc + PrKM1232));
        theVars->PR_Vel.v124 = V124 * (PR_con->GOAc * theVars->GLUc - KGc * PR_con->GLYc / KE124) / ((PR_con->GOAc + KM1241) * (theVars->GLUc + KM1242 * (1 + PR_con->GLYc / KI1221)));
        theVars->PR_Vel.v131 = V131 * PR_con->GLYc / (PR_con->GLYc + KM1311 * (1 + PR_con->SERc / KI1311));
        theVars->PR_Vel.v1in = theVars->V1T * (PR_con->GCEAc / (PR_con->GCEAc + KM1011 * (1 + PR_con->GCAc / KI1011)) - PR_con->GCEA / (PR_con->GCEA + KM1011 * (1 + PR_con->GCA / KI1011)));// Competive inhibition
        theVars->PR_Vel.v2out = V2T * (PR_con->GCA / (PR_con->GCA + KM1012 * (1 + PR_con->GCEA / KI1012)) - PR_con->GCAc / (PR_con->GCAc + KM1012 * (1 + PR_con->GCEAc / KI1012)));// Competive inhibition
    }
    if (theVars->record) {
        theVars->PR_VEL.insert(theVars->PR_TIME_N - 1, t, theVars->PR_Vel);

        // here is some parameters we need to output

        theVars->PR2OUT.GCEA = PR_con->GCEA;
        theVars->PR2OUT.GCA = PR_con->GCA;
        theVars->PR2OUT.PGA = PR_con->PGA;
        theVars->PR2OUT.PGCA = PR_con->PGCA;
        theVars->PR2OUT.GCAc = PR_con->GCAc;
        theVars->PR2OUT.GOAc = PR_con->GOAc;
        theVars->PR2OUT.SERc = PR_con->SERc;
        theVars->PR2OUT.GLYc = PR_con->GLYc;
        theVars->PR2OUT.HPRc = PR_con->HPRc;
        theVars->PR2OUT.GCEAc = PR_con->GCEAc;
        theVars->PR2OUT.RuBP = PR_con->RuBP;
        theVars->PR2OUT._v131 = theVars->PR_Vel.v131;
    }
}
