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

#include "../definitions.hpp"
#include "RCBase.hpp"

namespace ePhotosynthesis {
namespace RC {

/**
 Class for holding BF_RC data
 */
class BFRC : public RCBase<BFRC, MODULE_BF> {
public:
    DECLARE_VALUE_SET(BFRC, RCBase<BFRC, MODULE_BF>)
    BFRC() : RCBase<BFRC, MODULE_BF>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFRC object to copy
      */
    BFRC(const BFRC &other) : RCBase<BFRC, MODULE_BF>(other) {
      initMembers();
      *this = other;
    }
    BFRC& operator=(const BFRC &other) {
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
	Em_IPS = other.Em_IPS;
	Em_Cytf = other.Em_Cytf;
	Em_PG = other.Em_PG;
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
	copyMembers(other);
	return *this;
    }
};

  DEFINE_VALUE_SET_HEADER(BFRC);

}  // namespace RC
}  // namespace ePhotosynthesis
