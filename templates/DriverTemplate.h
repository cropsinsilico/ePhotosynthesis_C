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

#include "driver.hpp"

/**************************************************************************************************
 * This file provides the template for adding new Driver methods.
 * Replace DriverTemplate with the name of the new driver and save this file as <new driver_name.hpp>
 * in the include directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 **************************************************************************************************/
class DriverTemplate : public Driver
{
public:
    /*
      Constructor, additional variables can be added to the current initialization
    */
    DriverTemplate(Variables *theVars, const double st, const double stp, const double etime,
                   const int maxSteps, const double atol, const double rtol) :
        Driver(theVars, st, stp, etime, maxSteps, atol, rtol) {

    }
    // Destructor
    virtual ~DriverTemplate() override;

    /*
       Method to get the inputs for the initial calculation
    */
    void setup() override;

    /*
      Method to get the results and put the in a class variable.
    */
    void getResults() override;

private:
    /*
      Method to do the calculations, this is the method given to the ODE solver
    */
    arr MB(realtype t, N_Vector u) override;
};

