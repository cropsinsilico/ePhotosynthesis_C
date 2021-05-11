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

#include "definitions.hpp"

/**
 Class for holding the inputs to RedoxReg_mb
 */
class RedoxRegCon {
public:
    RedoxRegCon() : RA_con(new RACon()) {}

    ~RedoxRegCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RedoxRegCon object to copy
      */
    RedoxRegCon(const RedoxRegCon* other) {
        clear();
        RA_con = other->RA_con;
        Thion = other->Thion;
        //RA_con.setParent(this);
    }
    /**
      Constructor to create an object from the contained classes

      @param rother A RACon object to incorporate
      @param thio
      */
    RedoxRegCon(RACon* rother, double thio = 0.) {
        clear();
        RA_con = rother;
        Thion = thio;
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RedoxRegCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        if (RA_con == nullptr)
            RA_con = new RACon();
        RA_con->fromArray(vec, offset);
        Thion = vec[offset + 92];
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr toArray() {
        arr rvec = RA_con->toArray();
        rvec.push_back(Thion);
        return rvec;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return RACon::size() + 1;
    }
    RACon* RA_con = nullptr;
    double Thion = 0;

private:
    void clear() {
        if (RA_con != nullptr) {
            delete RA_con;
            RA_con = nullptr;
        }
    }
};
