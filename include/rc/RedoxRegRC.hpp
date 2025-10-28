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

// class for holding RedoxRegRC data
class RedoxRegRC : public RCBase<RedoxRegRC, MODULE_RedoxReg> {
public:
    DECLARE_VALUE_SET(RedoxRegRC, RCBase<RedoxRegRC, MODULE_RedoxReg>)
    RedoxRegRC() : RCBase<RedoxRegRC, MODULE_RedoxReg>() {
        initMembers();
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RedoxRegRC object to copy
      */
    RedoxRegRC(const RedoxRegRC &other) : RCBase<RedoxRegRC, MODULE_RedoxReg>(other) {
      initMembers();
      *this = other;
    }
    RedoxRegRC& operator=(const RedoxRegRC &other) {
      Em_Thiom = other.Em_Thiom;
      Em_FBPase = other.Em_FBPase;
      Em_SBPase = other.Em_SBPase;
      Em_PRK = other.Em_PRK;
      Em_ATPase = other.Em_ATPase;
      Thiom0 = other.Thiom0;
      FBPase0 = other.FBPase0;
      SBPase0 = other.SBPase0;
      PRK0 = other.PRK0;
      ATPase0 = other.ATPase0;
      copyMembers(other);
      return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(RedoxRegRC);

}  // namespace RC
}  // namespace ePhotosynthesis
