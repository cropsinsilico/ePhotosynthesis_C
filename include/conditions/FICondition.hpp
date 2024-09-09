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
class FI;
}
namespace conditions {

class FIBFCondition;

/**
 Class for holding the inputs to FI_mb
 */
class FICondition : public ConditionBase<FICondition, FIBFCondition, MODULE_FI> {
public:
    FICondition(FIBFCondition* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FICondition object to copy
      */
    FICondition(const FICondition* const other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FICondition(const arr &vec, const std::size_t offset = 0);

    double A = 0.;          ///< The concentration of excitons in the peripheral antenna
    double U = 0.;          ///< The concentration fo excitons in the core antenna
    double P680ePheo = 0.;  ///< QF add
    double P680pPheon = 0.; ///< The concentration for the P680+ Pheo-
    double P680pPheo = 0.;  ///< The concentration of P680+ Pheo
    double P680Pheon = 0.;  ///< The concentration of P680Pheo-
    double Yz = 0.;         ///< The concentration of reduced tyrosine// --unused
    double S1T = 0.;        ///< The concentration of S1 in combination with reduced tyrosine
    double S2T = 0.;        ///< The concentration of S2 in combination with reduced tyrosine
    double S3T = 0.;        ///< The concentration of S3 in combination with reduced tyrosine
    double S0T = 0.;        ///< The concentration of S0 in combination with reduced tyrosine
    double S1Tp = 0.;       ///< The concentration of S1 in combination with oxidized tyrosine
    double S2Tp = 0.;       ///< The concentration of S2 in combination with oxidized tyrosine
    double S3Tp = 0.;       ///< The concentration of S3 in combination with oxidized tyrosine
    double S0Tp = 0.;       ///< The concentration of S0 in combination with oxidized tyrosine
    double QAQB = 0.;       ///< The concentration of [QAQB]
    double QAnQB = 0.;      ///< The concentration of [QA-QB]
    double QAQBn = 0.;      ///< The concentration of [QAQB-]
    double QAnQBn = 0.;     ///< The concentration of [QA-QB-]
    double QAQB2n = 0.;     ///< The concentration of [QAQB2-]
    double QAnQB2n = 0.;    ///< The concentration of [QA-QB2-]
    double PQn = 0.;        ///< The concentration of reduced PQ, i.e. PQH2;

    /**
      Write the contents of the instance to the output stream.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      \returns The output stream
      */
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;

    SET_GET_BOOL(BF_connect, FI_COND_PQn)
private:
    friend ConditionBase;
    friend modules::FI;
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
        if (BF_connect)
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
    static void reset(){
        setBF_connect(false);
    }

    static const std::size_t count;  // size of the current serialized output

#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
