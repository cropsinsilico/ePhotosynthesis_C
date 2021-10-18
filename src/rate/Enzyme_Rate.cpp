#include <math.h>
#include "Variables.hpp"
#include "modules/Enzyme.hpp"
#include "conditions/EnzymeCondition.hpp"
#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double Mchl_CP = 15.0;
const double MC_CP = 15.0;
const double RatioPPDK = 0.; //PDK in BSC
//const double MC_UTP = 1.26; //0.75
const double U = 0.; // light partition coefficient
//const double G = 0.667; // No unit
const double V = 0.; // light partition coefficient
const bool Pvpr8M = false; // if 0 Glycerate kinase in BSchl; if 1 Glycerate kinase in Mchl;
const double Ratio = 4; // Enezyme activity variation factor
const bool RedoxEnyAct = true; //if RedoxEnyAct=0 activities of photosynthetic enzymes are not regulated by light intensity; %%if RedoxEnyAct=1 include light induced enzyme activation
const bool PRac = true;
const double gm = 3.;
const double Sc = 3. * pow(10, 4); //3.36*10^4;%ubarL/mmolubarL/mmol

void Enzyme::_Rate(const double t, const conditions::EnzymeCondition *const Enzyme_con, Variables *theVars) {
    const double I = theVars->TestLi * CONVERT / 1000 * 0.85;

    //TempFactor=TempResponseEnzymes(WeatherTemperature);
    const double TempCorr_V1 = theVars->TempFactor[0];
    const double TempCorr_V2 = theVars->TempFactor[1];
    const double TempCorr_V5 = theVars->TempFactor[2];
    const double TempCorr_V6 = theVars->TempFactor[3];
    const double TempCorr_KmCO2_6 = theVars->TempFactor[4];
    const double TempCorr_KmO2_6 = theVars->TempFactor[5];
    const double TempCorr_Vm_OC = theVars->TempFactor[6];
    const double TempCorr_Jmax = theVars->TempFactor[7];
    const double TempCorr_Vm_Enz = theVars->TempFactor[8];

    // structure to store the Michaelis-Menten kinetic parameters
    const double KmCO2_1 = KValue[0][0];
    const double Ke_1 = KValue[0][1];
    const double KmHCO3_2 = KValue[1][0];
    const double KmPEP_2 = KValue[1][1];
    //double Kimal_2 = KValue[1][2];
    const double KmNADPH_3 = KValue[2][0];
    const double KmOAA_3 = KValue[2][1];
    const double KmNADP_3 = KValue[2][2];
    const double Kmmal_3 = KValue[2][3];
    const double Ke_3 = KValue[2][4];
    const double KmCO2_4 = KValue[3][0];
    const double KmNADP_4 = KValue[3][1];
    const double KmNADPH_4 = KValue[3][2];
    const double KmPyr_4 = KValue[3][3];
    const double Kmmal_4 = KValue[3][4];
    const double Ke_4 = KValue[3][5];
    const double KiPEP_5 = KValue[4][0];
    const double KmATP_5 = KValue[4][1];
    const double KmPyr_5 = KValue[4][2];
    const double KmCO2_6 = KValue[5][0] * TempCorr_KmCO2_6;
    const double KmO2_6 = KValue[5][1] * TempCorr_KmO2_6;
    const double KmRuBP_6 = KValue[5][2];
    const double KiPGA_6 = KValue[5][3];
    const double KiFBP_6 = KValue[5][4];
    const double KiSBP_6 = KValue[5][5];
    const double KiPi_6 = KValue[5][6];
    //const double KiNADPH_6 = KValue[5][7];
    // KmADP_7 = KValue[6][0];  KmATP_7 = KValue[6][1];  KmPGA_7 = KValue[6][2];// --unused
    // KmDPGA_8 = KValue[7][0];  KmNADPH_8 = KValue[7][1];// --unused
    const double Ke_9 = KValue[8][0];
    const double KmDHAP_10 = KValue[9][0];
    const double KmFBP_10 = KValue[9][1];
    const double KmGAP_10 = KValue[9][2];
    const double Ke_10 = KValue[9][3];
    const double KiF6P_11 = KValue[10][0];
    const double KiPi_11 = KValue[10][1];
    const double KmFBP_11 = KValue[10][2];
    const double Ke_11 = KValue[10][3];
    const double KmDHAP_12 = KValue[11][0];
    const double KmE4P_12 = KValue[11][1];
    const double Ke_12 = KValue[11][2];
    const double KiPi_13 = KValue[12][0];
    const double KmSBP_13 = KValue[12][1];
    const double Ke_13 = KValue[12][2];
    const double KmE4P_14 = KValue[13][0];
    const double KmF6P_14 = KValue[13][1];
    const double KmGAP_14 = KValue[13][2];
    const double KmXu5P = KValue[13][3];
    const double Ke_14 = KValue[13][4];
    const double KmGAP_15 = KValue[14][0];
    const double KmRi5P_15 = KValue[14][1];
    const double KmS7P_15 = KValue[14][2];
    const double KmXu5P_15 = KValue[14][3];
    const double Ke_15 = KValue[14][4];
    const double Ke_16 = KValue[15][0];
    const double Ke_17 = KValue[16][0];
    const double KiADP_18 = KValue[17][0];
    const double Ki_ADP_18 = KValue[17][1];
    const double KiPGA_18 = KValue[17][2];
    const double KiPi_18 = KValue[17][3];
    const double KiRuBP_18 = KValue[17][4];
    const double KmATP_18 = KValue[17][5];
    const double KmRu5P_18 = KValue[17][6];
    const double Ke_18 = KValue[17][7];

    // KmADP_7Mchl = KValue[18][0];  KmATP_7Mchl = KValue[18][1];  KmPGA_7Mchl = KValue[18][2];// --unused
    // KmDPGA_8Mchl = KValue[19][0];  KmNADPH_8Mchl = KValue[19][0];// --unused

    const double KiADP_Starch = KValue[20][0];
    const double KmATP_Starch = KValue[20][1];
    const double KmG1P_Starch = KValue[20][2];
    const double KaF6P_Starch = KValue[20][3];
    const double KaFBP_Starch = KValue[20][4];
    const double KaPGA_Starch = KValue[20][5];
    const double Ke_Starch1 = KValue[20][6];
    const double  Ke_Starch2 = KValue[20][7];
    const double KmPGA_PGASink = KValue[21][0];
    const double KmDHAP_Suc1 = KValue[22][0];
    const double KmGAP_Suc1 = KValue[22][1];
    const double KmFBP_Suc1 = KValue[22][2];
    const double Ke_Suc1 = KValue[22][3];
    const double KiF26BP_Suc2 = KValue[23][0];
    const double KiF6P_Suc2 = KValue[23][1];
    const double KiPi_Suc2 = KValue[23][2];
    const double KmFBP_Suc2 = KValue[23][3];
    const double Ke_Suc2 = KValue[23][4];
    const double Ke_Suc5 = KValue[24][0];
    const double Ke_Suc6 = KValue[24][1];
    const double KmG1P_Suc7 = KValue[25][0];
    const double KmPPi_Suc7 = KValue[25][1];
    const double KmUDPG_Suc7 = KValue[25][2];
    const double KmUTP_Suc7 = KValue[25][3];
    const double Ke_Suc7 = KValue[25][4];
    const double KiFBP_Suc8 = KValue[26][0];
    const double KiPi_Suc8 = KValue[26][1];
    const double KiSuc_Suc8 = KValue[26][2];
    const double KiSucP_Suc8 = KValue[26][3];
    const double KiUDP_Suc8 = KValue[26][4];
    const double KmF6P_Suc8 = KValue[26][5];
    const double KmUDPG_Suc8 = KValue[26][6];
    const double Ke_Suc8 = KValue[26][7];
    const double KmSuc_Suc9 = KValue[27][0];
    const double KmSucP_Suc9 = KValue[27][1];
    const double Ke_Suc9 = KValue[27][2];
    const double KmSuc_Suc10 = KValue[28][0];
    const double KiADP_Suc3 = KValue[29][0];
    const double KIDHAP_Suc3 = KValue[29][1];
    const double KmATP_Suc3 = KValue[29][2];
    const double KmF26BP_Suc3 = KValue[29][3];
    const double KmF6P_Suc3 = KValue[29][4];
    const double Ke_Suc3 = KValue[29][5];
    const double KiF6P_Suc4 = KValue[30][0];
    const double KiPi_Suc4 = KValue[30][1];
    const double KmF26BP_Suc4 = KValue[30][2];
    const double KePi = KValue[35][0];
    const double KmADP_ATPM = KValue[31][0];
    const double KmATP_ATPM = KValue[31][1];
    const double KmPi_ATPM = KValue[31][2];
    const double X = KValue[31][3];
    const double Y = KValue[31][4];
    const double F = KValue[31][5];
    const double Q = KValue[31][6];
    const double D = KValue[31][7];
    const double Ke_ATPM = KValue[31][8];
    const double KmNADP_NADPHM = KValue[32][0];
    const double KmNADPH_NADPHM = KValue[32][1];
    const double Ke_NADPHM = KValue[32][2];
    const double E = KValue[32][3];
    const double KmADP_ATPB = KValue[33][0];
    const double KmPi_ATPB = KValue[33][1];
    const double  KmATP_ATPB = KValue[33][2];
    const double Ke_ATPB = KValue[33][3];
    const double  G = KValue[33][4];
    const double KmNADP_NADPHB = KValue[36][0];
    const double KmNADPH_NADPHB = KValue[36][1];
    const double Ke_NADPHB = KValue[36][2];
    // Voaa = KValue[34][0];  Vmal = KValue[34][1];  Vpyr = KValue[34][2];  Vpep = KValue[34][3];  Vt = KValue[34][4];  Vleak = KValue[34][5]; Vpga = KValue[34][6];// --unused
    const double KmCO2_PR1 = KValue[37][0] * TempCorr_KmCO2_6;
    const double KmO2_PR1 = KValue[37][1] * TempCorr_KmO2_6;
    const double KmRuBP_PR1 = KValue[37][2];
    const double KiPGA_PR1 = KValue[37][3];
    const double KiFBP_PR1 = KValue[37][4];
    const double KiSBP_PR1 = KValue[37][5];
    const double KiPi_PR1 = KValue[37][6];
    const double KiNADPH_PR1 = KValue[37][7];
    const double KmPGCA_PR2 = KValue[38][0];
    const double KiPI_PR2 = KValue[38][1];
    const double KiGCA_PR2 = KValue[38][2];
    const double KmGCA_PR3 = KValue[39][0];
    const double Ke_PS4 = KValue[40][0];
    const double KmGOA_PS4 = KValue[40][1];
    const double KmGLU_PS4 = KValue[40][2];
    const double KiGLY_PS4 = KValue[40][3];
    const double KmGLY_PS5 = KValue[41][0];
    const double KiSER_PS5 = KValue[41][1];
    const double Ke_PR6 = KValue[42][0];
    const double KmGOA_PR6 = KValue[42][1];
    const double KmSER_PR6 = KValue[42][2];
    const double KmGLY_PR6 = KValue[42][3];
    const double Ke_PR7 = KValue[43][0];
    const double KiHPR_PR7 = KValue[43][1];
    const double KmHPR_PR7 = KValue[43][2];
    const double Ke_PR8 = KValue[44][0];
    const double KmATP_PR8 = KValue[44][1];
    const double KmGCEA_PR8 = KValue[44][2];
    const double KiPGA_PR8 = KValue[44][3];
    const double KmGCA_PR9 = KValue[45][0];
    const double KiGCEA_PR9 = KValue[45][1];
    const double KmGCEA_PR10 = KValue[46][0];
    const double KiGCA_PR10 = KValue[46][1];
    const double KmPGA_62 = KValue[47][0];
    const double KmPEP_62 = KValue[47][1];
    const double Ke_62 = KValue[47][2];


    // MC_Sucrose = s(15 + 7);// --unused
    // Bchl_Starch = s(40 + 7);// --unused
    // Bchl_Rubisco = s(41 + 7);// --unused

    // Bmito_CO2 = s(80 + 7);// --unused
    //double Bchl_Asp = Enzyme_con->Bchl_Asp;
    //double Bchl_Ala = Enzyme_con->Bchl_Ala;
    //double Mchl_Asp = Enzyme_con->Mchl_Asp;
    //double Mchl_Ala = Enzyme_con->Mchl_Ala;

    //global Velocity_s;

    const double Vm_1 = theVars->Velocity_s[0] * TempCorr_V1;
    double Vm_2 = theVars->Velocity_s[1] * TempCorr_V2;
    double Vm_3 = theVars->Velocity_s[2] * TempCorr_Vm_Enz;
    const double Vm_4 = theVars->Velocity_s[3] * TempCorr_Vm_Enz;
    double Vm_5 = theVars->Velocity_s[4] * TempCorr_V5;
    double Vm_6 = theVars->Velocity_s[5] * TempCorr_V6;
    double Vm_78 = theVars->Velocity_s[6] * TempCorr_Vm_Enz;
    const double Vm_10 = theVars->Velocity_s[8] * TempCorr_Vm_Enz;
    double Vm_11 = theVars->Velocity_s[9] * TempCorr_Vm_Enz;
    const double Vm_12 = theVars->Velocity_s[10] * TempCorr_Vm_Enz;
    double Vm_13 = theVars->Velocity_s[11] * TempCorr_Vm_Enz;
    const double Vm_14 = theVars->Velocity_s[12] * TempCorr_Vm_Enz;
    const double Vm_15 = theVars->Velocity_s[13] * TempCorr_Vm_Enz;
    double Vm_18 = theVars->Velocity_s[14] * TempCorr_Vm_Enz;
    double Vm_78Mchl = theVars->Velocity_s[15] * TempCorr_Vm_Enz;
    const double Vm_Starch = theVars->Velocity_s[17] * TempCorr_Vm_Enz;
    const double Vm_PGASink = theVars->Velocity_s[18] * TempCorr_Vm_Enz;
    const double Vm_Suc1 = theVars->Velocity_s[19] * TempCorr_Vm_Enz;
    const double Vm_Suc2 = theVars->Velocity_s[20] * TempCorr_Vm_Enz;
    const double Vm_Suc7 = theVars->Velocity_s[21] * TempCorr_Vm_Enz;
    const double Vm_Suc8 = theVars->Velocity_s[22] * TempCorr_Vm_Enz;
    const double Vm_Suc9 = theVars->Velocity_s[23] * TempCorr_Vm_Enz;
    const double Vm_Suc10 = theVars->Velocity_s[24] * TempCorr_Vm_Enz;
    const double Vm_Suc3 = theVars->Velocity_s[25] * TempCorr_Vm_Enz;
    const double Vm_Suc4 = theVars->Velocity_s[26] * TempCorr_Vm_Enz;
    const double Jmax = theVars->Velocity_s[28] * TempCorr_Jmax;
    double Vm_ATPM = theVars->Velocity_s[29] * TempCorr_Vm_Enz;
    const double Vm_NADPHM = theVars->Velocity_s[30] * TempCorr_Vm_Enz;
    double Vm_ATPB = theVars->Velocity_s[31] * TempCorr_Vm_Enz;
    const double Vm_NADPHB = theVars->Velocity_s[32] * TempCorr_Vm_Enz;
    //Vm_PR1=Vm_6*0.11*TempCorr_Vm_OC;
    const double Vm_PR2 = theVars->Velocity_s[34] * TempCorr_Vm_Enz;
    const double Vm_PR3 = theVars->Velocity_s[35] * TempCorr_Vm_Enz;
    const double Vm_PR4 = theVars->Velocity_s[36] * TempCorr_Vm_Enz;
    const double Vm_PR5 = theVars->Velocity_s[37] * TempCorr_Vm_Enz;
    const double Vm_PR6 = theVars->Velocity_s[38] * TempCorr_Vm_Enz;
    const double Vm_PR7 = theVars->Velocity_s[39] * TempCorr_Vm_Enz;
    double Vm_PR8 = theVars->Velocity_s[40] * TempCorr_Vm_Enz;
    const double VTgca_PR9 = theVars->Velocity_s[41] * TempCorr_Vm_Enz;
    double VTgcea_PR10 = theVars->Velocity_s[42] * TempCorr_Vm_Enz;
    const double Vm_62 = theVars->Velocity_s[43] * TempCorr_Vm_Enz;
    const double Vtp_Bchl = theVars->Velocity_s[44] * TempCorr_Vm_Enz;
    const double Vtp_Mchl = theVars->Velocity_s[45] * TempCorr_Vm_Enz;
    double Vm_Sta1 = theVars->Velocity_s[46] * TempCorr_Vm_Enz;
    const double Vm_Sta2 = theVars->Velocity_s[47] * TempCorr_Vm_Enz;
    const double Vm_Sta3 = theVars->Velocity_s[48] * TempCorr_Vm_Enz;
    const double Vm_OAA_M = theVars->Velocity_s[49] * TempCorr_Vm_Enz;
    const double Vm_PYR_B = theVars->Velocity_s[50] * TempCorr_Vm_Enz;
    const double Vm_PYR_M = theVars->Velocity_s[51] * TempCorr_Vm_Enz;
    const double Vm_PEP_M = theVars->Velocity_s[52] * TempCorr_Vm_Enz;
    const double Pmal = theVars->Velocity_s[53] * TempCorr_Vm_Enz;
    const double Ppyr = theVars->Velocity_s[54] * TempCorr_Vm_Enz;
    const double Pco2 = theVars->Velocity_s[55] * TempCorr_Vm_Enz;
    const double PC3P = theVars->Velocity_s[56] * TempCorr_Vm_Enz;
    const double Pco2_B = theVars->Velocity_s[57] * TempCorr_Vm_Enz;

    const double Vm_MAL_B = Vm_PEP_M * TempCorr_Vm_Enz;
    const double Vm_MAL_M = Vm_PEP_M * TempCorr_Vm_Enz;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //global Bchl_CA;//assume constant concentration
    //global Bchl_CN;
    //global Bchl_CP;
    //global MC_CU;
    //global MC_CA;
    //global MC_CP;
    //global MC_UTP;
    //global Mchl_CP;
    //global Mchl_CA;
    //global Mchl_CN;
    const double Mchl_NADP = Mchl_CN - Enzyme_con->Mchl_NADPH;
    const double Mchl_Pi = Mchl_CP - Enzyme_con->Mchl_PGA - 2 * Enzyme_con->Mchl_DPGA - Enzyme_con->Mchl_T3P - Enzyme_con->Mchl_ATP - Enzyme_con->Mchl_PEP;
    const double Mchl_GAP = Ke_9 * Enzyme_con->Mchl_T3P / (1 + Ke_9);
    const double Mchl_DHAP = Enzyme_con->Mchl_T3P / (1 + Ke_9);
    const double Mchl_ADP = Mchl_CA - Enzyme_con->Mchl_ATP;

    const double MC_UDP = MC_CU - MC_UTP - Enzyme_con->MC_UDPG;
    const double MC_PiT = MC_CP - 2 * Enzyme_con->MC_FBP - 2 * Enzyme_con->MC_F26BP - Enzyme_con->MC_PGA - Enzyme_con->MC_T3P - Enzyme_con->MC_HexP - Enzyme_con->MC_SUCP - MC_UTP - Enzyme_con->MC_ATP - Enzyme_con->MC_PEP;//1
    const double MC_Pi = (pow(pow(KePi, 2) + 4 * KePi * MC_PiT, 0.5) - KePi) / 2;
    const double MC_PPi = MC_PiT - MC_Pi;
    const double MC_GAP = Ke_9 * Enzyme_con->MC_T3P / (1 + Ke_9);
    const double MC_DHAP = Enzyme_con->MC_T3P / (1 + Ke_9);
    // MC_G6P = Enzyme_con->MC_HexP / (1 / Ke_Suc5 + Ke_Suc6 + 1);// --unused
    const double MC_G1P = Ke_Suc6 * Enzyme_con->MC_HexP / (1 / Ke_Suc5 + Ke_Suc6 + 1);
    const double MC_F6P = (Enzyme_con->MC_HexP / Ke_Suc5) / (1 / Ke_Suc5 + Ke_Suc6 + 1);
    const double MC_ADP = MC_CA - Enzyme_con->MC_ATP;
    const double BSC_GAP = Ke_9 * Enzyme_con->BSC_T3P / (1 + Ke_9);
    const double BSC_DHAP = Enzyme_con->BSC_T3P / (1 + Ke_9);
    const double Bchl_NADP = Bchl_CN - Enzyme_con->Bchl_NADPH;
    const double Bchl_Xu5P = (Enzyme_con->Bchl_Pent / Ke_17) / (1 / Ke_16 + 1 / Ke_17 + 1);
    const double Bchl_Ru5P = Enzyme_con->Bchl_Pent / (1 / Ke_16 + 1 / Ke_17 + 1);
    const double Bchl_Ri5P = (Enzyme_con->Bchl_Pent / Ke_16) / (1 / Ke_16 + 1 / Ke_17 + 1);

    const double Bchl_Pi = Bchl_CP - Enzyme_con->Bchl_PGA - 2 * Enzyme_con->Bchl_DPGA - Enzyme_con->Bchl_T3P - 2 * Enzyme_con->Bchl_FBP - Enzyme_con->Bchl_HexP - Enzyme_con->Bchl_E4P - 2 * Enzyme_con->Bchl_SBP - Enzyme_con->Bchl_S7P - Enzyme_con->Bchl_Pent - 2 * Enzyme_con->Bchl_RuBP - Enzyme_con->Bchl_ATP - Enzyme_con->Bchl_PGCA - Enzyme_con->Bchl_PPi - Enzyme_con->Bchl_PEP;//////WY
    //global Pim;
    //theVars->Pim = Bchl_Pi;  // --unused
    const double Bchl_GAP = Ke_9 * Enzyme_con->Bchl_T3P / (1 + Ke_9);
    const double Bchl_DHAP = Enzyme_con->Bchl_T3P / (1 + Ke_9);
    // Bchl_G6P = Enzyme_con->Bchl_HexP / (1 / Ke_Starch1 + Ke_Starch2 + 1);// --unused
    const double Bchl_G1P = Ke_Starch2 * Enzyme_con->Bchl_HexP / (1 / Ke_Starch1 + Ke_Starch2 + 1);
    const double Bchl_F6P = (Enzyme_con->Bchl_HexP / Ke_Starch1) / (1 / Ke_Starch1 + Ke_Starch2 + 1);
    const double Bchl_ADP = Bchl_CA - Enzyme_con->Bchl_ATP - Enzyme_con->Bchl_ADPG;
    const double Bmito_NAD = 1 - Enzyme_con->Bmito_NADH;

    //global CI;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Enzyme activation //WY 201902
    //PPDK
    //global PPDKRP;
    const double AMP = 0;
    const double PPi = 0;
    const double ET_PKRP = theVars->PPDKRP;//0.000025*3;//////change 0.0001
    const double Kcat_EA_PPDKRP_I = 1.125; // s-1
    const double Kcat_EA_PPDKRP_A = 0.578; // s-1
    // inactivation
    const double Km_EA_PPDKRP_I_ADP = 0.052; //mM
    const double Ki_EA_PPDKRP_I_Pyr = 0.08;
    const double Km_EA_PPDKRP_I_E = 0.0012;
    // activation
    const double Km_EA_PPDKRP_A_Pi = 0.65;
    const double Ki_EA_PPDKRP_A_AMP = 0.4;
    const double Km_EA_PPDKRP_A_EP = 0.0007;
    const double Ki_EA_PPDKRP_A_ADP = 0.085;
    const double Ki_EA_PPDKRP_A_PPI = 0.16;
    // inactivation rate
    theVars->Enzyme_Vel.vEA_PPDKRP_I = ET_PKRP * Kcat_EA_PPDKRP_I * Enzyme_con->E_PPDK_Mchl * Mchl_ADP / ((Enzyme_con->E_PPDK_Mchl + Km_EA_PPDKRP_I_E) * (Mchl_ADP + Km_EA_PPDKRP_I_ADP * (1 + Enzyme_con->Mchl_Pyruvate / Ki_EA_PPDKRP_I_Pyr)));
    // activation rate
    theVars->Enzyme_Vel.vEA_PPDKRP_A = ET_PKRP * Kcat_EA_PPDKRP_A * Enzyme_con->EP_PPDK_Mchl * Mchl_Pi / ((Enzyme_con->EP_PPDK_Mchl + Km_EA_PPDKRP_A_EP * (1 + Mchl_ADP / Ki_EA_PPDKRP_A_ADP + PPi / Ki_EA_PPDKRP_A_PPI)) * (Mchl_Pi + Km_EA_PPDKRP_A_Pi * (1 + AMP / Ki_EA_PPDKRP_A_AMP)));
    const double A_PPDK = Enzyme_con->E_PPDK_Mchl / (Enzyme_con->E_PPDK_Mchl + Enzyme_con->EP_PPDK_Mchl);
    if (theVars->EAPPDK)
        Vm_5 = Vm_5 * A_PPDK;


    // kcat_ATPsyn=16;
    // kcat_NADPMDH=1520;
    // kcat_PEPC=66;
    // kcat_Rubisco=4.1;
    // Kcat_GAPDH=50;
    // kcat_FBP=22.9;
    // kcat_SBP=81;
    // kcat_PRK=615;



    Vm_ATPM = Enzyme_con->parent->AE_con->Mchl_ActATPsynthase * Vm_ATPM;

    if (RedoxEnyAct) {
        Vm_3 = Enzyme_con->parent->AE_con->Mchl_ActNADPMDH * Vm_3;
        Vm_2 = Enzyme_con->parent->AE_con->Bchl_ActPEPC * Vm_2;
        Vm_ATPB = Enzyme_con->parent->AE_con->Bchl_ActATPsynthase * Vm_ATPB;
        Vm_78Mchl = Enzyme_con->parent->AE_con->Mchl_ActGAPDH * Vm_78Mchl;
        Vm_78 = Enzyme_con->parent->AE_con->Bchl_ActGAPDH * Vm_78;
        Vm_11 = Enzyme_con->parent->AE_con->Bchl_ActFBPase * Vm_11;
        Vm_13 = Enzyme_con->parent->AE_con->Bchl_ActSBPase * Vm_13;
        Vm_18 = Enzyme_con->parent->AE_con->Bchl_ActPRK * Vm_18;
    }

    /////// ???????????????????????????????
    if (PRac)
        Vm_6 = (Enzyme_con->parent->AE_con->Bchl_ActRubisco) * Vm_6 * V6sen;


    //////////////////////////////////////////////////////////////////////////////////////////
    //const double Sc = 3 * pow(10, 4);
    // vs = 20;// m2/L// --unused

    if (!theVars->Para_mata) {
        theVars->Enzyme_Vel.vinf = gm * Sc * pow(10, (-3)) * (theVars->CI - Enzyme_con->MC_CO2);
    } else {
        theVars->Enzyme_Vel.vinf = gm * Sc * pow(10, (-3)) * (Enzyme_con->parent->Leaf_con->Ci / (3 * pow(10, 4)) - Enzyme_con->MC_CO2);
    }


    //C4 Cycle 5
    theVars->Enzyme_Vel.v1 = Vm_1 * (Enzyme_con->MC_CO2 - Enzyme_con->MC_HCO3 / Ke_1) / (KmCO2_1 + Enzyme_con->MC_CO2);
    const double Kimal_2 = 15;
    const double Kimal_2n = 1.5;
    const double Vm_2P = Enzyme_con->parent->AE_con->Bchl_ActPEPC * Vm_2;
    const double Vm_2OH = Vm_2 - Vm_2P;
    const double v_2P = Vm_2 * Enzyme_con->MC_HCO3 * Enzyme_con->MC_PEP / (Enzyme_con->MC_PEP + KmPEP_2 * (1 + Enzyme_con->MC_Malate / Kimal_2 + Enzyme_con->MC_OAA / 1)) / (Enzyme_con->MC_HCO3 + KmHCO3_2);
    const double v_2OH = Vm_2OH * Enzyme_con->MC_HCO3 * Enzyme_con->MC_PEP / (Enzyme_con->MC_PEP + KmPEP_2 * (1 + Enzyme_con->MC_Malate / (Kimal_2n) + Enzyme_con->MC_OAA / 1)) / (Enzyme_con->MC_HCO3 + KmHCO3_2);
    theVars->Enzyme_Vel.v2 = v_2P + v_2OH;

    theVars->Enzyme_Vel.v3 = Vm_3 * (Enzyme_con->Mchl_OAA * Enzyme_con->Mchl_NADPH - Mchl_NADP * Enzyme_con->Mchl_Malate / Ke_3) / (KmOAA_3 * KmNADPH_3 * (1 + Enzyme_con->Mchl_OAA / KmOAA_3 + Enzyme_con->Mchl_NADPH / KmNADPH_3 + Mchl_NADP / KmNADP_3 + Enzyme_con->Mchl_Malate / Kmmal_3 + Enzyme_con->Mchl_OAA * Enzyme_con->Mchl_NADPH / (KmOAA_3 * KmNADPH_3) + Mchl_NADP * Enzyme_con->Mchl_Malate / (KmNADP_3 * Kmmal_3)));

    theVars->Enzyme_Vel.v4 = Vm_4 * (Enzyme_con->Bchl_Malate * Bchl_NADP - Enzyme_con->Bchl_Pyruvate * Enzyme_con->Bchl_NADPH * Enzyme_con->Bchl_CO2 / Ke_4) / (Kmmal_4 * KmNADP_4) / (1 + Enzyme_con->Bchl_Malate / Kmmal_4 + Bchl_NADP / KmNADP_4 + Enzyme_con->Bchl_Pyruvate / KmPyr_4 + Enzyme_con->Bchl_NADPH / KmNADPH_4 + Enzyme_con->Bchl_CO2 / KmCO2_4 + Enzyme_con->Bchl_Malate * Bchl_NADP / (Kmmal_4 * KmNADP_4) + Enzyme_con->Bchl_Pyruvate * Enzyme_con->Bchl_NADPH / (KmPyr_4 * KmNADPH_4) + Enzyme_con->Bchl_Pyruvate * Enzyme_con->Bchl_CO2 / (KmPyr_4 * KmCO2_4) + Enzyme_con->Bchl_NADPH * Enzyme_con->Bchl_CO2 / (KmNADPH_4 * KmCO2_4) + Enzyme_con->Bchl_Pyruvate * Enzyme_con->Bchl_NADPH * Enzyme_con->Bchl_CO2 / (KmPyr_4 * KmNADPH_4 * KmCO2_4));

    theVars->Enzyme_Vel.v5 = Vm_5 * Enzyme_con->Mchl_Pyruvate * Enzyme_con->Mchl_ATP / (Enzyme_con->Mchl_Pyruvate + KmPyr_5 * (1 + Enzyme_con->Mchl_PEP / KiPEP_5)) / (Enzyme_con->Mchl_ATP + KmATP_5);
    //////// PPDK in BSC
    //global v5B;
    const double Vm_5B = RatioPPDK * Vm_5;
    theVars->Enzyme_Vel.v5B = Vm_5B * Enzyme_con->Bchl_Pyruvate * Enzyme_con->Bchl_ATP / (Enzyme_con->Bchl_Pyruvate + KmPyr_5 * (1 + Enzyme_con->Bchl_PEP / KiPEP_5)) / (Enzyme_con->Bchl_ATP + KmATP_5);
    //////////
    // // // Calvin cycle
    theVars->Enzyme_Vel.v6 = Vm_6 * Enzyme_con->Bchl_RuBP * Enzyme_con->Bchl_CO2 / ((Enzyme_con->Bchl_CO2 + KmCO2_6 * (1 + Enzyme_con->Bchl_O2 / KmO2_6)) * (Enzyme_con->Bchl_RuBP + KmRuBP_6 * (1 + Enzyme_con->Bchl_PGA / KiPGA_6 + Enzyme_con->Bchl_FBP / KiFBP_6 + Enzyme_con->Bchl_SBP / KiSBP_6 + Bchl_Pi / KiPi_6)));

    theVars->Enzyme_Vel.v7 = 0;//Not used
    theVars->Enzyme_Vel.v8 = 0;//Not used

    const double KmPGA_78 = 5;
    const double KmATP_78 = 0.3;
    const double KmNADPH_78 = 0.1;
    // KmADP_78 = 0.5;// --unused
    // KmNADP_78 = 0.5;// --unused
    //v78=Vm_78*(Enzyme_con->Bchl_PGA*Enzyme_con->Bchl_ATP*Enzyme_con->Bchl_NADPH)/((Enzyme_con->Bchl_PGA+KmPGA_78)*(Enzyme_con->Bchl_ATP+KmATP_78*(1+Bchl_ADP/KmADP_78))*(Enzyme_con->Bchl_NADPH+KmNADPH_78*(1+Bchl_NADP/KmNADP_78)));
    theVars->Enzyme_Vel.v78 = Vm_78 * (Enzyme_con->Bchl_PGA * Enzyme_con->Bchl_ATP * Enzyme_con->Bchl_NADPH) / ((Enzyme_con->Bchl_PGA + KmPGA_78) * (Enzyme_con->Bchl_ATP + KmATP_78) * (Enzyme_con->Bchl_NADPH + KmNADPH_78));

    theVars->Enzyme_Vel.v10 = Vm_10 * (Bchl_GAP * Bchl_DHAP - Enzyme_con->Bchl_FBP / Ke_10) / (KmGAP_10 * KmDHAP_10 * (1 + Bchl_GAP / KmGAP_10 + Bchl_DHAP / KmDHAP_10 + Enzyme_con->Bchl_FBP / KmFBP_10 + Bchl_GAP * Bchl_DHAP / (KmGAP_10 * KmDHAP_10)));
    theVars->Enzyme_Vel.v11 = Vm_11 * (Enzyme_con->Bchl_FBP - Bchl_F6P * Bchl_Pi / Ke_11) / (Enzyme_con->Bchl_FBP + KmFBP_11 * (1 + Bchl_F6P / KiF6P_11 + Bchl_Pi / KiPi_11));
    theVars->Enzyme_Vel.v12 = Vm_12 * (Bchl_DHAP * Enzyme_con->Bchl_E4P - Enzyme_con->Bchl_SBP / Ke_12) / ((Enzyme_con->Bchl_E4P + KmE4P_12) * (Bchl_DHAP + KmDHAP_12));
    theVars->Enzyme_Vel.v13 = Vm_13 * (Enzyme_con->Bchl_SBP - Bchl_Pi * Enzyme_con->Bchl_S7P / Ke_13) / (Enzyme_con->Bchl_SBP + KmSBP_13 * (1 + Bchl_Pi / KiPi_13));
    theVars->Enzyme_Vel.v14 = Vm_14 * (Bchl_F6P * Bchl_GAP - Bchl_Xu5P * Enzyme_con->Bchl_E4P / Ke_14) / ((Bchl_F6P + KmF6P_14 * (1 + Bchl_Xu5P / KmXu5P + Enzyme_con->Bchl_E4P / KmE4P_14)) * (Bchl_GAP + KmGAP_14));
    theVars->Enzyme_Vel.v15 = Vm_15 * (Bchl_GAP * Enzyme_con->Bchl_S7P - Bchl_Ri5P * Bchl_Xu5P / Ke_15) / ((Bchl_GAP + KmGAP_15 * (1 + Bchl_Xu5P / KmXu5P_15 + Bchl_Ri5P / KmRi5P_15)) * (Enzyme_con->Bchl_S7P + KmS7P_15));
    theVars->Enzyme_Vel.v18 = Vm_18 * (Enzyme_con->Bchl_ATP * Bchl_Ru5P - Bchl_ADP * Enzyme_con->Bchl_RuBP / Ke_18) / ((Enzyme_con->Bchl_ATP * (1 + Bchl_ADP / KiADP_18) + KmATP_18 * (1 + Bchl_ADP / Ki_ADP_18)) * (Bchl_Ru5P + KmRu5P_18 * (1 + Enzyme_con->Bchl_PGA / KiPGA_18 + Enzyme_con->Bchl_RuBP / KiRuBP_18 + Bchl_Pi / KiPi_18)));
    theVars->Enzyme_Vel.v7Mchl = 0;//Not used;
    theVars->Enzyme_Vel.v8Mchl = 0;//Not used ;

    const double KmPGA_78Mchl = 5;
    const double KmATP_78Mchl = 0.3;
    const double KmNADPH_78Mchl = 0.1;
    // KmADP_78Mchl = 0.5;// --unused
    // KmNADP_78Mchl = 0.5;// --unused

    theVars->Enzyme_Vel.v78Mchl = Vm_78Mchl * (Enzyme_con->Mchl_PGA * Enzyme_con->Mchl_ATP * Enzyme_con->Mchl_NADPH) / ((Enzyme_con->Mchl_PGA + KmPGA_78Mchl) * (Enzyme_con->Mchl_ATP + KmATP_78Mchl) * (Enzyme_con->Mchl_NADPH + KmNADPH_78Mchl));
    theVars->Enzyme_Vel.vStarch1 = Vm_Starch * Bchl_G1P * Enzyme_con->Bchl_ATP / ((Bchl_G1P + KmG1P_Starch) * ((1 + Bchl_ADP / KiADP_Starch) * (Enzyme_con->Bchl_ATP + KmATP_Starch) + KmATP_Starch * Bchl_Pi / (KaPGA_Starch * Enzyme_con->Bchl_PGA + KaF6P_Starch * Bchl_F6P + KaFBP_Starch * Enzyme_con->Bchl_FBP)));
    // KmATP_Starch2=1;
    // Vm_Starch2=0.1/20;
    // KmStarch_Starch2=1;
    theVars->Enzyme_Vel.vStarch2 = 0;//Not used

    //  2.7.7.27
    const double KaPGA_Sta1 = 0.2;//0.2252;
    const double KmG1P_Sta1 = 0.06;//0.06;
    const double KmATP_Sta1 = 0.12;//0.18
    const double KIAPi_ATP_Sta1 = 2.96;
    const double KmPPi_Sta1 = 0.033;
    const double KICPP1_ATP_Sta1 = 13.8e-4;
    const double KmADPG_Sta1 = 0.24;
    const double KIAADP_ATP_Sta1 = 2.0;
    const double Ke_Sta1 = 1.1;

    //global vSta1;
    Vm_Sta1 = Vm_Sta1 * Enzyme_con->Bchl_PGA / (Enzyme_con->Bchl_PGA + KaPGA_Sta1);
    theVars->Enzyme_Vel.vSta1 = Vm_Sta1 * (Bchl_G1P * Enzyme_con->Bchl_ATP - Enzyme_con->Bchl_ADPG * Enzyme_con->Bchl_PPi / Ke_Sta1) / (KmG1P_Sta1 * KmATP_Sta1 * (1 + Bchl_ADP / KIAADP_ATP_Sta1 + Enzyme_con->Bchl_PPi / KICPP1_ATP_Sta1 + Bchl_Pi / KIAPi_ATP_Sta1) * (1 + Bchl_G1P / KmG1P_Sta1 + Enzyme_con->Bchl_ATP * (1 + Bchl_Pi / KIAPi_ATP_Sta1 + Bchl_ADP / KIAADP_ATP_Sta1) / (KmATP_Sta1 * (1 + Bchl_ADP / KIAADP_ATP_Sta1 + Enzyme_con->Bchl_PPi / KICPP1_ATP_Sta1 + Bchl_Pi / KIAPi_ATP_Sta1)) + Enzyme_con->Bchl_ADPG / KmADPG_Sta1 + Enzyme_con->Bchl_PPi / KmPPi_Sta1 + Bchl_G1P * Enzyme_con->Bchl_ATP * (1 + Bchl_Pi / KIAPi_ATP_Sta1 + Bchl_ADP / KIAADP_ATP_Sta1) / (KmG1P_Sta1 * KmATP_Sta1 * (1 + Bchl_ADP / KIAADP_ATP_Sta1 + Enzyme_con->Bchl_PPi / KICPP1_ATP_Sta1 + Bchl_Pi / KIAPi_ATP_Sta1)) + Enzyme_con->Bchl_ADPG * Enzyme_con->Bchl_PPi / (KmADPG_Sta1 * KmPPi_Sta1)));
    //  3.6.1.1
    const double KmPPi_Sta2 = 0.154;
    const double Ke_Sta2 = 15700.0;
    theVars->Enzyme_Vel.vSta2 = Vm_Sta2 * (Enzyme_con->Bchl_PPi - Bchl_Pi * Bchl_Pi / Ke_Sta2) / (Enzyme_con->Bchl_PPi + KmPPi_Sta2);

    //  2.4.1.21
    const double KmADPG_Sta3 = 0.077;
    theVars->Enzyme_Vel.vSta3 = Vm_Sta3 * Enzyme_con->Bchl_ADPG / (Enzyme_con->Bchl_ADPG + KmADPG_Sta3);
    //global vStarch;
    //theVars->vStarch = vSta3;  // --unused


    const double Kmpi_hexp = 1.5;
    const double Kmhexp_hexp = 1;//1/5;//WY20/02
    //KmATP_hexp=0.8;
    double Vm_Hep = 0.0005;
    if (Enzyme_con->Bchl_HexP >= 0 && Enzyme_con->Bchl_HexP <6)
        Vm_Hep = (6 - Enzyme_con->Bchl_HexP) / 6 * 0.0005;

    if (Enzyme_con->Bchl_HexP > 6)
        Vm_Hep = 0;


    theVars->Enzyme_Vel.vhexp = Vm_Hep * (Bchl_Pi / (Kmpi_hexp * (1 + Enzyme_con->Bchl_HexP / Kmhexp_hexp) + Bchl_Pi));
    if (Enzyme_con->Bchl_HexP > 6)
        theVars->Enzyme_Vel.vhexp = 0;

    //global vHexP;
    //theVars->vHexP = vhexp;  // --unused
    theVars->Enzyme_Vel.vPGASink = Vm_PGASink * Enzyme_con->MC_PGA / (Enzyme_con->MC_PGA + KmPGA_PGASink);//MC
    //global vpgasink;
    //theVars->vpgasink = vPGASink;  // --unused

    theVars->Enzyme_Vel.vSuc1 = Vm_Suc1 * (MC_GAP * MC_DHAP - Enzyme_con->MC_FBP / Ke_Suc1) / (KmGAP_Suc1 * KmDHAP_Suc1 * (1 + MC_GAP / KmGAP_Suc1 + MC_DHAP / KmDHAP_Suc1 + Enzyme_con->MC_FBP / KmFBP_Suc1 + MC_GAP * MC_DHAP / (KmGAP_Suc1 * KmDHAP_Suc1)));

    theVars->Enzyme_Vel.vSuc2 = Vm_Suc2 * (Enzyme_con->MC_FBP - MC_F6P * MC_Pi / Ke_Suc2) / (KmFBP_Suc2 * (1 + Enzyme_con->MC_F26BP / KiF26BP_Suc2) * (1 + Enzyme_con->MC_FBP / (KmFBP_Suc2 * (1 + Enzyme_con->MC_F26BP / KiF26BP_Suc2)) + MC_Pi / KiPi_Suc2 + MC_F6P / KiF6P_Suc2 + MC_Pi * MC_F6P / (KiPi_Suc2 * KiF6P_Suc2)));
    theVars->Enzyme_Vel.vSuc7 = Vm_Suc7 * (MC_UTP * MC_G1P - Enzyme_con->MC_UDPG * MC_PPi / Ke_Suc7) / (KmUTP_Suc7 * KmG1P_Suc7 * (1 + MC_UTP / KmUTP_Suc7 + MC_G1P / KmG1P_Suc7 + Enzyme_con->MC_UDPG / KmUDPG_Suc7 + MC_PPi / KmPPi_Suc7 + MC_UTP * MC_G1P / (KmUTP_Suc7 * KmG1P_Suc7) + Enzyme_con->MC_UDPG * MC_PPi / (KmUDPG_Suc7 * KmPPi_Suc7)));
    theVars->Enzyme_Vel.vSuc8 = Vm_Suc8 * (MC_F6P * Enzyme_con->MC_UDPG - Enzyme_con->MC_SUCP * MC_UDP / Ke_Suc8) / ((MC_F6P + KmF6P_Suc8 * (1 + Enzyme_con->MC_FBP / KiFBP_Suc8)) * (Enzyme_con->MC_UDPG + KmUDPG_Suc8 * (1 + MC_UDP / KiUDP_Suc8) * (1 + Enzyme_con->MC_SUCP / KiSucP_Suc8) * (1 + Enzyme_con->MC_SUC / KiSuc_Suc8) * (1 + MC_Pi / KiPi_Suc8)));
    theVars->Enzyme_Vel.vSuc9 = Vm_Suc9 * (Enzyme_con->MC_SUCP - Enzyme_con->MC_SUC * MC_Pi / Ke_Suc9) / (Enzyme_con->MC_SUCP + KmSucP_Suc9 * (1 + Enzyme_con->MC_SUC / KmSuc_Suc9));
    theVars->Enzyme_Vel.vSuc10 = Vm_Suc10 * Enzyme_con->MC_SUC / (Enzyme_con->MC_SUC + KmSuc_Suc10);
    //global vSuc;
    //theVars->vSuc = theVars->vSuc10;  // --unused
    theVars->Enzyme_Vel.vSuc3 = Vm_Suc3 * (Enzyme_con->MC_ATP * MC_F6P - MC_ADP * Enzyme_con->MC_F26BP / Ke_Suc3) / ((MC_F6P + KmF6P_Suc3 * (1 + Enzyme_con->MC_F26BP / KmF26BP_Suc3) * (1 + MC_DHAP / KIDHAP_Suc3)) * (Enzyme_con->MC_ATP + KmATP_Suc3 * (1 + MC_ADP / KiADP_Suc3)));
    theVars->Enzyme_Vel.vSuc4 = Vm_Suc4 * Enzyme_con->MC_F26BP / (KmF26BP_Suc4 * (1 + Enzyme_con->MC_F26BP / KmF26BP_Suc4) * (1 + MC_Pi / KiPi_Suc4) * (1 + MC_F6P / KiF6P_Suc4));

    //ATP&NADPH 3
    const double ETRa = D * ((F / 2 * X * I + Y * Jmax - sqrt( pow((F / 2 * X * I + Y * Jmax), 2) - 4 * Q * F / 2 * X * I * Y * Jmax)) / (2 * Q));
    const double ETRn = E * ((F / 2 * X * I + Y * Jmax - sqrt( pow((F / 2 * X * I + Y * Jmax), 2) - 4 * Q * F / 2 * X * I * Y * Jmax)) / (2 * Q));
    theVars->Enzyme_Vel.vATPM = std::min(Vm_ATPM, ETRa) * (Mchl_ADP * Mchl_Pi - Enzyme_con->Mchl_ATP / (Ke_ATPM)) / (KmADP_ATPM * KmPi_ATPM * (1 + Mchl_ADP / KmADP_ATPM + Mchl_Pi / KmPi_ATPM + Enzyme_con->Mchl_ATP / KmATP_ATPM + Mchl_ADP * Mchl_Pi / (KmADP_ATPM * KmPi_ATPM)));
    theVars->Enzyme_Vel.vNADPHM = std::min(Vm_NADPHM, ETRn) * (Mchl_NADP - Enzyme_con->Mchl_NADPH / Ke_NADPHM) / (KmNADP_NADPHM * (1 + Mchl_NADP / KmNADP_NADPHM + Enzyme_con->Mchl_NADPH / KmNADPH_NADPHM));
    // //WY202003

    // vATPM=min(Vm_ATPM*(Mchl_ADP*Mchl_Pi-Enzyme_con->Mchl_ATP/(Ke_ATPM))/(KmADP_ATPM*KmPi_ATPM*(1+Mchl_ADP/KmADP_ATPM+Mchl_Pi/KmPi_ATPM+Enzyme_con->Mchl_ATP/KmATP_ATPM+Mchl_ADP*Mchl_Pi/(KmADP_ATPM*KmPi_ATPM))),ETRa);
    //const double vNADPHM = std::min(Vm_NADPHM * (Mchl_NADP - Enzyme_con->Mchl_NADPH / Ke_NADPHM) / (KmNADP_NADPHM * (1 + Mchl_NADP / KmNADP_NADPHM + Enzyme_con->Mchl_NADPH / KmNADPH_NADPHM)), ETRn);

    theVars->Enzyme_Vel.vO2_Mchl = theVars->Enzyme_Vel.vNADPHM / 2;

    const double ETRab = G * ((F * (1 - U) * (1 - X) * I + (1 - V) * (1 - Y) * Jmax - sqrt( pow((F * (1 - U) * (1 - X) * I + (1 - V) * (1 - Y) * Jmax), 2) - 4 * Q * F * (1 - U) * (1 - X) * I * (1 - V) * (1 - Y) * Jmax)) / (2 * Q));
    const double ETRabl = D * ((F / 2 * U * (1 - X) * I + V * (1 - Y) * Jmax - sqrt( pow((F / 2 * U * (1 - X) * I + V * (1 - Y) * Jmax), 2) - 4 * Q * F / 2 * U * (1 - X) * I * V * (1 - Y) * Jmax)) / (2 * Q));
    const double ETRnbl = E * ((F / 2 * U * (1 - X) * I + V * (1 - Y) * Jmax - sqrt( pow((F / 2 * U * (1 - X) * I + V * (1 - Y) * Jmax), 2) - 4 * Q * F / 2 * U * (1 - X) * I * V * (1 - Y) * Jmax)) / (2 * Q));

    theVars->Enzyme_Vel.vATPB = std::min(Vm_ATPB, ETRab + ETRabl) * (Bchl_ADP * Bchl_Pi - Enzyme_con->Bchl_ATP / Ke_ATPB) / (KmADP_ATPB * KmPi_ATPB * (1 + Bchl_ADP / KmADP_ATPB + Bchl_Pi / KmPi_ATPB + Enzyme_con->Bchl_ATP / KmATP_ATPB + Bchl_ADP * Bchl_Pi / (KmADP_ATPB * KmPi_ATPB)));
    theVars->Enzyme_Vel.vNADPHB = std::min(Vm_NADPHB, ETRnbl) * (Bchl_NADP - Enzyme_con->Bchl_NADPH / Ke_NADPHB) / (KmNADP_NADPHB * (1 + Bchl_NADP / KmNADP_NADPHB + Enzyme_con->Bchl_NADPH / KmNADPH_NADPHB));
    // vATPB=min(Vm_ATPB*(Bchl_ADP*Bchl_Pi-Enzyme_con->Bchl_ATP/Ke_ATPB)/(KmADP_ATPB*KmPi_ATPB*(1+Bchl_ADP/KmADP_ATPB+Bchl_Pi/KmPi_ATPB+Enzyme_con->Bchl_ATP/KmATP_ATPB+Bchl_ADP*Bchl_Pi/(KmADP_ATPB*KmPi_ATPB))),ETRab+ETRabl);
    //theVars->Enzyme_Vel.vNADPHB = std::min(Vm_NADPHB * (Bchl_NADP - Enzyme_con->Bchl_NADPH / Ke_NADPHB) / (KmNADP_NADPHB * (1 + Bchl_NADP / KmNADP_NADPHB + Enzyme_con->Bchl_NADPH / KmNADPH_NADPHB)), ETRnbl);

    theVars->Enzyme_Vel.vO2_Bchl = theVars->Enzyme_Vel.vNADPHB / 2;
    //Transport 17
    const double Km_OAA_M = 0.053;
    const double Kimal_OAA_M = 7.5;
    theVars->Enzyme_Vel.vOAA_M = Vm_OAA_M * (Enzyme_con->MC_OAA - Enzyme_con->Mchl_OAA) / (Enzyme_con->MC_OAA + Km_OAA_M * (1 + Enzyme_con->MC_Malate / Kimal_OAA_M));

    const double Km_MAL_M = 0.5;
    const double KiOAA_MAL_M = 0.3;//0.3;
    // Vm_MAL_B=3/20;
    const double Km_MAL_B = 1;
    theVars->Enzyme_Vel.vMAL_M = Vm_MAL_M * (Enzyme_con->Mchl_Malate - Enzyme_con->MC_Malate) / (Enzyme_con->Mchl_Malate + Km_MAL_M * (1 + Enzyme_con->Mchl_OAA / KiOAA_MAL_M));// 2  Enzyme_con->Mchl_malate -> MC.malate
    theVars->Enzyme_Vel.vMAL = Pmal * (Enzyme_con->MC_Malate - Enzyme_con->BSC_Malate);// 3 Enzyme_con->MC_malate -> BSC_malate
    theVars->Enzyme_Vel.vMAL_B = Vm_MAL_B * (Enzyme_con->BSC_Malate - Enzyme_con->Bchl_Malate) / (Enzyme_con->BSC_Malate + Km_MAL_B);// 4 BSC_malate -> Enzyme_con->Bchl_malate

    const double Km_PYR_B = 0.1;
    theVars->Enzyme_Vel.vPYR_B = Vm_PYR_B * (Enzyme_con->Bchl_Pyruvate - Enzyme_con->BSC_Pyruvate / 10) / (Km_PYR_B + Enzyme_con->Bchl_Pyruvate);

    theVars->Enzyme_Vel.vPYR = Ppyr * (Enzyme_con->BSC_Pyruvate - Enzyme_con->MC_Pyruvate);// 6 BSC_pyruvate -> MC_pyruvate

    const double Km_PYR_M = 0.1;
    theVars->Enzyme_Vel.vPYR_M = Vm_PYR_M * (Enzyme_con->MC_Pyruvate - Enzyme_con->Mchl_Pyruvate / 10) / (Km_PYR_M + Enzyme_con->MC_Pyruvate);

    const double Km_PEP_M = 0.5;
    theVars->Enzyme_Vel.vPEP_M = Vm_PEP_M * (Enzyme_con->Mchl_PEP - Enzyme_con->MC_PEP) / (Km_PEP_M + Enzyme_con->Mchl_PEP);// 8 Mchl_PEP + MC_Pi -> Enzyme_con->MC_PEP + Mchl_Pi
    const double Vm_PEP_B = Vm_PEP_M;
    theVars->Enzyme_Vel.vPEP_B = Vm_PEP_B * (Enzyme_con->Bchl_PEP - Enzyme_con->BSC_PEP) / (Km_PEP_M + Enzyme_con->Bchl_PEP);

    const double KmPGA_B = 2;
    const double KmGAP_B = 2;
    const double KmDHAP_B = 2;

    theVars->Enzyme_Vel.vPGA_B = Vtp_Bchl * Enzyme_con->Bchl_PGA / (Enzyme_con->Bchl_PGA + KmPGA_B * (1 + Bchl_DHAP / KmDHAP_B) * (1 + Bchl_GAP / KmGAP_B)) - Vtp_Bchl * Enzyme_con->BSC_PGA / (Enzyme_con->BSC_PGA + KmPGA_B * (1 + BSC_DHAP / KmDHAP_B) * (1 + BSC_GAP / KmGAP_B));
    theVars->Enzyme_Vel.vGAP_B = Vtp_Bchl * BSC_GAP / (BSC_GAP + KmGAP_B * (1 + Enzyme_con->BSC_PGA / KmPGA_B) * (1 + BSC_DHAP / KmDHAP_B)) - Vtp_Bchl * Bchl_GAP / (Bchl_GAP + KmGAP_B * (1 + Enzyme_con->Bchl_PGA / KmPGA_B) * (1 + Bchl_DHAP / KmDHAP_B));
    theVars->Enzyme_Vel.vDHAP_B = Vtp_Bchl * BSC_DHAP / (BSC_DHAP + KmDHAP_B * (1 + Enzyme_con->BSC_PGA / KmPGA_B) * (1 + BSC_GAP / KmGAP_B)) - Vtp_Bchl * Bchl_DHAP / (Bchl_DHAP + KmDHAP_B * (1 + Enzyme_con->Bchl_PGA / KmPGA_B) * (1 + Bchl_GAP / KmGAP_B));

    theVars->Enzyme_Vel.vPGA = PC3P * (Enzyme_con->BSC_PGA - Enzyme_con->MC_PGA);// 10 BSC_PGA+MC.Pi -> MC.PGA+BSC_Pi
    theVars->Enzyme_Vel.vGAP = PC3P * (MC_GAP - BSC_GAP);// 13 MC_GAP+BSC_Pi ->BSC_GAP+MC_Pi
    theVars->Enzyme_Vel.vDHAP = PC3P * (MC_DHAP - BSC_DHAP);// 16 MC_DHAP+BSC_Pi ->BSC_DHAP+MC_Pi

    const double KmPGA = 2;
    const double KmGAP = 2;
    const double KmDHAP = 2;
    theVars->Enzyme_Vel.vPGA_M = Vtp_Mchl * Enzyme_con->MC_PGA / (Enzyme_con->MC_PGA + KmPGA * (1 + MC_DHAP / KmDHAP) * (1 + MC_GAP / KmGAP)) - Vtp_Mchl * Enzyme_con->Mchl_PGA / (Enzyme_con->Mchl_PGA + KmPGA * (1 + Mchl_DHAP / KmDHAP) * (1 + Mchl_GAP / KmGAP));
    theVars->Enzyme_Vel.vGAP_M = Vtp_Mchl * Mchl_GAP / (Mchl_GAP + KmGAP * (1 + Enzyme_con->Mchl_PGA / KmPGA) * (1 + Mchl_DHAP / KmDHAP)) - Vtp_Mchl * MC_GAP / (MC_GAP + KmGAP * (1 + Enzyme_con->MC_PGA / KmPGA) * (1 + MC_DHAP / KmDHAP));
    theVars->Enzyme_Vel.vDHAP_M = Vtp_Mchl * Mchl_DHAP / (Mchl_DHAP + KmDHAP * (1 + Enzyme_con->Mchl_PGA / KmPGA) * (1 + Mchl_GAP / KmGAP)) - Vtp_Mchl * MC_DHAP / (MC_DHAP + KmDHAP * (1 + Enzyme_con->MC_PGA / KmPGA) * (1 + MC_GAP / KmGAP));

    theVars->Enzyme_Vel.vtATP = 5 * (Enzyme_con->Mchl_ATP - Enzyme_con->MC_ATP * 2);

    theVars->Enzyme_Vel.vleak_B = Pco2_B * (Enzyme_con->Bchl_CO2 - Enzyme_con->BSC_CO2);//0.5376*10*(Enzyme_con->Bchl_CO2-Enzyme_con->BSC_CO2);//Vleak*(Enzyme_con->Bchl_CO2-Enzyme_con->BSC_CO2);//  Bchl_CO2 -> BSC_CO2

    theVars->Enzyme_Vel.vleak = Pco2 * (Enzyme_con->BSC_CO2 - Enzyme_con->MC_CO2);//Vleak*(Enzyme_con->BSC_CO2-Enzyme_con->MC_CO2);//BSC_CO2->MC_CO2
    //theVars->vleakage = vleak;  // --unused
    //theVars->vleakage2 = vleak_B;  // --unused
    // Photorespiration
    const double Vm_PR1 = Vm_6 * 0.11 * TempCorr_Vm_OC;//////WY202010

    theVars->Enzyme_Vel.vpr1 = Vm_PR1 * Enzyme_con->Bchl_RuBP * Enzyme_con->Bchl_O2 / ((Enzyme_con->Bchl_O2 + KmO2_PR1 * (1 + Enzyme_con->Bchl_CO2 / KmCO2_PR1)) * (Enzyme_con->Bchl_RuBP + KmRuBP_PR1 * (1 + Enzyme_con->Bchl_PGA / KiPGA_PR1 + Enzyme_con->Bchl_FBP / KiFBP_PR1 + Enzyme_con->Bchl_SBP / KiSBP_PR1 + Bchl_Pi / KiPi_PR1 + Enzyme_con->Bchl_NADPH / KiNADPH_PR1)));
    theVars->Enzyme_Vel.vpr2 = Vm_PR2 * Enzyme_con->Bchl_PGCA / (Enzyme_con->Bchl_PGCA + KmPGCA_PR2 * (1 + Enzyme_con->Bchl_GCA / KiGCA_PR2) * (1 + Bchl_Pi / KiPI_PR2));
    theVars->Enzyme_Vel.vpr3 = Vm_PR3 * Enzyme_con->Bper_GCA / (Enzyme_con->Bper_GCA + KmGCA_PR3);
    theVars->Enzyme_Vel.vpr4 = Vm_PR4 * (Enzyme_con->Bper_GOA * Bper_GLU - Bper_KG * Enzyme_con->Bper_GLY / Ke_PS4) / ((Enzyme_con->Bper_GOA + KmGOA_PS4) * (Bper_GLU + KmGLU_PS4 * (1 + Enzyme_con->Bper_GLY / KiGLY_PS4)));
    theVars->Enzyme_Vel.vpr5 = Vm_PR5 * Enzyme_con->Bper_GLY / (Enzyme_con->Bper_GLY + KmGLY_PS5 * (1 + Enzyme_con->Bper_SER / KiSER_PS5));
    theVars->Enzyme_Vel.vpr6 = Vm_PR6 * (Enzyme_con->Bper_GOA * Enzyme_con->Bper_SER - Enzyme_con->Bper_HPR * Enzyme_con->Bper_GLY / Ke_PR6) / ((Enzyme_con->Bper_GOA + KmGOA_PR6) * (Enzyme_con->Bper_SER + KmSER_PR6 * (1 + Enzyme_con->Bper_GLY / KmGLY_PR6)));
    theVars->Enzyme_Vel.vpr7 = Vm_PR7 * (Enzyme_con->Bper_HPR * Bper_NADH - Bper_NAD * Enzyme_con->Bper_GCEA / Ke_PR7) / (Enzyme_con->Bper_HPR + KmHPR_PR7 * (1 + Enzyme_con->Bper_HPR / KiHPR_PR7));
    theVars->Enzyme_Vel.vpr9 = VTgca_PR9 * (Enzyme_con->Bchl_GCA / (Enzyme_con->Bchl_GCA + KmGCA_PR9 * (1 + Enzyme_con->Bchl_GCEA / KiGCEA_PR9)) - Enzyme_con->Bper_GCA / (Enzyme_con->Bper_GCA + KmGCA_PR9 * (1 + Enzyme_con->Bper_GCEA / KiGCEA_PR9)));

    double VTgcea_PR10M;
    double Vm_PR8M;
    if (!Pvpr8M) {
        Vm_PR8M = 0;
        VTgcea_PR10M = 0;
    } else {
        Vm_PR8M = Vm_PR8;
        VTgcea_PR10M = VTgcea_PR10;
        Vm_PR8 = 0;
        VTgcea_PR10 = 0;
    }

    theVars->Enzyme_Vel.vpr8 = Vm_PR8 * (Enzyme_con->Bchl_ATP * Enzyme_con->Bchl_GCEA - Bchl_ADP * Enzyme_con->Bchl_PGA / Ke_PR8) / ((Enzyme_con->Bchl_ATP + KmATP_PR8 * (1 + Enzyme_con->Bchl_PGA / KiPGA_PR8)) * (Enzyme_con->Bchl_GCEA + KmGCEA_PR8));

    theVars->Enzyme_Vel.vpr10 = VTgcea_PR10 * (Enzyme_con->Bper_GCEA / (Enzyme_con->Bper_GCEA + KmGCEA_PR10 * (1 + Enzyme_con->Bper_GCA / KiGCA_PR10)) - Enzyme_con->Bchl_GCEA / (Enzyme_con->Bchl_GCEA + KmGCEA_PR10 * (1 + Enzyme_con->Bchl_GCA / KiGCA_PR10)));

    theVars->Enzyme_Vel.vpr10M = VTgcea_PR10M * (Enzyme_con->Bper_GCEA / (Enzyme_con->Bper_GCEA + KmGCEA_PR10 * (1 + Enzyme_con->Bper_GCA / KiGCA_PR10)) - Enzyme_con->Mchl_GCEA / (Enzyme_con->Mchl_GCEA + KmGCEA_PR10));
    theVars->Enzyme_Vel.vpr8M = Vm_PR8M * (Enzyme_con->Mchl_ATP * Enzyme_con->Mchl_GCEA - Mchl_ADP * Enzyme_con->Mchl_PGA / Ke_PR8) / ((Enzyme_con->Mchl_ATP + KmATP_PR8 * (1 + Enzyme_con->Mchl_PGA / KiPGA_PR8)) * (Enzyme_con->Mchl_GCEA + KmGCEA_PR8));


    //PGA enolase, phosphoglyceromutase in MC PGA->PEP
    theVars->Enzyme_Vel.v62 = Vm_62 * (Enzyme_con->MC_PGA - Enzyme_con->MC_PEP / Ke_62) / (KmPGA_62 * (1 + Enzyme_con->MC_PGA / KmPGA_62 + Enzyme_con->MC_PEP / KmPEP_62));

    // KmATP_gly1 = 0.081;// --unused
    // KmF6P_gly1 = 0.1;// --unused
    // Ke_gly1 = 2800;// --unused
    // Vm_gly1B = 0.035;// --unused
    theVars->Enzyme_Vel.vgly1B = 0;//Vm_gly1B*(Bchl_F6P*Enzyme_con->Bchl_ATP-Enzyme_con->Bchl_FBP*Bchl_ADP/Ke_gly1)/(KmF6P_gly1+Bchl_F6P)/(KmATP_gly1+Enzyme_con->Bchl_ATP);

    ////////////////////
    //PCK pathway
    ////////////////////
    //2.6.1.1M   PCK1
    const double KmAsp_PCK1 = 2.5;
    const double KmOxog_PCK1 = 0.14;
    const double KmGlu_PCK1 = 17;
    const double KmOAA_PCK1 = 0.056;
    const double Ke_PCK1 = 1 / 0.148;
    //2.6.1.1B   PCK2
    const double KmAsp_PCK2 = 2.5;
    const double KmOxog_PCK2 = 0.14;
    const double KmGlu_PCK2 = 17;
    const double KmOAA_PCK2 = 0.056;
    const double Ke_PCK2 = 0.148;
    //4.1.1.49   PCK3
    const double KmOAA_PCK3 = 0.06;
    const double KmATP_PCK3 = 0.034 * 3;
    //2.6.1.2B   PCK4
    const double KmPyr_PCK4 = 0.33;
    const double KmGlu_PCK4 = 5;
    const double KmAla_PCK4 = 6.67;
    const double KmOxog_PCK4 = 0.15;
    const double Ke_PCK4 = 1;
    //2.6.1.2M   PCK5
    const double KmPyr_PCK5 = 0.33;
    const double KmGlu_PCK5 = 5;
    const double KmAla_PCK5 = 6.67;
    const double KmOxog_PCK5 = 0.15;
    const double Ke_PCK5 = 1;
    //1.1.1.82B  PCK6
    const double KmNADPH_PCK6 = 0.05;
    const double KmOAA_PCK6 = 0.056;
    const double KmNADP_PCK6 = 0.045;
    const double Kmmal_PCK6 = 32.0;
    const double Ke_PCK6 = 4450.0; // No unit
    //global Ratio;
    const double Vm_PCK1 = 1.5 / 20 * Ratio;
    const double Vm_PCK2 = 1.5 / 20 * Ratio;
    const double Vm_PCK4 = 1.5 / 20 * Ratio;
    const double Vm_PCK5 = 1.5 / 20 * Ratio;
    const double Vm_PCK6 = 0.4 / 20 * Ratio;
    const double Vm_PCK3 = 0.15 / 20 * Ratio;
    theVars->Enzyme_Vel.vPCK1 = Vm_PCK1 * (Enzyme_con->MC_OAA * Enzyme_con->MC_Glu - Enzyme_con->MC_OxoG * Enzyme_con->MC_Asp / Ke_PCK1) / (KmOAA_PCK1 * KmGlu_PCK1 * (1 + Enzyme_con->MC_OAA / KmOAA_PCK1 + Enzyme_con->MC_Glu / KmGlu_PCK1 + Enzyme_con->MC_OxoG / KmOxog_PCK1 + Enzyme_con->MC_Asp / KmAsp_PCK1 + Enzyme_con->MC_OAA / KmOAA_PCK1 * Enzyme_con->MC_Glu / KmGlu_PCK1 + Enzyme_con->MC_OxoG / KmOxog_PCK1 * Enzyme_con->MC_Asp / KmAsp_PCK1));
    theVars->Enzyme_Vel.vPCK2 = Vm_PCK2 * (Enzyme_con->BSC_OxoG * Enzyme_con->BSC_Asp - Enzyme_con->BSC_OAA * Enzyme_con->BSC_Glu / Ke_PCK2) / (KmOxog_PCK2 * KmAsp_PCK2 * (1 + Enzyme_con->BSC_OAA / KmOAA_PCK2 + Enzyme_con->BSC_Glu / KmGlu_PCK2 + Enzyme_con->BSC_OxoG / KmOxog_PCK2 + Enzyme_con->BSC_Asp / KmAsp_PCK2 + Enzyme_con->BSC_OAA / KmOAA_PCK2 * Enzyme_con->BSC_Glu / KmGlu_PCK2 + Enzyme_con->BSC_OxoG / KmOxog_PCK2 * Enzyme_con->BSC_Asp / KmAsp_PCK2));
    theVars->Enzyme_Vel.vPCK3 = Vm_PCK3 * Enzyme_con->BSC_OAA * Enzyme_con->BSC_ATP / ((Enzyme_con->BSC_OAA + KmOAA_PCK3) * (Enzyme_con->BSC_ATP + KmATP_PCK3));
    theVars->Enzyme_Vel.vPCK4 = Vm_PCK4 * (Enzyme_con->BSC_Glu * Enzyme_con->BSC_Pyruvate - Enzyme_con->BSC_Ala * Enzyme_con->BSC_OxoG / Ke_PCK4) / (KmGlu_PCK4 * KmPyr_PCK4 * (1 + Enzyme_con->BSC_Glu / KmGlu_PCK4 + Enzyme_con->BSC_Pyruvate / KmPyr_PCK4 + Enzyme_con->BSC_Ala / KmAla_PCK4 + Enzyme_con->BSC_OxoG / KmOxog_PCK4 + Enzyme_con->BSC_Glu / KmGlu_PCK4 * Enzyme_con->BSC_Pyruvate / KmPyr_PCK4 + Enzyme_con->BSC_Ala / KmAla_PCK4 * Enzyme_con->BSC_OxoG / KmOxog_PCK4));
    theVars->Enzyme_Vel.vPCK5 = Vm_PCK5 * (Enzyme_con->MC_Ala * Enzyme_con->MC_OxoG - Enzyme_con->MC_Glu * Enzyme_con->MC_Pyruvate / Ke_PCK5) / (KmAla_PCK5 * KmOxog_PCK5 * (1 + Enzyme_con->MC_Glu / KmGlu_PCK5 + Enzyme_con->MC_Pyruvate / KmPyr_PCK5 + Enzyme_con->MC_Ala / KmAla_PCK5 + Enzyme_con->MC_OxoG / KmOxog_PCK5 + Enzyme_con->MC_Glu / KmGlu_PCK5 * Enzyme_con->MC_Pyruvate / KmPyr_PCK5 + Enzyme_con->MC_Ala / KmAla_PCK5 * Enzyme_con->MC_OxoG / KmOxog_PCK5));
    theVars->Enzyme_Vel.vPCK6 = Vm_PCK6 * (Enzyme_con->Bchl_OAA * Enzyme_con->Bchl_NADPH - Bchl_NADP * Enzyme_con->Bchl_Malate / Ke_PCK6) / (KmOAA_PCK6 * KmNADPH_PCK6 * (1 + Enzyme_con->Bchl_OAA / KmOAA_PCK6 + Enzyme_con->Bchl_NADPH / KmNADPH_PCK6 + Bchl_NADP / KmNADP_PCK6 + Enzyme_con->Bchl_Malate / Kmmal_PCK6 + Enzyme_con->Bchl_OAA * Enzyme_con->Bchl_NADPH / (KmOAA_PCK6 * KmNADPH_PCK6) + Bchl_NADP * Enzyme_con->Bchl_Malate / (KmNADP_PCK6 * Kmmal_PCK6)));
    //////////////////////////
    //Transport for PCK
    //////////////////////////
    //Tasp
    //global vAsp;
    theVars->Enzyme_Vel.vAsp = 0.664 / 20 * (Enzyme_con->MC_Asp - Enzyme_con->BSC_Asp);
    //TAla
    theVars->Enzyme_Vel.vAla = 0.8715 / 20 * (Enzyme_con->BSC_Ala - Enzyme_con->MC_Ala);
    //TPEP
    theVars->Enzyme_Vel.vPEP = 0.653625 / 20 * (Enzyme_con->BSC_PEP - Enzyme_con->MC_PEP);
    //TOAAB
    theVars->Enzyme_Vel.vOAA_B = 1 * (Enzyme_con->BSC_OAA - Enzyme_con->Bchl_OAA);
    //ATPB
    const double VtATP = 3;
    theVars->Enzyme_Vel.vATP_B = VtATP * (Enzyme_con->Bchl_ATP - 2 * Enzyme_con->BSC_ATP);
    //////////////////////////////////////////////////////////////////////
    //NAD-ME pathway
    //////////////////////////////////////////////////////////////////////
    //1.1.1.37 MDH_BM
    //KeMDH_BM=1/(6.65*10^(-5));
    const double Vm_MDH_BM = 0.3;
    const double KmOAA_MDH_BM = 0.126;//Maize
    const double KmNADH_MDH_BM = 0.025;
    //1.1.1.38 NADME
    const double VmNADME = 2;
    //KeNADME=0.0011*55.35*1000;// Wedding, R.T.; Black, M.K.; Plant Physiol.; 72, 1021 (1983).
    const double Km_MAL_NADME = 3;
    const double Km_NAD_NADME = 0.5;
    const double Ki_NADH_NADME = 0.15;
    const double Ki_PYR_NADME = 14;

    theVars->Enzyme_Vel.vMDH_BM = Vm_MDH_BM * Enzyme_con->Bmito_OAA * Enzyme_con->Bmito_NADH / (KmOAA_MDH_BM + Enzyme_con->Bmito_OAA) / (KmNADH_MDH_BM + Enzyme_con->Bmito_NADH);
    theVars->Enzyme_Vel.vNADME = VmNADME * Enzyme_con->Bmito_MAL * Bmito_NAD / (Km_MAL_NADME * (1 + Enzyme_con->Bmito_PYR / Ki_PYR_NADME) + Enzyme_con->Bmito_MAL) / (Km_NAD_NADME * (1 + Enzyme_con->Bmito_NADH / Ki_NADH_NADME) + Bmito_NAD);
    theVars->Enzyme_Vel.vtOAA_Bm = 1 * (Enzyme_con->BSC_OAA - Enzyme_con->Bmito_OAA);
    theVars->Enzyme_Vel.vPYR_Bm = 1 * (Enzyme_con->Bmito_PYR - Enzyme_con->BSC_Pyruvate);
    theVars->Enzyme_Vel.vMAL_Bm = 1 * (Enzyme_con->BSC_Malate - Enzyme_con->Bmito_MAL);

    //////////O2 diffusion////////

    theVars->Enzyme_Vel.vtO2 = 0.122633 * (Enzyme_con->BSC_O2 - Enzyme_con->MC_O2);
    theVars->Enzyme_Vel.vtO2_B = Pco2_B * (Enzyme_con->Bchl_O2 - Enzyme_con->BSC_O2);
    theVars->Enzyme_Vel.vtO2_M = Pco2_B * (Enzyme_con->Mchl_O2 - Enzyme_con->MC_O2);
    const double Bchl_Asp = Enzyme_con->BSC_Asp;
    const double Bchl_Ala = Enzyme_con->BSC_Ala;
    const double Mchl_Asp = Enzyme_con->MC_Asp;
    const double Mchl_Ala = Enzyme_con->MC_Ala;

    theVars->Enzyme_Vel.vAspB = 10 * (Enzyme_con->BSC_Asp - Bchl_Asp);
    theVars->Enzyme_Vel.vAlaB = 10 * (Enzyme_con->BSC_Ala - Bchl_Ala);
    theVars->Enzyme_Vel.vAspM = 10 * (Enzyme_con->MC_Asp - Mchl_Asp);
    theVars->Enzyme_Vel.vAlaM = 10 * (Enzyme_con->MC_Ala - Mchl_Ala);

    //global pathway_option;
    if (pathway_option == 0) {
        theVars->Enzyme_Vel.vPCK1 = 0;
        theVars->Enzyme_Vel.vPCK2 = 0;
        theVars->Enzyme_Vel.vPCK3 = 0;
        theVars->Enzyme_Vel.vPCK4 = 0;
        theVars->Enzyme_Vel.vPCK5 = 0;
        theVars->Enzyme_Vel.vPCK6 = 0;
        theVars->Enzyme_Vel.vAsp = 0;
        theVars->Enzyme_Vel.vAla = 0;
        //    vPEP=0;
        theVars->Enzyme_Vel.vOAA_B = 0;
        theVars->Enzyme_Vel.vATP_B = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 1) {
        theVars->Enzyme_Vel.vPCK3 = 0;
        theVars->Enzyme_Vel.vATP_B = 0;
        theVars->Enzyme_Vel.vPEP = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 2) {
        theVars->Enzyme_Vel.vPCK6 = 0;
        theVars->Enzyme_Vel.vOAA_B = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 3) {
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 4) {
        theVars->Enzyme_Vel.v3 = 0;
        theVars->Enzyme_Vel.v4 = 0;
        theVars->Enzyme_Vel.v5 = 0;
        theVars->Enzyme_Vel.vPCK6 = 0;
        theVars->Enzyme_Vel.vPYR_B = 0;
        theVars->Enzyme_Vel.vPYR_M = 0;
        theVars->Enzyme_Vel.vOAA_B = 0;
        theVars->Enzyme_Vel.vOAA_M = 0;
        theVars->Enzyme_Vel.vMAL_M = 0;
        theVars->Enzyme_Vel.vMAL_B = 0;
        theVars->Enzyme_Vel.vPEP_M = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 6) {
        theVars->Enzyme_Vel.vMAL_B = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vNADME = 0;
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vPYR_Bm = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
    } else if (pathway_option == 7) {
        theVars->Enzyme_Vel.v3 = 0;
        theVars->Enzyme_Vel.vOAA_M = 0;
        theVars->Enzyme_Vel.vMAL_M = 0;
        theVars->Enzyme_Vel.vMAL = 0;
        theVars->Enzyme_Vel.vMAL_B = 0;
        theVars->Enzyme_Vel.vPYR_B = 0;
        theVars->Enzyme_Vel.v4 = 0;
        theVars->Enzyme_Vel.vPCK6 = 0;
        theVars->Enzyme_Vel.vOAA_B = 0;
        theVars->Enzyme_Vel.vPCK3 = 0;
        theVars->Enzyme_Vel.vMAL_Bm = 0;
        theVars->Enzyme_Vel.vPYR = 0;
    } else if (pathway_option == 8) {
        theVars->Enzyme_Vel.vtOAA_Bm = 0;
        theVars->Enzyme_Vel.vMDH_BM = 0;
        theVars->Enzyme_Vel.vOAA_B = 0;
        theVars->Enzyme_Vel.v4 = 0;
        theVars->Enzyme_Vel.vPCK6 = 0;
        theVars->Enzyme_Vel.vMAL_B = 0;
        theVars->Enzyme_Vel.vPYR_B = 0;
        vNAE = theVars->Enzyme_Vel.vNADME;// 1NAD-ME-> 2.5 ATP
    }


#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->Enzyme_Vel)
#endif
    if (theVars->record) {
        if (t > Enzyme::TIME) {
            Enzyme::N++;
            Enzyme::TIME = t;
        }

        theVars->Enzyme_VEL.insert(Enzyme::N - 1, t, theVars->Enzyme_Vel);
    }

    //global enzyme_flux;


    //if (TIME_N == 0)
    //    theVars->TIME_M = 1;


    //if (t > theVars->OLD_TIME_M) {
    //    theVars->TIME_M = theVars->TIME_M + 1;
    //    theVars->OLD_TIME_M = t;
    //}
    //theVars->Meta_VEL[theVars->TIME_M][0] = t;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][1] = vinf;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][2] = theVars->v1;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][3] = theVars->v2;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][4] = theVars->v3;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][5] = theVars->v4;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][6] = theVars->v5;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][7] = theVars->v6;  // --unused
    //theVars->Meta_VEL[theVars->TIME_M][8] = theVars->vpr1;  // --unused
}
