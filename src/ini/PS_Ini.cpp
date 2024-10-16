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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double PS::TIME = 0.;
std::size_t PS::N = 1;
const std::size_t PSCondition::count = 12;
bool PSCondition::C3 = false;
bool PS::C3 = false;

arr PS::Param = {0., 0.};

DEFINE_VALUE_SET_STATIC(PS);
DEFINE_VALUE_SET(PSCondition);


void PS::_initCalc(Variables *theVars, PSCondition* PS_con) {
    if (theVars->useC3) {
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0) {
            PS::Vfactor1 = theVars->VfactorCp[0];
            PS::Vfactor2 = theVars->VfactorCp[2];
            PS::Vfactor3 = theVars->VfactorCp[3];
            PS::Vfactor5 = theVars->VfactorCp[5];
            PS::Vfactor7 = theVars->VfactorCp[7];
            PS::Vfactor13 = theVars->VfactorCp[12];
            PS::Vfactor23 = theVars->VfactorCp[16];
        }
        if (theVars->GRNT == 1 && theVars->Tp > 25) {
            PS::Vf_T3 = theVars->VfactorT[9];
            PS::Vf_T2 = theVars->VfactorT[21];
            PS::Vf_T1 = theVars->VfactorT[24];
            PS::Vf_T6 = theVars->VfactorT[16];
            PS::Vf_T9 = theVars->VfactorT[25];
            PS::Vf_T5 = theVars->VfactorT[1];
            PS::Vf_T23 = theVars->VfactorT[2];
            PS::Vf_T13 = theVars->VfactorT[3];
        }
        PS::KM11 = 0.0097 * exp(PS::c_c - PS::dHa_c * 1000. / (PS::R * (theVars->Tp + 273.15))) / 272.38;
        PS::KM12 = 0.244 * exp(PS::c_o - PS::dHa_o * 1000. / (PS::R * (theVars->Tp + 273.15))) / 165.82;
	
        if (theVars->GP == 0) {
            PS::V1 = theVars->EnzymeAct.at("V1");
            PS::V2 = theVars->EnzymeAct.at("V2");
            PS::V3 = theVars->EnzymeAct.at("V3");
            PS::V5 = theVars->EnzymeAct.at("V5");
            PS::V6 = theVars->EnzymeAct.at("V6");
            PS::V7 = theVars->EnzymeAct.at("V7");
            PS::V8 = theVars->EnzymeAct.at("V8");
            PS::V9 = theVars->EnzymeAct.at("V9");
            PS::V10 = theVars->EnzymeAct.at("V10");
            PS::V13 = theVars->EnzymeAct.at("V13");
            PS::V23 = theVars->EnzymeAct.at("V23");
            PS::V16 = theVars->EnzymeAct.at("V16");
        }
        PS::PsV1_0 = PS::V1 * PS::Vfactor1 * PS::Vf_T1 ;   //  1   Rubisco RuBP+CO2<->2PGA
        PS::PsV2_0 = PS::V2 * PS::Vfactor2 * PS::Vf_T2 ;   //  2   PGA Kinase  PGA+ATP <-> ADP + DPGA
        PS::PsV3_0 = PS::V3 * PS::Vfactor3 * PS::Vf_T3 ;   //  3   GAP dehydragenase   DPGA+NADPH <->GAP + OP+NADP

        PS::PsV5_0 = PS::V5 * PS::Vfactor5 * PS::Vf_T5;    //  5   Aldolase    GAP+DHAP <->FBP
        PS::PsV6_0 = PS::V6 * PS::Vf_T6;                   //  6   FBPase  FBP<->F6P+OP
        PS::PsV7_0 = PS::V7 * PS::Vfactor7;                //  7   Transketolase   F6P+GAP<->E4P+Xu5P
        PS::PsV8_0 = PS::V8 * PS::Vfactor5 * PS::Vf_T5;    //  8   Aldolase    E4P+DHAP<->SBP
        PS::PsV9_0 = PS::V9 * PS::Vf_T9;                   //  9   SBPase  SBP<->S7P+OP
        PS::PsV10_0 = PS::V10 * PS::Vfactor7;              //  10  Transketolase   S7P+GAP<->Ri5P+Xu5P

        PS::PsV13_0 = PS::V13 * PS::Vfactor13 * PS::Vf_T13; //  13  Ribulosebiphosphate kinase  Ru5P+ATP<->RuBP+ADP
        PS::PsV16 = PS::V16  ;                             //  16  ATP synthase    ADP+Pi<->ATP

        PS::PsV23_0 = PS::V23 * PS::Vfactor23 * PS::Vf_T23;//  23  ADP-glucose pyrophosphorylase and   ADPG+Gn<->G(n+1)+ADP
        PS::PsV31  =   PS::V31  * theVars->alpha2;         //  31  Phosphate translocator  DHAPi<->DHAPo
        PS::PsV32  =   PS::V32  * theVars->alpha2;         //  32  Phosphate translocator  PGAi<->PGAo
        PS::PsV33  =   PS::V33  * theVars->alpha2;         //  33  Phosphate translocator  GAPi<->GAPo
        PS::Ru_Act = -3. * pow(10., -5.) * pow(theVars->Tp, 3.) + 0.0013 * pow(theVars->Tp, 2.) - 0.0106 * theVars->Tp + 0.8839; //Rubisco activition state   % SHARED
        PS::PsV1 = PS::PsV1_0 * PS::Ru_Act * pow(PS::Q10_1, (theVars->Tp - 25.) / 10.);                     //   SHARED
        PS::PsV2 = PS::PsV2_0 * pow(PS::Q10_2, (theVars->Tp - 25.) / 10.);
        PS::PsV3 = PS::PsV3_0 * pow(PS::Q10_3, (theVars->Tp - 25.) / 10.);
        PS::PsV5 = PS::PsV5_0 * pow(PS::Q10_5, (theVars->Tp - 25.) / 10.);
        PS::PsV6 = PS::PsV6_0 * pow(PS::Q10_6, (theVars->Tp - 25.) / 10.);
        PS::PsV7 = PS::PsV7_0 * pow(PS::Q10_7, (theVars->Tp - 25.) / 10.);
        PS::PsV8 = PS::PsV8_0 * pow(PS::Q10_8, (theVars->Tp - 25.) / 10.);
        PS::PsV9 = PS::PsV9_0 * pow(PS::Q10_9, (theVars->Tp - 25.) / 10.);
        PS::PsV10 = PS::PsV10_0 * pow(PS::Q10_10, (theVars->Tp - 25.) / 10.);
        PS::PsV13= PS::PsV13_0 * pow(PS::Q10_13, (theVars->Tp - 25.) / 10.);
        PS::PsV23 = PS::PsV23_0 * pow(PS::Q10_23, (theVars->Tp - 25.) / 10.);
        PS::I2 = theVars->TestLi * theVars->alfa * (1. - theVars->fc) / 2.;
        PS::J = (I2 + PS::Jmax - sqrt(pow(I2 + PS::Jmax, 2.) - 4. * PS::Theta * I2 * PS::Jmax)) / (2. * PS::Theta);
    } else {
        PS::PS_C_CP *= theVars->PSRatio[0];  // Global constant for the total phosphate
        PS::PS_C_CA *= theVars->PSRatio[1];  // Global constant for the total adenylates
        // langmm: Present in Matlab version
        // PS::PS_C_CN *= theVars->PSRatio[2];  // Global constant for the cytosolic Phosphate concentration;
        PS::PS_PEXT *= theVars->PSRatio[3];  // Global constant for the cytosolic Phosphate concentration;

        // Initialize the constants for the different reactions
        PS::KM11 *= theVars->PSRatio[19]; // CO2 1 RuBP+CO2->2PGA
        PS::KM12 *= theVars->PSRatio[20];  // O2 1 RuBP+CO2->2PGA
        PS::KM13 *= theVars->PSRatio[21];   // RuBP 1 RuBP+CO2->2PGA

        PS::KI11 *= theVars->PSRatio[22];   // PGA
        PS::KI12 *= theVars->PSRatio[23];   // FBP
        PS::KI13 *= theVars->PSRatio[24];  // SBP
        PS::KI14 *= theVars->PSRatio[25];    // Pi
        PS::KI15 *= theVars->PSRatio[26];   // NADPH

        PS::KM21 *= theVars->PSRatio[27];  // PGA 2 PGA+ATP <-> ADP + DPGA
        PS::KM22 *= theVars->PSRatio[28];  // ATP 2 PGA+ATP <-> ADP + DPGA
        PS::KM23 *= theVars->PSRatio[29];   // ADP

        PS::KM31a *= theVars->PSRatio[30]; // BPGA 3 DPGA+NADPH <->GAP + OP+NADP
        PS::KM32b *= theVars->PSRatio[31];   // NADPH 3 DPGA+NADPH <->GAP + OP+NADP

        PS::KE4 *= theVars->PSRatio[34]; // Using the value from Patterson

        PS::KM51 *= theVars->PSRatio[35];    // GAP 5 GAP+DHAP <->FBP
        PS::KM52 *= theVars->PSRatio[36];    // DHAP 5 GAP+DHAP <->FBP
        PS::KM53 *= theVars->PSRatio[37];   // FBP 5 GAP+DHAP <->FBP     // Original Value: 0.02
        PS::KE5 *= theVars->PSRatio[38];   // Defult: 7.1

        PS::KM61 *= theVars->PSRatio[39];  // FBP 6 FBP<->F6P+OP
        PS::KI61 *= theVars->PSRatio[40];    // F6P
        PS::KI62 *= theVars->PSRatio[41];    // Pi
        PS::KE6 *= theVars->PSRatio[42];

        PS::KE7 *= theVars->PSRatio[47];     // The equilibrium constant for this reaction  New   Laisk  Bassham and Krause 1969 BBA

        PS::KM81 *= theVars->PSRatio[49];    // DHAP
        PS::KM82 *= theVars->PSRatio[50];    // E4P estimate
        PS::KE8 *= theVars->PSRatio[51];   // The equilibrium constant for this reaction  New  mM-1  Laisk  Bassham and Krause 1969 BBA. Default: 1.107

        PS::KM9 *= theVars->PSRatio[52];   // SBP 9 SBP<->S7P+OP
        PS::KI9 *= theVars->PSRatio[53];    // The inibintion constant for Pi;
        PS::KE9 *= theVars->PSRatio[54];

        PS::KM10 *= theVars->PSRatio[55];    // R5P 10 S7P+GAP<->Ri5P+Xu5P
        PS::KM101 *= theVars->PSRatio[56];   // Xu5P
        PS::KM102 *= theVars->PSRatio[57];  // Estimate for GAP
        PS::KM103 *= theVars->PSRatio[58]; // Estimate for S7P
        //theVars->KE10 *= theVars->PSRatio[59];//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA

        PS::KE11 *= theVars->PSRatio[60];    // Equilibrium Constant 11 Ri5P<-->Ru5P
        PS::KE12 *= theVars->PSRatio[61];   // Equilibrium Constant 12 Xu5P<-->Ru5P

        PS::KM131 *= theVars->PSRatio[62];  // Ru5P 13 Ru5P+ATP<->RuBP+ADP
        PS::KM132 *= theVars->PSRatio[63]; //  ATP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI131 *= theVars->PSRatio[64];    // PGA 13 Ru5P+ATP<->RuBP+ADP
        PS::KI132 *= theVars->PSRatio[65];   // RuBP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI133 *= theVars->PSRatio[66];    // Pi 13 Ru5P+ATP<->RuBP+ADP
        PS::KI134 *= theVars->PSRatio[67];   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI135 *= theVars->PSRatio[68];   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KE13 *= theVars->PSRatio[69];

        PS::KM161 *= theVars->PSRatio[70]; // ADP 16 ADP+Pi<->ATP
        PS::KM162 *= theVars->PSRatio[71];   // Pi 16 ADP+Pi<-> ATP
        PS::KM163 *= theVars->PSRatio[72];   // ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
        PS::KE16 *= theVars->PSRatio[73];  // The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA

        PS::KE21 *= theVars->PSRatio[74];    // Equilibrium constant 21 F6P<->G6P
        PS::KE22 *= theVars->PSRatio[75];  //  Equilibrium constant 22 G6P<->G1P

        PS::KM311 *= theVars->PSRatio[76]; // DHAP 31 DHAPi<->DHAPo
        //theVars->KM312 *= theVars->PSRatio[77];//  Pi 31 DHAPi<->DHAPo
        PS::KM313 *= theVars->PSRatio[78];  // Pext 31 DHAPi<->DHAPo
        PS::KM32 *= theVars->PSRatio[79];   // PGA 32 PGAi<->PGAo
        PS::KM33 *= theVars->PSRatio[80];  // GAP 33 GAPi<->GAPo


        // Now put in the constant for the new ADPG Pyrophosphorylase and starch
        // synthase

        // ATP + Glucose-1-Phosphate --> ADPG + PPi

        PS::KM231 *= theVars->PSRatio[81]; // G1P 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM232 *= theVars->PSRatio[82]; //  ATP 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM233 *= theVars->PSRatio[83];  // ADPG 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM234 *= theVars->PSRatio[84];   //  PPi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KE23 *= theVars->PSRatio[85];


        PS::KA231 *= theVars->PSRatio[86]; // PGA 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KI231 *= theVars->PSRatio[87];  //0.9 ;       //   Pi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989 WY201803
        PS::KVmo *= theVars->PSRatio[88]; //   The minimum maximum velocity        Laisk et al 1989
        // ADPG --> ADP + Gn     // The starch synthesis reaction 24.     Laisk et al
        // 1989

        PS::KM241 *= theVars->PSRatio[89]; //   ADPG    ADPG --> ADP + Gn       Laisk et al 1989

        PS::KE25 *= theVars->PSRatio[92];

        // Initialize the Vmax for different reactions

        if (theVars->GP == 0) {
            PS::V1 = 2.93 * PS::SC1 / PS::STOM1 * theVars->PSRatio[4]; // (Harris & Koniger, 1997)
            PS::V2 = 30.15 * PS::SC * PS::STOM2 * theVars->PSRatio[5]; // (Harris & Koniger, 1997)
            PS::V3 = 4.04 * PS::SC * PS::STOM2 * theVars->PSRatio[6];  // 1.57*PS::SC     ; // (Harris & Koniger, 1997)
            PS::V5 = 1.22 * PS::SC * theVars->PSRatio[7];          // (Harris & Koniger, 1997)
            PS::V6 = 0.734 * PS::SC / PS::STOM1 * theVars->PSRatio[8]; // (Harris & Koniger, 1997)
            PS::V7 = 3.12 * PS::SC * 4. * theVars->PSRatio[9];     // (Harris & Koniger, 1997)
            PS::V8 = 1.22 * PS::SC * theVars->PSRatio[10];         // (Harris & Koniger, 1997)
            PS::V9 = 0.32 * 3. * theVars->PSRatio[11];         // 0.17*PS::SC *FC ; // (Harris & Koniger, 1997) *3.
            //theVars->V10 = theVars->V7; // (Harris & Koniger, 1997)
            PS::V13 = 10.81 * PS::SC1 * theVars->PSRatio[12];      // (Harris & Koniger, 1997)
            PS::V16 = 5.47 * theVars->PSRatio[13];             // (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
            PS::V23 = 2. * theVars->PSRatio[14];
        }
        PS::V24 *= theVars->PSRatio[15];
        PS::V31 *= theVars->PSRatio[16];
        PS::V32 *= theVars->PSRatio[17];
        PS::V33 *= theVars->PSRatio[18];//WY 2018103
        PS::KE57 *= theVars->PSRatio[93];
        PS::Km8p5p *= theVars->PSRatio[94];
        PS::Km5p5p *= theVars->PSRatio[95];
        PS::KE810 *= theVars->PSRatio[96];
        PS::Km5gap *= theVars->PSRatio[97];
        PS::Km8f6p *= theVars->PSRatio[98];
        PS::Km8s7p *= theVars->PSRatio[99];
        PS::Km8gap *= theVars->PSRatio[100];
        PS::MaxCoeff *= theVars->PSRatio[101];
        PS::PsV31 = PS::V31 * PS::RegFactor; // 31 Phosphate translocator DHAPi<->DHAPo
        PS::PsV32 = PS::V32 * PS::RegFactor; // 32 Phosphate translocator PGAi<->PGAo
        PS::PsV33 = PS::V33 * PS::RegFactor; // 33 Phosphate translocator GAPi<->GAPo
    }
    theVars->ADP = PS::PS_C_CA - PS_con->ATP;
    PS::KE2Ratio = (1. + 1. / PS::KE21 + PS::KE22);
    PS::KE1Ratio = (1. + 1. / PS::KE11 + 1. / PS::KE12);
}


PSCondition* PS::_init(Variables *theVars) {
    setC3(theVars->useC3);
    PSCondition* PS_con = new PSCondition();

    PS::R = 8.314;
    PS::c_c = 38.28;
    PS::dHa_c = 80.99;
    PS::c_o = 14.68;
    PS::dHa_o = 23.72;
    PS::RegFactor = 1.;
    
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
        PS::PS_C_CP = 22.;   //   Global constant for the total phosphate
        PS::PS_C_CA = 1.5;   //   Global constant for the total adenylates
        PS::PS_C_CN = 1.;    //   Global constant for the cytosolic Phosphate concentration;
        if (theVars->GRNC == 1 && theVars->CO2_cond > 0) {
            PS::Vfactor1 = theVars->VfactorCp[0];
            PS::Vfactor2 = theVars->VfactorCp[2];
            PS::Vfactor3 = theVars->VfactorCp[3];
            PS::Vfactor5 = theVars->VfactorCp[5];
            PS::Vfactor7 = theVars->VfactorCp[7];
            PS::Vfactor13 = theVars->VfactorCp[12];
            PS::Vfactor23 = theVars->VfactorCp[16];
        }
        if (theVars->GRNT == 1 && theVars->Tp > 25) {
            PS::Vf_T3 = theVars->VfactorT[9];
            PS::Vf_T2 = theVars->VfactorT[21];
            PS::Vf_T1 = theVars->VfactorT[24];
            PS::Vf_T6 = theVars->VfactorT[16];
            PS::Vf_T9 = theVars->VfactorT[25];
            PS::Vf_T5 = theVars->VfactorT[1];
            PS::Vf_T23 = theVars->VfactorT[2];
            PS::Vf_T13 = theVars->VfactorT[3];
        }

        //PsKM11_0 = ;
        //PsKM12_0 = ;  // O2 1 RuBP+CO2->2PGA  0.28 DEFAUL.
        PS::KM11 = 0.0097 * exp(PS::c_c - PS::dHa_c * 1000. / (PS::R * (theVars->Tp + 273.15))) / 272.38;
        PS::KM12 = 0.244 * exp(PS::c_o - PS::dHa_o * 1000. / (PS::R * (theVars->Tp + 273.15))) / 165.82;

        PS::KM13 = 0.02;   //  RuBP 1 RuBP+CO2->2PGA
        PS::KI11 = 0.84;   // PGA
        PS::KI12 = 0.04;   // FBP
        PS::KI13 = 0.075;  // SBP
        PS::KI14 = 0.9;    // Pi
        PS::KI15 = 0.07;   // NADPH
        PS::KM21 = 0.240;  // PGA 2 PGA+ATP <-> ADP + DPGA
        PS::KM22 = 0.390;  //  ATP 2 PGA+ATP <-> ADP + DPGA
        PS::KM23 = 0.23;   //  ADP
        PS::KM31a = 0.004; // BPGA 3 DPGA+NADPH <->GAP + OP+NADP
        PS::KM32b = 0.1;   //  NADPH 3 DPGA+NADPH <->GAP + OP+NADP
        //KM41 = 2.5 ;     // DHAP 4 DHAP <->GAP
        //KM42 = 0.68;     //  GAP 4 DHAP <->GAP
        PS::KE4 = 0.05;    //   Using the value from Patterson
        PS::KM51 = 0.3;    // GAP 5 GAP+DHAP <->FBP
        PS::KM52 = 0.4;    //  DHAP 5 GAP+DHAP <->FBP
        PS::KM53 = 0.02;   // FBP 5 GAP+DHAP <->FBP     // Original Value: 0.02
        PS::KE5 = 7.100;   // Defult: 7.1
        PS::KM61 = 0.033;  //  FBP 6 FBP<->F6P+OP
        PS::KI61 = 0.7;    //   F6P
        PS::KI62 = 12.;    //   Pi
        PS::KE6 = 6.66 * pow(10., 5.);    // The equilibrium constant for this reaction        // New    mM     Laisk or Bassham and Krause 1969 BBA
        PS::KM71 = 0.100;  // Xu5P 7 F6P+GAP<->E4P+Xu5P      // jn
        PS::KM72 = 0.100;  //  E4P 7 F6P+GAP<->E4P+Xu5P
        PS::KM73 = 0.1;    //   F6P This value was based on estimate
        PS::KM74 = 0.1000; // Estimate for GAP ORIGINAL 0.1
        PS::KE7 = 10.;     // The equilibrium constant for this reaction             // New           Laisk  Bassham and Krause 1969 BBA
        //PsKM8 = 0.02;  // SBP 8 E4P+DHAP<->SBP
        PS::KM81 = 0.4;    // DHAP
        PS::KM82 = 0.2;    // E4P estimate
        PS::KE8 = 1.017;   // The equilibrium constant for this reaction                  // New    mM-1         Laisk  Bassham and Krause 1969 BBA. Default: 1.107
        PS::KM9 = 0.05;    //  SBP 9 SBP<->S7P+OP
        PS::KI9 = 12.;     //   The inibintion constant for Pi;
        PS::KE9 = 6.66 * pow(10., 5.) ; // The equilibrium constant of this reaction           // New   mM      Laisk  Bassham and Krause 1969 BBA
        PS::KM10 = 1.5;    // R5P 10 S7P+GAP<->Ri5P+Xu5P
        PS::KM101 = 0.1;   //   Xu5P
        PS::KM102 = 0.072; //   Estimate for GAP
        PS::KM103 = 0.46;  //   Estimate for S7P                                    // New
        PS::KE10 = 1. / 0.85;      //   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA
        PS::KE11 = 0.4;    // Equilibrium Constant 11 Ri5P<-->Ru5P
        PS::KE12 = 0.67;   //  Equilibrium Constant 12 Xu5P<-->Ru5P

        PS::KM131 = 0.05;  // Ru5P 13 Ru5P+ATP<->RuBP+ADP
        PS::KM132 = 0.059; //  ATP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI131 = 2.;    // PGA 13 Ru5P+ATP<->RuBP+ADP
        PS::KI132 = 0.7 ;  // RuBP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI133 = 4.;    // Pi 13 Ru5P+ATP<->RuBP+ADP
        PS::KI134 = 2.5;   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI135 = 0.4;   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KE13 = 6.846 * pow(10., 3.);   //   The equilibrium constant for this reaction  // New From Laisk or Bassham and Krause 1969 BBA
        PS::KM161 = 0.014; // ADP 16 ADP+Pi<->ATP
        PS::KM162 = 0.3;   // Pi 16 ADP+Pi<-> ATP
        PS::KM163 = 0.3;   // ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
        PS::KE16 = 5.734;  // The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA
        PS::KE21 = 2.3;
        PS::KE22 = 0.058;
        //PsKE21 = 2.3;    // Equilibrium constant 21 F6P<->G6P
        //PsKE22 = 0.058;  // Equilibrium constant 22 G6P<->G1P
        PS::KM231 = 0.08;  // G1P 23 G1P+ATP+Gn<->PPi+ADP+Gn+1
        PS::KM232 = 0.08;  // ATP 23 G1P+ATP+Gn<->PPi+ADP+Gn+1
        PS::KA231 = 0.1;   // PGA 23 G1P+ATP+Gn<->PPi+ADP+Gn+1
        PS::KA232 = 0.02;  // F6P 23 G1P+ATP+Gn<->PPi+ADP+Gn+1
        PS::KA233 = 0.02;  // FBP 23 G1P+ATP+Gn<->PPi+ADP+Gn+1
        PS::KI23 = 10.;    // ADP 23 G1P+ATP+Gn<->PPi+ADP+Gn+1

        PS::KM311 = 0.077; // DHAP 31 DHAPi<->DHAPo
        PS::KM312 = 0.63;  // Pi 31 DHAPi<->DHAPo
        PS::KM313 = 0.74;  // Pext 31 DHAPi<->DHAPo
        PS::KM32 = 0.25;   // PGA 32 PGAi<->PGAo
        PS::KM33 = 0.075;  // GAP 33 GAPi<->GAPo

        // Initialize the values of the global variables

        if (theVars->GP == 0) {
            PS::V1 = theVars->EnzymeAct.at("V1");
            PS::V2 = theVars->EnzymeAct.at("V2");
            PS::V3 = theVars->EnzymeAct.at("V3");
            PS::V5 = theVars->EnzymeAct.at("V5");
            PS::V6 = theVars->EnzymeAct.at("V6");
            PS::V7 = theVars->EnzymeAct.at("V7");
            PS::V8 = theVars->EnzymeAct.at("V8");
            PS::V9 = theVars->EnzymeAct.at("V9");
            PS::V10 = theVars->EnzymeAct.at("V10");
            PS::V13 = theVars->EnzymeAct.at("V13");
            PS::V23 = theVars->EnzymeAct.at("V23");
            PS::V16 = theVars->EnzymeAct.at("V16");
        }

        PS::V31 = 3.73 / 3.;   // 1.05 *PS::SC  *1.0 ; % (Lilley, Chon, Mosbach & Heldt, 1977b) 31 Phosphate translocator DHAPi<->DHAPo   1.05 defulat
        PS::V32 = 3.73 / 3.;   //1.05 *PS::SC *1.0;     % (Lilley et al., 1977b) 32 Phosphate translocator PGAi<->PGAo 1.05 default
        PS::V33 = 3.73 / 3.;   //1.05 *PS::SC * 1.0;     % (Lilley et al., 1977b) 33 Phosphate translocator GAPi<->GAPo 1.05 default

        PS::PsV1_0 = PS::V1 * PS::Vfactor1 * PS::Vf_T1 ;   //  1   Rubisco RuBP+CO2<->2PGA
        PS::PsV2_0 = PS::V2 * PS::Vfactor2 * PS::Vf_T2 ;   //  2   PGA Kinase  PGA+ATP <-> ADP + DPGA
        PS::PsV3_0 = PS::V3 * PS::Vfactor3 * PS::Vf_T3 ;   //  3   GAP dehydragenase   DPGA+NADPH <->GAP + OP+NADP

        PS::PsV5_0 = PS::V5 * PS::Vfactor5 * PS::Vf_T5;    //  5   Aldolase    GAP+DHAP <->FBP
        PS::PsV6_0 = PS::V6 * PS::Vf_T6;                   //  6   FBPase  FBP<->F6P+OP
        PS::PsV7_0 = PS::V7 * PS::Vfactor7;                //  7   Transketolase   F6P+GAP<->E4P+Xu5P
        PS::PsV8_0 = PS::V8 * PS::Vfactor5 * PS::Vf_T5;    //  8   Aldolase    E4P+DHAP<->SBP
        PS::PsV9_0 = PS::V9 * PS::Vf_T9;                   //  9   SBPase  SBP<->S7P+OP
        PS::PsV10_0 = PS::V10 * PS::Vfactor7;              //  10  Transketolase   S7P+GAP<->Ri5P+Xu5P

        PS::PsV13_0 = PS::V13 * PS::Vfactor13 * PS::Vf_T13; //  13  Ribulosebiphosphate kinase  Ru5P+ATP<->RuBP+ADP
        PS::PsV16 = PS::V16  ;                             //  16  ATP synthase    ADP+Pi<->ATP

        PS::PsV23_0 = PS::V23 * PS::Vfactor23 * PS::Vf_T23;//  23  ADP-glucose pyrophosphorylase and   ADPG+Gn<->G(n+1)+ADP
        PS::PsV31  =   PS::V31  * theVars->alpha2;         //  31  Phosphate translocator  DHAPi<->DHAPo
        PS::PsV32  =   PS::V32  * theVars->alpha2;         //  32  Phosphate translocator  PGAi<->PGAo
        PS::PsV33  =   PS::V33  * theVars->alpha2;         //  33  Phosphate translocator  GAPi<->GAPo
        PS::Ru_Act = -3. * pow(10., -5.) * pow(theVars->Tp, 3.) + 0.0013 * pow(theVars->Tp, 2.) - 0.0106 * theVars->Tp + 0.8839; //Rubisco activition state   % SHARED
        PS::PsV1 = PS::PsV1_0 * PS::Ru_Act * pow(PS::Q10_1, (theVars->Tp - 25.) / 10.);                     //   SHARED
        PS::PsV2 = PS::PsV2_0 * pow(PS::Q10_2, (theVars->Tp - 25.) / 10.);
        PS::PsV3 = PS::PsV3_0 * pow(PS::Q10_3, (theVars->Tp - 25.) / 10.);
        PS::PsV5 = PS::PsV5_0 * pow(PS::Q10_5, (theVars->Tp - 25.) / 10.);
        PS::PsV6 = PS::PsV6_0 * pow(PS::Q10_6, (theVars->Tp - 25.) / 10.);
        PS::PsV7 = PS::PsV7_0 * pow(PS::Q10_7, (theVars->Tp - 25.) / 10.);
        PS::PsV8 = PS::PsV8_0 * pow(PS::Q10_8, (theVars->Tp - 25.) / 10.);
        PS::PsV9 = PS::PsV9_0 * pow(PS::Q10_9, (theVars->Tp - 25.) / 10.);
        PS::PsV10 = PS::PsV10_0 * pow(PS::Q10_10, (theVars->Tp - 25.) / 10.);
        PS::PsV13= PS::PsV13_0 * pow(PS::Q10_13, (theVars->Tp - 25.) / 10.);
        PS::PsV23 = PS::PsV23_0 * pow(PS::Q10_23, (theVars->Tp - 25.) / 10.);
        PS::I2 = theVars->TestLi * theVars->alfa * (1. - theVars->fc) / 2.;
        PS::J = (I2 + PS::Jmax - sqrt(pow(I2 + PS::Jmax, 2.) - 4. * PS::Theta * I2 * PS::Jmax)) / (2. * PS::Theta);

    } else {
        PS::PS_C_CP = 15. * theVars->PSRatio[0];  // Global constant for the total phosphate
        PS::PS_C_CA = 1.5 * theVars->PSRatio[1];  // Global constant for the total adenylates
        // langmm: Present in Matlab version
        // PS::PS_C_CN = 1.0 * theVars->PSRatio[2];  // Global constant for the cytosolic Phosphate concentration;
        PS::PS_PEXT = 0.5 * theVars->PSRatio[3];  // Global constant for the cytosolic Phosphate concentration;

        // Initialize the constants for the different reactions
        PS::KM11 = 0.0115 * theVars->PSRatio[19]; // CO2 1 RuBP+CO2->2PGA
        PS::KM12 = 0.222 * theVars->PSRatio[20];  // O2 1 RuBP+CO2->2PGA
        PS::KM13 = 0.02 * theVars->PSRatio[21];   // RuBP 1 RuBP+CO2->2PGA

        PS::KI11 = 0.84 * theVars->PSRatio[22];   // PGA
        PS::KI12 = 0.04 * theVars->PSRatio[23];   // FBP
        PS::KI13 = 0.075 * theVars->PSRatio[24];  // SBP
        PS::KI14 = 0.9 * theVars->PSRatio[25];    // Pi
        PS::KI15 = 0.07 * theVars->PSRatio[26];   // NADPH

        PS::KM21 = 0.240 * theVars->PSRatio[27];  // PGA 2 PGA+ATP <-> ADP + DPGA
        PS::KM22 = 0.390 * theVars->PSRatio[28];  // ATP 2 PGA+ATP <-> ADP + DPGA
        PS::KM23 = 0.23 * theVars->PSRatio[29];   // ADP

        PS::KM31a = 0.004 * theVars->PSRatio[30]; // BPGA 3 DPGA+NADPH <->GAP + OP+NADP
        PS::KM32b = 0.1 * theVars->PSRatio[31];   // NADPH 3 DPGA+NADPH <->GAP + OP+NADP

        PS::KE4 = 1. / 0.05 * theVars->PSRatio[34]; // Using the value from Patterson

        PS::KM51 = 0.3 * theVars->PSRatio[35];    // GAP 5 GAP+DHAP <->FBP
        PS::KM52 = 0.4 * theVars->PSRatio[36];    // DHAP 5 GAP+DHAP <->FBP
        PS::KM53 = 0.02 * theVars->PSRatio[37];   // FBP 5 GAP+DHAP <->FBP     // Original Value: 0.02
        PS::KE5 = 7.100 * theVars->PSRatio[38];   // Defult: 7.1

        PS::KM61 = 0.033 * theVars->PSRatio[39];  // FBP 6 FBP<->F6P+OP
        PS::KI61 = 0.7 * theVars->PSRatio[40];    // F6P
        PS::KI62 = 12. * theVars->PSRatio[41];    // Pi
        PS::KE6 = 6.66 * pow(10, 5) * theVars->PSRatio[42];

        PS::KE7 = 0.1 * theVars->PSRatio[47];     // The equilibrium constant for this reaction  New   Laisk  Bassham and Krause 1969 BBA

        PS::KM81 = 0.4 * theVars->PSRatio[49];    // DHAP
        PS::KM82 = 0.2 * theVars->PSRatio[50];    // E4P estimate
        PS::KE8 = 1.017 * theVars->PSRatio[51];   // The equilibrium constant for this reaction  New  mM-1  Laisk  Bassham and Krause 1969 BBA. Default: 1.107

        PS::KM9 = 0.05 * theVars->PSRatio[52];   // SBP 9 SBP<->S7P+OP
        PS::KI9 = 12. * theVars->PSRatio[53];    // The inibintion constant for Pi;
        PS::KE9 = 6.66 * pow(10, 5) * theVars->PSRatio[54];

        PS::KM10 = 0.5 * theVars->PSRatio[55];    // R5P 10 S7P+GAP<->Ri5P+Xu5P
        PS::KM101 = 0.1 * theVars->PSRatio[56];   // Xu5P
        PS::KM102 = 0.09 * theVars->PSRatio[57];  // Estimate for GAP
        PS::KM103 = 0.015 * theVars->PSRatio[58]; // Estimate for S7P
        //theVars->KE10 = 1 / 0.85 * theVars->PSRatio[59];//   The equilibrium constant for this reaction          // New From Laisk or Bassham and Krause 1969 BBA

        PS::KE11 = 0.4 * theVars->PSRatio[60];    // Equilibrium Constant 11 Ri5P<-->Ru5P
        PS::KE12 = 0.67 * theVars->PSRatio[61];   // Equilibrium Constant 12 Xu5P<-->Ru5P

        PS::KM131 = 0.05 * theVars->PSRatio[62];  // Ru5P 13 Ru5P+ATP<->RuBP+ADP
        PS::KM132 = 0.059 * theVars->PSRatio[63]; //  ATP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI131 = 2. * theVars->PSRatio[64];    // PGA 13 Ru5P+ATP<->RuBP+ADP
        PS::KI132 = 0.7 * theVars->PSRatio[65];   // RuBP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI133 = 4. * theVars->PSRatio[66];    // Pi 13 Ru5P+ATP<->RuBP+ADP
        PS::KI134 = 2.5 * theVars->PSRatio[67];   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KI135 = 0.4 * theVars->PSRatio[68];   // ADP 13 Ru5P+ATP<->RuBP+ADP
        PS::KE13 = 6.846 * pow(10, 3) * theVars->PSRatio[69];

        PS::KM161 = 0.014 * theVars->PSRatio[70]; // ADP 16 ADP+Pi<->ATP
        PS::KM162 = 0.3 * theVars->PSRatio[71];   // Pi 16 ADP+Pi<-> ATP
        PS::KM163 = 0.3 * theVars->PSRatio[72];   // ATP 16  ADP+Pi<-> ATP                           // New       Based on Laisk
        PS::KE16 = 5.734 * theVars->PSRatio[73];  // The equilibrium constant for this reaction      // NEW, From Laisk or Bassham and Krause 1969 BBA

        PS::KE21 = 2.3 * theVars->PSRatio[74];    // Equilibrium constant 21 F6P<->G6P
        PS::KE22 = 0.058 * theVars->PSRatio[75];  //  Equilibrium constant 22 G6P<->G1P

        PS::KM311 = 0.077 * theVars->PSRatio[76]; // DHAP 31 DHAPi<->DHAPo
        //theVars->KM312 = 0.63 * theVars->PSRatio[77];//  Pi 31 DHAPi<->DHAPo
        PS::KM313 = 0.74 * theVars->PSRatio[78];  // Pext 31 DHAPi<->DHAPo
        PS::KM32 = 0.25 * theVars->PSRatio[79];   // PGA 32 PGAi<->PGAo
        PS::KM33 = 0.075 * theVars->PSRatio[80];  // GAP 33 GAPi<->GAPo


        // Now put in the constant for the new ADPG Pyrophosphorylase and starch
        // synthase

        // ATP + Glucose-1-Phosphate --> ADPG + PPi

        PS::KM231 = 0.031 * theVars->PSRatio[81]; // G1P 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM232 = 0.045 * theVars->PSRatio[82]; //  ATP 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM233 = 0.14 * theVars->PSRatio[83];  // ADPG 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KM234 = 0.8 * theVars->PSRatio[84];   //  PPi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KE23 = 7.6 * pow(10, (-3)) * theVars->PSRatio[85];


        PS::KA231 = 0.23 * theVars->PSRatio[86]; // PGA 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989
        PS::KI231 = 0.9 * theVars->PSRatio[87];  //0.9 ;       //   Pi 23 G1P+ATP<->ADPG + PPi        Laisk et al 1989 WY201803
        PS::KVmo = 0.007 * theVars->PSRatio[88]; //   The minimum maximum velocity        Laisk et al 1989
        // ADPG --> ADP + Gn     // The starch synthesis reaction 24.     Laisk et al
        // 1989

        PS::KM241 = 0.2 * theVars->PSRatio[89]; //   ADPG    ADPG --> ADP + Gn       Laisk et al 1989

        PS::KE25 = 1.2 * 107 * theVars->PSRatio[92];

        // Initialize the Vmax for different reactions

        if (theVars->GP == 0) {
            // Initialize the values of the global variables

            PS::SC = 1.;  // Scalling coefficient for the stroma volume per mg chl. defualt 2
            PS::SC1 = 1.;
            PS::STOM1 = 1.;
            PS::STOM2 = 1.;

            PS::V1 = 2.93 * PS::SC1 / PS::STOM1 * theVars->PSRatio[4]; // (Harris & Koniger, 1997)
            PS::V2 = 30.15 * PS::SC * PS::STOM2 * theVars->PSRatio[5]; // (Harris & Koniger, 1997)
            PS::V3 = 4.04 * PS::SC * PS::STOM2 * theVars->PSRatio[6];  // 1.57*PS::SC     ; // (Harris & Koniger, 1997)
            PS::V5 = 1.22 * PS::SC * theVars->PSRatio[7];          // (Harris & Koniger, 1997)
            PS::V6 = 0.734 * PS::SC / PS::STOM1 * theVars->PSRatio[8]; // (Harris & Koniger, 1997)
            PS::V7 = 3.12 * PS::SC * 4. * theVars->PSRatio[9];     // (Harris & Koniger, 1997)
            PS::V8 = 1.22 * PS::SC * theVars->PSRatio[10];         // (Harris & Koniger, 1997)
            PS::V9 = 0.32 * 3. * theVars->PSRatio[11];         // 0.17*PS::SC *FC ; // (Harris & Koniger, 1997) *3.
            //theVars->V10 = theVars->V7; // (Harris & Koniger, 1997)
            PS::V13 = 10.81 * PS::SC1 * theVars->PSRatio[12];      // (Harris & Koniger, 1997)
            PS::V16 = 5.47 * theVars->PSRatio[13];             // (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
            PS::V23 = 2. * theVars->PSRatio[14];
        }
        PS::V24 = 2. * theVars->PSRatio[15];
        PS::V31 = 1.0 * theVars->PSRatio[16] * 20;
        PS::V32 = 1.0 * theVars->PSRatio[17];
        PS::V33 = 1.0 * theVars->PSRatio[18] * 20;//WY 2018103
        PS::KE57 = 1.005 * 0.1 * theVars->PSRatio[93];
        PS::Km8p5p = 0.118 * theVars->PSRatio[94];
        PS::Km5p5p = 0.616 * theVars->PSRatio[95];
        PS::KE810 = 0.8446 * theVars->PSRatio[96];
        PS::Km5gap = 0.2727 * theVars->PSRatio[97];
        PS::Km8f6p = 0.5443 * theVars->PSRatio[98];
        PS::Km8s7p = 0.01576 * theVars->PSRatio[99];
        PS::Km8gap = 0.09 * theVars->PSRatio[100];
        PS::MaxCoeff = 5. * theVars->PSRatio[101];
        PS::PsV31 = PS::V31 * RegFactor; // 31 Phosphate translocator DHAPi<->DHAPo
        PS::PsV32 = PS::V32 * RegFactor; // 32 Phosphate translocator PGAi<->PGAo
        PS::PsV33 = PS::V33 * RegFactor; // 33 Phosphate translocator GAPi<->GAPo

    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the location for transfering variables//
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    theVars->ADP = PS::PS_C_CA - PS_con->ATP;
    PS::KE2Ratio = (1. + 1. / PS::KE21 + PS::KE22);
    PS::KE1Ratio = (1. + 1. / PS::KE11 + 1. / PS::KE12);

    return PS_con;
}

void PS::_initAlt(Variables *theVars, PSCondition* PS_con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<PS>();
    theVars->initParam(*PS_con);

    if (theVars->useC3) {
      if (theVars->GRNC == 1 && theVars->CO2_cond > 0.) {
        PS::set(MOD::PS::Vfactor1, theVars->VfactorCp[0]);
        PS::set(MOD::PS::Vfactor2, theVars->VfactorCp[2]);
        PS::set(MOD::PS::Vfactor3, theVars->VfactorCp[3]);
        PS::set(MOD::PS::Vfactor5, theVars->VfactorCp[5]);
        PS::set(MOD::PS::Vfactor7, theVars->VfactorCp[7]);
        PS::set(MOD::PS::Vfactor13, theVars->VfactorCp[12]);
        PS::set(MOD::PS::Vfactor23, theVars->VfactorCp[16]);
      }
      if (theVars->GRNT == 1 && theVars->Tp > 25) {
        PS::set(MOD::PS::Vf_T3, theVars->VfactorT[9]);
        PS::set(MOD::PS::Vf_T2, theVars->VfactorT[21]);
        PS::set(MOD::PS::Vf_T1, theVars->VfactorT[24]);
        PS::set(MOD::PS::Vf_T6, theVars->VfactorT[16]);
        PS::set(MOD::PS::Vf_T9, theVars->VfactorT[25]);
        PS::set(MOD::PS::Vf_T5, theVars->VfactorT[1]);
        PS::set(MOD::PS::Vf_T23, theVars->VfactorT[2]);
        PS::set(MOD::PS::Vf_T13, theVars->VfactorT[3]);
      }
      if (theVars->GP == 0) {
        PS::setFromEnzymeAct(MOD::PS::V1, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V2, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V3, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V5, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V6, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V7, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V8, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V9, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V10, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V13, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V23, theVars->EnzymeAct);
        PS::setFromEnzymeAct(MOD::PS::V16, theVars->EnzymeAct);
      }

      PS::set(MOD::PS::KM11,
              0.0097 *
              exp(PS::get(MOD::PS::c_c) -
                  PS::get(MOD::PS::dHa_c) * 1000. /
                  (PS::get(MOD::PS::R) *
                   (theVars->Tp + 273.15))) / 272.38);
      PS::set(MOD::PS::KM12,
              0.244 *
              exp(PS::get(MOD::PS::c_o) -
                  PS::get(MOD::PS::dHa_o) * 1000. /
                  (PS::get(MOD::PS::R) *
                   (theVars->Tp + 273.15))) / 165.82);

      PS::set(MOD::PS::PsV1_0,
              PS::get(MOD::PS::V1) *
              PS::get(MOD::PS::Vfactor1) *
              PS::get(MOD::PS::Vf_T1));
      PS::set(MOD::PS::PsV2_0,
              PS::get(MOD::PS::V2) *
              PS::get(MOD::PS::Vfactor2) *
              PS::get(MOD::PS::Vf_T2));
      PS::set(MOD::PS::PsV3_0,
              PS::get(MOD::PS::V3) *
              PS::get(MOD::PS::Vfactor3) *
              PS::get(MOD::PS::Vf_T3));
      PS::set(MOD::PS::PsV5_0,
              PS::get(MOD::PS::V5) *
              PS::get(MOD::PS::Vfactor5) *
              PS::get(MOD::PS::Vf_T5));
      PS::set(MOD::PS::PsV6_0,
              PS::get(MOD::PS::V6) *
              PS::get(MOD::PS::Vf_T6));
      PS::set(MOD::PS::PsV7_0,
              PS::get(MOD::PS::V7) *
              PS::get(MOD::PS::Vfactor7));
      PS::set(MOD::PS::PsV8_0,
              PS::get(MOD::PS::V8) *
              PS::get(MOD::PS::Vfactor5) *
              PS::get(MOD::PS::Vf_T5));
      PS::set(MOD::PS::PsV9_0,
              PS::get(MOD::PS::V9) *
              PS::get(MOD::PS::Vf_T9));
      PS::set(MOD::PS::PsV10_0,
              PS::get(MOD::PS::V10) *
              PS::get(MOD::PS::Vfactor7));
      PS::set(MOD::PS::PsV13_0,
              PS::get(MOD::PS::V13) *
              PS::get(MOD::PS::Vfactor13) *
              PS::get(MOD::PS::Vf_T13));
      PS::set(MOD::PS::PsV16, PS::get(MOD::PS::V16));
      PS::set(MOD::PS::PsV23_0,
              PS::get(MOD::PS::V23) *
              PS::get(MOD::PS::Vfactor23) *
              PS::get(MOD::PS::Vf_T23));
      PS::set(MOD::PS::PsV31,
              PS::get(MOD::PS::V31) *
              theVars->alpha2);
      PS::set(MOD::PS::PsV32,
              PS::get(MOD::PS::V32) *
              theVars->alpha2);
      PS::set(MOD::PS::PsV33,
              PS::get(MOD::PS::V33)
              * theVars->alpha2);
      PS::set(MOD::PS::Ru_Act,
              -3. * pow(10., -5.) * pow(theVars->Tp, 3.)
              + 0.0013 * pow(theVars->Tp, 2.)
              - 0.0106 * theVars->Tp + 0.8839);
      PS::set(MOD::PS::PsV1,
              PS::get(MOD::PS::PsV1_0) *
              PS::get(MOD::PS::Ru_Act) *
              pow(PS::get(MOD::PS::Q10_1), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV2,
              PS::get(MOD::PS::PsV2_0) *
              pow(PS::get(MOD::PS::Q10_2), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV3,
              PS::get(MOD::PS::PsV3_0) *
              pow(PS::get(MOD::PS::Q10_3), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV5,
              PS::get(MOD::PS::PsV5_0) *
              pow(PS::get(MOD::PS::Q10_5), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV6,
              PS::get(MOD::PS::PsV6_0)
              * pow(PS::get(MOD::PS::Q10_6), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV7,
              PS::get(MOD::PS::PsV7_0) *
              pow(PS::get(MOD::PS::Q10_7), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV8,
              PS::get(MOD::PS::PsV8_0) *
              pow(PS::get(MOD::PS::Q10_8), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV9,
              PS::get(MOD::PS::PsV9_0) *
              pow(PS::get(MOD::PS::Q10_9), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV10,
              PS::get(MOD::PS::PsV10_0) *
              pow(PS::get(MOD::PS::Q10_10), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV13,
              PS::get(MOD::PS::PsV13_0) *
              pow(PS::get(MOD::PS::Q10_13), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::PsV23,
              PS::get(MOD::PS::PsV23_0) *
              pow(PS::get(MOD::PS::Q10_23), (theVars->Tp - 25.) / 10.));
      PS::set(MOD::PS::I2,
              theVars->TestLi *
              theVars->alfa *
              (1. - theVars->fc) / 2.);
      PS::set(MOD::PS::J,
              (I2 + PS::get(MOD::PS::Jmax)
               - sqrt(pow(I2 + PS::get(MOD::PS::Jmax), 2.)
                      - 4. * PS::get(MOD::PS::Theta) *
                      I2 * PS::get(MOD::PS::Jmax))) /
              (2. * PS::get(MOD::PS::Theta)));
      
      
    } else {

      size_t i = 0;
      for (PS::iterator it = PS::begin(); it != PS::end(); it++) {
        if (i == 102)
          break;
        switch (it->first) {
        case (MOD::PS::V1) : {
          CHECK_RATIO_IDX(i, 4, it->first);
          it->second *= PS::get(MOD::PS::SC1) / PS::get(MOD::PS::STOM1) * theVars->PSRatio[i];
          i++;
          break;
        }
        case (MOD::PS::V2) :
        case (MOD::PS::V3) : {
          CHECK_RATIO_IDX(i, 5, MOD::PS::V2);
          CHECK_RATIO_IDX(i, 6, MOD::PS::V3);
          it->second *= PS::get(MOD::PS::SC) * PS::get(MOD::PS::STOM2) * theVars->PSRatio[i];
          i++;
          break;
        }
        case (MOD::PS::V5) :
        case (MOD::PS::V7) :
        case (MOD::PS::V8) : {
          CHECK_RATIO_IDX(i, 7, MOD::PS::V5);
          CHECK_RATIO_IDX(i, 9, MOD::PS::V7);
          CHECK_RATIO_IDX(i, 10, MOD::PS::V8);
          it->second *= PS::get(MOD::PS::SC) * theVars->PSRatio[i];
          i++;
          break;
        }
        case (MOD::PS::V6) : {
          CHECK_RATIO_IDX(i, 8, it->first);
          it->second *= PS::get(MOD::PS::SC) / PS::get(MOD::PS::STOM1) * theVars->PSRatio[i];
          i++;
          break;
        }
        case (MOD::PS::V10) : {
          // Skipped
          break;
        }
        case (MOD::PS::V13) : {
          CHECK_RATIO_IDX(i, 12, it->first);
          it->second *= PS::get(MOD::PS::SC1) * theVars->PSRatio[i];
          i++;
          break;
        }
        case (MOD::PS::PS_C_CN) : {
          // langmm: Present in Matlab version
          i++;
          break;
        }
        case (MOD::PS::KM32b) : {
          CHECK_RATIO_IDX(i, 31, it->first);
          it->second *= theVars->PSRatio[i];
          // PSRatio 32 & 33 skipped as KM41 & KM42 not used
          i = 34;
          break;
        }
        case (MOD::PS::KE7) : {
          CHECK_RATIO_IDX(i, 47, it->first);
          it->second *= theVars->PSRatio[i];
          // PSRatio 48 skipped
          i = 49;
          break;
        }
        // case (MOD::PS::KM41) :
        // case (MOD::PS::KM42) :
        case (MOD::PS::KM71) :
        case (MOD::PS::KM72) :
        case (MOD::PS::KM73) :
        case (MOD::PS::KM74) :
        case (MOD::PS::KE10) :
        case (MOD::PS::KM312) :
        case (MOD::PS::KA232) :
        case (MOD::PS::KA233) : {
          // PSRatio unused
          i++;
          break;
        }
        case (MOD::PS::KI23) : {
          // No PSRatio
          break;
        }
        default : {
          CHECK_RATIO_IDX(i, 42, MOD::PS::KE6);
          CHECK_RATIO_IDX(i, 89, MOD::PS::KM241);
          CHECK_RATIO_IDX(i, 92, MOD::PS::KE25);
          CHECK_RATIO_IDX(i, 93, MOD::PS::KE57);
          it->second *= theVars->PSRatio[i];
          i++;
        }
        }
      }
      PS::set(MOD::PS::PsV31, PS::get(MOD::PS::V31) * RegFactor);
      PS::set(MOD::PS::PsV32, PS::get(MOD::PS::V32) * RegFactor);
      PS::set(MOD::PS::PsV33, PS::get(MOD::PS::V33) * RegFactor);
    }
    
#ifndef CHECK_VALUE_SET_ALTS
    theVars->ADP = PS::get(MOD::PS::PS_C_CA) - PS_con->get(COND::PS::ATP);
#endif // CHECK_VALUE_SET_ALTS
    PS::set(MOD::PS::KE2Ratio,
            (1. + 1. / PS::get(MOD::PS::KE21) +
             PS::get(MOD::PS::KE22)));
    PS::set(MOD::PS::KE1Ratio,
            (1. + 1. / PS::get(MOD::PS::KE11) +
             1. / PS::get(MOD::PS::KE12)));
    
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(PS_con);
#endif // CHECK_VALUE_SET_ALTS
}

DEFINE_DEFAULT_CHECKALT(PS)

void PS::_reset() {
    PS::PiTc = 0.;
    PS::V1 = 0.;
    PS::KM12 = 0.;
    PS::KM11 = 0.;
    PS::V3 = 0.;
    PS::V6 = 0.;
    PS::V9 = 0.;
    PS::V13 = 0.;
    PS::V16 = 0.;
    PS::V23 = 0.;
    PS::Theta = 0.;
    PS::beta = 0.;
    PS::Jmax = 0.;

    PS::KA231 = 0.;
    PS::KE11 = 0.;
    PS::KE12 = 0.;
    PS::KE13 = 0.;
    PS::KE16 = 0.;
    PS::KE21 = 0.;
    PS::KE22 = 0.;
    PS::KE23 = 0.;
    PS::KE25 = 0.;
    PS::KE4 = 0.;
    PS::KE5 = 0.;
    PS::KE6 = 0.;
    PS::KE7 = 0.;
    PS::KE8 = 0.;
    PS::KE9 = 0.;
    PS::KI11 = 0.;
    PS::KI12 = 0.;
    PS::KI13 = 0.;
    PS::KI131 = 0.;
    PS::KI132 = 0.;
    PS::KI133 = 0.;
    PS::KI134 = 0.;
    PS::KI135 = 0.;
    PS::KI14 = 0.;
    PS::KI15 = 0.;
    PS::KI231 = 0.;
    PS::KI61 = 0.;
    PS::KI62 = 0.;
    PS::KI9 = 0.;
    PS::KM10 = 0.;
    PS::KM101 = 0.;
    PS::KM102 = 0.;
    PS::KM103 = 0.;
    PS::KM13 = 0.;
    PS::KM131 = 0.;
    PS::KM132 = 0.;
    PS::KM161 = 0.;
    PS::KM162 = 0.;
    PS::KM163 = 0.;
    PS::KM21 = 0.;
    PS::KM22 = 0.;
    PS::KM23 = 0.;
    PS::KM231 = 0.;
    PS::KM232 = 0.;
    PS::KM233 = 0.;
    PS::KM234 = 0.;
    PS::KM241 = 0.;
    PS::KM311 = 0.;
    PS::KM313 = 0.;
    PS::KM31a = 0.;
    PS::KM32 = 0.;
    PS::KM32b = 0.;
    PS::KM33 = 0.;
    PS::KM51 = 0.;
    PS::KM52 = 0.;
    PS::KM53 = 0.;
    PS::KM61 = 0.;
    PS::KM81 = 0.;
    PS::KM82 = 0.;
    PS::KM9 = 0.;
    PS::KVmo = 0.;
    PS::PS_C_CA = 0.;
    PS::PS_C_CP = 0.;
    PS::PS_C_CN = 0.;
    PS::PS_PEXT = 0.;
    PS::V24 = 0.;
    PS::V31 = 0.;
    PS::V32 = 0.;
    PS::V33 = 0.;
    PS::V2 = 0.;
    PS::V5 = 0.;
    PS::V7 = 0.;
    PS::V8 = 0.;
    PS::V10 = 0.;
    PS::V1Reg = 0.;
    PS::KA232 = 0.;
    PS::KA233 = 0.;
    PS::KI23 = 0.;
    PS::KM312 = 0.;
    PS::KE10 = 0.;
    PS::KM71 = 0.;
    PS::KM72 = 0.;
    PS::KM73 = 0.;
    PS::KM74 = 0.;
    PS::Vfactor1 = 1.;
    PS::Vfactor2 = 1.;
    PS::Vfactor3 = 1.;
    PS::Vfactor5 = 1.;
    PS::Vfactor7 = 1.;
    PS::Vfactor13 = 1.;
    PS::Vfactor23 = 1.;
    PS::Vf_T3 = 1.;
    PS::Vf_T2 = 1.;
    PS::Vf_T1 = 1.;
    PS::Vf_T6 = 1.;
    PS::Vf_T5 = 1.;
    PS::Vf_T9 = 1.;
    PS::Vf_T13 = 1.;
    PS::Vf_T23 = 1.;
    PS::PsV1 = 0.;
    PS::C3 = false;
    PS::_NADPH = 0.;

    PS::KE1Ratio = 0.;
    PS::KE2Ratio = 0.;
    PS::PsV1_0 = 0.;
    PS::PsV2_0 = 0.;
    PS::PsV3_0 = 0.;
    PS::PsV5_0 = 0.;
    PS::PsV6_0 = 0.;
    PS::PsV7_0 = 0.;
    PS::PsV8_0 = 0.;
    PS::PsV9_0 = 0.;
    PS::PsV10_0 = 0.;
    PS::PsV13_0 = 0.;
    PS::PsV16 = 0.;
    PS::PsV23_0 = 0.;
    PS::PsV31 = 0.;
    PS::PsV32 = 0.;
    PS::PsV33 = 0.;
    PS::Ru_Act = 0.;
    PS::PsV2 = 0.;
    PS::PsV3 = 0.;
    PS::PsV5 = 0.;
    PS::PsV6 = 0.;
    PS::PsV7 = 0.;
    PS::PsV8 = 0.;
    PS::PsV9 = 0.;
    PS::PsV10 = 0.;
    PS::PsV13 = 0.;
    PS::PsV23 = 0.;
    PS::I2 = 0.;
    PS::J = 0.;
    PS::KE57 = 0.;
    PS::Km8p5p = 0.;
    PS::Km5p5p = 0.;
    PS::KE810 = 0.;
    PS::Km5gap = 0.;
    PS::Km8f6p = 0.;
    PS::Km8s7p = 0.;
    PS::Km8gap = 0.;
    PS::MaxCoeff = 0.;

    PS::Param = {0., 0.};
    PS::TIME = 0.;
    PS::N = 1;
    conditions::PSCondition::reset();
}
