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
#include <math.h>
#include "Variables.hpp"
#include "modules/FI.hpp"

void FI::_Rate(const double t, const FIContainer* FI_Con, Variables *theVars) {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Step 1 Get the rate constant and the initial concentrations //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // The rate constant used in the model

    double PQ = theVars->FI_Pool.PQT - FI_Con->PQn - theVars->FIBF2FI_PQa;

    if (theVars->BF_FI_com)
        PQ = theVars->FIBF2FI_PQ;

    const double P680PheoT = 1;

    const double P680Pheo = P680PheoT - FI_Con->P680pPheo - FI_Con->P680Pheon - FI_Con->P680pPheon - FI_Con->P680ePheo; //QF add  '- P680ePheo'

    //const double It = theVars->FI_Param[0]; // It The total incident light intensity

    // rate: FI_Con.U -> U*
    theVars->FI_Vel.Ic = theVars->FI_Param[0] * theVars->ChlT / (theVars->ChlT2 + theVars->ChlPSI); // Ic The incident light on the core antenna; theVars->ChlT is defined in upper lines as the total amount of Chl in one U.
    // rate: FI_Con.A -> A*
    theVars->FI_Vel.Ia = theVars->FI_Param[0] * (theVars->ChlT2 - theVars->ChlT) / (theVars->ChlT2 + theVars->ChlPSI); // Ia The incident light on the peripheral antenna

    //////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the rate of different reactions //
    ////////////////////////////////////////////////////////////////////////////////////////////

    const double q = (FI_Con->QAQB + FI_Con->QAQBn + FI_Con->QAQB2n) / (FI_Con->QAQB + FI_Con->QAQBn + FI_Con->QAQB2n + FI_Con->QAnQB + FI_Con->QAnQBn + FI_Con->QAnQB2n);
    theVars->FI_Vel.vA_d  = FI_Con->A * theVars->FI_RC.kA_d; // vA_d The rate of heat dissipation from peripheral antenna
    theVars->FI_Vel.vA_f  = FI_Con->A * theVars->FI_RC.kA_f; // vA_f The rate of fluorescence emission from peripheral antenna
    theVars->FI_Vel.vA_U  = FI_Con->A * theVars->FI_RC.kA_U; // vA_U The rate of exciton transfer from peripheral antenna to core antenna in open reaction center
    theVars->FI_Vel.vU_A  = FI_Con->U * theVars->FI_RC.kU_A; // vU_A The rate of exciton transfer from core antenna to perpheral antenna in open center
    theVars->FI_Vel.vU_f  = FI_Con->U * theVars->FI_RC.kU_f; // vU_f The rate of fluorescence emission from core antenna
    theVars->FI_Vel.vU_d  = FI_Con->U * theVars->FI_RC.kU_d * (1 - q); // vU_d The rate of heat dissipation from core antenna

    const double P = 1;

    // energy flow:
    theVars->FI_Vel.vU_P680 = theVars->FI_Vel.Ic + theVars->FI_Vel.vA_U - theVars->FI_Vel.vU_A - theVars->FI_Vel.vU_f - theVars->FI_Vel.vU_d;//QF add  , total energy coming to P680 and == the rate of P680 -> P680*, except f and d, energy is transport to P680 reaction center
    theVars->FI_Vel.vP680_d = FI_Con->P680ePheo * theVars->FI_RC.kU_d * (1 - q);
    theVars->FI_Vel.v1  = FI_Con->P680ePheo * theVars->FI_RC.k1 * q + FI_Con->P680ePheo * P * (1 - q) * theVars->FI_RC.k1 / 6.2 + FI_Con->P680ePheo * (1 - P) * (1 - q) * theVars->FI_RC.k1; // v1 The rate of primary charge separation

    theVars->FI_Vel.v_r1  = FI_Con->P680pPheon * theVars->FI_RC.k_r1 * q + FI_Con->P680pPheon * (1 - q) * theVars->FI_RC.k_r1 * 3; // v_r1 The rate of charge recombination
    theVars->FI_Vel.vP680_f = theVars->FI_Vel.vU_P680 - (theVars->FI_Vel.v1 - theVars->FI_Vel.v_r1) - theVars->FI_Vel.vP680_d;

    double Coeff = FI_Con->P680pPheon / P680PheoT;
    theVars->FI_Vel.v1z_1  = FI_Con->S1T * theVars->FI_RC.kz * Coeff; // v1z_1 The rate of oxidation of FI_Con.S1T by P680pPheon
    theVars->FI_Vel.v2z_1  = FI_Con->S2T * theVars->FI_RC.kz * Coeff; // v2z_1 The rate of oxidation of FI_Con.S2T  by P680pPheon
    theVars->FI_Vel.v3z_1  = FI_Con->S3T * theVars->FI_RC.kz * Coeff; // v3z_1 The rate of oxidation of FI_Con.S3T  by P680pPheon
    theVars->FI_Vel.v0z_1  = FI_Con->S0T * theVars->FI_RC.kz * Coeff; // v0z_1 The rate of oxidation of FI_Con.S0T  by P680pPheon

    theVars->FI_Vel.vz_1 = theVars->FI_Vel.v1z_1 + theVars->FI_Vel.v2z_1 + theVars->FI_Vel.v3z_1 + theVars->FI_Vel.v0z_1; // vz_1 The rate of FI_Con.P680pPheon reduction

    Coeff = FI_Con->P680pPheo / P680PheoT;

    theVars->FI_Vel.v1z_2 = FI_Con->S1T * theVars->FI_RC.kz * Coeff; // v1z_2 The rate of oxidation of FI_Con.S1T by P680pPheo
    theVars->FI_Vel.v2z_2 = FI_Con->S2T * theVars->FI_RC.kz * Coeff; // v2z_2 The rate of oxidation of FI_Con.S2T  by P680pPheo
    theVars->FI_Vel.v3z_2 = FI_Con->S3T * theVars->FI_RC.kz * Coeff; // v3z_2 The rate of oxidation of FI_Con.S3T  by P680pPheo
    theVars->FI_Vel.v0z_2 = FI_Con->S0T * theVars->FI_RC.kz * Coeff; // v0z_2 The rate of oxidation of FI_Con.S0T  by P680pPheo
    theVars->FI_Vel.vz_2 = theVars->FI_Vel.v1z_2 + theVars->FI_Vel.v2z_2 + theVars->FI_Vel.v3z_2 + theVars->FI_Vel.v0z_2; // vz_2 The rate of FI_Con.P680pPheo reduction

    theVars->FI_Vel.v1z = theVars->FI_Vel.v1z_1 + theVars->FI_Vel.v1z_2;
    theVars->FI_Vel.v2z = theVars->FI_Vel.v2z_1 + theVars->FI_Vel.v2z_2;
    theVars->FI_Vel.v3z = theVars->FI_Vel.v3z_1 + theVars->FI_Vel.v3z_2;
    theVars->FI_Vel.v0z = theVars->FI_Vel.v0z_1 + theVars->FI_Vel.v0z_2;

    theVars->FI_Vel.vAB1 = FI_Con->QAnQB * theVars->FI_RC.kAB1;   // vAB1 The rate of electron transfer from QA- to QB
    theVars->FI_Vel.vBA1 = FI_Con->QAQBn * theVars->FI_RC.kBA1;   // vBA1 The rate of electron transfer from QB- to QA
    theVars->FI_Vel.vAB2 = FI_Con->QAnQBn * theVars->FI_RC.kAB2;  // vAB2 The rate of electron transfer from QA- to QB-
    theVars->FI_Vel.vBA2 = FI_Con->QAQB2n * theVars->FI_RC.kBA2;  // vBA2 The rate of electron transfer from QB2- TO QA
    theVars->FI_Vel.v3 = FI_Con->QAQB2n * PQ * theVars->FI_RC.k3 / theVars->FI_Pool.PQT; // v3 The rate of exchange of QAQBH2 with PQ
    theVars->FI_Vel.v_r3 = FI_Con->QAQB * FI_Con->PQn * theVars->FI_RC.k_r3 / theVars->FI_Pool.PQT; // v_r3 The rate of exchange of FI_Con.QAQB with PQH2

    theVars->FI_Vel.v3_n = FI_Con->QAnQB2n * PQ * theVars->FI_RC.k3 / theVars->FI_Pool.PQT; // v3_n The rate of exchange of QAnQBH2 with PQ
    theVars->FI_Vel.v_r3_n = FI_Con->QAnQB * FI_Con->PQn * theVars->FI_RC.k_r3 / theVars->FI_Pool.PQT; // v_r3_n The rate of exchange of FI_Con.QAnQB with PQH2

    theVars->FI_Vel.v_pq_ox = FI_Con->PQn * theVars->FI_RC.k_pq_oxy; // v_pq_ox The rate of PQH2 oxidation

    theVars->FI_Vel.v2_1 = FI_Con->P680pPheon * theVars->FI_RC.k2 * q; // v2_1 The rate of FI_Con.P680pPheon oxidation
    theVars->FI_Vel.v2_2 = FI_Con->P680Pheon * theVars->FI_RC.k2 * q;  // v2_1 The rate of FI_Con.P680pPheon oxidation
    const double a = FI_Con->QAQB / (FI_Con->QAQB + FI_Con->QAQBn + FI_Con->QAQB2n);
    const double b = FI_Con->QAQBn / (FI_Con->QAQB + FI_Con->QAQBn + FI_Con->QAQB2n);
    const double c = FI_Con->QAQB2n / (FI_Con->QAQB + FI_Con->QAQBn + FI_Con->QAQB2n);

    theVars->FI_Vel.v2_00_1 = theVars->FI_Vel.v2_1 * a; // v2_00_1 The rate of reduction of FI_Con.QAQB by P680pPheon
    theVars->FI_Vel.v2_01_1 = theVars->FI_Vel.v2_1 * b; // v2_01_1 The rate of reduction of FI_Con.QAQBn by P680pPheon
    theVars->FI_Vel.v2_02_1 = theVars->FI_Vel.v2_1 * c; // v2_02_1 The rate of reduction of FI_Con.QAQB2n by P680pPheon

    theVars->FI_Vel.v2_00_2 = theVars->FI_Vel.v2_2 * a; // v2_00_2 The rate of reduction of FI_Con.QAQB by P680Pheon
    theVars->FI_Vel.v2_01_2 = theVars->FI_Vel.v2_2 * b; // v2_01_2 The rate of reduction of FI_Con.QAQBn by P680Pheon
    theVars->FI_Vel.v2_02_2 = theVars->FI_Vel.v2_2 * c; // v2_02_2 The rate of reduction of FI_Con.QAQB2n by P680Pheon

    const double KE = 1000000;
    const double Coeff1 = FI_Con->P680pPheo / P680PheoT;
    theVars->FI_Vel.vr2_00_1 = FI_Con->QAnQB * theVars->FI_RC.k2 / KE * Coeff1;   // vr2_00_1 The reverse reaction of The rate of reduction of FI_Con.QAQB by P680pPheon
    theVars->FI_Vel.vr2_01_1 = FI_Con->QAnQBn * theVars->FI_RC.k2 / KE * Coeff1;  // vr2_01_1 The reverse reaction of The rate of reduction of FI_Con.QAQBn by P680pPheon
    theVars->FI_Vel.vr2_02_1 = FI_Con->QAnQB2n * theVars->FI_RC.k2 / KE * Coeff1; // vr2_02_1 The reverse reaction of The rate of reduction of FI_Con.QAQB2n by P680pPheon
    theVars->FI_Vel.vr2_1 = theVars->FI_Vel.vr2_00_1 + theVars->FI_Vel.vr2_01_1 + theVars->FI_Vel.vr2_02_1;

    const double Coeff2 = P680Pheo / P680PheoT;// Coeff2
    theVars->FI_Vel.vr2_00_2 = FI_Con->QAnQB * theVars->FI_RC.k2 / KE * Coeff2;// vr2_00_2 The reverse reaction of The rate of reduction of FI_Con.QAQB by P680Pheon
    theVars->FI_Vel.vr2_01_2 = FI_Con->QAnQBn * theVars->FI_RC.k2 / KE * Coeff2;// vr2_01_2 The reverse reaction of The rate of reduction of FI_Con.QAQBn by P680Pheon
    theVars->FI_Vel.vr2_02_2 = FI_Con->QAnQB2n * theVars->FI_RC.k2 / KE * Coeff2;// vr2_02_2 The reverse reaction of The rate of reduction of FI_Con.QAQB2n by P680Pheon
    theVars->FI_Vel.vr2_2 = theVars->FI_Vel.vr2_00_2 + theVars->FI_Vel.vr2_01_2 + theVars->FI_Vel.vr2_02_2;// vr2_2

    theVars->FI_Vel.vP680qU = pow(10, 9) * FI_Con->U * (FI_Con->P680pPheo + FI_Con->P680pPheon) + FI_Con->U * 0.15 * (theVars->FI_RC.kU_f + theVars->FI_RC.kU_d) * PQ / theVars->FI_Pool.PQT;
    theVars->FI_Vel.vP680qA = pow(10, 9) * FI_Con->A * (FI_Con->P680pPheo + FI_Con->P680pPheon) + FI_Con->A * 0.15 * (theVars->FI_RC.kA_f + theVars->FI_RC.kA_d) * PQ / theVars->FI_Pool.PQT;

    ////////////////////////////////////////
    //////  FOR TESITNG  ////
    //const double f = vA_f + vU_f;
    ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //   Part V Output of Velocity for plot          //
    //////////////////////////////////////////////////////////////////////////////////////////////////

    if (t > theVars->FI_OLD_TIME) {
        theVars->FI_TIME_N = theVars->FI_TIME_N + 1;
        theVars->FI_OLD_TIME = t;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assign table
    ////////////////////////////////////////////////////////////////////////////////

    theVars->FI_Vel.vS1_S2  = FI_Con->S1Tp * theVars->FI_RC.k12; // vS1_S2 The rate of transition from S1 to S2
    theVars->FI_Vel.vS2_S3  = FI_Con->S2Tp * theVars->FI_RC.k23; // vS2_S3 The rate of transition from S2 to S3
    theVars->FI_Vel.vS3_S0  = FI_Con->S3Tp * theVars->FI_RC.k30; // vS3_S0 The rate of transition from S3 to S0
    theVars->FI_Vel.vS0_S1  = FI_Con->S0Tp * theVars->FI_RC.k01; // vS0_S1 The rate of transition from S0 to S1

    DEBUG_INTERNAL(theVars->FI_Vel)
    if (theVars->record) {
        theVars->FI_VEL.insert(theVars->FI_TIME_N - 1, t, theVars->FI_Vel);
    }
}
