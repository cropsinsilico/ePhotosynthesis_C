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
#include "RACondition.hpp"

namespace ePhotosynthesis {
namespace modules {
class RedoxReg;
}
namespace conditions {

/**
 Class for holding the inputs to RedoxReg_mb
 */
class RedoxRegCondition : public ConditionBase<RedoxRegCondition, RedoxRegCondition> {
public:
    RedoxRegCondition() : RA_con(new RACondition()) {}

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RedoxRegCon object to copy
      */
    RedoxRegCondition(const RedoxRegCondition* const other);

    /**
      Constructor to create an object from the contained classes

      @param rother A RACon object to incorporate
      @param thio
      */
    RedoxRegCondition(RACondition* rother, double thio = 0.);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RedoxRegCondition(const arr &vec, const std::size_t offset = 0);

    void setParent(RedoxRegCondition* par) {(void)par;}
    RACondition* RA_con = nullptr;
    double Thion = 0;
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
    SET_GET(V6)
    SET_GET(V9)
    SET_GET(V13)
    SET_GET(V16)
private:
    friend ConditionBase;
    friend class modules::RedoxReg;
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, const std::size_t offset = 0) override;

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr _toArray() const override;

    void _clear() override;

    /**
      Get the size of the data vector
      */
    static std::size_t _size() {
        return RACondition::size() + 1;
    }
    static void reset() {
        V6 = 0.;
        V9 = 0.;
        V13 = 0.;
        V16 = 0.;
    }
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Middle;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
