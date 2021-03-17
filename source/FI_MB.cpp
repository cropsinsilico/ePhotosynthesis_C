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

// This is the routine for calculation of the mass balance equations for the fluorescence induction model
// This routine is composed of two components;
// 1) The initialization of the rates that was transfered from the FI_Rate routine
// 2) The computation of the mass balance equations

arr FI::FI_Mb(const double t, const FICon &FI_Con, Variables *theVars) {

    //////////////////////////////////////////////////////////////////
    //   Calculate the rates first   //
    //////////////////////////////////////////////////////////////////
    if (theVars->useC3) {
        theVars->FI_Param[0] = theVars->LI * 30;
    } else {
        Condition(t, theVars);
        theVars->FI_Param[0] = theVars->GLight;
    }

    FI_Rate(t, FI_Con, theVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation //
    ////////////////////////////////////////////////////////////////

    // This page defines the mass balance equation for the system under study
    // One problem need to be taken care of is the variables needed to transfer from FI_CalV to FI_mb
    // The Major Variables
    arr FI_mb = zeros(22);
    FI_mb[0] = theVars->FI_Vel.Ia - theVars->FI_Vel.vA_f - theVars->FI_Vel.vA_d - theVars->FI_Vel.vA_U + theVars->FI_Vel.vU_A; // A 6
    FI_mb[1] = theVars->FI_Vel.Ic + theVars->FI_Vel.vA_U - theVars->FI_Vel.vU_A - theVars->FI_Vel.vU_f - theVars->FI_Vel.vU_d - theVars->FI_Vel.v1 + theVars->FI_Vel.v_r1 - theVars->FI_Vel.vP680qU; // U
    FI_mb[2] = theVars->FI_Vel.vU_P680 + theVars->FI_Vel.v_r1 - theVars->FI_Vel.v1 - theVars->FI_Vel.vP680_d; // P680ePheo QF add
    FI_mb[3] = theVars->FI_Vel.v1 - theVars->FI_Vel.v_r1 - theVars->FI_Vel.vz_1 - theVars->FI_Vel.v2_1 + theVars->FI_Vel.vr2_1; // P680pPheon
    FI_mb[4] = theVars->FI_Vel.v2_1 - theVars->FI_Vel.vr2_1 - theVars->FI_Vel.vz_2; // P680pPheo
    FI_mb[5] = theVars->FI_Vel.vz_1 - theVars->FI_Vel.v2_2 + theVars->FI_Vel.vr2_2; // P680Pheon
    FI_mb[6] = theVars->FI_Vel.vS1_S2 + theVars->FI_Vel.vS2_S3 + theVars->FI_Vel.vS3_S0 + theVars->FI_Vel.vS0_S1 - theVars->FI_Vel.vz_1 - theVars->FI_Vel.vz_2; // Yz
    FI_mb[7] = theVars->FI_Vel.vS0_S1 - theVars->FI_Vel.v1z;  // S1T
    FI_mb[8] = theVars->FI_Vel.vS1_S2 - theVars->FI_Vel.v2z;  // S2T
    FI_mb[9] = theVars->FI_Vel.vS2_S3 - theVars->FI_Vel.v3z;  // S3T
    FI_mb[10] = theVars->FI_Vel.vS3_S0 - theVars->FI_Vel.v0z; // S0T
    FI_mb[11] = theVars->FI_Vel.v1z - theVars->FI_Vel.vS1_S2; // S1Tp
    FI_mb[12] = theVars->FI_Vel.v2z - theVars->FI_Vel.vS2_S3; // S2Tp
    FI_mb[13] = theVars->FI_Vel.v3z - theVars->FI_Vel.vS3_S0; // S3Tp
    FI_mb[14] = theVars->FI_Vel.v0z - theVars->FI_Vel.vS0_S1; // S0Tp
    FI_mb[15] = theVars->FI_Vel.v3 - theVars->FI_Vel.v_r3 - theVars->FI_Vel.v2_00_1 - theVars->FI_Vel.v2_00_2 + theVars->FI_Vel.vr2_00_1 + theVars->FI_Vel.vr2_00_2; // QAQB
    FI_mb[16] = theVars->FI_Vel.v2_00_1 + theVars->FI_Vel.v2_00_2 - theVars->FI_Vel.vr2_00_1 - theVars->FI_Vel.vr2_00_2 - theVars->FI_Vel.vAB1 + theVars->FI_Vel.vBA1 + theVars->FI_Vel.v3_n - theVars->FI_Vel.v_r3_n; // QAnQB
    FI_mb[17] = theVars->FI_Vel.vAB1 - theVars->FI_Vel.vBA1 - theVars->FI_Vel.v2_01_1 - theVars->FI_Vel.v2_01_2 + theVars->FI_Vel.vr2_01_1 + theVars->FI_Vel.vr2_01_2; // QAQBn
    FI_mb[18] = theVars->FI_Vel.vBA2 - theVars->FI_Vel.vAB2 + theVars->FI_Vel.v2_01_1 + theVars->FI_Vel.v2_01_2 - theVars->FI_Vel.vr2_01_1 - theVars->FI_Vel.vr2_01_2; // QAnQBn
    FI_mb[19] = theVars->FI_Vel.vAB2 - theVars->FI_Vel.vBA2 - theVars->FI_Vel.v3 + theVars->FI_Vel.v_r3 - theVars->FI_Vel.v2_02_1 - theVars->FI_Vel.v2_02_2 + theVars->FI_Vel.vr2_02_1 + theVars->FI_Vel.vr2_02_2; // QAQB2n
    FI_mb[20] = 0 - theVars->FI_Vel.v3_n + theVars->FI_Vel.v_r3_n + theVars->FI_Vel.v2_02_1 + theVars->FI_Vel.v2_02_2 - theVars->FI_Vel.vr2_02_1 - theVars->FI_Vel.vr2_02_2; // QAnQB2n
    FI_mb[21] = theVars->FI_Vel.v3 + theVars->FI_Vel.v3_n - theVars->FI_Vel.v_r3 - theVars->FI_Vel.v_r3_n - theVars->FI_Vel.v_pq_ox; // PQn
    return FI_mb;
}
