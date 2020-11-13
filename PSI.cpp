#include "globals.hpp"

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


arr PSI(varptr &myVars) {
    
    //global PS_C_CA;             //   Global constant for the total adenylates
    //global PS_C_CP;             //   Global constant for the total phosphate
    //global PS_C_CN;             //   Global constant for the total NADP+NADPH
    //global PS_PEXT;             //   Global constant for the cytosolic Phosphate concentration;
    
    myVars.PS_C_CP = 15;
    myVars.PS_C_CA = 1.5;
    myVars.PS_C_CN = 1;
    myVars.PS_PEXT = 0.5;
    
    //global PSPR_RA_CA;
    myVars.PSPR_RA_CA = myVars.PS_C_CA;
    
    
    const double RuBP = 2.000;
    const double PGA = 2.400;
    const double DPGA = 0.0011;
    const double T3P = 0.5;
    const double NONE = 0;
    const double FBP = 0.670;
    const double E4P = 0.050;
    const double S7P = 2.000;
    const double SBP = 0.300;
    const double ATP = 0.68;
    const double NADPH = 0.21;
    const double HexP = 2.2;
    const double PenP = 0.25;
    
    const double CO2 = 0.012;
    const double O2 = 0.264;
    
    arr PSs = zeros(15);
    PSs[0] = RuBP;
    PSs[1] = PGA;
    PSs[2] = DPGA;
    PSs[3] = T3P;
    PSs[4] = NONE;
    PSs[5] = FBP;
    PSs[6] = E4P;
    PSs[7] = S7P;
    PSs[8] = SBP;
    PSs[9] = ATP;
    PSs[10] = NADPH;
    PSs[11] = CO2;
    PSs[12] = O2;
    PSs[13] = HexP;
    PSs[14] = PenP;
    
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
    
    //global	KM231;
    //global	KM232;
    //global	KA231;
    //global	KA232;
    //global	KA233;
    //global	KI23;
    
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
    
    myVars.KM11 = 0.0115;		// 	CO2	1	RuBP+CO2->2PGA
    myVars.KM12 = 0.222;		//	O2	1	RuBP+CO2->2PGA  0.28 DEFAUL.
    myVars.KM13 = 0.02;		// 	RuBP	1	RuBP+CO2->2PGA
    
    myVars.KI11 = 0.84;    // PGA
    myVars.KI12 = 0.04;       // FBP
    myVars.KI13 = 0.075;       // SBP
    myVars.KI14 = 0.9;       // Pi
    myVars.KI15 = 0.07;       // NADPH
    
    
    myVars.KM21 = 0.240;		//	PGA	2	PGA+ATP <-> ADP + DPGA
    myVars.KM22 = 0.390;		// 	ATP	2	PGA+ATP <-> ADP + DPGA
    myVars.KM23 = 0.23;        //  ADP
    
    myVars.KM31a = 0.004;		//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
    myVars.KM32b = 0.1;	    // 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP
    
    myVars.KM41 = 2.5;	    //	DHAP	4	DHAP <->GAP
    myVars.KM42 = 0.68;		// 	GAP	4	DHAP <->GAP
    myVars.KE4 = 0.05;       //   Using the value from Patterson
    
    myVars.KM51 = 0.3;	    //	GAP	5	GAP+DHAP <->FBP
    myVars.KM52 = 0.4;	    // 	DHAP	5	GAP+DHAP <->FBP
    myVars.KM53 = 0.02;		//	FBP	5	GAP+DHAP <->FBP     // Original Value: 0.02
    myVars.KE5 = 7.100;          // Defult: 7.1
    
    myVars.KM61 = 0.033;		// 	FBP	6	FBP<->F6P+OP
    myVars.KI61 = 0.7;       //   F6P
    myVars.KI62 = 12;       //   Pi
    myVars.KE6 = 6.66 * pow(10, 5);
    
    myVars.KM71 = 0.100;		//	Xu5P	7	F6P+GAP<->E4P+Xu5P      // jn
    myVars.KM72 = 0.100;		// 	E4P	7	F6P+GAP<->E4P+Xu5P
    myVars.KM73 = 0.1;         //   F6P This value was based on estimate
    myVars.KM74 = 0.1000;         // Estimate for GAP ORIGINAL 0.1
    myVars.KE7 = 10;       // The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA
    
    myVars.KM8 = 0.02;		//	SBP	8	E4P+DHAP<->SBP
    myVars.KM81 = 0.4;       // DHAP
    myVars.KM82 = 0.2;       // E4P estimate
    myVars.KE8 = 1.017;     // The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107
    
    myVars.KM9 = 0.05;		// 	SBP	9	SBP<->S7P+OP
    myVars.KI9 = 12;       //   The inibintion constant for Pi;
    myVars.KE9 = 6.66 * pow(10, 5);
    
    
    myVars.KM10 = 1.5;	    //	R5P	10	S7P+GAP<->Ri5P+Xu5P
    myVars.KM101 = 0.1;       //   Xu5P
    myVars.KM102 = 0.072;       //   Estimate for GAP
    myVars.KM103 = 0.46;        //   Estimate for S7P                                    // New
    myVars.KE10 = 1 / 0.85;//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA
    
    myVars.KE11 = 0.4;	    //	Equilibrium Constant	11	Ri5P<-->Ru5P
    myVars.KE12 = 0.67;		// 	Equilibrium Constant	12	Xu5P<-->Ru5P
    
    myVars.KM131 = 0.05;		    //	Ru5P	13	Ru5P+ATP<->RuBP+ADP
    myVars.KM132 = 0.059;		    // 	ATP	13	Ru5P+ATP<->RuBP+ADP
    myVars.KI131 = 2;			//	PGA	13	Ru5P+ATP<->RuBP+ADP
    myVars.KI132 = 0.7;			//	RuBP	13	Ru5P+ATP<->RuBP+ADP
    myVars.KI133 = 4;			//	Pi	13	Ru5P+ATP<->RuBP+ADP
    myVars.KI134 = 2.5;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
    myVars.KI135 = 0.4;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
    myVars.KE13 = 6.846 * pow(10, 3);
    
    myVars.KM161 = 0.014;		//	ADP	16	ADP+Pi<->ATP
    myVars.KM162 = 0.3;		// 	Pi	16	ADP+Pi<-> ATP
    myVars.KM163 = 0.3;        //   ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
    myVars.KE16 = 5.734;      //   The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA
    
    
    myVars.KE21 = 2.3;		//	Equilibrium constant	21	F6P<->G6P
    myVars.KE22 = 0.058;		// 	Equilibrium constant	22	G6P<->G1P
    
    myVars.KM231 = 0.08;		//	G1P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    myVars.KM232 = 0.08;		// 	ATP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    myVars.KA231 = 0.1;		//	PGA	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    //myVars.KA232 = 0.02;		// 	F6P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1  // --unused
    //myVars.KA233 = 0.02;		//	FBP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1  // --unused
    //myVars.KI23 = 10;		    // 	ADP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1  // --unused
    
    myVars.KM311 = 0.077;		//	DHAP	31	DHAPi<->DHAPo
    myVars.KM312 = 0.63;		// 	Pi	31	DHAPi<->DHAPo
    myVars.KM313 = 0.74;		//	Pext	31	DHAPi<->DHAPo
    myVars.KM32 = 0.25;		// 	PGA	32	PGAi<->PGAo
    myVars.KM33 = 0.075;		//	GAP	33	GAPi<->GAPo
    
    // Initialize the Vmax for different reactions
    
    //global	V1;
    //global	V2;
    //global	V3;
    // global	V4	;
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
    
    // FC is a fussl factor here.
    // FC = 1;       // Defulat is 2.5.// --unused
    // fc16 = 1;     // default 1.5.// --unused
    // SC = 1;        // Scalling coefficient for the stroma volume per mg chl. defualt 2// --unused
    
    myVars.V31 = 3.73 / 3;// 1.05 *SC  *1.0 ;	//	(Lilley, Chon, Mosbach & Heldt, 1977b)	31	Phosphate translocator	DHAPi<->DHAPo   1.05 defulat
    myVars.V32 = 3.73 / 3;//1.05 *SC *1.0;	    //	(Lilley et al., 1977b)	32	Phosphate translocator	PGAi<->PGAo 1.05 default
    myVars.V33 = 3.73 / 3;//1.05 *SC * 1.0;	    //	(Lilley et al., 1977b)	33	Phosphate translocator	GAPi<->GAPo 1.05 default
    
    ////////////////////////////////////////////////////////////////////////////////////H////////////////
    //   Here is the location for transfering variables
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //global PS2RA_RuBP_ini;
    myVars.PS2RA_RuBP_ini = RuBP;
    
    //global BF_FI_com;
    //global PS2BF_ATP;
    //global PS2BF_ADP;
    //global PS2BF_Pi;
    
    //myVars.PS2BF_ATP = ATP;  // --unused
    myVars.PS2BF_ADP = myVars.PS_C_CA - ATP;
    
    //global V31_ps2ca;
    //global V32_ps2ca;
    //global V33_ps2ca;
    //myVars.V31_ps2ca = myVars.V31;  // --unused
    //myVars.V32_ps2ca = myVars.V32;  // --unused
    //myVars.V33_ps2ca = myVars.V33;  // --unused
    return PSs;
}
