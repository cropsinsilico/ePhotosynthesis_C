#include "Variables.hpp"

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


arr PSI(Variables &myVars) {
    myVars.PS_C_CP = 15;    //   Global constant for the total phosphate
    myVars.PS_C_CA = 1.5;    //   Global constant for the total adenylates
    myVars.PS_C_CN = 1;       //   Global constant for the total NADP+NADPH
    myVars.PS_PEXT = 0.5;     //   Global constant for the cytosolic Phosphate concentration;

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

    myVars.KM311 = 0.077;		//	DHAP	31	DHAPi<->DHAPo
    myVars.KM312 = 0.63;		// 	Pi	31	DHAPi<->DHAPo
    myVars.KM313 = 0.74;		//	Pext	31	DHAPi<->DHAPo
    myVars.KM32 = 0.25;		// 	PGA	32	PGAi<->PGAo
    myVars.KM33 = 0.075;		//	GAP	33	GAPi<->GAPo

    // Initialize the Vmax for different reactions

    myVars.V31 = 3.73 / 3;// 1.05 *SC  *1.0 ;	//	(Lilley, Chon, Mosbach & Heldt, 1977b)	31	Phosphate translocator	DHAPi<->DHAPo   1.05 defulat
    myVars.V32 = 3.73 / 3;//1.05 *SC *1.0;	    //	(Lilley et al., 1977b)	32	Phosphate translocator	PGAi<->PGAo 1.05 default
    myVars.V33 = 3.73 / 3;//1.05 *SC * 1.0;	    //	(Lilley et al., 1977b)	33	Phosphate translocator	GAPi<->GAPo 1.05 default

    ////////////////////////////////////////////////////////////////////////////////////H////////////////
    //   Here is the location for transfering variables
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    myVars.PS2RA_RuBP_ini = RuBP;

    myVars.PS2BF_ADP = myVars.PS_C_CA - ATP;

    return PSs;
}
