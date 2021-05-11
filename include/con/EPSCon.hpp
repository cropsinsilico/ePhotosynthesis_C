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

#include "definitions.hpp"
#include "FIBFCon.hpp"
#include "CMCon.hpp"

class RACon;
/**
 Class for holding the inputs to EPS_mb
 */
class EPSCon {
public:
    EPSCon(RACon* par = nullptr) : CM_con(new CMCon(this)), FIBF_con(new FIBFCon(this)), parent(par) {}

    ~EPSCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The EPSCon object to copy
      */
    EPSCon(const EPSCon* other) {
        clear();
        CM_con = other->CM_con;
        FIBF_con = other->FIBF_con;
        CM_con->setParent(this);
        FIBF_con->setParent(this);
    }

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    EPSCon(realtype *x, const uint adjust = 0) {
        fromArray(x, adjust);
    }
    /**
      Constructor to create an object from the contained classes

      @param fother A FIBFCon object to incorporate
      @param cother A CMCon object to incorporate
      */
    EPSCon(FIBFCon* fother, CMCon* cother) {
        clear();
        CM_con = cother;
        FIBF_con = fother;
        CM_con->setParent(this);
        FIBF_con->setParent(this);
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    EPSCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, const size_t offset = 0) {
        if (FIBF_con == nullptr)
            FIBF_con = new FIBFCon(this);
        if (CM_con == nullptr)
            CM_con = new CMCon(this);
        FIBF_con->fromArray(vec, offset);
        CM_con->fromArray(vec, offset + FIBF_con->size());
    }
    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    void fromArray(realtype *x, const uint adjust = 0) {
        arr vec = zeros(size());
        for (size_t i = 0; i < size() - adjust; i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr fvec = FIBF_con->toArray();
        arr cvec = CM_con->toArray();
        fvec.reserve(size());
        fvec.insert(fvec.end(), cvec.begin(), cvec.end());
        return fvec;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return CMCon::size() + FIBFCon::size();
    }

    void setParent(RACon* par) {parent = par;}
    friend std::ostream& operator<<(std::ostream &out, const EPSCon &in);
    CMCon* CM_con = nullptr;
    FIBFCon* FIBF_con = nullptr;
    RACon* parent;

private:
    void clear() {
        if (CM_con != nullptr) {
            delete CM_con;
            CM_con = nullptr;
        }
        if (FIBF_con != nullptr) {
            delete FIBF_con;
            FIBF_con = nullptr;
        }
    }
};
