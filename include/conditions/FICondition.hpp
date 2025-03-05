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

#include "conditions/ConditionBase.hpp"

#define PARENT_FI FIBF
#define NRATIO_FI 23
#define PARAM_TYPES_FI COND, POOL, RC, VEL
#define CONNECT_FI BF
#define COUNT_FI 22
#define CONTROL_FI (BF_connect, PQn)
#define BOOL_MEMBERS_FI

namespace ePhotosynthesis {

  FORWARD_DECLARE_CONDITION(FI);

namespace conditions {

/**
 Class for holding the inputs to FI_mb
 */
class FICondition : public ConditionBase<FICondition, FIBFCondition, MODULE_FI> {
public:
    DECLARE_CONDITION(FI)

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FICondition(const arr &vec, const std::size_t offset = 0);

    SET_GET_BOOL(BF_connect, PQn)
private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        if (BF_connect)
            return count - 1;
        return count;
    }

    /**
      Reset any static data members to their initial state
      */
    static void _reset(const bool noChildren = false) {
	ParentClass::_reset(noChildren);
        setBF_connect(false);
    }

};

    DEFINE_CONDITION_HEADER(FI);

}  // namespace conditions
}  // namespace ePhotosynthesis
