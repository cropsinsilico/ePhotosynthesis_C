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

#include "PS_PR.hpp"
#include "SUCS.hpp"
#include "driver.hpp"

class EPSCon;
/**
 Class for holding the inputs to CM_mb
 */
class CMCon {
public:
    CMCon(EPSCon* par = nullptr) : PS_PR_con(new PS_PRCon(this)), SUCS_con(new SUCSCon(this)), parent(par) {}

    ~CMCon() {
        clear();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The CMCon object to copy
      */
    CMCon(const CMCon* other) {
        clear();
        PS_PR_con = other->PS_PR_con;
        SUCS_con = other->SUCS_con;
        PS_PR_con->setParent(this);
        SUCS_con->setParent(this);
    }

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    CMCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    CMCon(realtype *x) {
        fromArray(x);
    }
    /**
      Constructor to create an object from the contained classes

      @param pother A PS_PRCon object to incorporate
      @param sother A SUCSCon object to incorporate
      */
    CMCon(PS_PRCon* pother, SUCSCon* sother) {
        clear();
        PS_PR_con = pother;
        SUCS_con = sother;
        PS_PR_con->setParent(this);
        SUCS_con->setParent(this);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        if (PS_PR_con == nullptr)
            PS_PR_con = new PS_PRCon(this);
        if (SUCS_con == nullptr)
            SUCS_con = new SUCSCon(this);
        arr pvec(PS_PRCon::size());
        std::copy(vec.begin() + offset, vec.begin() + PS_PRCon::size() + offset, pvec.begin());
        pvec[23] = vec[35 + offset];

        PS_PR_con->fromArray(pvec);
        SUCS_con->fromArray(vec, offset + PS_PRCon::size() - 1);
    }
    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    void fromArray(realtype *x) {
        arr vec(36);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr psprvec = PS_PR_con->toArray();
        arr svec = SUCS_con->toArray();
        psprvec.reserve(size());
        double temp = psprvec[23];
        psprvec.insert(psprvec.end() - 1, svec.begin(), svec.end());
        psprvec[35] = temp;

        return psprvec;
    }

    /**
      Get the size of the data vector
      */
    static size_t size() {
        return PS_PRCon::size() + SUCSCon::size();
    }

    void setParent(EPSCon* par) {parent = par;}

    friend std::ostream& operator<<(std::ostream &out, const CMCon &in);
    PS_PRCon* PS_PR_con = nullptr;
    SUCSCon* SUCS_con = nullptr;

    EPSCon* parent;
private:
    void clear() {
        if (PS_PR_con != nullptr) {
            delete PS_PR_con;
            PS_PR_con = nullptr;
        }
        if (SUCS_con != nullptr) {
            delete SUCS_con;
            SUCS_con = nullptr;
        }
    }
};

/**
 Class for running the CM calculations in an ODE solver
 */
class CMDriver : public Driver {
public:
    CMDriver(Variables *theVars, const double st, const double stp, const double etime,
             const int maxSteps, const double atol, const double rtol) :
        Driver(theVars, st, stp, etime, maxSteps, atol, rtol) {
    }
    virtual ~CMDriver() override;
    /**
      The driver code
      */
    void setup() override;
    void getResults() override;
private:
    /**
      Initialize the variables

      @return A CMCon object for input into calculations
      */
    CMCon* CM_Ini();
    /**
      Calculate the output values based on the inputs

      @param t The time of the current calculation (0 is the beginning of the calculations)
      @param u The input data parameters for the calculations
      @param[in,out] u_dot The dxdt values for the input parameters
      @param[in,out] user_data Pointer to a UserData object for extra parameters
      @return A vector containing the updated values
      */
    arr MB(realtype t, N_Vector u) override;
};

/**
  Initialize the variables

  @param theVars The global variables
  @return A CMCon object for input into calculations
  */
CMCon* CMInit(Variables *theVars);
/**
  Calculate the output values based on the inputs

  @param t The current timestamp
  @param CM_con CMCon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
arr CM_Mb(const double t, const CMCon* CM_con, Variables *theVars);
