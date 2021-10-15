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

std::vector<arr> Enzyme::KValue = {};
double Enzyme::TIME = 0.;
std::size_t Enzyme::N = 1;

//the code for the initalizer EnzymeIni.m
//simulation of our enzyme system

EnzymeCondition* Enzyme::_init(Variables *theVars) {
    // global I;
    //global VPPDKsen;
    //global VMDHsen;
    //global VMEsen;
    //global VGAPDHsen;
    //global VSBPsen;
    //global VFBPsen;
    //global VPRKsen;
    //global Vexsen;
    //global Jmaxsen;

    // Convert = 1E6 / (2.35E5);// --unused
    Bchl_CA = 1.5;   // constant set in globals.hpp
    Bchl_CN = 0.5;   // constant set in globals.hpp
    //Bchl_CP= 40.0;

    MC_CU = 1.5;   // constant set in globals.hpp
    MC_CA = 1.0;   // constant set in globals.hpp
    Bchl_CP = 25.0;
    Mchl_CA = 1.5;   // constant set in globals.hpp
    Mchl_CN = 0.5;   // constant set in globals.hpp

    MC_UTP = 1.26;//0.75   // constant set in globals.hpp

    //global Bper_GLU;
    //global Bper_KG;
    //global Bper_NADH;
    //global Bper_NAD;
    Bper_GLU = 24;   // constant set in globals.hpp
    Bper_KG = 0.4;   // constant set in globals.hpp
    Bper_NADH = 0.47;   // constant set in globals.hpp
    Bper_NAD = 0.4;   // constant set in globals.hpp

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
    const double MC_HCO3 = 0.3;//WY1911 0.005
    const double MC_OAA = 0.01;
    const double MC_PEP = 0.1;
    const double MC_Malate = 1.0;
    const double MC_Pyruvate = 2;////////////0.02,0427////////////
    const double MC_PGA = 0.3;
    const double MC_FBP = 0.04;
    const double MC_UDPG = 0.035;
    const double MC_SUCP = 0.0;
    const double MC_SUC = 0.0;
    const double MC_F26BP = 7.8E-5;
    const double MC_ATP = 0.35;//0.35
    const double MC_T3P = 0.55;
    const double MC_HexP = 2.4;
    const double MC_Sucrose = 0.0;
    //Mchl_
    const double Mchl_OAA = 0.005;
    const double Mchl_Malate = 1.8;
    const double Mchl_PEP = 0.1;
    const double Mchl_Pyruvate = 0.01;
    const double Mchl_NADPH = 0.21;
    const double Mchl_ATP = 1.4;
    const double Mchl_PGA = 0.04;
    const double Mchl_DPGA = 0.0;
    const double Mchl_T3P = 0.6;
    //BSC
    const double BSC_T3P = 0.45;
    const double BSC_PGA = 0.2;
    const double BSC_Malate = 0.8;
    const double BSC_Pyruvate = 2;//////0427,0.15////////
    const double BSC_CO2 = 0.001;
    //Bchl
    const double Bchl_CO2 = 0.01;
    const double Bchl_RuBP = 2.0;
    const double Bchl_PGA = 0.3;
    const double Bchl_DPGA = 0;
    const double Bchl_ATP = 1.4;
    const double Bchl_NADPH = 0.1;
    const double Bchl_SBP = 0.015;
    const double Bchl_S7P = 0.045;
    const double Bchl_FBP = 0.06;
    const double Bchl_E4P = 0.05;
    const double Bchl_Starch = 0.0;
    const double Bchl_Rubisco = 1.456965457;
    const double Bchl_T3P = 0.5;
    const double Bchl_HexP = 6;//2.2;WY2002
    const double Bchl_Pent = 0.05;
    const double Bchl_Malate = 0.3;
    const double Bchl_Pyruvate = 0.23;

    const double Bchl_PGCA = 0.0029;
    const double Bchl_GCA = 0.36;
    const double Bchl_GCEA = 0.1812;

    const double Bper_GCA = 0.36;
    const double Bper_GOA = 0.028;
    const double Bper_GLY = 1.8;
    const double Bper_SER = 7.5;
    const double Bper_HPR = 0.0035;
    const double Bper_GCEA = 0.1812;

    const double Bchl_PPi = 0;
    const double Bchl_ADPG = 0;


    //global CI;
    const double MC_CO2 = 0.8 * theVars->CI;

    const double MC_Glu = 15;
    const double MC_OxoG = 3;
    const double MC_Asp = 5 * 2;
    const double MC_Ala = 5 * 2;

    const double BSC_OxoG = 3;
    const double BSC_Glu = 15;
    const double BSC_Asp = 5 * 2;
    const double BSC_Ala = 5 * 2;
    const double BSC_OAA = 0;
    const double BSC_PEP = 0.1;
    const double BSC_ATP = 0.5;
    const double Bchl_OAA = 0;

    //global O2;
    const double MC_O2 = theVars->O2_cond;
    const double Mchl_O2 = theVars->O2_cond;
    const double BSC_O2 = theVars->O2_cond;
    const double Bchl_O2 = theVars->O2_cond;

    const double Bchl_PEP = 0.1;
    const double Mchl_GCEA = 0.1812;

    const double Bmito_OAA = 0;
    const double Bmito_MAL = 0;
    const double Bmito_PYR = 5;
    const double Bmito_CO2 = 0.001;
    const double Bmito_NADH = 0.3;

    const double Bchl_Asp = 0;
    const double Bchl_Ala = 0;
    const double Mchl_Asp = 0;
    const double Mchl_Ala = 0;
    const double E_PPDK_Mchl = 0;// E_PPDK active
    const double EP_PPDK_Mchl = 0.616;//1.28;//EP_PPDK inactive //WY202010 0.616 asumming Vmax_PPDK = 80


    EnzymeCondition *Enzyme_con = new EnzymeCondition();
    Enzyme_con->MC_HCO3 = MC_HCO3;
    Enzyme_con->MC_OAA = MC_OAA;
    Enzyme_con->MC_PEP = MC_PEP;
    Enzyme_con->MC_Malate = MC_Malate;
    Enzyme_con->MC_Pyruvate = MC_Pyruvate;
    Enzyme_con->MC_PGA = MC_PGA;
    Enzyme_con->MC_FBP = MC_FBP;
    Enzyme_con->MC_UDPG = MC_UDPG;
    Enzyme_con->MC_SUCP = MC_SUCP;
    Enzyme_con->MC_SUC = MC_SUC;
    Enzyme_con->MC_F26BP = MC_F26BP;
    Enzyme_con->MC_ATP = MC_ATP;
    Enzyme_con->MC_T3P = MC_T3P;
    Enzyme_con->MC_HexP = MC_HexP;
    Enzyme_con->MC_Sucrose = MC_Sucrose;
    Enzyme_con->Mchl_OAA = Mchl_OAA;
    Enzyme_con->Mchl_Malate = Mchl_Malate;
    Enzyme_con->Mchl_PEP = Mchl_PEP;
    Enzyme_con->Mchl_Pyruvate = Mchl_Pyruvate;
    Enzyme_con->Mchl_NADPH = Mchl_NADPH;
    Enzyme_con->Mchl_ATP = Mchl_ATP;
    Enzyme_con->Mchl_PGA = Mchl_PGA;
    Enzyme_con->Mchl_DPGA = Mchl_DPGA;
    Enzyme_con->Mchl_T3P = Mchl_T3P;
    Enzyme_con->BSC_T3P = BSC_T3P;
    Enzyme_con->BSC_PGA = BSC_PGA;
    Enzyme_con->BSC_Malate = BSC_Malate;
    Enzyme_con->BSC_Pyruvate = BSC_Pyruvate;
    Enzyme_con->BSC_CO2 = BSC_CO2;
    Enzyme_con->Bchl_CO2 = Bchl_CO2;
    Enzyme_con->Bchl_RuBP = Bchl_RuBP;
    Enzyme_con->Bchl_PGA = Bchl_PGA;
    Enzyme_con->Bchl_DPGA = Bchl_DPGA;
    Enzyme_con->Bchl_ATP = Bchl_ATP;
    Enzyme_con->Bchl_NADPH = Bchl_NADPH;
    Enzyme_con->Bchl_SBP = Bchl_SBP;
    Enzyme_con->Bchl_S7P = Bchl_S7P;
    Enzyme_con->Bchl_FBP = Bchl_FBP;
    Enzyme_con->Bchl_E4P = Bchl_E4P;
    Enzyme_con->Bchl_Starch = Bchl_Starch;
    Enzyme_con->Bchl_Rubisco = Bchl_Rubisco;
    Enzyme_con->Bchl_T3P = Bchl_T3P;
    Enzyme_con->Bchl_HexP = Bchl_HexP;
    Enzyme_con->Bchl_Pent = Bchl_Pent;
    Enzyme_con->Bchl_Malate = Bchl_Malate;
    Enzyme_con->Bchl_Pyruvate = Bchl_Pyruvate;

    Enzyme_con->Bchl_PGCA = Bchl_PGCA;
    Enzyme_con->Bchl_GCA = Bchl_GCA;
    Enzyme_con->Bchl_GCEA = Bchl_GCEA;

    Enzyme_con->Bper_GCA = Bper_GCA;
    Enzyme_con->Bper_GOA = Bper_GOA;
    Enzyme_con->Bper_GLY = Bper_GLY;
    Enzyme_con->Bper_SER = Bper_SER;
    Enzyme_con->Bper_HPR = Bper_HPR;
    Enzyme_con->Bper_GCEA = Bper_GCEA;
    Enzyme_con->MC_CO2 = MC_CO2;

    Enzyme_con->Bchl_PPi = Bchl_PPi;
    Enzyme_con->Bchl_ADPG = Bchl_ADPG;

    Enzyme_con->MC_Glu = MC_Glu;
    Enzyme_con->MC_OxoG = MC_OxoG;
    Enzyme_con->MC_Asp = MC_Asp;
    Enzyme_con->MC_Ala = MC_Ala;
    Enzyme_con->BSC_OxoG = BSC_OxoG;
    Enzyme_con->BSC_Glu = BSC_Glu;
    Enzyme_con->BSC_Asp = BSC_Asp;
    Enzyme_con->BSC_Ala = BSC_Ala;
    Enzyme_con->BSC_OAA = BSC_OAA;
    Enzyme_con->BSC_PEP = BSC_PEP;
    Enzyme_con->BSC_ATP = BSC_ATP;
    Enzyme_con->Bchl_OAA = Bchl_OAA;
    Enzyme_con->MC_O2 = MC_O2;
    Enzyme_con->Mchl_O2 = Mchl_O2;
    Enzyme_con->BSC_O2 = BSC_O2;
    Enzyme_con->Bchl_O2 = Bchl_O2;
    Enzyme_con->Bchl_PEP = Bchl_PEP;//////////////WY PPDK in BSCytosol
    Enzyme_con->Mchl_GCEA = Mchl_GCEA;

    Enzyme_con->Bmito_OAA = Bmito_OAA;
    Enzyme_con->Bmito_MAL = Bmito_MAL;
    Enzyme_con->Bmito_PYR = Bmito_PYR;
    Enzyme_con->Bmito_CO2 = Bmito_CO2;
    Enzyme_con->Bmito_NADH = Bmito_NADH;

    Enzyme_con->Bchl_Asp = Bchl_Asp;
    Enzyme_con->Bchl_Ala = Bchl_Ala;
    Enzyme_con->Mchl_Asp = Mchl_Asp;
    Enzyme_con->Mchl_Ala = Mchl_Ala;
    Enzyme_con->E_PPDK_Mchl = E_PPDK_Mchl;// E_PPDK active
    Enzyme_con->EP_PPDK_Mchl = EP_PPDK_Mchl;//EP_PPDK inactive


    //Km Ki(mM)
    //4.2.1.1        1
    const double KmCO2_1 = 2.8;//2.8;
    const double Ke_1 = 11.2;//20;//;// No unit  k=4.3*10^(-7)  PH=7.5 13.6 11.82
    //4.1.1.31       2 //KmHCO3_2 =0.05 KmPEP_2 =0.047
    //
    const double KmPEP_2 = 0.1;
    const double KmHCO3_2 = 0.02;
    const double Kimal_2 = 1;
    //////////WY202003 Maize KmHCO3_2 =0.04216; KmPEP_2 =1;   Kimal_2 =15;
    //KmPEP_2 =0.038;
    //1.1.1.82       3
    const double KmNADPH_3 = 0.024;
    const double KmOAA_3 = 0.056;
    const double KmNADP_3 = 0.073;
    const double Kmmal_3 = 32.0;
    const double Ke_3 = 4450.0; // No unit
    //1.1.1.40       4 Kmmal_4 =0.04;
    const double KmCO2_4 = 1.1;
    const double KmNADP_4 = 0.0080;
    const double KmNADPH_4 = 0.045;
    const double KmPyr_4 = 3.;
    const double Kmmal_4 = 0.23;
    const double Ke_4 = 0.051 * 55.35 * 1000;//0.0344 KmNADP_4 =0.008
    //2.7.9.1        5 KiPEP_5 =0.5;
    const double KiPEP_5 = 0.15;
    const double KmATP_5 = 0.082;
    const double KmPyr_5 = 0.082;
    // KmPyr_5 =0.25
    //4.1.1.39       6  KiPGA_6 =0.84; KiFBP_6 =0.4;
    const double KmCO2_6 = 0.0162;
    const double KmO2_6 = 0.183;
    const double KmRuBP_6 = 0.02;
    const double KiPGA_6 = 2.52;
    const double KiFBP_6 = 0.04;
    const double KiSBP_6 = 0.075;
    const double KiPi_6 = 0.9 * 3;
    const double KiNADPH_6 = 0.07 * 3;
    //KiNADPH_6 =0.07;KiPi_6 =0.9
    //2.7.2.3        7
    const double KmADP_7 = 0.5;
    const double KmATP_7 = 0.3;
    const double KmPGA_7 = 2.4;//1.2;//KmPGA_7 =0.24
    //1.2.1.13       8
    const double KmDPGA_8 = 0.4;
    const double KmNADPH_8 = 0.1;
    //5.3.1.1        9
    const double Ke_9 = 0.05;
    //4.1.2.13FBP    10
    const double KmDHAP_10 = 0.45;
    const double KmFBP_10 = 0.0923;
    const double KmGAP_10 = 0.04;  //KmGAP_10 =0.3;
    const double Ke_10 = 7.1; // 1/millimolarity
    //3.1.3.11       11 KmFBP_11 =0.033; Chloroplast fructose-1,6-bisphosphatase
    //from Oryza differs in salt tolerance property from the Porteresia enzyme
    //and is protected by osmolytes  Ghosh, S.; Bagchi, S.; Lahiri Majumder, A.;Plant Sci. 160, 1171-1181 (2001)
    const double KiF6P_11 = 0.7;
    const double KiPi_11 = 12.0;
    const double KmFBP_11 = 0.066;
    const double Ke_11 = 666000.0;
    //4.1.2.13SBP    12
    const double KmDHAP_12 = 0.4;
    const double KmE4P_12 = 0.2;
    const double Ke_12 = 1.017;  // 1/millimolarity
    //3.1.3.37       13
    const double KiPi_13 = 12.0;
    const double KmSBP_13 = 0.05;
    const double Ke_13 = 666000.0;
    //2.2.1.1X       14 Datta, A.G.; Racker, E.; J. Biol. Chem.; 236, 617 (1961).
    const double KmE4P_14 = 0.1;
    const double KmF6P_14 = 0.1;
    const double KmGAP_14 = 0.1;
    const double KmXu5P = 0.1;
    const double Ke_14 = 0.084;  // No unit Ke_14 =10.0£»Ke_14 =0.076
    //2.2.1.1R       15
    const double KmGAP_15 = 0.072;
    const double KmRi5P_15 = 1.5;
    const double KmS7P_15 = 0.015;
    const double KmXu5P_15 = 0.1;  // KmS7P_15 =0.015  KmS7P_15 =0.46
    const double Ke_15 = 0.9;//1.176470588235294;  // No unit Datta, A.G.; Racker, E.; J. Biol. Chem.; 236, 617 (1961).
    //5.3.1.6:Chl    16
    const double Ke_16 = 0.4;
    //5.1.3.1:Chl    17
    const double Ke_17 = 0.67;
    //2.7.1.19       18 // KiPGA_18 =1.0;
    //const double KiPi_18 =1.0;
    //const double KmATP_18 =0.059;
    //const double KmATP_18=0.625
    const double KiADP_18 = 2.5;
    const double Ki_ADP_18 = 0.4;
    const double KiPGA_18 = 2.0;
    const double KiPi_18 = 4.0;
    const double KiRuBP_18 = 0.7;
    const double KmATP_18 = 0.625;
    const double KmRu5P_18 = 0.05; // KiPi_18 =4.0;
    const double Ke_18 = 6846.0; // No unit

    //2.7.2.3:MChl  7Mchl
    const double KmADP_7Mchl = 0.5;
    const double KmATP_7Mchl = 0.3;
    const double KmPGA_7Mchl = 2.4;
    //1.2.1.13:MChl   8Mchl
    const double KmDPGA_8Mchl = 0.4;
    const double KmNADPH_8Mchl = 0.1;

    //StarchSynthesis:Chl   Starchsyn  KmG1P_Starch =0.24;
    //const double KaF6P_Starch =0.06;
    const double KaF6P_Starch = 0.12;
    //const double KaF6P_Starch = 0.02;
    //const doubleKaFBP_Starch =0.06;
    const double KaFBP_Starch = 0.12;
    //const double KaFBP_Starch =0.02;
    const double KaPGA_Starch = 0.3;// Ka (No unit)KmG1P_Starch =0.08;
    //const double KaPGA_Starch = 0.1;

    const double KiADP_Starch = 10.0;
    const double KmATP_Starch = 0.08;
    const double KmG1P_Starch = 0.48;
    const double Ke_Starch1 = 2.3;
    const double Ke_Starch2 = 0.058;
    //PGASink
    const double KmPGA_PGASink = 1;//0.5;

    //4.1.2.13FBP:Cel    Suc1
    const double KmDHAP_Suc1 = 0.45;
    const double KmGAP_Suc1 = 0.04 / 2;
    //const double KmGAP_Suc1 =0.1;
    const double KmFBP_Suc1 = 0.0023;
    //KmDHAP_Suc1 =0.4;
    //const double KmFBP_Suc1 =0.2;
    const double Ke_Suc1 = 12;//12.0; 10000 // 1/millimolarity
    //3.1.3.11:Cel    Suc2 KiF26BP_Suc2 =7.0E-5;KmFBP_Suc2 =0.165;Ke_Suc2=6663.0
    //Roles of the residues Lys115 and Tyr116 in the binding of an allosteric inhibitor AMP to pea cytosolic D-fructose-1,6-bisphosphatase
    //Jang, H.; Cho, M.; Kwon, Y.; Bhoo, S.H.; Jeon, J.; Hahn, T.; J. Appl. Biol. Chem. 51, 45-49 (2008)
    const double KiF26BP_Suc2 = 0.00007;
    const double KiF6P_Suc2 = 0.7;
    const double KiPi_Suc2 = 12.0;
    const double KmFBP_Suc2 = 0.00108;
    const double Ke_Suc2 = 174.0;
    //5.3.1.9:Cel    Suc5
    const double Ke_Suc5 = 2.3;
    //5.4.2.2:Cel    Suc6
    const double Ke_Suc6 = 0.0584;
    //2.7.7.9:Cel    Suc7
    const double KmG1P_Suc7 = 0.14;
    const double KmPPi_Suc7 = 0.11;
    const double KmUDPG_Suc7 = 0.12;
    const double KmUTP_Suc7 = 0.1;
    const double Ke_Suc7 = 0.31;//0.31; // No unit
    //2.4.1.14:Cel   Suc8
    const double KiFBP_Suc8 = 0.8;
    const double KiPi_Suc8 = 5.0;
    const double KiSuc_Suc8 = 50.0;
    const double KiSucP_Suc8 = 0.4;
    const double KiUDP_Suc8 = 0.7;
    const double KmF6P_Suc8 = 0.8;
    const double KmUDPG_Suc8 = 1.3;
    const double Ke_Suc8 = 10.0;  // No unit
    //3.1.3.24:Cel     Suc9
    const double KmSuc_Suc9 = 80.0;
    const double KmSucP_Suc9 = 0.35;
    const double Ke_Suc9 = 780.0;
    //SUCSink:Cel   Suc10
    const double KmSuc_Suc10 = 1.5;
    //2.7.1.105:Cel     Suc3
    const double KiADP_Suc3 = 0.16;
    const double KIDHAP_Suc3 = 0.7;
    const double KmATP_Suc3 = 1.32;
    const double KmF26BP_Suc3 = 0.021;
    const double KmF6P_Suc3 = 1.4;
    const double Ke_Suc3 = 590.0;// No unit
    //3.1.3.46:Cel    Suc4
    const double KiF6P_Suc4 = 0.1;
    const double KiPi_Suc4 = 0.5 * 10;
    const double KmF26BP_Suc4 = 0.032;
    //3.6.1.1:Cel
    const double KePi = 128.4;
    //3.6.3.14:MChl   ATPM
    const double KmADP_ATPM = 0.014;
    const double KmATP_ATPM = 0.11;
    const double KmPi_ATPM = 0.3;
    const double Ke_ATPM = 5.734;        //1/millimolarity
    //global Xd;//0.667
    const double X = 0.667;
    const double Y = 0.6;
    const double F = 0.7225;
    const double Q = 0.9;
    const double D = 1;// No unit
    //1.18.1.2:MChl  NADPHM
    const double KmNADP_NADPHM = 0.05;
    const double KmNADPH_NADPHM = 0.058; // KmNADP_NADPHM =0.0072;
    //const double KmNADPH_NADPHM =0.0058;
    const double Ke_NADPHM = 502;
    const double E = 0.5; // No unit
    //V3.6.3.14:Chl       ATPB
    const double KmADP_ATPB = 0.014;
    const double KmPi_ATPB = 0.11;
    const double KmATP_ATPB = 0.3;
    const double Ke_ATPB = 5.734; // 1/millimolarity
    const double G = 0.667; // No unit
    //1.18.1.2:BChl  NADPHB
    const double KmNADP_NADPHB = 0.05;
    const double KmNADPH_NADPHB = 0.058;
    const double Ke_NADPHB = 502;  // No unit

    //4.1.1.39 O2 1
    //KmCO2_6 =0.01935;
    //const double KmO2_6 =0.222;
    //const double KmRuBP_6 =0.02;
    //const double KiPGA_6 =2.52;
    //const double KiFBP_6 =0.8;
    //const double KiSBP_6 =0.75;
    //const double KiPi_6 =0.9;
    //const double KiNADPH_6 =0.07;
    const double KmCO2_PR1 = 0.0162;
    const double KmO2_PR1 = 0.183;
    const double KmRuBP_PR1 = 0.02;
    const double KiPGA_PR1 = 2.52;
    const double KiFBP_PR1 = 0.04;
    const double KiSBP_PR1 = 0.75;
    const double KiPi_PR1 = 0.9 * 3;
    const double KiNADPH_PR1 = 0.21;
    //3.1.3.18 2
    const double KmPGCA_PR2 = 0.026;
    const double KiPI_PR2 = 2.55;
    const double KiGCA_PR2 = 94.0;//KmPGCA_PR2=0.026; 0.57
    //1.1.3.15 3
    const double KmGCA_PR3 = 0.1;//KmGCA_PR3= 0.1;0.02
    //2.6.1.4 4
    const double Ke_PS4 = 607.0;
    const double KmGOA_PS4 = 0.15;
    const double KmGLU_PS4 = 1.7;
    const double KiGLY_PS4 = 2.0;
    //GLY_SER:Mit 5
    const double KmGLY_PS5 = 6.0;
    const double KiSER_PS5 = 4.0;
    //2.6.1.45 6
    const double Ke_PR6 = 0.24;
    const double KmGOA_PR6 = 0.15;
    const double KmSER_PR6 = 2.7;
    const double KmGLY_PR6 = 33.0;
    //1.1.1.29 7
    const double Ke_PR7 = 250000.0;
    const double KiHPR_PR7 = 12.0;
    const double KmHPR_PR7 = 0.09;
    //2.7.1.31 8
    const double Ke_PR8 = 300.0;
    const double KmATP_PR8 = 0.21;
    //const double KmGCEA_PR8 = 0.25;
    const double KiPGA_PR8 = 0.72;
    //KmATP_PR8= 0.21;
    const double KmGCEA_PR8=0.25;
    //const double KiPGA_PR8=0.36;
    //Tgca 9
    const double KmGCA_PR9 = 0.2;
    const double KiGCEA_PR9 = 0.22;
    //Tgcea 10
    const double KmGCEA_PR10 = 0.39;
    const double KiGCA_PR10 = 0.28;

    // Transport coeffcient (1/second)
    const double Voaa = 1.5;
    const double Vmal = 1.5;
    const double Vpyr = 1.5;
    const double Vpep = 1.5;
    const double Vt = 1.5;
    const double Vleak = 1;
    const double Vpga = 2;

    const double KmPGA_62 = 0.08;
    const double KmPEP_62 = 0.3;
    const double Ke_62 = 0.4302;//G66 = +0.5;

    KValue.clear();
    for (int i = 0; i < 48; i++) {
        KValue.push_back(std::vector<double>(10, 0.));
    }


    //global KValue;
    // KValue = zeros(48, 10);
    KValue[0][0] = KmCO2_1;
    KValue[0][1] = Ke_1;
    KValue[1][0] = KmHCO3_2;
    KValue[1][1] = KmPEP_2;
    KValue[1][2] = Kimal_2;
    KValue[2][0] = KmNADPH_3;
    KValue[2][1] = KmOAA_3;
    KValue[2][2] = KmNADP_3;
    KValue[2][3] = Kmmal_3;
    KValue[2][4] = Ke_3;
    KValue[3][0] = KmCO2_4;
    KValue[3][1] = KmNADP_4;
    KValue[3][2] = KmNADPH_4;
    KValue[3][3] = KmPyr_4;
    KValue[3][4] = Kmmal_4;
    KValue[3][5] = Ke_4;
    KValue[4][0] = KiPEP_5;
    KValue[4][1] = KmATP_5;
    KValue[4][2] = KmPyr_5;
    KValue[5][0] = KmCO2_6;
    KValue[5][1] = KmO2_6;
    KValue[5][2] = KmRuBP_6;
    KValue[5][3] = KiPGA_6;
    KValue[5][4] = KiFBP_6;
    KValue[5][5] = KiSBP_6;
    KValue[5][6] = KiPi_6;
    KValue[5][7] = KiNADPH_6;
    KValue[6][0] = KmADP_7;
    KValue[6][1] = KmATP_7;
    KValue[6][2] = KmPGA_7;
    KValue[7][0] = KmDPGA_8;
    KValue[7][1] = KmNADPH_8;
    KValue[8][0] = Ke_9;
    KValue[9][0] = KmDHAP_10;
    KValue[9][1] = KmFBP_10;
    KValue[9][2] = KmGAP_10;
    KValue[9][3] = Ke_10;
    KValue[10][0] = KiF6P_11;
    KValue[10][1] = KiPi_11;
    KValue[10][2] = KmFBP_11;
    KValue[10][3] = Ke_11;
    KValue[11][0] = KmDHAP_12;
    KValue[11][1] = KmE4P_12;
    KValue[11][2] = Ke_12;
    KValue[12][0] = KiPi_13;
    KValue[12][1] = KmSBP_13;
    KValue[12][2] = Ke_13;
    KValue[13][0] = KmE4P_14;
    KValue[13][1] = KmF6P_14;
    KValue[13][2] = KmGAP_14;
    KValue[13][3] = KmXu5P;
    KValue[13][4] = Ke_14;
    KValue[14][0] = KmGAP_15;
    KValue[14][1] = KmRi5P_15;
    KValue[14][2] = KmS7P_15;
    KValue[14][3] = KmXu5P_15;
    KValue[14][4] = Ke_15;
    KValue[15][0] = Ke_16;
    KValue[16][0] = Ke_17;
    KValue[17][0] = KiADP_18;
    KValue[17][1] = Ki_ADP_18;
    KValue[17][2] = KiPGA_18;
    KValue[17][3] = KiPi_18;
    KValue[17][4] = KiRuBP_18;
    KValue[17][5] = KmATP_18;
    KValue[17][6] = KmRu5P_18;
    KValue[17][7] = Ke_18;

    KValue[18][0] = KmADP_7Mchl;
    KValue[18][1] = KmATP_7Mchl;
    KValue[18][2] = KmPGA_7Mchl;
    KValue[19][0] = KmDPGA_8Mchl;
    KValue[19][0] = KmNADPH_8Mchl;

    KValue[20][0] = KiADP_Starch;
    KValue[20][1] = KmATP_Starch;
    KValue[20][2] = KmG1P_Starch;
    KValue[20][3] = KaF6P_Starch;
    KValue[20][4] = KaFBP_Starch;
    KValue[20][5] = KaPGA_Starch;
    KValue[20][6] = Ke_Starch1;
    KValue[20][7] = Ke_Starch2;
    KValue[21][0] = KmPGA_PGASink;
    KValue[22][0] = KmDHAP_Suc1;
    KValue[22][1] = KmGAP_Suc1;
    KValue[22][2] = KmFBP_Suc1;
    KValue[22][3] = Ke_Suc1;
    KValue[23][0] = KiF26BP_Suc2;
    KValue[23][1] = KiF6P_Suc2;
    KValue[23][2] = KiPi_Suc2;
    KValue[23][3] = KmFBP_Suc2;
    KValue[23][4] = Ke_Suc2;
    KValue[24][0] = Ke_Suc5;
    KValue[24][1] = Ke_Suc6;
    KValue[25][0] = KmG1P_Suc7;
    KValue[25][1] = KmPPi_Suc7;
    KValue[25][2] = KmUDPG_Suc7;
    KValue[25][3] = KmUTP_Suc7;
    KValue[25][4] = Ke_Suc7;
    KValue[26][0] = KiFBP_Suc8;
    KValue[26][1] = KiPi_Suc8;
    KValue[26][2] = KiSuc_Suc8;
    KValue[26][3] = KiSucP_Suc8;
    KValue[26][4] = KiUDP_Suc8;
    KValue[26][5] = KmF6P_Suc8;
    KValue[26][6] = KmUDPG_Suc8;
    KValue[26][7] = Ke_Suc8;
    KValue[27][0] = KmSuc_Suc9;
    KValue[27][1] = KmSucP_Suc9;
    KValue[27][2] = Ke_Suc9;
    KValue[28][0] = KmSuc_Suc10;
    KValue[29][0] = KiADP_Suc3;
    KValue[29][1] = KIDHAP_Suc3;
    KValue[29][2] = KmATP_Suc3;
    KValue[29][3] = KmF26BP_Suc3;
    KValue[29][4] = KmF6P_Suc3;
    KValue[29][5] = Ke_Suc3;
    KValue[30][0] = KiF6P_Suc4;
    KValue[30][1] = KiPi_Suc4;
    KValue[30][2] = KmF26BP_Suc4;
    KValue[35][0] = KePi;

    KValue[31][0] = KmADP_ATPM;
    KValue[31][1] = KmATP_ATPM;
    KValue[31][2] = KmPi_ATPM;
    KValue[31][3] = X;
    KValue[31][4] = Y;
    KValue[31][5] = F;
    KValue[31][6] = Q;
    KValue[31][7] = D;
    KValue[31][8] = Ke_ATPM;
    KValue[32][0] = KmNADP_NADPHM;
    KValue[32][1] = KmNADPH_NADPHM;
    KValue[32][2] = Ke_NADPHM;
    KValue[32][3] = E;
    KValue[33][0] = KmADP_ATPB;
    KValue[33][1] = KmPi_ATPB;
    KValue[33][2] = KmATP_ATPB;
    KValue[33][3] = Ke_ATPB;
    KValue[33][4] = G;
    KValue[36][0] = KmNADP_NADPHB;
    KValue[36][1] = KmNADPH_NADPHB;
    KValue[36][2] = Ke_NADPHB;

    KValue[34][0] = Voaa;
    KValue[34][1] = Vmal;
    KValue[34][2] = Vpyr;
    KValue[34][3] = Vpep;
    KValue[34][4] = Vt;
    KValue[34][5] = Vleak;
    KValue[34][6] = Vpga;


    KValue[37][0] = KmCO2_PR1;
    KValue[37][1] = KmO2_PR1;
    KValue[37][2] = KmRuBP_PR1;
    KValue[37][3] = KiPGA_PR1;
    KValue[37][4] = KiFBP_PR1;
    KValue[37][5] = KiSBP_PR1;
    KValue[37][6] = KiPi_PR1;
    KValue[37][7] = KiNADPH_PR1;
    KValue[38][0] = KmPGCA_PR2;
    KValue[38][1] = KiPI_PR2;
    KValue[38][2] = KiGCA_PR2;
    KValue[39][0] = KmGCA_PR3;
    KValue[40][0] = Ke_PS4;
    KValue[40][1] = KmGOA_PS4;
    KValue[40][2] = KmGLU_PS4;
    KValue[40][3] = KiGLY_PS4;
    KValue[41][0] = KmGLY_PS5;
    KValue[41][1] = KiSER_PS5;
    KValue[42][0] = Ke_PR6;
    KValue[42][1] = KmGOA_PR6;
    KValue[42][2] = KmSER_PR6;
    KValue[42][3] = KmGLY_PR6;
    KValue[43][0] = Ke_PR7;
    KValue[43][1] = KiHPR_PR7;
    KValue[43][2] = KmHPR_PR7;
    KValue[44][0] = Ke_PR8;
    KValue[44][1] = KmATP_PR8;
    KValue[44][2] = KmGCEA_PR8;
    KValue[44][3] = KiPGA_PR8;
    KValue[45][0] = KmGCA_PR9;
    KValue[45][1] = KiGCEA_PR9;
    KValue[46][0] = KmGCEA_PR10;
    KValue[46][1] = KiGCA_PR10;

    KValue[47][0] = KmPGA_62;
    KValue[47][1] = KmPEP_62;
    KValue[47][2] = Ke_62;
    //mM/(L*s)

    ////////////////////////////////////////////////////////////////////////////
    //Temp correction
    const double Ea_Vpmax = 94.8 * 1000;
    const double dS_Vpmax = 0.25 * 1000;
    const double Hd_Vpmax = 73.3 * 1000;
    const double Ea_Vcmax = 78 * 1000;
    const double Ea_PPDK = 58.1 * 1000;
    const double R = 8.3144598;// m2 kg s-2 K-1 mol-1
    const double MTempCorr_V2 = exp(Ea_Vpmax * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15))) * (1 + exp((298.15 * dS_Vpmax - Hd_Vpmax) / (298.15 * R))) / (1 + exp(((theVars->Tp + 273.15) * dS_Vpmax - Hd_Vpmax) / ((theVars->Tp + 273.15) * R)));
    const double MTempCorr_V6 = exp(Ea_Vcmax * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15)));
    const double MTempCorr_V5 = exp(Ea_PPDK * ((theVars->Tp + 273.15) - 298.15) / (298.15 * R * (theVars->Tp + 273.15)));
    const double Vpmax_25 = theVars->Vpmax / MTempCorr_V2;
    const double Vcmax_25 = theVars->Vcmax / MTempCorr_V6;
    const double Vppdkmax_25 = theVars->Vcmax / MTempCorr_V5;
    const double Vm_2 = Vpmax_25 / 1000 / theVars->FactorVP;//3.6;//4;
    const double Vm_1 = 200;
    const double Vm_6o = 0.065;//2.4;//1.8;//2.9;//3.4967170968;2.913930914 2.09803025808
    const double Vppdkmax_E = Vppdkmax_25 / 1000;
    const double Vm_6 = Vcmax_25 / 1000 / theVars->FactorVC;
    const double Vm_3 = 1.8 * Vppdkmax_E * VMDHsen;///2.4*Vm_2;///1.2;
    const double Vm_4 = 1.33 * Vppdkmax_E * VMEsen;///2.4*Vm_2;///1.5;//2.4;//3.2
    const double Vm_5 = 1.33 * Vppdkmax_E * VPPDKsen;///2.4*Vm_2;//*1.5;//2.4; WY 201902
    const double Vm_78 = 0.4 * Vm_6 / Vm_6o * VGAPDHsen;//Vm_7 = 30.15;//16.07510272;// 30.15;//16.07510272
    const double Vm_8 = 0;//4.308781695;
    const double Vm_10 = 0.0731 * Vm_6 / Vm_6o;//2.9253370968*1.5;//1.4626685484;
    const double Vm_11 = 0.0436 * Vm_6 / Vm_6o * VFBPsen;
    const double Vm_12 = 0.1097 * Vm_6 / Vm_6o;
    const double Vm_13 = 0.0292 * Vm_6 / Vm_6o * VSBPsen;
    const double Vm_14 = 0.2810 * Vm_6 / Vm_6o;
    const double Vm_15 = 0.2810 * Vm_6 / Vm_6o;
    const double Vm_18 = 1.7552 * Vm_6 / Vm_6o * VPRKsen;


    const double Vm_78Mchl = 0.3 * Vm_6 / Vm_6o * VGAPDHsen;//Vm_7Mchl = 15.1;
    const double Vm_8Mchl = 0;//2.6929885593*Vm_6/Vm_6o;
    const double Vm_Starch = 0.0133 * Vm_6 / Vm_6o;
    const double Vm_Sta1 = 0.03 * Vm_6 / Vm_6o;
    const double Vm_Sta2 = 1 * Vm_6 / Vm_6o;
    const double Vm_Sta3 = 0.025 * Vm_6 / Vm_6o;

    const double Vm_PGASink = 0.002 * Vm_6 / Vm_6o;//0.5/5;
    const double Vm_Suc1 = 0.0081 * Vm_6 / Vm_6o;
    const double Vm_Suc2 = 0.0064 * Vm_6 / Vm_6o;
    const double Vm_Suc7 = 0.0058 * Vm_6 / Vm_6o;
    const double Vm_Suc8 = 0.0278 * Vm_6 / Vm_6o;
    const double Vm_Suc9 = 0.0278 * Vm_6 / Vm_6o;
    const double Vm_Suc10 = 0.0035 * Vm_6 / Vm_6o;//2.0
    const double Vm_Suc3 = 0.0010 * Vm_6 / Vm_6o;
    const double Vm_Suc4 = 8.4096e-4 * Vm_6 / Vm_6o;

    const double Jmax = 0.5 * Vm_6 / Vm_6o * Jmaxsen;// 20;
    const double Vm_ATPM = 0.3 * Vm_6 / Vm_6o;
    const double Vm_NADPHM = 0.2 * Vm_6 / Vm_6o;
    const double Vm_ATPB = 0.3 * Vm_6 / Vm_6o;
    const double Vm_NADPHB = 0.2 * Vm_6 / Vm_6o;

    const double Vm_PR1 = Vm_6 * 0.11;//0.69934341936;(Cousins 2010 0.11)
    const double Vm_PR2 = 2.6210 * Vm_6 / Vm_6o;
    const double Vm_PR3 = 0.0728 * Vm_6 / Vm_6o;
    const double Vm_PR4 = 0.1373 * Vm_6 / Vm_6o;
    const double Vm_PR5 = 0.1247 * Vm_6 / Vm_6o;
    const double Vm_PR6 = 0.1653 * Vm_6 / Vm_6o;
    const double Vm_PR7 = 0.5005 * Vm_6 / Vm_6o;
    const double Vm_PR8 = 0.2858 * Vm_6 / Vm_6o;
    const double VTgca_PR9 = 0.3 * Vm_6 / Vm_6o;
    const double VTgcea_PR10 = 0.25 * Vm_6 / Vm_6o;

    const double Vm_62 = 0.001 * Vexsen;//mM/s //1.45 E-5;

    const double Vm_OAA_M = 0.08 * Vm_6 / Vm_6o;
    const double Vm_PYR_B = 0.15 * Vm_6 / Vm_6o;
    const double Vm_PYR_M = 0.15 * Vm_6 / Vm_6o;
    const double Vm_PEP_M = 0.15 * Vm_6 / Vm_6o;
    const double Vtp_Bchl = 0.75;
    const double Vtp_Mchl = 0.75;

    // transport between two cell types
    //global phi;
    //global Lpd;

    const double Pmal = 0.0421 * (phi / 0.03) / (Lpd / 400.);
    const double Ppyr = 0.0436 * (phi / 0.03) / (Lpd / 400.);
    const double Pco2 = 0.1139 * (phi / 0.03) / (Lpd / 400.);
    const double PC3P = 0.0327;
    const double Pc3p = PC3P * (phi / 0.03) / (Lpd / 400.);
    const double Pco2_B = 0.4;////PCO2_B=0.002 cm s-1 SChl/Sl =10

    //global MRd;
    //global vrpd;
    theVars->vrpd = theVars->MRd / 2 / 1000;//0.0005;

    //global Velocity_s;
    // Velocity_s = zeros(58, 1);
    theVars->Velocity_s[0] = Vm_1;
    theVars->Velocity_s[1] = Vm_2;
    theVars->Velocity_s[2] = Vm_3;
    theVars->Velocity_s[3] = Vm_4;
    theVars->Velocity_s[4] = Vm_5;
    theVars->Velocity_s[5] = Vm_6;
    theVars->Velocity_s[6] = Vm_78;
    theVars->Velocity_s[7] = Vm_8;
    theVars->Velocity_s[8] = Vm_10;
    theVars->Velocity_s[9] = Vm_11;
    theVars->Velocity_s[10] = Vm_12;
    theVars->Velocity_s[11] = Vm_13;
    theVars->Velocity_s[12] = Vm_14;
    theVars->Velocity_s[13] = Vm_15;
    theVars->Velocity_s[14] = Vm_18;
    theVars->Velocity_s[15] = Vm_78Mchl;
    theVars->Velocity_s[16] = Vm_8Mchl;
    theVars->Velocity_s[17] = Vm_Starch;
    theVars->Velocity_s[18] = Vm_PGASink;
    theVars->Velocity_s[19] = Vm_Suc1;
    theVars->Velocity_s[20] = Vm_Suc2;
    theVars->Velocity_s[21] = Vm_Suc7;
    theVars->Velocity_s[22] = Vm_Suc8;
    theVars->Velocity_s[23] = Vm_Suc9;
    theVars->Velocity_s[24] = Vm_Suc10;
    theVars->Velocity_s[25] = Vm_Suc3;
    theVars->Velocity_s[26] = Vm_Suc4;
    theVars->Velocity_s[27] = 0; //theVars->TestLi * CONVERT/1000;
    theVars->Velocity_s[28] = Jmax;
    theVars->Velocity_s[29] = Vm_ATPM;
    theVars->Velocity_s[30] = Vm_NADPHM;
    theVars->Velocity_s[31] = Vm_ATPB;
    theVars->Velocity_s[32] = Vm_NADPHB;
    theVars->Velocity_s[33] = Vm_PR1;
    theVars->Velocity_s[34] = Vm_PR2;
    theVars->Velocity_s[35] = Vm_PR3;
    theVars->Velocity_s[36] = Vm_PR4;
    theVars->Velocity_s[37] = Vm_PR5;
    theVars->Velocity_s[38] = Vm_PR6;
    theVars->Velocity_s[39] = Vm_PR7;
    theVars->Velocity_s[40] = Vm_PR8;
    theVars->Velocity_s[41] = VTgca_PR9;
    theVars->Velocity_s[42] = VTgcea_PR10;
    theVars->Velocity_s[43] = Vm_62;
    theVars->Velocity_s[44] = Vtp_Bchl;
    theVars->Velocity_s[45] = Vtp_Mchl;
    theVars->Velocity_s[46] = Vm_Sta1;
    theVars->Velocity_s[47] = Vm_Sta2;
    theVars->Velocity_s[48] = Vm_Sta3;
    theVars->Velocity_s[49] = Vm_OAA_M;
    theVars->Velocity_s[50] = Vm_PYR_B;
    theVars->Velocity_s[51] = Vm_PYR_M;
    theVars->Velocity_s[52] = Vm_PEP_M;
    theVars->Velocity_s[53] = Pmal;
    theVars->Velocity_s[54] = Ppyr;
    theVars->Velocity_s[55] = Pco2;
    theVars->Velocity_s[56] = Pc3p;
    theVars->Velocity_s[57] = Pco2_B;


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

    KValue.clear();
}
