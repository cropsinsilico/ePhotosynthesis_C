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
#include "../definitions.hpp"
#include "CM_Driver.hpp"
#include "DynaPS_Driver.hpp"
#include "EPS_Driver.hpp"
#include "trDynaPS_Driver.hpp"

#define CASE_DRIVER_METHOD(drv, X)					\
  case drv: {								\
    X(drv);								\
    break;								\
  }
#define SWITCH_DRIVER(drv, X)						\
  switch (drv) {							\
    FOR_EACH_WITH_SUFFIX_ARGS(CASE_DRIVER_METHOD, (X), MEMBERS_DRIVER);	\
  default:								\
    std::cerr << __func__ << ": Invalid driver choice given: \"" <<	\
      drv << "\"" << std::endl;						\
    exit(EXIT_FAILURE);							\
  }

namespace ePhotosynthesis {
namespace drivers {
  
  EPHOTO_API void select_driver(const DriverType& driverChoice,
				const bool useC3, const bool x = true);

  EPHOTO_API Driver* create_driver(const DriverType& driverChoice,
				   Variables *theVars,
				   const double begintime,
				   const double stepsize,
				   const double stoptime,
				   const int maxSubSteps,
				   const double abstol,
				   const double reltol,
				   const std::size_t para = 1,
				   const double ratio = 1,
				   const bool showWarn = false,
                                   const std::vector<std::string>& outVars = {});
  EPHOTO_API Driver* create_driver(const DriverType& driverChoice,
				   Variables *theVars,
				   const DriverParam& param);
}
}
