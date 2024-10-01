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

#include "ModuleBase.hpp"
#include "conditions/RuACTCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class RuACTModuleTest;
}
#endif
namespace modules {
class RA;

/**
 Class for RuACT related functions
 */
class RuACT : public MODULE_BASE(RuACT) {
public:
    DECLARE_MODULE(RuACT)
    SET_GET_BOOL_MODULE(EPS_connect, conditions::RuACT)
private:
    friend ModuleBase;
    friend class modules::RA;
#ifdef TESTING
    friend class test::RuACTModuleTest;
#endif
    /**
      Function to set the initial state of the RuACTCondition class.

      \param theVars Pointer to the global variables
      \return A RuACTCondition object with values set based on the input
      */
    static conditions::RuACTCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RuACTCondition at the given time stamp.

      \param t The current timestamp
      \param RuACT_Con RuACTCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::RuACTCondition* const RuACT_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RuACTCondition at the given time stamp.

      \param t The current timestamp
      \param RuACT_Con RuACTCondition object giving the input parameters
      \param theVars The global variables
      \return A RuACTCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::RuACTCondition* _MB_con(const double t,
                                               const conditions::RuACTCondition* const RuACT_Con,
                                               Variables *theVars);
    /**
      Calculate the Rates of RuACT based on the input RuACTCondition.

      \param t The current timestamp
      \param RuACT_Con RuACTCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::RuACTCondition* const RuACT_Con,
                      Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        activase = 0.;
        TIME = 0.;
        N = 1;
        conditions::RuACTCondition::reset();
    }
    SET_GET(activase)
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the RuACT TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(RuACT);
  
}  // namespace modules
}  // namespace ePhotosynthesis
