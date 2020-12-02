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

void PS::PS_Rate(const double t, const PSCon &PS_con, const arr &Param, Variables *theVars) {
    double PsPEXT = PS_PEXT;

    // First the physical and chemical constant for all the reactions

    // Set the value to the local variables: for example: PrKM11

    // Initialize the PrVmax of the different reactions based on the global variables Vmax
    const double RegFactor = 1;

    double PsV6 = theVars->V6;	//	6	FBPase	FBP<->F6P+OP
    double PsV9 = theVars->V9;	//	9	SBPase	SBP<->S7P+OP
    double PsV13 = theVars->V13;	//	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
    double PsV16 = theVars->V16;	//	16	ATP synthase	ADP+Pi<->ATP
    const double PsV31 = V31 * RegFactor;//	31	Phosphate translocator	DHAPi<->DHAPo
    const double PsV32 = V32 * RegFactor;//	32	Phosphate translocator	PGAi<->PGAo
    const double PsV33 = V33 * RegFactor;//	33	Phosphate translocator	GAPi<->GAPo

    if (theVars->PSPR_SUCS_com)
        PsPEXT = theVars->SUCS2PS_Pic;


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // First here is one way of the redox regulation, assuming the regulation is instataneous.
    // in case that there are more work using the equilibrium of Thio with enzyme
    // as a way to regulate enzyme activities.
    if (theVars->RedoxReg_RA_com) {
        PsV6 = theVars->Redox2PS_V6;
        PsV9 = theVars->Redox2PS_V9;
        PsV13 = theVars->Redox2PS_V13;
        PsV16 = theVars->Redox2PS_V16;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // The second method of the redox regulation.
    // First set the global poool from the RROEA.

    //double ATPreg;
    //if (!theVars->RROEA_EPS_com) {
        // ATPreg = ATP/3;
        // ATPreg = PGA/3;         // If there is no regulation of enzyme activity, some forcing needed to be added.
    //} else {
    //    ATPreg = 1;
    //}

    // Assuming that the regulation exists no matter there is enzyme regulation or not. ATPReg is
    // used to regulate the TP export and starch synthesis.


    // Now Calculate the concentration of the auxiliary variables.

    const double DHAP = PS_con.T3P / (1 + KE4);
    const double GAP = KE4 * PS_con.T3P / (1 + KE4);
    //////////////////////////////////////////////////
    // DHAP=  T3P*KE4/(1+KE4); ////WY201803
    // GAP =  T3P/(1+KE4);
    //////////////////////////////////////////////////////////////////////

    double ATPreg = DHAP / 3;// If there is no regulation of enzyme activity, some forcing needed to be added.
    if (ATPreg > 1)
        ATPreg = 1;

    const double ADP = PS_C_CA - PS_con.ATP;

    const double F6P = (PS_con.HexP / KE21) / (1 + 1 / KE21 + KE22);
    const double G6P = PS_con.HexP / (1 + 1 / KE21 + KE22);
    const double G1P = (PS_con.HexP * KE22) / (1 + 1 / KE21 + KE22);

    const double Ru5P = PS_con.PenP / (1 + 1 / KE11 + 1 / KE12);
    const double Ri5P = (PS_con.PenP / KE11) / (1 + 1 / KE11 + 1 / KE12);
    const double Xu5P = (PS_con.PenP / KE12) / (1 + 1 / KE11 + 1 / KE12);

    const double Pit = PS_C_CP - PS_con.PGA - 2 * PS_con.DPGA - GAP - DHAP - 2 * PS_con.FBP - F6P - PS_con.E4P - 2 * PS_con.SBP - PS_con.S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con.RuBP - G6P - G1P - PS_con.ATP - Param[1];
    const double Pi = 0.5 * (-KE25 +  pow((KE25 * KE25 + 4 * Pit * KE25), 0.5));
    const double OPOP = Pit - Pi;

    if (!theVars->RedoxReg_RA_com) {
        ATPreg = PS_con.PGA / 3;
    } else {
        ATPreg = 1;
    }

    theVars->V1Reg = 1 + PS_con.PGA / KI11 + PS_con.FBP / KI12 + PS_con.SBP / KI13 + Pi / KI14 + PS_con.NADPH / KI15;

    //double v1;
    double tmp;
    if (theVars->RUBISCOMETHOD == 2) {
        tmp = theVars->V1 * PS_con.RuBP / (PS_con.RuBP + KM13 * theVars->V1Reg);
        theVars->PS_Vel.v1 = tmp * theVars->CO2_cond / (theVars->CO2_cond + theVars->KM11 * (1 + theVars->O2_cond / theVars->KM12));

        if (PS_con.RuBP < theVars->V1 / 2.5)
            theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con.RuBP / (theVars->V1 / 2.5);


    } else if (theVars->RUBISCOMETHOD == 1){
        //fprintf("M1  ");
        theVars->PS_Vel.v1 = theVars->V1 * theVars->CO2_cond / (theVars->CO2_cond + theVars->KM11 * (1 + theVars->O2_cond / theVars->KM12));
        if (PS_con.RuBP < theVars->V1 / 2.5)
            theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con.RuBP / (theVars->V1 / 2.0);// DNF was 2.5 not 2.0
    }

    const double KE57 = 1.005 * 0.1 * theVars->PSRatio[93];
    const double Km8p5p = 0.118 * theVars->PSRatio[94];
    const double Km5p5p = 0.616 * theVars->PSRatio[95];
    const double KE810 = 0.8446 * theVars->PSRatio[96];
    const double Km5gap = 0.2727 * theVars->PSRatio[97];
    const double Km8f6p = 0.5443 * theVars->PSRatio[98];
    const double Km8s7p = 0.01576 * theVars->PSRatio[99];
    const double Km8gap = 0.09 * theVars->PSRatio[100];
    const double Den = 1 + (1 + GAP / Km5gap) * (F6P / Km8f6p + PS_con.S7P / Km8s7p) + GAP / Km8gap + 1 / Km8p5p * (Xu5P * (1 + PS_con.E4P * Ri5P / Km5p5p) + PS_con.E4P + Ri5P);

    const double Va = KVmo + theVars->V23 * (PS_con.PGA / (KA231 * (1 + PS_con.PGA / KA231)));
    const double v23num = Va * (PS_con.ATP * G1P - PS_con.ADPG * OPOP / KE23);// The reason we set this here is to assume that we can obtain a reverse reaction here. However, a more realistic
    // way to achieve the homeostasis might be to allow starch breakdown and allow regulation of SBPase and FBPase.

    // WY 201803
    const double v23den2 = (1 + Pi / KI231) * KM231 * KM232 * (1 + PS_con.ATP / KM232 + G1P / KM231 + PS_con.ATP * G1P / (KM231 * KM232) + PS_con.ADPG / KM233 + OPOP / KM234 + PS_con.ADPG * OPOP / (KM233 * KM234));

    const double MaxCoeff = 5 * theVars->PSRatio[101];


    theVars->PS2RA_ATP = PS_con.ATP;

    // information is sent back to PR by PS2PR_Pi global variable.

    theVars->PS2PR_Pi = Pi;

    theVars->PS2PR_ATP = PS_con.ATP;

    theVars->PS2PR_ADP = ADP;

    theVars->PS2BF_ADP = ADP;

    theVars->PS2BF_Pi = Pi;

    //theVars->PS_Vel.v1 = v1;
    theVars->PS_Vel.v2 = theVars->V2 * PS_con.PGA * PS_con.ATP / ((PS_con.PGA + KM21) * (PS_con.ATP + KM22 * (1 + ADP / KM23)));
    theVars->PS_Vel.v3 = theVars->V3 * PS_con.DPGA * PS_con.NADPH / ((PS_con.DPGA + KM31a) * (PS_con.NADPH + KM32b));
    theVars->PS_Vel.v4 = 0;
    theVars->PS_Vel.v5 = V5 * (GAP * DHAP - PS_con.FBP / KE5) / ((KM51 * KM52) * (1 + GAP / KM51 + DHAP / KM52 + PS_con.FBP / KM53 + GAP * DHAP / (KM51 * KM52)));
    theVars->PS_Vel.v6 = PsV6 * (PS_con.FBP - F6P * Pi / KE6) / (PS_con.FBP + KM61 * (1 + F6P / KI61 + Pi / KI62));
    theVars->PS_Vel.v7 = V7 * (F6P * GAP * KE57 - PS_con.E4P * Xu5P) / (Km8p5p * Km5p5p * Den);
    theVars->PS_Vel.v8 = V8 * (DHAP * PS_con.E4P - PS_con.SBP / KE8) / ((PS_con.E4P + KM82) * (DHAP + KM81));
    theVars->PS_Vel.v9 = PsV9 * (PS_con.SBP - Pi * PS_con.S7P / KE9) / (PS_con.SBP + KM9 * (1 + Pi / KI9));
    theVars->PS_Vel.v10 = V7 * (PS_con.S7P * GAP * KE810 - Xu5P * Ri5P) / (Km8p5p * Km5p5p * Den);
    theVars->PS_Vel.v13 = PsV13 * (PS_con.ATP * Ru5P - ADP * PS_con.RuBP / KE13) / ((PS_con.ATP * (1 + ADP / KI134) + KM132 * (1 + ADP / KI135)) * (Ru5P + KM131 * (1 + PS_con.PGA / KI131 + PS_con.RuBP / KI132 + Pi / KI133)));
    theVars->PS_Vel.v16 = PsV16 * (ADP * Pi - PS_con.ATP / KE16) / (KM161 * KM162 * (1 + ADP / KM161 + Pi / KM162 + PS_con.ATP / KM163 + ADP * Pi / (KM161 * KM162)));
    theVars->PS_Vel.v23 = v23num / v23den2;
    theVars->PS_Vel.v31 = (PsV31 * DHAP / (DHAP + KM311) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
    theVars->PS_Vel.v32 = (PsV32 * PS_con.PGA / (PS_con.PGA + KM32) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
    theVars->PS_Vel.v33 = (PsV33 * GAP / (GAP + KM33) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
    theVars->PS_Vel.v24 = (V24 * (PS_con.ADPG)) / (KM241 * (1 + PS_con.ADPG / KM241));
    theVars->PS_Vel.v25 = (0.5 * theVars->PSRatio[102] / 100 / 5) * (1 - PS_con.RuBP / MaxCoeff) * PS_con.ATP / (PS_con.ATP + 1);

    // Getting the information for output as figures.

    if (t > theVars->PS_OLD_TIME) {
            theVars->PS_TIME_N = theVars->PS_TIME_N + 1;
            theVars->PS_OLD_TIME = t;
        }
    if (theVars->record) {
        theVars->PS_VEL.insert(theVars->PS_TIME_N - 1, t, theVars->PS_Vel);

        // Transfer the variables for output

        theVars->PS2OUT.RuBP = PS_con.RuBP;
        theVars->PS2OUT.PGA = PS_con.PGA;
        theVars->PS2OUT.DPGA = PS_con.DPGA;
        theVars->PS2OUT.T3P = PS_con.T3P;
        theVars->PS2OUT.ADPG = PS_con.ADPG;
        theVars->PS2OUT.FBP = PS_con.FBP;
        theVars->PS2OUT.E4P = PS_con.E4P;
        theVars->PS2OUT.S7P = PS_con.S7P;
        theVars->PS2OUT.SBP = PS_con.SBP;
        theVars->PS2OUT.ATP = PS_con.ATP;
        theVars->PS2OUT.NADPH = PS_con.NADPH;
        theVars->PS2OUT.CO2 = theVars->CO2_cond;
        theVars->PS2OUT.O2 = theVars->O2_cond;
        theVars->PS2OUT.HexP = PS_con.HexP;
        theVars->PS2OUT.PenP = PS_con.PenP;
        theVars->PS2OUT._Pi = Pi;
        theVars->PS2OUT._ADP = ADP;
        theVars->PS2OUT._v1 = theVars->PS_Vel.v1;
    }
}
