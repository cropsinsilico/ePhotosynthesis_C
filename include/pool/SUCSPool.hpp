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
 Class for SUCS_Pool data
 */
class SUCSPool : public PoolBase<SUCSPool, MODULE_SUCS> {
public:
    DECLARE_VALUE_SET(SUCSPool, PoolBase<SUCSPool, MODULE_SUCS>)
    SUCSPool() :
      PoolBase<SUCSPool, MODULE_SUCS>() {
        initMembers();
    }
    SUCSPool(const SUCSPool &other) : PoolBase<SUCSPool, MODULE_SUCS>(other) {
      initMembers();
      *this = other;
    }
    SUCSPool& operator=(const SUCSPool &other) {
        ATc = other.ATc;
        UTc = other.UTc;
        PTc = other.PTc;
	copyMembers(other);
	return *this;
    }
};

  DEFINE_VALUE_SET_HEADER(SUCSPool);

}  // namespace pool
}  // namespace ePhotosynthesis
