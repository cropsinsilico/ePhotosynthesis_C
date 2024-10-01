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
#include <math.h>
#include "Variables.hpp"
#include "modules/PR.hpp"
#include "modules/PS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double PR::TIME = 0.;
std::size_t PR::N = 1;
const std::size_t PRCondition::count = 10;
bool PRCondition::PS_connect = false;
bool PR::PS_connect = false;
bool PRCondition::PS_RuBP = false;
bool PR::PS_RuBP = false;

DEFINE_VALUE_SET_STATIC(PR);
DEFINE_VALUE_SET(PRCondition);

PRCondition* PR::_init(Variables *theVars) {
    PR::setPS_connect(theVars->PR_PS_com);
    if (theVars->useC3) {
        PR::NADHc = 0.47;
        PR::NADc = 0.4;
        PR::GLUc = 24.;
        PR::KGc = 0.4;
    } else {
        PR::NADHc = 0.47 * theVars->PRRatio[8];
        PR::NADc = 0.4 * theVars->PRRatio[9];
        PR::GLUc = 24. * theVars->PRRatio[12];
        PR::KGc = 0.4 * theVars->PRRatio[13];

        PR::PR_ADP = 0.82 * theVars->PRRatio[14];
        PR::PR_ATP = 0.68 * theVars->PRRatio[15];
    }

    PRCondition* PR_con = new PRCondition();

    PR_con->GCEA = 0.1812;   // Glycerate in chloroplast; derived based on V113
    PR_con->GCA = 0.36;      // Derived from radioactive labelling experiment; assuem equal concenatration
                            // inside and outshide chloroplast
    if (theVars->useC3) {
        PR::PGA = 2.4;       // PGA in chloroplast;
    } else {
        PR::PGA = 4.3;       // PGA in chloroplast;4.3 is the original value;
    }
    PR_con->PGCA = 0.0029;   // Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;

    PR_con->GCAc = 0.36;     // See the note for GCA.
    PR_con->GOAc = 0.028;    // Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
    PR_con->SERc = 7.5;      // Serine in cytosol; 7.5 original value
    PR_con->GLYc = 1.8;      // Glycine in cytosol; 1.8 original vlaue
    PR_con->HPRc = 0.0035;   // HydroxylPyruvate; derived from equation 123;
    PR_con->GCEAc = 0.1812;  // Glycerate in cytosol; assume at equilibrium with GCEA initially.
    PR_con->RuBP = 2.;       // RuBP concentration

    if (theVars->useC3) {
        PR::V1T = 5.;
        PR::Vfactor112 = 1.;
        PR::Vfactor113 = 1.;
        PR::Vfactor121 = 1.;
        PR::Vfactor122 = 1.;
        PR::Vfactor123 = 1.;
        PR::Vfactor124 = 1.;
        PR::Vfactor131 = 1.;
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0) {
            PR::Vfactor112 = theVars->VfactorCp[25];
            PR::Vfactor113 = theVars->VfactorCp[26];
            PR::Vfactor121 = theVars->VfactorCp[28];
            PR::Vfactor122 = theVars->VfactorCp[29];
            PR::Vfactor123 = theVars->VfactorCp[30];
            PR::Vfactor131 = theVars->VfactorCp[32];
            PR::Vfactor124 = theVars->VfactorCp[11];
        }

        PR::Vf_T131 = 1.;
        PR::Vf_T113 = 1.;
        PR::Vf_T123 = 1.;
        PR::Vf_T121 = 1.;
        PR::Vf_T122 = 1.;
        PR::Vf_T112 = 1.;

        if (theVars->GRNT == 1 && theVars->Tp > 25.){
            PR::Vf_T131 = theVars->VfactorT[4];
            PR::Vf_T113 = theVars->VfactorT[6];
            PR::Vf_T123 = theVars->VfactorT[7];
            PR::Vf_T121 = theVars->VfactorT[8];
            PR::Vf_T122 = theVars->VfactorT[11];
            PR::Vf_T112 = theVars->VfactorT[22];
        }

        if (theVars->GP == 0) {
            PR::V111 = PS::getV1() * 0.22;
            PR::V112 = theVars->EnzymeAct.at("V112");
            PR::V113 = theVars->EnzymeAct.at("V113");
            PR::V121 = theVars->EnzymeAct.at("V121");
            PR::V122 = theVars->EnzymeAct.at("V122");
            PR::V123 = theVars->EnzymeAct.at("V123");
            PR::V124 = theVars->EnzymeAct.at("V124");
            PR::V131 = theVars->EnzymeAct.at("V131");
        }
        // The constant for calculating the glycolate uptake
        PR::V2T = 6.;            // The original value is 0.32.
        // Reaction: 111: RUBP+O2<-->PGlycolate + PGA
        PR::KO = PS::getKM12();  // Michaelis constant for O2
        PR::KC = PS::getKM11();  // Michaelis constant for CO2
        PR::KR = 0.02;           // Michaelis constant for RUBP

        // Reaction: 112: PGlycolate-->Pi+Glycolate;
        PR::KM112 = 0.026;   // Km112 for PGlycolate;
        PR::KI1122 = 94.;    // Inhibition constant for Glycolate;
        PR::KI1121 = 2.55;   // The competitive Pi inhibition for PGlycolate

        // Reaction 113  : Gcea+ATP<-->ADP + PGA
        PR::KM1131 = 0.21;   // Km for ATP;
        PR::KM1132 = 0.25;   // Km for Gcea;
        PR::KI113 = 0.36;    // Ki for ATP BY pga;  %%%%%%%%%%%%%%%%%%%%%%%%% Competitive inhibition for ATP; in original paper it is 0.36;
        PR::KE113 = 300.;    // New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics

        // To set global information for different reactions
        PR::KM121 = 0.1;     //Glycolate +O2<-->H2O2+Glyoxylate

        PR::KM1221 = 0.15;   // Michaelis constant for glyoxylate;
        PR::KM1222 = 2.7;    // Michaelis constant for serinie;
        PR::KI1221 = 33.;    // Inhibition constant for Glycine;
        PR::KE122 = 0.24;    //  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.

        PR::KM123 = 0.09;    //   Michaelis constant for hydroxylpyruvate;
        PR::KI123 = 12.;     // Inhibition constant for hydroxypyruvate;
        PR::KE123 = 1. / (4. * pow(10., -6.));  // Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 1/(4*10^(-6);

        PR::KM1241 = 0.15;   // Michaelis constant for glyoxylate
        PR::KM1242 = 1.7;    // Michaelis constant for Glu
        PR::KI124 = 2.0;     // This KI is one guessed
        PR::KE124 = 607.;    // New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K 607.

        PR::KM1311 = 6.;     // Michaelis constant for Glycine;
        PR::KI1311 = 4.;     // Inhibition constant for Serine

        PR::KM1312 = 0.075;  // Michaelis constant for NAD;
        PR::KI1312 = 0.015;  // Inhibition constant for NADH; Since in the current program, we assume that P protein limit the rate of the overall glycin decarboxylase; the KI1312 and KM1312 were not used.

        PR::KM1011 = 0.39;
        PR::KI1011 = 0.28;

        PR::KM1012 = 0.2;
        PR::KI1012 = 0.22;
        const double tempRatio = (theVars->Tp - 25.) / 10.;
    
        PR::PrV112 = PR::V112 * PR::Vfactor112 * PR::Vf_T112 * pow(PR::Q10_112, tempRatio);
        PR::PrV113 = PR::V113 * PR::Vfactor113 * PR::Vf_T113 * pow(PR::Q10_113, tempRatio);
        PR::PrV121 = PR::V121 * PR::Vfactor121 * PR::Vf_T121 * pow(PR::Q10_121, tempRatio);
        PR::PrV122 = PR::V122 * PR::Vfactor122 * PR::Vf_T122 * pow(PR::Q10_122, tempRatio);
        PR::PrV123 = PR::V123 * PR::Vfactor123 * PR::Vf_T123 * pow(PR::Q10_123, tempRatio);
        PR::PrV124 = PR::V124 * PR::Vfactor124 * pow(PR::Q10_124, tempRatio);
        PR::PrV131 = PR::V131 * PR::Vfactor131 * PR::Vf_T131 * pow(PR::Q10_131, tempRatio);

    } else {
        // To set global information for different reactions
        // Reaction: 110: RuBP + CO2 <--> 2PGA

        PR::CE = 1.; // This is the coefficient for calibrating the volume effect // Default is 4.

        // Reaction: 111: RUBP+O2<-->PGlycolate + PGA

        PR::KO = 0.222 * theVars->PRRatio[16];  // Michaelis constant for O2
        PR::KC = 0.0115 * theVars->PRRatio[17]; // Michaelis constant for CO2

        if (theVars->PR_PS_com) {
            PR::KC = PS::getKM11();
            PR::KO = PS::getKM12();
        }

        PR::KR = 0.02 * theVars->PRRatio[18]; // Michaelis constant for RUBP

        // Reaction: 112: PGlycolate-->Pi+Glycolate;

        PR::KM112 = 0.026 * theVars->PRRatio[19]; // Km112 for PGlycolate;
        PR::KI1122 = 94. * theVars->PRRatio[20];  // Inhibition constant for Glycolate;
        PR::KI1121 = 2.55 * theVars->PRRatio[21]; // The competitive Pi inhibition for PGlycolate


        // Reaction 113  : Gcea+ATP<-->ADP + PGA
        PR::KM1131 = 0.21 * theVars->PRRatio[22]; // Km for ATP;
        PR::KM1132 = 0.25 * theVars->PRRatio[23]; // Km for Gcea;
        PR::KI113 = 0.36 * theVars->PRRatio[24];  // Competitive inhibition for ATP; in original paper it is 0.36;
        PR::KE113 = 300. * theVars->PRRatio[25];  // New       Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics  300, as default


        // Reactoin 121; Glycolate +O2<-->H2O2+Glyoxylate
        PR::KM121 = 0.1 * theVars->PRRatio[26];

        // Reaction 122  : Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
        PR::KM1221 = 0.15 * theVars->PRRatio[27]; // Michaelis constant for glyoxylate;
        PR::KM1222 = 2.7 * theVars->PRRatio[28];  // Michaelis constant for serinie;
        PR::KI1221 = 33. * theVars->PRRatio[29];  // Inhibition constant for Glycine;
        PR::KE122 = 0.24 * theVars->PRRatio[30];  //  New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.

        // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate

        PR::KM123 = 0.09 * theVars->PRRatio[31];  //   Michaelis constant for hydroxylpyruvate;
        PR::KI123 = 12. * theVars->PRRatio[32];   // Inhibition constant for hydroxypyruvate;
        PR::KE123 = 1. / (4. * pow(10., -6.)) * theVars->PRRatio[33];

        // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
        PR::KM1241 = 0.15 * theVars->PRRatio[34]; // Michaelis constant for glyoxylate
        PR::KM1242 = 1.7 * theVars->PRRatio[35];  // Michaelis constant for Glu
        PR::KE124 = 607. * theVars->PRRatio[37];  // New       Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K' 607.

        // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
        PR::KM1311 = 6. * theVars->PRRatio[38];  // Michaelis constant for Glycine;
        PR::KI1311 = 4. * theVars->PRRatio[39];  // Inhibition constant for Serine

        // The consant for calculating the glycerate uptake.
        PR::V1T = 0.25 * PR::CE * 20. * theVars->PRRatio[42];
        PR::KM1011 = 0.39 * theVars->PRRatio[43];
        PR::KI1011 = 0.28 * theVars->PRRatio[44];

        // The constant for calculating the glycolate output
        PR::V2T = 0.32 * PR::CE * 10. * 2. * theVars->PRRatio[45];
        PR::KM1012 = 0.2 * theVars->PRRatio[46];
        PR::KI1012 = 0.22 * theVars->PRRatio[47];

        if (theVars->GP == 0) {
            if (theVars->PR_PS_com) {
                V111 = 0.24 * PS::getV1() * theVars->PRRatio[0];
            } else {
                V111 = 3.7 * 0.24 * 1.;
            }
            PR::V112 = 52.41992121 * theVars->PRRatio[1];
            PR::V113 = 5.715787563 * theVars->PRRatio[2];
            PR::V121 = 1.456108923 * theVars->PRRatio[3];
            PR::V122 = 3.306190845 * 3. * theVars->PRRatio[4];
            PR::V123 = 10.00978112 * theVars->PRRatio[5];
            PR::V124 = 2.745819515 * theVars->PRRatio[6];
            PR::V131 = 2.494745448 * theVars->PRRatio[7];
        }
    }

    // Reaction 110: RuBP + CO2 <--> 2PGA
    // Reaction 111: RUBP+O2<-->PGlycolate + PGA
    // Reaction 112: PGlycolate-->Pi+Glycolate;
    // Reaction 113: Gcea+ATP<-->ADP + PGA
    // Reaction 121: Glycolate +O2<-->H2O2+Glyoxylate
    // Reaction 122: Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
    
    return PR_con;
}

PRCondition* PR::_initAlt(Variables *theVars, PRCondition* PR_con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<PR>();
    theVars->initParam(*PR_con);
    
    // Useful intermediate constants
    const double tempRatio = (theVars->Tp - 25.) / 10.;

    // Reaction 110: RuBP + CO2 <--> 2PGA
    // Reaction 111: RUBP+O2<-->PGlycolate + PGA
    // Reaction 112: PGlycolate-->Pi+Glycolate;
    // Reaction 113: Gcea+ATP<-->ADP + PGA
    // Reaction 121: Glycolate +O2<-->H2O2+Glyoxylate
    // Reaction 122: Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
    // Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
    // Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
    // Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
    
    if (theVars->useC3) {
      if (theVars->GRNC == 1 && theVars->CO2_cond > 0.) {
        PR::set(MOD::PR::Vfactor112, theVars->VfactorCp[25]);
        PR::set(MOD::PR::Vfactor113, theVars->VfactorCp[26]);
        PR::set(MOD::PR::Vfactor121, theVars->VfactorCp[28]);
        PR::set(MOD::PR::Vfactor122, theVars->VfactorCp[29]);
        PR::set(MOD::PR::Vfactor123, theVars->VfactorCp[30]);
        PR::set(MOD::PR::Vfactor131, theVars->VfactorCp[32]);
        PR::set(MOD::PR::Vfactor124, theVars->VfactorCp[11]);
      }
      if (theVars->GRNT == 1 && theVars->Tp > 25) {
        PR::set(MOD::PR::Vf_T131, theVars->VfactorT[4]);
        PR::set(MOD::PR::Vf_T113, theVars->VfactorT[6]);
        PR::set(MOD::PR::Vf_T123, theVars->VfactorT[7]);
        PR::set(MOD::PR::Vf_T121, theVars->VfactorT[8]);
        PR::set(MOD::PR::Vf_T122, theVars->VfactorT[11]);
        PR::set(MOD::PR::Vf_T112, theVars->VfactorT[22]);
      }
      if (theVars->GP == 0) {
        PR::set(MOD::PR::V111, PS::getV1() * 0.22);
        PR::setFromEnzymeAct(MOD::PR::V112, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V113, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V121, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V122, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V123, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V124, theVars->EnzymeAct);
        PR::setFromEnzymeAct(MOD::PR::V131, theVars->EnzymeAct);
      }
      PR::set(MOD::PR::KO, PS::getKM12());
      PR::set(MOD::PR::KC, PS::getKM11());
      
      PR::set(MOD::PR::PrV112,
              PR::get(MOD::PR::V112) *
              PR::get(MOD::PR::Vfactor112) *
              PR::get(MOD::PR::Vf_T112) *
              pow(PR::get(MOD::PR::Q10_112), tempRatio));
      PR::set(MOD::PR::PrV113,
              PR::get(MOD::PR::V113) *
              PR::get(MOD::PR::Vfactor113) *
              PR::get(MOD::PR::Vf_T113) *
              pow(PR::get(MOD::PR::Q10_113), tempRatio));
      PR::set(MOD::PR::PrV121,
              PR::get(MOD::PR::V121) *
              PR::get(MOD::PR::Vfactor121) *
              PR::get(MOD::PR::Vf_T121) *
              pow(PR::get(MOD::PR::Q10_121), tempRatio));
      PR::set(MOD::PR::PrV122,
              PR::get(MOD::PR::V122) *
              PR::get(MOD::PR::Vfactor122) *
              PR::get(MOD::PR::Vf_T122) *
              pow(PR::get(MOD::PR::Q10_122), tempRatio));
      PR::set(MOD::PR::PrV123,
              PR::get(MOD::PR::V123) *
              PR::get(MOD::PR::Vfactor123) *
              PR::get(MOD::PR::Vf_T123) *
              pow(PR::get(MOD::PR::Q10_123), tempRatio));
      PR::set(MOD::PR::PrV124,
              PR::get(MOD::PR::V124) *
              PR::get(MOD::PR::Vfactor124) *
              pow(PR::get(MOD::PR::Q10_124), tempRatio));
      PR::set(MOD::PR::PrV131,
              PR::get(MOD::PR::V131) *
              PR::get(MOD::PR::Vfactor131) *
              PR::get(MOD::PR::Vf_T131) *
              pow(PR::get(MOD::PR::Q10_131), tempRatio));
    } else {

      if (theVars->GP == 0) {
        if (theVars->PR_PS_com) {
          PR::set(MOD::PR::V111, PS::getV1() * 0.24);
        } else {
          PR::set(MOD::PR::V111, 3.7 * 0.24 * 1.);
        }
      }

      if (theVars->PR_PS_com) {
        PR::set(MOD::PR::KC, PS::getKM11());
        PR::set(MOD::PR::KO, PS::getKM12());
      }
        
      size_t i = 0;
      for (PR::iterator it = PR::begin(); it != PR::end(); it++) {
        if (i == 48)
          break;
        switch (it->first) {
        case (MOD::PR::V111) : {
          if (theVars->GP == 0 && theVars->PR_PS_com)
            it->second *= theVars->PRRatio[i];
          i++;
          break;
        }
        case (MOD::PR::NADc) : {
          it->second *= theVars->PRRatio[i];
          // PRRatio 10 & 11 skipped
          i = 12;
          break;
        }
        case (MOD::PR::KC) :
        case (MOD::PR::KO) : {
          if (!theVars->PR_PS_com) {
            it->second *= theVars->PRRatio[i];
          }
          i++;
          break;
        }
        case (MOD::PR::V1T) :
        case (MOD::PR::V2T) : {
          it->second *= PR::get(MOD::PR::CE) * theVars->PRRatio[i];
          i++;
          break;
        }
        case (MOD::PR::KI1311) : {
          it->second *= theVars->PRRatio[i];
          // PRRatio 40 & 41 skipped
          i = 42;
          break;
        }
        case (MOD::PR::KI124) :
        case (MOD::PR::KM1312) :
        case (MOD::PR::KI1312) : {
          // PRRatio not used
          i++;
          break;
        }
        default : {
          it->second *= theVars->PRRatio[i];
          i++;
        }
        }
      }

    }

#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
#endif // CHECK_VALUE_SET_ALTS
    return PR_con;
}

void PR::_reset() {
    PR::KC = 0.;
    PR::KE113 = 0.;
    PR::KE122 = 0.;
    PR::KE123 = 0.;
    PR::KE124 = 0.;
    PR::KGc = 0.;
    PR::KI1011 = 0.;
    PR::KI1012 = 0.;
    PR::KI1121 = 0.;
    PR::KI1122 = 0.;
    PR::KI113 = 0.;
    PR::KI1221 = 0.;
    PR::KI123 = 0.;
    PR::KI1311 = 0.;
    PR::KM1011 = 0.;
    PR::KM1012 = 0.;
    PR::KM112 = 0.;
    PR::KM1131 = 0.;
    PR::KM1132 = 0.;
    PR::KM121 = 0.;
    PR::KM1221 = 0.;
    PR::KM1222 = 0.;
    PR::KM123 = 0.;
    PR::KM1241 = 0.;
    PR::KM1242 = 0.;
    PR::KM1311 = 0.;
    PR::KO = 0.;
    PR::KR = 0.;
    PR::NADHc = 0.;
    PR::NADc = 0.;
    PR::PR_ADP = 0.;
    PR::PR_ATP = 0.;
    PR::V111 = 0.;
    PR::V112 = 0.;
    PR::V113 = 0.;
    PR::V121 = 0.;
    PR::V122 = 0.;
    PR::V123 = 0.;
    PR::V124 = 0.;
    PR::V131 = 0.;
    PR::V2T = 0.;
    PR::Vfactor112 = 0.;
    PR::Vfactor113 = 0.;
    PR::Vfactor121 = 0.;
    PR::Vfactor122 = 0.;
    PR::Vfactor123 = 0.;
    PR::Vfactor124 = 0.;
    PR::Vfactor131 = 0.;
    PR::Vf_T131 = 0.;
    PR::Vf_T113 = 0.;
    PR::Vf_T123 = 0.;
    PR::Vf_T121 = 0.;
    PR::Vf_T122 = 0.;
    PR::Vf_T112 = 0.;
    PR::V1T = 0.;
    PR::PGA = 0.;
    PR::GLUc = 0.;
    setPS_connect(false);
    setPS_RuBP(false);
    PR::RUBISCOTOTAL = 0.;

    PR::PrV112 = 0.;
    PR::PrV113 = 0.;
    PR::PrV121 = 0.;
    PR::PrV122 = 0.;
    PR::PrV123 = 0.;
    PR::PrV124 = 0.;
    PR::PrV131 = 0.;

    PR::TIME = 0.;
    PR::N = 1;
    conditions::PRCondition::reset();
}

void PR::_updateAlts(Variables *theVars, PRCondition* PR_con) {
#ifdef CHECK_VALUE_SET_ALTS
    PR::updateAlts();
    PR_con->updateAlts();
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(PR_con);
#endif // CHECK_VALUE_SET_ALTS
}
