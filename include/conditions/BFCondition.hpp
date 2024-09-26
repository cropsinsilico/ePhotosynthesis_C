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

namespace ePhotosynthesis {
namespace modules {
class BF;
}
namespace conditions {
class FIBFCondition;

#define MEMBERS_BFCondition_NOT_IN_ARRAY EMPTY_MEMBER_LIST
#define MEMBERS_BFCondition_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_BFCondition_CONSTANT EMPTY_MEMBER_LIST

  
/**
 Class for holding the inputs to BF_mb
 */
class BFCondition : public ConditionBase<BFCondition, FIBFCondition, MODULE_BF> {
public:
    DECLARE_VALUE_SET(BFCondition, ConditionBase<BFCondition, FIBFCondition, MODULE_BF>)
    BFCondition(FIBFCondition* par = nullptr) {
        setParent(par);
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCondition object to copy
      */
    BFCondition(const BFCondition* const other);
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    BFCondition(const arr &vec, const std::size_t offset = 0);

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
    SET_GET_BOOL(PS_connect, ATP)
    SET_GET_BOOL(RROEA_connect, Fdn)
    SET_GET_BOOL(FI_connect, Q)

private:
    friend ConditionBase;
    friend class modules::BF;
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
        std::size_t c = count;
        if (FI_connect)
            c--;
        if (PS_connect)
            c--;
        if (RROEA_connect)
            c--;
        return c;
    }

    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override {}

    /**
      Reset any static data members to their initial state
      */
    static void reset() {
      setPS_connect(false);
      setRROEA_connect(false);
      setFI_connect(false);
    }
    static const std::size_t count;  // The Maximum size of the serialized vector.
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

  DEFINE_VALUE_SET_HEADER(BFCondition);

}  // namespace conditions
}  // namespace ePhotosynthesis
