#include "Variables.hpp"
#include "globals.hpp"
#include "modules/Enzyme.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double VolMC = 0.01;
const double VolMchl = 0.02;
const double VolBSC = 0.0045;
const double VolBchl = 0.009;
const double Volper = 0.00045;
const double Volmito = 0.00045;
const bool option2 = true;  //O2 diffusion
const double Bchl_TotalATPsynthase = 0.00545;
// Bchl_TotalRca = 0.00160;// --unused
const double Bchl_TotalGAPDH = 0.00328;
const double Bchl_TotalFBPase = 0.00286;
const double Bchl_TotalSBPase = 0.0006;
const double Bchl_TotalPRK = 0.00130;
const double Mchl_TotalATPsynthase = 0.00545;
const double Mchl_TotalGAPDH = 0.00492;
const double Mchl_TotalNADPMDH = 0.00006;

EnzymeCondition* Enzyme::_MB_con(const double t, const EnzymeCondition* Enzyme_con, Variables *theVars) {

    Rate(t, Enzyme_con, theVars);

    const double Delta_MC_CO2 = (theVars->Enzyme_Vel.vinf - theVars->Enzyme_Vel.v1 + theVars->Enzyme_Vel.vleak + theVars->vrpd) / VolMC;
    const double Delta_MC_HCO3 = (theVars->Enzyme_Vel.v1 - theVars->Enzyme_Vel.v2) / VolMC;
    const double Delta_MC_Malate = (theVars->Enzyme_Vel.vMAL_M - theVars->Enzyme_Vel.vMAL) / VolMC;
    const double Delta_MC_PGA = (theVars->Enzyme_Vel.vPGA - theVars->Enzyme_Vel.vPGA_M - theVars->Enzyme_Vel.v62 - theVars->Enzyme_Vel.vPGASink) / VolMC;
    const double Delta_MC_FBP = (theVars->Enzyme_Vel.vSuc1 - theVars->Enzyme_Vel.vSuc2) / VolMC;
    const double Delta_MC_UDPG = (theVars->Enzyme_Vel.vSuc7 - theVars->Enzyme_Vel.vSuc8) / VolMC;
    const double Delta_MC_SUCP = (theVars->Enzyme_Vel.vSuc8 - theVars->Enzyme_Vel.vSuc9) / VolMC;
    const double Delta_MC_SUC = (theVars->Enzyme_Vel.vSuc9 - theVars->Enzyme_Vel.vSuc10) / VolMC;
    const double Delta_MC_F26BP = (theVars->Enzyme_Vel.vSuc3 - theVars->Enzyme_Vel.vSuc4) / VolMC;
    const double Delta_MC_ATP = (theVars->Enzyme_Vel.vtATP - theVars->Enzyme_Vel.vSuc7 - theVars->Enzyme_Vel.vSuc3) / VolMC;
    const double Delta_MC_T3P = (theVars->Enzyme_Vel.vGAP_M + theVars->Enzyme_Vel.vDHAP_M - theVars->Enzyme_Vel.vGAP - theVars->Enzyme_Vel.vDHAP - 2 * theVars->Enzyme_Vel.vSuc1) / VolMC;
    const double Delta_MC_HexP = (theVars->Enzyme_Vel.vSuc2 + theVars->Enzyme_Vel.vSuc4 - theVars->Enzyme_Vel.vSuc3 - theVars->Enzyme_Vel.vSuc7 - theVars->Enzyme_Vel.vSuc8) / VolMC;
    const double Delta_MC_Sucrose = (theVars->Enzyme_Vel.vSuc10) / VolMC;

    const double Delta_Mchl_OAA = (theVars->Enzyme_Vel.vOAA_M - theVars->Enzyme_Vel.v3) / VolMchl;
    const double Delta_Mchl_Malate = (theVars->Enzyme_Vel.v3 - theVars->Enzyme_Vel.vMAL_M) / VolMchl;
    const double Delta_Mchl_PEP = (theVars->Enzyme_Vel.v5 - theVars->Enzyme_Vel.vPEP_M) / VolMchl;
    const double Delta_Mchl_Pyruvate = (theVars->Enzyme_Vel.vPYR_M - theVars->Enzyme_Vel.v5) / VolMchl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double vATPsynthase_Act_MchlF;
    if (theVars->AE_Vel.vATPsynthase_Act_Mchl >= 0) {
        vATPsynthase_Act_MchlF = theVars->AE_Vel.vATPsynthase_Act_Mchl;
    } else {
        vATPsynthase_Act_MchlF = 0;
    }
    double vNADPMDH_ActF;
    if (theVars->AE_Vel.vNADPMDH_Act >= 0) {
        vNADPMDH_ActF = theVars->AE_Vel.vNADPMDH_Act;
    } else {
        vNADPMDH_ActF = 0;
    }
    double vGAPDH_Act_MchlF;
    if (theVars->AE_Vel.vGAPDH_Act_Mchl >= 0) {
        vGAPDH_Act_MchlF = theVars->AE_Vel.vGAPDH_Act_Mchl;
    } else {
        vGAPDH_Act_MchlF = 0;
    }
    const double Delta_Mchl_NADPH = (theVars->Enzyme_Vel.vNADPHM - theVars->Enzyme_Vel.v3 - theVars->Enzyme_Vel.v78Mchl - vATPsynthase_Act_MchlF * Mchl_TotalATPsynthase - vNADPMDH_ActF * Mchl_TotalNADPMDH - vGAPDH_Act_MchlF * Mchl_TotalGAPDH) / VolMchl;//WY1911
    const double Delta_Mchl_ATP = (theVars->Enzyme_Vel.vATPM - 2 * theVars->Enzyme_Vel.v5 - theVars->Enzyme_Vel.v78Mchl - theVars->Enzyme_Vel.vtATP - theVars->Enzyme_Vel.vpr8M) / VolMchl;
    const double Delta_Mchl_PGA = (theVars->Enzyme_Vel.vPGA_M - theVars->Enzyme_Vel.v78Mchl) / VolMchl;
    const double Delta_Mchl_DPGA = 0;
    const double Delta_Mchl_T3P = (theVars->Enzyme_Vel.v78Mchl - theVars->Enzyme_Vel.vGAP_M - theVars->Enzyme_Vel.vDHAP_M) / VolMchl;

    const double Delta_BSC_T3P = (theVars->Enzyme_Vel.vGAP + theVars->Enzyme_Vel.vDHAP - theVars->Enzyme_Vel.vGAP_B - theVars->Enzyme_Vel.vDHAP_B) / VolBSC;
    const double Delta_BSC_PGA = (theVars->Enzyme_Vel.vPGA_B - theVars->Enzyme_Vel.vPGA) / VolBSC;

    const double Delta_Bchl_CO2 = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.v6 - theVars->Enzyme_Vel.vleak_B) / VolBchl;
    const double Delta_Bchl_RuBP = (theVars->Enzyme_Vel.v18 - theVars->Enzyme_Vel.v6 - theVars->Enzyme_Vel.vpr1) / VolBchl;
    const double Delta_Bchl_PGA = (2 * theVars->Enzyme_Vel.v6 - theVars->Enzyme_Vel.v78 - theVars->Enzyme_Vel.vPGA_B + theVars->Enzyme_Vel.vpr1 + theVars->Enzyme_Vel.vpr8) / VolBchl;
    const double Delta_Bchl_DPGA = 0;
    const double Delta_Bchl_SBP = (theVars->Enzyme_Vel.v12 - theVars->Enzyme_Vel.v13) / VolBchl;
    const double Delta_Bchl_S7P = (theVars->Enzyme_Vel.v13 - theVars->Enzyme_Vel.v15) / VolBchl;
    const double Delta_Bchl_FBP = (theVars->Enzyme_Vel.v10 - theVars->Enzyme_Vel.v11 + theVars->Enzyme_Vel.vgly1B) / VolBchl;
    const double Delta_Bchl_E4P = (theVars->Enzyme_Vel.v14 - theVars->Enzyme_Vel.v12) / VolBchl;
    const double Delta_Bchl_Starch = theVars->Enzyme_Vel.vSta3 / VolBchl;
    const double Delta_Bchl_Rubisco = 0;
    const double Delta_Bchl_T3P = (theVars->Enzyme_Vel.vGAP_B + theVars->Enzyme_Vel.vDHAP_B + theVars->Enzyme_Vel.v78 - 2 * theVars->Enzyme_Vel.v10 - theVars->Enzyme_Vel.v14 - theVars->Enzyme_Vel.v15 - theVars->Enzyme_Vel.v12) / VolBchl;
    const double Delta_Bchl_HexP = (theVars->Enzyme_Vel.v11 - theVars->Enzyme_Vel.v14 - theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vgly1B + theVars->Enzyme_Vel.vhexp) / VolBchl;
    const double Delta_Bchl_Pent = (theVars->Enzyme_Vel.v14 + 2 * theVars->Enzyme_Vel.v15 - theVars->Enzyme_Vel.v18) / VolBchl;
    const double Delta_Bchl_Pyruvate = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.vPYR_B - theVars->Enzyme_Vel.v5B) / VolBchl;////////WY
    const double Delta_Bchl_PPi = (theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vSta2) / VolBchl;
    const double Delta_Bchl_ADPG = (theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vSta3) / VolBchl;
    const double Delta_Bchl_PGCA = (theVars->Enzyme_Vel.vpr1 - theVars->Enzyme_Vel.vpr2) / VolBchl;
    const double Delta_Bchl_GCA = (theVars->Enzyme_Vel.vpr2 - theVars->Enzyme_Vel.vpr9) / VolBchl;
    const double Delta_Bchl_GCEA = (theVars->Enzyme_Vel.vpr10 - theVars->Enzyme_Vel.vpr8) / VolBchl;

    const double Delta_Bper_GCA = (theVars->Enzyme_Vel.vpr9 - theVars->Enzyme_Vel.vpr3) / Volper;
    const double Delta_Bper_GOA = (theVars->Enzyme_Vel.vpr3 - theVars->Enzyme_Vel.vpr4 - theVars->Enzyme_Vel.vpr6) / Volper;
    const double Delta_Bper_GLY = (theVars->Enzyme_Vel.vpr4 + theVars->Enzyme_Vel.vpr6 - 2 * theVars->Enzyme_Vel.vpr5) / Volper;
    const double Delta_Bper_SER = (theVars->Enzyme_Vel.vpr5 - theVars->Enzyme_Vel.vpr6) / Volper;
    const double Delta_Bper_HPR = (theVars->Enzyme_Vel.vpr6 - theVars->Enzyme_Vel.vpr7) / Volper;
    const double Delta_Bper_GCEA = (theVars->Enzyme_Vel.vpr7 - theVars->Enzyme_Vel.vpr10) / Volper;

    double Delta_MC_O2 = 0;
    double Delta_Mchl_O2 = theVars->Enzyme_Vel.vO2_Mchl - theVars->Enzyme_Vel.vtO2_M;
    double Delta_BSC_O2 = theVars->Enzyme_Vel.vtO2_B - theVars->Enzyme_Vel.vtO2;
    double Delta_Bchl_O2 = theVars->Enzyme_Vel.vO2_Bchl - theVars->Enzyme_Vel.vtO2_B;

    const double Delta_MC_OAA = (theVars->Enzyme_Vel.v2 - theVars->Enzyme_Vel.vOAA_M - theVars->Enzyme_Vel.vPCK1) / VolMC;
    const double Delta_MC_Pyruvate = (theVars->Enzyme_Vel.vPYR - theVars->Enzyme_Vel.vPYR_M + theVars->Enzyme_Vel.vPCK5) / VolMC;
    const double Delta_MC_PEP = (theVars->Enzyme_Vel.vPEP_M - theVars->Enzyme_Vel.v2 + theVars->Enzyme_Vel.v62 + theVars->Enzyme_Vel.vPEP) / VolMC;

    const double Delta_BSC_PEP = (theVars->Enzyme_Vel.vPCK3 - theVars->Enzyme_Vel.vPEP + theVars->Enzyme_Vel.vPEP_B) / VolBSC;////////////WY

    double vATPsynthase_Act_BchlF;
    if (theVars->AE_Vel.vATPsynthase_Act_Bchl >= 0) {
        vATPsynthase_Act_BchlF = theVars->AE_Vel.vATPsynthase_Act_Bchl;
    } else {
        vATPsynthase_Act_BchlF = 0;
    }
    //     if vPEPC_Act>=0
    //     vPEPC_ActF=vPEPC_Act;
    //     else
    //     vPEPC_ActF=0;
    //     end
    double vGAPDH_Act_BchlF;
    if (theVars->AE_Vel.vGAPDH_Act_Bchl >= 0) {
        vGAPDH_Act_BchlF = theVars->AE_Vel.vGAPDH_Act_Bchl;
    } else {
        vGAPDH_Act_BchlF = 0;
    }
    double vFBPase_ActF;
    if (theVars->AE_Vel.vFBPase_Act >= 0) {
        vFBPase_ActF = theVars->AE_Vel.vFBPase_Act;
    } else {
        vFBPase_ActF = 0;
    }
    double vSBPase_ActF;
    if (theVars->AE_Vel.vSBPase_Act >= 0) {
        vSBPase_ActF = theVars->AE_Vel.vSBPase_Act;
    } else {
        vSBPase_ActF = 0;
    }
    double vPRK_ActF;
    if (theVars->AE_Vel.vPRK_Act >= 0) {
        vPRK_ActF = theVars->AE_Vel.vPRK_Act;
    } else {
        vPRK_ActF = 0;
    }

    const double Delta_Bchl_NADPH = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.v78 - theVars->Enzyme_Vel.vPCK6 + theVars->Enzyme_Vel.vNADPHB - vATPsynthase_Act_BchlF * Bchl_TotalATPsynthase - vGAPDH_Act_BchlF * Bchl_TotalGAPDH - vFBPase_ActF * Bchl_TotalFBPase - vSBPase_ActF * Bchl_TotalSBPase - vPRK_ActF * Bchl_TotalPRK) / VolBchl;//+ vNADPHB;

    const double Delta_Bchl_Malate = (theVars->Enzyme_Vel.vMAL_B - theVars->Enzyme_Vel.v4 + theVars->Enzyme_Vel.vPCK6) / VolBchl;
    const double Delta_Bchl_OAA = (theVars->Enzyme_Vel.vOAA_B - theVars->Enzyme_Vel.vPCK6) / VolBchl;
    //Delta_Bchl_ATP=(vATPB - v78 - v18 - vSta1 - vpr8-vgly1B-vATP_B-2*v5B)/VolBchl;////////////WY
    double vRubisco_ActF;
    if (theVars->AE_Vel.vRubisco_Act >= 0) {
        vRubisco_ActF = theVars->AE_Vel.vRubisco_Act;
    } else {
        vRubisco_ActF = 0;
    }
    const double Bchl_TotalRubisco = 0.0159;
    const double Delta_Bchl_ATP = (theVars->Enzyme_Vel.vATPB - theVars->Enzyme_Vel.v78 - theVars->Enzyme_Vel.v18 - theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vpr8 - theVars->Enzyme_Vel.vgly1B - theVars->Enzyme_Vel.vATP_B - 2 * theVars->Enzyme_Vel.v5B - vRubisco_ActF * Bchl_TotalRubisco) / VolBchl;////////////WY1911
    const double Delta_MC_Glu = (theVars->Enzyme_Vel.vPCK5 - theVars->Enzyme_Vel.vPCK1) / VolMC;
    const double Delta_MC_OxoG = (theVars->Enzyme_Vel.vPCK1 - theVars->Enzyme_Vel.vPCK5) / VolMC;
    const double Delta_MC_Asp = (theVars->Enzyme_Vel.vPCK1 - theVars->Enzyme_Vel.vAsp) / VolMC;
    const double Delta_MC_Ala = (theVars->Enzyme_Vel.vAla - theVars->Enzyme_Vel.vPCK5) / VolMC;
    const double Delta_BSC_OxoG = (theVars->Enzyme_Vel.vPCK4 - theVars->Enzyme_Vel.vPCK2) / VolBSC;
    const double Delta_BSC_Glu = (theVars->Enzyme_Vel.vPCK2 - theVars->Enzyme_Vel.vPCK4) / VolBSC;
    const double Delta_BSC_Asp = (theVars->Enzyme_Vel.vAsp - theVars->Enzyme_Vel.vPCK2) / VolBSC;
    const double Delta_BSC_Ala = (theVars->Enzyme_Vel.vPCK4 - theVars->Enzyme_Vel.vAla) / VolBSC;
    const double Delta_Bchl_PEP = (theVars->Enzyme_Vel.v5B - theVars->Enzyme_Vel.vPEP_B) / VolBchl;
    const double Delta_Mchl_GCEA = (theVars->Enzyme_Vel.vpr10M - theVars->Enzyme_Vel.vpr8M) / VolMchl;

    //    Delta_BSC_CO2= (vpr5+vleak_B - vleak+vrpd+vPCK3)/VolBSC;
    //    Delta_BSC_ATP=vATP_B-vPCK3;
    //    Delta_BSC_Pyruvate= (vPYR_B - vPYR-vPCK4)/VolBSC;
    //    Delta_BSC_OAA=(vPCK2-vOAA_B-vPCK3)/VolBSC;
    //    Delta_BSC_Malate= (vMAL - vMAL_B)/VolBSC;



    const double Delta_BSC_CO2 = (theVars->Enzyme_Vel.vpr5 + theVars->Enzyme_Vel.vleak_B - theVars->Enzyme_Vel.vleak + theVars->vrpd + theVars->Enzyme_Vel.vPCK3 + theVars->Enzyme_Vel.vNADME) / VolBSC;

    //   Delta_BSC_CO2= (vpr5+vleak_B - vleak+vPCK3+vNADME)/VolBSC;
    const double Delta_BSC_ATP = (theVars->Enzyme_Vel.vATP_B - theVars->Enzyme_Vel.vPCK3 + vNAE * 2.5) / VolBSC;

    const double Delta_BSC_Pyruvate = (theVars->Enzyme_Vel.vPYR_B - theVars->Enzyme_Vel.vPYR - theVars->Enzyme_Vel.vPCK4 + theVars->Enzyme_Vel.vPYR_Bm) / VolBSC;
    const double Delta_BSC_OAA = (theVars->Enzyme_Vel.vPCK2 - theVars->Enzyme_Vel.vOAA_B - theVars->Enzyme_Vel.vPCK3 - theVars->Enzyme_Vel.vtOAA_Bm) / VolBSC;
    const double Delta_BSC_Malate = (theVars->Enzyme_Vel.vMAL - theVars->Enzyme_Vel.vMAL_B - theVars->Enzyme_Vel.vMAL_Bm) / VolBSC;

    const double Delta_Bmito_OAA = (theVars->Enzyme_Vel.vtOAA_Bm - theVars->Enzyme_Vel.vMDH_BM) / Volmito;
    const double Delta_Bmito_MAL = (theVars->Enzyme_Vel.vMDH_BM + theVars->Enzyme_Vel.vMAL_Bm - theVars->Enzyme_Vel.vNADME) / Volmito;
    const double Delta_Bmito_PYR = (theVars->Enzyme_Vel.vNADME - theVars->Enzyme_Vel.vPYR_Bm) / Volmito;
    const double Delta_Bmito_CO2 = 0;
    const double Delta_Bmito_NADH = (theVars->Enzyme_Vel.vNADME - theVars->Enzyme_Vel.vMDH_BM - vNAE) / Volmito;
    const double Delta_Bchl_Asp = theVars->Enzyme_Vel.vAspB;
    const double Delta_Bchl_Ala = theVars->Enzyme_Vel.vAlaB;
    const double Delta_Mchl_Asp = theVars->Enzyme_Vel.vAspM;
    const double Delta_Mchl_Ala = theVars->Enzyme_Vel.vAlaM;

    const double Delta_E_PPDKdt = (theVars->Enzyme_Vel.vEA_PPDKRP_A - theVars->Enzyme_Vel.vEA_PPDKRP_I) / VolMchl;
    const double Delta_EP_PPDKdt = (theVars->Enzyme_Vel.vEA_PPDKRP_I - theVars->Enzyme_Vel.vEA_PPDKRP_A) / VolMchl;
    if (!option2) {
        Delta_MC_O2 = 0;
        Delta_Mchl_O2 = 0;
        Delta_BSC_O2 = 0;
        Delta_Bchl_O2 = 0;
    }


    EnzymeCondition* dydt = new EnzymeCondition();

    dydt->MC_HCO3 = Delta_MC_HCO3;
    dydt->MC_OAA = Delta_MC_OAA;
    dydt->MC_PEP = Delta_MC_PEP;
    dydt->MC_Malate = Delta_MC_Malate;
    dydt->MC_Pyruvate = Delta_MC_Pyruvate;
    dydt->MC_PGA = Delta_MC_PGA;
    dydt->MC_FBP = Delta_MC_FBP;
    dydt->MC_UDPG = Delta_MC_UDPG;
    dydt->MC_SUCP = Delta_MC_SUCP;
    dydt->MC_SUC = Delta_MC_SUC;
    dydt->MC_F26BP = Delta_MC_F26BP;
    dydt->MC_ATP = Delta_MC_ATP;
    dydt->MC_T3P = Delta_MC_T3P;
    dydt->MC_HexP = Delta_MC_HexP;
    dydt->MC_Sucrose = Delta_MC_Sucrose;
    dydt->Mchl_OAA = Delta_Mchl_OAA;
    dydt->Mchl_Malate = Delta_Mchl_Malate;
    dydt->Mchl_PEP = Delta_Mchl_PEP;
    dydt->Mchl_Pyruvate = Delta_Mchl_Pyruvate;
    dydt->Mchl_NADPH = Delta_Mchl_NADPH;
    dydt->Mchl_ATP = Delta_Mchl_ATP;
    dydt->Mchl_PGA = Delta_Mchl_PGA;
    dydt->Mchl_DPGA = Delta_Mchl_DPGA;
    dydt->Mchl_T3P = Delta_Mchl_T3P;
    dydt->BSC_T3P = Delta_BSC_T3P;
    dydt->BSC_PGA = Delta_BSC_PGA;
    dydt->BSC_Malate = Delta_BSC_Malate;
    dydt->BSC_Pyruvate = Delta_BSC_Pyruvate;
    dydt->BSC_CO2 = Delta_BSC_CO2;
    dydt->Bchl_CO2 = Delta_Bchl_CO2;
    dydt->Bchl_RuBP = Delta_Bchl_RuBP;
    dydt->Bchl_PGA = Delta_Bchl_PGA;
    dydt->Bchl_DPGA = Delta_Bchl_DPGA;
    dydt->Bchl_ATP = Delta_Bchl_ATP;
    dydt->Bchl_NADPH = Delta_Bchl_NADPH;
    dydt->Bchl_SBP = Delta_Bchl_SBP;
    dydt->Bchl_S7P = Delta_Bchl_S7P;
    dydt->Bchl_FBP = Delta_Bchl_FBP;
    dydt->Bchl_E4P = Delta_Bchl_E4P;
    dydt->Bchl_Starch = Delta_Bchl_Starch;
    dydt->Bchl_Rubisco = Delta_Bchl_Rubisco;
    dydt->Bchl_T3P = Delta_Bchl_T3P;
    dydt->Bchl_HexP = Delta_Bchl_HexP;
    dydt->Bchl_Pent = Delta_Bchl_Pent;
    dydt->Bchl_Malate = Delta_Bchl_Malate;
    dydt->Bchl_Pyruvate = Delta_Bchl_Pyruvate;
    dydt->Bchl_PGCA = Delta_Bchl_PGCA;
    dydt->Bchl_GCA = Delta_Bchl_GCA;
    dydt->Bchl_GCEA = Delta_Bchl_GCEA;
    dydt->Bper_GCA = Delta_Bper_GCA;
    dydt->Bper_GOA = Delta_Bper_GOA;
    dydt->Bper_GLY = Delta_Bper_GLY;
    dydt->Bper_SER = Delta_Bper_SER;
    dydt->Bper_HPR = Delta_Bper_HPR;
    dydt->Bper_GCEA = Delta_Bper_GCEA;
    dydt->MC_CO2 = Delta_MC_CO2;
    dydt->Bchl_PPi = Delta_Bchl_PPi;
    dydt->Bchl_ADPG = Delta_Bchl_ADPG;

    dydt->MC_Glu = Delta_MC_Glu;
    dydt->MC_OxoG = Delta_MC_OxoG;
    dydt->MC_Asp = Delta_MC_Asp;
    dydt->MC_Ala = Delta_MC_Ala;

    dydt->BSC_OxoG = Delta_BSC_OxoG;
    dydt->BSC_Glu = Delta_BSC_Glu;
    dydt->BSC_Asp = Delta_BSC_Asp;
    dydt->BSC_Ala = Delta_BSC_Ala;
    dydt->BSC_OAA = Delta_BSC_OAA;
    dydt->BSC_PEP = Delta_BSC_PEP;
    dydt->BSC_ATP = Delta_BSC_ATP;
    dydt->Bchl_OAA = Delta_Bchl_OAA;
    dydt->MC_O2 = Delta_MC_O2;
    dydt->Mchl_O2 = Delta_Mchl_O2;
    dydt->BSC_O2 = Delta_BSC_O2;
    dydt->Bchl_O2 = Delta_Bchl_O2;
    dydt->Bchl_PEP = Delta_Bchl_PEP;//////WY PPDK in BS
    dydt->Mchl_GCEA = Delta_Mchl_GCEA;
    dydt->Bmito_OAA = Delta_Bmito_OAA;
    dydt->Bmito_MAL = Delta_Bmito_MAL;
    dydt->Bmito_PYR = Delta_Bmito_PYR;
    dydt->Bmito_CO2 = Delta_Bmito_CO2;
    dydt->Bmito_NADH = Delta_Bmito_NADH;
    dydt->Bchl_Asp = Delta_Bchl_Asp;
    dydt->Bchl_Ala = Delta_Bchl_Ala;
    dydt->Mchl_Asp = Delta_Mchl_Asp;
    dydt->Mchl_Ala = Delta_Mchl_Ala;
    dydt->E_PPDK_Mchl = Delta_E_PPDKdt;
    dydt->EP_PPDK_Mchl = Delta_EP_PPDKdt;


    return dydt;
}
