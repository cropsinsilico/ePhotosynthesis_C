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
#include "vel/VelBase.hpp"

namespace ePhotosynthesis {
namespace vel {

/**
 Class for holding the results of the FI_Rate calculations
 */
class FIVel : public VelBase<FIVel, MODULE_FI> {
public:
    DECLARE_VALUE_SET(FIVel, VelBase<FIVel, MODULE_FI>)
    FIVel() : VelBase<FIVel, MODULE_FI>() {
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIVel object to copy
      */
    FIVel(const FIVel &other) : VelBase<FIVel, MODULE_FI>(other) {
	initMembers();
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
	copyMembers(other);
    }

    friend std::ostream& operator<<(std::ostream& out, const FIVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
