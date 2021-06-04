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

#include "RACon.hpp"
#include "XanCycleCon.hpp"
#include <sundials/sundials_types.h>

class trDynaPSCon;
/**
 Class for holding inputs to DynaPS_mb
 */
class DynaPSCon : public ConBase<DynaPSCon> {
public:
    DynaPSCon(trDynaPSCon* par = nullptr) : RA_con(new RACon(this)), XanCycle_con(new XanCycleCon(this)), parent(par) {}

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The DynaPSCon object to copy
      */
    DynaPSCon(const DynaPSCon* other);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    DynaPSCon(realtype *x);

    /**
      Constructor to create an object from the contained classes

      @param rother A RACon object to incorporate
      @param xother A XanCycleCon object to incorporate
      */
    DynaPSCon(RACon* rother, XanCycleCon* xother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    DynaPSCon(const arr &vec, size_t offset = 0);

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

    void setParent(trDynaPSCon* par) {parent = par;}
    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return 120;
//return RACon::size() + XanCycleCon::size();
    }

    RACon* RA_con = nullptr;
    XanCycleCon* XanCycle_con = nullptr;

    trDynaPSCon* parent;

    void _clear();
};
