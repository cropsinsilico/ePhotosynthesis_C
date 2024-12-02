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

#include "definitions.hpp"
#include "driver.hpp"
#include "conditions/trDynaPSCondition.hpp"

namespace ePhotosynthesis {
  FORWARD_DECLARE_DRIVER(trDynaPS);
namespace drivers {

/**
 Class for running trDynaPS with an ODE solver
 */
class trDynaPSDriver : public DriverBase<trDynaPSDriver, MODULE_trDynaPS> {
    DECLARE_DRIVER(trDynaPS)
public:
    /** \copydoc drivers::Driver::Driver */
    trDynaPSDriver(Variables *theVars, const double startTime,
		   const double stepSize, const double endTime,
                   const int maxSubsteps,
		   const double atol, const double rtol,
		   const std::size_t para, const double ratio,
		   const bool showWarn = false);

    /**
      \copydoc drivers::Driver::setup
      */
    void setup() override;

    /**
      \copydoc drivers::Driver::getResults
      */
    void getResults() override;

};

}  // namespace drivers
}  // namespace ePhotosynthesis
