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

// class for holding FIBFRC data
class FIBFRC : public RCBase<FIBFRC, MODULE_FIBF> {
public:
    DECLARE_VALUE_SET(FIBFRC, RCBase<FIBFRC, MODULE_FIBF>)
    FIBFRC() : RCBase<FIBFRC, MODULE_FIBF>() {
        initMembers();
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIBFRC object to copy
      */
    FIBFRC(const FIBFRC &other) : RCBase<FIBFRC, MODULE_FIBF>(other) {
      initMembers();
      *this = other;
    }
    FIBFRC& operator=(const FIBFRC &other) {
      RC0 = other.RC0; // The maximal relaxation constant for heat dissipation, which is one term borrowed from Laisk et al., 1997;
      RC = other.RC;
      copyMembers(other);
      return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(FIBFRC);

}  // namespace RC
}  // namespace ePhotosynthesis
