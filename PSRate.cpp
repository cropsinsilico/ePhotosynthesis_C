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

//    You should have received a copy of the GNU General Public License (GMymatrix.txtPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PSRate(double t, PSCon &PS_con, arr &Param, Variables *myVars) {
    double PsPEXT = myVars->PS_PEXT;

    // First the physical and chemical constant for all the reactions

    // Set the value to the local variables: for example: PrKM11

    const double PsKM11 = myVars->KM11;	// 	CO2	1	RuBP+CO2->2PGA
    const double PsKM12 = myVars->KM12;	//	O2	1	RuBP+CO2->2PGA
    const double PsKM13 = myVars->KM13;	// 	RuBP	1	RuBP+CO2->2PGA
    const double PsKI11 = myVars->KI11;   //   PGA
    const double PsKI12 = myVars->KI12;   //   FBP
    const double PsKI13 = myVars->KI13;   //   SBP
    const double PsKI14 = myVars->KI14;   //   Pi
    const double PsKI15 = myVars->KI15;   //   NADPH
    const double PsKM21 = myVars->KM21;	//	PGA	2	PGA+ATP <-> ADP + DPGA
    const double PsKM22 = myVars->KM22;	// 	ATP	2	PGA+ATP <-> ADP + DPGA
    const double PsKM23 = myVars->KM23;   //   ADP
    const double PsKM31a = myVars->KM31a;	//	BPGA	3	DPGA+NADPH <->GAP + OP+NADP
    const double PsKM32b = myVars->KM32b;	// 	NADPH	3	DPGA+NADPH <->GAP + OP+NADP
    const double PsKM51 = myVars->KM51;	//	GAP	5	GAP+DHAP <->FBP
    const double PsKM52 = myVars->KM52;	// 	DHAP	5	GAP+DHAP <->FBP
    const double PsKM53 = myVars->KM53;	//	FBP	5	GAP+DHAP <->FBP
    const double PsKE5 = myVars->KE5;
    const double PsKM61 = myVars->KM61;	// 	FBP	6	FBP<->F6P+OP
    const double PsKI61 = myVars->KI61;
    const double PsKI62 = myVars->KI62;
    const double PsKM81 = myVars->KM81;   // DHAP
    const double PsKM82 = myVars->KM82;   // E4P
    const double PsKM9 = myVars->KM9;	// 	SBP	9	SBP<->S7P+OP
    const double PsKI9 = myVars->KI9;
    const double PsKE11 = myVars->KE11;	//	Equilibrium Constant	11	Ri5P<-->Ru5P
    const double PsKE12 = myVars->KE12;	// 	Equilibrium Constant	12	Xu5P<-->Ru5P
    const double PsKM131 = myVars->KM131;	//	Ru5P	13	Ru5P+ATP<->RuBP+ADP
    const double PsKM132 = myVars->KM132;	// 	ATP	13	Ru5P+ATP<->RuBP+ADP
    const double PsKI131 = myVars->KI131;	//	PGA
    const double PsKI132 = myVars->KI132;	//	RuBP
    const double PsKI133 = myVars->KI133;	//	Pi
    const double PsKI134 = myVars->KI134;	//	ADP
    const double PsKI135 = myVars->KI135;	//	ADP
    const double PsKM161 = myVars->KM161;	//	ADP	16	ADP+Pi<->ATP
    const double PsKM162 = myVars->KM162;	// 	Pi	16	ADP+Pi<-> ATP
    const double PsKE21 = myVars->KE21;	//	Equilibrium constant	21	F6P<->G6P
    const double PsKE22 = myVars->KE22;	// 	Equilibrium constant	22	G6P<->G1P
    const double PsKM311 = myVars->KM311;	//	DHAP	31	DHAPi<->DHAPo
    const double PsKM313 = myVars->KM313;	//	Pext	31	DHAPi<->DHAPo
    const double PsKM32 = myVars->KM32;	// 	PGA	32	PGAi<->PGAo
    const double PsKM33 = myVars->KM33;	//	GAP	33	GAPi<->GAPo


    const double PsKM231 = myVars->KM231;
    const double PsKM232 = myVars->KM232;
    const double PsKM233 = myVars->KM233;
    const double PsKM234 = myVars->KM234;
    const double PsKA231 = myVars->KA231;
    const double PsKI231 = myVars->KI231;
    const double PsKVmo = myVars->KVmo;
    const double PsKE23 = myVars->KE23;

    const double PsKM241 = myVars->KM241;
    const double PsKE25 = myVars->KE25;

    const double PsKE6 = myVars->KE6;
    const double PsKE8 = myVars->KE8;
    const double PsKE9 = myVars->KE9;
    const double PsKE13 = myVars->KE13;
    const double PsKE16 = myVars->KE16;
    const double PsKM163 = myVars->KM163;

    // Initialize the PrVmax of the different reactions based on the global variables Vmax
    const double RegFactor = 1;

    const double PsV1 = myVars->V1;	//	1	Rubisco	RuBP+CO2<->2PGA
    const double PsV2 = myVars->V2;	//	2	PGA Kinase	PGA+ATP <-> ADP + DPGA
    const double PsV3 = myVars->V3;	//	3	GAP dehydragenase	DPGA+NADPH <->GAP + OP+NADP
    const double PsV5 = myVars->V5;	//	5	Aldolase	GAP+DHAP <->FBP
    double PsV6 = myVars->V6;	//	6	FBPase	FBP<->F6P+OP
    const double PsV7 = myVars->V7;	//	7	Transketolase	F6P+GAP<->E4P+Xu5P
    const double PsV8 = myVars->V8;	//	8	Aldolase	E4P+DHAP<->SBP
    double PsV9 = myVars->V9;	//	9	SBPase	SBP<->S7P+OP
    double PsV13 = myVars->V13;	//	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
    double PsV16 = myVars->V16;	//	16	ATP synthase	ADP+Pi<->ATP
    const double PsV23 = myVars->V23;//	23	ATP + G-1P -> ADPG + PPi
    const double PsV31 = myVars->V31 * RegFactor;//	31	Phosphate translocator	DHAPi<->DHAPo
    const double PsV32 = myVars->V32 * RegFactor;//	32	Phosphate translocator	PGAi<->PGAo
    const double PsV33 = myVars->V33 * RegFactor;//	33	Phosphate translocator	GAPi<->GAPo
    const double PsV24 = myVars->V24;        // 24    ADPG --> ADP + Gn

    if (myVars->PSPR_SUCS_com)
        PsPEXT = myVars->SUCS2PS_Pic;


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // First here is one way of the redox regulation, assuming the regulation is instataneous.
    // in case that there are more work using the equilibrium of Thio with enzyme
    // as a way to regulate enzyme activities.
    if (myVars->RedoxReg_RA_com) {
        PsV6 = myVars->Redox2PS_V6;
        PsV9 = myVars->Redox2PS_V9;
        PsV13 = myVars->Redox2PS_V13;
        PsV16 = myVars->Redox2PS_V16;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // The second method of the redox regulation.
    // First set the global poool from the RROEA.

    double ATPreg;
    if (!myVars->RROEA_EPS_com) {
        // ATPreg = ATP/3;
        // ATPreg = PGA/3;         // If there is no regulation of enzyme activity, some forcing needed to be added.
    } else {
        ATPreg = 1;
    }

    // Setting the concentration
    const double RuBP = PS_con.RuBP;
    const double PGA = PS_con.PGA;
    const double DPGA = PS_con.DPGA;
    const double T3P = PS_con.T3P;
    const double ADPG = PS_con.ADPG;
    const double FBP = PS_con.FBP;
    const double E4P = PS_con.E4P;
    const double S7P = PS_con.S7P;
    const double SBP = PS_con.SBP;
    const double ATP = PS_con.ATP;
    const double NADPH = PS_con.NADPH;
    double CO2 = PS_con.CO2;
    double O2 = PS_con.O2;
    const double HexP = PS_con.HexP;
    const double PenP = PS_con.PenP;

    // Assuming that the regulation exists no matter there is enzyme regulation or not. ATPReg is
    // used to regulate the TP export and starch synthesis.


    // Now Calculate the concentration of the auxiliary variables.

    if (!myVars->StomCond_TrDynaPS_com) {
        CO2 = myVars->CO2_cond;
        O2 = myVars->O2_cond;
    }

    const double DHAP = T3P / (1 + myVars->KE4);
    const double GAP = myVars->KE4 * T3P / (1 + myVars->KE4);
    //////////////////////////////////////////////////
    // DHAP=  T3P*KE4/(1+KE4); ////WY201803
    // GAP =  T3P/(1+KE4);
    //////////////////////////////////////////////////////////////////////

    ATPreg = DHAP / 3;// If there is no regulation of enzyme activity, some forcing needed to be added.
    if (ATPreg > 1)
        ATPreg = 1;

    const double ADP = myVars->PS_C_CA - ATP;

    const double F6P = (HexP / PsKE21) / (1 + 1 / PsKE21 + PsKE22);
    const double G6P = HexP / (1 + 1 / PsKE21 + PsKE22);
    const double G1P = (HexP * PsKE22) / (1 + 1 / PsKE21 + PsKE22);

    const double Ru5P = PenP / (1 + 1 / PsKE11 + 1 / PsKE12);
    const double Ri5P = (PenP / PsKE11) / (1 + 1 / PsKE11 + 1 / PsKE12);
    const double Xu5P = (PenP / PsKE12) / (1 + 1 / PsKE11 + 1 / PsKE12);

    const double PR2PS_Pgca = Param[1];
    const double Pit = myVars->PS_C_CP - PGA - 2 * DPGA - GAP - DHAP - 2 * FBP - F6P - E4P - 2 * SBP - S7P - Xu5P - Ri5P - Ru5P - 2 * RuBP - G6P - G1P - ATP - PR2PS_Pgca;
    const double Pi = 0.5 * (-PsKE25 +  pow((PsKE25 * PsKE25 + 4 * Pit * PsKE25), 0.5));
    const double OPOP = Pit - Pi;

    if (!myVars->RedoxReg_RA_com) {
        ATPreg = PGA / 3;
    } else {
        ATPreg = 1;
    }

    myVars->V1Reg = 1 + PGA / PsKI11 + FBP / PsKI12 + SBP / PsKI13 + Pi / PsKI14 + NADPH / PsKI15;

    double v1;
    double tmp;
    if (myVars->RUBISCOMETHOD == 2) {
        tmp = PsV1 * RuBP / (RuBP + PsKM13 * myVars->V1Reg);
        v1 = tmp * CO2 / (CO2 + PsKM11 * (1 + O2 / PsKM12));

        if (RuBP < PsV1 / 2.5)
            v1 = v1 * RuBP / (PsV1 / 2.5);


    } else if (myVars->RUBISCOMETHOD == 1){
        //fprintf("M1  ");
        v1 = PsV1 * CO2 / (CO2 + PsKM11 * (1 + O2 / PsKM12));
        if (RuBP < PsV1 / 2.5)
            v1 = v1 * RuBP / (PsV1 / 2.0);// DNF was 2.5 not 2.0
    }

    const double v2 = PsV2 * PGA * ATP / ((PGA + PsKM21) * (ATP + PsKM22 * (1 + ADP / PsKM23)));
    const double v3 = PsV3 * DPGA * NADPH / ((DPGA + PsKM31a) * (NADPH + PsKM32b));
    const double v5 = PsV5 * (GAP * DHAP - FBP / PsKE5) / ((PsKM51 * PsKM52) * (1 + GAP / PsKM51 + DHAP / PsKM52 + FBP / PsKM53 + GAP * DHAP / (PsKM51 * PsKM52)));
    const double v8 = PsV8 * (DHAP * E4P - SBP / PsKE8) / ((E4P + PsKM82) * (DHAP + PsKM81));

    const double KE57 = 1.005 * 0.1 * myVars->PSRatio[93];
    const double Km8p5p = 0.118 * myVars->PSRatio[94];
    const double Km5p5p = 0.616 * myVars->PSRatio[95];
    const double KE810 = 0.8446 * myVars->PSRatio[96];
    const double Km5gap = 0.2727 * myVars->PSRatio[97];
    const double Km8f6p = 0.5443 * myVars->PSRatio[98];
    const double Km8s7p = 0.01576 * myVars->PSRatio[99];
    const double Km8gap = 0.09 * myVars->PSRatio[100];
    const double Den = 1 + (1 + GAP / Km5gap) * (F6P / Km8f6p + S7P / Km8s7p) + GAP / Km8gap + 1 / Km8p5p * (Xu5P * (1 + E4P * Ri5P / Km5p5p) + E4P + Ri5P);

    const double v7 = PsV7 * (F6P * GAP * KE57 - E4P * Xu5P) / (Km8p5p * Km5p5p * Den);
    const double v10 = PsV7 * (S7P * GAP * KE810 - Xu5P * Ri5P) / (Km8p5p * Km5p5p * Den);
    const double v6 = PsV6 * (FBP - F6P * Pi / PsKE6) / (FBP + PsKM61 * (1 + F6P / PsKI61 + Pi / PsKI62));

    const double v9 = PsV9 * (SBP - Pi * S7P / PsKE9) / (SBP + PsKM9 * (1 + Pi / PsKI9));
    const double v13 = PsV13 * (ATP * Ru5P - ADP * RuBP / PsKE13) / ((ATP * (1 + ADP / PsKI134) + PsKM132 * (1 + ADP / PsKI135)) * (Ru5P + PsKM131 * (1 + PGA / PsKI131 + RuBP / PsKI132 + Pi / PsKI133)));
    const double v16 = PsV16 * (ADP * Pi - ATP / PsKE16) / (PsKM161 * PsKM162 * (1 + ADP / PsKM161 + Pi / PsKM162 + ATP / PsKM163 + ADP * Pi / (PsKM161 * PsKM162)));

    const double Va = PsKVmo + PsV23 * (PGA / (PsKA231 * (1 + PGA / PsKA231)));
    const double v23num = Va * (ATP * G1P - ADPG * OPOP / PsKE23);// The reason we set this here is to assume that we can obtain a reverse reaction here. However, a more realistic
    // way to achieve the homeostasis might be to allow starch breakdown and allow regulation of SBPase and FBPase.

    // WY 201803
    const double v23den2 = (1 + Pi / PsKI231) * PsKM231 * PsKM232 * (1 + ATP / PsKM232 + G1P / PsKM231 + ATP * G1P / (PsKM231 * PsKM232) + ADPG / PsKM233 + OPOP / PsKM234 + ADPG * OPOP / (PsKM233 * PsKM234));
    const double v23 = v23num / v23den2;
    const double v24num = PsV24 * (ADPG);// Similar to the argument for reaction 23. The control of homeostasis might be better enforced at the point of reaction 23.

    const double v24dem = PsKM241 * (1 + ADPG / PsKM241);
    const double v24 = v24num / v24dem;

    const double MaxCoeff = 5 * myVars->PSRatio[101];
    const double V25max = 0.5 * myVars->PSRatio[102] / 100 / 5;//WY201803

    const double v25 = V25max * (1 - RuBP / MaxCoeff) * ATP / (ATP + 1);
    //WY201803
    double v31 = PsV31 * DHAP / (DHAP + PsKM311) * PsPEXT / (PsPEXT + PsKM313);
    double v32 = PsV32 * PGA / (PGA + PsKM32) * PsPEXT / (PsPEXT + PsKM313);
    double v33 = PsV33 * GAP / (GAP + PsKM33) * PsPEXT / (PsPEXT + PsKM313);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    v31 = v31 * ATPreg;
    v32 = v32 * ATPreg;
    v33 = v33 * ATPreg;

    myVars->PS2EPS_V16 = v16;

    myVars->PS2EPS_v3 = v3;

    myVars->PSPR_RA_O2 = O2;
    myVars->PSPR_RA_CO2 = CO2;

    myVars->PS2RA_ATP = ATP;

    // information is sent back to PR by PS2PR_Pi global variable.

    myVars->PS2PR_Pi = Pi;

    myVars->PS2PR_ATP = ATP;

    myVars->PS2PR_ADP = ADP;

    myVars->PS2BF_ADP = ADP;

    myVars->PS2BF_Pi = Pi;

    // Notice the series PS2CM is used both in the CM model and the FPSReg model and thereafter.
    myVars->PS2CM_vdhap = v31;

    myVars->PS2CM_vpga = v32;

    myVars->PS2CM_vgap = v33;

    myVars->PS2PRC = CO2;
    myVars->PS2PRO = O2;

    myVars->PS2RubACC = CO2;
    myVars->PS2RubACO = O2;

    myVars->PS_Vel.v1 = v1;
    myVars->PS_Vel.v2 = v2;
    myVars->PS_Vel.v3 = v3;
    myVars->PS_Vel.v4 = 0;
    myVars->PS_Vel.v5 = v5;
    myVars->PS_Vel.v6 = v6;
    myVars->PS_Vel.v7 = v7;
    myVars->PS_Vel.v8 = v8;
    myVars->PS_Vel.v9 = v9;
    myVars->PS_Vel.v10 = v10;
    myVars->PS_Vel.v13 = v13;
    myVars->PS_Vel.v16 = v16;
    myVars->PS_Vel.v23 = v23;
    myVars->PS_Vel.v31 = v31;
    myVars->PS_Vel.v32 = v32;
    myVars->PS_Vel.v33 = v33;
    myVars->PS_Vel.v24 = v24;
    myVars->PS_Vel.v25 = v25;

    // Getting the information for output as figures.

    if (t > myVars->PS_OLD_TIME) {
            myVars->PS_TIME_N = myVars->PS_TIME_N + 1;
            myVars->PS_OLD_TIME = t;
        }
    if (myVars->record) {
        myVars->PS_VEL.insert(myVars->PS_TIME_N - 1, t, myVars->PS_Vel);

        // Transfer the variables for output

        myVars->PS2OUT[0] = RuBP;
        myVars->PS2OUT[1] = PGA;
        myVars->PS2OUT[2] = DPGA;
        myVars->PS2OUT[3] = T3P;
        myVars->PS2OUT[4] = ADPG;
        myVars->PS2OUT[5] = FBP;
        myVars->PS2OUT[6] = E4P;
        myVars->PS2OUT[7] = S7P;
        myVars->PS2OUT[8] = SBP;
        myVars->PS2OUT[9] = ATP;
        myVars->PS2OUT[10] = NADPH;
        myVars->PS2OUT[11] = CO2;
        myVars->PS2OUT[12] = O2;
        myVars->PS2OUT[13] = HexP;
        myVars->PS2OUT[14] = PenP;
        myVars->PS2OUT[15] = Pi;
        myVars->PS2OUT[16] = ADP;
        myVars->PS2OUT[17] = v1;
    }
}
