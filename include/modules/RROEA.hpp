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
#include "conditions/RROEACondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class RROEAModuleTest;
}
#endif
namespace modules {
class trDynaPS;

#define MEMBERS_RROEA_CONSTANT				\
  SC,							\
    fc16,						\
    FC
#define MEMBERS_RROEA_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_RROEA_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for RROEA related functions
 */
class RROEA : public ModuleBase<RROEA, conditions::RROEACondition, MODULE_RROEA> {
public:
    DECLARE_VALUE_SET_STATIC(RROEA, ModuleBase<RROEA, conditions::RROEACondition, MODULE_RROEA>)
private:
    friend ModuleBase;
    friend class modules::trDynaPS;
#ifdef TESTING
    friend class test::RROEAModuleTest;
#endif
    /**
      Function to set the initial state of the RROEACondition class.

      \param theVars Pointer to the global variables
      \return A RROEACondition object with values set based on the input
      */
    static conditions::RROEACondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RROEACondition at the given time stamp.

      \param t The current timestamp
      \param RROEA_Con RROEACondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::RROEACondition* const RROEA_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RROEACondition at the given time stamp.

      \param t The current timestamp
      \param RROEA_Con RROEACondition object giving the input parameters
      \param theVars The global variables
      \return A RROEACondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::RROEACondition* _MB_con(const double t,
                                               const conditions::RROEACondition* const RROEA_Con,
                                               Variables *theVars);

    /**
      Calculate the Rates of RROEA based on the input RROEACondition.

      \param t The current timestamp
      \param RROEA_Con RROEACondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::RROEACondition* const RROEA_Con,
                      Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        TIME = 0.;
        N = 1;
        conditions::RROEACondition::reset();
    }
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the RROEA TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(RROEA);

}  // namespace modules
}  // namespace ePhotosynthesis
