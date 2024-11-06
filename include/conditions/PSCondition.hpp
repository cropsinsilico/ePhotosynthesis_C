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

//#include "definitions.hpp"
#include "ConditionBase.hpp"

#define PARENT_PS PS_PR
#define NRATIO_PS 103
#define PARAM_TYPES_PS COND, VEL

namespace ePhotosynthesis {
namespace modules {
class PS;
}
namespace conditions {

class PS_PRCondition;

/**
 Class for holding the inputs to PS_mb
 */
class PSCondition : public ConditionBase<PSCondition, PS_PRCondition, MODULE_PS> {
public:
    DECLARE_CONDITION(PS)
    PSCondition(PS_PRCondition* par = nullptr) {
        setParent(par);
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PSCon object to copy
      */
    PSCondition(const PSCondition* const other);
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    PSCondition(const arr &vec, const std::size_t offset = 0);

    SET_GET_BOOL(C3, ADPG)
private:
    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        if (C3)
            return count - 1;
        return count;
    }
    /**
      Reset any static data members to their initial state
      */
    static void _reset() {
      setC3(false);
    }

    static const std::size_t count;  // size of the current serialized output
};

    DEFINE_CONDITION_HEADER(PS);

}  // namespace conditions
}  // namespace ePhotosynthesis
