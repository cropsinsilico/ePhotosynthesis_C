#include "globals.hpp"
#include "PS.hpp"
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



PSCon PSInitial(varptr *myVars) {
    //global PSRatio;
    //global PS_C_CA;             //   Global constant for the total adenylates
    //global PS_C_CP;             //   Global constant for the total phosphate
    //global PS_C_CN;             //   Global constant for the total NADP+NADPH
    //global PS_PEXT;             //   Global constant for the cytosolic Phosphate concentration;
    
    myVars->PS_C_CP = 15 * myVars->PSRatio[0];
    myVars->PS_C_CA = 1.5 * myVars->PSRatio[1];
    myVars->PS_C_CN = 1 * myVars->PSRatio[2];
    myVars->PS_PEXT = 0.5 * myVars->PSRatio[3];
    
    //global PSPR_RA_CA;
    myVars->PSPR_RA_CA = myVars->PS_C_CA;
    
    const double RuBP = 2.000;
    const double PGA = 2.400;
    const double DPGA = 0.0011;
    const double T3P = 0.5;
    const double ADPG = 0.005;
    const double FBP = 0.670;
    const double E4P = 0.050;
    const double S7P = 2.000;
    const double SBP = 0.300;
    const double ATP = 0.68;
    const double NADPH = 0.21;
    const double HexP = 2.2;
    const double PenP = 0.25;
    
    const double CO2 = 0.012;
    const double O2 = 0.21 * 1.26;
    
    PSCon PS_con;
    PS_con.RuBP = RuBP;
    PS_con.PGA = PGA;
    PS_con.DPGA = DPGA;
    PS_con.T3P = T3P;
    PS_con.ADPG = ADPG;
    PS_con.FBP = FBP;
    PS_con.E4P = E4P;
    PS_con.S7P = S7P;
    PS_con.SBP = SBP;
    PS_con.ATP = ATP;
    PS_con.NADPH = NADPH;
    PS_con.CO2 = CO2;
    PS_con.O2 = O2;
    PS_con.HexP = HexP;
    PS_con.PenP = PenP;
    
    
    // Initialize the constants for the different reactions
    
    //global	KM11;
    //global	KM12;
    //global	KM13;
    //global  KI11;
    //global  KI12;
    //global  KI13;
    //global  KI14;
    //global  KI15;
    
    //global	KM21;
    //global	KM22;
    //global  KM23;
    
    //global	KM31a;
    //global	KM32b;
    
    //global	KM41;
    //global	KM42;
    //global  KE4;
    
    //global	KM51;
    //global	KM52;
    //global	KM53;
    //global  KE5;
    
    //global	KM61;
    //global  KI61;
    //global  KI62;
    
    //global	KM71;
    //global  KM72;
    //global	KM73;
    //global  KM74;
    
    //global	KM8;
    //global  KM81;
    //global  KM82;
    
    //global	KM9;
    //global  KI9;
    
    //global	KM10;
    //global  KM101;
    //global  KM102;
    //global  KM103;
    
    //global	KE11;
    
    //global	KE12;
    
    //global	KM131;
    //global	KM132;
    //global	KI131;
    //global	KI132;
    //global	KI133;
    //global	KI134;
    //global	KI135;
    
    //global	KM161;
    //global	KM162;
    
    //global	KE21;
    
    //global	KE22;
    
    
    
    //global	KM311;
    //global	KM312;
    //global	KM313;
    
    //global	KM32;
    //global	KM33;
    
    
    //global KE6;
    //global KE7;
    //global KE8;
    //global KE9;
    //global KE10;
    //global KE13;
    //global KE16;
    
    //global KM103;
    //global KM163;
    
    myVars->KM11 = 0.0115 * myVars->PSRatio[19];// 	CO2	1	RuBP+CO2->2PGA
    myVars->KM12 = 0.222 * myVars->PSRatio[20];//	O2	1	RuBP+CO2->2PGA
    myVars->KM13 = 0.02 * myVars->PSRatio[21];// 	RuBP	1	RuBP+CO2->2PGA
    
    myVars->KI11 = 0.84 * myVars->PSRatio[22];// PGA
    myVars->KI12 = 0.04 * myVars->PSRatio[23];// FBP
    myVars->KI13 = 0.075 * myVars->PSRatio[24];// SBP
    myVars->KI14 = 0.9 * myVars->PSRatio[25];// Pi
    myVars->KI15 = 0.07 * myVars->PSRatio[26];// NADPH
    
    myVars->KM21 = 0.240 * myVars->PSRatio[27];//	PGA	2	PGA+ATP <-> ADP + DPGA
    myVars->KM22 = 0.390 * myVars->PSRatio[28];// 	ATP	2	PGA+ATP <-> ADP + DPGA
    myVars->KM23 = 0.23 * myVars->PSRatio[29];//  ADP
    
    myVars->KM31a = 0.004 * myVars->PSRatio[30];//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
    myVars->KM32b = 0.1 * myVars->PSRatio[31];// 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP
    
    myVars->KM41 = 2.5 * myVars->PSRatio[32];//	DHAP	4	DHAP <->GAP
    myVars->KM42 = 0.68 * myVars->PSRatio[33];// 	GAP	4	DHAP <->GAP
    myVars->KE4 = 1 / 0.05 * myVars->PSRatio[34];//   Using the value from Patterson
    
    myVars->KM51 = 0.3 * myVars->PSRatio[35];//	GAP	5	GAP+DHAP <->FBP
    myVars->KM52 = 0.4 * myVars->PSRatio[36];// 	DHAP	5	GAP+DHAP <->FBP
    myVars->KM53 = 0.02 * myVars->PSRatio[37];//	FBP	5	GAP+DHAP <->FBP     // Original Value: 0.02
    myVars->KE5 = 7.100 * myVars->PSRatio[38];// Defult: 7.1
    
    myVars->KM61 = 0.033 * myVars->PSRatio[39];// 	FBP	6	FBP<->F6P+OP
    myVars->KI61 = 0.7 * myVars->PSRatio[40];//   F6P
    myVars->KI62 = 12 * myVars->PSRatio[41];//   Pi
    myVars->KE6 = 6.66 * pow(10, 5) * myVars->PSRatio[42];
    
    myVars->KM71 = 0.100 * myVars->PSRatio[43];//	Xu5P	7	F6P+GAP<->E4P+Xu5P      // jn
    myVars->KM72 = 0.100 * myVars->PSRatio[44];// 	E4P	7	F6P+GAP<->E4P+Xu5P
    myVars->KM73 = 0.1 * myVars->PSRatio[45];//   F6P This value was based on estimate
    myVars->KM74 = 0.1000 * myVars->PSRatio[46];// Estimate for GAP ORIGINAL 0.1
    myVars->KE7 = 0.1 * myVars->PSRatio[47];// The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA
    
    myVars->KM8 = 0.02 * myVars->PSRatio[48];//	SBP	8	E4P+DHAP<->SBP
    myVars->KM81 = 0.4 * myVars->PSRatio[49];// DHAP
    myVars->KM82 = 0.2 * myVars->PSRatio[50];// E4P estimate
    myVars->KE8 = 1.017 * myVars->PSRatio[51];// The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107
    
    myVars->KM9 = 0.05 * myVars->PSRatio[52];// 	SBP	9	SBP<->S7P+OP
    myVars->KI9 = 12 * myVars->PSRatio[53];//   The inibintion constant for Pi;
    myVars->KE9 = 6.66 * pow(10, 5) * myVars->PSRatio[54];
    
    myVars->KM10 = 0.5 * myVars->PSRatio[55];//	R5P	10	S7P+GAP<->Ri5P+Xu5P
    myVars->KM101 = 0.1 * myVars->PSRatio[56];//   Xu5P
    myVars->KM102 = 0.09 * myVars->PSRatio[57];//   Estimate for GAP
    myVars->KM103 = 0.015 * myVars->PSRatio[58];//   Estimate for S7P                                    // New
    myVars->KE10 = 1 / 0.85 * myVars->PSRatio[59];//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA
    
    myVars->KE11 = 0.4 * myVars->PSRatio[60];//	Equilibrium Constant	11	Ri5P<-->Ru5P
    myVars->KE12 = 0.67 * myVars->PSRatio[61];// 	Equilibrium Constant	12	Xu5P<-->Ru5P
    
    myVars->KM131 = 0.05 * myVars->PSRatio[62];//	Ru5P	13	Ru5P+ATP<->RuBP+ADP
    myVars->KM132 = 0.059 * myVars->PSRatio[63];// 	ATP	13	Ru5P+ATP<->RuBP+ADP
    myVars->KI131 = 2 * myVars->PSRatio[64];//	PGA	13	Ru5P+ATP<->RuBP+ADP
    myVars->KI132 = 0.7 * myVars->PSRatio[65];//	RuBP	13	Ru5P+ATP<->RuBP+ADP
    myVars->KI133 = 4 * myVars->PSRatio[66];//	Pi	13	Ru5P+ATP<->RuBP+ADP
    myVars->KI134 = 2.5 * myVars->PSRatio[67];//	ADP	13	Ru5P+ATP<->RuBP+ADP
    myVars->KI135 = 0.4 * myVars->PSRatio[68];//	ADP	13	Ru5P+ATP<->RuBP+ADP
    myVars->KE13 = 6.846 * pow(10, 3) * myVars->PSRatio[69];
    
    myVars->KM161 = 0.014 * myVars->PSRatio[70];//	ADP	16	ADP+Pi<->ATP
    myVars->KM162 = 0.3 * myVars->PSRatio[71];// 	Pi	16	ADP+Pi<-> ATP
    myVars->KM163 = 0.3 * myVars->PSRatio[72];//   ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
    myVars->KE16 = 5.734 * myVars->PSRatio[73];//   The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA
    
    
    myVars->KE21 = 2.3 * myVars->PSRatio[74];//	Equilibrium constant	21	F6P<->G6P
    myVars->KE22 = 0.058 * myVars->PSRatio[75];// 	Equilibrium constant	22	G6P<->G1P
    
    // KM231	=	0.08;		//	G1P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    // KM232	=	0.08;		// 	ATP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    // KA231	=	0.1;		//	PGA	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    // KA232	=	0.02;		// 	F6P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    // KA233	=	0.02;		//	FBP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    // KI23	=	10;		    // 	ADP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    
    myVars->KM311 = 0.077 * myVars->PSRatio[76];//	DHAP	31	DHAPi<->DHAPo
    myVars->KM312 = 0.63 * myVars->PSRatio[77];// 	Pi	31	DHAPi<->DHAPo
    myVars->KM313 = 0.74 * myVars->PSRatio[78];//	Pext	31	DHAPi<->DHAPo
    myVars->KM32 = 0.25 * myVars->PSRatio[79];// 	PGA	32	PGAi<->PGAo
    myVars->KM33 = 0.075 * myVars->PSRatio[80];//	GAP	33	GAPi<->GAPo
    
    
    // Now put in the constant for the new ADPG Pyrophosphorylase and starch
    // synthase
    
    // ATP + Glucose-1-Phosphate --> ADPG + PPi
    
    //global	KM231;
    //global	KM232;
    //global	KM233;
    //global	KM234;
    //global	KA231;
    //global	KI231;
    //global  KVmo;
    //global  KE23;
    
    myVars->KM231 = 0.031 * myVars->PSRatio[81];//	G1P	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    myVars->KM232 = 0.045 * myVars->PSRatio[82];// 	ATP	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    myVars->KM233 = 0.14 * myVars->PSRatio[83];//	ADPG	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    myVars->KM234 = 0.8 * myVars->PSRatio[84];// 	PPi	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    myVars->KE23 = 7.6 * pow(10, (-3)) * myVars->PSRatio[85];
    
    
    myVars->KA231 = 0.23 * myVars->PSRatio[86];//	PGA	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989
    myVars->KI231 = 0.9 * myVars->PSRatio[87];//0.9 ;       //   Pi	23	G1P+ATP<->ADPG + PPi        Laisk et al 1989 WY201803
    myVars->KVmo = 0.007 * myVars->PSRatio[88];//   The minimum maximum velocity        Laisk et al 1989
    // ADPG --> ADP + Gn     // The starch synthesis reaction 24.     Laisk et al
    // 1989
    
    //global KM241;
    //global KM242;
    //global KE24;
    
    myVars->KM241 = 0.2 * myVars->PSRatio[89];//   ADPG    ADPG --> ADP + Gn       Laisk et al 1989
    myVars->KM242 = 0.6 * myVars->PSRatio[90];//   ADP     ADPG --> ADP + Gn       Laisk et al 1989
    myVars->KE24 = 7.4 * pow(10, 5) * myVars->PSRatio[91];
    
    //global KE25;
    myVars->KE25 = 1.2 * 107 * myVars->PSRatio[92];
    
    // Initialize the Vmax for different reactions
    
    //global	V1;
    //global	V2;
    //global	V3;
    //global	V5;
    //global	V6;
    //global	V7;
    //global	V8;
    //global	V9;
    //global	V10;
    //global	V11;
    //global	V12;
    //global	V13;
    //global	V16;
    //global	V21;
    //global	V22;
    //global	V23;
    //global	V31;
    //global	V32;
    //global	V33;
    //global  V24;
    
    
    //global GP;
    if (myVars->GP == 0) {
        // FC is a fussl factor here.
        //const double FC = 1;       // Defulat is 2.5.
        //fc16 = 1;     // default 1.5.
        // Initialize the values of the global variables
        const double SC = 1;        // Scalling coefficient for the stroma volume per mg chl. defualt 2
        const double SC1 = 1;
        
        //const double SC222 = 2;
        const double STOM1 = 1;
        const double STOM2 = 1;
        
        myVars->V1 = 2.93 * SC1 / STOM1 * myVars->PSRatio[4];//	(Harris & Koniger, 1997)
        myVars->V2 = 30.15 * SC * STOM2 * myVars->PSRatio[5];//	(Harris & Koniger, 1997)
        myVars->V3 = 4.04 * SC * STOM2 * myVars->PSRatio[6];// 1.57*SC	    ;	//	(Harris & Koniger, 1997)
        myVars->V5 = 1.22 * SC * myVars->PSRatio[7];//	(Harris & Koniger, 1997)
        myVars->V6 = 0.734 * SC / STOM1 * myVars->PSRatio[8];//	(Harris & Koniger, 1997)
        myVars->V7 = 3.12 * SC * 4 * myVars->PSRatio[9];//	(Harris & Koniger, 1997)
        myVars->V8 = 1.22 * SC * myVars->PSRatio[10];//	(Harris & Koniger, 1997)
        myVars->V9 = 0.32 * 3 * myVars->PSRatio[11];// 0.17*SC *FC	;	//	(Harris & Koniger, 1997) *3.
        myVars->V10 = myVars->V7;	//	(Harris & Koniger, 1997)
        myVars->V13 = 10.81 * SC1 * myVars->PSRatio[12];//	(Harris & Koniger, 1997)
        myVars->V16 = 5.47 * myVars->PSRatio[13];// (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
        myVars->V23 = 2 * myVars->PSRatio[14];
        
        
    }
    myVars->V24 = 2 * myVars->PSRatio[15];
    myVars->V31 = 1.0 * myVars->PSRatio[16] * 20;
    myVars->V32 = 1.0 * myVars->PSRatio[17];
    myVars->V33 = 1.0 * myVars->PSRatio[18] * 20;//WY 2018103
    
    
    //global Cond_V16;        // This parameter is used to modifying the V16 if needed.
    //Cond_myVars->V16 = myVars->V16;  // --unused
    
    //global PS2SUCSV32;
    myVars->PS2SUCSV32 = myVars->V32;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the location for transfering variables//
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //global PS2RA_RuBP_ini;
    myVars->PS2RA_RuBP_ini = RuBP;
    
    //global BF_FI_com;
    //global PS2BF_ATP;
    //global PS2BF_ADP;
    //global PS2BF_Pi;
    
    //myVars->PS2BF_ATP = ATP;  // --unused
    myVars->PS2BF_ADP = myVars->PS_C_CA - ATP;
    
    //global PS2PR_V1;
    myVars->PS2PR_V1 = myVars->V1;
    
    //global CO2_PS2StomCond;
    //myVars->CO2_PS2StomCond = CO2;  // --unused
    
    //global PS2SUCS_PGA;
    //myVars->PS2SUCS_PGA = PGA;  // --unused
    return PS_con;
}
