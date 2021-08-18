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
namespace conditions {

class PS_PRCondition;
/**
 Class for holding the inputs to PR_mb
 */
class PRCondition : public ConditionBase<PRCondition, PS_PRCondition> {
public:
    PRCondition(PS_PRCondition* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PRCon object to copy
      */
    PRCondition(const PRCondition* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    PRCondition(const arr vec, size_t offset = 0);


    double GCEA = 0.;
    double GCA = 0.;
    double PGCA = 0.;
    double GCAc = 0.;
    double GOAc = 0.;
    double SERc = 0.;
    double GLYc = 0.;
    double HPRc = 0.;
    double GCEAc = 0.;
    double RuBP = 0.;
    double _v131;
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
    SET_GET_BOOL(PS_connect)
    SET_GET_BOOL(PS_RuBP)
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
    /**
      Get the size of the data vector
      */
    static size_t _size() {
        if (PS_RuBP)
            return count - 1;
        return count;
    }

    void _clear() {}
    static const size_t count;
#ifdef INCDEBUG
    const Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
