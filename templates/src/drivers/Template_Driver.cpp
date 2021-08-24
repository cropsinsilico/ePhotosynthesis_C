#include "Variables.hpp"
#include "globals.hpp"
#include "drivers/TemplateDriver.hpp"
#include "modules/TemplateModule.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

/** ************************************************************************************************
 * This file provides the template for implementing new Driver methods.
 * Replace Template_Driver with the name of the new driver and save this file as <new driver_name.cpp>
 * in the source directory.
 ************************************************************************************************* */


TemplateDriver::~TemplateDriver() {
  modules::TemplateModule::_reset();
}

/*
  This method is used to set up the inputs for the first calculation. The internal pointer theVars
  will contain some of the global settings. The internal array constraints will contain the initial
  values for the calculations.
*/
void TemplateDriver::setup() {
    // Do any initialization here

    TemplateCondition* Temp_con = Init();

    constraints = Temp_con->toArray(); // set the initial constraints
    delete Temp_con;
}

void TemplateDriver::getResults() {
    // The internal variables intermediateResults will contain the results from the last calculation run
    // but this may not be the values needed as the ODE may stop one time step short of the needed
    // stoping time
    TemplateCondition* Temp_con = new TemplateCondition(intermediateRes);
    arr tmp = TemplateModule::MB(time, Temp_con, theVars);
    // Any final results must be stored in the results array
}

/*
  Method to initialize variables. Comment out if not needed.
*/
TemplateConditions* Init() {
  return TemplateModule::init(theVars);
}
/*
  This method is the one given to the ODE solver
*/
arr TemplateDriver::MB(realtype t, N_Vector u) {
    // convert the input data into an array
    realtype *x = N_VGetArrayPointer(u);

    // convert the array to an object for input into the calculations
    TemplateCondition* Temp_con = new TemplateCondition(x);

    arr dxdt = TemaplateModule::MB(t, Temp_con, theVars);
    delete Temp_con;

    return dxdt;
}
