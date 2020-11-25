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

RROEACon RROEA_Ini(Variables *myVars) {
    myVars->RROEA_OLD_TIME = 0;
    myVars->RROEA_TIME_N = 1;

    double Coeff = 1;

    // The rate constant used in the model
    myVars->RROEA_RC.ke2GAPDH = 22 / 60 * Coeff;	//	The rate constant of electron transfer to GAPDH. From literature.
    myVars->RROEA_RC.ke2MDH = 20 / 60 * Coeff;		//	The rate constant of electront transfer to MDH, this rate is totally ASSUMED.
    myVars->RROEA_RC.ke2FBPase = 1.38 / 60 * Coeff;	//	The rate constant of electron transfer from thioredoxin to FBPase.
    myVars->RROEA_RC.ke2SBPase = 1.65 / 60 * Coeff;	//	The rate constant of electron tranfer from thioredoxin to SBPase
    myVars->RROEA_RC.ke2PRK = 59.8 / 60 * Coeff;	    //	The rate constant of electron transfer from thioredoxin to PRK, Phosphoribulase kinase
    myVars->RROEA_RC.ke2RubACT = 6.35 / 60 * Coeff;	//	The rate constant of electron transfer from thioredoxin to Rubisco activase
    myVars->RROEA_RC.ke2Fd = 18.5 * Coeff;	    //	The rate constant of electron transfer to fe
    // Jmax of 180 micro mole per meter square per second.
    myVars->RROEA_RC.keFd2Thio = 10 * Coeff;	//	The rate constant of electron transfer from fd to thio
    myVars->RROEA_RC.keFd2Calvin = 7 * Coeff;	    //	The rate constant of electron transfer from fd to Calvin cycle
    // Of course, this is a big assumption where the electron is transfered to NADPH
    // then to the Calvin cycle. This rate is much higher than the electron transfer to differnet
    // enzymes.

    myVars->RROEA_RC.ke2ATPGPP = 6.3 / 60 * Coeff;	    //	The rate constant of electron transfer to ATPGPP

    // Here is all the equilibriun constants for the different reactions in photosystem
    myVars->RROEA_KE.KEe2FBPase = 0.311167869;
    myVars->RROEA_KE.KEe2SBPase = 0.459194309;
    myVars->RROEA_KE.KEe2PRK = 0.677638775;
    myVars->RROEA_KE.KEe2ATPase = 2.177727336;
    myVars->RROEA_KE.KEe2RuACT = 0.677638775;
    myVars->RROEA_KE.KEe2GAPDH = 0.044461692;
    myVars->RROEA_KE.KEe2MDH = 0.044461692;
    myVars->RROEA_KE.KEe2ATPGPP = 1.;
    myVars->RROEA_KE.KEeFd2Thio = 24776.;

    // The following calculate the total concentration of different enzymes.

    const double MDH_Vmax = 2;       // This value is assumed and there is no literature about it. Need to be
    // fixed.

    double SC;
    double fc16;
    double FC;
    if (!myVars->RROEA_EPS_com) {

        FC = 1;
        fc16 = 1;
        SC = 1;

        myVars->V3 = 5.04 * SC;//	(Harris & Koniger, 1997)	3	GAP dehydragenase	DPGA+NADPH <->GAP + OP+NADP
        myVars->V6 = 1.155 * SC;//	(Harris & Koniger, 1997)	6	FBPase	FBP<->F6P+OP    1.155
        myVars->V9 = 0.168 * SC * FC;//	(Harris & Koniger, 1997)	9	SBPase	SBP<->S7P+OP    0.168 as original value; 0.4168 was its value.
        myVars->V13 = 8.0094 * SC;//	(Harris & Koniger, 1997)	13	Ribulosebiphosphate kinase	Ru5P+ATP<->RuBP+ADP
        myVars->V16 = 3 * SC * fc16;//	(Aflalo & Shavit, 1983, Davenport & McLeod, 1986)	16	ATP synthase	ADP+Pi<->ATP    1.47
        myVars->V23 = 1.68 * SC * FC;//	(Latzko, Steup & Schachtele, 1981)	23	ADP-glucose pyrophosphorylase and	ADPG+Gn<->G(n+1)+ADP 0.18
    }


    const double NA = 100;

    const double SA_GAPDH = 620;
    const double SA_MDH = 184;
    const double SA_PRK = 410;
    const double SA_FBPase = 119;
    const double SA_SBPase = 70;
    const double SA_ATPGPP = 10;

    const double SA_ATPase = NA;

    const double mw_GAPDH = 147000;
    const double mw_MDH = 87000;
    const double mw_PRK = 40000;

    const double mw_FBPase = 195000;
    const double mw_SBPase = 66000;
    const double mw_ATPGPP = 210000;
    const double mw_ATPase = 500000;

    double FdT = 0.081;

    if (myVars->RROEA_EPS_com)
        FdT = myVars->BF2RROEA_FdT;

    myVars->RROEA_Pool.GAPDH = myVars->V3 * 1000 * 60 / SA_GAPDH / mw_GAPDH;
    myVars->RROEA_Pool.FBPase = myVars->V6 * 1000 * 60 / SA_FBPase / mw_FBPase;
    myVars->RROEA_Pool.SBPase = myVars->V9 * 1000 * 60 / SA_SBPase / mw_SBPase;
    myVars->RROEA_Pool.PRK = myVars->V13 * 1000 * 60 / SA_PRK / mw_PRK;
    myVars->RROEA_Pool.ATPase = myVars->V16 * 1000 * 60 / SA_ATPase / mw_ATPase;
    myVars->RROEA_Pool.ATPGPP = myVars->V23 * 1000 * 60 / SA_ATPGPP / mw_ATPGPP;
    myVars->RROEA_Pool.MDH = MDH_Vmax * 1000 * 60 / SA_MDH / mw_MDH;
    myVars->RROEA_Pool.ThioT = 0.081;
    myVars->RROEA_Pool.FdT = FdT;
    myVars->RROEA_Pool.RuACTT = 0.0056;


    Coeff = 0.3;

    const double GAPDH = myVars->RROEA_Pool.GAPDH * Coeff;// 	The concentration of active GAPDH
    const double SBPase = myVars->RROEA_Pool.SBPase * Coeff;// 	The concentration of active SBPase
    const double PRK = myVars->RROEA_Pool.PRK * Coeff;//   The concentration of active PRK
    const double ATPase = myVars->RROEA_Pool.ATPase * Coeff;//   The concentratino of active ATP synthase
    const double ATPGPP = myVars->RROEA_Pool.ATPGPP * Coeff;//   The concnetratin of active ATP Glucose pyrophosphorylas
    double Fd = 0.081 * Coeff;// The initial concentraiton of reduced fd

    if (myVars->RROEA_EPS_com)
        Fd = myVars->BF2RROEA_Fdn;

    RROEACon RROEA_con;
    RROEA_con.GAPDH = myVars->RROEA_Pool.GAPDH * Coeff;	//	The initial concentration of active GAPDH
    RROEA_con.FBPase = myVars->RROEA_Pool.FBPase * Coeff;	//	The initial concentration of active FBPase
    RROEA_con.SBPase = myVars->RROEA_Pool.SBPase * Coeff;	//	The initial concentration of active SBPase
    RROEA_con.PRK = myVars->RROEA_Pool.PRK * Coeff;	//	The initial concentration of actove PRK
    RROEA_con.ATPase = myVars->RROEA_Pool.ATPase * Coeff;	//	The initial concentration of actove ATPase
    RROEA_con.ATPGPP = myVars->RROEA_Pool.ATPGPP * Coeff;	//	The initial concentration of actove ATPGPP
    RROEA_con.MDH = 0;	//	The initial concentration of actove MDH
    RROEA_con.Thio = 0.081 * Coeff;	//	The initial concentration of reduced thioredoxin
    RROEA_con.Fd = Fd;	//	The initial concentration of reduced ferrodoxin
    RROEA_con.RuACT = 0.0056 * Coeff;	//	The initial concentration of active Rubisco activase

    // Here defines the information for transfer between models

    myVars->RROEA2PS_GAPDH = GAPDH;
    myVars->RROEA2PS_SBPase = SBPase;
    myVars->RROEA2PS_PRK = PRK;
    myVars->RROEA2PS_ATPase = ATPase;
    myVars->RROEA2PS_ATPGPP = ATPGPP;
    return RROEA_con;
}
