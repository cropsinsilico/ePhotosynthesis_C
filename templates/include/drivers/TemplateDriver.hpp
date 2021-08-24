#pragma once

#include "definitions.hpp"
#include "drivers/driver.hpp"
#include "conditions/TemplateCondition.hpp"

/** ************************************************************************************************
 * This file provides the template for adding new Driver methods.
 * Replace Template with the name of the new driver and save this file as <new_driver_name>.hpp
 * in the include/drivers directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 ************************************************************************************************* */
namespace ePhotosynthesis {
namespace drivers {
class TemplateDriver : public Driver {
public:
    /*
      Constructor, additional variables can be added to the current initialization
    */
    TemplateDriver(Variables *theVars, const double st, const double stp, const double etime,
                   const int maxSteps, const double atol, const double rtol,
                   const bool showWarnings = false) :
        Driver(theVars, st, stp, etime, maxSteps, atol, rtol, showWarnings) {
#ifdef INCDEBUG
        ePhotosynthesis::conditions::TemplateCondition::setTop();
#endif
    }
    // Destructor
    ~TemplateDriver() override;

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
    TemplateCondition* Init();
};

}  // namespace drivers
}  // namespace ePhotosynthesis
