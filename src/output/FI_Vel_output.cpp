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

#include "vel/FIVel.hpp"

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
    out << "  v1z_1 = " << in.v1z_1 << std::endl;
    out << "  v2z_1 = " << in.v2z_1 << std::endl;
    out << "  v3z_1 = " << in.v3z_1 << std::endl;
    out << "  v0z_1 = " << in.v0z_1 << std::endl;
    out << "  vz_2 = " << in.vz_2 << std::endl;
    out << "  v1z_2 = " << in.v1z_2 << std::endl;
    out << "  v2z_2 = " << in.v2z_2 << std::endl;
    out << "  v3z_2 = " << in.v3z_2 << std::endl;
    out << "  v0z_2 = " << in.v0z_2 << std::endl;
    out << "  v1z = " << in.v1z << std::endl;
    out << "  v2z = " << in.v2z << std::endl;
    out << "  v3z = " << in.v3z << std::endl;
    out << "  v0z = " << in.v0z << std::endl;
    out << "  vAB1 = " << in.vAB1 << std::endl;
    out << "  vBA1 = " << in.vBA1 << std::endl;
    out << "  vAB2 = " << in.vAB2 << std::endl;
    out << "  vBA2 = " << in.vBA2 << std::endl;
    out << "  v3 = " << in.v3 << std::endl;
    out << "  v_r3 = " << in.v_r3 << std::endl;
    out << "  v3_n = " << in.v3_n << std::endl;
    out << "  v_r3_n = " << in.v_r3_n << std::endl;
    out << "  v_pq_ox = " << in.v_pq_ox << std::endl;
    out << "  Ic = " << in.Ic << std::endl;
    out << "  Ia = " << in.Ia << std::endl;
    out << "  v2_1 = " << in.v2_1 << std::endl;
    out << "  v2_2 = " << in.v2_2 << std::endl;
    out << "  v2_00_1 = " << in.v2_00_1 << std::endl;
    out << "  v2_01_1 = " << in.v2_01_1 << std::endl;
    out << "  v2_02_1 = " << in.v2_02_1 << std::endl;
    out << "  v2_00_2 = " << in.v2_00_2 << std::endl;
    out << "  v2_01_2 = " << in.v2_01_2 << std::endl;
    out << "  v2_02_2 = " << in.v2_02_2 << std::endl;
    out << "  vr2_00_1 = " << in.vr2_00_1 << std::endl;
    out << "  vr2_01_1 = " << in.vr2_01_1 << std::endl;
    out << "  vr2_02_1 = " << in.vr2_02_1 << std::endl;
    out << "  vr2_1 = " << in.vr2_1 << std::endl;
    out << "  vr2_00_2 = " << in.vr2_00_2 << std::endl;
    out << "  vr2_01_2 = " << in.vr2_01_2 << std::endl;
    out << "  vr2_02_2 = " << in.vr2_02_2 << std::endl;
    out << "  vr2_2 = " << in.vr2_2 << std::endl;
    out << "  vP680qU = " << in.vP680qU << std::endl;
    out << "  vP680qA = " << in.vP680qA << std::endl;
    out << "  vU_P680 = " << in.vU_P680 << std::endl;
    out << "  vP680_d = " << in.vP680_d << std::endl;
    out << "  vP680_f = " << in.vP680_f << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream &out, const FIVel* in) {
    out << *in;
    return out;
}
