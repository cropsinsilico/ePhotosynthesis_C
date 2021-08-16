#pragma once

#include "modules/ModuleBase.hpp"

/** ************************************************************************************************
 * This file provides the template for adding new module class.
 * Replace ContainerTemplate with the name of the new module and save this file as <new_module_name>.hpp
 * in the include/modules directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 * Container is the container object which belongs to this module.
 ************************************************************************************************* */

class ModuleTemplate : public ModuleBase<ModuleTemplate, Container> {
private:
    friend ModuleBase;
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A BFCon object with values set base on the input
      */
    static Container* _init(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param BF_con BFCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const Container* BF_con, Variables *theVars);

    /**
      Calculate the Rates of BF based on the inputs

      @param t The current timestamp
      @param BF_con BFCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const Container* BF_con, Variables *theVars);

};
