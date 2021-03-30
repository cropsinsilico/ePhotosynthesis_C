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

std::ostream& operator<<(std::ostream &out, const FICon &in) {
    out << "FICon" << std::endl;
    out << " A = " << in.A << std::endl;
    out << " U = " << in.U << std::endl;
    out << " P680ePheo = " << in.P680ePheo << std::endl;
    out << " P680pPheon= " << in.P680pPheon<< std::endl;
    out << " P680pPheo = " << in.P680pPheo << std::endl;
    out << " P680Pheon = " << in.P680Pheon << std::endl;
    out << " Yz= " << in.Yz<< std::endl;
    out << " S1T = " << in.S1T << std::endl;
    out << " S2T = " << in.S2T << std::endl;
    out << " S3T = " << in.S3T << std::endl;
    out << " S0T = " << in.S0T << std::endl;
    out << " S1Tp= " << in.S1Tp<< std::endl;
    out << " S2Tp= " << in.S2Tp<< std::endl;
    out << " S3Tp= " << in.S3Tp<< std::endl;
    out << " S0Tp= " << in.S0Tp<< std::endl;
    out << " QAQB= " << in.QAQB<< std::endl;
    out << " QAnQB = " << in.QAnQB << std::endl;
    out << " QAQBn = " << in.QAQBn << std::endl;
    out << " QAnQBn= " << in.QAnQBn<< std::endl;
    out << " QAQB2n= " << in.QAQB2n<< std::endl;
    out << " QAnQB2n = " << in.QAnQB2n << std::endl;
    out << " PQn = " << in.PQn << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream &out, const FIVel &in) {
    out << "FIVel" << std::endl;
    out << "  vA_d = " << in.vA_d << std::endl;
    out << "  vA_f = " << in.vA_f << std::endl;
    out << "  vA_U = " << in.vA_U << std::endl;
    out << "  vU_A = " << in.vU_A << std::endl;
    out << "  vU_f = " << in.vU_f << std::endl;
    out << "  vU_d = " << in.vU_d << std::endl;
    out << "  v1 = " << in.v1 << std::endl;
    out << "  v_r1 = " << in.v_r1 << std::endl;
    out << "  vS1_S2 = " << in.vS1_S2 << std::endl;
    out << "  vS2_S3 = " << in.vS2_S3 << std::endl;
    out << "  vS3_S0 = " << in.vS3_S0 << std::endl;
    out << "  vS0_S1 = " << in.vS0_S1 << std::endl;
    out << "  vz_1 = " << in.vz_1 << std::endl;
    out << "  v1z_1= " << in.v1z_1<< std::endl;
    out << "  v2z_1= " << in.v2z_1<< std::endl;
    out << "  v3z_1= " << in.v3z_1<< std::endl;
    out << "  v0z_1= " << in.v0z_1<< std::endl;
    out << "  vz_2 = " << in.vz_2 << std::endl;
    out << "  v1z_2= " << in.v1z_2<< std::endl;
    out << "  v2z_2= " << in.v2z_2<< std::endl;
    out << "  v3z_2= " << in.v3z_2<< std::endl;
    out << "  v0z_2= " << in.v0z_2<< std::endl;
    out << "  v1z= " << in.v1z<< std::endl;
    out << "  v2z= " << in.v2z<< std::endl;
    out << "  v3z= " << in.v3z<< std::endl;
    out << "  v0z= " << in.v0z<< std::endl;
    out << "  vAB1 = " << in.vAB1 << std::endl;
    out << "  vBA1 = " << in.vBA1 << std::endl;
    out << "  vAB2 = " << in.vAB2 << std::endl;
    out << "  vBA2 = " << in.vBA2 << std::endl;
    out << "  v3 = " << in.v3 << std::endl;
    out << "  v_r3 = " << in.v_r3 << std::endl;
    out << "  v3_n = " << in.v3_n << std::endl;
    out << "  v_r3_n = " << in.v_r3_n << std::endl;
    out << "  v_pq_ox= " << in.v_pq_ox<< std::endl;
    out << "  Ic = " << in.Ic << std::endl;
    out << "  Ia = " << in.Ia << std::endl;
    out << "  v2_1 = " << in.v2_1 << std::endl;
    out << "  v2_2 = " << in.v2_2 << std::endl;
    out << "  v2_00_1= " << in.v2_00_1<< std::endl;
    out << "  v2_01_1= " << in.v2_01_1<< std::endl;
    out << "  v2_02_1= " << in.v2_02_1<< std::endl;
    out << "  v2_00_2= " << in.v2_00_2<< std::endl;
    out << "  v2_01_2= " << in.v2_01_2<< std::endl;
    out << "  v2_02_2= " << in.v2_02_2<< std::endl;
    out << "  vr2_00_1 = " << in.vr2_00_1 << std::endl;
    out << "  vr2_01_1 = " << in.vr2_01_1 << std::endl;
    out << "  vr2_02_1 = " << in.vr2_02_1 << std::endl;
    out << "  vr2_1= " << in.vr2_1<< std::endl;
    out << "  vr2_00_2 = " << in.vr2_00_2 << std::endl;
    out << "  vr2_01_2 = " << in.vr2_01_2 << std::endl;
    out << "  vr2_02_2 = " << in.vr2_02_2 << std::endl;
    out << "  vr2_2= " << in.vr2_2<< std::endl;
    out << "  vP680qU= " << in.vP680qU<< std::endl;
    out << "  vP680qA= " << in.vP680qA<< std::endl;
    out << "  vU_P680= " << in.vU_P680<< std::endl;
    out << "  vP680_d= " << in.vP680_d<< std::endl;
    out << "  vP680_f= " << in.vP680_f<< std::endl;

    return out;
}
