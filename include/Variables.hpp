#pragma once

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
#include <map>
#include "definitions.hpp"
#include "FI.hpp"
#include "BF.hpp"
#include "PS.hpp"
#include "PR.hpp"
#include "RROEA.hpp"
#include "RuACT.hpp"
#include "XanCycle.hpp"
#include "RedoxReg.hpp"
#include "SUCS.hpp"
#include "FIBF.hpp"

/**
  Structure to hold global variables
  */
struct Variables {
    bool record = false;
    bool BF_FI_com = false;
    bool EPS_SUCS_com = false;
    bool FIBF_PSPR_com = false;
    bool PR_PS_com = false;
    bool PSPR_SUCS_com = false;
    bool RROEA_EPS_com = false;
    bool RedoxReg_RA_com = false;
    bool RuACT_EPS_com = false;
    bool XanCycle_BF_com = false;

    int GP = 0;
    int GRNC = 0;
    int GRNT = 0;
    double BF_OLD_TIME = 0;
    size_t BF_TIME_N = 1;
    double FI_OLD_TIME = 0;
    size_t FI_TIME_N = 1;
    double PR_OLD_TIME = 0;
    size_t PR_TIME_N = 1;
    double PS_OLD_TIME = 0;
    size_t PS_TIME_N = 1;
    double PS_PR_OLDTIME = 0;
    size_t PS_PR_TIME_N = 1;
    double RROEA_OLD_TIME = 0;
    size_t RROEA_TIME_N = 1;
    double RedoxReg_OLD_TIME = 0;
    size_t RedoxReg_TIME_N = 1;
    double RuACT_OLD_TIME = 0;
    size_t RuACT_TIME_N = 1;
    double SUCS_OLD_TIME = 0;
    size_t SUCS_TIME_N = 1;
    double XanCycle_OLD_TIME = 0;
    size_t XanCycle_TIME_N = 1;

    const int RUBISCOMETHOD = 1;
    int TestSucPath = 0.;
    int trDynaPS2RedReg_cal = 0.;

    const double AVR = 30;
    const double Fd_Thio_ET = 500;
    const double HPR = 4.66;
    const double input_LHCI = 6;
    const double input_LHCII = 13;
    const double input_PSI = 1;
    const double input_PSIIcore = 1;
    const double O2 = 210;
    const double PSIIantennaSize = 37;
    const double PSIantennaSize = 95;
    const double ThioT = 0.5;
    const double Thio_Oxidation = 0.1;
    const double VolRatioStCyto = 1;

    double cATPsyn = 0.;
    double CPSi = 0.;
    double cNADPHsyn = 0.;
    //double LI = 0.;
    double cpsii = 0;

    double BF2RROEA_FdT = 0.;
    double BF2RROEA_Fdn = 0.;
    double BF2RedoxReg_Fdt = 0.;
    double BF2XanCycle_pHl = 0.;
    double CO2_cond = 0.;
    double ChlPSI = 0.;
    double ChlT = 0.;
    double ChlT2 = 0.;
    double EPS_ATP_Rate = 0.;
    double FIBF2FI_PQ = 0.;
    double FIBF2FI_PQa = 0.;
    double FIBF_RA_Mg = 0.;
    double GLUc = 0.;
    double GLight = 0.;
    double GammaStar = 0.;
    double Glight = 0.;
    double KM11 = 0.;
    double KM12 = 0.;
    double O2_cond = 0.;
    double PS12ratio = 0.;
    double PS2BF_ADP = 0.;
    double PS2BF_Pi = 0.;
    double PS2PR_ADP = 0.;
    double PS2PR_ATP = 0.;
    double PS2PR_Pi = 0.;
    double PS2RA_ATP = 0.;
    double PS2RA_RuBP_ini = 0.;
    double PS2SUCSV32 = 0.;
    double PS2EPS_v3 = 0;
    double PSPR_RA_CA = 0.;
    double PsV1 = 0.;
    double RROEA2RuACT_RuAC = 0.;
    double RUBISCOTOTAL = 0.;
    double Redox2PS_V13 = 0.;
    double Redox2PS_V16 = 0.;
    double Redox2PS_V6 = 0.;
    double Redox2PS_V9 = 0.;
    double RedoxReg_Param = 0;
    double SUCS2PS_Pic = 0.;
    double TestATPCost = 0.;
    double TestCa = 0.;
    double TestLi = 0.;
    double V1 = 0.;
    double V13 = 0.;
    double V16 = 0.;
    double V1Reg = 0.;
    double V1T = 0.;
    double V2 = 0.;
    double V23 = 0.;
    double V3 = 0.;
    double V6 = 0.;
    double V9 = 0.;
    double XanCycle2FIBF_Xstate = 0.;
    double activase = 0.;
    double kmCO2 = 0.;
    double kmO2 = 0.;
    double PS_PR_Param = 0;

    double Tp = 0;
    double alfa = 0;
    double fc = 0;
    double Theta = 0;
    double beta = 0;
    double Jmax = 0.;
    // Parameters
    arr PR_Param = zeros(2);
    arr BF_Param = zeros(2);
    arr FI_PARAM = zeros(2);
    arr FI_Param = zeros(2);
    arr RROEA_Param = zeros(2);
    arr RuACT_Param = zeros(2);
    arr SUCS_Param = zeros(2);
    arr XanCycle_Param = zeros(2);

    // Vel
    BFVel BF_Vel;
    FIVel FI_Vel;
    PRVel PR_Vel;
    PSVel PS_Vel;
    RROEAVel RROEA_Vel;
    RedoxRegVel RedoxReg_Vel;
    RuACTVel RuACT_Vel;
    SUCSVel SUCS_Vel;
    XanCycleVel XanCycle_Vel;

    // Ratio
    arr BFRatio = ones(49);
    arr FIRatio = ones(23);
    arr PRRatio = ones(48);
    arr PSRatio = ones(103);
    arr RacRatio = ones(16);
    arr SUCRatio = ones(66);
    arr XanRatio = ones(4);

    std::map<std::string, double> EnzymeAct;
    arr BFVmax = zeros(18);
    arr FIVmax = zeros(21);


    arr VfactorCp = zeros(33);
    arr VfactorT = ones(28);

    // Pool
    BFPool BF_Pool;
    FIBFPool FIBF_Pool;
    FIPool FI_Pool;
    RROEAPool RROEA_Pool;
    RuACTPool RuACT_Pool;
    SUCSPool SUCS_Pool;

    // RC
    BFRC BF_RC;
    FIRC FI_RC;
    RROEARC RROEA_RC;
    RuACTRC RuACT_RC;

    // OUT
    arr BF2OUT = zeros(5);
    PRCon PR2OUT;
    PSCon PS2OUT;
    SUCSCon SUCS2OUT;
    XanCycleCon XanCycle2OUT;

    // misc
    arr FluxTR;
    RROEAKE RROEA_KE;

    TimeSeries<BFVel> BF_VEL = TimeSeries<BFVel>();
    TimeSeries<std::vector<double> > CO2A = TimeSeries<std::vector<double> > ();
    TimeSeries<FIVel> FI_VEL = TimeSeries<FIVel> ();
    TimeSeries<PRVel> PR_VEL = TimeSeries<PRVel> ();
    TimeSeries<PSVel> PS_VEL = TimeSeries<PSVel> ();
    TimeSeries<RROEAVel> RROEA_VEL = TimeSeries<RROEAVel> ();
    std::vector<arr> RedoxReg_MP;
    TimeSeries<RedoxRegVel> RedoxReg_VEL = TimeSeries<RedoxRegVel> ();
    TimeSeries<RuACTVel> RuACT_VEL = TimeSeries<RuACTVel> ();
    TimeSeries<SUCSVel> SUCS_VEL = TimeSeries<SUCSVel> ();
    TimeSeries<XanCycleVel> XanCycle_VEL = TimeSeries<XanCycleVel> ();

    bool useC3 = false;
};
