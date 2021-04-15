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

#include "RA.hpp"
#include "XanCycle.hpp"
#include "driver.hpp"

class trDynaPSCon;
/**
 Class for holding inputs to DynaPS_mb
 */
class DynaPSCon {
public:
    DynaPSCon(trDynaPSCon* par = nullptr) : RA_con(new RACon(this)), XanCycle_con(new XanCycleCon(this)), parent(par) {}
    ~DynaPSCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The DynaPSCon object to copy
      */
    DynaPSCon(const DynaPSCon* other) {
        clear();
        RA_con = other->RA_con;
        XanCycle_con = other->XanCycle_con;
        RA_con->setParent(this);
        XanCycle_con->setParent(this);
    }
    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    DynaPSCon(realtype *x) {
        fromArray(x);
    }
    /**
      Constructor to create an object from the contained classes

      @param rother A RACon object to incorporate
      @param xother A XanCycleCon object to incorporate
      */
    DynaPSCon(RACon* rother, XanCycleCon* xother) {
        clear();
        RA_con = rother;
        XanCycle_con = xother;
        RA_con->setParent(this);
        XanCycle_con->setParent(this);
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    DynaPSCon(const arr &vec, size_t offset = 0){
            fromArray(vec, offset);
    }
    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    void fromArray(realtype *x) {
        arr vec(120);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        if (RA_con == nullptr)
            RA_con = new RACon(this);
        if (XanCycle_con == nullptr)
            XanCycle_con = new XanCycleCon(this);
        RA_con->fromArray(vec, offset);
        XanCycle_con->fromArray(vec, offset + RA_con->size());
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr rvec = RA_con->toArray();
        arr xvec = XanCycle_con->toArray();
        rvec.insert(rvec.end(), xvec.begin(), xvec.end());
        return rvec;
    }
    void setParent(trDynaPSCon* par) {parent = par;}
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return RACon::size() + XanCycleCon::size();
    }
    RACon* RA_con = nullptr;
    XanCycleCon* XanCycle_con = nullptr;

    trDynaPSCon* parent;

private:
    void clear() {
        if (RA_con != nullptr) {
            delete RA_con;
            RA_con = nullptr;
        }
        if (XanCycle_con != nullptr) {
            delete XanCycle_con;
            XanCycle_con = nullptr;
        }
    }
};

/**
  Initialize the variables

  @param theVars The global variables
  @return A DynaPSCon object for input into calculations
  */
DynaPSCon* DynaPS_Init(Variables *theVars);
/**
  Calculate the output values based on the inputs

  @param t The current timestamp
  @param DynaPS_con DynaPSCon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
arr DynaPS_Mb(const double t, const DynaPSCon* DynaPS_con, Variables *theVars);

/**
 Class for running DynaPS through an ODE solver
 */
class DynaPSDrive : public Driver {
public:
    DynaPSDrive(Variables *theVars, const double st, const double stp, const double etime,
                const int maxSteps, const double atol, const double rtol, const size_t para,
                const double ratio) : Driver(theVars, st, stp, etime, maxSteps, atol, rtol) {
        ParaNum = para;
        Ratio = ratio;
    }
    virtual ~DynaPSDrive() override;
    /**
      The driver code
      */
    void setup() override;
    void getResults() override;

private:
    /**
      Calculate the output values based on the inputs

      @param t The time of the current calculation (0 is the beginning of the calculations)
      @param u The input data parameters for the calculations
      @param[in,out] u_dot The dxdt values for the input parameters
      @param[in,out] user_data Pointer to a UserData object for extra parameters
      @return A vector containing the updated values
      */
    arr MB(realtype t, N_Vector u) override;
    /**
      Initialize the variables

      @return A CMCon object for input into calculations
      */
    DynaPSCon* DynaPS_Ini();
    size_t ParaNum;
    double Ratio;
};
