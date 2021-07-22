#pragma once

#include "drivers/driver.hpp"
#include "containers/ContainerTemplate.hpp"

/** ************************************************************************************************
 * This file provides the template for adding new Driver methods.
 * Replace DriverTemplate with the name of the new driver and save this file as <new_driver_name>.hpp
 * in the include/drivers directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 ************************************************************************************************* */
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
    ~DriverTemplate() override;

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

    /*
      Most drivers call a function to initialize the variables. If none is needed
      then remove the below signature. Otherwise rename the function as needed.
    */
    ContainerTemplate* Init();
};

