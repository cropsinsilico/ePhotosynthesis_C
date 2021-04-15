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

#include "RROEA.hpp"
#include "DynaPS.hpp"
#include "driver.hpp"

/**
 Class for holding inputs to trDynaPS_mb
 */
class trDynaPSCon {
public:
    trDynaPSCon() : RROEA_con(new RROEACon(this)), DynaPS_con(new DynaPSCon(this)) {}
    ~trDynaPSCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The trDynaPSCon object to copy
      */
    trDynaPSCon(const trDynaPSCon* other) {
        clear();
        RROEA_con = other->RROEA_con;
        DynaPS_con = other->DynaPS_con;
        RROEA_con->setParent(this);
        DynaPS_con->setParent(this);
    }
    /**
      Constructor to create an object from the contained classes

      @param dother A DynaPSCon object to incorporate
      @param rother A RROEACon object to incorporate
      */
    trDynaPSCon(DynaPSCon* dother, RROEACon* rother) {
        clear();
        DynaPS_con = dother;
        RROEA_con = rother;
        RROEA_con->setParent(this);
        DynaPS_con->setParent(this);
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    trDynaPSCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    trDynaPSCon(realtype *x) {
        fromArray(x);
    }
    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    void fromArray(realtype *x) {
        arr vec(size());
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
        if (DynaPS_con == nullptr)
            DynaPS_con = new DynaPSCon(this);
        DynaPS_con->fromArray(vec, offset);
        if (RROEA_con == nullptr)
            RROEA_con = new RROEACon(this);
        RROEA_con->fromArray(vec, offset + 110);
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr toArray() {
        arr dyvec = DynaPS_con->toArray();
        arr rrvec = RROEA_con->toArray();
        arr vec = zeros(120);
        for (size_t i = 0; i < 96; i++)
            vec[i] = dyvec[i];
        for (size_t i = 0; i < 10; i++)
            vec[i+110] = rrvec[i];
        return vec;

    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return 120; //RROEA_con.size() + DynaPS_con.size();
    }
    RROEACon* RROEA_con = nullptr;
    DynaPSCon* DynaPS_con = nullptr;

private:
    void clear() {
        if (RROEA_con != nullptr) {
            delete RROEA_con;
            RROEA_con = nullptr;
        }
        if (DynaPS_con != nullptr) {
            delete DynaPS_con;
            DynaPS_con = nullptr;
        }
    }
};

/**
  Initialize the variables

  @param theVars The global variables
  @return A trDynaPSCon object for input into calculations
  */
trDynaPSCon* trDynaPS_Init(Variables *theVars);

/**
  Calculate the output values based on the inputs

  @param trDynaPS_con trDynaPSCon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
arr trDynaPS_Mb(const double t, const trDynaPSCon* trDynaPS_con, Variables *theVars);

/**
 Class for running trDynaPS with an ODE solver
 */
class trDynaPSDriver : public Driver {
public:
    /**
      Constructor for the class.

      @param[in,out] theVars The global variables
      @param start The starting time for the calculations
      @param step The step size for the calculations
      @param endtime The ending time for the calculations
      */
    trDynaPSDriver(Variables *theVars, const double st, const double stp, const double etime,
                   const int maxSteps, const double atol, const double rtol, const size_t para,
                   const double ratio) :
        Driver(theVars, st, stp, etime, maxSteps, atol, rtol) {
        ParaNum = para;
        Ratio = ratio;
    }

    virtual ~trDynaPSDriver() override;

    /**
      The driver
      */
    void setup() override;
    void getResults() override;
    //arr trDynaPS_Drive(size_t ParaNum, double Ratio);

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

      @return A trDynaPSCon object for input into calculations
      */
    trDynaPSCon* trDynaPS_Ini();

    size_t ParaNum;
    double Ratio;
};
