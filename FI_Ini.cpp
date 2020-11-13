#include "globals.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// FI_Init.m     This is the routine that initialize the parameters, initial conditions for simulation of fluorescence induction curve.
// The following information is initialized sequentially 1) Rate constants; 2) Initial concentration ( or conditions); 3) THe maximum
// concentration of components of photosystems.

void FI_Ini(varptr *myVars) {
    //global FIRatio;
    //////////////////////////////////////////////////////////////////////////
    // Initilization of the rate constant //
    ////////////////////////////////////////////////////////////////////////////
    
    // The rate constant used in the model
    // The rate constant used in the model
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
    //global FI_RC;
    // FI_RC = zeros(5, 1);
    // The rate constant used in the model
    // The rate constant used in the model
    myVars->FI_RC[0] = kA_d;	//	The rate constant of heat dissipation from peripheral antenna	Lazar (1999), 0.25~1 *10^(9)
    myVars->FI_RC[1] = kA_f;	//	The rate constant of fluorescence emission from peripheral antenna	Lazar 1999, with a lifetime of 5 ns at closed reaction center
    myVars->FI_RC[2] = kA_U;	//	The rate constant of exciton transfer from periphral antenna to core antenna	Reference needed, a guess
    myVars->FI_RC[3] = kU_A;	//	The rate constant of exciton transfer from core antenna to peripheral antenna	Reference needed, a guess
    myVars->FI_RC[4] = kU_d;	//	The rate constant of  heat emission from core antenna
    myVars->FI_RC[5] = kU_f;	//	The rate constant of fluorescence emission from core antenna
    myVars->FI_RC[6] = k1;	//	The rate constant of primary charge separation for open reaction center
    myVars->FI_RC[7] = k_r1;	//	The rate constant of charge recombination for open reactoin center
    myVars->FI_RC[8] = kz;	//	The rate constant of the Tyrosine oxidation	Lazar (1999); 3.8~50 * 10^6
    myVars->FI_RC[9] = k12;	//	The rate constant of the S1 to S2 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC[10] = k23;	//	The rate constant of the S2 to S3 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC[11] = k30;	//	The rate constant of the S3 to S0 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC[12] = k01;	//	The rate constant of the S0 to S1 transition	Lazar (1999); 0.667~33.3 * 10^3
    myVars->FI_RC[13] = k2;	//	The rate constant of the QA reduction by Pheo-	Lazar (1999); 2~2.3 * 10^9
    myVars->FI_RC[14] = kAB1;	//	The rate constant of QAQB-->QAQB-	Lazar (1999); 2.5~5 * 10^3
    myVars->FI_RC[15] = kBA1;	//	The rate constant of the QAQB- -->QAQB	Lazar (1999)
    myVars->FI_RC[16] = kAB2;	//	The rate constant of the QAQB- --> QAQB2-	Lazar (1999); 1.25~3.33 * 10^3
    myVars->FI_RC[17] = kBA2;	//	The rate constant of the QAQB2- --> QAQB- 	Lazar (1999), or same as kAB2 depend on the equilibium constant
    myVars->FI_RC[18] = k3;	//	The rate constant of the exchange of PQ and QBH2	Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
    myVars->FI_RC[19] = k_r3;	//	The rate constant of the exchange of QB and PQH2	Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
    myVars->FI_RC[20] = k_pq_oxy;	//	The rate constant of the PQH2 oxidation	Lazar (1999),50~500
    
    
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
    // FI_ini.m
    // This is the program that initialize the major variables used in the fluorescence induction system.In this file, the n represent negative charges, _red represent that the components are associated with the closed reaction center; while _ox represent a system with open reaction center.
    //global FI_Con;
    myVars->FI_Con[0] = A;	// 	The concentration of excitons in the peripheral antenna
    myVars->FI_Con[1] = U;	//	The concentration fo excitons in the core antenna
    myVars->FI_Con[2] = P680Pheo;	// 	The concentration of the P680Pheo
    myVars->FI_Con[3] = P680pPheon;	//	The concentration for the P680+ Pheo-
    myVars->FI_Con[4] = P680pPheo;	// 	The concentration of P680+ Pheo
    myVars->FI_Con[5] = P680Pheon;	//	The concentration of P680Pheo-
    myVars->FI_Con[6] = Yz;           //	The concentration of reduced tyrosine
    myVars->FI_Con[7] = S1T;           // 	The concentration of S1 in combination with reduced tyrosine
    myVars->FI_Con[8] = S2T;           //	The concentration of S2 in combination with reduced tyrosine
    myVars->FI_Con[9] = S3T;           // 	The concentration of S3 in combination with reduced tyrosine
    myVars->FI_Con[10] = S0T;           //	The concentration of S0 in combination with reduced tyrosine
    myVars->FI_Con[11] = S1Tp;	// 	The concentration of S1 in combination with oxidized tyrosine
    myVars->FI_Con[12] = S2Tp;	// 	The concentration of S2 in combination with oxidized tyrosine
    myVars->FI_Con[13] = S3Tp;	//	The concentration of S3 in combination with oxidized tyrosine
    myVars->FI_Con[14] = S0Tp;	// 	The concentration of S0 in combination with oxidized tyrosine
    myVars->FI_Con[15] = QAQB;	// 	The concentration of [QAQB]
    myVars->FI_Con[16] = QAnQB;	// 	The concentration of [QA-QB];
    myVars->FI_Con[17] = QAQBn;	//	The concentration of [QAQB-]
    myVars->FI_Con[18] = QAnQBn;	// 	The concentration of [QA-QB-];
    myVars->FI_Con[19] = QAQB2n;	//	The concentration of [QAQB2-]
    myVars->FI_Con[20] = QAnQB2n;	// 	The concentration of [QA-QB2-];
    myVars->FI_Con[21] = PQn;	//	The concentration of reduced PQ, i.e. PQH2;
    
    //global FI_Pool;
    const double QBt = 1 * myVars->FIRatio[21];// The total concentration of Qb site;
    const double PQT = 8 * myVars->FIRatio[22];// The total concentration of PQ;
    
    myVars->FI_Pool[0] = QBt;
    myVars->FI_Pool[1] = PQT;
    
    
    //global FIBF_AUX;
    // FIBF_AUX = zeros(5, 1);
    
    //global FI_RC_Reg_o;
    //myVars->FI_RC_Reg_o(1) = myVars->FI_RC[10];  // --unused
    //myVars->FI_RC_Reg_o(2) = myVars->FI_RC[11];  // --unused
    //myVars->FI_RC_Reg_o(3) = myVars->FI_RC[12];  // --unused
    //myVars->FI_RC_Reg_o(4) = myVars->FI_RC[6];  // --unused
    
}
