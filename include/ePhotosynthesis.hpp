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

#include "globals.hpp"
#include "drivers/drivers.hpp"
#include "Variables.hpp"

namespace ePhotosynthesis {

  /**
     @brief Run a simulation
     @param[in,out] theVars Variables instance containing parameters
         for the simulation. The results of the simulation will also be
         stored here at the end of the simulation.
     @param[in] driverChoice Driver that should be used to run the
         simulation. Valid values are:
             1 - trDynaPS (default): PS, PR, FI, BF, SUCS,
                        RuACT, XanCycle, RROEA
             2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle
             3 - CM: PS, PR, SUCS
             4 - EPS: PS, PR, FI, BF, SUCS
     @param[in] begintime The starting time for the calculations.
     @param[in] stoptime The time to stop calculations.
     @param[in] stepsize The step size to use in the calculations.
     @param[in] maxSubSteps The maximum number of iterations at each
         time step.
     @param[in] abstol Absolute tolerance for calculations.
     @param[in] reltol Relative tolerance for calculations.
     @param[in] outputFile Full path to the text file where outputs
         should be saved.
     @param[in] outputVars Vector of the names of variables that should
         be output to outputFile at the end of a simulation. Variables
         for modules, equilibirium constants, etc. can be specified by
         including the C++ scope of the variable. (e.g. MOD:BF:cATPsyn
         would output the concentration of ATP synthase). If not provided
         the output variables will be determined by the driver.
     @param[in] outputParam Control for how often model parameters should
         be output during the course of the simulation. Choices are:
             0 - (default) Don't output parameters during the run.
             1 - Output the initial parameter values.
             2 - Output the initial and final parameter values.
             3 - Output the parameter values for each step in addition
                 to the initial and final values.
     @param[in] outputParamBase Base name for files parameter values
         should be output to at times through the run as specified by
         outputParam.
     @param[in] outputParamVars Vector of the names of variables that
         should be output at the times during the run indicated by
         outputParam. If not provided, all variables will be output.
     @returns Map of variables names and values at the end of the simulation.
   */
  EPHOTO_API std::map<std::string, double>
  run_simulation(Variables*& theVars,
                 const DriverType driverChoice,
                 const double& begintime = 0.0,
                 const double& stoptime = 5000.0,
                 const double& stepsize = 1.0,
                 const int& maxSubSteps = 750,
                 const double& abstol=1e-5,
                 const double& reltol=1e-4,
                 const std::string& outputFile="",
                 const std::vector<std::string>& outputVars={},
                 int outputParam=0,
                 const std::string& outputParamBase="",
                 const std::vector<std::string>& outputParamVars={});

  /**
     @brief Run a simulation
     @param[in] driverChoice Driver that should be used to run the
         simulation. Valid values are:
             1 - trDynaPS (default): PS, PR, FI, BF, SUCS, RuACT,
                     XanCycle, RROEA
             2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle
             3 - CM: PS, PR, SUCS
             4 - EPS: PS, PR, FI, BF, SUCS
     @param[in] inputs Map between variable names and values that they
         should be set to.
     @param[in] begintime The starting time for the calculations.
     @param[in] stoptime The time to stop calculations.
     @param[in] stepsize The step size to use in the calculations.
     @param[in] maxSubSteps The maximum number of iterations at each
         time step.
     @param[in] abstol Absolute tolerance for calculations.
     @param[in] reltol Relative tolerance for calculations.
     @param[in] useC3 Use the C3 model, automatically set to true for
         the EPS driver.
     @param[in] evnFile The full path to the file containing environmental
         parameters.
     @param[in] atpcostFile The full path to the file containing the ATP
         cost.
     @param[in] enzymeFile The full path to the file containing enzyme
         activities.
     @param[in] grnFile The full path to the file containing transcription
         factor ratios for relevant enzymes.
     @param[in] outputFile Full path to the text file where outputs
         should be saved.
     @param[in] outputVars Vector of the names of variables that should
         be output to outputFile at the end of a simulation. Variables
         for modules, equilibirium constants, etc. can be specified by
         including the C++ scope of the variable. (e.g. MOD:BF:cATPsyn
         would output the concentration of ATP synthase). If not provided
         the output variables will be determined by the driver.
     @param[in] outputParam Control for how often model parameters should
         be output during the course of the simulation. Choices are:
             0 - (default) Don't output parameters during the run.
             1 - Output the initial parameter values.
             2 - Output the initial and final parameter values.
             3 - Output the parameter values for each step in addition
                 to the initial and final values.
     @param[in] outputParamBase Base name for files parameter values
         should be output to at times through the run as specified by
         outputParam.
     @param[in] outputParamVars Vector of the names of variables that
         should be output at the times during the run indicated by
         outputParam. If not provided, all variables will be output.
     @param[in] dbglvl Debug level.
     @param[in] debugDelta Debug deltas.
     @param[in] debugInternal Debug internals.
     @param[in] record Record output values for all steps (this can
         significantly slow the program).
     @param[in] theVars Variables instance containing some parameters
         for the simulation. It will be updated with variables passed
         as arguments and read from provided input files. This instance
         will be deleted at the end of the simulation.
     @returns Map of variables names and values at the end of the simulation.
   */
  EPHOTO_API std::map<std::string, double>
  run_simulation(DriverType driverChoice,
                 const double& begintime = 0.0,
                 const double& stoptime = 5000.0,
                 const double& stepsize = 1.0,
                 const int& maxSubSteps = 750,
                 const double& abstol=1e-5,
                 const double& reltol=1e-4,
                 std::map<std::string, std::string> inputs = {},
                 bool useC3=false,
                 const std::string& evnFile="",
                 const std::string& atpcostFile="",
                 const std::string& enzymeFile="",
                 const std::string& grnFile="",
                 const std::string& outputFile="",
                 const std::vector<std::string>& outputVars={},
                 const int& outputParam=0,
                 const std::string& outputParamBase="",
                 const std::vector<std::string>& outputParamVars={},
                 ushort dbglvl=0,
                 const bool debugDelta=false,
                 const bool debugInternal=false,
                 const bool& record=false,
                 Variables *theVars=nullptr);
  
}
