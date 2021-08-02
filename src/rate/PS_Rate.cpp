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
#include "modules/SUCS.hpp"
//#include "conditions/PS_PRCondition.hpp"
//#include "conditions/CMCondition.hpp"
#include "conditions/EPSCondition.hpp"
#include "conditions/RedoxRegCondition.hpp"
#include "modules/BF.hpp"

#define RegFactor 1.

void PS::_Rate(const double t, const PSCondition* PS_con, Variables *theVars) {
    double PsV6, PsV9, PsV13, PsV16, PsPEXT;
    //DEBUG_MESSAGE(PS_con, 5)

    PsPEXT = PS_PEXT;

    if (theVars->PSPR_SUCS_com)
        PsPEXT = SUCS::getSUCS2PS_Pic();
    double NADPH;
    if (theVars->FIBF_PSPR_com) {
        NADPH = PS_con->parent->parent->parent->FIBF_con->BF_con->NADPH;
    } else {
        NADPH = PS::_NADPH;
    }

    // Assuming that the regulation exists no matter there is enzyme regulation or not. ATPReg is
    // used to regulate the TP export and starch synthesis.
    // Now Calculate the concentration of the auxiliary variables.

    const double DHAP = PS_con->T3P / (1 + KE4);
    const double GAP = KE4 * PS_con->T3P / (1 + KE4);
    //////////////////////////////////////////////////
    // DHAP=  T3P*KE4/(1+KE4); ////WY201803
    // GAP =  T3P/(1+KE4);
    //////////////////////////////////////////////////////////////////////


    theVars->ADP = PS_C_CA - PS_con->ATP;
    const double F6P = (PS_con->HexP / KE21) / (1 + 1 / KE21 + KE22);
    const double G6P = PS_con->HexP / (1 + 1 / KE21 + KE22);
    const double G1P = (PS_con->HexP * KE22) / (1 + 1 / KE21 + KE22);
    const double Ru5P = PS_con->PenP / (1 + 1 / KE11 + 1 / KE12);
    const double Ri5P = (PS_con->PenP / KE11) / (1 + 1 / KE11 + 1 / KE12);
    const double Xu5P = (PS_con->PenP / KE12) / (1 + 1 / KE11 + 1 / KE12);

    double ATPreg;

    if (!theVars->RedoxReg_RA_com) {
        ATPreg = PS_con->PGA / 3;
    } else {
        ATPreg = 1;
    }

    //theVars->PS_Vel.v1 = v1;
    if (theVars->useC3) {
        const SUCSCondition *SUCS_con = PS_con->parent->parent->SUCS_con;

        PiTc = theVars->SUCS_Pool.PTc - 2 * (SUCS_con->FBPc + SUCS_con->F26BPc) - (SUCS_con->PGAc + SUCS_con->T3Pc + SUCS_con->HexPc + SUCS_con->SUCP + SUCS::UTPc + SUCS::ATPc);
        PsPEXT = (pow(pow(SUCS::KE61, 2) + 4 * SUCS::KE61 * PiTc, 0.5) - SUCS::KE61)/2;   //  SHARED

        theVars->Pi = PS_C_CP - PS_con->PGA - 2 * PS_con->DPGA - GAP - DHAP - 2 * PS_con->FBP - F6P - PS_con->E4P - 2 * PS_con->SBP - PS_con->S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con->RuBP - G6P - G1P - PS_con->ATP - PS_con->parent->PR_con->PGCA;

        //V1Reg = 1 + PS_con->PGA / KI11 + PS_con->FBP / KI12 + PS_con->SBP / KI13 + theVars->Pi / KI14 + NADPH / KI15;

        const double PsV1_0	= V1 * Vfactor1 * Vf_T1	;	//	1	Rubisco	RuBP+CO2<->2PGA
        const double PsV2_0	= V2 * Vfactor2 * Vf_T2 ;	//	2	PGA Kinase	PGA+ATP <-> ADP + DPGA
        const double PsV3_0	= V3 * Vfactor3 * Vf_T3	;	//	3	GAP dehydragenase	DPGA+NADPH <->GAP + OP+NADP

        const double PsV5_0	= V5 * Vfactor5 * Vf_T5;	//	5	Aldolase	GAP+DHAP <->FBP
        const double PsV6_0	= V6 * Vf_T6	;	//	6	FBPase	FBP<->F6P+OP
        const double PsV7_0	= V7 * Vfactor7	;	//	7	Transketolase	F6P+GAP<->E4P+Xu5P
        const double PsV8_0	= V8 * Vfactor5 * Vf_T5	;	//	8	Aldolase	E4P+DHAP<->SBP
        const double PsV9_0	= V9 * Vf_T9	;	//	9	SBPase	SBP<->S7P+OP
        const double PsV10_0 = V10 * Vfactor7	;	//	10	Transketolase	S7P+GAP<->Ri5P+Xu5P

        const double PsV13_0 = V13 * Vfactor13 * Vf_T13;	//	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
        double PsV16 = V16	;	//	16	ATP synthase	ADP+Pi<->ATP

        const double PsV23_0 = V23 * Vfactor23 * Vf_T23;//	23	ADP-glucose pyrophosphorylase and	ADPG+Gn<->G(n+1)+ADP
        const double PsV31 = V31 ;	//	31	Phosphate translocator	DHAPi<->DHAPo
        const double PsV32 = V32	;	//	32	Phosphate translocator	PGAi<->PGAo
        const double PsV33 = V33	;	//	33	Phosphate translocator	GAPi<->GAPo

        const double Ru_Act = -3. * pow(10., -5) * pow(theVars->Tp, 3) + 0.0013 * pow(theVars->Tp, 2) - 0.0106 * theVars->Tp + 0.8839; //Rubisco activition state   % SHARED
        PsV1 = PsV1_0 * Ru_Act * pow(Q10_1, (theVars->Tp - 25) / 10);                     //   SHARED
        const double PsV2 = PsV2_0 * pow(Q10_2, (theVars->Tp - 25) / 10);
        const double PsV3 = PsV3_0 * pow(Q10_3, (theVars->Tp - 25) / 10);
        const double PsV5 = PsV5_0 * pow(Q10_5, (theVars->Tp - 25) / 10);
        double PsV6 = PsV6_0 * pow(Q10_6, (theVars->Tp - 25) / 10);
        const double PsV7 = PsV7_0 * pow(Q10_7, (theVars->Tp - 25) / 10);
        const double PsV8 = PsV8_0 * pow(Q10_8, (theVars->Tp - 25) / 10);
        double PsV9 = PsV9_0 * pow(Q10_9, (theVars->Tp - 25) / 10);
        const double PsV10 = PsV10_0 * pow(Q10_10, (theVars->Tp - 25) / 10);
        double PsV13= PsV13_0 * pow(Q10_13, (theVars->Tp - 25) / 10);
        const double PsV23 = PsV23_0 * pow(Q10_23, (theVars->Tp - 25) / 10);

        // First here is one way of the redox regulation, assuming the regulation is instataneous.
        // in case that there are more work using the equilibrium of Thio with enzyme
        // as a way to regulate enzyme activities.
        if (theVars->RedoxReg_RA_com) {
            PsV6 = RedoxRegCondition::v6();
            PsV9 = RedoxRegCondition::v9();
            PsV13 = RedoxRegCondition::v13();
            PsV16 = RedoxRegCondition::v16();
        }

        const double CO2 = theVars->CO2_cond;
        const double O2 = theVars->O2_cond;


        V1Reg = 1 + PS_con->PGA / KI11 + PS_con->FBP / KI12 + PS_con->SBP / KI13 + theVars->Pi / KI14 + NADPH / KI15;   // SHARED

        if (theVars->RUBISCOMETHOD == 2) {
            const double tmp = PsV1 * PS_con->RuBP / (PS_con->RuBP + KM13 * V1Reg);
            theVars->PS_Vel.v1 = tmp * CO2 / (CO2 + KM11 * (1 + O2 / KM12));
            if (PS_con->RuBP < PsV1 / 2) {
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PsV1 / 2);
            }

        } else if (theVars->RUBISCOMETHOD == 1) {
            theVars->PS_Vel.v1 = PsV1 * CO2 / (CO2 + KM11 * (1 + O2 / KM12));
            if (PS_con->RuBP < PsV1 / 2)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PsV1 / 2);
        }

        theVars->PS_Vel.v2 = PsV2 * PS_con->PGA * PS_con->ATP / ((PS_con->PGA + KM21) * (PS_con->ATP + KM22 * (1 + theVars->ADP / KM23)));
        theVars->PS_Vel.v3 = PsV3 * PS_con->DPGA * NADPH / ((PS_con->DPGA + KM31a) * (NADPH + KM32b));
        theVars->PS_Vel.v5 = PsV5 * (GAP * DHAP - PS_con->FBP / KE5) / ((KM51 * KM52) * (1 + GAP / KM51 + DHAP / KM52 + PS_con->FBP / KM53 + GAP * DHAP / (KM51 * KM52)));
        theVars->PS_Vel.v8 = PsV8 * (DHAP * PS_con->E4P - PS_con->SBP / KE8) / ((PS_con->E4P + KM82) * (DHAP + KM81));
        theVars->PS_Vel.v6 = PsV6 * (PS_con->FBP - F6P * theVars->Pi / KE6) / (PS_con->FBP + KM61 * (1 + F6P / KI61 + theVars->Pi / KI62));
        theVars->PS_Vel.v7 = PsV7 * (F6P * GAP - Xu5P * PS_con->E4P / KE7) / ((F6P + KM73 * (1 + Xu5P / KM71 + PS_con->E4P / KM72)) * (GAP + KM74));
        theVars->PS_Vel.v9 = PsV9 * (PS_con->SBP - theVars->Pi * PS_con->S7P / KE9) / (PS_con->SBP + KM9 * (1 + theVars->Pi / KI9));
        theVars->PS_Vel.v10 = PsV10 * (GAP * PS_con->S7P - Ri5P * Xu5P / KE10) / ((GAP + KM102 * (1 + Xu5P / KM101 + Ri5P / KM10)) * (PS_con->S7P + KM103));
        theVars->PS_Vel.v13 = PsV13 * (PS_con->ATP * Ru5P - theVars->ADP * PS_con->RuBP / KE13) / ((PS_con->ATP * (1 + theVars->ADP / KI134) + KM132*(1 + theVars->ADP / KI135)) * (Ru5P + KM131 * (1 + PS_con->PGA / KI131 + PS_con->RuBP / KI132 + theVars->Pi / KI133)));

        const double I2 = theVars->TestLi * alfa * (1 - fc) / 2;
        const double J = (I2 + Jmax - sqrt(pow(I2 + Jmax, 2) - 4 * Theta * I2 * Jmax)) / (2 * Theta);
        theVars->PS_Vel.v16 = std::min(beta * J, PsV16 * (theVars->ADP * theVars->Pi - PS_con->ATP / KE16)/(KM161 * KM162 * (1 + theVars->ADP / KM161 + theVars->Pi / KM162 + PS_con->ATP / KM163 + theVars->ADP * theVars->Pi /(KM161 * KM162))));

        theVars->PS_Vel.v23 = PsV23 * G1P * PS_con->ATP /((G1P + KM231) * ((1 + theVars->ADP / KI23) * (PS_con->ATP + KM232) + (KM232 * theVars->Pi /(KA231 * PS_con->PGA + KA232 * F6P + KA233 * PS_con->FBP))));

        theVars->PS_Vel.v23 = PsV23 * G1P * PS_con->ATP / ((G1P + KM231) * ((1 + theVars->ADP / KI23) * (PS_con->ATP + KM232)+(KM232 * theVars->Pi/(KA231 * PS_con->PGA + KA232 * F6P + KA233 * PS_con->FBP))));
        const double N = 1 + (1 + KM313 / PsPEXT) * (theVars->Pi / KM312 + PS_con->PGA / KM32 + GAP / KM33 + DHAP / KM311);

        // The ATP regualtion really is implicit in the light regulation of sucrose synthesis.
        theVars->PS_Vel.v31 = PsV31 * DHAP / (N * KM311)  ;
        theVars->PS_Vel.v32 = PsV32 * PS_con->PGA / (N * KM32);
        theVars->PS_Vel.v33 = PsV33 * GAP / (N * KM33);

        theVars->PS_Vel.v23 = theVars->PS_Vel.v23 * ATPreg;
        theVars->PS_Vel.v31 = theVars->PS_Vel.v31 * ATPreg;
        theVars->PS_Vel.v32 = theVars->PS_Vel.v32 * ATPreg;
        theVars->PS_Vel.v33 = theVars->PS_Vel.v33 * ATPreg;
        if (!theVars->FIBF_PSPR_com){    // ModelMethod = 0 means that there is no connection between FIBF and PSPR.
            if (theVars->PS_Vel.v16 == 0.)    // This assmed that light reguate the export of triose phosphate export. This function should use
                 theVars->PS_Vel.v23 = 0;            // ATP as a signal.
        } else {
            if (BF::getEPS_ATP_Rate() == 0.)
                 theVars->PS_Vel.v23 = 0;
        }
        // Notice the series PS2CM is used both in the CM model and the FPSReg model and thereafter.
        //theVars->EPS_NADPH = PS_con->NADPH;

    } else {
        const double Pit = PS_C_CP - PS_con->PGA - 2 * PS_con->DPGA - GAP - DHAP - 2 * PS_con->FBP - F6P - PS_con->E4P - 2 * PS_con->SBP - PS_con->S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con->RuBP - G6P - G1P - PS_con->ATP - Param[1];
        theVars->Pi = 0.5 * (-KE25 +  pow((KE25 * KE25 + 4 * Pit * KE25), 0.5));
        const double OPOP = Pit - theVars->Pi;
        const double KE57 = 1.005 * 0.1 * theVars->PSRatio[93];
        const double Km8p5p = 0.118 * theVars->PSRatio[94];
        const double Km5p5p = 0.616 * theVars->PSRatio[95];
        const double KE810 = 0.8446 * theVars->PSRatio[96];
        const double Km5gap = 0.2727 * theVars->PSRatio[97];
        const double Km8f6p = 0.5443 * theVars->PSRatio[98];
        const double Km8s7p = 0.01576 * theVars->PSRatio[99];
        const double Km8gap = 0.09 * theVars->PSRatio[100];
        const double Den = 1 + (1 + GAP / Km5gap) * (F6P / Km8f6p + PS_con->S7P / Km8s7p) + GAP / Km8gap + 1 / Km8p5p * (Xu5P * (1 + PS_con->E4P * Ri5P / Km5p5p) + PS_con->E4P + Ri5P);

        const double Va = KVmo + V23 * (PS_con->PGA / (KA231 * (1 + PS_con->PGA / KA231)));
        const double v23num = Va * (PS_con->ATP * G1P - PS_con->ADPG * OPOP / KE23);// The reason we set this here is to assume that we can obtain a reverse reaction here. However, a more realistic
        // way to achieve the homeostasis might be to allow starch breakdown and allow regulation of SBPase and FBPase.

        // WY 201803
        const double v23den2 = (1 + theVars->Pi / KI231) * KM231 * KM232 * (1 + PS_con->ATP / KM232 + G1P / KM231 + PS_con->ATP * G1P / (KM231 * KM232) + PS_con->ADPG / KM233 + OPOP / KM234 + PS_con->ADPG * OPOP / (KM233 * KM234));

        const double MaxCoeff = 5 * theVars->PSRatio[101];

        V1Reg = 1 + PS_con->PGA / KI11 + PS_con->FBP / KI12 + PS_con->SBP / KI13 + theVars->Pi / KI14 + NADPH / KI15;

        // Initialize the PrVmax of the different reactions based on the global variables Vmax
        PsV6 = V6;            // 6 FBPase FBP<->F6P+OP
        PsV9 = V9;            // 9 SBPase SBP<->S7P+OP
        PsV13 = V13;          // 13 Ribulosebiphosphate kinase Ru5P+ATP<->RuBP+ADP
        PsV16 = V16;          // 16 ATP synthase ADP+Pi<->ATP
        const double PsV31 = V31 * RegFactor; // 31 Phosphate translocator DHAPi<->DHAPo
        const double PsV32 = V32 * RegFactor; // 32 Phosphate translocator PGAi<->PGAo
        const double PsV33 = V33 * RegFactor; // 33 Phosphate translocator GAPi<->GAPo

        // First here is one way of the redox regulation, assuming the regulation is instataneous.
        // in case that there are more work using the equilibrium of Thio with enzyme
        // as a way to regulate enzyme activities.
        if (theVars->RedoxReg_RA_com) {
            PsV6 = RedoxRegCondition::v6();
            PsV9 = RedoxRegCondition::v9();
            PsV13 = RedoxRegCondition::v13();
            PsV16 = RedoxRegCondition::v16();
        }

        if (theVars->RUBISCOMETHOD == 2) {
            const double tmp = V1 * PS_con->RuBP / (PS_con->RuBP + KM13 * V1Reg);
            theVars->PS_Vel.v1 = tmp * theVars->CO2_cond / (theVars->CO2_cond + KM11 * (1 + theVars->O2_cond / KM12));

            if (PS_con->RuBP < V1 / 2.5)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (V1 / 2.5);


        } else if (theVars->RUBISCOMETHOD == 1){
            //fprintf("M1  ");
            theVars->PS_Vel.v1 = V1 * theVars->CO2_cond / (theVars->CO2_cond + KM11 * (1 + theVars->O2_cond / KM12));
            if (PS_con->RuBP < V1 / 2.5)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (V1 / 2.0);// DNF was 2.5 not 2.0
        }

        theVars->PS_Vel.v2 = V2 * PS_con->PGA * PS_con->ATP / ((PS_con->PGA + KM21) * (PS_con->ATP + KM22 * (1 + theVars->ADP / KM23)));
        theVars->PS_Vel.v3 = V3 * PS_con->DPGA * NADPH / ((PS_con->DPGA + KM31a) * (NADPH + KM32b));
        theVars->PS_Vel.v4 = 0;
        theVars->PS_Vel.v5 = V5 * (GAP * DHAP - PS_con->FBP / KE5) / ((KM51 * KM52) * (1 + GAP / KM51 + DHAP / KM52 + PS_con->FBP / KM53 + GAP * DHAP / (KM51 * KM52)));
        theVars->PS_Vel.v6 = PsV6 * (PS_con->FBP - F6P * theVars->Pi / KE6) / (PS_con->FBP + KM61 * (1 + F6P / KI61 + theVars->Pi / KI62));

        theVars->PS_Vel.v7 = V7 * (F6P * GAP * KE57 - PS_con->E4P * Xu5P) / (Km8p5p * Km5p5p * Den);
        theVars->PS_Vel.v8 = V8 * (DHAP * PS_con->E4P - PS_con->SBP / KE8) / ((PS_con->E4P + KM82) * (DHAP + KM81));
        theVars->PS_Vel.v9 = PsV9 * (PS_con->SBP - theVars->Pi * PS_con->S7P / KE9) / (PS_con->SBP + KM9 * (1 + theVars->Pi / KI9));
        theVars->PS_Vel.v10 = V7 * (PS_con->S7P * GAP * KE810 - Xu5P * Ri5P) / (Km8p5p * Km5p5p * Den);

        theVars->PS_Vel.v13 = PsV13 * (PS_con->ATP * Ru5P - theVars->ADP * PS_con->RuBP / KE13) / ((PS_con->ATP * (1 + theVars->ADP / KI134) + KM132 * (1 + theVars->ADP / KI135)) * (Ru5P + KM131 * (1 + PS_con->PGA / KI131 + PS_con->RuBP / KI132 + theVars->Pi / KI133)));
        theVars->PS_Vel.v16 = PsV16 * (theVars->ADP * theVars->Pi - PS_con->ATP / KE16) / (KM161 * KM162 * (1 + theVars->ADP / KM161 + theVars->Pi / KM162 + PS_con->ATP / KM163 + theVars->ADP * theVars->Pi / (KM161 * KM162)));
        theVars->PS_Vel.v23 = v23num / v23den2;
        theVars->PS_Vel.v31 = (PsV31 * DHAP / (DHAP + KM311) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
        theVars->PS_Vel.v32 = (PsV32 * PS_con->PGA / (PS_con->PGA + KM32) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
        theVars->PS_Vel.v33 = (PsV33 * GAP / (GAP + KM33) * PsPEXT / (PsPEXT + KM313)) * ATPreg;
        theVars->PS_Vel.v24 = (V24 * (PS_con->ADPG)) / (KM241 * (1 + PS_con->ADPG / KM241));
        theVars->PS_Vel.v25 = (0.5 * theVars->PSRatio[102] / 100 / 5) * (1 - PS_con->RuBP / MaxCoeff) * PS_con->ATP / (PS_con->ATP + 1);
    }
    theVars->PS2BF_Pi = theVars->Pi;
    DEBUG_INTERNAL(theVars->PS_Vel)
    // Getting the information for output as figures.


    if (theVars->record) {
        if (t > PS::TIME) {
            PS::N++;
            PS::TIME = t;
        }
        theVars->PS_VEL.insert(PS::N - 1, t, theVars->PS_Vel);

        // Transfer the variables for output

        theVars->PS2OUT.RuBP = PS_con->RuBP;
        theVars->PS2OUT.PGA = PS_con->PGA;
        theVars->PS2OUT.DPGA = PS_con->DPGA;
        theVars->PS2OUT.T3P = PS_con->T3P;
        theVars->PS2OUT.ADPG = PS_con->ADPG;
        theVars->PS2OUT.FBP = PS_con->FBP;
        theVars->PS2OUT.E4P = PS_con->E4P;
        theVars->PS2OUT.S7P = PS_con->S7P;
        theVars->PS2OUT.SBP = PS_con->SBP;
        theVars->PS2OUT.ATP = PS_con->ATP;
        theVars->PS2OUT.HexP = PS_con->HexP;
        theVars->PS2OUT.PenP = PS_con->PenP;
        theVars->PS2OUT._Pi = theVars->Pi;
        theVars->PS2OUT._ADP = theVars->ADP;
        theVars->PS2OUT._v1 = theVars->PS_Vel.v1;
    }
}
