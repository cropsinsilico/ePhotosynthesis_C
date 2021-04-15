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

#include "EPS.hpp"
#include "RuACT.hpp"
class DynaPSCon;
/**
 Class for holding input for RA_mb
 */
class RACon {
public:
    RACon(DynaPSCon* par = nullptr) : RuACT_con(new RuACTCon(this)), EPS_con(new EPSCon(this)), parent(par) {}
    ~RACon() {
        if (RuACT_con != nullptr) {
            delete RuACT_con;
            RuACT_con = nullptr;
        }
        if (EPS_con != nullptr) {
            delete EPS_con;
            EPS_con = nullptr;
        }
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RACon object to copy
      */
    RACon(const RACon *other) {
        RuACT_con = other->RuACT_con;
        EPS_con = other->EPS_con;
        RuACT_con->setParent(this);
        EPS_con->setParent(this);
    }
    /**
      Constructor to create an object from the contained classes

      @param eother A EPSCon object to incorporate
      @param rother A RuACTCon object to incorporate
      */
    RACon(EPSCon* eother, RuACTCon* rother) {
        RuACT_con = rother;
        EPS_con = eother;
        RuACT_con->setParent(this);
        EPS_con->setParent(this);
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RACon(const arr &vec, size_t offset = 0){
        fromArray(vec, offset);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        if (EPS_con == nullptr)
            EPS_con = new EPSCon(this);
        if (RuACT_con == nullptr)
            RuACT_con = new RuACTCon(this);
        EPS_con->fromArray(vec, offset);
        RuACT_con->fromArray(vec, offset + EPS_con->size());
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr evec = EPS_con->toArray();
        arr rvec = RuACT_con->toArray();
        evec.reserve(size());
        evec.insert(evec.end(), rvec.begin(), rvec.end());
        return evec;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return EPSCon::size() + RuACTCon::size();
    }

    void setParent(DynaPSCon* par) {parent = par;}
    RuACTCon* RuACT_con = nullptr;
    EPSCon* EPS_con = nullptr;

    DynaPSCon* parent;
};

/**
  Initialize the variables

  @param theVars The global variables
  @return A RACon object for input into calculations
  */
RACon* RA_Ini(Variables *theVars);

/**
  Calculate the output values based on the inputs

  @param RA_Con RACon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
arr RA_Mb(const double t, const RACon* RA_Con, Variables *theVars);
