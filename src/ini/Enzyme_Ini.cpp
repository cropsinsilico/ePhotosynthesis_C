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
#include "modules/Enzyme.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double Enzyme::Bchl_CA = 0.;
double Enzyme::Bchl_CN = 0.;
double Enzyme::MC_CU = 0.;
double Enzyme::MC_CA = 0.;
double Enzyme::Mchl_CA = 0.;
double Enzyme::Mchl_CN = 0.;
double Enzyme::MC_UTP = 0.;
double Enzyme::Bper_GLU = 0.;
double Enzyme::Bper_KG = 0.;
double Enzyme::Bper_NADH = 0.;
double Enzyme::Bper_NAD = 0.;
double Enzyme::Bchl_CP = 0.;
double Enzyme::vNAE = 0.;
double Enzyme::V6sen = 0.;

int Enzyme::pathway_option = 0;
ePhotosynthesis::Velocity_s Enzyme::Vel_s = ePhotosynthesis::Velocity_s();
ePhotosynthesis::KValue Enzyme::KVal = ePhotosynthesis::KValue();
double Enzyme::TIME = 0.;
std::size_t Enzyme::N = 1;

//the code for the initalizer EnzymeIni.m
//simulation of our enzyme system

EnzymeCondition* Enzyme::_init(Variables *theVars) {

    Bchl_CA = 1.5;
    Bchl_CN = 0.5;
    //Bchl_CP= 40.0;

    MC_CU = 1.5;
    MC_CA = 1.0;
    Bchl_CP = 25.0;
    Mchl_CA = 1.5;
    Mchl_CN = 0.5;

    MC_UTP = 1.26;	// 0.75

    Bper_GLU = 24;
    Bper_KG = 0.4;
    Bper_NADH = 0.47;
    Bper_NAD = 0.4;

    //MC
    V6sen = 1.;
    const double VMDHsen = 1.;
    const double VMEsen = 1.;
    const double VPPDKsen = 1.;
    const double VGAPDHsen = 1.;
    const double VFBPsen = 1.;
    const double VSBPsen = 1.;
    const double VPRKsen = 1.;
    const double Jmaxsen = 1.;
    const double Vexsen = 1.;
    const double phi = 0.03;
    const double Lpd = 400.;

    EnzymeCondition *Enzyme_con = new EnzymeCondition();
    //Mchl_
    Enzyme_con->MC_HCO3 = 0.3;	// WY1911 0.005
    Enzyme_con->MC_OAA = 0.01;
    Enzyme_con->MC_PEP = 0.1;
    Enzyme_con->MC_Malate = 1.0;
    Enzyme_con->MC_Pyruvate = 2.;	// //////////0.02,0427////////////
    Enzyme_con->MC_PGA = 0.3;
    Enzyme_con->MC_FBP = 0.04;
    Enzyme_con->MC_UDPG = 0.035;
    Enzyme_con->MC_SUCP = 0.0;
    Enzyme_con->MC_SUC = 0.0;
    Enzyme_con->MC_F26BP = 7.8e-5;
    Enzyme_con->MC_ATP = 0.35;
    Enzyme_con->MC_T3P = 0.55;
    Enzyme_con->MC_HexP = 2.4;
    Enzyme_con->MC_Sucrose = 0.0;
    Enzyme_con->Mchl_OAA = 0.005;
    Enzyme_con->Mchl_Malate = 1.8;
    Enzyme_con->Mchl_PEP = 0.1;
    Enzyme_con->Mchl_Pyruvate = 0.01;
    Enzyme_con->Mchl_NADPH = 0.21;
    Enzyme_con->Mchl_ATP = 1.4;
    Enzyme_con->Mchl_PGA = 0.04;
    Enzyme_con->Mchl_DPGA = 0.0;
    Enzyme_con->Mchl_T3P = 0.6;

    //BSC
    Enzyme_con->BSC_T3P = 0.45;
    Enzyme_con->BSC_PGA = 0.2;
    Enzyme_con->BSC_Malate = 0.8;
    Enzyme_con->BSC_Pyruvate = 2.;	// ////0427,0.15////////
    Enzyme_con->BSC_CO2 = 0.001;

    //Bchl
    Enzyme_con->Bchl_CO2 = 0.01;
    Enzyme_con->Bchl_RuBP = 2.0;
    Enzyme_con->Bchl_PGA = 0.3;
    Enzyme_con->Bchl_DPGA = 0.;
    Enzyme_con->Bchl_ATP = 1.4;
    Enzyme_con->Bchl_NADPH = 0.1;
    Enzyme_con->Bchl_SBP = 0.015;
    Enzyme_con->Bchl_S7P = 0.045;
    Enzyme_con->Bchl_FBP = 0.06;
    Enzyme_con->Bchl_E4P = 0.05;
    Enzyme_con->Bchl_Starch = 0.0;
    Enzyme_con->Bchl_Rubisco = 1.456965457;
    Enzyme_con->Bchl_T3P = 0.5;
    Enzyme_con->Bchl_HexP = 6.;	// 2.2;WY2002
    Enzyme_con->Bchl_Pent = 0.05;
    Enzyme_con->Bchl_Malate = 0.3;
    Enzyme_con->Bchl_Pyruvate = 0.23;

    Enzyme_con->Bchl_PGCA = 0.0029;
    Enzyme_con->Bchl_GCA = 0.36;
    Enzyme_con->Bchl_GCEA = 0.1812;

    Enzyme_con->Bper_GCA = 0.36;
    Enzyme_con->Bper_GOA = 0.028;
    Enzyme_con->Bper_GLY = 1.8;
    Enzyme_con->Bper_SER = 7.5;
    Enzyme_con->Bper_HPR = 0.0035;
    Enzyme_con->Bper_GCEA = 0.1812;

    Enzyme_con->MC_CO2 = 0.8 * theVars->CI;

    Enzyme_con->Bchl_PPi = 0.;
    Enzyme_con->Bchl_ADPG = 0.;

    Enzyme_con->MC_Glu = 15.;
    Enzyme_con->MC_OxoG = 3.;
    Enzyme_con->MC_Asp = 5. * 2.;
    Enzyme_con->MC_Ala = 5. * 2.;
    Enzyme_con->BSC_OxoG = 3.;
    Enzyme_con->BSC_Glu = 15.;
    Enzyme_con->BSC_Asp = 5. * 2.;
    Enzyme_con->BSC_Ala = 5. * 2.;
    Enzyme_con->BSC_OAA = 0.;
    Enzyme_con->BSC_PEP = 0.1;
    Enzyme_con->BSC_ATP = 0.5;
    Enzyme_con->Bchl_OAA = 0.;
    Enzyme_con->MC_O2 = theVars->O2_cond;
    Enzyme_con->Mchl_O2 = theVars->O2_cond;
    Enzyme_con->BSC_O2 = theVars->O2_cond;
    Enzyme_con->Bchl_O2 = theVars->O2_cond;
    Enzyme_con->Bchl_PEP = 0.1;	// ////////////WY PPDK in BSCytosol
    Enzyme_con->Mchl_GCEA = 0.1812;

    Enzyme_con->Bmito_OAA = 0.;
    Enzyme_con->Bmito_MAL = 0.;
    Enzyme_con->Bmito_PYR = 5.;
    Enzyme_con->Bmito_CO2 = 0.001;
    Enzyme_con->Bmito_NADH = 0.3;

    Enzyme_con->Bchl_Asp = 0.;
    Enzyme_con->Bchl_Ala = 0.;
    Enzyme_con->Mchl_Asp = 0.;
    Enzyme_con->Mchl_Ala = 0.;
    Enzyme_con->E_PPDK_Mchl = 0;	//  E_PPDK active
    Enzyme_con->EP_PPDK_Mchl = 0.616;	// 1.28;	// EP_PPDK inactive //WY202010 0.616 asumming Vmax_PPDK = 80


    //Km Ki(mM)
    KVal.KmCO2_1 = 2.8;
    //4.2.1.1        1
    KVal.Ke_1 = 11.2;    //    20;       //  No unit  k=4.3*10^(-7)  PH=7.5 13.6 11.82
    //4.1.1.31       2 //KmHCO3_2 =0.05 KmPEP_2 =0.047

    KVal.KmHCO3_2 = 0.02;
    KVal.KmPEP_2 = 0.1;
    KVal.Kimal_2 = 1.;
    //WY202003 Maize KmHCO3_2 =0.04216; KmPEP_2 =1;   Kimal_2 =15;
    //KmPEP_2 =0.038;
    //1.1.1.82       3
    KVal.KmNADPH_3 = 0.024;
    KVal.KmOAA_3 = 0.056;
    KVal.KmNADP_3 = 0.073;
    KVal.Kmmal_3 = 32.0;
    KVal.Ke_3 = 4450.0; // No unit  ;
    //1.1.1.40       4 Kmmal_4 =0.04;
    KVal.KmCO2_4 = 1.1;
    KVal.KmNADP_4 = 0.0080;
    KVal.KmNADPH_4 = 0.045;
    KVal.KmPyr_4 = 3.;
    KVal.Kmmal_4 = 0.23;
    KVal.Ke_4 = 0.051 * 55.35 * 1000.;   //   0.0344 KmNADP_4 =0.008;
    //2.7.9.1        5 KiPEP_5 =0.5;
    KVal.KiPEP_5 = 0.15;
    KVal.KmATP_5 = 0.082;
    KVal.KmPyr_5 = 0.082;
    // KmPyr_5 =0.25
    //4.1.1.39       6  KiPGA_6 =0.84; KiFBP_6 =0.4;
    KVal.KmCO2_6 = 0.0162;
    KVal.KmO2_6 = 0.183; //   0.222
    KVal.KmRuBP_6 = 0.02;
    KVal.KiPGA_6 = 2.52;
    KVal.KiFBP_6 = 0.04; //  0.8
    KVal.KiSBP_6 = 0.075; //  0.75
    KVal.KiPi_6 = 0.9 * 3.;  //    0.9
    KVal.KiNADPH_6 = 0.07 * 3.; //    0.07
    //KiNADPH_6 =0.07;KiPi_6 =0.9
    //2.7.2.3        7
    KVal.KmADP_7 = 0.5;
    KVal.KmATP_7 = 0.3;
    KVal.KmPGA_7 = 2.4; // 1.2 //  =0.24
    //1.2.1.13       8
    KVal.KmDPGA_8 = 0.4;
    KVal.KmNADPH_8 = 0.1;
    //5.3.1.1        9
    KVal.Ke_9 = 0.05;
    //4.1.2.13FBP    10
    KVal.KmDHAP_10 = 0.45;
    KVal.KmFBP_10 = 0.0923;
    KVal.KmGAP_10 = 0.04;  // =0.3;
    KVal.Ke_10 = 7.1; // 1/millimolarity
    //3.1.3.11       11 KmFBP_11 =0.033; Chloroplast fructose-1,6-bisphosphatase
    //from Oryza differs in salt tolerance property from the Porteresia enzyme
    //and is protected by osmolytes  Ghosh, S.; Bagchi, S.; Lahiri Majumder, A.;Plant Sci. 160, 1171-1181 (2001)
    KVal.KiF6P_11 = 0.7;
    KVal.KiPi_11 = 12.0;
    KVal.KmFBP_11 = 0.066;
    KVal.Ke_11 = 666000.0;
    //4.1.2.13SBP    12
    KVal.KmDHAP_12 = 0.4;
    KVal.KmE4P_12 = 0.2;
    KVal.Ke_12 = 1.017;  // 1/millimolarity  ;
    //3.1.3.37       13
    KVal.KiPi_13 = 12.0;
    KVal.KmSBP_13 = 0.05;
    KVal.Ke_13 = 666000.0;
    //2.2.1.1X       14 Datta, A.G.; Racker, E.; J. Biol. Chem.; 236, 617 (1961).
    KVal.KmE4P_14 = 0.1;
    KVal.KmF6P_14 = 0.1;
    KVal.KmGAP_14 = 0.1;
    KVal.KmXu5P = 0.1;
    KVal.Ke_14 = 0.084;  // No unit  =10.0 =0.076
    //2.2.1.1R       15
    KVal.KmGAP_15 = 0.072;
    KVal.KmRi5P_15 = 1.5;
    KVal.KmS7P_15 = 0.015;
    KVal.KmXu5P_15 = 0.1;  //  =0.015   =0.46
    KVal.Ke_15 = 0.9; //  1.176470588235294 // No unit Datta, A.G.; Racker, E.; J. Biol. Chem.; 236, 617 (1961).
    //5.3.1.6:Chl    16
    KVal.Ke_16 = 0.4;
    //5.1.3.1:Chl    17
    KVal.Ke_17 = 0.67;
    //2.7.1.19       18 // KiPGA_18 =1.0;
    KVal.KiADP_18 = 2.5;
    KVal.Ki_ADP_18 = 0.4;
    KVal.KiPGA_18 = 2.0;
    KVal.KiPi_18 = 4.0;    //   1.0
    KVal.KiRuBP_18 = 0.7;
    KVal.KmATP_18 = 0.625;  //  0.059  ;
    KVal.KmRu5P_18 = 0.05; //;
    KVal.Ke_18 = 6846.0; // No unit  ;
    //2.7.2.3:MChl  7Mchl
    KVal.KmADP_7Mchl = 0.5;
    KVal.KmATP_7Mchl = 0.3;
    KVal.KmPGA_7Mchl = 2.4;
    //1.2.1.13:MChl   8Mchl
    KVal.KmDPGA_8Mchl = 0.4;
    KVal.KmNADPH_8Mchl = 0.1;
    //StarchSynthesis:Chl   Starchsyn  KmG1P_Starch =0.24;
    KVal.KiADP_Starch = 10.0;
    KVal.KmATP_Starch = 0.08;
    KVal.KmG1P_Starch = 0.48;   //   0.08 ;
    KVal.KaF6P_Starch = 0.12; // 0.06 0.02  ;
    KVal.KaFBP_Starch = 0.12;   //  0.02  ;
    KVal.KaPGA_Starch = 0.3;    //  0.1  Ka (No unit)  ;
    KVal.Ke_Starch1 = 2.3;
    KVal.Ke_Starch2 = 0.058;
    KVal.KmPGA_PGASink = 1.; //  0.5
    //4.1.2.13FBP:Cel    Suc1
    KVal.KmDHAP_Suc1 = 0.45;   //  0.4  ;
    KVal.KmGAP_Suc1 = 0.04 / 2.;  //  0.1 ;
    KVal.KmFBP_Suc1 = 0.0023; //   0.2  ;
    KVal.Ke_Suc1 = 12.; // 12.0 10000 // 1/millimolarity  ;
    //3.1.3.11:Cel    Suc2 KiF26BP_Suc2 =7.0E-5;KmFBP_Suc2 =0.165;Ke_Suc2=6663.0
    //Roles of the residues Lys115 and Tyr116 in the binding of an allosteric inhibitor AMP to pea cytosolic D-fructose-1,6-bisphosphatase
    //Jang, H.; Cho, M.; Kwon, Y.; Bhoo, S.H.; Jeon, J.; Hahn, T.; J. Appl. Biol. Chem. 51, 45-49 (2008)
    KVal.KiF26BP_Suc2 = 0.00007;
    KVal.KiF6P_Suc2 = 0.7;
    KVal.KiPi_Suc2 = 12.0;
    KVal.KmFBP_Suc2 = 0.00108;
    KVal.Ke_Suc2 = 174.0;
    //5.3.1.9:Cel    Suc5
    KVal.Ke_Suc5 = 2.3;
    //5.4.2.2:Cel    Suc6
    KVal.Ke_Suc6 = 0.0584;
    //2.7.7.9:Cel    Suc7
    KVal.KmG1P_Suc7 = 0.14;
    KVal.KmPPi_Suc7 = 0.11;
    KVal.KmUDPG_Suc7 = 0.12;
    KVal.KmUTP_Suc7 = 0.1;
    KVal.Ke_Suc7 = 0.31; // 0.31// No unit   ;
    //2.4.1.14:Cel   Suc8
    KVal.KiFBP_Suc8 = 0.8;
    KVal.KiPi_Suc8 = 5.0;
    KVal.KiSuc_Suc8 = 50.0;
    KVal.KiSucP_Suc8 = 0.4;
    KVal.KiUDP_Suc8 = 0.7;
    KVal.KmF6P_Suc8 = 0.8;
    KVal.KmUDPG_Suc8 = 1.3;
    KVal.Ke_Suc8 = 10.0;  // No unit  ;
    //3.1.3.24:Cel     Suc9
    KVal.KmSuc_Suc9 = 80.0;
    KVal.KmSucP_Suc9 = 0.35;
    KVal.Ke_Suc9 = 780.0;
    //SUCSink:Cel   Suc10
    KVal.KmSuc_Suc10 = 1.5;
    //2.7.1.105:Cel     Suc3
    KVal.KiADP_Suc3 = 0.16;
    KVal.KIDHAP_Suc3 = 0.7;
    KVal.KmATP_Suc3 = 1.32;
    KVal.KmF26BP_Suc3 = 0.021;
    KVal.KmF6P_Suc3 = 1.4;
    KVal.Ke_Suc3 = 590.0;  //  No unit  ;
    //3.1.3.46:Cel    Suc4
    KVal.KiF6P_Suc4 = 0.1;
    KVal.KiPi_Suc4 = 0.5 * 10.;
    KVal.KmF26BP_Suc4 = 0.032;
    //3.6.1.1:Cel
    KVal.KePi = 128.4;
    //3.6.3.14:MChl   ATPM
    KVal.KmADP_ATPM = 0.014;
    KVal.KmATP_ATPM = 0.11;
    KVal.KmPi_ATPM = 0.3;
    KVal.X = 0.667;
    KVal.Y = 0.6;
    KVal.F = 0.7225;
    KVal.Q = 0.9;
    KVal.D = 1.; //  No unit  ;
    KVal.Ke_ATPM = 5.734;        //1/millimolarity ;
    //1.18.1.2:MChl  NADPHM
    KVal.KmNADP_NADPHM = 0.05;
    KVal.KmNADPH_NADPHM = 0.058; //  =0.0072;
    KVal.Ke_NADPHM = 502.;
    KVal.E = 0.5; // No unit  ;
    //V3.6.3.14:Chl       ATPB
    KVal.KmADP_ATPB = 0.014;
    KVal.KmPi_ATPB = 0.11;
    KVal.KmATP_ATPB = 0.3;
    KVal.Ke_ATPB = 5.734; // 1/millimolarity  ;
    KVal.G = 0.667; // No unit  ;
    //1.18.1.2:BChl  NADPHB
    KVal.KmNADP_NADPHB = 0.05;
    KVal.KmNADPH_NADPHB = 0.058;
    KVal.Ke_NADPHB = 502.;  // No unit  ;
    // Transport coeffcient (1/second)
    KVal.Voaa = 1.5;
    KVal.Vmal = 1.5;
    KVal.Vpyr = 1.5;
    KVal.Vpep = 1.5;
    KVal.Vt = 1.5;
    KVal.Vleak = 1.;
    KVal.Vpga = 2.;
    KVal.KmCO2_PR1 = 0.0162;
    KVal.KmO2_PR1 = 0.183;
    KVal.KmRuBP_PR1 = 0.02;
    KVal.KiPGA_PR1 = 2.52;
    KVal.KiFBP_PR1 = 0.04;
    KVal.KiSBP_PR1 = 0.75;
    KVal.KiPi_PR1 = 0.9 * 3.;
    KVal.KiNADPH_PR1 = 0.21;
    //3.1.3.18 2
    KVal.KmPGCA_PR2 = 0.026;
    KVal.KiPI_PR2 = 2.55;
    KVal.KiGCA_PR2 = 94.0;
    //1.1.3.15 3
    KVal.KmGCA_PR3 = 0.1;    // = 0.1;0.0;
    //2.6.1.4 4
    KVal.Ke_PS4 = 607.0;
    KVal.KmGOA_PS4 = 0.15;
    KVal.KmGLU_PS4 = 1.7;
    KVal.KiGLY_PS4 = 2.0;
    //GLY_SER:Mit 5
    KVal.KmGLY_PS5 = 6.0;
    KVal.KiSER_PS5 = 4.0;
    //2.6.1.45 6
    KVal.Ke_PR6 = 0.24;
    KVal.KmGOA_PR6 = 0.15;
    KVal.KmSER_PR6 = 2.7;
    KVal.KmGLY_PR6 = 33.0;
    //1.1.1.29 7
    KVal.Ke_PR7 = 250000.0;
    KVal.KiHPR_PR7 = 12.0;
    KVal.KmHPR_PR7 = 0.09;
    //2.7.1.31 8
    KVal.Ke_PR8 = 300.0;
    KVal.KmATP_PR8 = 0.21;
    KVal.KmGCEA_PR8 = 0.25;
    KVal.KiPGA_PR8 = 0.72;   //   0.36  ;
    //Tgca 9
    KVal.KmGCA_PR9 = 0.2;
    KVal.KiGCEA_PR9 = 0.22;
    //Tgcea 10
    KVal.KmGCEA_PR10 = 0.39;
    KVal.KiGCA_PR10 = 0.28;
    KVal.KmPGA_62 = 0.08;
    KVal.KmPEP_62 = 0.3;
    KVal.Ke_62 = 0.4302; // G66 = +0.5;  ;
    //mM/(L*s)

    ////////////////////////////////////////////////////////////////////////////
    //Temp correction
    const double Ea_Vpmax = 94.8 * 1000.;
    const double dS_Vpmax = 0.25 * 1000.;
    const double Hd_Vpmax = 73.3 * 1000.;
    const double Ea_Vcmax = 78. * 1000.;
    const double Ea_PPDK = 58.1 * 1000.;
    const double R = 8.3144598;	//  m2 kg s-2 K-1 mol-1
    const double MTempCorr_V2 = exp(Ea_Vpmax * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15))) * (1 + exp((298.15 * dS_Vpmax - Hd_Vpmax) / (298.15 * R))) / (1 + exp(((theVars->Tp + 273.15) * dS_Vpmax - Hd_Vpmax) / ((theVars->Tp + 273.15) * R)));
    const double MTempCorr_V6 = exp(Ea_Vcmax * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15)));
    const double MTempCorr_V5 = exp(Ea_PPDK * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15)));
    const double Vpmax_25 = theVars->Vpmax / MTempCorr_V2;
    const double Vcmax_25 = theVars->Vcmax / MTempCorr_V6;
    const double Vppdkmax_25 = theVars->Vcmax / MTempCorr_V5;
    const double Vm_6o = 0.065;	// 2.4;	// 1.8;	// 2.9;	// 3.4967170968;2.913930914 2.09803025808
    const double Vppdkmax_E = Vppdkmax_25 / 1000.;

    const double Vm_6 = Vcmax_25 / 1000. / theVars->FactorVC;

    const double PC3P = 0.0327;

    theVars->vrpd = theVars->MRd / 2. / 1000.;	// 0.0005;

    Vel_s.Vm_1 = 200.;
    Vel_s.Vm_2 = Vpmax_25 / 1000. / theVars->FactorVP;	// 3.6;	// 4;
    Vel_s.Vm_3 = 1.8 * Vppdkmax_E * VMDHsen;	// 2.4*Vm_2;	// 1.2;
    Vel_s.Vm_4 = 1.33 * Vppdkmax_E * VMEsen;	// 2.4*Vm_2;	// /1.5;	// 2.4;	// 3.2
    Vel_s.Vm_5 = 1.33 * Vppdkmax_E * VPPDKsen;	// 2.4*Vm_2;	// *1.5;	// 2.4; WY 201902
    Vel_s.Vm_6 = Vm_6;
    Vel_s.Vm_78 = 0.4 * Vm_6 / Vm_6o * VGAPDHsen;	// Vm_7 = 30.15;	// 16.07510272;	//  30.15;	// 16.07510272
    Vel_s.Vm_8 = 0.;	// 4.308781695;
    Vel_s.Vm_10 = 0.0731 * Vm_6 / Vm_6o;	// 2.9253370968*1.5;	// 1.4626685484;
    Vel_s.Vm_11 = 0.0436 * Vm_6 / Vm_6o * VFBPsen;
    Vel_s.Vm_12 = 0.1097 * Vm_6 / Vm_6o;
    Vel_s.Vm_13 = 0.0292 * Vm_6 / Vm_6o * VSBPsen;
    Vel_s.Vm_14 = 0.2810 * Vm_6 / Vm_6o;
    Vel_s.Vm_15 = 0.2810 * Vm_6 / Vm_6o;
    Vel_s.Vm_18 = 1.7552 * Vm_6 / Vm_6o * VPRKsen;
    Vel_s.Vm_78Mchl = 0.3 * Vm_6 / Vm_6o * VGAPDHsen;
    //Vm_7Mchl = 15.1;
    Vel_s.Vm_8Mchl = 0.;	// 2.6929885593*Vm_6/Vm_6o;
    Vel_s.Vm_Starch = 0.0133 * Vm_6 / Vm_6o;
    Vel_s.Vm_PGASink = 0.002 * Vm_6 / Vm_6o;	// 0.5/5;
    Vel_s.Vm_Suc1 = 0.0081 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc2 = 0.0064 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc7 = 0.0058 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc8 = 0.0278 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc9 = 0.0278 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc10 = 0.0035 * Vm_6 / Vm_6o;	// 2.0
    Vel_s.Vm_Suc3 = 0.0010 * Vm_6 / Vm_6o;
    Vel_s.Vm_Suc4 = 8.4096e-4 * Vm_6 / Vm_6o;
    //Vel_s[27] = 0.; //theVars->TestLi * CONVERT/1000;
    Vel_s.Jmax = 0.5 * Vm_6 / Vm_6o * Jmaxsen;	//  20;
    Vel_s.Vm_ATPM = 0.3 * Vm_6 / Vm_6o;
    Vel_s.Vm_NADPHM = 0.2 * Vm_6 / Vm_6o;
    Vel_s.Vm_ATPB = 0.3 * Vm_6 / Vm_6o;
    Vel_s.Vm_NADPHB = 0.2 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR1 = Vm_6 * 0.11;	// 0.69934341936;(Cousins 2010 0.11) ;
    Vel_s.Vm_PR2 = 2.6210 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR3 = 0.0728 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR4 = 0.1373 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR5 = 0.1247 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR6 = 0.1653 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR7 = 0.5005 * Vm_6 / Vm_6o;
    Vel_s.Vm_PR8 = 0.2858 * Vm_6 / Vm_6o;
    Vel_s.VTgca_PR9 = 0.3 * Vm_6 / Vm_6o;
    Vel_s.VTgcea_PR10 = 0.25 * Vm_6 / Vm_6o;
    Vel_s.Vm_62 = 0.001 * Vexsen;	// mM/s //1.45 E-5;
    Vel_s.Vtp_Bchl = 0.75;
    Vel_s.Vtp_Mchl = 0.75;
    Vel_s.Vm_Sta1 = 0.03 * Vm_6 / Vm_6o;
    Vel_s.Vm_Sta2 = 1. * Vm_6 / Vm_6o;
    Vel_s.Vm_Sta3 = 0.025 * Vm_6 / Vm_6o;
    Vel_s.Vm_OAA_M = 0.08 * Vm_6 / Vm_6o;
    Vel_s.Vm_PYR_B = 0.15 * Vm_6 / Vm_6o;
    Vel_s.Vm_PYR_M = 0.15 * Vm_6 / Vm_6o;
    Vel_s.Vm_PEP_M = 0.15 * Vm_6 / Vm_6o;
    // transport between two cell types
    Vel_s.Pmal = 0.0421 * (phi / 0.03) / (Lpd / 400.);
    Vel_s.Ppyr = 0.0436 * (phi / 0.03) / (Lpd / 400.);
    Vel_s.Pco2 = 0.1139 * (phi / 0.03) / (Lpd / 400.);
    Vel_s.Pc3p = PC3P * (phi / 0.03) / (Lpd / 400.);
    Vel_s.Pco2_B = 0.4;	// PCO2_B=0.002 cm s-1 SChl/Sl =10


    return Enzyme_con;
}

void Enzyme::_reset() {
    Bchl_CA = 0.;
    Bchl_CN = 0.;
    MC_CU = 0.;
    MC_CA = 0.;
    Mchl_CA = 0.;
    Mchl_CN = 0.;
    MC_UTP = 0.;
    Bper_GLU = 0.;
    Bper_KG = 0.;
    Bper_NADH = 0.;
    Bper_NAD = 0.;
    Bchl_CP = 0.;
    vNAE = 0.;

    pathway_option = 0;
}
