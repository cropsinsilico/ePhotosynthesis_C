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

SUCSCon SUCS_Ini(Variables *myVars) {
    myVars->SUCS_OLD_TIME = 0;
    myVars->SUCS_TIME_N = 1;

    myVars->KE501 = 1 / 0.05 * myVars->SUCRatio[15];
    myVars->Km511 = 0.02 * myVars->SUCRatio[16];
    myVars->Km512 = 0.3 * myVars->SUCRatio[17];
    myVars->Km513 = 0.4 * myVars->SUCRatio[18];
    myVars->KE51 = 12 * myVars->SUCRatio[19];
    //myVars->Km514 = 0.014 * myVars->SUCRatio[20];  // --unused
    myVars->Km521 = 0.0025 * myVars->SUCRatio[21];
    myVars->KI521 = 0.7 * myVars->SUCRatio[22];
    myVars->KI522 = 12 * myVars->SUCRatio[23];
    myVars->KI523 = 7 * pow(10, (-5)) * myVars->SUCRatio[24];
    myVars->KE52 = 6663 * myVars->SUCRatio[25];
    myVars->KE531 = 2.3 * myVars->SUCRatio[26];
    myVars->KE541 = 0.0584 * myVars->SUCRatio[27];
    myVars->Km551 = 0.14 * myVars->SUCRatio[28];
    myVars->Km552 = 0.1 * myVars->SUCRatio[29];
    myVars->Km553 = 0.11 * myVars->SUCRatio[30];
    myVars->Km554 = 0.12 * myVars->SUCRatio[31];
    myVars->KE55 = 0.31 * myVars->SUCRatio[32];
    myVars->Km561 = 0.8 * myVars->SUCRatio[33];
    myVars->Km562 = 2.4 * myVars->SUCRatio[34];
    myVars->KI561 = 0.7 * myVars->SUCRatio[35];
    myVars->KI562 = 0.8 * myVars->SUCRatio[36];
    myVars->KI563 = 0.4 * myVars->SUCRatio[37];
    myVars->KI564 = 11 * myVars->SUCRatio[38];
    myVars->KI565 = 50 * myVars->SUCRatio[39];
    myVars->KE56 = 10 * myVars->SUCRatio[40];
    myVars->Km571 = 0.35 * myVars->SUCRatio[41];
    myVars->Ki572 = 10 * myVars->SUCRatio[42];
    myVars->KE57 = 780 * myVars->SUCRatio[43];
    myVars->Km581 = 0.032 * myVars->SUCRatio[44];
    myVars->KI581 = 0.1 * myVars->SUCRatio[45];
    myVars->KI582 = 0.5 * myVars->SUCRatio[46];
    myVars->Km591 = 0.5 * myVars->SUCRatio[47];
    myVars->Km593 = 0.5 * myVars->SUCRatio[49];
    myVars->KI591 = 0.16 * myVars->SUCRatio[50];
    myVars->KE59 = 590 * myVars->SUCRatio[52];
    myVars->KE61 = 1.2 * 107 * myVars->SUCRatio[58];
    myVars->Km621 = 5 * myVars->SUCRatio[59];

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
        myVars->V51 = 0.107376831 * SC * myVars->SUCRatio[0];//	DHAP+GAP --FBP          // default 0.5
        myVars->V52 = 0.063979048 * SC * myVars->SUCRatio[1];//	FBP --F6P + Pi
        myVars->V55 = 0.115403205 * SC * myVars->SUCRatio[2];//	G1P+UTP --OPOP+UDPG
        myVars->V56 = 0.055503446 * SC * myVars->SUCRatio[3];//	UDPG+F6P--SUCP + UDP
        myVars->V57 = 0.55503446 * SC1 * myVars->SUCRatio[4];//	SUCP--Pi + SUC; 0.27 DEFALT
        myVars->V58 = 0.016819226 * SC * myVars->SUCRatio[5];//	F26BP--F6P + Pi
    }
    myVars->V59 = 0.03 * SC * myVars->SUCRatio[6];//	F6P + ATP --ADP + F26BP // defalut 0.03  (* 0.3)
    myVars->V60 = 6.1 * myVars->SUCRatio[7];//	ATP+UDP --UTP + ADP
    //myVars->V61 = 10000;	        //	POPO --2PO   // constant set in globals.hpp
    myVars->V62 = 2 * SC1 * myVars->SUCRatio[8];//	SUC Sink        0.9 works.
    myVars->Vdhap_in = 1.05 * SC1 * myVars->SUCRatio[9];//   DHAP export from chloroplast
    myVars->Vgap_in = 1.05 * SC1 * myVars->SUCRatio[10];//   GAP export from chloroplast
    myVars->Vpga_in = 1.05 * SC1 * myVars->SUCRatio[11];//   PGA export from chloropalst


    //////////////////////////////////////////////////////////////////
    // Here is some pool values      //
    //////////////////////////////////////////////////////////////////

    myVars->SUCS_Pool.ATc = 1.0 * myVars->SUCRatio[12];// mM
    myVars->SUCS_Pool.UTc = 1.5 * myVars->SUCRatio[13];// mM
    myVars->SUCS_Pool.PTc = 15 * myVars->SUCRatio[14];//

    return SUCS_Con;
}
