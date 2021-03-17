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
double SUCS::V60 = 0;
double SUCS::V61 = 0.;
double SUCS::V62 = 0.;
double SUCS::Vdhap_in = 0.;
double SUCS::Vgap_in = 0.;
double SUCS::Vpga_in = 0.;
double SUCS::Km592 = 0.;
double SUCS::KI592 = 0.;
double SUCS::Km601 = 0.;
double SUCS::Km602 = 0.;
double SUCS::Km603 = 0.;
double SUCS::Km604 = 0.;
double SUCS::KE60 = 0.;
double SUCS::Vfactor51 = 0;
double SUCS::Vfactor52 = 0;
double SUCS::Vfactor56 = 0;
double SUCS::Vfactor57 = 0;
double SUCS::Vfactor59 = 0;


SUCSCon SUCS::SUCS_Ini(Variables *theVars) {
    theVars->SUCS_OLD_TIME = 0;
    theVars->SUCS_TIME_N = 1;

    if (theVars->useC3) {
        KE501 = 0.05;     //	Equilibrium Constant		50		KE501		0.05		[Bassham, 1869 #832]
        Km511 = 0.02;     //	FBP	4.1.2.13	51		Km511	FBP	0.02	Pisum sativum	(Anderson, Heinrikson et al. 1975)
        Km512 = 0.3;      //	FBP	4.1.2.13	51		Km512	GAP	0.3	Spinacia oleracea	(Iwaki, Wadano et al. 1991)
        Km513 = 0.4;      //	FBP	4.1.2.13	51		Km513	DHAP	0.4	Spinacia oleracea	(Iwaki, Wadano et al. 1991)
        KE51 = 12;        //  Based on Thomas et al 1997 Biochem Journal. The fifth citation in the paper.
        Km521 = 0.0025;   //	FBPase[1]	3.1.3.11	52		Km521	FBP	0.0025	Pisum sativum	(Jang, Lee et al. 2003)
        KI521 = 0.7;      //	FBPase	3.1.3.11	52		KI521	F6P	0.7		[Heldt, 1983 #841]
        KI522 = 12;       //	FBPase	3.1.3.11	52		KI522	Pi	12	Pisum sativum	(Charles & Halliwell 1997)
        KI523 = 7*pow(10, -5); //	FBPase	3.1.3.11	52		KI523	F26BP	7*10^(-5)	Pisum sativum <Com>	{Jang, 2003 #2523}
        KE52 = 6663;      //	FBPase	3.1.3.11	52		KE52			6663	[Bassham, 1869 #832]
        KE531 = 2.3;      //	Equilibrium Constant	5.3.1.9	53		KE531		2.3[2]		[Bassham, 1869 #832]
        KE541 = 0.0584;   //	Equilibrium Constant	5.4.2.2	54	G1P G6P	KE541	G1P G6P	0.0584		[Bassham, 1869 #832]
        Km551 = 0.14;     //	UGPase	2.7.7.9	55		Km551	G1P	0.14	Solanum tuberosum	(Nakano, Omura et al. 1989)
        Km552 = 0.1;      //	UDPase	2.7.7.9	55		Km552	UTP	0.1	Solanum tuberosum	(Nakano, Omura et al. 1989)
        Km553 = 0.11;     //	UGPase	2.7.7.9	55		Km553	OPOP	0.11	Solanum tuberosum	(Nakano, Omura et al. 1989)
        Km554 = 0.12;     //	UGPase	2.7.7.9	55		Km554	UDPGlu	0.12	Solanum tuberosum	(Nakano, Omura et al. 1989)
        KE55 = 0.31;      //	UGPase	2.7.7.9	55		KE55	Equi	0.31		Lunn and Rees 1990
        Km561 = 0.8;      //	SPase	2.4.1.14	56		Km561	D-F6P	0.8	Pisum sativum	(Lunn and Ap Rees 1990)
        Km562 = 2.4;      //	Spase	2.4.1.14	56		Km562	UDP-glucose	2.4	Pisum sativum	(Lunn and Ap Rees 1990)
        KI561 = 0.7;      //				Inhibitor	KI561	UDP	0.7	Spinacia oleracea	(Harbron, Foyer et al. 1981)
        KI562 = 0.8;      //	Sucrose Synthesase			Inhibitor	KI562	FBP	0.8	Spinacia oleracea	(Harbron, Foyer et al. 1981)
        KI563 = 0.4;      //				Inhibitor	KI563	SUCP	0.4	Spinacia oleracea	(Harbron, Foyer et al. 1981)
        KI564 = 11;       //		2.4.1.14	56	Inhibitor	KI564	Pi	11	Spinacia oleracea	(Harbron, Foyer et al. 1981)
        KI565 = 50;       //		2.4.1.14	56	Inhibitor	KI565	Sucrose	50	Spinacia oleracea	{Salerno, 1978 #2525}
        KE56 = 10;        //					KE56		10	Pisum sativum	Lunn and Rees, 1990
        Km571 = 0.35;     //	SPP	3.1.3.24	57.1		Km571	SUCP	0.35	Pisum sativum	(Whitaker 1984)
        Ki572 = 80;       //	SPP	3.1.3.24	57.2		Ki572	SUC	80	Daucus carota	(Whitaker 1984)
        KE57 = 780;       //	SPP	3.1.3.24	57.2		KE57	Equili	780		Lunn and Rees 1990
        Km581 = 0.032;    //	F26BPa	3.1.3.46	58		Km581	F26BP	0.032	Spinacia oleracea	(Macdonald, Chou et al. 1989)
        KI581 = 0.1;      //	F26BPa	3.1.3.46	58		KI581	F6P	0.1	Arabidopsis thaliana	(Villadsen and Nielsen 2001)
        KI582 = 0.5;      //	F26BPa	3.1.3.46	58		KI582	OP	0.5	Arabidopsis thaliana	(Villadsen and Nielsen 2001)
        Km591 = 0.5;      //	6PF2K	2.7.1.105	59		Km591	ATP	0.5	Spinacia oleracea	(Walker and Huber 1987)
        Km593 = 0.5;      //	6PF2K	2.7.1.105	59		Km593	F6P	0.5	Spinacia oleracea	(Walker and Huber 1987)
        KI591 = 0.16;     //			59		KI591	ADP	0.16	Rattus norvegicus	(Kretschmer and Hofmann 1984)
        KE61 = 1.2*107;   //	Pyrophosphate hydrolysis				KE61		1.2*107		{Flodgaard, 1974 #2521}
        Km621 = 5;        //	Vsink			Notice: pH dependent	Km621	Sucrose	5		{Weschke, 2000 #2522}
        KE59 = 590;       //	6PF2K	2.7.1.105	59		KE59		590		Cornish-Bowden, 1997
        Km592 = 0.021;    //	6PF2K	2.7.1.105	59		Km592	F26BP	0.021	Sparus aurate	(Garcia de Frutos and Baanante 1995)
        KI592 = 0.7;      //	6PF2K	2.7.1.105	59		KI592	DHAP	0.7	Spinacia oleracea	{Markham, 2002 #2524}
        Km601 = 0.042;    //	Nucleoside Diphosphate Kinase	2.7.4.6	60	NI	Km601	ADP	0.042	Rat	Kamura and Shimada 1988
        Km602 = 1.66;     //	Nucleoside Diphosphate Kinase	2.7.4.6	60	NI	Km602	ATP	1.66	Rat	Kamura and Shimada 1988
        Km603 = 0.28;     //	Nucleoside Diphosphate Kinase	2.7.4.6	60	NI	Km603	UDP	0.28	Saccharomyces cerevisiae	{Jong, 1991 #2518}
        Km604 = 16;       //	Nucleoside Diphosphate Kinase	2.7.4.6	60	NI	Km604	UTP	16	Rattus norvegicus	{Fukuchi, 1994 #2519}
        KE60 = 16;        //	Nucleoside Diphosphate Kinase	2.7.4.6	60	NI	KE60		16	1.04	{Lynn, 1978 #2520}
        Vfactor51 = 1.;
        Vfactor52 = 1.;
        Vfactor56 = 1.;
        Vfactor57 = 1.;
        Vfactor59 = 1.;
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0.) {
            Vfactor52=theVars->VfactorCp[19];
            Vfactor56=theVars->VfactorCp[22];
            Vfactor57=theVars->VfactorCp[23];
            Vfactor59=theVars->VfactorCp[24];

        }
    } else {
        KE501 = 1 / 0.05 * theVars->SUCRatio[15];
        Km511 = 0.02 * theVars->SUCRatio[16];
        Km512 = 0.3 * theVars->SUCRatio[17];
        Km513 = 0.4 * theVars->SUCRatio[18];
        KE51 = 12 * theVars->SUCRatio[19];
        //theVars->Km514 = 0.014 * theVars->SUCRatio[20];  // --unused
        Km521 = 0.0025 * theVars->SUCRatio[21];
        KI521 = 0.7 * theVars->SUCRatio[22];
        KI522 = 12 * theVars->SUCRatio[23];
        KI523 = 7 * pow(10, (-5)) * theVars->SUCRatio[24];
        KE52 = 6663 * theVars->SUCRatio[25];
        KE531 = 2.3 * theVars->SUCRatio[26];
        KE541 = 0.0584 * theVars->SUCRatio[27];
        Km551 = 0.14 * theVars->SUCRatio[28];
        Km552 = 0.1 * theVars->SUCRatio[29];
        Km553 = 0.11 * theVars->SUCRatio[30];
        Km554 = 0.12 * theVars->SUCRatio[31];
        KE55 = 0.31 * theVars->SUCRatio[32];
        Km561 = 0.8 * theVars->SUCRatio[33];
        Km562 = 2.4 * theVars->SUCRatio[34];
        KI561 = 0.7 * theVars->SUCRatio[35];
        KI562 = 0.8 * theVars->SUCRatio[36];
        KI563 = 0.4 * theVars->SUCRatio[37];
        KI564 = 11 * theVars->SUCRatio[38];
        KI565 = 50 * theVars->SUCRatio[39];
        KE56 = 10 * theVars->SUCRatio[40];
        Km571 = 0.35 * theVars->SUCRatio[41];
        Ki572 = 10 * theVars->SUCRatio[42];
        KE57 = 780 * theVars->SUCRatio[43];
        Km581 = 0.032 * theVars->SUCRatio[44];
        KI581 = 0.1 * theVars->SUCRatio[45];
        KI582 = 0.5 * theVars->SUCRatio[46];
        Km591 = 0.5 * theVars->SUCRatio[47];
        Km593 = 0.5 * theVars->SUCRatio[49];
        KI591 = 0.16 * theVars->SUCRatio[50];
        KE59 = 590 * theVars->SUCRatio[52];
        KE61 = 1.2 * 107 * theVars->SUCRatio[58];
        Km621 = 5 * theVars->SUCRatio[59];
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves of active enzyme in a dark adapted leaves;
    // mM

    // Assign value to a variable that is transferred to the program
    SUCSCon SUCS_Con;
    if (theVars->useC3) {
        SUCS_Con.T3Pc = 2.3;
        SUCS_Con.FBPc = 2.;
        SUCS_Con.HexPc = 5.8;
        SUCS_Con.F26BPc = 7.8 * pow(10, -6);
        SUCS_Con.ATPc = 0.35;
        SUCS_Con.ADPc = 0.65;
        SUCS_Con.UDPGc = 0.57;
        SUCS_Con.UTPc = 0.75;
        SUCS_Con.SUCP = 0.;
        SUCS_Con.SUC = 0.;
        SUCS_Con.PGAc = 0.;
    } else {
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
    }


    // The following calculate the total concentration of different enzymes.
    const double SC = 10;
    const double SC1 = 1;

    if (theVars->GP == 0) {
        // Unit: mmol l-1 s-1;
        if (theVars->useC3) {
            V51	= theVars->EnzymeAct[19];
            V52	= theVars->EnzymeAct[20];
            V55	= theVars->EnzymeAct[21];
            V56	= theVars->EnzymeAct[22];
            V57	= theVars->EnzymeAct[23];
            V58	= theVars->EnzymeAct[24];

        } else {
            V51 = 0.107376831 * SC * theVars->SUCRatio[0]; // DHAP+GAP --FBP          // default 0.5
            V52 = 0.063979048 * SC * theVars->SUCRatio[1]; // FBP --F6P + Pi
            V55 = 0.115403205 * SC * theVars->SUCRatio[2]; // G1P+UTP --OPOP+UDPG
            V56 = 0.055503446 * SC * theVars->SUCRatio[3]; // UDPG+F6P--SUCP + UDP
            V57 = 0.55503446 * SC1 * theVars->SUCRatio[4]; // SUCP--Pi + SUC; 0.27 DEFALT
            V58 = 0.016819226 * SC * theVars->SUCRatio[5]; // F26BP--F6P + Pi
        }
    }
    if (theVars->useC3) {
        V59	= theVars->EnzymeAct[25];
        V60	= 6.1;	//	ATP+UDP --UTP + ADP
        V61	= 10000;	//	POPO --2PO
        V62	= 2;	//	SUC Sink        0.2 works.

    } else {
        V59 = 0.03 * SC * theVars->SUCRatio[6];            // F6P + ATP --ADP + F26BP // defalut 0.03  (* 0.3)
        //theVars->V60 = 6.1 * theVars->SUCRatio[7];// ATP+UDP --UTP + ADP
        //theVars->V61 = 10000;         // POPO --2PO   // constant set in globals.hpp
        V62 = 2 * SC1 * theVars->SUCRatio[8];              // SUC Sink        0.9 works.
        Vdhap_in = 1.05 * SC1 * theVars->SUCRatio[9];      // DHAP export from chloroplast
        Vgap_in = 1.05 * SC1 * theVars->SUCRatio[10];      // GAP export from chloroplast
        Vpga_in = 1.05 * SC1 * theVars->SUCRatio[11];      // PGA export from chloropalst
    }


    //////////////////////////////////////////////////////////////////
    // Here is some pool values      //
    //////////////////////////////////////////////////////////////////
    if (theVars->useC3) {
        theVars->SUCS_Pool.ATc = 1.0; // mM
        theVars->SUCS_Pool.UTc = 1.5; // mM
        theVars->SUCS_Pool.PTc = 15;  //
    } else {
        theVars->SUCS_Pool.ATc = 1.0 * theVars->SUCRatio[12]; // mM
        theVars->SUCS_Pool.UTc = 1.5 * theVars->SUCRatio[13]; // mM
        theVars->SUCS_Pool.PTc = 15 * theVars->SUCRatio[14];  //
    }

    return SUCS_Con;
}
