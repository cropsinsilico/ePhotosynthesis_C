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
#include "Leaf_C4Condition.hpp"
#include "Enzyme_C4Condition.hpp"
#include "AE_C4Condition.hpp"

namespace ePhotosynthesis {
namespace modules {
class RAC4leafMeta;
}
namespace conditions {

class RAC4leafMetaCondition : public ConditionBase<RAC4leafMetaCondition, RAC4leafMetaCondition> {
public:
    RAC4leafMetaCondition() : Leaf_con(new LeafCondition(this)), Enzyme_con(new EnzymeCondition(this)),
    AE_con(new AECondition(this)) {}
    RAC4leafMetaCondition(const RAC4leafMetaCondition* const other);
    RAC4leafMetaCondition(const arr &vec, const std::size_t offset = 0);
    RAC4leafMetaCondition(LeafCondition* lother, EnzymeCondition* eother, AECondition* aother);
    RAC4leafMetaCondition(realtype *x);
    ~RAC4leafMetaCondition() override {
        _clear();
    }

#ifdef INCDEBUG
    static void setTop() {}
#endif

    LeafCondition* Leaf_con = nullptr;
    EnzymeCondition* Enzyme_con = nullptr;
    AECondition* AE_con = nullptr;

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const override;

private:
    friend ConditionBase;
    friend class modules::RAC4leafMeta;
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
            count = LeafCondition::size() + EnzymeCondition::size() + AECondition::size();
        return count;
    }
    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override;

    /**
      Reset any static data members to their initial state
      */
    static void _reset() {
        LeafCondition::reset();
        EnzymeCondition::reset();
        AECondition::reset();
        count = 0;
    }
    static std::size_t count;
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Top;;
#endif

};

}  // namespace conditions
}  // namespace ePhotosynthesis
