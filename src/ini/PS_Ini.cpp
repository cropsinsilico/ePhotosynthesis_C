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
#include "modules/PS.hpp"

#define R 8.314
#define c_c 38.28
#define dHa_c 80.99
#define c_o 14.68
#define dHa_o 23.72


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
double PS::V2 = 0.;
double PS::V3 = 0.;
double PS::V6 = 0.;
double PS::V9 = 0.;
double PS::V13 = 0.;
double PS::V16 = 0.;
double PS::V23 = 0.;
double PS::V1 = 0.;
double PS::V5 = 0.;
double PS::V7 = 0.;
double PS::V8 = 0.;
double PS::V10 = 0.;
double PS::PS_C_CN = 0;
double PS::KA232 = 0;
double PS::KA233 = 0;
double PS::KI23 = 0;
double PS::KM312 = 0;
double PS::KE10 = 0;
double PS::KM11 = 0;
double PS::KM12 = 0;
double PS::KM71 = 0;
double PS::KM72 = 0;
double PS::KM73 = 0;
double PS::KM74 = 0;
double PS::Vfactor1 = 1;
double PS::Vfactor2 = 1;
double PS::Vfactor3 = 1;
double PS::Vfactor5 = 1;
double PS::Vfactor7 = 1;
double PS::Vfactor13 = 1;
double PS::Vfactor23 = 1;
double PS::Vf_T3 = 1;
double PS::Vf_T2 = 1;
double PS::Vf_T1 = 1;
double PS::Vf_T6 = 1;
double PS::Vf_T5 = 1;
double PS::Vf_T9 = 1;
double PS::Vf_T13 = 1;
double PS::Vf_T23 = 1;
double PS::PsV1 = 0.;
double PS::_NADPH = 0.;
double PS::PiTc = 0.;
double PS::V1Reg = 0.;
double PS::TIME = 0.;
double PS::alfa = 0;
double PS::fc = 0;
double PS::Theta = 0;
double PS::beta = 0;
double PS::Jmax = 0.;

size_t PS::N = 1;
const size_t PSCondition::count = 12;
bool PSCondition::useC3 = false;
bool PS::useC3 = false;

arr PS::Param = {0, 0};

PSCondition* PS::_init(Variables *theVars) {
    setC3(theVars->useC3);
    PSCondition* PS_con = new PSCondition();
    PS_con->RuBP = 2.000;
    PS_con->PGA = 2.400;
    PS_con->DPGA = 0.0011;
    PS_con->T3P = 0.5;
    if (!theVars->useC3) {
        PS_con->ADPG = 0.005;
    } else {
        PS_con->ADPG = 0.;
    }
    PS_con->FBP = 0.670;
    PS_con->E4P = 0.050;
    PS_con->S7P = 2.000;
    PS_con->SBP = 0.300;
    PS_con->ATP = 0.68;
    PS::_NADPH = 0.21;
    PS_con->HexP = 2.2;
    PS_con->PenP = 0.25;

    if (theVars->useC3) {
        PS_C_CP = 22;    //   Global constant for the total phosphate
        PS_C_CA = 1.5;   //   Global constant for the total adenylates
        PS_C_CN = 1.;    //   Global constant for the cytosolic Phosphate concentration;
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0) {
            Vfactor1 = theVars->VfactorCp[0];
            Vfactor2 = theVars->VfactorCp[2];
            Vfactor3 = theVars->VfactorCp[3];
            Vfactor5 = theVars->VfactorCp[5];
            Vfactor7 = theVars->VfactorCp[7];
            Vfactor13 = theVars->VfactorCp[12];
            Vfactor23 = theVars->VfactorCp[16];
        }
        if (theVars->GRNT == 1 && theVars->Tp > 25) {
            Vf_T3 = theVars->VfactorT[9];
            Vf_T2 = theVars->VfactorT[21];
            Vf_T1 = theVars->VfactorT[24];
            Vf_T6 = theVars->VfactorT[16];
            Vf_T9 = theVars->VfactorT[25];
            Vf_T5 = theVars->VfactorT[1];
            Vf_T23 = theVars->VfactorT[2];
            Vf_T13 = theVars->VfactorT[3];

        }

        //PsKM11_0	=	;
        //PsKM12_0	=	;		//	O2	1	RuBP+CO2->2PGA  0.28 DEFAUL.
        KM11=0.0097*exp(c_c-dHa_c*1000/(R*(theVars->Tp+273.15)))/272.38;
        KM12=0.244*exp(c_o-dHa_o*1000/(R*(theVars->Tp+273.15)))/165.82;
        //std::cout << c_o << "  " << dHa_o << "  " << R << "  " << theVars->Tp << "  " << c_c << "  " << dHa_c << std::endl;
        KM13	=	0.02;		// 	RuBP	1	RuBP+CO2->2PGA
        KI11    =   0.84   ;    // PGA
        KI12    =0.04   ;       // FBP
        KI13    = 0.075 ;       // SBP
        KI14    = 0.9   ;       // Pi
        KI15    = 0.07  ;       // NADPH
        KM21	=	0.240;		//	PGA	2	PGA+ATP <-> ADP + DPGA
        KM22	=	0.390;		// 	ATP	2	PGA+ATP <-> ADP + DPGA
        KM23    =   0.23  ;        //  ADP
        KM31a	=	0.004;		//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
        KM32b	=	0.1	;	    // 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP
        //KM41	=	2.5	;	    //	DHAP	4	DHAP <->GAP
        //KM42	=	0.68;		// 	GAP	4	DHAP <->GAP
        KE4     =   0.05;       //   Using the value from Patterson
        KM51	=	0.3	;	    //	GAP	5	GAP+DHAP <->FBP
        KM52	=	0.4	;	    // 	DHAP	5	GAP+DHAP <->FBP
        KM53	=	0.02;		//	FBP	5	GAP+DHAP <->FBP     // Original Value: 0.02
        KE5     = 7.100;          // Defult: 7.1
        KM61	=	0.033;		// 	FBP	6	FBP<->F6P+OP
        KI61    = 0.7   ;       //   F6P
        KI62    = 12    ;       //   Pi
        KE6     =   6.66 * pow(10, 5);    // The equilibrium constant for this reaction        // New    mM     Laisk or Bassham and Krause 1969 BBA
        KM71	=	0.100;		//	Xu5P	7	F6P+GAP<->E4P+Xu5P      // jn
        KM72	=	0.100;		// 	E4P	7	F6P+GAP<->E4P+Xu5P
        KM73    = 0.1;         //   F6P This value was based on estimate
        KM74    = 0.1000;         // Estimate for GAP ORIGINAL 0.1
        KE7     =   10 ;       // The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA
        //PsKM8	    =	0.02;		//	SBP	8	E4P+DHAP<->SBP
        KM81    = 0.4   ;       // DHAP
        KM82    = 0.2   ;       // E4P estimate
        KE8     = 1.017 ;     // The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107
        KM9	    =	0.05;		// 	SBP	9	SBP<->S7P+OP
        KI9     = 12    ;       //   The inibintion constant for Pi;
        KE9     =   6.66 * pow(10, 5) ; // The equilibrium constant of this reaction           // New   mM      Laisk  Bassham and Krause 1969 BBA
        KM10	=	1.5	;	    //	R5P	10	S7P+GAP<->Ri5P+Xu5P
        KM101   =   0.1 ;       //   Xu5P
        KM102   = 0.072 ;       //   Estimate for GAP
        KM103   = 0.46 ;        //   Estimate for S7P                                    // New
        KE10    = 1/0.85 ;      //   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA
        KE11	=	0.4	;	    //	Equilibrium Constant	11	Ri5P<-->Ru5P
        KE12	=	0.67;		// 	Equilibrium Constant	12	Xu5P<-->Ru5P

        KM131	=	0.05;		    //	Ru5P	13	Ru5P+ATP<->RuBP+ADP
        KM132	=	0.059;		    // 	ATP	13	Ru5P+ATP<->RuBP+ADP
        KI131	=	2	;			//	PGA	13	Ru5P+ATP<->RuBP+ADP
        KI132	=	0.7	;			//	RuBP	13	Ru5P+ATP<->RuBP+ADP
        KI133	=	4	;			//	Pi	13	Ru5P+ATP<->RuBP+ADP
        KI134	=	2.5	;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
        KI135	=	0.4	;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
        KE13    =   6.846 * pow(10, 3);   //   The equilibrium constant for this reaction  // New From Laisk or Bassham and Krause 1969 BBA
        KM161	=	0.014;		//	ADP	16	ADP+Pi<->ATP
        KM162	=	0.3;		// 	Pi	16	ADP+Pi<-> ATP
        KM163   =   0.3;        //   ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
        KE16    =   5.734;      //   The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA
        KE21 = 2.3;
        KE22 = 0.058;
        //PsKE21	=	2.3;		//	Equilibrium constant	21	F6P<->G6P
        //PsKE22	=	0.058;		// 	Equilibrium constant	22	G6P<->G1P
        KM231	=	0.08;		//	G1P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
        KM232	=	0.08;		// 	ATP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
        KA231	=	0.1;		//	PGA	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
        KA232	=	0.02;		// 	F6P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
        KA233	=	0.02;		//	FBP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
        KI23	=	10;		    // 	ADP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1

        KM311	=	0.077;		//	DHAP	31	DHAPi<->DHAPo
        KM312	=	0.63;		// 	Pi	31	DHAPi<->DHAPo
        KM313	=	0.74;		//	Pext	31	DHAPi<->DHAPo
        KM32	=	0.25;		// 	PGA	32	PGAi<->PGAo
        KM33	=	0.075;		//	GAP	33	GAPi<->GAPo

        // Initialize the values of the global variables

        if (theVars->GP == 0) {
            V1 = theVars->EnzymeAct.at("V1");
            V2 = theVars->EnzymeAct.at("V2");
            V3 = theVars->EnzymeAct.at("V3");
            V5 = theVars->EnzymeAct.at("V5");
            V6 = theVars->EnzymeAct.at("V6");
            V7 = theVars->EnzymeAct.at("V7");
            V8 = theVars->EnzymeAct.at("V8");
            V9 = theVars->EnzymeAct.at("V9");
            V10	= theVars->EnzymeAct.at("V10");
            V13 = theVars->EnzymeAct.at("V13");
            V23 = theVars->EnzymeAct.at("V23");
            V16 = theVars->EnzymeAct.at("V16");
        }

        V31 = 3.73/3;   // 1.05 *SC  *1.0 ;	%	(Lilley, Chon, Mosbach & Heldt, 1977b)	31	Phosphate translocator	DHAPi<->DHAPo   1.05 defulat
        V32 = 3.73/3;   //1.05 *SC *1.0;	    %	(Lilley et al., 1977b)	32	Phosphate translocator	PGAi<->PGAo 1.05 default
        V33 = 3.73/3;   //1.05 *SC * 1.0;	    %	(Lilley et al., 1977b)	33	Phosphate translocator	GAPi<->GAPo 1.05 default

    } else {
        PS_C_CP = 15 * theVars->PSRatio[0];    //   Global constant for the total phosphate
        PS_C_CA = 1.5 * theVars->PSRatio[1];   //   Global constant for the total adenylates
        PS_PEXT = 0.5 * theVars->PSRatio[3];   //   Global constant for the cytosolic Phosphate concentration;

        // Initialize the constants for the different reactions
        KM11 = 0.0115 * theVars->PSRatio[19]; //  CO2 1 RuBP+CO2->2PGA
        KM12 = 0.222 * theVars->PSRatio[20];  // O2 1 RuBP+CO2->2PGA
        KM13 = 0.02 * theVars->PSRatio[21];            //  RuBP 1 RuBP+CO2->2PGA

        KI11 = 0.84 * theVars->PSRatio[22];  // PGA
        KI12 = 0.04 * theVars->PSRatio[23];  // FBP
        KI13 = 0.075 * theVars->PSRatio[24]; // SBP
        KI14 = 0.9 * theVars->PSRatio[25];   // Pi
        KI15 = 0.07 * theVars->PSRatio[26];  // NADPH

        KM21 = 0.240 * theVars->PSRatio[27]; // PGA 2 PGA+ATP <-> ADP + DPGA
        KM22 = 0.390 * theVars->PSRatio[28]; //  ATP 2 PGA+ATP <-> ADP + DPGA
        KM23 = 0.23 * theVars->PSRatio[29];  //  ADP

        KM31a = 0.004 * theVars->PSRatio[30]; // BPGA 3 DPGA+NADPH <->GAP + OP+NADP
        KM32b = 0.1 * theVars->PSRatio[31];   //  NADPH 3 DPGA+NADPH <->GAP + OP+NADP

        KE4 = 1 / 0.05 * theVars->PSRatio[34]; // Using the value from Patterson

        KM51 = 0.3 * theVars->PSRatio[35];  // GAP 5 GAP+DHAP <->FBP
        KM52 = 0.4 * theVars->PSRatio[36];  //  DHAP 5 GAP+DHAP <->FBP
        KM53 = 0.02 * theVars->PSRatio[37]; // FBP 5 GAP+DHAP <->FBP     // Original Value: 0.02
        KE5 = 7.100 * theVars->PSRatio[38]; // Defult: 7.1

        KM61 = 0.033 * theVars->PSRatio[39]; // FBP 6 FBP<->F6P+OP
        KI61 = 0.7 * theVars->PSRatio[40];   // F6P
        KI62 = 12 * theVars->PSRatio[41];    // Pi
        KE6 = 6.66 * pow(10, 5) * theVars->PSRatio[42];

        KE7 = 0.1 * theVars->PSRatio[47];   // The equilibrium constant for this reaction  New   Laisk  Bassham and Krause 1969 BBA

        KM81 = 0.4 * theVars->PSRatio[49];  // DHAP
        KM82 = 0.2 * theVars->PSRatio[50];  // E4P estimate
        KE8 = 1.017 * theVars->PSRatio[51]; // The equilibrium constant for this reaction  New  mM-1  Laisk  Bassham and Krause 1969 BBA. Default: 1.107

        KM9 = 0.05 * theVars->PSRatio[52]; // SBP 9 SBP<->S7P+OP
        KI9 = 12 * theVars->PSRatio[53];   // The inibintion constant for Pi;
        KE9 = 6.66 * pow(10, 5) * theVars->PSRatio[54];

        KM10 = 0.5 * theVars->PSRatio[55];    // R5P 10 S7P+GAP<->Ri5P+Xu5P
        KM101 = 0.1 * theVars->PSRatio[56];   // Xu5P
        KM102 = 0.09 * theVars->PSRatio[57];  // Estimate for GAP
        KM103 = 0.015 * theVars->PSRatio[58]; // Estimate for S7P
        //theVars->KE10 = 1 / 0.85 * theVars->PSRatio[59];//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA

        KE11 = 0.4 * theVars->PSRatio[60];   // Equilibrium Constant 11 Ri5P<-->Ru5P
        KE12 = 0.67 * theVars->PSRatio[61];  // Equilibrium Constant 12 Xu5P<-->Ru5P

        KM131 = 0.05 * theVars->PSRatio[62]; // Ru5P 13 Ru5P+ATP<->RuBP+ADP
        KM132 = 0.059 * theVars->PSRatio[63];//  ATP 13 Ru5P+ATP<->RuBP+ADP
        KI131 = 2 * theVars->PSRatio[64];    // PGA 13 Ru5P+ATP<->RuBP+ADP
        KI132 = 0.7 * theVars->PSRatio[65];  // RuBP 13 Ru5P+ATP<->RuBP+ADP
        KI133 = 4 * theVars->PSRatio[66];    // Pi 13 Ru5P+ATP<->RuBP+ADP
        KI134 = 2.5 * theVars->PSRatio[67];  // ADP 13 Ru5P+ATP<->RuBP+ADP
        KI135 = 0.4 * theVars->PSRatio[68];  // ADP 13 Ru5P+ATP<->RuBP+ADP
        KE13 = 6.846 * pow(10, 3) * theVars->PSRatio[69];

        KM161 = 0.014 * theVars->PSRatio[70]; // ADP 16 ADP+Pi<->ATP
        KM162 = 0.3 * theVars->PSRatio[71];   // Pi 16 ADP+Pi<-> ATP
        KM163 = 0.3 * theVars->PSRatio[72];   // ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
        KE16 = 5.734 * theVars->PSRatio[73];  // The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA

        KE21 = 2.3 * theVars->PSRatio[74];  // Equilibrium constant 21 F6P<->G6P
        KE22 = 0.058 * theVars->PSRatio[75];//  Equilibrium constant 22 G6P<->G1P

        KM311 = 0.077 * theVars->PSRatio[76]; // DHAP 31 DHAPi<->DHAPo
        //theVars->KM312 = 0.63 * theVars->PSRatio[77];//  Pi 31 DHAPi<->DHAPo
        KM313 = 0.74 * theVars->PSRatio[78];  // Pext 31 DHAPi<->DHAPo
        KM32 = 0.25 * theVars->PSRatio[79];   // PGA 32 PGAi<->PGAo
        KM33 = 0.075 * theVars->PSRatio[80];  // GAP 33 GAPi<->GAPo


        // Now put in the constant for the new ADPG Pyrophosphorylase and starch
        // synthase

        // ATP + Glucose-1-Phosphate --> ADPG + PPi

        KM231 = 0.031 * theVars->PSRatio[81]; // G1P 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        KM232 = 0.045 * theVars->PSRatio[82]; //  ATP 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        KM233 = 0.14 * theVars->PSRatio[83];  // ADPG 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        KM234 = 0.8 * theVars->PSRatio[84];   //  PPi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        KE23 = 7.6 * pow(10, (-3)) * theVars->PSRatio[85];


        KA231 = 0.23 * theVars->PSRatio[86]; // PGA 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        KI231 = 0.9 * theVars->PSRatio[87];  //0.9 ;       //   Pi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989 WY201803
        KVmo = 0.007 * theVars->PSRatio[88]; //   The minimum maximum velocity        Laisk et al 1989
        // ADPG --> ADP + Gn     // The starch synthesis reaction 24.     Laisk et al
        // 1989

        KM241 = 0.2 * theVars->PSRatio[89]; //   ADPG    ADPG --> ADP + Gn       Laisk et al 1989

        KE25 = 1.2 * 107 * theVars->PSRatio[92];

        // Initialize the Vmax for different reactions

        if (theVars->GP == 0) {
            // Initialize the values of the global variables

            const double SC = 1;        // Scalling coefficient for the stroma volume per mg chl. defualt 2
            const double SC1 = 1;
            const double STOM1 = 1;
            const double STOM2 = 1;

            V1 = 2.93 * SC1 / STOM1 * theVars->PSRatio[4]; // (Harris & Koniger, 1997)
            V2 = 30.15 * SC * STOM2 * theVars->PSRatio[5]; // (Harris & Koniger, 1997)
            V3 = 4.04 * SC * STOM2 * theVars->PSRatio[6];  // 1.57*SC     ; // (Harris & Koniger, 1997)
            V5 = 1.22 * SC * theVars->PSRatio[7];                   // (Harris & Koniger, 1997)
            V6 = 0.734 * SC / STOM1 * theVars->PSRatio[8]; // (Harris & Koniger, 1997)
            V7 = 3.12 * SC * 4 * theVars->PSRatio[9];               // (Harris & Koniger, 1997)
            V8 = 1.22 * SC * theVars->PSRatio[10];                  // (Harris & Koniger, 1997)
            V9 = 0.32 * 3 * theVars->PSRatio[11]; // 0.17*SC *FC ; // (Harris & Koniger, 1997) *3.
            //theVars->V10 = theVars->V7; // (Harris & Koniger, 1997)
            V13 = 10.81 * SC1 * theVars->PSRatio[12];      // (Harris & Koniger, 1997)
            V16 = 5.47 * theVars->PSRatio[13];             // (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
            V23 = 2 * theVars->PSRatio[14];
        }
        V24 = 2 * theVars->PSRatio[15];
        V31 = 1.0 * theVars->PSRatio[16] * 20;
        V32 = 1.0 * theVars->PSRatio[17];
        V33 = 1.0 * theVars->PSRatio[18] * 20;//WY 2018103
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the location for transfering variables//
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    theVars->ADP = PS_C_CA - PS_con->ATP;

    return PS_con;
}
