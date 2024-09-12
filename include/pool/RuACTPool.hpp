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
namespace pool {

#define MEMBERS_RuACTPool			\
  ET,						\
    Rac,					\
    C,						\
    O,						\
    M
#define MEMBERS_RuACTPool_CONSTANT EMPTY_MEMBER_LIST
#define MEMBERS_RuACTPool_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_RuACTPool_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for RuACT_Pool data
 */
class RuACTPool : public ValueSet<MODULE_RuACT, PARAM_TYPE_POOL> {
public:
    DECLARE_VALUE_SET(RuACTPool, ValueSet<MODULE_RuACT, PARAM_TYPE_POOL>)
    RuACTPool() :
      ValueSet<MODULE_RuACT, PARAM_TYPE_POOL>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTPool object to copy
      */
    RuACTPool(const RuACTPool &other) :
      ValueSet<MODULE_RuACT, PARAM_TYPE_POOL>(other) {
        initMembers();
        ET = other.ET;
        Rac = other.Rac;
        C = other.C;
        O = other.O;
        M = other.M;
	copyMembers(other);
    }

};

  DEFINE_VALUE_SET_HEADER(RuACTPool);

}  // namespace pool
}  // namespace ePhotosynthesis
