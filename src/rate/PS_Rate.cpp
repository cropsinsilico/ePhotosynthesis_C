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
#include "conditions/EPSCondition.hpp"
#include "conditions/RedoxRegCondition.hpp"
#include "modules/BF.hpp"

#define RegFactor 1.

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void PS::_Rate(const double t, const PSCondition* PS_con, Variables *theVars) {
    double PsV6, PsV9, PsV13, PsV16, PsPEXT;

    PsPEXT = PS::PS_PEXT;

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

    const double DHAP = PS_con->T3P / (1 + PS::KE4);
    const double GAP = PS::KE4 * PS_con->T3P / (1 + PS::KE4);
    //////////////////////////////////////////////////
    // DHAP=  T3P*PS::KE4/(1+PS::KE4); ////WY201803
    // GAP =  T3P/(1+PS::KE4);
    //////////////////////////////////////////////////////////////////////


    theVars->ADP = PS_C_CA - PS_con->ATP;
    const double F6P = (PS_con->HexP / PS::KE21) / (1 + 1 / PS::KE21 + PS::KE22);
    const double G6P = PS_con->HexP / (1 + 1 / PS::KE21 + PS::KE22);
    const double G1P = (PS_con->HexP * PS::KE22) / (1 + 1 / PS::KE21 + PS::KE22);
    const double Ru5P = PS_con->PenP / (1 + 1 / PS::KE11 + 1 / PS::KE12);
    const double Ri5P = (PS_con->PenP / PS::KE11) / (1 + 1 / PS::KE11 + 1 / PS::KE12);
    const double Xu5P = (PS_con->PenP / PS::KE12) / (1 + 1 / PS::KE11 + 1 / PS::KE12);

    double ATPreg;

    if (!theVars->RedoxReg_RA_com) {
        ATPreg = PS_con->PGA / 3;
    } else {
        ATPreg = 1;
    }

    //theVars->PS_Vel.v1 = v1;
    if (theVars->useC3) {
        const SUCSCondition *SUCS_con = PS_con->parent->parent->SUCS_con;

        PS::PiTc = theVars->SUCS_Pool.PTc - 2 * (SUCS_con->FBPc + SUCS_con->F26BPc) - (SUCS_con->PGAc + SUCS_con->T3Pc + SUCS_con->HexPc + SUCS_con->SUCP + SUCS::UTPc + SUCS::ATPc);
        PsPEXT = (pow(pow(SUCS::KE61, 2) + 4 * SUCS::KE61 * PS::PiTc, 0.5) - SUCS::KE61)/2;   //  SHARED

        theVars->Pi = PS::PS_C_CP - PS_con->PGA - 2 * PS_con->DPGA - GAP - DHAP - 2 * PS_con->FBP - F6P - PS_con->E4P - 2 * PS_con->SBP - PS_con->S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con->RuBP - G6P - G1P - PS_con->ATP - PS_con->parent->PR_con->PGCA;

        //V1Reg = 1 + PS_con->PGA / KI11 + PS_con->FBP / KI12 + PS_con->SBP / KI13 + theVars->Pi / KI14 + NADPH / KI15;

        const double PsV1_0	= PS::V1 * PS::Vfactor1 * PS::Vf_T1	;	//	1	Rubisco	RuBP+CO2<->2PGA
        const double PsV2_0	= PS::V2 * PS::Vfactor2 * PS::Vf_T2 ;	//	2	PGA Kinase	PGA+ATP <-> ADP + DPGA
        const double PsV3_0	= PS::V3 * PS::Vfactor3 * PS::Vf_T3	;	//	3	GAP dehydragenase	DPGA+NADPH <->GAP + OP+NADP

        const double PsV5_0	= PS::V5 * PS::Vfactor5 * PS::Vf_T5;	//	5	Aldolase	GAP+DHAP <->FBP
        const double PsV6_0	= PS::V6 * PS::Vf_T6	;	//	6	FBPase	FBP<->F6P+OP
        const double PsV7_0	= PS::V7 * PS::Vfactor7	;	//	7	Transketolase	F6P+GAP<->E4P+Xu5P
        const double PsV8_0	= PS::V8 * PS::Vfactor5 * PS::Vf_T5	;	//	8	Aldolase	E4P+DHAP<->SBP
        const double PsV9_0	= PS::V9 * PS::Vf_T9	;	//	9	SBPase	SBP<->S7P+OP
        const double PsV10_0 = PS::V10 * PS::Vfactor7	;	//	10	Transketolase	S7P+GAP<->Ri5P+Xu5P

        const double PsV13_0 = PS::V13 * PS::Vfactor13 * PS::Vf_T13;	//	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
        double PsV16 = PS::V16	;	//	16	ATP synthase	ADP+Pi<->ATP

        const double PsV23_0 = PS::V23 * PS::Vfactor23 * PS::Vf_T23;//	23	ADP-glucose pyrophosphorylase and	ADPG+Gn<->G(n+1)+ADP
        const double PsV31	=	PS::V31  * theVars->alpha2;	//	31	Phosphate translocator	DHAPi<->DHAPo
        const double PsV32	=	PS::V32  * theVars->alpha2;	//	32	Phosphate translocator	PGAi<->PGAo
        const double PsV33	=	PS::V33  * theVars->alpha2;	//	33	Phosphate translocator	GAPi<->GAPo

        const double Ru_Act = -3. * pow(10., -5.) * pow(theVars->Tp, 3.) + 0.0013 * pow(theVars->Tp, 2.) - 0.0106 * theVars->Tp + 0.8839; //Rubisco activition state   % SHARED
        PsV1 = PsV1_0 * Ru_Act * pow(Q10_1, (theVars->Tp - 25.) / 10.);                     //   SHARED
        const double PsV2 = PsV2_0 * pow(Q10_2, (theVars->Tp - 25.) / 10.);
        const double PsV3 = PsV3_0 * pow(Q10_3, (theVars->Tp - 25.) / 10.);
        const double PsV5 = PsV5_0 * pow(Q10_5, (theVars->Tp - 25.) / 10.);
        double PsV6 = PsV6_0 * pow(Q10_6, (theVars->Tp - 25.) / 10.);
        const double PsV7 = PsV7_0 * pow(Q10_7, (theVars->Tp - 25.) / 10.);
        const double PsV8 = PsV8_0 * pow(Q10_8, (theVars->Tp - 25.) / 10.);
        double PsV9 = PsV9_0 * pow(Q10_9, (theVars->Tp - 25.) / 10.);
        const double PsV10 = PsV10_0 * pow(Q10_10, (theVars->Tp - 25.) / 10.);
        double PsV13= PsV13_0 * pow(Q10_13, (theVars->Tp - 25.) / 10.);
        const double PsV23 = PsV23_0 * pow(Q10_23, (theVars->Tp - 25.) / 10.);

        // First here is one way of the redox regulation, assuming the regulation is instataneous.
        // in case that there are more work using the equilibrium of Thio with enzyme
        // as a way to regulate enzyme activities.
        if (theVars->RedoxReg_RA_com) {
            PsV6 = RedoxRegCondition::getV6();
            PsV9 = RedoxRegCondition::getV9();
            PsV13 = RedoxRegCondition::getV13();
            PsV16 = RedoxRegCondition::getV16();
        }

        const double CO2 = theVars->CO2_cond;
        const double O2 = theVars->O2_cond;


        V1Reg = 1 + PS_con->PGA / PS::KI11 + PS_con->FBP / PS::KI12 + PS_con->SBP / PS::KI13 + theVars->Pi / PS::KI14 + NADPH / PS::KI15;   // SHARED

        if (theVars->RUBISCOMETHOD == 2) {
            const double tmp = PS::PsV1 * PS_con->RuBP / (PS_con->RuBP + PS::KM13 * PS::V1Reg);
            theVars->PS_Vel.v1 = tmp * CO2 / (CO2 + PS::KM11 * (1. + O2 / PS::KM12));
            if (PS_con->RuBP < PS::PsV1 / 2.) {
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PS::PsV1 / 2);
            }

        } else if (theVars->RUBISCOMETHOD == 1) {
            theVars->PS_Vel.v1 = PS::PsV1 * CO2 / (CO2 + KM11 * (1. + O2 / PS::KM12));
            if (PS_con->RuBP < PS::PsV1 / 2.)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PS::PsV1 / 2);
        }

        theVars->PS_Vel.v2 = PsV2 * PS_con->PGA * PS_con->ATP / ((PS_con->PGA + PS::KM21) * (PS_con->ATP + PS::KM22 * (1. + theVars->ADP / PS::KM23)));
        theVars->PS_Vel.v3 = PsV3 * PS_con->DPGA * NADPH / ((PS_con->DPGA + PS::KM31a) * (NADPH + PS::KM32b));
        theVars->PS_Vel.v5 = PsV5 * (GAP * DHAP - PS_con->FBP / PS::KE5) / ((PS::KM51 * PS::KM52) * (1. + GAP / PS::KM51 + DHAP / PS::KM52 + PS_con->FBP / PS::KM53 + GAP * DHAP / (PS::KM51 * PS::KM52)));
        theVars->PS_Vel.v8 = PsV8 * (DHAP * PS_con->E4P - PS_con->SBP / PS::KE8) / ((PS_con->E4P + PS::KM82) * (DHAP + PS::KM81));
        theVars->PS_Vel.v6 = PsV6 * (PS_con->FBP - F6P * theVars->Pi / PS::KE6) / (PS_con->FBP + PS::KM61 * (1 + F6P / PS::KI61 + theVars->Pi / PS::KI62));
        theVars->PS_Vel.v7 = PsV7 * (F6P * GAP - Xu5P * PS_con->E4P / PS::KE7) / ((F6P + PS::KM73 * (1 + Xu5P / PS::KM71 + PS_con->E4P / PS::KM72)) * (GAP + KM74));
        theVars->PS_Vel.v9 = PsV9 * (PS_con->SBP - theVars->Pi * PS_con->S7P / PS::KE9) / (PS_con->SBP + PS::KM9 * (1 + theVars->Pi / PS::KI9));
        theVars->PS_Vel.v10 = PsV10 * (GAP * PS_con->S7P - Ri5P * Xu5P / PS::KE10) / ((GAP + PS::KM102 * (1 + Xu5P / PS::KM101 + Ri5P / PS::KM10)) * (PS_con->S7P + PS::KM103));
        theVars->PS_Vel.v13 = PsV13 * (PS_con->ATP * Ru5P - theVars->ADP * PS_con->RuBP / PS::KE13) / ((PS_con->ATP * (1 + theVars->ADP / PS::KI134) + PS::KM132*(1 + theVars->ADP / PS::KI135)) * (Ru5P + PS::KM131 * (1 + PS_con->PGA / PS::KI131 + PS_con->RuBP / PS::KI132 + theVars->Pi / PS::KI133)));

        const double I2 = theVars->TestLi * theVars->alfa * (1 - theVars->fc) / 2;
        const double J = (I2 + PS::Jmax - sqrt(pow(I2 + PS::Jmax, 2) - 4 * PS::Theta * I2 * PS::Jmax)) / (2 * PS::Theta);
        theVars->PS_Vel.v16 = std::min(PS::beta * J, PsV16 * (theVars->ADP * theVars->Pi - PS_con->ATP / PS::KE16)/(PS::KM161 * PS::KM162 * (1 + theVars->ADP / PS::KM161 + theVars->Pi / PS::KM162 + PS_con->ATP / PS::KM163 + theVars->ADP * theVars->Pi /(PS::KM161 * PS::KM162))));

        theVars->PS_Vel.v23 = PsV23 * G1P * PS_con->ATP /((G1P + PS::KM231) * ((1 + theVars->ADP / PS::KI23) * (PS_con->ATP + PS::KM232) + (PS::KM232 * theVars->Pi /(PS::KA231 * PS_con->PGA + PS::KA232 * F6P + PS::KA233 * PS_con->FBP))));

        theVars->PS_Vel.v23 = PsV23 * G1P * PS_con->ATP / ((G1P + PS::KM231) * ((1 + theVars->ADP / PS::KI23) * (PS_con->ATP + PS::KM232) + (PS::KM232 * theVars->Pi / (PS::KA231 * PS_con->PGA + PS::KA232 * F6P + PS::KA233 * PS_con->FBP))));
        const double N = 1 + (1 + PS::KM313 / PsPEXT) * (theVars->Pi / PS::KM312 + PS_con->PGA / PS::KM32 + GAP / PS::KM33 + DHAP / PS::KM311);

        // The ATP regualtion really is implicit in the light regulation of sucrose synthesis.
        theVars->PS_Vel.v31 = PsV31 * DHAP / (N * PS::KM311)  ;
        theVars->PS_Vel.v32 = PsV32 * PS_con->PGA / (N * PS::KM32);
        theVars->PS_Vel.v33 = PsV33 * GAP / (N * PS::KM33);

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
        const double Pit = PS::PS_C_CP - PS_con->PGA - 2 * PS_con->DPGA - GAP - DHAP - 2 * PS_con->FBP - F6P - PS_con->E4P - 2 * PS_con->SBP - PS_con->S7P - Xu5P - Ri5P - Ru5P - 2 * PS_con->RuBP - G6P - G1P - PS_con->ATP - PS::Param[1];
        theVars->Pi = 0.5 * (-PS::KE25 +  pow((PS::KE25 * PS::KE25 + 4 * Pit * PS::KE25), 0.5));
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

        const double Va = PS::KVmo + PS::V23 * (PS_con->PGA / (PS::KA231 * (1 + PS_con->PGA / PS::KA231)));
        const double v23num = Va * (PS_con->ATP * G1P - PS_con->ADPG * OPOP / PS::KE23);// The reason we set this here is to assume that we can obtain a reverse reaction here. However, a more realistic
        // way to achieve the homeostasis might be to allow starch breakdown and allow regulation of SBPase and FBPase.

        // WY 201803
        const double v23den2 = (1 + theVars->Pi / PS::KI231) * PS::KM231 * PS::KM232 * (1 + PS_con->ATP / PS::KM232 + G1P / PS::KM231 + PS_con->ATP * G1P / (PS::KM231 * PS::KM232) + PS_con->ADPG / PS::KM233 + OPOP / PS::KM234 + PS_con->ADPG * OPOP / (PS::KM233 * PS::KM234));

        const double MaxCoeff = 5 * theVars->PSRatio[101];

        PS::V1Reg = 1 + PS_con->PGA / PS::KI11 + PS_con->FBP / PS::KI12 + PS_con->SBP / PS::KI13 + theVars->Pi / PS::KI14 + NADPH / PS::KI15;

        // Initialize the PrVmax of the different reactions based on the global variables Vmax
        PsV6 = PS::V6;            // 6 FBPase FBP<->F6P+OP
        PsV9 = PS::V9;            // 9 SBPase SBP<->S7P+OP
        PsV13 = PS::V13;          // 13 Ribulosebiphosphate kinase Ru5P+ATP<->RuBP+ADP
        PsV16 = PS::V16;          // 16 ATP synthase ADP+Pi<->ATP
        const double PsV31 = PS::V31 * RegFactor; // 31 Phosphate translocator DHAPi<->DHAPo
        const double PsV32 = PS::V32 * RegFactor; // 32 Phosphate translocator PGAi<->PGAo
        const double PsV33 = PS::V33 * RegFactor; // 33 Phosphate translocator GAPi<->GAPo

        // First here is one way of the redox regulation, assuming the regulation is instataneous.
        // in case that there are more work using the equilibrium of Thio with enzyme
        // as a way to regulate enzyme activities.
        if (theVars->RedoxReg_RA_com) {
            PsV6 = RedoxRegCondition::getV6();
            PsV9 = RedoxRegCondition::getV9();
            PsV13 = RedoxRegCondition::getV13();
            PsV16 = RedoxRegCondition::getV16();
        }

        if (theVars->RUBISCOMETHOD == 2) {
            const double tmp = V1 * PS_con->RuBP / (PS_con->RuBP + PS::KM13 * PS::V1Reg);
            theVars->PS_Vel.v1 = tmp * theVars->CO2_cond / (theVars->CO2_cond + PS::KM11 * (1 + theVars->O2_cond / PS::KM12));

            if (PS_con->RuBP < PS::V1 / 2.5)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PS::V1 / 2.5);


        } else if (theVars->RUBISCOMETHOD == 1){
            //fprintf("M1  ");
            theVars->PS_Vel.v1 = PS::V1 * theVars->CO2_cond / (theVars->CO2_cond + PS::KM11 * (1 + theVars->O2_cond / PS::KM12));
            if (PS_con->RuBP < PS::V1 / 2.5)
                theVars->PS_Vel.v1 = theVars->PS_Vel.v1 * PS_con->RuBP / (PS::V1 / 2.0);// DNF was 2.5 not 2.0
        }

        theVars->PS_Vel.v2 = PS::V2 * PS_con->PGA * PS_con->ATP / ((PS_con->PGA + PS::KM21) * (PS_con->ATP + PS::KM22 * (1 + theVars->ADP / PS::KM23)));
        theVars->PS_Vel.v3 = PS::V3 * PS_con->DPGA * NADPH / ((PS_con->DPGA + PS::KM31a) * (NADPH + PS::KM32b));
        theVars->PS_Vel.v4 = 0;
        theVars->PS_Vel.v5 = PS::V5 * (GAP * DHAP - PS_con->FBP / PS::KE5) / ((PS::KM51 * PS::KM52) * (1 + GAP / PS::KM51 + DHAP / PS::KM52 + PS_con->FBP / PS::KM53 + GAP * DHAP / (PS::KM51 * PS::KM52)));
        theVars->PS_Vel.v6 = PsV6 * (PS_con->FBP - F6P * theVars->Pi / PS::KE6) / (PS_con->FBP + PS::KM61 * (1 + F6P / PS::KI61 + theVars->Pi / PS::KI62));

        theVars->PS_Vel.v7 = PS::V7 * (F6P * GAP * KE57 - PS_con->E4P * Xu5P) / (Km8p5p * Km5p5p * Den);
        theVars->PS_Vel.v8 = PS::V8 * (DHAP * PS_con->E4P - PS_con->SBP / PS::KE8) / ((PS_con->E4P + PS::KM82) * (DHAP + PS::KM81));
        theVars->PS_Vel.v9 = PsV9 * (PS_con->SBP - theVars->Pi * PS_con->S7P / PS::KE9) / (PS_con->SBP + PS::KM9 * (1 + theVars->Pi / PS::KI9));
        theVars->PS_Vel.v10 = PS::V7 * (PS_con->S7P * GAP * KE810 - Xu5P * Ri5P) / (Km8p5p * Km5p5p * Den);

        theVars->PS_Vel.v13 = PsV13 * (PS_con->ATP * Ru5P - theVars->ADP * PS_con->RuBP / PS::KE13) / ((PS_con->ATP * (1 + theVars->ADP / PS::KI134) + PS::KM132 * (1 + theVars->ADP / PS::KI135)) * (Ru5P + PS::KM131 * (1 + PS_con->PGA / PS::KI131 + PS_con->RuBP / PS::KI132 + theVars->Pi / PS::KI133)));
        theVars->PS_Vel.v16 = PsV16 * (theVars->ADP * theVars->Pi - PS_con->ATP / PS::KE16) / (PS::KM161 * PS::KM162 * (1 + theVars->ADP / PS::KM161 + theVars->Pi / PS::KM162 + PS_con->ATP / PS::KM163 + theVars->ADP * theVars->Pi / (PS::KM161 * PS::KM162)));
        theVars->PS_Vel.v23 = v23num / v23den2;
        theVars->PS_Vel.v31 = (PsV31 * DHAP / (DHAP + PS::KM311) * PsPEXT / (PsPEXT + PS::KM313)) * ATPreg;
        theVars->PS_Vel.v32 = (PsV32 * PS_con->PGA / (PS_con->PGA + PS::KM32) * PsPEXT / (PsPEXT + PS::KM313)) * ATPreg;
        theVars->PS_Vel.v33 = (PsV33 * GAP / (GAP + PS::KM33) * PsPEXT / (PsPEXT + PS::KM313)) * ATPreg;
        theVars->PS_Vel.v24 = (PS::V24 * (PS_con->ADPG)) / (PS::KM241 * (1 + PS_con->ADPG / PS::KM241));
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
