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
 Class for holding the result of the Enzyme_Rate calculations
 */
class EnzymeVel {
public:
    EnzymeVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The EnzymeVel object to copy
      */
    EnzymeVel(const EnzymeVel &other) {
        v1 = other.v1;
        v2 = other.v2;
        v3 = other.v3;
        v4 = other.v4;
        v5 = other.v5;
        v6 = other.v6;
        v7 = other.v7;
        v8 = other.v8;
        v10 = other.v10;
        v11 = other.v11;
        v12 = other.v12;
        v13 = other.v13;
        v14 = other.v14;
        v15 = other.v15;
        v18 = other.v18;
        v7Mchl = other.v7Mchl;
        v8Mchl = other.v8Mchl;
        vStarch1 = other.vStarch1;
        vPGASink = other.vPGASink;
        vSuc1 = other.vSuc1;
        vSuc2 = other.vSuc2;
        vSuc3 = other.vSuc3;
        vSuc4 = other.vSuc4;
        vSuc7 = other.vSuc7;
        vSuc8 = other.vSuc8;
        vSuc9 = other.vSuc9;
        vSuc10 = other.vSuc10;
        vATPM = other.vATPM;
        vNADPHM = other.vNADPHM;
        vATPB = other.vATPB;
        vOAA_M = other.vOAA_M;
        vMAL_M = other.vMAL_M;
        vMAL = other.vMAL;
        vMAL_B = other.vMAL_B;
        vPYR_B = other.vPYR_B;
        vPYR = other.vPYR;
        vPYR_M = other.vPYR_M;
        vPEP_M = other.vPEP_M;
        vPGA_B = other.vPGA_B;
        vPGA = other.vPGA;
        vPGA_M = other.vPGA_M;
        vGAP_M = other.vGAP_M;
        vGAP = other.vGAP;
        vGAP_B = other.vGAP_B;
        vDHAP_M = other.vDHAP_M;
        vDHAP = other.vDHAP;
        vDHAP_B = other.vDHAP_B;
        vleak_B = other.vleak_B;
        vleak = other.vleak;
        vNADPHB = other.vNADPHB;
        vpr1 = other.vpr1;
        vpr2 = other.vpr2;
        vpr3 = other.vpr3;
        vpr4 = other.vpr4;
        vpr5 = other.vpr5;
        vpr6 = other.vpr6;
        vpr7 = other.vpr7;
        vpr8 = other.vpr8;
        vpr9 = other.vpr9;
        vpr10 = other.vpr10;
        vinf = other.vinf;
        v62 = other.v62;
        v78 = other.v78;
        v78Mchl = other.v78Mchl;
        vStarch2 = other.vStarch2;
        vgly1B = other.vgly1B;
        vhexp = other.vhexp;
        vSta1 = other.vSta1;
        vSta2 = other.vSta2;
        vSta3 = other.vSta3;
        vtATP = other.vtATP;
        vPCK1 = other.vPCK1;
        vPCK2 = other.vPCK2;
        vPCK3 = other.vPCK3;
        vPCK4 = other.vPCK4;
        vPCK5 = other.vPCK5;
        vPCK6 = other.vPCK6;
        vAsp = other.vAsp;
        vAla = other.vAla;
        vPEP = other.vPEP;
        vOAA_B = other.vOAA_B;
        vATP_B = other.vATP_B;
        vO2_Mchl = other.vO2_Mchl;
        vO2_Bchl = other.vO2_Bchl;
        vtO2 = other.vtO2;
        vtO2_B = other.vtO2_B;
        vtO2_M = other.vtO2_M;
        v5B = other.v5B;
        vPEP_B = other.vPEP_B;
        vpr10M = other.vpr10M;
        vpr8M = other.vpr8M;
        vMDH_BM = other.vMDH_BM;
        vNADME = other.vNADME;
        vtOAA_Bm = other.vtOAA_Bm;
        vPYR_Bm = other.vPYR_Bm;
        vMAL_Bm = other.vMAL_Bm;
        vAspB = other.vAspB;
        vAlaB = other.vAlaB;
        vAspM = other.vAspM;
        vAlaM = other.vAlaM;
        vEA_PPDKRP_I = other.vEA_PPDKRP_I;
        vEA_PPDKRP_A = other.vEA_PPDKRP_A;
    }

    double v1 = 0.;
    double v2 = 0.;
    double v3 = 0.;
    double v4 = 0.;
    double v5 = 0.;
    double v6 = 0.;
    double v7 = 0.;
    double v8 = 0.;
    double v10 = 0.;
    double v11 = 0.;
    double v12 = 0.;
    double v13 = 0.;
    double v14 = 0.;
    double v15 = 0.;
    double v18 = 0.;
    double v7Mchl = 0.;
    double v8Mchl = 0.;
    double vStarch1 = 0.;
    double vPGASink = 0.;
    double vSuc1 = 0.;
    double vSuc2 = 0.;
    double vSuc3 = 0.;
    double vSuc4 = 0.;
    double vSuc7 = 0.;
    double vSuc8 = 0.;
    double vSuc9 = 0.;
    double vSuc10 = 0.;
    double vATPM = 0.;
    double vNADPHM = 0.;
    double vATPB = 0.;
    double vOAA_M = 0.;
    double vMAL_M = 0.;
    double vMAL = 0.;
    double vMAL_B = 0.;
    double vPYR_B = 0.;
    double vPYR = 0.;
    double vPYR_M = 0.;
    double vPEP_M = 0.;
    double vPGA_B = 0.;
    double vPGA = 0.;
    double vPGA_M = 0.;
    double vGAP_M = 0.;
    double vGAP = 0.;
    double vGAP_B = 0.;
    double vDHAP_M = 0.;
    double vDHAP = 0.;
    double vDHAP_B = 0.;
    double vleak_B = 0.;
    double vleak = 0.;
    double vNADPHB = 0.;
    double vpr1 = 0.;
    double vpr2 = 0.;
    double vpr3 = 0.;
    double vpr4 = 0.;
    double vpr5 = 0.;
    double vpr6 = 0.;
    double vpr7 = 0.;
    double vpr8 = 0.;
    double vpr9 = 0.;
    double vpr10 = 0.;
    double vinf = 0.;
    double v62 = 0.;
    double v78 = 0.;
    double v78Mchl = 0.;
    double vStarch2 = 0.;
    double vgly1B = 0.;
    double vhexp = 0.;
    double vSta1 = 0.;
    double vSta2 = 0.;
    double vSta3 = 0.;
    double vtATP = 0.;
    double vPCK1 = 0.;
    double vPCK2 = 0.;
    double vPCK3 = 0.;
    double vPCK4 = 0.;
    double vPCK5 = 0.;
    double vPCK6 = 0.;
    double vAsp = 0.;
    double vAla = 0.;
    double vPEP = 0.;
    double vOAA_B = 0.;
    double vATP_B = 0.;
    double vO2_Mchl = 0.;
    double vO2_Bchl = 0.;
    double vtO2 = 0.;
    double vtO2_B = 0.;
    double vtO2_M = 0.;
    double v5B = 0.;
    double vPEP_B = 0.;
    double vpr10M = 0.;
    double vpr8M = 0.;
    double vMDH_BM = 0.;
    double vNADME = 0.;
    double vtOAA_Bm = 0.;
    double vPYR_Bm = 0.;
    double vMAL_Bm = 0.;
    double vAspB = 0.;
    double vAlaB = 0.;
    double vAspM = 0.;
    double vAlaM = 0.;
    double vEA_PPDKRP_I = 0.;
    double vEA_PPDKRP_A = 0.;

    friend std::ostream& operator<<(std::ostream& out, const EnzymeVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
