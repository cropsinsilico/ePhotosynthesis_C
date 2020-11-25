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

// This is the routine that initialize the parameters, initial conditions for simulation of fluorescence induction curve.
// The following information is initialized sequentially 1) Rate constants; 2) Initial concentration ( or conditions); 3) THe maximum
// concentration of components of photosystems.

FICon FI_Ini(Variables *myVars) {
    //////////////////////////////////////////////////////////////////////////
    // Initilization of the rate constant //
    ////////////////////////////////////////////////////////////////////////////

    // The rate constant used in the model
    // Reference

    const double kA_d = 2 * pow(10, 8) * myVars->FIRatio[0];
    const double kA_f = 6.3 * pow(10, 6) * 0.2 * myVars->FIRatio[1];
    const double kA_U = pow(10, 10) * myVars->FIRatio[2];
    const double kU_A = pow(10, 10) * myVars->FIRatio[3];
    const double kU_d = 2 * pow(10, 8) * myVars->FIRatio[4];
    const double kU_f = 6.3 * pow(10, 6) * 0.2 * myVars->FIRatio[5];

    const double k1 = 2.5 * pow(10, 11) * myVars->FIRatio[6];
    const double k_r1 = 3 * pow(10, 8) * myVars->FIRatio[7];
    const double kz = 5 * pow(10, 6) * myVars->FIRatio[8];
    const double k12 = 30000 * myVars->FIRatio[9];//	The rate constant of the S1 to S2 transition	Lazar (1999); 0.667~33.3 * 10^3
    const double k23 = 10000 * myVars->FIRatio[10];//	The rate constant of the S2 to S3 transition	Lazar (1999); 0.667~33.3 * 10^3
    const double k30 = 3000 * myVars->FIRatio[11];//	The rate constant of the S3 to S0 transition	Lazar (1999); 0.667~33.3 * 10^3
    const double k01 = 500 * myVars->FIRatio[12];//	The rate constant of the S0 to S1 transition	Lazar (1999); 0.667~33.3 * 10^3

    const double k2 = 2 * pow(10, 9) * myVars->FIRatio[13];
    const double kAB1 = 2500 * myVars->FIRatio[14];//	The rate constant of QAQB-->QAQB-	Lazar (1999); 2.5~5 * 10^3
    const double kBA1 = 200 * myVars->FIRatio[15];//	The rate constant of the QAQB- -->QAQB	Lazar (1999)
    const double kAB2 = 3300 * myVars->FIRatio[16];//	The rate constant of the QAQB- --> QAQB2-	Lazar (1999); 1.25~3.33 * 10^3
    const double kBA2 = 250 * myVars->FIRatio[17];//	The rate constant of the QAQB2- --> QAQB- 	Lazar (1999), or same as kAB2 depend on the equilibium constant
    const double k3 = 800 * myVars->FIRatio[18];//	The rate constant of the exchange of PQ and QBH2	Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
    const double k_r3 = 80 * myVars->FIRatio[19];//	The rate constant of the exchange of QB and PQH2	Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
    const double k_pq_oxy = 500 * myVars->FIRatio[20];//	The rate constant of the PQH2 oxidation	Lazar (1999),50~500

    // The rate constant used in the model
    myVars->FI_RC.kA_d = kA_d;	//	The rate constant of heat dissipation from peripheral antenna	Lazar (1999), 0.25~1 *10^(9)
    myVars->FI_RC.kA_f = kA_f;	//	The rate constant of fluorescence emission from peripheral antenna	Lazar 1999, with a lifetime of 5 ns at closed reaction center
    myVars->FI_RC.kA_U = kA_U;	//	The rate constant of exciton transfer from periphral antenna to core antenna	Reference needed, a guess
    myVars->FI_RC.kU_A = kU_A;	//	The rate constant of exciton transfer from core antenna to peripheral antenna	Reference needed, a guess
    myVars->FI_RC.kU_d = kU_d;	//	The rate constant of  heat emission from core antenna
    myVars->FI_RC.kU_f = kU_f;	//	The rate constant of fluorescence emission from core antenna
    myVars->FI_RC.k1 = k1;	//	The rate constant of primary charge separation for open reaction center
    myVars->FI_RC.k_r1 = k_r1;	//	The rate constant of charge recombination for open reactoin center
    myVars->FI_RC.kz = kz;	//	The rate constant of the Tyrosine oxidation	Lazar (1999); 3.8~50 * 10^6
    myVars->FI_RC.k12 = k12;	//	The rate constant of the S1 to S2 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC.k23 = k23;	//	The rate constant of the S2 to S3 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC.k30 = k30;	//	The rate constant of the S3 to S0 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC.k01 = k01;	//	The rate constant of the S0 to S1 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC.k2 = k2;	//	The rate constant of the QA reduction by Pheo-	Lazar (1999); 2~2.3 * 10^9
    myVars->FI_RC.kAB1 = kAB1;	//	The rate constant of QAQB-->QAQB-	Lazar (1999); 2.5~5 * 10^3
    myVars->FI_RC.kBA1 = kBA1;	//	The rate constant of the QAQB- -->QAQB	Lazar (1999)
    myVars->FI_RC.kAB2 = kAB2;	//	The rate constant of the QAQB- --> QAQB2-	Lazar (1999); 1.25~3.33 * 10^3
    myVars->FI_RC.kBA2 = kBA2;	//	The rate constant of the QAQB2- --> QAQB- 	Lazar (1999), or same as kAB2 depend on the equilibium constant
    myVars->FI_RC.k3 = k3;	//	The rate constant of the exchange of PQ and QBH2	Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
    myVars->FI_RC.k_r3 = k_r3;	//	The rate constant of the exchange of QB and PQH2	Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
    myVars->FI_RC.k_pq_oxy = k_pq_oxy;	//	The rate constant of the PQH2 oxidation	Lazar (1999),50~500


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves for a dark adapted leaves;
    // Unit		micro mol per m2
    // Initialize the leaves for a dark adapted leaves;
    //		mircomol per m2

    // Initialize the leaves for a dark adapted leaves;
    //	 Micro mol m2

    const double A = 0;	// 	The concentration of excitons in the peripheral antenna
    const double U = 0;	//	The concentration fo excitons in the core antenna
    const double P680Pheo = 1;	// 	The concentration of the P680Pheo
    const double P680pPheon = 0;	//	The concentration for the P680+ Pheo-
    const double P680pPheo = 0;	// 	The concentration of P680+ Pheo
    const double P680Pheon = 0;	//	The concentration of P680Pheo-
    const double Yz = 1;	    //	The concentration of reduced tyrosine
    const double S1T = 0.8;	// 	The concentration of S1 in combination with reduced tyrosine
    const double S2T = 0;	//	The concentration of S2 in combination with reduced tyrosine
    const double S3T = 0;	// 	The concentration of S3 in combination with reduced tyrosine
    const double S0T = 0.2;	//	The concentration of S0 in combination with reduced tyrosine
    const double S1Tp = 0;	// 	The concentration of S1 in combination with oxidized tyrosine
    const double S2Tp = 0;	// 	The concentration of S2 in combination with oxidized tyrosine
    const double S3Tp = 0;	//	The concentration of S3 in combination with oxidized tyrosine
    const double S0Tp = 0;	// 	The concentration of S0 in combination with oxidized tyrosine
    const double QAQB = 1;	// 	The concentration of [QAQB]
    const double QAnQB = 0;	// 	The concentration of [QA-QB];
    const double QAQBn = 0;	//	The concentration of [QAQB-]
    const double QAnQBn = 0;	// 	The concentration of [QA-QB-];
    const double QAQB2n = 0;	//	The concentration of [QAQB2-]
    const double QAnQB2n = 0;	// 	The concentration of [QA-QB2-];
    const double PQn = 5;	//	The concentration of reduced PQ, i.e. PQH2;

    // Assign the value to a array
    // This is the program that initialize the major variables used in the fluorescence induction system.In this file, the n represent negative charges, _red represent that the components are associated with the closed reaction center; while _ox represent a system with open reaction center.
    FICon FI_Con;
    FI_Con.A = A;	// 	The concentration of excitons in the peripheral antenna
    FI_Con.U = U;	//	The concentration fo excitons in the core antenna
    FI_Con.P680ePheo = P680Pheo;	// 	The concentration of the P680Pheo
    FI_Con.P680pPheon = P680pPheon;	//	The concentration for the P680+ Pheo-
    FI_Con.P680pPheo = P680pPheo;	// 	The concentration of P680+ Pheo
    FI_Con.P680Pheon = P680Pheon;	//	The concentration of P680Pheo-
    FI_Con.Yz = Yz;           //	The concentration of reduced tyrosine
    FI_Con.S1T = S1T;           // 	The concentration of S1 in combination with reduced tyrosine
    FI_Con.S2T = S2T;           //	The concentration of S2 in combination with reduced tyrosine
    FI_Con.S3T = S3T;           // 	The concentration of S3 in combination with reduced tyrosine
    FI_Con.S0T = S0T;           //	The concentration of S0 in combination with reduced tyrosine
    FI_Con.S1Tp = S1Tp;	// 	The concentration of S1 in combination with oxidized tyrosine
    FI_Con.S2Tp = S2Tp;	// 	The concentration of S2 in combination with oxidized tyrosine
    FI_Con.S3Tp = S3Tp;	//	The concentration of S3 in combination with oxidized tyrosine
    FI_Con.S0Tp = S0Tp;	// 	The concentration of S0 in combination with oxidized tyrosine
    FI_Con.QAQB = QAQB;	// 	The concentration of [QAQB]
    FI_Con.QAnQB = QAnQB;	// 	The concentration of [QA-QB];
    FI_Con.QAQBn = QAQBn;	//	The concentration of [QAQB-]
    FI_Con.QAnQBn = QAnQBn;	// 	The concentration of [QA-QB-];
    FI_Con.QAQB2n = QAQB2n;	//	The concentration of [QAQB2-]
    FI_Con.QAnQB2n = QAnQB2n;	// 	The concentration of [QA-QB2-];
    FI_Con.PQn = PQn;	//	The concentration of reduced PQ, i.e. PQH2;

    const double QBt = 1 * myVars->FIRatio[21];// The total concentration of Qb site;
    const double PQT = 8 * myVars->FIRatio[22];// The total concentration of PQ;

    myVars->FI_Pool.QBt = QBt;
    myVars->FI_Pool.PQT = PQT;
    return FI_Con;
}
