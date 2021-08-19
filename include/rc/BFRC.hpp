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
namespace RC {

/**
 Class for holding BF_RC data
 */
class BFRC {
public:
    BFRC(){}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFRC object to copy
      */
    BFRC(const BFRC &other) {
        K1 = other.K1;
        K2 = other.K2;
        K3 = other.K3;
        K4 = other.K4;
        K5 = other.K5;
        K6 = other.K6;
        K7 = other.K7;
        K8 = other.K8;
        K9 = other.K9;
        K10 = other.K10;
        Vmax11 = other.Vmax11;
        Kqi = other.Kqi;
        PK = other.PK;
        PMg = other.PMg;
        PCl = other.PCl;
        Kau = other.Kau;
        Kua = other.Kua;
        Kf = other.Kf;
        Kd = other.Kd;
        KE8 = other.KE8;
        KE9 = other.KE9;
        K15 = other.K15;
        K16 = other.K16;
        MemCap = other.MemCap;
        RVA = other.RVA;
        KBs = other.KBs;
        KBl = other.KBl;
        KM1ATP = other.KM1ATP;
        KM1ADP = other.KM1ADP;
        KM1PI = other.KM1PI;
        KM2NADP = other.KM2NADP;
        KM2NADPH = other.KM2NADPH;
        V2M = other.V2M;
        KE2 = other.KE2;
    }
    double K1 = 0.;    ///< The rate constant for formation of ISP.QH2 complex
    double K2 = 0.;    ///< The rate constant for ISP.QH2-->QH(semi) + ISPH(red)
    double K3 = 0.;    ///< The rate constant for QH. + cytbL --> Q + cytbL- + H+   Unit: s-1
    double K4 = 0.;    ///< The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
    double K5 = 0.;    ///< The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
    double K6 = 0.;    ///< The rate constant  for CytbH- + Q- --> cytbH + Q2-      Unit: s-1
    double K7 = 0.;    ///< The rate constant for Q binding to Qi site
    double K8 = 0.;    ///< The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+        Unit: s-1
    double K9 = 0.;    ///< The rate constant for the electron transport from cytc1 to cytc2        Unit: s-1
    double K10 = 0.;   ///< The rate constant for the electron transport from cytc2 to P700 Unit: s-1
    double Vmax11 = 0.; ///< The maximum rate of ATP synthesis       Unit: mmol l-1 s-1
    double Kqi = 0.;   ///< The rate constant for uptake of two protons from the stroma to Q2-      s-1
    double PK = 0.;    ///< The permeability constant for K Unit: cm s-1
    double PMg = 0.;   ///< The permeability constant for Mg        Unit: cm s-1
    double PCl = 0.;   ///< The permeability constant for Cl        Unit: cm s-1
    double Kau = 0.;   ///< The rate constant for exciton transfer from perpheral antenna to core antenna, see FI   Unit: s-1
    double Kua = 0.;   ///< The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI  Unit: s-1
    double Kf = 0.;    ///< The rate constant for fluorescence emission, see the note in FI Unit: s-1
    double Kd = 0.;    ///< The rate constant for heat dissipation
    double KE8 = 0.;   ///< ISPHr + cytc1 --> ISPHox + cytc1-       Unit: s-1
    double KE9 = 0.;   ///< cytc1- + cytc2 --> cytc1 + cytc2-       Unit: s-1
    double K15 = 0.;   ///< The rate constant for primary charge separation in PSI  Unit: s-1
    double K16 = 0.;   ///< The rate constant for electron tranfer from electron acceptor of PSI to Fd      Unit: s-1
    double MemCap = 0.; ///< The membrane capacity
    double RVA = 0.;   ///< The ratio of lumen volume to thylakoid membrane area
    double KBs = 0.;   ///< The buffer equilibrium constant in stroma
    double KBl = 0.;   ///< The buffer equilibrium constant in lumen
    double KM1ATP = 0.; ///< The michaelis menton constant for ATP for ATP synthesis
    double KM1ADP = 0.; ///< The michaelis menton constant for ATP for ADP synthesis
    double KM1PI = 0.; ///< The michaelis menton constant for ATP for PI synthesis
    double KM2NADP = 0.; ///< The michaelis menten constant for NADP  Unit: mmol l-1 s-1
    double KM2NADPH = 0.; ///< The michaelis menten constant for NADPH Unit: mmol l-1 s-1
    double V2M = 0.;   ///< The maximum rate of NADPH formation     Unit: mmol l-1 s-1
    double KE2 = 0.;   ///< Equilibrium constant
};

}  // namespace RC
}  // namespace ePhotosynthesis
