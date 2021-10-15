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

#include "PSCondition.hpp"
#include "PRCondition.hpp"

namespace ePhotosynthesis {
namespace modules {
class PS_PR;
}
namespace conditions {

class CMCondition;
/**
 Class to hold inputs to PS_PR_mb
 */
class PS_PRCondition : public ConditionBase<PS_PRCondition, CMCondition> {
public:
    PS_PRCondition(CMCondition* par = nullptr) : PS_con(new PSCondition(this)), PR_con(new PRCondition(this)) {
        setParent(par);
    }

    ~PS_PRCondition() override {
        _clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PS_PRCon object to copy
      */
    PS_PRCondition(const PS_PRCondition* const other);

    /**
      Constructor to create an object from the contained classes

      @param sother A PSCon object to incorporate
      @param rother A PRCon object to incorporate
      */
    PS_PRCondition(PSCondition* sother, PRCondition* rother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    PS_PRCondition(const arr &vec, const std::size_t offset = 0);

    PSCondition* PS_con = nullptr;   // child Condition
    PRCondition* PR_con = nullptr;   // child Condition

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const override;
private:
    friend ConditionBase;
    friend class modules::PS_PR;
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
            count = PSCondition::size() + PRCondition::size();
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
    const static Debug::DebugLevel _dlevel = Debug::Middle;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
