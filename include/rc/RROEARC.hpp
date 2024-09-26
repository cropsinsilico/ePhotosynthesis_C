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
#include "ValueSet.hpp"

namespace ePhotosynthesis {
namespace RC {

#define MEMBERS_RROEARC_CONSTANT \
  ke2ATPase,			 \
    Coeff
#define MEMBERS_RROEARC_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_RROEARC_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  

/**
 Class for RROEA_RC data
 */
class RROEARC : public ValueSet<MODULE_RROEA, PARAM_TYPE_RC> {
public:
    DECLARE_VALUE_SET(RROEARC, ValueSet<MODULE_RROEA, PARAM_TYPE_RC>)
    RROEARC() : ValueSet<MODULE_RROEA, PARAM_TYPE_RC>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEARC object to copy
      */
    RROEARC(const RROEARC &other) :
      ValueSet<MODULE_RROEA, PARAM_TYPE_RC>(other) {
        initMembers();
        ke2GAPDH = other.ke2GAPDH;
        ke2MDH = other.ke2MDH;
        ke2FBPase = other.ke2FBPase;
        ke2SBPase = other.ke2SBPase;
        ke2PRK = other.ke2PRK;
	// ke2ATPase = other.ke2ATPase;
        ke2RubACT = other.ke2RubACT;
        ke2Fd = other.ke2Fd;
        keFd2Thio = other.keFd2Thio;
        keFd2Calvin = other.keFd2Calvin;
        ke2ATPGPP = other.ke2ATPGPP;
	copyMembers(other);
    }

};

  DEFINE_VALUE_SET_HEADER(RROEARC);

}  // namespace RC
}  // namespace ePhotosynthesis
