#pragma once

#include "modules/ModuleBase.hpp"
#include "conditions/DependentCondition.hpp"

namespace ePhotosynthesis {
namespace modules {

/** ************************************************************************************************
 * This file provides the template for adding new module class.
 * Replace Dependent with the name of the new module and save this file as <new_module_name>.hpp
 * in the include/modules directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 * Condition is the condition object which belongs to this module.
 ************************************************************************************************* */

class ParentModule;
class DependentModule : public ModuleBase<DependentModule, conditions::DependentCondition> {
private:
    friend ModuleBase;
    friend class modules::ParentModule;
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A DependentCondition object with values set base on the input
      */
    static DependentCondition* _init(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param Temp_con DependentCondition object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const conditions::DependentCondition* const Dep_con,
                   Variables *theVars);
    static conditions::DependentCondition* _MB_con(const double t,
                                                   const conditions::DependentCondition* const Dep_con,
                                                   Variables *theVars);

    /**
      Calculate the Rates based on the inputs

      @param t The current timestamp
      @param Temp_con DependentCondition object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const conditions::DependentCondition* const Dep_con,
                      Variables *theVars) {}

    static void _reset();
};

}  // namespace modules
}  // namespace ePhotosynthesis
