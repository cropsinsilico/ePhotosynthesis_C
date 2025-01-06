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

namespace ePhotosynthesis {
namespace modules {
class PS;
}
namespace conditions {

class PS_PRCondition;

/**
 Class for holding the inputs to PS_mb
 */
class PSCondition : public ConditionBase<PSCondition, PS_PRCondition> {
public:
    PSCondition(PS_PRCondition* par = nullptr) {
        setParent(par);
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

    double RuBP = 0.;
    double PGA = 0.;
    double DPGA = 0.;
    double T3P = 0.;
    double ADPG = 0.;
    double FBP = 0.;
    double E4P = 0.;
    double S7P = 0.;
    double SBP = 0.;
    double ATP = 0.;
    double HexP = 0.;
    double PenP = 0.;
    double _Pi = 0.;
    double _ADP = 0.;
    double _v1 = 0.;

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const override;
    SET_GET_BOOL(C3)
private:
    friend ConditionBase;
    friend class modules::PS;
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
        if (C3)
            return count - 1;
        return count;
    }

    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override {}

    /**
      Reset any static data members to their initial state
      */
    static void reset() {
        C3 = false;
    }

    EPHOTO_API static const std::size_t count;  // size of the current serialized output
#ifdef INCDEBUG
    EPHOTO_API const static Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
