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

#define MEMBERS_RROEAPool_CONSTANT		\
    SA_GAPDH,					\
    SA_FBPase,					\
    SA_SBPase,					\
    SA_PRK,					\
    SA_ATPase,					\
    SA_ATPGPP,					\
    SA_MDH,					\
    mw_GAPDH,					\
    mw_FBPase,					\
    mw_SBPase,					\
    mw_PRK,					\
    mw_ATPase,					\
    mw_ATPGPP,					\
    mw_MDH,					\
    MDH_Vmax
#define MEMBERS_RROEAPool_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_RROEAPool_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for RROEA_Pool data
 */
class RROEAPool : public ValueSet<MODULE_RROEA, PARAM_TYPE_POOL> {
public:
    DECLARE_VALUE_SET(RROEAPool, ValueSet<MODULE_RROEA, PARAM_TYPE_POOL>)
    RROEAPool() :
      ValueSet<MODULE_RROEA, PARAM_TYPE_POOL>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEAPool object to copy
      */
    RROEAPool(const RROEAPool &other) :
      ValueSet<MODULE_RROEA, PARAM_TYPE_POOL>(other) {
        initMembers();
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
    }

};

  DEFINE_VALUE_SET_HEADER(RROEAPool);

}  // namespace pool
}  // namespace ePhotosynthesis
