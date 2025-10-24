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
#include "Variables.hpp"
#include "drivers/CM_Driver.hpp"
#include "modules/CM.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

DEFINE_DRIVER(CM);

CMDriver::CMDriver(Variables *theVars, const double startTime,
		   const double stepSize, const double endTime,
		   const int maxSubsteps,
		   const double atol, const double rtol,
		   const std::size_t para, const double ratio,
		   const bool showWarn,
		   const std::vector<std::string>& outVars) :
    DriverBase(theVars, startTime, stepSize, endTime, maxSubsteps,
	       atol, rtol, para, ratio, showWarn, outVars) {
#ifdef INCDEBUG
    ePhotosynthesis::conditions::CMCondition::setTop();
#endif
    init(theVars->useC3);
    if (outputVars.empty()) {
      outputVars.push_back("Light intensity");
      outputVars.push_back("CO2AR");
    }
}


void CMDriver::setup_connections(Variables* theVars) {
    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    theVars->PR_PS_com = true;
    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

}

void CMDriver::setup_variables(Variables* theVars) {

    theVars->alpha1 = 1.0;
    theVars->alpha2 = 1.0;

}

void CMDriver::setup_param(Variables* theVars) {

    theVars->SUCS_Param[0] = 1;
    theVars->SUCS_Param[1] = 1;

    theVars->PS_PR_Param = 0;

}

void CMDriver::getResults(Variables* theVars) {
    
    if (!theVars) theVars = currentVariables();

    const double CO2AR = TargetFunVal(theVars);

    results = {CO2AR};

}
