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
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */

#include "PS_PRCon.hpp"
#include "SUCSCon.hpp"

class EPSCon;
/**
 Class for holding the inputs to CM_mb
 */
class CMCon : public ConBase<CMCon> {
public:
    CMCon(EPSCon* par = nullptr) : PS_PR_con(new PS_PRCon(this)), SUCS_con(new SUCSCon(this)), parent(par) {}

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The CMCon object to copy
      */
    CMCon(const CMCon* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    CMCon(const arr &vec, size_t offset = 0);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    CMCon(realtype *x);

    /**
      Constructor to create an object from the contained classes

      @param pother A PS_PRCon object to incorporate
      @param sother A SUCSCon object to incorporate
      */
    CMCon(PS_PRCon* pother, SUCSCon* sother);

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, size_t offset = 0);

    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    //void fromArray(realtype *x) {
    //    arr vec(36);
    //    for (size_t i = 0; i < size(); i++)
    //        vec[i] = x[i];
    //    fromArray(vec);
    //}
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return PS_PRCon::size() + SUCSCon::size();
    }

    void setParent(EPSCon* par) {parent = par;}

    friend std::ostream& operator<<(std::ostream &out, const CMCon &in);
    PS_PRCon* PS_PR_con = nullptr;
    SUCSCon* SUCS_con = nullptr;

    EPSCon* parent;

    void _clear();
};

