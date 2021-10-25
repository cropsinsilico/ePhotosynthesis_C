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

#include "vel/EnzymeVel.hpp"

namespace ePhotosynthesis {
namespace vel {
std::ostream& operator<<(std::ostream &out, const EnzymeVel &in) {
    out << "EnzymeVel" << std::endl;
    out << "  v1 = " << in.v1 << std::endl;
    out << "  v2 = " << in.v2 << std::endl;
    out << "  v3 = " << in.v3 << std::endl;
    out << "  v4 = " << in.v4 << std::endl;
    out << "  v5 = " << in.v5 << std::endl;
    out << "  v6 = " << in.v6 << std::endl;
    out << "  v7 = " << in.v7 << std::endl;
    out << "  v8 = " << in.v8 << std::endl;
    out << "  v10 = " << in.v10 << std::endl;
    out << "  v11 = " << in.v11 << std::endl;
    out << "  v12 = " << in.v12 << std::endl;
    out << "  v13 = " << in.v13 << std::endl;
    out << "  v14 = " << in.v14 << std::endl;
    out << "  v15 = " << in.v15 << std::endl;
    out << "  v18 = " << in.v18 << std::endl;
    out << "  v7Mchl = " << in.v7Mchl << std::endl;
    out << "  v8Mchl = " << in.v8Mchl << std::endl;
    out << "  vStarch1 = " << in.vStarch1 << std::endl;
    out << "  vPGASink = " << in.vPGASink << std::endl;
    out << "  vSuc1 = " << in.vSuc1 << std::endl;
    out << "  vSuc2 = " << in.vSuc2 << std::endl;
    out << "  vSuc3 = " << in.vSuc3 << std::endl;
    out << "  vSuc4 = " << in.vSuc4 << std::endl;
    out << "  vSuc7 = " << in.vSuc7 << std::endl;
    out << "  vSuc8 = " << in.vSuc8 << std::endl;
    out << "  vSuc9 = " << in.vSuc9 << std::endl;
    out << "  vSuc10 = " << in.vSuc10 << std::endl;
    out << "  vATPM = " << in.vATPM << std::endl;
    out << "  vNADPHM = " << in.vNADPHM << std::endl;
    out << "  vATPB = " << in.vATPB << std::endl;
    out << "  vOAA_M = " << in.vOAA_M << std::endl;
    out << "  vMAL_M = " << in.vMAL_M << std::endl;
    out << "  vMAL = " << in.vMAL << std::endl;
    out << "  vMAL_B = " << in.vMAL_B << std::endl;
    out << "  vPYR_B = " << in.vPYR_B << std::endl;
    out << "  vPYR = " << in.vPYR << std::endl;
    out << "  vPYR_M = " << in.vPYR_M << std::endl;
    out << "  vPEP_M = " << in.vPEP_M << std::endl;
    out << "  vPGA_B = " << in.vPGA_B << std::endl;
    out << "  vPGA = " << in.vPGA << std::endl;
    out << "  vPGA_M = " << in.vPGA_M << std::endl;
    out << "  vGAP_M = " << in.vGAP_M << std::endl;
    out << "  vGAP = " << in.vGAP << std::endl;
    out << "  vGAP_B = " << in.vGAP_B << std::endl;
    out << "  vDHAP_M = " << in.vDHAP_M << std::endl;
    out << "  vDHAP = " << in.vDHAP << std::endl;
    out << "  vDHAP_B = " << in.vDHAP_B << std::endl;
    out << "  vleak_B = " << in.vleak_B << std::endl;
    out << "  vleak = " << in.vleak << std::endl;
    out << "  vNADPHB = " << in.vNADPHB << std::endl;
    out << "  vpr1 = " << in.vpr1 << std::endl;
    out << "  vpr2 = " << in.vpr2 << std::endl;
    out << "  vpr3 = " << in.vpr3 << std::endl;
    out << "  vpr4 = " << in.vpr4 << std::endl;
    out << "  vpr5 = " << in.vpr5 << std::endl;
    out << "  vpr6 = " << in.vpr6 << std::endl;
    out << "  vpr7 = " << in.vpr7 << std::endl;
    out << "  vpr8 = " << in.vpr8 << std::endl;
    out << "  vpr9 = " << in.vpr9 << std::endl;
    out << "  vpr10 = " << in.vpr10 << std::endl;
    out << "  vinf = " << in.vinf << std::endl;
    out << "  v62 = " << in.v62 << std::endl;
    out << "  v78 = " << in.v78 << std::endl;
    out << "  v78Mchl = " << in.v78Mchl << std::endl;
    out << "  vStarch2 = " << in.vStarch2 << std::endl;
    out << "  vgly1B = " << in.vgly1B << std::endl;
    out << "  vhexp = " << in.vhexp << std::endl;
    out << "  vSta1 = " << in.vSta1 << std::endl;
    out << "  vSta2 = " << in.vSta2 << std::endl;
    out << "  vSta3 = " << in.vSta3 << std::endl;
    out << "  vtATP = " << in.vtATP << std::endl;
    out << "  vPCK1 = " << in.vPCK1 << std::endl;
    out << "  vPCK2 = " << in.vPCK2 << std::endl;
    out << "  vPCK3 = " << in.vPCK3 << std::endl;
    out << "  vPCK4 = " << in.vPCK4 << std::endl;
    out << "  vPCK5 = " << in.vPCK5 << std::endl;
    out << "  vPCK6 = " << in.vPCK6 << std::endl;
    out << "  vAsp = " << in.vAsp << std::endl;
    out << "  vAla = " << in.vAla << std::endl;
    out << "  vPEP = " << in.vPEP << std::endl;
    out << "  vOAA_B = " << in.vOAA_B << std::endl;
    out << "  vATP_B = " << in.vATP_B << std::endl;
    out << "  vO2_Mchl = " << in.vO2_Mchl << std::endl;
    out << "  vO2_Bchl = " << in.vO2_Bchl << std::endl;
    out << "  vtO2 = " << in.vtO2 << std::endl;
    out << "  vtO2_B = " << in.vtO2_B << std::endl;
    out << "  vtO2_M = " << in.vtO2_M << std::endl;
    out << "  v5B = " << in.v5B << std::endl;
    out << "  vPEP_B = " << in.vPEP_B << std::endl;
    out << "  vpr10M = " << in.vpr10M << std::endl;
    out << "  vpr8M = " << in.vpr8M << std::endl;
    out << "  vMDH_BM = " << in.vMDH_BM << std::endl;
    out << "  vNADME = " << in.vNADME << std::endl;
    out << "  vtOAA_Bm = " << in.vtOAA_Bm << std::endl;
    out << "  vPYR_Bm = " << in.vPYR_Bm << std::endl;
    out << "  vMAL_Bm = " << in.vMAL_Bm << std::endl;
    out << "  vAspB = " << in.vAspB << std::endl;
    out << "  vAlaB = " << in.vAlaB << std::endl;
    out << "  vAspM = " << in.vAspM << std::endl;
    out << "  vAlaM = " << in.vAlaM << std::endl;
    out << "  vEA_PPDKRP_I = " << in.vEA_PPDKRP_I << std::endl;
    out << "  vEA_PPDKRP_A = " << in.vEA_PPDKRP_A << std::endl;

    return out;
}

}
}
