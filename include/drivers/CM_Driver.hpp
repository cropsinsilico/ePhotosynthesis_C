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
#include "conditions/CMCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class CMDriverTest;
}
#endif
namespace drivers {

/**
 Class for running the CM calculations in an ODE solver
 */
class CMDriver : public Driver {
public:
    /**
      \copydoc drivers::Driver::Driver
      */
    CMDriver(Variables *theVars, const double startTime, const double stepSize, const double endTime,
             const int maxSubsteps, const double atol, const double rtol,
             const bool showWarn = false) :
        Driver(theVars, startTime, stepSize, endTime, maxSubsteps, atol, rtol, showWarn) {
#ifdef INCDEBUG
                ePhotosynthesis::conditions::CMCondition::setTop();
#endif

    }
    ~CMDriver() override;

    /**
      \copydoc drivers::Driver::setup
      */
    void setup() override;

    /**
      \copydoc drivers::Driver::getResults
      */
    void getResults() override;
private:
#ifdef TESTING
    friend class test::CMDriverTest;
#endif

    /**
      Initialize the variables

      @return A CMCondition object for input into calculations
      */
    conditions::CMCondition* CM_Ini();

    /**
      \copydoc drivers::Driver::MB
      */
    arr MB(realtype t, N_Vector u) override;
};

}  // namespace drivers
}  // namespace ePhotosynthesis
