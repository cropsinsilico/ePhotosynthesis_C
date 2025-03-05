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

#include "conditions/FIBFCondition.hpp"
#include "conditions/CMCondition.hpp"

#define PARENT_EPS RA
#define CHILDREN_EPS FIBF, CM
#define PARAM_TYPES_EPS COND

namespace ePhotosynthesis {

  FORWARD_DECLARE_CONDITION(EPS);
  
namespace conditions {

/**
 Class for holding the inputs to EPS_mb
 */
class EPSCondition : public ConditionBase<EPSCondition, RACondition, MODULE_EPS> {
public:
    DECLARE_CONDITION_COMPOSITE(EPS)
    EPSCondition(RACondition* par = nullptr) : CM_con(new CMCondition(this)), FIBF_con(new FIBFCondition(this)) {
        setParent(par);
        initMembers();
    }
    ~EPSCondition() override {
        _clear();
    }

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    EPSCondition(realtype *x);

    /**
      Constructor to create an object from the contained classes

      @param fother A FIBFCondition object to incorporate
      @param cother A CMCondition object to incorporate
      */
    EPSCondition(FIBFCondition* fother, CMCondition* cother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    EPSCondition(const arr &vec, const std::size_t offset = 0);

    CMCondition* CM_con = nullptr;       // child Condition
    FIBFCondition* FIBF_con = nullptr;   // child Condition

private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        return CMCondition::size() + FIBFCondition::size();
    }

};

  DEFINE_CONDITION_COMPOSITE_HEADER(EPS);

}  // namespace conditions
}  // namespace ePhotosynthesis
