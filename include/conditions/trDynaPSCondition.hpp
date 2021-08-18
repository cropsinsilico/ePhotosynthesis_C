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

#include "RROEACondition.hpp"
#include "DynaPSCondition.hpp"

namespace ePhotosynthesis {
namespace conditions {

/**
 Class for holding inputs to trDynaPS_mb
 */
class trDynaPSCondition : public ConditionBase<trDynaPSCondition, trDynaPSCondition> {
public:
    trDynaPSCondition() : RROEA_con(new RROEACondition(this)), DynaPS_con(new DynaPSCondition(this)) {}

    ~trDynaPSCondition() {
        _clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The trDynaPSCon object to copy
      */
    trDynaPSCondition(const trDynaPSCondition* other);

    /**
      Constructor to create an object from the contained classes

      @param dother A DynaPSCon object to incorporate
      @param rother A RROEACon object to incorporate
      */
    trDynaPSCondition(DynaPSCondition* dother, RROEACondition* rother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    trDynaPSCondition(const arr &vec, size_t offset = 0);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    trDynaPSCondition(realtype *x);

    RROEACondition* RROEA_con = nullptr;
    DynaPSCondition* DynaPS_con = nullptr;

    void setParent(trDynaPSCondition* par) {
        (void)par;
    }
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;

private:
    friend ConditionBase;
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, size_t offset = 0);

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr _toArray();

    void _clear();

    /**
      Get the size of the data vector
      */
    static size_t _size() {
        if (count == 0)
            count = RROEACondition::size() + DynaPSCondition::size();
        return count;
    }

    static size_t count;
#ifdef INCDEBUG
    const Debug::DebugLevel _dlevel = Debug::Top;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
