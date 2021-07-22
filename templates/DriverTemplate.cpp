#include "drivers/DriverTemplate.hpp"

/** ************************************************************************************************
 * This file provides the template for implementing new Driver methods.
 * Replace DriverTemplate with the name of the new driver and save this file as <new driver_name.cpp>
 * in the source directory.
 ************************************************************************************************* */


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
  Method to initialize variables. Comment out if not needed.
*/
Constraints* Init() {

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
