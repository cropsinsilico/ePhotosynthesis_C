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
#include "FI.hpp"
#include "BF.hpp"

class EPSCon;
/**
 Class for FIBF_Pool data
 */
class FIBFPool {
public:
  FIBFPool() {}

  /**
    Copy constructor that makes a deep copy of the given object

    @param other The FIBFPool object to copy
    */
  FIBFPool(const FIBFPool &other) {
      PQT = other.PQT;
  }
  double PQT;
};

/**
 Class for input to FIBF_mb
 */
class FIBFCon {
public:
    FIBFCon(EPSCon* par = nullptr) : BF_con(new BFCon(this)), FI_con(new FICon(this)), parent(par) {}
    ~FIBFCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIBFCon object to copy
      */
    FIBFCon(const FIBFCon* other) {
        clear();
        BF_con = other->BF_con;
        FI_con = other->FI_con;
        BF_con->setParent(this);
        FI_con->setParent(this);
        kd = other->kd;
    }
    /**
      Constructor to create an object from the contained classes

      @param bother A BFCon object to incorporate
      @param fother A FICon object to incorporate
      */
    FIBFCon(BFCon* bother, FICon* fother) {
        clear();
        BF_con = bother;
        FI_con = fother;
        BF_con->setParent(this);
        FI_con->setParent(this);
        kd = pow(10, 8) * 0.5;
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FIBFCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, const size_t offset = 0) {
        if (BF_con == nullptr)
            BF_con = new BFCon(this);
        if (FI_con == nullptr)
            FI_con = new FICon(this);
        BF_con->fromArray(vec, offset);
        FI_con->fromArray(vec, offset + BFCon::size());
        kd = vec[offset + BFCon::size() + FICon::size()];
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr toArray() {
        arr bvec = BF_con->toArray();
        arr fvec = FI_con->toArray();
        bvec.reserve(size());
        bvec.insert(bvec.end(), fvec.begin(), fvec.end());
        arr fivec = {kd};
        bvec.insert(bvec.end(), fivec.begin(), fivec.end());
        return bvec;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return BFCon::size() + FICon::size() + 1;
    }
    friend std::ostream& operator<<(std::ostream &out, const FIBFCon &in);
    void setParent(EPSCon* par) {parent = par;}
    BFCon* BF_con = nullptr;
    FICon* FI_con = nullptr;
    double kd = 0; // The initialization of the initial rate constant for heat dissipation

    EPSCon* parent;

private:
    void clear() {
        if (BF_con != nullptr){
            delete BF_con;
            BF_con = nullptr;
        }
        if (FI_con != nullptr) {
            delete FI_con;
            FI_con = nullptr;
        }
    }

};

/**
  Initialize the variables

  @param theVars The global variables
  */
void FIBF_Ini(Variables *theVars);

/**
  Calculate the output values based on the inputs

  @param t The current timestamp
  @param FIBF_Con FIBFCon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
arr FIBF_Mb(const double t, const FIBFCon* FIBF_Con, Variables *theVars);
