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
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */

#include "conditions/PS_PRCondition.hpp"
#include "conditions/SUCSCondition.hpp"

#define PARENT_CM EPS
#define CHILDREN_CM PS_PR, SUCS
#define PARAM_TYPES_CM COND

namespace ePhotosynthesis {

  FORWARD_DECLARE_CONDITION(CM);
  
namespace conditions {

/**
 Class for holding the inputs to CM_mb
 */
class CMCondition : public ConditionBase<CMCondition, EPSCondition, MODULE_CM> {
public:
    DECLARE_CONDITION_COMPOSITE(CM)
    CMCondition(EPSCondition* par = nullptr) : PS_PR_con(new PS_PRCondition(this)), SUCS_con(new SUCSCondition(this)) {
        setParent(par);
        initMembers();
    }
    ~CMCondition() {
        _clear();
    }

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    CMCondition(const arr &vec, const std::size_t offset = 0);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    CMCondition(realtype *x);

    /**
      Constructor to create an object from the contained classes

      @param pother A PS_PRCondition object to incorporate
      @param sother A SUCSCondition object to incorporate
      */
    CMCondition(PS_PRCondition* pother, SUCSCondition* sother);

    PS_PRCondition* PS_PR_con = nullptr;   // child Condition
    SUCSCondition* SUCS_con = nullptr;     // child Condition

private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        count = PS_PRCondition::size() + SUCSCondition::size();
        return count;
    }
};

  DEFINE_CONDITION_COMPOSITE_HEADER(CM);

}  // namespace conditions
}  // namespace ePhotosynthesis
