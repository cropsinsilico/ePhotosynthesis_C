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
#include <math.h>
#include "Variables.hpp"
#include "modules/PR.hpp"
#include "modules/PS.hpp"
#include "conditions/PS_PRCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void PR::_Rate(const double t, const PRCondition* const PR_con, Variables *theVars) {
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
    double RuBP;
    if (PR::PS_RuBP) {
        RuBP = PR_con->parent->PS_con->RuBP;
    } else {
        RuBP = PR_con->RuBP;
    }

    if (theVars->useC3) {

        const double PrV111 = PS::getPsV1() * 0.24;

       if (theVars->PR_PS_com) {
            PR::KC= PS::getKM11();
            PR::KO= PS::getKM12();
        }

        if (theVars->RUBISCOMETHOD == 2) {            // Using michelies and enzyme information
            double PrV111t;
            if (theVars->PR_PS_com) {       // FOr the combined PS-PR model
                PrV111t = PrV111*RuBP / (RuBP + PR::KR * PS::getV1Reg());
            } else {                    // For the PR model
                PrV111t = PrV111*RuBP / (RuBP + PR::KR);
            }
            theVars->PR_Vel.v111 = PrV111t * theVars->O2_cond / (theVars->O2_cond + PR::KO *
                                                                 (1. + theVars->CO2_cond / PR::KC));

            if (RuBP < PR::RUBISCOTOTAL)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * RuBP / PR::RUBISCOTOTAL;

        } else if (theVars->RUBISCOMETHOD == 1) {
            theVars->PR_Vel.v111 = PrV111 * theVars->O2_cond / (theVars->O2_cond + PR::KO *
                                                                (1. + theVars->CO2_cond / PR::KC));
            if (RuBP < PR::RUBISCOTOTAL)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * RuBP / PR::RUBISCOTOTAL;

        }

        theVars->PR_Vel.v112 = PR::PrV112 * PR_con->PGCA / (PR_con->PGCA + PR::KM112 *
                                                            (1. + PR_con->GCA / PR::KI1122) *
                                                            (1. + theVars->Pi / PR::KI1121));

         if (theVars->PR_PS_com) {            // For the combined PS-PR MODEL
             theVars->PR_Vel.v113 = PR::PrV113 * (PR_con->parent->PS_con->ATP * PR_con->GCEA -
                                                  theVars->ADP * PR_con->parent->PS_con->PGA / PR::KE113) /
                                    ((PR_con->parent->PS_con->ATP + PR::KM1131 *
                                      (1. + PR_con->parent->PS_con->PGA / PR::KI113)) *
                                     (PR_con->GCEA + PR::KM1132));
         } else {
             theVars->PR_Vel.v113 = PR::PrV113 * (PR_con->parent->PS_con->ATP * PR_con->GCEA -
                                                  theVars->ADP * PR::PGA / PR::KE113) /
                                    ((PR_con->parent->PS_con->ATP + PR::KM1131 *
                                      (1. + 2.5 / PR::KI113)) * (PR_con->GCEA + PR::KM1132));
         }

        theVars->PR_Vel.v121 = PR::PrV121 * PR_con->GCAc / (PR_con->GCAc + PR::KM121);

        theVars->PR_Vel.v122 = PR::PrV122 * (PR_con->GOAc * PR_con->SERc - PR_con->HPRc *
                                             PR_con->GLYc / PR::KE122) / ((PR_con->GOAc + PR::KM1221) *
                                                                          (PR_con->SERc + PR::KM1222 *
                                                                           (1. + PR_con->GLYc / PR::KI1221)));
        theVars->PR_Vel.v123 = PR::PrV123 * (PR_con->HPRc * PR::NADHc - PR_con->GCEA * PR::NADc / PR::KE123) /
                               (PR_con->HPRc + PR::KM123 * (1. + PR_con->HPRc / PR::KI123));
        theVars->PR_Vel.v124 = PR::PrV124 * (PR_con->GOAc * PR::GLUc - PR::KGc * PR_con->GLYc / PR::KE124) /
                               ((PR_con->GOAc + PR::KM1241) * (PR::GLUc + PR::KM1242 *
                                                               (1. + PR_con->GLYc / PR::KI124)));

        theVars->PR_Vel.v131 = PR::PrV131 * PR_con->GLYc/(PR_con->GLYc + PR::KM1311 *
                                                          (1. + PR_con->SERc / PR::KI1311));
        theVars->PR_Vel.v2out = PR::V2T * (PR_con->GCA/(PR_con->GCA + PR::KM1012 *
                                                        (1. + PR_con->GCEA / PR::KI1012)) -
                                           PR_con->GCAc /(PR_con->GCAc + PR::KM1012 *
                                                          (1. + PR_con->GCEAc / PR::KI1012)));   // Competive inhibition
        theVars->PR_Vel.v1in = PR::V1T *(PR_con->GCEAc / (PR_con->GCEAc + PR::KM1011 *
                                                          (1. + PR_con->GCAc / PR::KI1011)) -
                                         PR_con->GCEA /(PR_con->GCEA + PR::KM1011 *
                                                        (1. + PR_con->GCA / PR::KI1011)));  // Competive inhibition

    } else {
        theVars->Pi = theVars->PR_Param[1];    // Value from spinach. Assume constant currently.

        // Information from PS cycle is transfered back to the PR cycle for calcualtion.

        double ATP;
        double ADP;
        if (theVars->PR_PS_com) {
            ADP = theVars->ADP;
            ATP = PR_con->parent->PS_con->ATP;
        } else {
            ADP = PR::PR_ADP;
            ATP = PR::PR_ATP;
        }

        double PrV111t;
        if (theVars->RUBISCOMETHOD == 2) {
            if (theVars->PR_PS_com) {
                PrV111t = PR::V111 * RuBP / (RuBP + PR::KR * PS::getV1Reg());
            } else {
                PrV111t = PR::V111 * RuBP / (RuBP + PR::KR);
            }
            theVars->PR_Vel.v111 = PrV111t * theVars->O2_cond / (theVars->O2_cond + PR::KO *
                                                                 (1. + theVars->CO2_cond / PR::KC));

            if (RuBP < PS::getPsV1() / 2.5)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * (2.5 * RuBP / PrV111t);


        } else if (theVars->RUBISCOMETHOD == 1){
            theVars->PR_Vel.v111 = PR::V111 * theVars->O2_cond / (theVars->O2_cond + PR::KO *
                                                                  (1. + theVars->CO2_cond / PR::KC));
            if (RuBP < PR::RUBISCOTOTAL)
                theVars->PR_Vel.v111 = theVars->PR_Vel.v111 * RuBP / PR::RUBISCOTOTAL;

        } else {
            throw std::invalid_argument("Invalid value of RUBISCOMETHOD used");
        }

        //double v113;
        if (theVars->PR_PS_com) {
            theVars->PR_Vel.v113 = PR::V113 * (ATP * PR_con->GCEA - ADP * PR_con->parent->PS_con->PGA / PR::KE113) /
                                   ((ATP + PR::KM1131 * (1. + PR_con->parent->PS_con->PGA / PR::KI113)) *
                                    (PR_con->GCEA + PR::KM1132));// This is the old version.

        } else {
            theVars->PR_Vel.v113 = PR::V113 * (ATP * PR_con->GCEA - ADP * PR::PGA / PR::KE113) /
                                   ((ATP + PR::KM1131 * (1. + 2.5 / PR::KI113)) *
                                    (PR_con->GCEA + PR::KM1132));// This is the old version.
        }


        // The reaction: PR_con->GOAc + Serine --> HPR + Gly

        const double PrKM1232 = 0.5;

        // The following is used to take the information back to the PRmb routine.
        theVars->PR_Vel.v112 = PR::V112 * PR_con->PGCA / (PR_con->PGCA + PR::KM112 *
                                                          (1. + PR_con->GCA / PR::KI1122) *
                                                          (1. + theVars->Pi / PR::KI1121));
        theVars->PR_Vel.v121 = PR::V121 * PR_con->GCAc / (PR_con->GCAc + PR::KM121);
        theVars->PR_Vel.v122 = PR::V122 * (PR_con->GOAc * PR_con->SERc - PR_con->HPRc * PR_con->GLYc /
                                           PR::KE122) / ((PR_con->GOAc + PR::KM1221) *
                                                         (PR_con->SERc + PR::KM1222 *
                                                          (1. + PR_con->GLYc / PR::KI1221)));
        theVars->PR_Vel.v123 = PR::V123 * (PR_con->HPRc * PR::NADHc - PR_con->GCEAc * PR::NADc / PR::KE123) /
                               ((PR_con->HPRc + PR::KM123 * (1. + PR_con->HPRc / PR::KI123)) *
                                (PR::NADHc + PrKM1232));
        theVars->PR_Vel.v124 = PR::V124 * (PR_con->GOAc * PR::GLUc - PR::KGc * PR_con->GLYc / PR::KE124) /
                               ((PR_con->GOAc + PR::KM1241) * (PR::GLUc + PR::KM1242 *
                                                               (1. + PR_con->GLYc / PR::KI1221)));
        theVars->PR_Vel.v131 = PR::V131 * PR_con->GLYc / (PR_con->GLYc + PR::KM1311 *
                                                          (1. + PR_con->SERc / PR::KI1311));
        theVars->PR_Vel.v1in = PR::V1T * (PR_con->GCEAc / (PR_con->GCEAc + PR::KM1011 *
                                                           (1. + PR_con->GCAc / PR::KI1011)) -
                                          PR_con->GCEA / (PR_con->GCEA + PR::KM1011 *
                                                          (1. + PR_con->GCA / PR::KI1011)));// Competive inhibition
        theVars->PR_Vel.v2out = PR::V2T * (PR_con->GCA / (PR_con->GCA + PR::KM1012 *
                                                          (1. + PR_con->GCEA / PR::KI1012)) -
                                           PR_con->GCAc / (PR_con->GCAc + PR::KM1012 *
                                                           (1. + PR_con->GCEAc / PR::KI1012)));// Competive inhibition
    }

#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->PR_Vel)
#endif
    if (theVars->record) {
        if (t > PR::TIME) {
            PR::N++;
            PR::TIME = t;
        }

        theVars->PR_VEL.insert(PR::N - 1, t, theVars->PR_Vel);

        // here is some parameters we need to output

        theVars->PR2OUT.GCEA = PR_con->GCEA;
        theVars->PR2OUT.GCA = PR_con->GCA;
        theVars->PR2OUT.PGCA = PR_con->PGCA;
        theVars->PR2OUT.GCAc = PR_con->GCAc;
        theVars->PR2OUT.GOAc = PR_con->GOAc;
        theVars->PR2OUT.SERc = PR_con->SERc;
        theVars->PR2OUT.GLYc = PR_con->GLYc;
        theVars->PR2OUT.HPRc = PR_con->HPRc;
        theVars->PR2OUT.GCEAc = PR_con->GCEAc;
        if (!PR::PS_RuBP)
            theVars->PR2OUT.RuBP = RuBP;
        theVars->PR2OUT._v131 = theVars->PR_Vel.v131;
    }
}
