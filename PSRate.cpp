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

void PSRate(double t, PSCon &PS_con, arr &Param, Variables *myVars) {
    double PsPEXT = myVars->PS_PEXT;

    // First the physical and chemical constant for all the reactions

    // Set the value to the local variables: for example: PrKM11

    // Initialize the PrVmax of the different reactions based on the global variables Vmax
    const double RegFactor = 1;

    double PsV6 = myVars->V6;	//	6	FBPase	FBP<->F6P+OP
    double PsV9 = myVars->V9;	//	9	SBPase	SBP<->S7P+OP
    double PsV13 = myVars->V13;	//	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
    double PsV16 = myVars->V16;	//	16	ATP synthase	ADP+Pi<->ATP
    const double PsV31 = myVars->V31 * RegFactor;//	31	Phosphate translocator	DHAPi<->DHAPo
    const double PsV32 = myVars->V32 * RegFactor;//	32	Phosphate translocator	PGAi<->PGAo
    const double PsV33 = myVars->V33 * RegFactor;//	33	Phosphate translocator	GAPi<->GAPo

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

    //double ATPreg;
    //if (!myVars->RROEA_EPS_com) {
        // ATPreg = ATP/3;
        // ATPreg = PGA/3;         // If there is no regulation of enzyme activity, some forcing needed to be added.
    //} else {
    //    ATPreg = 1;
    //}

    // Assuming that the regulation exists no matter there is enzyme regulation or not. ATPReg is
    // used to regulate the TP export and starch synthesis.


    // Now Calculate the concentration of the auxiliary variables.

    const double DHAP = PS_con.T3P / (1 + myVars->KE4);
    const double GAP = myVars->KE4 * PS_con.T3P / (1 + myVars->KE4);
    //////////////////////////////////////////////////
    // DHAP=  T3P*KE4/(1+KE4); ////WY201803
    // GAP =  T3P/(1+KE4);
    //////////////////////////////////////////////////////////////////////

    double ATPreg = DHAP / 3;// If there is no regulation of enzyme activity, some forcing needed to be added.
    if (ATPreg > 1)
        ATPreg = 1;

    const double ADP = myVars->PS_C_CA - PS_con.ATP;

    const double F6P = (PS_con.HexP / myVars->KE21) / (1 + 1 / myVars->KE21 + myVars->KE22);
    const double G6P = PS_con.HexP / (1 + 1 / myVars->KE21 + myVars->KE22);
    const double G1P = (PS_con.HexP * myVars->KE22) / (1 + 1 / myVars->KE21 + myVars->KE22);

    const double Ru5P = PS_con.PenP / (1 + 1 / myVars->KE11 + 1 / myVars->KE12);
    const double Ri5P = (PS_con.PenP / myVars->KE11) / (1 + 1 / myVars->KE11 + 1 / myVars->KE12);
    const double Xu5P = (PS_con.PenP / myVars->KE12) / (1 + 1 / myVars->KE11 + 1 / myVars->KE12);

    const double Pit = myVars->PS_C_CP - PS_con.PGA - 2 * PS_con.DPGA - GAP - DHAP - 2 * PS_con.FBP - F6P - PS_con.E4P - 2 * PS_con.SBP - PS_con.S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con.RuBP - G6P - G1P - PS_con.ATP - Param[1];
    const double Pi = 0.5 * (-myVars->KE25 +  pow((myVars->KE25 * myVars->KE25 + 4 * Pit * myVars->KE25), 0.5));
    const double OPOP = Pit - Pi;

    if (!myVars->RedoxReg_RA_com) {
        ATPreg = PS_con.PGA / 3;
    } else {
        ATPreg = 1;
    }

    myVars->V1Reg = 1 + PS_con.PGA / myVars->KI11 + PS_con.FBP / myVars->KI12 + PS_con.SBP / myVars->KI13 + Pi / myVars->KI14 + PS_con.NADPH / myVars->KI15;

    //double v1;
    double tmp;
    if (myVars->RUBISCOMETHOD == 2) {
        tmp = myVars->V1 * PS_con.RuBP / (PS_con.RuBP + myVars->KM13 * myVars->V1Reg);
        myVars->PS_Vel.v1 = tmp * myVars->CO2_cond / (myVars->CO2_cond + myVars->KM11 * (1 + myVars->O2_cond / myVars->KM12));

        if (PS_con.RuBP < myVars->V1 / 2.5)
            myVars->PS_Vel.v1 = myVars->PS_Vel.v1 * PS_con.RuBP / (myVars->V1 / 2.5);


    } else if (myVars->RUBISCOMETHOD == 1){
        //fprintf("M1  ");
        myVars->PS_Vel.v1 = myVars->V1 * myVars->CO2_cond / (myVars->CO2_cond + myVars->KM11 * (1 + myVars->O2_cond / myVars->KM12));
        if (PS_con.RuBP < myVars->V1 / 2.5)
            myVars->PS_Vel.v1 = myVars->PS_Vel.v1 * PS_con.RuBP / (myVars->V1 / 2.0);// DNF was 2.5 not 2.0
    }

    const double KE57 = 1.005 * 0.1 * myVars->PSRatio[93];
    const double Km8p5p = 0.118 * myVars->PSRatio[94];
    const double Km5p5p = 0.616 * myVars->PSRatio[95];
    const double KE810 = 0.8446 * myVars->PSRatio[96];
    const double Km5gap = 0.2727 * myVars->PSRatio[97];
    const double Km8f6p = 0.5443 * myVars->PSRatio[98];
    const double Km8s7p = 0.01576 * myVars->PSRatio[99];
    const double Km8gap = 0.09 * myVars->PSRatio[100];
    const double Den = 1 + (1 + GAP / Km5gap) * (F6P / Km8f6p + PS_con.S7P / Km8s7p) + GAP / Km8gap + 1 / Km8p5p * (Xu5P * (1 + PS_con.E4P * Ri5P / Km5p5p) + PS_con.E4P + Ri5P);

    const double Va = myVars->KVmo + myVars->V23 * (PS_con.PGA / (myVars->KA231 * (1 + PS_con.PGA / myVars->KA231)));
    const double v23num = Va * (PS_con.ATP * G1P - PS_con.ADPG * OPOP / myVars->KE23);// The reason we set this here is to assume that we can obtain a reverse reaction here. However, a more realistic
    // way to achieve the homeostasis might be to allow starch breakdown and allow regulation of SBPase and FBPase.

    // WY 201803
    const double v23den2 = (1 + Pi / myVars->KI231) * myVars->KM231 * myVars->KM232 * (1 + PS_con.ATP / myVars->KM232 + G1P / myVars->KM231 + PS_con.ATP * G1P / (myVars->KM231 * myVars->KM232) + PS_con.ADPG / myVars->KM233 + OPOP / myVars->KM234 + PS_con.ADPG * OPOP / (myVars->KM233 * myVars->KM234));

    const double MaxCoeff = 5 * myVars->PSRatio[101];


    myVars->PS2RA_ATP = PS_con.ATP;

    // information is sent back to PR by PS2PR_Pi global variable.

    myVars->PS2PR_Pi = Pi;

    myVars->PS2PR_ATP = PS_con.ATP;

    myVars->PS2PR_ADP = ADP;

    myVars->PS2BF_ADP = ADP;

    myVars->PS2BF_Pi = Pi;

    //myVars->PS_Vel.v1 = v1;
    myVars->PS_Vel.v2 = myVars->V2 * PS_con.PGA * PS_con.ATP / ((PS_con.PGA + myVars->KM21) * (PS_con.ATP + myVars->KM22 * (1 + ADP / myVars->KM23)));
    myVars->PS_Vel.v3 = myVars->V3 * PS_con.DPGA * PS_con.NADPH / ((PS_con.DPGA + myVars->KM31a) * (PS_con.NADPH + myVars->KM32b));
    myVars->PS_Vel.v4 = 0;
    myVars->PS_Vel.v5 = myVars->V5 * (GAP * DHAP - PS_con.FBP / myVars->KE5) / ((myVars->KM51 * myVars->KM52) * (1 + GAP / myVars->KM51 + DHAP / myVars->KM52 + PS_con.FBP / myVars->KM53 + GAP * DHAP / (myVars->KM51 * myVars->KM52)));
    myVars->PS_Vel.v6 = PsV6 * (PS_con.FBP - F6P * Pi / myVars->KE6) / (PS_con.FBP + myVars->KM61 * (1 + F6P / myVars->KI61 + Pi / myVars->KI62));
    myVars->PS_Vel.v7 = myVars->V7 * (F6P * GAP * KE57 - PS_con.E4P * Xu5P) / (Km8p5p * Km5p5p * Den);
    myVars->PS_Vel.v8 = myVars->V8 * (DHAP * PS_con.E4P - PS_con.SBP / myVars->KE8) / ((PS_con.E4P + myVars->KM82) * (DHAP + myVars->KM81));
    myVars->PS_Vel.v9 = PsV9 * (PS_con.SBP - Pi * PS_con.S7P / myVars->KE9) / (PS_con.SBP + myVars->KM9 * (1 + Pi / myVars->KI9));
    myVars->PS_Vel.v10 = myVars->V7 * (PS_con.S7P * GAP * KE810 - Xu5P * Ri5P) / (Km8p5p * Km5p5p * Den);
    myVars->PS_Vel.v13 = PsV13 * (PS_con.ATP * Ru5P - ADP * PS_con.RuBP / myVars->KE13) / ((PS_con.ATP * (1 + ADP / myVars->KI134) + myVars->KM132 * (1 + ADP / myVars->KI135)) * (Ru5P + myVars->KM131 * (1 + PS_con.PGA / myVars->KI131 + PS_con.RuBP / myVars->KI132 + Pi / myVars->KI133)));
    myVars->PS_Vel.v16 = PsV16 * (ADP * Pi - PS_con.ATP / myVars->KE16) / (myVars->KM161 * myVars->KM162 * (1 + ADP / myVars->KM161 + Pi / myVars->KM162 + PS_con.ATP / myVars->KM163 + ADP * Pi / (myVars->KM161 * myVars->KM162)));
    myVars->PS_Vel.v23 = v23num / v23den2;
    myVars->PS_Vel.v31 = (PsV31 * DHAP / (DHAP + myVars->KM311) * PsPEXT / (PsPEXT + myVars->KM313)) * ATPreg;
    myVars->PS_Vel.v32 = (PsV32 * PS_con.PGA / (PS_con.PGA + myVars->KM32) * PsPEXT / (PsPEXT + myVars->KM313)) * ATPreg;
    myVars->PS_Vel.v33 = (PsV33 * GAP / (GAP + myVars->KM33) * PsPEXT / (PsPEXT + myVars->KM313)) * ATPreg;
    myVars->PS_Vel.v24 = (myVars->V24 * (PS_con.ADPG)) / (myVars->KM241 * (1 + PS_con.ADPG / myVars->KM241));
    myVars->PS_Vel.v25 = (0.5 * myVars->PSRatio[102] / 100 / 5) * (1 - PS_con.RuBP / MaxCoeff) * PS_con.ATP / (PS_con.ATP + 1);

    // Getting the information for output as figures.

    if (t > myVars->PS_OLD_TIME) {
            myVars->PS_TIME_N = myVars->PS_TIME_N + 1;
            myVars->PS_OLD_TIME = t;
        }
    if (myVars->record) {
        myVars->PS_VEL.insert(myVars->PS_TIME_N - 1, t, myVars->PS_Vel);

        // Transfer the variables for output

        myVars->PS2OUT[0] = PS_con.RuBP;
        myVars->PS2OUT[1] = PS_con.PGA;
        myVars->PS2OUT[2] = PS_con.DPGA;
        myVars->PS2OUT[3] = PS_con.T3P;
        myVars->PS2OUT[4] = PS_con.ADPG;
        myVars->PS2OUT[5] = PS_con.FBP;
        myVars->PS2OUT[6] = PS_con.E4P;
        myVars->PS2OUT[7] = PS_con.S7P;
        myVars->PS2OUT[8] = PS_con.SBP;
        myVars->PS2OUT[9] = PS_con.ATP;
        myVars->PS2OUT[10] = PS_con.NADPH;
        myVars->PS2OUT[11] = myVars->CO2_cond;
        myVars->PS2OUT[12] = myVars->O2_cond;
        myVars->PS2OUT[13] = PS_con.HexP;
        myVars->PS2OUT[14] = PS_con.PenP;
        myVars->PS2OUT[15] = Pi;
        myVars->PS2OUT[16] = ADP;
        myVars->PS2OUT[17] = myVars->PS_Vel.v1;
    }
}
