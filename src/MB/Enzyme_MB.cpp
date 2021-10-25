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
const double Bchl_TotalRubisco = 0.0159;


EnzymeCondition* Enzyme::_MB_con(const double t, const EnzymeCondition* Enzyme_con, Variables *theVars) {

    Rate(t, Enzyme_con, theVars);
    vNAE = 0.;

    const double vATPsynthase_Act_MchlF = std::max(theVars->AE_Vel.vATPsynthase_Act_Mchl, 0.);
    const double vNADPMDH_ActF = std::max(theVars->AE_Vel.vNADPMDH_Act, 0.);
    const double vGAPDH_Act_MchlF = std::max(theVars->AE_Vel.vGAPDH_Act_Mchl, 0.);
    const double vATPsynthase_Act_BchlF = std::max(theVars->AE_Vel.vATPsynthase_Act_Bchl, 0.);
    const double vGAPDH_Act_BchlF = std::max(theVars->AE_Vel.vGAPDH_Act_Bchl, 0.);
    const double vFBPase_ActF = std::max(theVars->AE_Vel.vFBPase_Act, 0.);
    const double vSBPase_ActF = std::max(theVars->AE_Vel.vSBPase_Act, 0.);
    const double vPRK_ActF = std::max(theVars->AE_Vel.vPRK_Act, 0.);
    const double vRubisco_ActF = std::max(theVars->AE_Vel.vRubisco_Act, 0.);

    //     if vPEPC_Act>=0
    //     vPEPC_ActF=vPEPC_Act;
    //     else
    //     vPEPC_ActF=0;
    //     end

    EnzymeCondition* dydt = new EnzymeCondition();

    dydt->MC_HCO3 = (theVars->Enzyme_Vel.v1 - theVars->Enzyme_Vel.v2) / VolMC;
    dydt->MC_OAA = (theVars->Enzyme_Vel.v2 - theVars->Enzyme_Vel.vOAA_M -
                    theVars->Enzyme_Vel.vPCK1) / VolMC;
    dydt->MC_PEP = (theVars->Enzyme_Vel.vPEP_M - theVars->Enzyme_Vel.v2 +
                    theVars->Enzyme_Vel.v62 + theVars->Enzyme_Vel.vPEP) / VolMC;
    dydt->MC_Malate = (theVars->Enzyme_Vel.vMAL_M - theVars->Enzyme_Vel.vMAL) / VolMC;
    dydt->MC_Pyruvate = (theVars->Enzyme_Vel.vPYR - theVars->Enzyme_Vel.vPYR_M +
                         theVars->Enzyme_Vel.vPCK5) / VolMC;
    dydt->MC_PGA = (theVars->Enzyme_Vel.vPGA - theVars->Enzyme_Vel.vPGA_M - theVars->Enzyme_Vel.v62 -
                    theVars->Enzyme_Vel.vPGASink) / VolMC;
    dydt->MC_FBP = (theVars->Enzyme_Vel.vSuc1 - theVars->Enzyme_Vel.vSuc2) / VolMC;
    dydt->MC_UDPG = (theVars->Enzyme_Vel.vSuc7 - theVars->Enzyme_Vel.vSuc8) / VolMC;
    dydt->MC_SUCP = (theVars->Enzyme_Vel.vSuc8 - theVars->Enzyme_Vel.vSuc9) / VolMC;
    dydt->MC_SUC = (theVars->Enzyme_Vel.vSuc9 - theVars->Enzyme_Vel.vSuc10) / VolMC;
    dydt->MC_F26BP = (theVars->Enzyme_Vel.vSuc3 - theVars->Enzyme_Vel.vSuc4) / VolMC;
    dydt->MC_ATP = (theVars->Enzyme_Vel.vtATP - theVars->Enzyme_Vel.vSuc7 -
                    theVars->Enzyme_Vel.vSuc3) / VolMC;
    dydt->MC_T3P = (theVars->Enzyme_Vel.vGAP_M + theVars->Enzyme_Vel.vDHAP_M -
                    theVars->Enzyme_Vel.vGAP - theVars->Enzyme_Vel.vDHAP - 2 *
                    theVars->Enzyme_Vel.vSuc1) / VolMC;
    dydt->MC_HexP = (theVars->Enzyme_Vel.vSuc2 + theVars->Enzyme_Vel.vSuc4 -
                     theVars->Enzyme_Vel.vSuc3 - theVars->Enzyme_Vel.vSuc7 -
                     theVars->Enzyme_Vel.vSuc8) / VolMC;
    dydt->MC_Sucrose = (theVars->Enzyme_Vel.vSuc10) / VolMC;
    dydt->Mchl_OAA = (theVars->Enzyme_Vel.vOAA_M - theVars->Enzyme_Vel.v3) / VolMchl;
    dydt->Mchl_Malate = (theVars->Enzyme_Vel.v3 - theVars->Enzyme_Vel.vMAL_M) / VolMchl;
    dydt->Mchl_PEP = (theVars->Enzyme_Vel.v5 - theVars->Enzyme_Vel.vPEP_M) / VolMchl;
    dydt->Mchl_Pyruvate = (theVars->Enzyme_Vel.vPYR_M - theVars->Enzyme_Vel.v5) / VolMchl;
    dydt->Mchl_NADPH = (theVars->Enzyme_Vel.vNADPHM - theVars->Enzyme_Vel.v3 -
                        theVars->Enzyme_Vel.v78Mchl - vATPsynthase_Act_MchlF *
                        Mchl_TotalATPsynthase - vNADPMDH_ActF * Mchl_TotalNADPMDH -
                        vGAPDH_Act_MchlF * Mchl_TotalGAPDH) / VolMchl;//WY1911
    dydt->Mchl_ATP = (theVars->Enzyme_Vel.vATPM - 2. * theVars->Enzyme_Vel.v5 -
                      theVars->Enzyme_Vel.v78Mchl - theVars->Enzyme_Vel.vtATP -
                      theVars->Enzyme_Vel.vpr8M) / VolMchl;
    dydt->Mchl_PGA = (theVars->Enzyme_Vel.vPGA_M - theVars->Enzyme_Vel.v78Mchl) / VolMchl;
    dydt->Mchl_DPGA = 0.;
    dydt->Mchl_T3P = (theVars->Enzyme_Vel.v78Mchl - theVars->Enzyme_Vel.vGAP_M -
                      theVars->Enzyme_Vel.vDHAP_M) / VolMchl;
    dydt->BSC_T3P = (theVars->Enzyme_Vel.vGAP + theVars->Enzyme_Vel.vDHAP -
                     theVars->Enzyme_Vel.vGAP_B - theVars->Enzyme_Vel.vDHAP_B) / VolBSC;
    dydt->BSC_PGA = (theVars->Enzyme_Vel.vPGA_B - theVars->Enzyme_Vel.vPGA) / VolBSC;
    //dydt->BSC_Malate = (theVars->Enzyme_Vel.vMAL - theVars->Enzyme_Vel.vMAL_B) / VolBSC;
    dydt->BSC_Malate = (theVars->Enzyme_Vel.vMAL - theVars->Enzyme_Vel.vMAL_B -
                        theVars->Enzyme_Vel.vMAL_Bm) / VolBSC;
    //dydt->BSC_Pyruvate= (theVars->Enzyme_Vel.vPYR_B - theVars->Enzyme_Vel.vPYR - theVars->Enzyme_Vel.vPCK4) /
    //                    VolBSC;
    dydt->BSC_Pyruvate = (theVars->Enzyme_Vel.vPYR_B - theVars->Enzyme_Vel.vPYR -
                          theVars->Enzyme_Vel.vPCK4 + theVars->Enzyme_Vel.vPYR_Bm) / VolBSC;
    //dydt->BSC_CO2= (theVars->Enzyme_Vel.vpr5 + theVars->Enzyme_Vel.vleak_B - theVars->Enzyme_Vel.vleak +
    //                theVars->vrpd + theVars->Enzyme_Vel.vPCK3)/VolBSC;
    //dydt->BSC_CO2= (theVars->Enzyme_Vel.vpr5 + theVars->Enzyme_Vel.vleak_B - theVars->Enzyme_Vel.vleak +
    //                theVars->Enzyme_Vel.vPCK3 + theVars->Enzyme_Vel.vNADME)/VolBSC;
    dydt->BSC_CO2 = (theVars->Enzyme_Vel.vpr5 + theVars->Enzyme_Vel.vleak_B -
                     theVars->Enzyme_Vel.vleak + theVars->vrpd + theVars->Enzyme_Vel.vPCK3 +
                     theVars->Enzyme_Vel.vNADME) / VolBSC;
    dydt->Bchl_CO2 = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.v6 -
                      theVars->Enzyme_Vel.vleak_B) / VolBchl;
    dydt->Bchl_RuBP = (theVars->Enzyme_Vel.v18 - theVars->Enzyme_Vel.v6 -
                       theVars->Enzyme_Vel.vpr1) / VolBchl;
    dydt->Bchl_PGA = (2. * theVars->Enzyme_Vel.v6 - theVars->Enzyme_Vel.v78 -
                      theVars->Enzyme_Vel.vPGA_B + theVars->Enzyme_Vel.vpr1 +
                      theVars->Enzyme_Vel.vpr8) / VolBchl;
    dydt->Bchl_DPGA = 0.;
    //dydt->Bchl_ATP = (theVars->Enzyme_Vel.vATPB - theVars->Enzyme_Vel.v78 - theVars->Enzyme_Vel.v18 -
    //                  theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vpr8 - theVars->Enzyme_Vel.vgly1B -
    //                  theVars->Enzyme_Vel.vATP_B - 2. * theVars->Enzyme_Vel.v5B) / VolBchl;////////////WY

    dydt->Bchl_ATP = (theVars->Enzyme_Vel.vATPB - theVars->Enzyme_Vel.v78 -
                      theVars->Enzyme_Vel.v18 - theVars->Enzyme_Vel.vSta1 -
                      theVars->Enzyme_Vel.vpr8 - theVars->Enzyme_Vel.vgly1B -
                      theVars->Enzyme_Vel.vATP_B - 2 * theVars->Enzyme_Vel.v5B -
                      vRubisco_ActF * Bchl_TotalRubisco) / VolBchl;////////////WY1911
    dydt->Bchl_NADPH = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.v78 -
                        theVars->Enzyme_Vel.vPCK6 + theVars->Enzyme_Vel.vNADPHB -
                        vATPsynthase_Act_BchlF * Bchl_TotalATPsynthase - vGAPDH_Act_BchlF *
                        Bchl_TotalGAPDH - vFBPase_ActF * Bchl_TotalFBPase - vSBPase_ActF *
                        Bchl_TotalSBPase - vPRK_ActF * Bchl_TotalPRK) / VolBchl;//+ vNADPHB;
    dydt->Bchl_SBP = (theVars->Enzyme_Vel.v12 - theVars->Enzyme_Vel.v13) / VolBchl;
    dydt->Bchl_S7P = (theVars->Enzyme_Vel.v13 - theVars->Enzyme_Vel.v15) / VolBchl;
    dydt->Bchl_FBP = (theVars->Enzyme_Vel.v10 - theVars->Enzyme_Vel.v11 +
                      theVars->Enzyme_Vel.vgly1B) / VolBchl;
    dydt->Bchl_E4P = (theVars->Enzyme_Vel.v14 - theVars->Enzyme_Vel.v12) / VolBchl;
    dydt->Bchl_Starch = theVars->Enzyme_Vel.vSta3 / VolBchl;
    dydt->Bchl_Rubisco = 0.;
    dydt->Bchl_T3P = (theVars->Enzyme_Vel.vGAP_B + theVars->Enzyme_Vel.vDHAP_B +
                      theVars->Enzyme_Vel.v78 - 2. * theVars->Enzyme_Vel.v10 -
                      theVars->Enzyme_Vel.v14 - theVars->Enzyme_Vel.v15 -
                      theVars->Enzyme_Vel.v12) / VolBchl;
    dydt->Bchl_HexP = (theVars->Enzyme_Vel.v11 - theVars->Enzyme_Vel.v14 -
                       theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vgly1B +
                       theVars->Enzyme_Vel.vhexp) / VolBchl;
    dydt->Bchl_Pent = (theVars->Enzyme_Vel.v14 + 2. * theVars->Enzyme_Vel.v15 -
                       theVars->Enzyme_Vel.v18) / VolBchl;
    dydt->Bchl_Malate = (theVars->Enzyme_Vel.vMAL_B - theVars->Enzyme_Vel.v4 +
                         theVars->Enzyme_Vel.vPCK6) / VolBchl;
    dydt->Bchl_Pyruvate = (theVars->Enzyme_Vel.v4 - theVars->Enzyme_Vel.vPYR_B -
                           theVars->Enzyme_Vel.v5B) / VolBchl;////////WY
    dydt->Bchl_PGCA = (theVars->Enzyme_Vel.vpr1 - theVars->Enzyme_Vel.vpr2) / VolBchl;
    dydt->Bchl_GCA = (theVars->Enzyme_Vel.vpr2 - theVars->Enzyme_Vel.vpr9) / VolBchl;
    dydt->Bchl_GCEA = (theVars->Enzyme_Vel.vpr10 - theVars->Enzyme_Vel.vpr8) / VolBchl;
    dydt->Bper_GCA = (theVars->Enzyme_Vel.vpr9 - theVars->Enzyme_Vel.vpr3) / Volper;
    dydt->Bper_GOA = (theVars->Enzyme_Vel.vpr3 - theVars->Enzyme_Vel.vpr4 -
                      theVars->Enzyme_Vel.vpr6) / Volper;
    dydt->Bper_GLY = (theVars->Enzyme_Vel.vpr4 + theVars->Enzyme_Vel.vpr6 - 2. *
                      theVars->Enzyme_Vel.vpr5) / Volper;
    dydt->Bper_SER = (theVars->Enzyme_Vel.vpr5 - theVars->Enzyme_Vel.vpr6) / Volper;
    dydt->Bper_HPR = (theVars->Enzyme_Vel.vpr6 - theVars->Enzyme_Vel.vpr7) / Volper;
    dydt->Bper_GCEA = (theVars->Enzyme_Vel.vpr7 - theVars->Enzyme_Vel.vpr10) / Volper;
    dydt->MC_CO2 = (theVars->Enzyme_Vel.vinf - theVars->Enzyme_Vel.v1 +
                    theVars->Enzyme_Vel.vleak + theVars->vrpd) / VolMC;
    dydt->Bchl_PPi = (theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vSta2) / VolBchl;
    dydt->Bchl_ADPG = (theVars->Enzyme_Vel.vSta1 - theVars->Enzyme_Vel.vSta3) / VolBchl;

    dydt->MC_Glu = (theVars->Enzyme_Vel.vPCK5 - theVars->Enzyme_Vel.vPCK1) / VolMC;
    dydt->MC_OxoG = (theVars->Enzyme_Vel.vPCK1 - theVars->Enzyme_Vel.vPCK5) / VolMC;
    dydt->MC_Asp = (theVars->Enzyme_Vel.vPCK1 - theVars->Enzyme_Vel.vAsp) / VolMC;
    dydt->MC_Ala = (theVars->Enzyme_Vel.vAla - theVars->Enzyme_Vel.vPCK5) / VolMC;

    dydt->BSC_OxoG = (theVars->Enzyme_Vel.vPCK4 - theVars->Enzyme_Vel.vPCK2) / VolBSC;
    dydt->BSC_Glu = (theVars->Enzyme_Vel.vPCK2 - theVars->Enzyme_Vel.vPCK4) / VolBSC;
    dydt->BSC_Asp = (theVars->Enzyme_Vel.vAsp - theVars->Enzyme_Vel.vPCK2) / VolBSC;
    dydt->BSC_Ala = (theVars->Enzyme_Vel.vPCK4 - theVars->Enzyme_Vel.vAla) / VolBSC;
    //dydt->BSC_OAA = (theVars->Enzyme_Vel.vPCK2 - theVars->Enzyme_Vel.vOAA_B - theVars->Enzyme_Vel.vPCK3) /
    //                VolBSC;
    dydt->BSC_OAA = (theVars->Enzyme_Vel.vPCK2 - theVars->Enzyme_Vel.vOAA_B -
                     theVars->Enzyme_Vel.vPCK3 - theVars->Enzyme_Vel.vtOAA_Bm) / VolBSC;
    dydt->BSC_PEP = (theVars->Enzyme_Vel.vPCK3 - theVars->Enzyme_Vel.vPEP +
                     theVars->Enzyme_Vel.vPEP_B) / VolBSC;////////////WY
    //dydt->BSC_ATP = theVars->Enzyme_Vel.vATP_B - theVars->Enzyme_Vel.vPCK3;
    dydt->BSC_ATP = (theVars->Enzyme_Vel.vATP_B - theVars->Enzyme_Vel.vPCK3 + vNAE * 2.5) / VolBSC;
    dydt->Bchl_OAA = (theVars->Enzyme_Vel.vOAA_B - theVars->Enzyme_Vel.vPCK6) / VolBchl;
    if (!option2) {
        dydt->MC_O2 = 0.;
        dydt->Mchl_O2 = 0.;
        dydt->BSC_O2 = 0.;
        dydt->Bchl_O2 = 0.;
    } else {
        dydt->MC_O2 = 0.;
        dydt->Mchl_O2 = theVars->Enzyme_Vel.vO2_Mchl - theVars->Enzyme_Vel.vtO2_M;
        dydt->BSC_O2 = theVars->Enzyme_Vel.vtO2_B - theVars->Enzyme_Vel.vtO2;
        dydt->Bchl_O2 = theVars->Enzyme_Vel.vO2_Bchl - theVars->Enzyme_Vel.vtO2_B;
    }
    dydt->Bchl_PEP = (theVars->Enzyme_Vel.v5B - theVars->Enzyme_Vel.vPEP_B) / VolBchl; //WY PPDK in BS
    dydt->Mchl_GCEA = (theVars->Enzyme_Vel.vpr10M - theVars->Enzyme_Vel.vpr8M) / VolMchl;
    dydt->Bmito_OAA = (theVars->Enzyme_Vel.vtOAA_Bm - theVars->Enzyme_Vel.vMDH_BM) / Volmito;
    dydt->Bmito_MAL = (theVars->Enzyme_Vel.vMDH_BM + theVars->Enzyme_Vel.vMAL_Bm -
                       theVars->Enzyme_Vel.vNADME) / Volmito;
    dydt->Bmito_PYR = (theVars->Enzyme_Vel.vNADME - theVars->Enzyme_Vel.vPYR_Bm) / Volmito;
    dydt->Bmito_CO2 = 0.;
    dydt->Bmito_NADH = (theVars->Enzyme_Vel.vNADME - theVars->Enzyme_Vel.vMDH_BM - vNAE) / Volmito;
    dydt->Bchl_Asp = theVars->Enzyme_Vel.vAspB;
    dydt->Bchl_Ala = theVars->Enzyme_Vel.vAlaB;
    dydt->Mchl_Asp = theVars->Enzyme_Vel.vAspM;
    dydt->Mchl_Ala = theVars->Enzyme_Vel.vAlaM;
    dydt->E_PPDK_Mchl = (theVars->Enzyme_Vel.vEA_PPDKRP_A - theVars->Enzyme_Vel.vEA_PPDKRP_I) / VolMchl;
    dydt->EP_PPDK_Mchl = (theVars->Enzyme_Vel.vEA_PPDKRP_I - theVars->Enzyme_Vel.vEA_PPDKRP_A) / VolMchl;

    return dydt;
}
