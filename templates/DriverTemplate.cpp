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

#include "DriverTemplate.h"

/**************************************************************************************************
 * This file provides the template for implementing new Driver methods.
 * Replace DriverTemplate with the name of the new driver and save this file as <new driver_name.cpp>
 * in the source directory.
 **************************************************************************************************/


DriverTemplate::~DriverTemplate() {}

/*
  This method is used to set up the inputs for the first calculation. The internal pointer theVars
  will contain some of the global settings. The internal array constraints will contain the initial
  values for the calculations.
*/
void DriverTemplate::setup() {
    constraints = zeros(0); // set the size of constraints to the size of the input data array
    // then populate the constraints
}

void DriverTemplate::getResults() {
    // The internal variables intermediateResults will contain the results from the last calculation run
    // but this may not be the values needed as the ODE may stop one time step short of the needed
    // stoping time

    // Any final results must be stored in the results array
}

/*
  This method is the one given to the ODE solver
*/
arr DriverTemplate::MB(realtype t, N_Vector u) {
    // convert the input data into an array
    realtype *x = N_VGetArrayPointer(u);

    // convert the array to an object for input into the calculations
    // Con con(x);
    arr dxdt; // = whatever code is being called, typically the function has the signature (t, con, theVars)

    return dxdt;
}
