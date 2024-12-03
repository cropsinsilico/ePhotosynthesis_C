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
#include <math.h>
#include "definitions.hpp"

#include "FICondition.hpp"
#include "BFCondition.hpp"

#define PARENT_FIBF EPS
#define NRATIO_FIBF 0
#define CHILDREN_FIBF BF, FI
#define PARAM_TYPES_FIBF COND, POOL

namespace ePhotosynthesis {

  FORWARD_DECLARE_CONDITION(FIBF);
  
namespace conditions {

/**
 Class for input to FIBF_mb
 */
class FIBFCondition : public ConditionBase<FIBFCondition, EPSCondition, MODULE_FIBF> {
public:
    DECLARE_CONDITION_COMPOSITE(FIBF)
    FIBFCondition(EPSCondition* par = nullptr) : BF_con(new BFCondition(this)), FI_con(new FICondition(this)) {
        setParent(par);
        initMembers();
    }
    ~FIBFCondition() override {
        _clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIBFCondition object to copy
      */
    FIBFCondition(const FIBFCondition* const other);

    /**
      Constructor to create an object from the contained classes

      @param bother A BFCondition object to incorporate
      @param fother A FICondition object to incorporate
      */
    FIBFCondition(BFCondition* bother, FICondition* fother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FIBFCondition(const arr &vec, const std::size_t offset = 0);


    BFCondition* BF_con = nullptr;  // child Condition
    FICondition* FI_con = nullptr;  // child Condition

private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        if (count == 0)
            count = BFCondition::size() + FICondition::size() + 1;
        return count;
    }

};

  DEFINE_CONDITION_COMPOSITE_HEADER(FIBF);

}  // namespace conditions
}  // namespace ePhotosynthesis
