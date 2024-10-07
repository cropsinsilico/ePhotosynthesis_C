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
#include "modules/SUCS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double SUCS::TIME = 0.;
std::size_t SUCS::N = 1;

const std::size_t SUCSCondition::count = 8;

DEFINE_VALUE_SET_STATIC(SUCS);
DEFINE_VALUE_SET(SUCSCondition);
DEFINE_VALUE_SET_NS(pool::, SUCSPool);

SUCSCondition* SUCS::_init(Variables *theVars) {
    SUCSCondition* SUCS_Con = new SUCSCondition();
    SUCS::UTPc = 0.75;
    if (theVars->useC3) {

        SUCS::Vf_T52 = 1.;
        SUCS::Vf_T59 = 1.;
        SUCS::Vf_T57 = 1.;
        SUCS::Vf_T51 = 1.;
        SUCS::Vf_T56 = 1.;

        SUCS::KE501 = 0.05;     // Equilibrium Constant  50  KE501  0.05  [Bassham, 1869 #832]
        SUCS::Km511 = 0.02;     // FBP 4.1.2.13 51  Km511 FBP 0.02 Pisum sativum (Anderson, Heinrikson et al. 1975)
        SUCS::Km512 = 0.3;      // FBP 4.1.2.13 51  Km512 GAP 0.3 Spinacia oleracea (Iwaki, Wadano et al. 1991)
        SUCS::Km513 = 0.4;      // FBP 4.1.2.13 51  Km513 DHAP 0.4 Spinacia oleracea (Iwaki, Wadano et al. 1991)
        SUCS::KE51 = 12.;       //  Based on Thomas et al 1997 Biochem Journal. The fifth citation in the paper.
        SUCS::Km521 = 0.0025;   // FBPase[1] 3.1.3.11 52  Km521 FBP 0.0025 Pisum sativum (Jang, Lee et al. 2003)
        SUCS::KI521 = 0.7;      // FBPase 3.1.3.11 52  KI521 F6P 0.7  [Heldt, 1983 #841]
        SUCS::KI522 = 12.;      // FBPase 3.1.3.11 52  KI522 Pi 12 Pisum sativum (Charles & Halliwell 1997)
        SUCS::KI523 = 7 * pow(10., -5.); // FBPase 3.1.3.11 52  KI523 F26BP 7*10^(-5) Pisum sativum <Com> {Jang, 2003 #2523}
        SUCS::KE52 = 6663.;     // FBPase 3.1.3.11 52  KE52   6663 [Bassham, 1869 #832]
        SUCS::KE531 = 2.3;      // Equilibrium Constant 5.3.1.9 53  KE531  2.3[2]  [Bassham, 1869 #832]
        SUCS::KE541 = 0.0584;   // Equilibrium Constant 5.4.2.2 54 G1P G6P KE541 G1P G6P 0.0584  [Bassham, 1869 #832]
        SUCS::Km551 = 0.14;     // UGPase 2.7.7.9 55  Km551 G1P 0.14 Solanum tuberosum (Nakano, Omura et al. 1989)
        SUCS::Km552 = 0.1;      // UDPase 2.7.7.9 55  Km552 UTP 0.1 Solanum tuberosum (Nakano, Omura et al. 1989)
        SUCS::Km553 = 0.11;     // UGPase 2.7.7.9 55  Km553 OPOP 0.11 Solanum tuberosum (Nakano, Omura et al. 1989)
        SUCS::Km554 = 0.12;     // UGPase 2.7.7.9 55  Km554 UDPGlu 0.12 Solanum tuberosum (Nakano, Omura et al. 1989)
        SUCS::KE55 = 0.31;      // UGPase 2.7.7.9 55  KE55 Equi 0.31  Lunn and Rees 1990
        SUCS::Km561 = 0.8;      // SPase 2.4.1.14 56  Km561 D-F6P 0.8 Pisum sativum (Lunn and Ap Rees 1990)
        SUCS::Km562 = 2.4;      // Spase 2.4.1.14 56  Km562 UDP-glucose 2.4 Pisum sativum (Lunn and Ap Rees 1990)
        SUCS::KI561 = 0.7;      //    Inhibitor KI561 UDP 0.7 Spinacia oleracea (Harbron, Foyer et al. 1981)
        SUCS::KI562 = 0.8;      // Sucrose Synthesase   Inhibitor KI562 FBP 0.8 Spinacia oleracea (Harbron, Foyer et al. 1981)
        SUCS::KI563 = 0.4;      //    Inhibitor KI563 SUCP 0.4 Spinacia oleracea (Harbron, Foyer et al. 1981)
        SUCS::KI564 = 11.;      //  2.4.1.14 56 Inhibitor KI564 Pi 11 Spinacia oleracea (Harbron, Foyer et al. 1981)
        SUCS::KI565 = 50.;      //  2.4.1.14 56 Inhibitor KI565 Sucrose 50 Spinacia oleracea {Salerno, 1978 #2525}
        SUCS::KE56 = 10.;       //     KE56  10 Pisum sativum Lunn and Rees, 1990
        SUCS::Km571 = 0.35;     // SPP 3.1.3.24 57.1  Km571 SUCP 0.35 Pisum sativum (Whitaker 1984)
        SUCS::Ki572 = 80.;      // SPP 3.1.3.24 57.2  Ki572 SUC 80 Daucus carota (Whitaker 1984)
        SUCS::KE57 = 780.;      // SPP 3.1.3.24 57.2  KE57 Equili 780  Lunn and Rees 1990
        SUCS::Km581 = 0.032;    // F26BPa 3.1.3.46 58  Km581 F26BP 0.032 Spinacia oleracea (Macdonald, Chou et al. 1989)
        SUCS::KI581 = 0.1;      // F26BPa 3.1.3.46 58  KI581 F6P 0.1 Arabidopsis thaliana (Villadsen and Nielsen 2001)
        SUCS::KI582 = 0.5;      // F26BPa 3.1.3.46 58  KI582 OP 0.5 Arabidopsis thaliana (Villadsen and Nielsen 2001)
        SUCS::Km591 = 0.5;      // 6PF2K 2.7.1.105 59  Km591 ATP 0.5 Spinacia oleracea (Walker and Huber 1987)
        SUCS::Km593 = 0.5;      // 6PF2K 2.7.1.105 59  Km593 F6P 0.5 Spinacia oleracea (Walker and Huber 1987)
        SUCS::KI591 = 0.16;     //   59  KI591 ADP 0.16 Rattus norvegicus (Kretschmer and Hofmann 1984)
        SUCS::KE61 = 1.2 * 107; // Pyrophosphate hydrolysis    KE61  1.2*107  {Flodgaard, 1974 #2521}
        SUCS::Km621 = 5.;       // Vsink   Notice: pH dependent Km621 Sucrose 5  {Weschke, 2000 #2522}
        SUCS::KE59 = 590.;      // 6PF2K 2.7.1.105 59  KE59  590  Cornish-Bowden, 1997
        SUCS::Km592 = 0.021;    // 6PF2K 2.7.1.105 59  Km592 F26BP 0.021 Sparus aurate (Garcia de Frutos and Baanante 1995)
        SUCS::KI592 = 0.7;      // 6PF2K 2.7.1.105 59  KI592 DHAP 0.7 Spinacia oleracea {Markham, 2002 #2524}
        SUCS::Km601 = 0.042;    // Nucleoside Diphosphate Kinase 2.7.4.6 60 NI Km601 ADP 0.042 Rat Kamura and Shimada 1988
        SUCS::Km602 = 1.66;     // Nucleoside Diphosphate Kinase 2.7.4.6 60 NI Km602 ATP 1.66 Rat Kamura and Shimada 1988
        SUCS::Km603 = 0.28;     // Nucleoside Diphosphate Kinase 2.7.4.6 60 NI Km603 UDP 0.28 Saccharomyces cerevisiae {Jong, 1991 #2518}
        SUCS::Km604 = 16.;      // Nucleoside Diphosphate Kinase 2.7.4.6 60 NI Km604 UTP 16 Rattus norvegicus {Fukuchi, 1994 #2519}
        SUCS::KE60 = 16.;       // Nucleoside Diphosphate Kinase 2.7.4.6 60 NI KE60  16 1.04 {Lynn, 1978 #2520}
        SUCS::Vfactor51 = 1.;
        SUCS::Vfactor52 = 1.;
        SUCS::Vfactor56 = 1.;
        SUCS::Vfactor57 = 1.;
        SUCS::Vfactor59 = 1.;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Initialization of the initial concentration of the different component //
        ///////////////////////////////////////////////////////////////////////////////////////////

        if (theVars->GRNC == 1 && theVars->CO2_cond > 0.) {
            SUCS::Vfactor52 = theVars->VfactorCp[19];
            SUCS::Vfactor56 = theVars->VfactorCp[22];
            SUCS::Vfactor57 = theVars->VfactorCp[23];
            SUCS::Vfactor59 = theVars->VfactorCp[24];

        }
        if (theVars->GRNT == 1 && theVars->Tp > 25) {
            SUCS::Vf_T52 = theVars->VfactorT[0];
            SUCS::Vf_T59 = theVars->VfactorT[12];
            SUCS::Vf_T57 = theVars->VfactorT[13];
            SUCS::Vf_T51 = theVars->VfactorT[17];
            SUCS::Vf_T56 = theVars->VfactorT[27];
        }

        if (theVars->GP == 0) {
            SUCS::V51 = theVars->EnzymeAct.at("V51");
            SUCS::V52 = theVars->EnzymeAct.at("V52");
            SUCS::V55 = theVars->EnzymeAct.at("V55");
            SUCS::V56 = theVars->EnzymeAct.at("V56");
            SUCS::V57 = theVars->EnzymeAct.at("V57");
            SUCS::V58 = theVars->EnzymeAct.at("V58");
        }
        SUCS::V59 = theVars->EnzymeAct.at("V59");
        SUCS::V60 = 6.1; // ATP+UDP --UTP + ADP
        SUCS::V61 = 10000; // POPO --2PO
        SUCS::V62 = 2; // SUC Sink        0.2 works.
        const double tempRatio = (theVars->Tp - 25.) / 10.;

        SUCS::SUCSV51 = SUCS::V51 * SUCS::Vfactor51 * SUCS::Vf_T51 *
                        pow(SUCS::Q10_51, tempRatio);       //  DHAP+GAP --FBP
        SUCS::SUCSV52 = SUCS::V52 * SUCS::Vfactor52 * SUCS::Vf_T52 *
                        pow(SUCS::Q10_52, tempRatio);       //  FBP --F6P + Pi
        SUCS::SUCSV55 = SUCS::V55 * pow(SUCS::Q10_55, tempRatio); //  G1P+UTP --OPOP+UDPG
        SUCS::SUCSV56 = SUCS::V56 * SUCS::Vfactor56 * SUCS::Vf_T56 *
                        pow(SUCS::Q10_56, tempRatio);       // UDPG+F6P--SUCP + UDP
        SUCS::SUCSV57 = SUCS::V57 * SUCS::Vfactor57 * SUCS::Vf_T57 *
                        pow(SUCS::Q10_57, tempRatio);       // SUCP--Pi + SUC
        SUCS::SUCSV58 = SUCS::V58 * pow(SUCS::Q10_58, tempRatio); // F26BP--F6P + Pi

        // Initialize the leaves of active enzyme in a dark adapted leaves;
        // mM

        // Assign value to a variable that is transferred to the program
        SUCS_Con->T3Pc = 2.3;
        SUCS_Con->FBPc = 2.;
        SUCS_Con->HexPc = 5.8;
        SUCS_Con->F26BPc = 7.8 * pow(10., -6.);
        SUCS::ATPc = 0.35;
        SUCS_Con->UDPGc = 0.57;
        //SUCS_Con->UTPc = 0.75;
        SUCS_Con->SUCP = 0.;
        SUCS_Con->SUC = 0.;
        SUCS_Con->PGAc = 0.;

        //////////////////////////////////////////////////////////////////
        // Here is some pool values      //
        //////////////////////////////////////////////////////////////////
        theVars->SUCS_Pool.ATc = 1.0; // mM
        theVars->SUCS_Pool.UTc = 1.5; // mM
        theVars->SUCS_Pool.PTc = 15.;  //
    } else {
        SUCS::KE501 = 1. / 0.05 * theVars->SUCRatio[15];
        SUCS::Km511 = 0.02 * theVars->SUCRatio[16];
        SUCS::Km512 = 0.3 * theVars->SUCRatio[17];
        SUCS::Km513 = 0.4 * theVars->SUCRatio[18];
        SUCS::KE51 = 12. * theVars->SUCRatio[19];
        SUCS::Km521 = 0.0025 * theVars->SUCRatio[21];
        SUCS::KI521 = 0.7 * theVars->SUCRatio[22];
        SUCS::KI522 = 12. * theVars->SUCRatio[23];
        SUCS::KI523 = 7. * pow(10, (-5)) * theVars->SUCRatio[24];
        SUCS::KE52 = 6663. * theVars->SUCRatio[25];
        SUCS::KE531 = 2.3 * theVars->SUCRatio[26];
        SUCS::KE541 = 0.0584 * theVars->SUCRatio[27];
        SUCS::Km551 = 0.14 * theVars->SUCRatio[28];
        SUCS::Km552 = 0.1 * theVars->SUCRatio[29];
        SUCS::Km553 = 0.11 * theVars->SUCRatio[30];
        SUCS::Km554 = 0.12 * theVars->SUCRatio[31];
        SUCS::KE55 = 0.31 * theVars->SUCRatio[32];
        SUCS::Km561 = 0.8 * theVars->SUCRatio[33];
        SUCS::Km562 = 2.4 * theVars->SUCRatio[34];
        SUCS::KI561 = 0.7 * theVars->SUCRatio[35];
        SUCS::KI562 = 0.8 * theVars->SUCRatio[36];
        SUCS::KI563 = 0.4 * theVars->SUCRatio[37];
        SUCS::KI564 = 11. * theVars->SUCRatio[38];
        SUCS::KI565 = 50. * theVars->SUCRatio[39];
        SUCS::KE56 = 10. * theVars->SUCRatio[40];
        SUCS::Km571 = 0.35 * theVars->SUCRatio[41];
        SUCS::Ki572 = 10. * theVars->SUCRatio[42];
        SUCS::KE57 = 780. * theVars->SUCRatio[43];
        SUCS::Km581 = 0.032 * theVars->SUCRatio[44];
        SUCS::KI581 = 0.1 * theVars->SUCRatio[45];
        SUCS::KI582 = 0.5 * theVars->SUCRatio[46];
        SUCS::Km591 = 0.5 * theVars->SUCRatio[47];
        SUCS::Km593 = 0.5 * theVars->SUCRatio[49];
        SUCS::KI591 = 0.16 * theVars->SUCRatio[50];
        SUCS::KE59 = 590. * theVars->SUCRatio[52];
        SUCS::KE61 = 1.2 * 107. * theVars->SUCRatio[58];
        SUCS::Km621 = 5. * theVars->SUCRatio[59];

        // Initialize the leaves of active enzyme in a dark adapted leaves;
        // mM

        // Assign value to a variable that is transferred to the program
        SUCS_Con->T3Pc = 2.;
        SUCS_Con->FBPc = 2.;
        SUCS_Con->HexPc = 5.8;
        SUCS_Con->F26BPc = 7.8 * pow(10., -6.);
        SUCS::ATPc = 0.4;
        SUCS_Con->UDPGc = 0.57;
        //SUCS_Con->UTPc = 0.75;
        SUCS_Con->SUCP = 0.;
        SUCS_Con->SUC = 0.;
        SUCS_Con->PGAc = 0.5;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Initialization of the initial concentration of the different component //
        ///////////////////////////////////////////////////////////////////////////////////////////

        // The following calculate the total concentration of different enzymes.
        const double SC = 10.;
        const double SC1 = 1.;
        SUCS::SC = SC;
        SUCS::SC1 = SC1;

        if (theVars->GP == 0) {
            // Unit: mmol l-1 s-1;
            SUCS::V51 = 0.107376831 * SC * theVars->SUCRatio[0]; // DHAP+GAP --FBP     default 0.5
            SUCS::V52 = 0.063979048 * SC * theVars->SUCRatio[1]; // FBP --F6P + Pi
            SUCS::V55 = 0.115403205 * SC * theVars->SUCRatio[2]; // G1P+UTP --OPOP+UDPG
            SUCS::V56 = 0.055503446 * SC * theVars->SUCRatio[3]; // UDPG+F6P--SUCP + UDP
            SUCS::V57 = 0.55503446 * SC1 * theVars->SUCRatio[4]; // SUCP--Pi + SUC; 0.27 DEFALT
            SUCS::V58 = 0.016819226 * SC * theVars->SUCRatio[5]; // F26BP--F6P + Pi
        }

        SUCS::V59 = 0.03 * SC * theVars->SUCRatio[6];            // F6P + ATP --ADP + F26BP // defalut 0.03  (* 0.3)
        //theVars->V60 = 6.1 * theVars->SUCRatio[7];// ATP+UDP --UTP + ADP
        //theVars->V61 = 10000;         // POPO --2PO   // constant set in globals.hpp
        SUCS::V62 = 2. * SC1 * theVars->SUCRatio[8];             // SUC Sink        0.9 works.
        SUCS::Vdhap_in = 1.05 * SC1 * theVars->SUCRatio[9];      // DHAP export from chloroplast
        SUCS::Vgap_in = 1.05 * SC1 * theVars->SUCRatio[10];      // GAP export from chloroplast
        SUCS::Vpga_in = 1.05 * SC1 * theVars->SUCRatio[11];      // PGA export from chloropalst

        //////////////////////////////////////////////////////////////////
        // Here is some pool values      //
        //////////////////////////////////////////////////////////////////
        theVars->SUCS_Pool.ATc = 1.0 * theVars->SUCRatio[12]; // mM
        theVars->SUCS_Pool.UTc = 1.5 * theVars->SUCRatio[13]; // mM
        theVars->SUCS_Pool.PTc = 15. * theVars->SUCRatio[14];  //
    }
    SUCS::KE5Ratio = 1. + SUCS::KE541 + 1. / SUCS::KE531;
    SUCS::ADPc = theVars->SUCS_Pool.ATc - SUCS::ATPc;
    
    return SUCS_Con;
}

void SUCS::_initAlt(Variables *theVars, SUCSCondition* SUCS_Con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<SUCS>();
    theVars->initParam(*SUCS_Con);
    theVars->initParam(theVars->SUCS_Pool);
    
    // Useful intermediate constants
    const double tempRatio = (theVars->Tp - 25.) / 10.;
      
    if (theVars->useC3) {
      if (theVars->GRNC == 1 && theVars->CO2_cond > 0.) {
        SUCS::set(MOD::SUCS::Vfactor52, theVars->VfactorCp[19]);
        SUCS::set(MOD::SUCS::Vfactor56, theVars->VfactorCp[22]);
        SUCS::set(MOD::SUCS::Vfactor57, theVars->VfactorCp[23]);
        SUCS::set(MOD::SUCS::Vfactor59, theVars->VfactorCp[24]);
      }
      if (theVars->GRNT == 1 && theVars->Tp > 25) {
        SUCS::set(MOD::SUCS::Vf_T52, theVars->VfactorT[0]);
        SUCS::set(MOD::SUCS::Vf_T59, theVars->VfactorT[12]);
        SUCS::set(MOD::SUCS::Vf_T57, theVars->VfactorT[13]);
        SUCS::set(MOD::SUCS::Vf_T51, theVars->VfactorT[17]);
        SUCS::set(MOD::SUCS::Vf_T56, theVars->VfactorT[27]);
      }
      if (theVars->GP == 0) {
        SUCS::setFromEnzymeAct(MOD::SUCS::V51, theVars->EnzymeAct);
        SUCS::setFromEnzymeAct(MOD::SUCS::V52, theVars->EnzymeAct);
        SUCS::setFromEnzymeAct(MOD::SUCS::V55, theVars->EnzymeAct);
        SUCS::setFromEnzymeAct(MOD::SUCS::V56, theVars->EnzymeAct);
        SUCS::setFromEnzymeAct(MOD::SUCS::V57, theVars->EnzymeAct);
        SUCS::setFromEnzymeAct(MOD::SUCS::V58, theVars->EnzymeAct);
      }
      SUCS::setFromEnzymeAct(MOD::SUCS::V59, theVars->EnzymeAct);
      
      // DHAP+GAP --FBP
      SUCS::set(MOD::SUCS::SUCSV51,
                SUCS::get(MOD::SUCS::V51) *
                SUCS::get(MOD::SUCS::Vfactor51) *
                SUCS::get(MOD::SUCS::Vf_T51) *
                pow(SUCS::get(MOD::SUCS::Q10_51), tempRatio));
      // FBP --F6P + Pi
      SUCS::set(MOD::SUCS::SUCSV52,
                SUCS::get(MOD::SUCS::V52) *
                SUCS::get(MOD::SUCS::Vfactor52) *
                SUCS::get(MOD::SUCS::Vf_T52) *
                pow(SUCS::get(MOD::SUCS::Q10_52), tempRatio));
      // G1P+UTP --OPOP+UDPG
      SUCS::set(MOD::SUCS::SUCSV55,
                SUCS::get(MOD::SUCS::V55) *
                pow(SUCS::get(MOD::SUCS::Q10_55), tempRatio));
      // UDPG+F6P--SUCP + UDP
      SUCS::set(MOD::SUCS::SUCSV56,
                SUCS::get(MOD::SUCS::V56) *
                SUCS::get(MOD::SUCS::Vfactor56) *
                SUCS::get(MOD::SUCS::Vf_T56) *
                pow(SUCS::get(MOD::SUCS::Q10_56), tempRatio));
      // SUCP--Pi + SUC
      SUCS::set(MOD::SUCS::SUCSV57,
                SUCS::get(MOD::SUCS::V57) *
                SUCS::get(MOD::SUCS::Vfactor57) *
                SUCS::get(MOD::SUCS::Vf_T57) *
                pow(SUCS::get(MOD::SUCS::Q10_57), tempRatio));
      // F26BP--F6P + Pi
      SUCS::set(MOD::SUCS::SUCSV58,
                SUCS::get(MOD::SUCS::V58) *
                pow(SUCS::get(MOD::SUCS::Q10_58), tempRatio));
    } else {
      
      size_t i = 0;
      for (SUCS::iterator it = SUCS::begin(); it != SUCS::end(); it++) {
        if (i == 60)
          break;
        switch (it->first) {
        case (MOD::SUCS::V51) :
        case (MOD::SUCS::V52) :
        case (MOD::SUCS::V55) :
        case (MOD::SUCS::V56) :
        case (MOD::SUCS::V57) :
        case (MOD::SUCS::V58) : {
          CHECK_RATIO_IDX(i, 0, MOD::SUCS::V51);
          CHECK_RATIO_IDX(i, 1, MOD::SUCS::V52);
          CHECK_RATIO_IDX(i, 2, MOD::SUCS::V55);
          CHECK_RATIO_IDX(i, 3, MOD::SUCS::V56);
          CHECK_RATIO_IDX(i, 4, MOD::SUCS::V57);
          CHECK_RATIO_IDX(i, 5, MOD::SUCS::V58);
          if (theVars->GP == 0) {
            if (it->first == MOD::SUCS::V57)
              it->second = it->second * SUCS::get(MOD::SUCS::SC1) * theVars->SUCRatio[i];
            else
              it->second = it->second * SUCS::get(MOD::SUCS::SC) * theVars->SUCRatio[i];
          }
          i++;
          break;
        }
        case (MOD::SUCS::V59) :
        case (MOD::SUCS::V62) :
        case (MOD::SUCS::Vdhap_in) :
        case (MOD::SUCS::Vgap_in) :
        case (MOD::SUCS::Vpga_in) : {
          CHECK_RATIO_IDX(i, 6, MOD::SUCS::V59);
          CHECK_RATIO_IDX(i, 8, MOD::SUCS::V62);
          CHECK_RATIO_IDX(i, 9, MOD::SUCS::Vdhap_in);
          CHECK_RATIO_IDX(i, 10, MOD::SUCS::Vgap_in);
          CHECK_RATIO_IDX(i, 11, MOD::SUCS::Vpga_in);
          if (it->first == MOD::SUCS::V59)
            it->second = it->second * SUCS::get(MOD::SUCS::SC) * theVars->SUCRatio[i];
          else
            it->second = it->second * SUCS::get(MOD::SUCS::SC1) * theVars->SUCRatio[i];
          i++;
          if (it->first == MOD::SUCS::Vpga_in) {
            // SUCRatio 12, 13, & 14 are used by SUCS_Pool
            i = 15;
          }
          break;
        }
        // Next ratio is skipped
        case (MOD::SUCS::KE51) : {
          CHECK_RATIO_IDX(i, 19, MOD::SUCS::KE51);
          it->second = it->second * theVars->SUCRatio[i];
          i = i + 2;
          break;
        }
        // Skipped, but has SUCRatio entry
        case (MOD::SUCS::V60) :
        case (MOD::SUCS::Km592) :
        case (MOD::SUCS::KI592) :
        case (MOD::SUCS::Km601) :
        case (MOD::SUCS::Km602) :
        case (MOD::SUCS::Km603) :
        case (MOD::SUCS::Km604) :
        case (MOD::SUCS::KE60) : {
          CHECK_RATIO_IDX(i, 7, MOD::SUCS::V60);
          i++;
          break;
        }
        // Skipped and does not have SUCRatio entry
        case (MOD::SUCS::V61) :
        case (MOD::SUCS::KI583) :
        case (MOD::SUCS::Vmatpf) : {
          break;
        }
        default : {
          CHECK_RATIO_IDX(i, 15, MOD::SUCS::KE501);
          CHECK_RATIO_IDX(i, 21, MOD::SUCS::Km521);
          CHECK_RATIO_IDX(i, 22, MOD::SUCS::KI521);
          CHECK_RATIO_IDX(i, 30, MOD::SUCS::Km553);
          CHECK_RATIO_IDX(i, 40, MOD::SUCS::KE56);
          CHECK_RATIO_IDX(i, 45, MOD::SUCS::KI581);
          CHECK_RATIO_IDX(i, 46, MOD::SUCS::KI582);
          CHECK_RATIO_IDX(i, 47, MOD::SUCS::Km591);
          CHECK_RATIO_IDX(i, 49, MOD::SUCS::Km593);
          CHECK_RATIO_IDX(i, 50, MOD::SUCS::KI591);
          CHECK_RATIO_IDX(i, 52, MOD::SUCS::KE59);
          it->second = it->second * theVars->SUCRatio[i];
          i++;
        }
        }
      }

      theVars->SUCS_Pool[POOL::SUCS::ATc] *= theVars->SUCRatio[12];
      theVars->SUCS_Pool[POOL::SUCS::UTc] *= theVars->SUCRatio[13];
      theVars->SUCS_Pool[POOL::SUCS::PTc] *= theVars->SUCRatio[14];
      
    }
    SUCS::set(MOD::SUCS::KE5Ratio,
              1. + SUCS::get(MOD::SUCS::KE541) +
              1. / SUCS::get(MOD::SUCS::KE531));
    SUCS::set(MOD::SUCS::ADPc,
              theVars->SUCS_Pool.get(POOL::SUCS::ATc) -
              SUCS::get(MOD::SUCS::ATPc));

    
    theVars->SUCS_Pool.checkAlts("SUCS::_init:SUCS_Pool: ");
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(SUCS_Con);
#endif // CHECK_VALUE_SET_ALTS
}

void SUCS::_checkAlts(Variables *theVars, const std::string& context) {
    theVars->SUCS_Pool.checkAlts(context + "SUCS_Pool:");
}

void SUCS::_updateAlts(Variables *theVars, const std::string& context) {
    theVars->SUCS_Pool.updateAlts(context + "SUCS_Pool:");
}

void SUCS::_reset() {
    SUCS::KE501 = 0.;
    SUCS::KE51 = 0.;
    SUCS::KE52 = 0.;
    SUCS::KE531 = 0.;
    SUCS::KE541 = 0.;
    SUCS::KE55 = 0.;
    SUCS::KE56 = 0.;
    SUCS::KE57 = 0.;
    SUCS::KE59 = 0.;
    SUCS::KE61 = 0.;
    SUCS::KI521 = 0.;
    SUCS::KI522 = 0.;
    SUCS::KI523 = 0.;
    SUCS::KI561 = 0.;
    SUCS::KI562 = 0.;
    SUCS::KI563 = 0.;
    SUCS::KI564 = 0.;
    SUCS::KI565 = 0.;
    SUCS::KI581 = 0.;
    SUCS::KI582 = 0.;
    SUCS::KI591 = 0.;
    SUCS::Ki572 = 0.;
    SUCS::Km511 = 0.;
    SUCS::Km512 = 0.;
    SUCS::Km513 = 0.;
    SUCS::Km521 = 0.;
    SUCS::Km551 = 0.;
    SUCS::Km552 = 0.;
    SUCS::Km553 = 0.;
    SUCS::Km554 = 0.;
    SUCS::Km561 = 0.;
    SUCS::Km562 = 0.;
    SUCS::Km571 = 0.;
    SUCS::Km581 = 0.;
    SUCS::Km591 = 0.;
    SUCS::Km593 = 0.;
    SUCS::Km621 = 0.;
    SUCS::V51 = 0.;
    SUCS::V52 = 0.;
    SUCS::V55 = 0.;
    SUCS::V56 = 0.;
    SUCS::V57 = 0.;
    SUCS::V58 = 0.;
    SUCS::V59 = 0.;
    SUCS::V60 = 0.;
    SUCS::V61 = 0.;
    SUCS::V62 = 0.;
    SUCS::Vdhap_in = 0.;
    SUCS::Vgap_in = 0.;
    SUCS::Vpga_in = 0.;
    SUCS::Km592 = 0.;
    SUCS::KI592 = 0.;
    SUCS::Km601 = 0.;
    SUCS::Km602 = 0.;
    SUCS::Km603 = 0.;
    SUCS::Km604 = 0.;
    SUCS::KE60 = 0.;
    SUCS::Vfactor51 = 0.;
    SUCS::Vfactor52 = 0.;
    SUCS::Vfactor56 = 0.;
    SUCS::Vfactor57 = 0.;
    SUCS::Vfactor59 = 0.;
    SUCS::Vf_T51 = 0.;
    SUCS::Vf_T52 = 0.;
    SUCS::Vf_T56 = 0.;
    SUCS::Vf_T57 = 0.;
    SUCS::Vf_T59 = 0.;
    SUCS::UTPc = 0.;
    SUCS::ATPc = 0.;

    SUCS::SUCS2PS_Pic = 0.;

    SUCS::KE5Ratio = 0.;
    SUCS::ADPc = 0.;
    SUCS::SUCSV51 = 0.;
    SUCS::SUCSV52 = 0.;
    SUCS::SUCSV55 = 0.;
    SUCS::SUCSV56 = 0.;
    SUCS::SUCSV57 = 0.;
    SUCS::SUCSV58 = 0.;
    SUCS::TIME = 0.;
    SUCS::N = 0;
    conditions::SUCSCondition::reset();
}
