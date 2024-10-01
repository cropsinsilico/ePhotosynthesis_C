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
namespace KE {

/**
 Class for RROEA_KE data
 */
class RROEAKE : public ValueSet<MODULE_RROEA, PARAM_TYPE_KE> {
public:
    DECLARE_VALUE_SET(RROEAKE, ValueSet<MODULE_RROEA, PARAM_TYPE_KE>)
    RROEAKE() : ValueSet<MODULE_RROEA, PARAM_TYPE_KE>() {
      initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEAKE object to copy
      */
    RROEAKE(const RROEAKE &other) : ValueSet<MODULE_RROEA, PARAM_TYPE_KE>(other) {
      initMembers();
      *this = other;
    }
    RROEAKE& operator=(const RROEAKE &other) {
        KEe2FBPase = other.KEe2FBPase;
        KEe2SBPase = other.KEe2SBPase;
        KEe2PRK = other.KEe2PRK;
        // KEe2ATPase = other.KEe2ATPase;
        KEe2RuACT = other.KEe2RuACT;
        KEe2GAPDH = other.KEe2GAPDH;
        KEe2MDH = other.KEe2MDH;
        KEe2ATPGPP = other.KEe2ATPGPP;
        KEeFd2Thio = other.KEeFd2Thio;
	copyMembers(other);
	return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(RROEAKE);

}  // namespace KE
}  // namespace ePhotosynthesis
