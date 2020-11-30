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

PSCon PS::PSI(Variables *myVars) {
    PS_C_CP = 15;    //   Global constant for the total phosphate
    PS_C_CA = 1.5;    //   Global constant for the total adenylates
    PS_PEXT = 0.5;     //   Global constant for the cytosolic Phosphate concentration;

    myVars->PSPR_RA_CA = PS_C_CA;

    PSCon PS_con;
    PS_con.RuBP = 2.000;
    PS_con.PGA = 2.400;
    PS_con.DPGA = 0.001;
    PS_con.T3P = 0.5;
    PS_con.ADPG = 0.;
    PS_con.FBP = 0.670;
    PS_con.E4P = 0.050;
    PS_con.S7P = 2.000;
    PS_con.SBP = 0.300;
    PS_con.ATP = 0.68;
    PS_con.NADPH = 0.21;
    PS_con.CO2 = 0.012;
    PS_con.O2 = 0.264;
    PS_con.HexP = 2.2;
    PS_con.PenP = 0.25;

    myVars->KM11 = 0.0115;		// 	CO2	1	RuBP+CO2->2PGA
    myVars->KM12 = 0.222;		//	O2	1	RuBP+CO2->2PGA  0.28 DEFAUL.
    KM13 = 0.02;		// 	RuBP	1	RuBP+CO2->2PGA

    KI11 = 0.84;    // PGA
    KI12 = 0.04;       // FBP
    KI13 = 0.075;       // SBP
    KI14 = 0.9;       // Pi
    KI15 = 0.07;       // NADPH


    KM21 = 0.240;		//	PGA	2	PGA+ATP <-> ADP + DPGA
    KM22 = 0.390;		// 	ATP	2	PGA+ATP <-> ADP + DPGA
    KM23 = 0.23;        //  ADP

    KM31a = 0.004;		//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
    KM32b = 0.1;	    // 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP

    KE4 = 0.05;       //   Using the value from Patterson

    KM51 = 0.3;	    //	GAP	5	GAP+DHAP <->FBP
    KM52 = 0.4;	    // 	DHAP	5	GAP+DHAP <->FBP
    KM53 = 0.02;		//	FBP	5	GAP+DHAP <->FBP     // Original Value: 0.02
    KE5 = 7.100;          // Defult: 7.1

    KM61 = 0.033;		// 	FBP	6	FBP<->F6P+OP
    KI61 = 0.7;       //   F6P
    KI62 = 12;       //   Pi
    KE6 = 6.66 * pow(10, 5);

    KE7 = 10;       // The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA

    KM81 = 0.4;       // DHAP
    KM82 = 0.2;       // E4P estimate
    KE8 = 1.017;     // The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107

    KM9 = 0.05;		// 	SBP	9	SBP<->S7P+OP
    KI9 = 12;       //   The inibintion constant for Pi;
    KE9 = 6.66 * pow(10, 5);


    KM10 = 1.5;	    //	R5P	10	S7P+GAP<->Ri5P+Xu5P
    KM101 = 0.1;       //   Xu5P
    KM102 = 0.072;       //   Estimate for GAP
    KM103 = 0.46;        //   Estimate for S7P                                    // New
    //myVars->KE10 = 1 / 0.85;//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA

    KE11 = 0.4;	    //	Equilibrium Constant	11	Ri5P<-->Ru5P
    KE12 = 0.67;		// 	Equilibrium Constant	12	Xu5P<-->Ru5P

    KM131 = 0.05;		    //	Ru5P	13	Ru5P+ATP<->RuBP+ADP
    KM132 = 0.059;		    // 	ATP	13	Ru5P+ATP<->RuBP+ADP
    KI131 = 2;			//	PGA	13	Ru5P+ATP<->RuBP+ADP
    KI132 = 0.7;			//	RuBP	13	Ru5P+ATP<->RuBP+ADP
    KI133 = 4;			//	Pi	13	Ru5P+ATP<->RuBP+ADP
    KI134 = 2.5;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
    KI135 = 0.4;			//	ADP	13	Ru5P+ATP<->RuBP+ADP
    KE13 = 6.846 * pow(10, 3);

    KM161 = 0.014;		//	ADP	16	ADP+Pi<->ATP
    KM162 = 0.3;		// 	Pi	16	ADP+Pi<-> ATP
    KM163 = 0.3;        //   ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
    KE16 = 5.734;      //   The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA


    KE21 = 2.3;		//	Equilibrium constant	21	F6P<->G6P
    KE22 = 0.058;		// 	Equilibrium constant	22	G6P<->G1P

    KM231 = 0.08;		//	G1P	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    KM232 = 0.08;		// 	ATP	23	G1P+ATP+Gn<->PPi+ADP+Gn+1
    KA231 = 0.1;		//	PGA	23	G1P+ATP+Gn<->PPi+ADP+Gn+1

    KM311 = 0.077;		//	DHAP	31	DHAPi<->DHAPo
    //myVars->KM312 = 0.63;		// 	Pi	31	DHAPi<->DHAPo
    KM313 = 0.74;		//	Pext	31	DHAPi<->DHAPo
    KM32 = 0.25;		// 	PGA	32	PGAi<->PGAo
    KM33 = 0.075;		//	GAP	33	GAPi<->GAPo

    // Initialize the Vmax for different reactions

    V31 = 3.73 / 3;// 1.05 *SC  *1.0 ;	//	(Lilley, Chon, Mosbach & Heldt, 1977b)	31	Phosphate translocator	DHAPi<->DHAPo   1.05 defulat
    V32 = 3.73 / 3;//1.05 *SC *1.0;	    //	(Lilley et al., 1977b)	32	Phosphate translocator	PGAi<->PGAo 1.05 default
    V33 = 3.73 / 3;//1.05 *SC * 1.0;	    //	(Lilley et al., 1977b)	33	Phosphate translocator	GAPi<->GAPo 1.05 default

    ////////////////////////////////////////////////////////////////////////////////////H////////////////
    //   Here is the location for transfering variables
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    myVars->PS2RA_RuBP_ini = PS_con.RuBP;

    myVars->PS2BF_ADP = PS_C_CA - PS_con.ATP;

    return PS_con;
}
