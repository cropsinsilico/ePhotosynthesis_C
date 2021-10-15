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

#include "PS_PRCondition.hpp"
#include "SUCSCondition.hpp"

namespace ePhotosynthesis {
namespace modules {
class CM;
}
namespace conditions {

class EPSCondition;
/**
 Class for holding the inputs to CM_mb
 */
class CMCondition : public ConditionBase<CMCondition, EPSCondition> {
public:
    CMCondition(EPSCondition* par = nullptr) : PS_PR_con(new PS_PRCondition(this)), SUCS_con(new SUCSCondition(this)) {
        setParent(par);
    }
    ~CMCondition() {
        _clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The CMCondition object to copy
      */
    CMCondition(const CMCondition* const other);

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

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const override;
#ifdef INCDEBUG
    static void setTop() {CMCondition::_dlevel = Debug::Top;}
#endif
private:
    friend ConditionBase;
    friend class modules::CM;
    /**
      \copydoc ConditionBase::_fromArray
      */
    void _fromArray(const arr &vec, const std::size_t offset = 0) override;

    /**
      \copydoc ConditionBase::_toArray
      */
    arr _toArray() const override;

    /**
      Get the size of the data vector

      \returns The size of the serialized vector.
      */
    static std::size_t _size() {
        if (count == 0)
            count = PS_PRCondition::size() + SUCSCondition::size();
        return count;
    }

    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override;

    /**
      Reset any static data members to their initial state
      */
    static void reset() {
        count = 0;
    }

    static std::size_t count;  // size of the current serialized output
#ifdef INCDEBUG
    static Debug::DebugLevel _dlevel;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
