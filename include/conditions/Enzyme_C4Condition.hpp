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
class RAC4leafMeta;
}
namespace conditions {
class RAC4leafMetaCondition;

/**
 Class for holding the inputs to BF_mb
 */
class EnzymeCondition : public ConditionBase<EnzymeCondition, RAC4leafMetaCondition> {
public:
    EnzymeCondition(RAC4leafMetaCondition* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCondition object to copy
      */
    EnzymeCondition(const EnzymeCondition* const other);
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    EnzymeCondition(const arr &vec, const std::size_t offset = 0);

    double MC_HCO3 = 0.;
    double MC_OAA = 0.;
    double MC_PEP = 0.;
    double MC_Malate = 0.;
    double MC_Pyruvate = 0.;
    double MC_PGA = 0.;
    double MC_FBP = 0.;
    double MC_UDPG = 0.;
    double MC_SUCP = 0.;
    double MC_SUC = 0.;
    double MC_F26BP = 0.;
    double MC_ATP = 0.;
    double MC_T3P = 0.;
    double MC_HexP = 0.;
    double MC_Sucrose = 0.;
    double Mchl_OAA = 0.;
    double Mchl_Malate = 0.;
    double Mchl_PEP = 0.;
    double Mchl_Pyruvate = 0.;
    double Mchl_NADPH = 0.;
    double Mchl_ATP = 0.;
    double Mchl_PGA = 0.;
    double Mchl_DPGA = 0.;
    double Mchl_T3P = 0.;
    double BSC_T3P = 0.;
    double BSC_PGA = 0.;
    double BSC_Malate = 0.;
    double BSC_Pyruvate = 0.;
    double BSC_CO2 = 0.;
    double Bchl_CO2 = 0.;
    double Bchl_RuBP = 0.;
    double Bchl_PGA = 0.;
    double Bchl_DPGA = 0.;
    double Bchl_ATP = 0.;
    double Bchl_NADPH = 0.;
    double Bchl_SBP = 0.;
    double Bchl_S7P = 0.;
    double Bchl_FBP = 0.;
    double Bchl_E4P = 0.;
    double Bchl_Starch = 0.;
    double Bchl_Rubisco = 0.;
    double Bchl_T3P = 0.;
    double Bchl_HexP = 0.;
    double Bchl_Pent = 0.;
    double Bchl_Malate = 0.;
    double Bchl_Pyruvate = 0.;
    double Bchl_PGCA = 0.;
    double Bchl_GCA = 0.;
    double Bchl_GCEA = 0.;
    double Bper_GCA = 0.;
    double Bper_GOA = 0.;
    double Bper_GLY = 0.;
    double Bper_SER = 0.;
    double Bper_HPR = 0.;
    double Bper_GCEA = 0.;
    double MC_CO2 = 0.;
    double Bchl_PPi = 0.;
    double Bchl_ADPG = 0.;

    double MC_Glu = 0.;
    double MC_OxoG = 0.;
    double MC_Asp = 0.;
    double MC_Ala = 0.;

    double BSC_OxoG = 0.;
    double BSC_Glu = 0.;
    double BSC_Asp = 0.;
    double BSC_Ala = 0.;
    double BSC_OAA = 0.;
    double BSC_PEP = 0.;
    double BSC_ATP = 0.;
    double Bchl_OAA = 0.;
    double MC_O2 = 0.;
    double Mchl_O2 = 0.;
    double BSC_O2 = 0.;
    double Bchl_O2 = 0.;
    double Bchl_PEP = 0.; //WY PPDK in BS
    double Mchl_GCEA = 0.;
    double Bmito_OAA = 0.;
    double Bmito_MAL = 0.;
    double Bmito_PYR = 0.;
    double Bmito_CO2 = 0.;
    double Bmito_NADH = 0.;
    double Bchl_Asp = 0.;
    double Bchl_Ala = 0.;
    double Mchl_Asp = 0.;
    double Mchl_Ala = 0.;
    double E_PPDK_Mchl = 0.;
    double EP_PPDK_Mchl = 0.;

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
        return count;
    }

    /**
      \copydoc ConditionBase::_clear
      */
    void _clear() override {}

    /**
      Reset any static data members to their initial state
      */
    static void _reset() {}
    static const std::size_t count;  // The Maximum size of the serialized vector.
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
