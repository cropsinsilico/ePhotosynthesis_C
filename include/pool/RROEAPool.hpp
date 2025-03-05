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
#include "pool/PoolBase.hpp"

namespace ePhotosynthesis {
namespace pool {

/**
 Class for RROEA_Pool data
 */
class RROEAPool : public PoolBase<RROEAPool, MODULE_RROEA> {
public:
    DECLARE_VALUE_SET(RROEAPool, PoolBase<RROEAPool, MODULE_RROEA>)
    RROEAPool() :
      PoolBase<RROEAPool, MODULE_RROEA>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEAPool object to copy
      */
    RROEAPool(const RROEAPool &other) : PoolBase<RROEAPool, MODULE_RROEA>(other) {
      initMembers();
      *this = other;
    }
    RROEAPool& operator=(const RROEAPool &other) {
        GAPDH = other.GAPDH;
        FBPase = other.FBPase;
        SBPase = other.SBPase;
        PRK = other.PRK;
        ATPase = other.ATPase;
        ATPGPP = other.ATPGPP;
        MDH = other.MDH;
        ThioT = other.ThioT;
        FdT = other.FdT;
        RuACTT = other.RuACTT;
	copyMembers(other);
	return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(RROEAPool);

}  // namespace pool
}  // namespace ePhotosynthesis
