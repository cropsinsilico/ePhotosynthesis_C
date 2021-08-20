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

#include "definitions.hpp"

namespace ePhotosynthesis {
namespace vel {

/**
 Class for holding the results of the FI_Rate calculations
 */
class FIVel {
public:
    FIVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIVel object to copy
      */
    FIVel(const FIVel &other) {
        vA_d = other.vA_d;
        vA_f = other.vA_f;
        vA_U = other.vA_U;
        vU_A = other.vU_A;
        vU_f = other.vU_f;
        vU_d = other.vU_d;
        v1 = other.v1;
        v_r1 = other.v_r1;
        vS1_S2 = other.vS1_S2;
        vS2_S3 = other.vS2_S3;
        vS3_S0 = other.vS3_S0;
        vS0_S1 = other.vS0_S1;
        vz_1 = other.vz_1;
        v1z_1 = other.v1z_1;
        v2z_1 = other.v2z_1;
        v3z_1 = other.v3z_1;
        v0z_1 = other.v0z_1;
        vz_2 = other.vz_2;
        v1z_2 = other.v1z_2;
        v2z_2 = other.v2z_2;
        v3z_2 = other.v3z_2;
        v0z_2 = other.v0z_2;
        v1z = other.v1z;
        v2z = other.v2z;
        v3z = other.v3z;
        v0z = other.v0z;
        vAB1 = other.vAB1;
        vBA1 = other.vBA1;
        vAB2 = other.vAB2;
        vBA2 = other.vBA2;
        v3 = other.v3;
        v_r3 = other.v_r3;
        v3_n = other.v3_n;
        v_r3_n = other.v_r3_n;
        v_pq_ox = other.v_pq_ox;
        Ic = other.Ic;
        Ia = other.Ia;
        v2_1 = other.v2_1;
        v2_2 = other.v2_2;
        v2_00_1 = other.v2_00_1;
        v2_01_1 = other.v2_01_1;
        v2_02_1 = other.v2_02_1;
        v2_00_2 = other.v2_00_2;
        v2_01_2 = other.v2_01_2;
        v2_02_2 = other.v2_02_2;
        vr2_00_1 = other.vr2_00_1;
        vr2_01_1 = other.vr2_01_1;
        vr2_02_1 = other.vr2_02_1;
        vr2_1 = other.vr2_1;
        vr2_00_2 = other.vr2_00_2;
        vr2_01_2 = other.vr2_01_2;
        vr2_02_2 = other.vr2_02_2;
        vr2_2 = other.vr2_2;
        vP680qU = other.vP680qU;
        vP680qA = other.vP680qA;
        vU_P680 = other.vU_P680;
        vP680_d = other.vP680_d;
        vP680_f = other.vP680_f;
    }

    double vA_d = 0.;    //  vA_d  The rate of heat dissipation from peripheral antenna
    double vA_f = 0.;    //  vA_f  The rate of fluorescence emission from peripheral antenna
    double vA_U = 0.;    //  vA_U  The rate of exciton transfer from peripheral antenna to core antenna in open reaction center
    double vU_A = 0.;    //  vU_A  The rate of exciton transfer from core antenna to perpheral antenna in open center
    double vU_f = 0.;    //  vU_f  The rate of fluorescence emission from core antenna
    double vU_d = 0.;    //  vU_d  The rate of heat dissipation from core antenna
    double v1 = 0.;      //  v1  The rate of primary charge separation
    double v_r1 = 0.;    //  v_r1  The rate of charge recombination
    double vS1_S2 = 0.;  //  vS1_S2  The rate of transition from S1 to S2
    double vS2_S3 = 0.;  //  vS2_S3  The rate of transition from S2 to S3
    double vS3_S0 = 0.;  //  vS3_S0  The rate of transition from S3 to S0
    double vS0_S1 = 0.;  //  vS0_S1  The rate of transition from S0 to S1
    double vz_1 = 0.;    //  vz_1  The rate of P680p reduction
    double v1z_1 = 0.;   //  v1z_1  The rate of oxidation of S1T by P680pPheon
    double v2z_1 = 0.;   //  v2z_1  The rate of oxidation of S2T  by P680pPheon
    double v3z_1 = 0.;   //  v3z_1  The rate of oxidation of S3T  by P680pPheon
    double v0z_1 = 0.;   //  v0z_1  The rate of oxidation of S0T  by P680pPheon
    double vz_2 = 0.;    //  vz_2  The rate of P680pPheon reduction
    double v1z_2 = 0.;   //  v1z_2  The rate of oxidation of S1T by P680pPheo
    double v2z_2 = 0.;   //  v2z_2  The rate of oxidation of S2T  by P680pPheo
    double v3z_2 = 0.;   //  v3z_2  The rate of oxidation of S3T  by P680pPheo
    double v0z_2 = 0.;   //  v0z_2  The rate of oxidation of S0T  by P680pPheo
    double v1z = 0.;
    double v2z = 0.;
    double v3z = 0.;
    double v0z = 0.;
    double vAB1 = 0.;    //  vAB1  The rate of electron transfer from QA- to QB
    double vBA1 = 0.;    //  vBA1  The rate of electron transfer from QB- to QA
    double vAB2 = 0.;    //  vAB2  The rate of electron transfer from QA- to QB-
    double vBA2 = 0.;    //  vBA2  The rate of electron transfer from QB2- TO QA
    double v3 = 0.;      //  v3  The rate of exchange of QAQBH2 with PQ
    double v_r3 = 0.;    //  v_r3  The rate of exchange of QAQB with PQH2
    double v3_n = 0.;    //  v3_n  The rate of exchange of QAnQBH2 with PQ
    double v_r3_n = 0.;  //  v_r3_n  The rate of exchange of QAnQB with PQH2
    double v_pq_ox = 0.; //  v_pq_ox  The rate of PQH2 oxidation
    double Ic = 0.;      //  Ic  The incident light on the core antenna
    double Ia = 0.;      //  Ia  The incident light on the peripheral antenna
    double v2_1 = 0.;    //  v2_1  The rate of P680pPheon oxidation
    double v2_2 = 0.;    //  v2_1  The rate of P680pPheon oxidation
    double v2_00_1 = 0.; //  v2_00_1  The rate of reduction of QAQB by P680pPheon
    double v2_01_1 = 0.; //  v2_01_1  The rate of reduction of QAQBn by P680pPheon
    double v2_02_1 = 0.; //  v2_02_1  The rate of reduction of QAQB2n by P680pPheon
    double v2_00_2 = 0.; //  v2_00_2  The rate of reduction of QAQB by P680Pheon
    double v2_01_2 = 0.; //  v2_01_2  The rate of reduction of QAQBn by P680Pheon
    double v2_02_2 = 0.; //  v2_02_2  The rate of reduction of QAQB2n by P680Pheon
    double vr2_00_1 = 0.;//  vr2_00_1  The reverse reaction of The rate of reduction of QAQB by P680pPheon
    double vr2_01_1 = 0.;//  vr2_01_1  The reverse reaction of The rate of reduction of QAQBn by P680pPheon
    double vr2_02_1 = 0.;//  vr2_02_1  The reverse reaction of The rate of reduction of QAQB2n by P680pPheon
    double vr2_1 = 0.;
    double vr2_00_2 = 0.;//  vr2_00_2  The reverse reaction of The rate of reduction of QAQB by P680Pheon
    double vr2_01_2 = 0.;//  vr2_01_2  The reverse reaction of The rate of reduction of QAQBn by P680Pheon
    double vr2_02_2 = 0.;//  vr2_02_2  The reverse reaction of The rate of reduction of QAQB2n by P680Pheon
    double vr2_2 = 0.;
    double vP680qU = 0.;
    double vP680qA = 0.;
    double vU_P680 = 0.;
    double vP680_d = 0.;
    double vP680_f = 0.;

    friend std::ostream& operator<<(std::ostream& out, const FIVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
