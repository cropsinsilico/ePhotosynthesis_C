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

#include "../ValueSet.hpp"

#define MEMBERS_RC BF, FI, FIBF, PR, PS, RROEA, RuACT, RedoxReg, SUCS, XanCycle

DECLARE_PARAM_BASE(RC)

#define COPY_RC_MEMBER(name) name = other.name
#define DECLARE_RC(name)                                                \
  DECLARE_VALUE_SET(name ## RC, RCBase<name ## RC, MODULE_ ## name>)    \
  name ## RC() : RCBase<name ## RC, MODULE_ ## name>() {                \
    initMembers();                                                      \
  }                                                                     \
  /** Copy constructor that makes a deep copy of the given object */    \
  /**  @param other The RC object to copy */                            \
  name ## RC(const name ## RC &other) : RCBase<name ## RC, MODULE_ ## name>(other) { \
    initMembers();                                                      \
    *this = other;                                                      \
  }                                                                     \
  name ## RC& operator=(const name ## RC &other) {                      \
    FOR_EACH(COPY_RC_MEMBER, VARS_MEMBER_NAMES(name, RC));              \
    copyMembers(other);                                                 \
    return *this;                                                       \
  }

#define DEFINE_RC_HEADER(name)                  \
  DEFINE_VALUE_SET_HEADER(name ## RC)
