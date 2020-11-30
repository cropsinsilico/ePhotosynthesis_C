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

double SUCS::KE501 = 0.;
double SUCS::KE51 = 0.;
double SUCS::KE52 = 0.;
double SUCS::KE531 = 0.;
double SUCS::KE541 = 0.;
double SUCS::KE55 = 0.;
double SUCS::KE56 = 0.;
double SUCS::KE57 = 0.;
double SUCS::KE59 = 0.;
double SUCS::KE61 = 0.;
double SUCS::KI521 = 0.;
double SUCS::KI522 = 0.;
double SUCS::KI523 = 0.;
double SUCS::KI561 = 0.;
double SUCS::KI562 = 0.;
double SUCS::KI563 = 0.;
double SUCS::KI564 = 0.;
double SUCS::KI565 = 0.;
double SUCS::KI581 = 0.;
double SUCS::KI582 = 0.;
double SUCS::KI591 = 0.;
double SUCS::Ki572 = 0.;
double SUCS::Km511 = 0.;
double SUCS::Km512 = 0.;
double SUCS::Km513 = 0.;
double SUCS::Km521 = 0.;
double SUCS::Km551 = 0.;
double SUCS::Km552 = 0.;
double SUCS::Km553 = 0.;
double SUCS::Km554 = 0.;
double SUCS::Km561 = 0.;
double SUCS::Km562 = 0.;
double SUCS::Km571 = 0.;
double SUCS::Km581 = 0.;
double SUCS::Km591 = 0.;
double SUCS::Km593 = 0.;
double SUCS::Km621 = 0.;
double SUCS::V51 = 0.;
double SUCS::V52 = 0.;
double SUCS::V55 = 0.;
double SUCS::V56 = 0.;
double SUCS::V57 = 0.;
double SUCS::V58 = 0.;
double SUCS::V59 = 0.;
double SUCS::V62 = 0.;
double SUCS::Vdhap_in = 0.;
double SUCS::Vgap_in = 0.;
double SUCS::Vpga_in = 0.;

SUCSCon SUCS::SUCS_Ini(Variables *myVars) {
    myVars->SUCS_OLD_TIME = 0;
    myVars->SUCS_TIME_N = 1;

    KE501 = 1 / 0.05 * myVars->SUCRatio[15];
    Km511 = 0.02 * myVars->SUCRatio[16];
    Km512 = 0.3 * myVars->SUCRatio[17];
    Km513 = 0.4 * myVars->SUCRatio[18];
    KE51 = 12 * myVars->SUCRatio[19];
    //myVars->Km514 = 0.014 * myVars->SUCRatio[20];  // --unused
    Km521 = 0.0025 * myVars->SUCRatio[21];
    KI521 = 0.7 * myVars->SUCRatio[22];
    KI522 = 12 * myVars->SUCRatio[23];
    KI523 = 7 * pow(10, (-5)) * myVars->SUCRatio[24];
    KE52 = 6663 * myVars->SUCRatio[25];
    KE531 = 2.3 * myVars->SUCRatio[26];
    KE541 = 0.0584 * myVars->SUCRatio[27];
    Km551 = 0.14 * myVars->SUCRatio[28];
    Km552 = 0.1 * myVars->SUCRatio[29];
    Km553 = 0.11 * myVars->SUCRatio[30];
    Km554 = 0.12 * myVars->SUCRatio[31];
    KE55 = 0.31 * myVars->SUCRatio[32];
    Km561 = 0.8 * myVars->SUCRatio[33];
    Km562 = 2.4 * myVars->SUCRatio[34];
    KI561 = 0.7 * myVars->SUCRatio[35];
    KI562 = 0.8 * myVars->SUCRatio[36];
    KI563 = 0.4 * myVars->SUCRatio[37];
    KI564 = 11 * myVars->SUCRatio[38];
    KI565 = 50 * myVars->SUCRatio[39];
    KE56 = 10 * myVars->SUCRatio[40];
    Km571 = 0.35 * myVars->SUCRatio[41];
    Ki572 = 10 * myVars->SUCRatio[42];
    KE57 = 780 * myVars->SUCRatio[43];
    Km581 = 0.032 * myVars->SUCRatio[44];
    KI581 = 0.1 * myVars->SUCRatio[45];
    KI582 = 0.5 * myVars->SUCRatio[46];
    Km591 = 0.5 * myVars->SUCRatio[47];
    Km593 = 0.5 * myVars->SUCRatio[49];
    KI591 = 0.16 * myVars->SUCRatio[50];
    KE59 = 590 * myVars->SUCRatio[52];
    KE61 = 1.2 * 107 * myVars->SUCRatio[58];
    Km621 = 5 * myVars->SUCRatio[59];

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves of active enzyme in a dark adapted leaves;
    //	mM

    // Assign value to a variable that is transferred to the program
    SUCSCon SUCS_Con;
    SUCS_Con.T3Pc = 2.;
    SUCS_Con.FBPc = 2.;
    SUCS_Con.HexPc = 5.8;
    SUCS_Con.F26BPc = 7.8 * pow(10, -6);
    SUCS_Con.ATPc = 0.4;
    SUCS_Con.ADPc = 0.4;
    SUCS_Con.UDPGc = 0.57;
    SUCS_Con.UTPc = 0.75;
    SUCS_Con.SUCP = 0.;
    SUCS_Con.SUC = 0.;
    SUCS_Con.PGAc = 0.5;


    // The following calculate the total concentration of different enzymes.
    const double SC = 10;
    const double SC1 = 1;

    if (myVars->GP == 0) {
        // Unit: mmol l-1 s-1;
        V51 = 0.107376831 * SC * myVars->SUCRatio[0];//	DHAP+GAP --FBP          // default 0.5
        V52 = 0.063979048 * SC * myVars->SUCRatio[1];//	FBP --F6P + Pi
        V55 = 0.115403205 * SC * myVars->SUCRatio[2];//	G1P+UTP --OPOP+UDPG
        V56 = 0.055503446 * SC * myVars->SUCRatio[3];//	UDPG+F6P--SUCP + UDP
        V57 = 0.55503446 * SC1 * myVars->SUCRatio[4];//	SUCP--Pi + SUC; 0.27 DEFALT
        V58 = 0.016819226 * SC * myVars->SUCRatio[5];//	F26BP--F6P + Pi
    }
    V59 = 0.03 * SC * myVars->SUCRatio[6];//	F6P + ATP --ADP + F26BP // defalut 0.03  (* 0.3)
    //myVars->V60 = 6.1 * myVars->SUCRatio[7];//	ATP+UDP --UTP + ADP
    //myVars->V61 = 10000;	        //	POPO --2PO   // constant set in globals.hpp
    V62 = 2 * SC1 * myVars->SUCRatio[8];//	SUC Sink        0.9 works.
    Vdhap_in = 1.05 * SC1 * myVars->SUCRatio[9];//   DHAP export from chloroplast
    Vgap_in = 1.05 * SC1 * myVars->SUCRatio[10];//   GAP export from chloroplast
    Vpga_in = 1.05 * SC1 * myVars->SUCRatio[11];//   PGA export from chloropalst


    //////////////////////////////////////////////////////////////////
    // Here is some pool values      //
    //////////////////////////////////////////////////////////////////

    myVars->SUCS_Pool.ATc = 1.0 * myVars->SUCRatio[12];// mM
    myVars->SUCS_Pool.UTc = 1.5 * myVars->SUCRatio[13];// mM
    myVars->SUCS_Pool.PTc = 15 * myVars->SUCRatio[14];//

    return SUCS_Con;
}
