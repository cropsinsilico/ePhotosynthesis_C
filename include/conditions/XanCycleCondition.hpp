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

#include "ConditionBase.hpp"

#define PARENT_XanCycle DynaPS
#define NRATIO_XanCycle 4
#define PARAM_TYPES_XanCycle COND, VEL

namespace ePhotosynthesis {
namespace modules {
class XanCycle;
}
namespace conditions {

class DynaPSCondition;

/**
 Class to hold the inputs to XanCycle_mb
 */
class XanCycleCondition : public ConditionBase<XanCycleCondition, DynaPSCondition, MODULE_XanCycle> {
public:
    DECLARE_CONDITION(XanCycle)
    XanCycleCondition(DynaPSCondition* par = nullptr) {
        setParent(par);
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The XanCycleCon object to copy
      */
    XanCycleCondition(const XanCycleCondition* const other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    XanCycleCondition(const arr &vec, const std::size_t offset = 0);

private:
    /** \copydoc ConditionBase::_clear */
    void _clear() override;
  
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        return count;
    }

    /**
      Reset any static data members to their initial state
      */
    static void _reset() {}

    static const std::size_t count;  // size of the current serialized output
};

    DEFINE_CONDITION_HEADER(XanCycle);

}  // namespace conditions
}  // namespace ePhotosynthesis
