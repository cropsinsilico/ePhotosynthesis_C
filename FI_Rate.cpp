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
 *    MERCHANTABILITY or FITNESS FOR FI_Con.A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include "Variables.hpp"

void FI_Rate(double t, FICon &FI_Con, Variables *myVars) {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Step 1 Get the rate constant and the initial concentrations //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // The rate constant used in the model

    double PQ = myVars->FI_Pool.PQT - FI_Con.PQn - myVars->FIBF2FI_PQa;

    if (myVars->BF_FI_com)
        PQ = myVars->FIBF2FI_PQ;

    const double P680PheoT = 1;

    const double P680Pheo = P680PheoT - FI_Con.P680pPheo - FI_Con.P680Pheon - FI_Con.P680pPheon - FI_Con.P680ePheo;//QF add  '- P680ePheo'

    //const double It = myVars->FI_Param[0];	//	It	The total incident light intensity

    // rate: FI_Con.U -> U*
    myVars->FI_Vel.Ic = myVars->FI_Param[0] * myVars->ChlT / (myVars->ChlT2 + myVars->ChlPSI);//	Ic	The incident light on the core antenna; myVars->ChlT is defined in upper lines as the total amount of Chl in one U.
    // rate: FI_Con.A -> A*
    myVars->FI_Vel.Ia = myVars->FI_Param[0] * (myVars->ChlT2 - myVars->ChlT) / (myVars->ChlT2 + myVars->ChlPSI);//	Ia	The incident light on the peripheral antenna

    //////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the rate of different reactions //
    ////////////////////////////////////////////////////////////////////////////////////////////

    const double q = (FI_Con.QAQB + FI_Con.QAQBn + FI_Con.QAQB2n) / (FI_Con.QAQB + FI_Con.QAQBn + FI_Con.QAQB2n + FI_Con.QAnQB + FI_Con.QAnQBn + FI_Con.QAnQB2n);
    myVars->FI_Vel.vA_d  = FI_Con.A * myVars->FI_RC.kA_d;	//	vA_d	The rate of heat dissipation from peripheral antenna
    myVars->FI_Vel.vA_f  = FI_Con.A * myVars->FI_RC.kA_f;	//	vA_f	The rate of fluorescence emission from peripheral antenna
    myVars->FI_Vel.vA_U  = FI_Con.A * myVars->FI_RC.kA_U;	//	vA_U	The rate of exciton transfer from peripheral antenna to core antenna in open reaction center
    myVars->FI_Vel.vU_A  = FI_Con.U * myVars->FI_RC.kU_A;	//	vU_A	The rate of exciton transfer from core antenna to perpheral antenna in open center
    myVars->FI_Vel.vU_f  = FI_Con.U * myVars->FI_RC.kU_f;	//	vU_f	The rate of fluorescence emission from core antenna
    myVars->FI_Vel.vU_d  = FI_Con.U * myVars->FI_RC.kU_d * (1 - q);	//	vU_d	The rate of heat dissipation from core antenna

    const double P = 1;

    // energy flow:
    myVars->FI_Vel.vU_P680 = myVars->FI_Vel.Ic + myVars->FI_Vel.vA_U - myVars->FI_Vel.vU_A - myVars->FI_Vel.vU_f - myVars->FI_Vel.vU_d;//QF add  , total energy coming to P680 and == the rate of P680 -> P680*, except f and d, energy is transport to P680 reaction center
    myVars->FI_Vel.vP680_d = FI_Con.P680ePheo * myVars->FI_RC.kU_d * (1 - q);
    myVars->FI_Vel.v1  = FI_Con.P680ePheo * myVars->FI_RC.k1 * q + FI_Con.P680ePheo * P * (1 - q) * myVars->FI_RC.k1 / 6.2 + FI_Con.P680ePheo * (1 - P) * (1 - q) * myVars->FI_RC.k1;	//	v1	The rate of primary charge separation

    myVars->FI_Vel.v_r1  = FI_Con.P680pPheon * myVars->FI_RC.k_r1 * q + FI_Con.P680pPheon * (1 - q) * myVars->FI_RC.k_r1 * 3;	//	v_r1	The rate of charge recombination
    myVars->FI_Vel.vP680_f = myVars->FI_Vel.vU_P680 - (myVars->FI_Vel.v1 - myVars->FI_Vel.v_r1) - myVars->FI_Vel.vP680_d;

    double Coeff = FI_Con.P680pPheon / P680PheoT;
    myVars->FI_Vel.v1z_1  = FI_Con.S1T * myVars->FI_RC.kz * Coeff;	//	v1z_1	The rate of oxidation of FI_Con.S1T by P680pPheon
    myVars->FI_Vel.v2z_1  = FI_Con.S2T * myVars->FI_RC.kz * Coeff;	//	v2z_1	The rate of oxidation of FI_Con.S2T  by P680pPheon
    myVars->FI_Vel.v3z_1  = FI_Con.S3T * myVars->FI_RC.kz * Coeff;	//	v3z_1	The rate of oxidation of FI_Con.S3T  by P680pPheon
    myVars->FI_Vel.v0z_1  = FI_Con.S0T * myVars->FI_RC.kz * Coeff;	//	v0z_1	The rate of oxidation of FI_Con.S0T  by P680pPheon

    myVars->FI_Vel.vz_1 = myVars->FI_Vel.v1z_1 + myVars->FI_Vel.v2z_1 + myVars->FI_Vel.v3z_1 + myVars->FI_Vel.v0z_1;//	vz_1	The rate of FI_Con.P680pPheon reduction

    Coeff = FI_Con.P680pPheo / P680PheoT;

    myVars->FI_Vel.v1z_2 = FI_Con.S1T * myVars->FI_RC.kz * Coeff;//	v1z_2	The rate of oxidation of FI_Con.S1T by P680pPheo
    myVars->FI_Vel.v2z_2 = FI_Con.S2T * myVars->FI_RC.kz * Coeff;//	v2z_2	The rate of oxidation of FI_Con.S2T  by P680pPheo
    myVars->FI_Vel.v3z_2 = FI_Con.S3T * myVars->FI_RC.kz * Coeff;//	v3z_2	The rate of oxidation of FI_Con.S3T  by P680pPheo
    myVars->FI_Vel.v0z_2 = FI_Con.S0T * myVars->FI_RC.kz * Coeff;//	v0z_2	The rate of oxidation of FI_Con.S0T  by P680pPheo
    myVars->FI_Vel.vz_2 = myVars->FI_Vel.v1z_2 + myVars->FI_Vel.v2z_2 + myVars->FI_Vel.v3z_2 + myVars->FI_Vel.v0z_2;//	vz_2	The rate of FI_Con.P680pPheo reduction

    myVars->FI_Vel.v1z = myVars->FI_Vel.v1z_1 + myVars->FI_Vel.v1z_2;
    myVars->FI_Vel.v2z = myVars->FI_Vel.v2z_1 + myVars->FI_Vel.v2z_2;
    myVars->FI_Vel.v3z = myVars->FI_Vel.v3z_1 + myVars->FI_Vel.v3z_2;
    myVars->FI_Vel.v0z = myVars->FI_Vel.v0z_1 + myVars->FI_Vel.v0z_2;

    myVars->FI_Vel.vAB1 = FI_Con.QAnQB * myVars->FI_RC.kAB1;//	vAB1	The rate of electron transfer from QA- to QB
    myVars->FI_Vel.vBA1 = FI_Con.QAQBn * myVars->FI_RC.kBA1;//	vBA1	The rate of electron transfer from QB- to QA
    myVars->FI_Vel.vAB2 = FI_Con.QAnQBn * myVars->FI_RC.kAB2;//	vAB2	The rate of electron transfer from QA- to QB-
    myVars->FI_Vel.vBA2 = FI_Con.QAQB2n * myVars->FI_RC.kBA2;//	vBA2	The rate of electron transfer from QB2- TO QA
    myVars->FI_Vel.v3 = FI_Con.QAQB2n * PQ * myVars->FI_RC.k3 / myVars->FI_Pool.PQT;//	v3	The rate of exchange of QAQBH2 with PQ
    myVars->FI_Vel.v_r3 = FI_Con.QAQB * FI_Con.PQn * myVars->FI_RC.k_r3 / myVars->FI_Pool.PQT;//	v_r3	The rate of exchange of FI_Con.QAQB with PQH2

    myVars->FI_Vel.v3_n = FI_Con.QAnQB2n * PQ * myVars->FI_RC.k3 / myVars->FI_Pool.PQT;//	v3_n	The rate of exchange of QAnQBH2 with PQ
    myVars->FI_Vel.v_r3_n = FI_Con.QAnQB * FI_Con.PQn * myVars->FI_RC.k_r3 / myVars->FI_Pool.PQT;//	v_r3_n	The rate of exchange of FI_Con.QAnQB with PQH2

    myVars->FI_Vel.v_pq_ox = FI_Con.PQn * myVars->FI_RC.k_pq_oxy;//	v_pq_ox	The rate of PQH2 oxidation

    myVars->FI_Vel.v2_1 = FI_Con.P680pPheon * myVars->FI_RC.k2 * q;//	v2_1	The rate of FI_Con.P680pPheon oxidation
    myVars->FI_Vel.v2_2 = FI_Con.P680Pheon * myVars->FI_RC.k2 * q;//	v2_1	The rate of FI_Con.P680pPheon oxidation
    const double a = FI_Con.QAQB / (FI_Con.QAQB + FI_Con.QAQBn + FI_Con.QAQB2n);//	a
    const double b = FI_Con.QAQBn / (FI_Con.QAQB + FI_Con.QAQBn + FI_Con.QAQB2n);//	b
    const double c = FI_Con.QAQB2n / (FI_Con.QAQB + FI_Con.QAQBn + FI_Con.QAQB2n);//	c

    myVars->FI_Vel.v2_00_1 = myVars->FI_Vel.v2_1 * a;//	v2_00_1	The rate of reduction of FI_Con.QAQB by P680pPheon
    myVars->FI_Vel.v2_01_1 = myVars->FI_Vel.v2_1 * b;//	v2_01_1	The rate of reduction of FI_Con.QAQBn by P680pPheon
    myVars->FI_Vel.v2_02_1 = myVars->FI_Vel.v2_1 * c;//	v2_02_1	The rate of reduction of FI_Con.QAQB2n by P680pPheon

    myVars->FI_Vel.v2_00_2 = myVars->FI_Vel.v2_2 * a;//	v2_00_2	The rate of reduction of FI_Con.QAQB by P680Pheon
    myVars->FI_Vel.v2_01_2 = myVars->FI_Vel.v2_2 * b;//	v2_01_2	The rate of reduction of FI_Con.QAQBn by P680Pheon
    myVars->FI_Vel.v2_02_2 = myVars->FI_Vel.v2_2 * c;//	v2_02_2	The rate of reduction of FI_Con.QAQB2n by P680Pheon

    const double KE = 1000000;
    const double Coeff1 = FI_Con.P680pPheo / P680PheoT;//	Coeff1
    myVars->FI_Vel.vr2_00_1 = FI_Con.QAnQB * myVars->FI_RC.k2 / KE * Coeff1;//	vr2_00_1	The reverse reaction of The rate of reduction of FI_Con.QAQB by P680pPheon
    myVars->FI_Vel.vr2_01_1 = FI_Con.QAnQBn * myVars->FI_RC.k2 / KE * Coeff1;//	vr2_01_1	The reverse reaction of The rate of reduction of FI_Con.QAQBn by P680pPheon
    myVars->FI_Vel.vr2_02_1 = FI_Con.QAnQB2n * myVars->FI_RC.k2 / KE * Coeff1;//	vr2_02_1	The reverse reaction of The rate of reduction of FI_Con.QAQB2n by P680pPheon
    myVars->FI_Vel.vr2_1 = myVars->FI_Vel.vr2_00_1 + myVars->FI_Vel.vr2_01_1 + myVars->FI_Vel.vr2_02_1;//	vr2_1

    const double Coeff2 = P680Pheo / P680PheoT;//	Coeff2
    myVars->FI_Vel.vr2_00_2 = FI_Con.QAnQB * myVars->FI_RC.k2 / KE * Coeff2;//	vr2_00_2	The reverse reaction of The rate of reduction of FI_Con.QAQB by P680Pheon
    myVars->FI_Vel.vr2_01_2 = FI_Con.QAnQBn * myVars->FI_RC.k2 / KE * Coeff2;//	vr2_01_2	The reverse reaction of The rate of reduction of FI_Con.QAQBn by P680Pheon
    myVars->FI_Vel.vr2_02_2 = FI_Con.QAnQB2n * myVars->FI_RC.k2 / KE * Coeff2;//	vr2_02_2	The reverse reaction of The rate of reduction of FI_Con.QAQB2n by P680Pheon
    myVars->FI_Vel.vr2_2 = myVars->FI_Vel.vr2_00_2 + myVars->FI_Vel.vr2_01_2 + myVars->FI_Vel.vr2_02_2;//	vr2_2

    myVars->FI_Vel.vP680qU = pow(10, 9) * FI_Con.U * (FI_Con.P680pPheo + FI_Con.P680pPheon) + FI_Con.U * 0.15 * (myVars->FI_RC.kU_f + myVars->FI_RC.kU_d) * PQ / myVars->FI_Pool.PQT;
    myVars->FI_Vel.vP680qA = pow(10, 9) * FI_Con.A * (FI_Con.P680pPheo + FI_Con.P680pPheon) + FI_Con.A * 0.15 * (myVars->FI_RC.kA_f + myVars->FI_RC.kA_d) * PQ / myVars->FI_Pool.PQT;

    ////////////////////////////////////////
    //////  FOR TESITNG  ////
    //const double f = vA_f + vU_f;
    ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //   Part V Output of Velocity for plot          //
    //////////////////////////////////////////////////////////////////////////////////////////////////

    if (t > myVars->FI_OLD_TIME) {
            myVars->FI_TIME_N = myVars->FI_TIME_N + 1;
            myVars->FI_OLD_TIME = t;
        }

    ////////////////////////////////////////////////////////////////////////////
    // Assign table
    ////////////////////////////////////////////////////////////////////////////////

    myVars->FI_Vel.vS1_S2  = FI_Con.S1Tp * myVars->FI_RC.k12;	//	vS1_S2	The rate of transition from S1 to S2
    myVars->FI_Vel.vS2_S3  = FI_Con.S2Tp * myVars->FI_RC.k23;	//	vS2_S3	The rate of transition from S2 to S3
    myVars->FI_Vel.vS3_S0  = FI_Con.S3Tp * myVars->FI_RC.k30;	//	vS3_S0	The rate of transition from S3 to S0
    myVars->FI_Vel.vS0_S1  = FI_Con.S0Tp * myVars->FI_RC.k01;	//	vS0_S1	The rate of transition from S0 to S1

    if (myVars->record) {
        myVars->FI_VEL.insert(myVars->FI_TIME_N - 1, t, myVars->FI_Vel);
    }
}
