#include "globals.hpp"
#include "FI.hpp"
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


// FI_mb.m   This is the routine for calculation of the mass balance equations for the fluorescence induction model
// This routine is composed of two components;
// 1) The initialization of the rates that was transfered from the FI_Rate routine
// 2) The computation of the mass balance equations

arr FI_Mb(double t, FICon &FI_Con, varptr *myVars) {
    
    //////////////////////////////////////////////////////////////////
    //   Calculate the rates first   //
    //////////////////////////////////////////////////////////////////
    //global GLight;
    Condition(t, myVars);
    const double light = myVars->GLight;
    
    myVars->FI_Param[0] = light;
    
    FI_Rate(t, FI_Con, myVars);
    
    ////////////////////////////////////////////////////////////////////////////////
    //   Get the rate of different reactions//
    ////////////////////////////////////////////////////////////////////////////////
    
    const double vA_d = myVars->FI_Vel.vA_d ;	//	vA_d	The rate of heat dissipation from peripheral antenna
    const double vA_f = myVars->FI_Vel.vA_f ;	//	vA_f	The rate of fluorescence emission from peripheral antenna
    const double vA_U = myVars->FI_Vel.vA_U ;	//	vA_U	The rate of exciton transfer from peripheral antenna to core antenna in open reaction center
    const double vU_A = myVars->FI_Vel.vU_A ;	//	vU_A	The rate of exciton transfer from core antenna to perpheral antenna in open center
    const double vU_f = myVars->FI_Vel.vU_f ;	//	vU_f	The rate of fluorescence emission from core antenna
    const double vU_d = myVars->FI_Vel.vU_d ;	//	vU_d	The rate of heat dissipation from core antenna
    const double v1 = myVars->FI_Vel.v1 ;	//	v1	The rate of primary charge separation
    const double v_r1 = myVars->FI_Vel.v_r1 ;	//	v_r1	The rate of charge recombination
    const double vS1_S2 = myVars->FI_Vel.vS1_S2 ;	//	vS1_S2	The rate of transition from S1 to S2
    const double vS2_S3 = myVars->FI_Vel.vS2_S3 ;	//	vS2_S3	The rate of transition from S2 to S3
    const double vS3_S0 = myVars->FI_Vel.vS3_S0 ;	//	vS3_S0	The rate of transition from S3 to S0
    const double vS0_S1 = myVars->FI_Vel.vS0_S1 ;	//	vS0_S1	The rate of transition from S0 to S1
    const double vz_1 = myVars->FI_Vel.vz_1 ;	//	vz_1	The rate of P680p reduction
    // v1z_1 = FI_Vel.v1z_1 ;	//	v1z_1	The rate of oxidation of S1T by P680pPheon// --unused
    // v2z_1 = FI_Vel.v2z_1 ;	//	v2z_1	The rate of oxidation of S2T  by P680pPheon// --unused
    // v3z_1 = FI_Vel.v3z_1 ;	//	v3z_1	The rate of oxidation of S3T  by P680pPheon// --unused
    // v0z_1 = FI_Vel.v0z_1 ;	//	v0z_1	The rate of oxidation of S0T  by P680pPheon// --unused
    const double vz_2 = myVars->FI_Vel.vz_2 ;	//	vz_2	The rate of P680pPheon reduction
    // v1z_2 = FI_Vel.v1z_2 ;	//	v1z_2	The rate of oxidation of S1T by P680pPheo// --unused
    // v2z_2 = FI_Vel.v2z_2 ;	//	v2z_2	The rate of oxidation of S2T  by P680pPheo// --unused
    // v3z_2 = FI_Vel.v3z_2 ;	//	v3z_2	The rate of oxidation of S3T  by P680pPheo// --unused
    // v0z_2 = FI_Vel.v0z_2 ;	//	v0z_2	The rate of oxidation of S0T  by P680pPheo// --unused
    const double v1z = myVars->FI_Vel.v1z ;	//	v1z
    const double v2z = myVars->FI_Vel.v2z ;	//	v2z
    const double v3z = myVars->FI_Vel.v3z ;	//	v3z
    const double v0z = myVars->FI_Vel.v0z ;	//	v0z
    const double vAB1 = myVars->FI_Vel.vAB1 ;	//	vAB1	The rate of electron transfer from QA- to QB
    const double vBA1 = myVars->FI_Vel.vBA1 ;	//	vBA1	The rate of electron transfer from QB- to QA
    const double vAB2 = myVars->FI_Vel.vAB2 ;	//	vAB2	The rate of electron transfer from QA- to QB-
    const double vBA2 = myVars->FI_Vel.vBA2 ;	//	vBA2	The rate of electron transfer from QB2- TO QA
    const double v3 = myVars->FI_Vel.v3 ;	//	v3	The rate of exchange of QAQBH2 with PQ
    const double v_r3 = myVars->FI_Vel.v_r3 ;	//	v_r3	The rate of exchange of QAQB with PQH2
    const double v3_n = myVars->FI_Vel.v3_n ;	//	v3_n	The rate of exchange of QAnQBH2 with PQ
    const double v_r3_n = myVars->FI_Vel.v_r3_n ;	//	v_r3_n	The rate of exchange of QAnQB with PQH2
    const double v_pq_ox = myVars->FI_Vel.v_pq_ox ;	//	v_pq_ox	The rate of PQH2 oxidation
    const double Ic = myVars->FI_Vel.Ic ;	//	Ic	The incident light on the core antenna
    const double Ia = myVars->FI_Vel.Ia ;	//	Ia	The incident light on the peripheral antenna
    const double v2_1 = myVars->FI_Vel.v2_1 ;	//	v2_1	The rate of P680pPheon oxidation
    const double v2_2 = myVars->FI_Vel.v2_2 ;	//	v2_1	The rate of P680pPheon oxidation
    const double v2_00_1 = myVars->FI_Vel.v2_00_1 ;	//	v2_00_1	The rate of reduction of QAQB by P680pPheon
    const double v2_01_1 = myVars->FI_Vel.v2_01_1 ;	//	v2_01_1	The rate of reduction of QAQBn by P680pPheon
    const double v2_02_1 = myVars->FI_Vel.v2_02_1 ;	//	v2_02_1	The rate of reduction of QAQB2n by P680pPheon
    const double v2_00_2 = myVars->FI_Vel.v2_00_2 ;	//	v2_00_2	The rate of reduction of QAQB by P680Pheon
    const double v2_01_2 = myVars->FI_Vel.v2_01_2 ;	//	v2_01_2	The rate of reduction of QAQBn by P680Pheon
    const double v2_02_2 = myVars->FI_Vel.v2_02_2 ;	//	v2_02_2	The rate of reduction of QAQB2n by P680Pheon
    const double vr2_00_1 = myVars->FI_Vel.vr2_00_1 ;	//	vr2_00_1	The reverse reaction of The rate of reduction of QAQB by P680pPheon
    const double vr2_01_1 = myVars->FI_Vel.vr2_01_1 ;	//	vr2_01_1	The reverse reaction of The rate of reduction of QAQBn by P680pPheon
    const double vr2_02_1 = myVars->FI_Vel.vr2_02_1 ;	//	vr2_02_1	The reverse reaction of The rate of reduction of QAQB2n by P680pPheon
    const double vr2_1 = myVars->FI_Vel.vr2_1 ;	//	vr2_1
    const double vr2_00_2 = myVars->FI_Vel.vr2_00_2 ;	//	vr2_00_2	The reverse reaction of The rate of reduction of QAQB by P680Pheon
    const double vr2_01_2 = myVars->FI_Vel.vr2_01_2 ;	//	vr2_01_2	The reverse reaction of The rate of reduction of QAQBn by P680Pheon
    const double vr2_02_2 = myVars->FI_Vel.vr2_02_2 ;	//	vr2_02_2	The reverse reaction of The rate of reduction of QAQB2n by P680Pheon
    const double vr2_2 = myVars->FI_Vel.vr2_2 ;	//	vr2_2
    const double vP680qU = myVars->FI_Vel.vP680qU ;	//	vP680qU
    // vP680qA = FI_Vel.vP680qA ;	//	vP680qA// --unused
    const double vU_P680 = myVars->FI_Vel.vU_P680 ;
    const double vP680_d = myVars->FI_Vel.vP680_d ;
    // vP680_f = FI_Vel.vP680_f ;// --unused
    
    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation //
    ////////////////////////////////////////////////////////////////
    
    // This page defines the mass balance equation for the system under study
    // One problem need to be taken care of is the variables needed to transfer from FI_CalV to FI_mb
    // The Major Variables
    arr FI_mb = zeros(22);
    //FI_mb	(	1	)	=	Ia - vA_f - vA_d - vA_U + vU_A - vP680qA	;	//	A
    FI_mb[0] = Ia - vA_f - vA_d - vA_U + vU_A;//	A	6
    FI_mb[1] = Ic + vA_U - vU_A - vU_f - vU_d - v1 + v_r1 - vP680qU;//	U
    FI_mb[2] = vU_P680 + v_r1 - v1 - vP680_d;//	P680ePheo	QF add
    FI_mb[3] = v1 - v_r1 - vz_1 - v2_1 + vr2_1;//	P680pPheon
    FI_mb[4] = v2_1 - vr2_1 - vz_2;//	P680pPheo
    FI_mb[5] = vz_1 - v2_2 + vr2_2;//	P680Pheon
    FI_mb[6] = vS1_S2 + vS2_S3 + vS3_S0 + vS0_S1 - vz_1 - vz_2;//	Yz
    FI_mb[7] = vS0_S1 - v1z;//	S1T
    FI_mb[8] = vS1_S2 - v2z;//	S2T
    FI_mb[9] = vS2_S3 - v3z;//	S3T
    FI_mb[10] = vS3_S0 - v0z;//	S0T
    FI_mb[11] = v1z - vS1_S2;//	S1Tp
    FI_mb[12] = v2z - vS2_S3;//	S2Tp
    FI_mb[13] = v3z - vS3_S0;//	S3Tp
    FI_mb[14] = v0z - vS0_S1;//	S0Tp
    FI_mb[15] = v3 - v_r3 - v2_00_1 - v2_00_2 + vr2_00_1 + vr2_00_2;//	QAQB
    FI_mb[16] = v2_00_1 + v2_00_2 - vr2_00_1 - vr2_00_2 - vAB1 + vBA1 + v3_n - v_r3_n;//	QAnQB
    FI_mb[17] = vAB1 - vBA1 - v2_01_1 - v2_01_2 + vr2_01_1 + vr2_01_2;//	QAQBn
    FI_mb[18] = vBA2 - vAB2 + v2_01_1 + v2_01_2 - vr2_01_1 - vr2_01_2;//	QAnQBn
    FI_mb[19] = vAB2 - vBA2 - v3 + v_r3 - v2_02_1 - v2_02_2 + vr2_02_1 + vr2_02_2;//	QAQB2n
    FI_mb[20] = 0 - v3_n + v_r3_n + v2_02_1 + v2_02_2 - vr2_02_1 - vr2_02_2;//	QAnQB2n
    FI_mb[21] = v3 + v3_n - v_r3 - v_r3_n - v_pq_ox;//	PQn
    return FI_mb;
}
