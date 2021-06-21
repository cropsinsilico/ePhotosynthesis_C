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
#include "driver.hpp"
#include "containers/EPSContainer.hpp"

/**
 Class for running EPD with an ODE solver
 */
class EPSDriver : public Driver {
public:
    EPSDriver(Variables *theVars, const double st, const double stp, const double etime,
              const int maxSteps, const double atol, const double rtol,
              const size_t para, const double ratio, const double Tp) :
        Driver(theVars, st, stp, etime, maxSteps, atol, rtol) {
        ParaNum = para;
        Ratio = ratio;
        this->Tp = Tp;
    }

    ~EPSDriver() override;

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
    EPSContainer* EPS_Init();

    //double Ca;
    //double Li;
    double AtpCost;
    double SucPath;
    size_t ParaNum;
    double Ratio;
    double Tp;
};
