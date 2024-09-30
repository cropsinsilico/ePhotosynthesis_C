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

namespace ePhotosynthesis {
namespace modules {
class FIBF;
}
namespace conditions {

class EPSCondition;

#define MEMBERS_FIBFCondition_CONSTANT EMPTY_MEMBER_LIST
#define MEMBERS_FIBFCondition_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_FIBFCondition_NOT_IN_ARRAY EMPTY_MEMBER_LIST

/**
 Class for input to FIBF_mb
 */
class FIBFCondition : public ConditionBase<FIBFCondition, EPSCondition> {
public:
    FIBFCondition(EPSCondition* par = nullptr) : BF_con(new BFCondition(this)), FI_con(new FICondition(this)) {
        setParent(par);
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

    double kd = 0; // The initialization of the initial rate constant for heat dissipation
    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;

private:
    friend ConditionBase;
    friend class modules::FIBF;
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
            count = BFCondition::size() + FICondition::size() + 1;
        return count;
    }

    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override;

    static std::size_t count;   // size of the current serialized output

    /**
      Reset any static data members to their initial state
      */
    static void reset() {
        count = 0;
    }
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Middle;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
