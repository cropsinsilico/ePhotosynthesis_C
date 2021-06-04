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
#include <math.h>
#include "definitions.hpp"

#include "FICon.hpp"
#include "BFCon.hpp"

class EPSCon;

/**
 Class for input to FIBF_mb
 */
class FIBFCon : public ConBase<FIBFCon> {
public:
    FIBFCon(EPSCon* par = nullptr) : BF_con(new BFCon(this)), FI_con(new FICon(this)), parent(par) {}
    //~FIBFCon() {
    //    clear();
    //}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIBFCon object to copy
      */
    FIBFCon(const FIBFCon* other);

    /**
      Constructor to create an object from the contained classes

      @param bother A BFCon object to incorporate
      @param fother A FICon object to incorporate
      */
    FIBFCon(BFCon* bother, FICon* fother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FIBFCon(const arr &vec, const size_t offset = 0);

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
      Get the size of the data vector
      */
    static size_t _size() {
        return BFCon::size() + FICon::size() + 1;
    }
    void _clear();

    friend std::ostream& operator<<(std::ostream &out, const FIBFCon &in);
    void setParent(EPSCon* par) {parent = par;}
    BFCon* BF_con = nullptr;
    FICon* FI_con = nullptr;
    double kd = 0; // The initialization of the initial rate constant for heat dissipation

    EPSCon* parent;

};

