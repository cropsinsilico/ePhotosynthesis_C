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

#include "RROEACondition.hpp"
#include "DynaPSCondition.hpp"

#define PARENT_trDynaPS trDynaPS
#define CHILDREN_trDynaPS DynaPS, RROEA
#define PARAM_TYPES_trDynaPS COND

namespace ePhotosynthesis {
namespace modules {
class trDynaPS;
}
namespace conditions {

/**
 Class for holding inputs to trDynaPS_mb
 */
class trDynaPSCondition : public ConditionBase<trDynaPSCondition, trDynaPSCondition, MODULE_trDynaPS> {
public:
    DECLARE_CONDITION_COMPOSITE(trDynaPS)
    trDynaPSCondition() : RROEA_con(new RROEACondition(this)), DynaPS_con(new DynaPSCondition(this)) {
        initMembers();
    }

    ~trDynaPSCondition() override {
        _clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The trDynaPSCon object to copy
      */
    trDynaPSCondition(const trDynaPSCondition* const other);

    /**
      Constructor to create an object from the contained classes

      @param dother A DynaPSCon object to incorporate
      @param rother A RROEACon object to incorporate
      */
    trDynaPSCondition(DynaPSCondition* dother, RROEACondition* rother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    trDynaPSCondition(const arr &vec, const std::size_t offset = 0);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    trDynaPSCondition(realtype *x);

    RROEACondition* RROEA_con = nullptr;   // child Condition
    DynaPSCondition* DynaPS_con = nullptr;   // child Condition

    void setParent(trDynaPSCondition* par) override {
        (void)par;
    }

private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        if (count == 0)
            count = RROEACondition::size() + DynaPSCondition::size();
        return count;
    }

};

  DEFINE_CONDITION_COMPOSITE_HEADER(trDynaPS);

}  // namespace conditions
}  // namespace ePhotosynthesis
