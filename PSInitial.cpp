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

double PS::KA231 = 0.;
double PS::KE11 = 0.;
double PS::KE12 = 0.;
double PS::KE13 = 0.;
double PS::KE16 = 0.;
double PS::KE21 = 0.;
double PS::KE22 = 0.;
double PS::KE23 = 0.;
double PS::KE25 = 0.;
double PS::KE4 = 0.;
double PS::KE5 = 0.;
double PS::KE6 = 0.;
double PS::KE7 = 0.;
double PS::KE8 = 0.;
double PS::KE9 = 0.;
double PS::KI11 = 0.;
double PS::KI12 = 0.;
double PS::KI13 = 0.;
double PS::KI131 = 0.;
double PS::KI132 = 0.;
double PS::KI133 = 0.;
double PS::KI134 = 0.;
double PS::KI135 = 0.;
double PS::KI14 = 0.;
double PS::KI15 = 0.;
double PS::KI231 = 0.;
double PS::KI61 = 0.;
double PS::KI62 = 0.;
double PS::KI9 = 0.;
double PS::KM10 = 0.;
double PS::KM101 = 0.;
double PS::KM102 = 0.;
double PS::KM103 = 0.;
double PS::KM13 = 0.;
double PS::KM131 = 0.;
double PS::KM132 = 0.;
double PS::KM161 = 0.;
double PS::KM162 = 0.;
double PS::KM163 = 0.;
double PS::KM21 = 0.;
double PS::KM22 = 0.;
double PS::KM23 = 0.;
double PS::KM231 = 0.;
double PS::KM232 = 0.;
double PS::KM233 = 0.;
double PS::KM234 = 0.;
double PS::KM241 = 0.;
double PS::KM311 = 0.;
double PS::KM313 = 0.;
double PS::KM31a = 0.;
double PS::KM32 = 0.;
double PS::KM32b = 0.;
double PS::KM33 = 0.;
double PS::KM51 = 0.;
double PS::KM52 = 0.;
double PS::KM53 = 0.;
double PS::KM61 = 0.;
double PS::KM81 = 0.;
double PS::KM82 = 0.;
double PS::KM9 = 0.;
double PS::KVmo = 0.;
double PS::PS_C_CA = 0.;
double PS::PS_C_CP = 0.;
double PS::PS_PEXT = 0.;
double PS::V24 = 0.;
double PS::V31 = 0.;
double PS::V32 = 0.;
double PS::V33 = 0.;
double PS::V5 = 0.;
double PS::V7 = 0.;
double PS::V8 = 0.;

PSCon PS::PSInitial(Variables *myVars) {
    PS_C_CP = 15 * myVars->PSRatio[0];   //   Global constant for the total phosphate
    PS_C_CA = 1.5 * myVars->PSRatio[1];    //   Global constant for the total adenylates
    PS_PEXT = 0.5 * myVars->PSRatio[3];   //   Global constant for the cytosolic Phosphate concentration;

    myVars->PSPR_RA_CA = PS_C_CA;

    PSCon PS_con;
    PS_con.RuBP = 2.000;
    PS_con.PGA = 2.400;
    PS_con.DPGA = 0.0011;
    PS_con.T3P = 0.5;
    PS_con.ADPG = 0.005;
    PS_con.FBP = 0.670;
    PS_con.E4P = 0.050;
    PS_con.S7P = 2.000;
    PS_con.SBP = 0.300;
    PS_con.ATP = 0.68;
    PS_con.NADPH = 0.21;
    PS_con.CO2 = 0.012;
    PS_con.O2 = 0.21 * 1.26;
    PS_con.HexP = 2.2;
    PS_con.PenP = 0.25;


    // Initialize the constants for the different reactions
    myVars->KM11 = 0.0115 * myVars->PSRatio[19];// 	CO2	1	RuBP+CO2->2PGA
    myVars->KM12 = 0.222 * myVars->PSRatio[20];//	O2	1	RuBP+CO2->2PGA
    KM13 = 0.02 * myVars->PSRatio[21];// 	RuBP	1	RuBP+CO2->2PGA

    KI11 = 0.84 * myVars->PSRatio[22];// PGA
    KI12 = 0.04 * myVars->PSRatio[23];// FBP
    KI13 = 0.075 * myVars->PSRatio[24];// SBP
    KI14 = 0.9 * myVars->PSRatio[25];// Pi
    KI15 = 0.07 * myVars->PSRatio[26];// NADPH

    KM21 = 0.240 * myVars->PSRatio[27];//	PGA	2	PGA+ATP <-> ADP + DPGA
    KM22 = 0.390 * myVars->PSRatio[28];// 	ATP	2	PGA+ATP <-> ADP + DPGA
    KM23 = 0.23 * myVars->PSRatio[29];//  ADP

    KM31a = 0.004 * myVars->PSRatio[30];//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
    KM32b = 0.1 * myVars->PSRatio[31];// 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP

    KE4 = 1 / 0.05 * myVars->PSRatio[34];//   Using the value from Patterson

    KM51 = 0.3 * myVars->PSRatio[35];//	GAP	5	GAP+DHAP <->FBP
    KM52 = 0.4 * myVars->PSRatio[36];// 	DHAP	5	GAP+DHAP <->FBP
    KM53 = 0.02 * myVars->PSRatio[37];//	FBP	5	GAP+DHAP <->FBP     // Original Value: 0.02
    KE5 = 7.100 * myVars->PSRatio[38];// Defult: 7.1

    KM61 = 0.033 * myVars->PSRatio[39];// 	FBP	6	FBP<->F6P+OP
    KI61 = 0.7 * myVars->PSRatio[40];//   F6P
    KI62 = 12 * myVars->PSRatio[41];//   Pi
    KE6 = 6.66 * pow(10, 5) * myVars->PSRatio[42];

    KE7 = 0.1 * myVars->PSRatio[47];// The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA

    KM81 = 0.4 * myVars->PSRatio[49];// DHAP
    KM82 = 0.2 * myVars->PSRatio[50];// E4P estimate
    KE8 = 1.017 * myVars->PSRatio[51];// The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107

    KM9 = 0.05 * myVars->PSRatio[52];// 	SBP	9	SBP<->S7P+OP
    KI9 = 12 * myVars->PSRatio[53];//   The inibintion constant for Pi;
    KE9 = 6.66 * pow(10, 5) * myVars->PSRatio[54];

    KM10 = 0.5 * myVars->PSRatio[55];//	R5P	10	S7P+GAP<->Ri5P+Xu5P
    KM101 = 0.1 * myVars->PSRatio[56];//   Xu5P
    KM102 = 0.09 * myVars->PSRatio[57];//   Estimate for GAP
    KM103 = 0.015 * myVars->PSRatio[58];//   Estimate for S7P                                    // New
    //myVars->KE10 = 1 / 0.85 * myVars->PSRatio[59];//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA

    KE11 = 0.4 * myVars->PSRatio[60];//	Equilibrium Constant	11	Ri5P<-->Ru5P
    KE12 = 0.67 * myVars->PSRatio[61];// 	Equilibrium Constant	12	Xu5P<-->Ru5P

    KM131 = 0.05 * myVars->PSRatio[62];//	Ru5P	13	Ru5P+ATP<->RuBP+ADP
    KM132 = 0.059 * myVars->PSRatio[63];// 	ATP	13	Ru5P+ATP<->RuBP+ADP
    KI131 = 2 * myVars->PSRatio[64];//	PGA	13	Ru5P+ATP<->RuBP+ADP
    KI132 = 0.7 * myVars->PSRatio[65];//	RuBP	13	Ru5P+ATP<->RuBP+ADP
    KI133 = 4 * myVars->PSRatio[66];//	Pi	13	Ru5P+ATP<->RuBP+ADP
    KI134 = 2.5 * myVars->PSRatio[67];//	ADP	13	Ru5P+ATP<->RuBP+ADP
    KI135 = 0.4 * myVars->PSRatio[68];//	ADP	13	Ru5P+ATP<->RuBP+ADP
    KE13 = 6.846 * pow(10, 3) * myVars->PSRatio[69];

    KM161 = 0.014 * myVars->PSRatio[70];//	ADP	16	ADP+Pi<->ATP
    KM162 = 0.3 * myVars->PSRatio[71];// 	Pi	16	ADP+Pi<-> ATP
    KM163 = 0.3 * myVars->PSRatio[72];//   ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
    KE16 = 5.734 * myVars->PSRatio[73];//   The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA


    KE21 = 2.3 * myVars->PSRatio[74];//	Equilibrium constant	21	F6P<->G6P
    KE22 = 0.058 * myVars->PSRatio[75];// 	Equilibrium constant	22	G6P<->G1P

    KM311 = 0.077 * myVars->PSRatio[76];//	DHAP	31	DHAPi<->DHAPo
    //myVars->KM312 = 0.63 * myVars->PSRatio[77];// 	Pi	31	DHAPi<->DHAPo
    KM313 = 0.74 * myVars->PSRatio[78];//	Pext	31	DHAPi<->DHAPo
    KM32 = 0.25 * myVars->PSRatio[79];// 	PGA	32	PGAi<->PGAo
    KM33 = 0.075 * myVars->PSRatio[80];//	GAP	33	GAPi<->GAPo


    // Now put in the constant for the new ADPG Pyrophosphorylase and starch
    // synthase

    // ATP + Glucose-1-Phosphate --> ADPG + PPi

    KM231 = 0.031 * myVars->PSRatio[81];//	G1P	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    KM232 = 0.045 * myVars->PSRatio[82];// 	ATP	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    KM233 = 0.14 * myVars->PSRatio[83];//	ADPG	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    KM234 = 0.8 * myVars->PSRatio[84];// 	PPi	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    KE23 = 7.6 * pow(10, (-3)) * myVars->PSRatio[85];


    KA231 = 0.23 * myVars->PSRatio[86];//	PGA	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    KI231 = 0.9 * myVars->PSRatio[87];//0.9 ;       //   Pi	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989 WY201803
    KVmo = 0.007 * myVars->PSRatio[88];//   The minimum maximum velocity        Laisk et al 1989
    // ADPG --> ADP + Gn     // The starch synthesis reaction 24.     Laisk et al
    // 1989

    KM241 = 0.2 * myVars->PSRatio[89];//   ADPG    ADPG --> ADP + Gn       Laisk et al 1989

    KE25 = 1.2 * 107 * myVars->PSRatio[92];

    // Initialize the Vmax for different reactions

    if (myVars->GP == 0) {
        // Initialize the values of the global variables
        const double SC = 1;        // Scalling coefficient for the stroma volume per mg chl. defualt 2
        const double SC1 = 1;
        const double STOM1 = 1;
        const double STOM2 = 1;

        myVars->V1 = 2.93 * SC1 / STOM1 * myVars->PSRatio[4];//	(Harris & Koniger, 1997)
        myVars->V2 = 30.15 * SC * STOM2 * myVars->PSRatio[5];//	(Harris & Koniger, 1997)
        myVars->V3 = 4.04 * SC * STOM2 * myVars->PSRatio[6];// 1.57*SC	    ;	//	(Harris & Koniger, 1997)
        V5 = 1.22 * SC * myVars->PSRatio[7];//	(Harris & Koniger, 1997)
        myVars->V6 = 0.734 * SC / STOM1 * myVars->PSRatio[8];//	(Harris & Koniger, 1997)
        V7 = 3.12 * SC * 4 * myVars->PSRatio[9];//	(Harris & Koniger, 1997)
        V8 = 1.22 * SC * myVars->PSRatio[10];//	(Harris & Koniger, 1997)
        myVars->V9 = 0.32 * 3 * myVars->PSRatio[11];// 0.17*SC *FC	;	//	(Harris & Koniger, 1997) *3.
        //myVars->V10 = myVars->V7;	//	(Harris & Koniger, 1997)
        myVars->V13 = 10.81 * SC1 * myVars->PSRatio[12];//	(Harris & Koniger, 1997)
        myVars->V16 = 5.47 * myVars->PSRatio[13];// (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
        myVars->V23 = 2 * myVars->PSRatio[14];
    }
    V24 = 2 * myVars->PSRatio[15];
    V31 = 1.0 * myVars->PSRatio[16] * 20;
    V32 = 1.0 * myVars->PSRatio[17];
    V33 = 1.0 * myVars->PSRatio[18] * 20;//WY 2018103

    myVars->PS2SUCSV32 = V32;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the location for transfering variables//
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    myVars->PS2RA_RuBP_ini = PS_con.RuBP;

    myVars->PS2BF_ADP = PS_C_CA - PS_con.ATP;

    return PS_con;
}
