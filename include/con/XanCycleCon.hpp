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

#include "ConBase.hpp"

class DynaPSCon;

/**
 Class to hold the inputs to XanCycle_mb
 */
class XanCycleCon : public ConBase<XanCycleCon> {
public:
    XanCycleCon(DynaPSCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The XanCycleCon object to copy
      */
    XanCycleCon(const XanCycleCon* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    XanCycleCon(const arr &vec, const size_t offset = 0);

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, const size_t offset = 0);

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    /**
      Set all data memebers to 0.
      */
    void _clear();

    void setParent(DynaPSCon* par) {parent = par;}
    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return count;
    }
    double Vx = 0.;  ///< The concentration of Violozanthin
    double Ax = 0.;  ///< The concentration of Anthrozanthin
    double Zx = 0.;  ///< The concentration of Zeaznthin
    double ABA = 0.; ///< The concentration of ABA
    DynaPSCon* parent;
private:
    static const size_t count;
};
