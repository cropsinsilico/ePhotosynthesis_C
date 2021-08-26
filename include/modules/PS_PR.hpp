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
#include "conditions/PS_PRCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class PS_PRModuleTest;
}
#endif
namespace modules {
class CM;

/**
 Class for the PS_PR Module functions.
 */
class PS_PR : public ModuleBase<PS_PR, conditions::PS_PRCondition> {
private:
    friend ModuleBase;
    friend class modules::CM;
#ifdef TESTING
    friend class test::PS_PRModuleTest;
#endif
    /**
      Function to set the initial state of the PS_PRCondition class.

      \param theVars Pointer to the global variables
      \return A PS_PRCondition object with values set based on the input
      */
    static conditions::PS_PRCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the PS_PRCondition at the given time stamp.

      \param t The current timestamp
      \param PS_PRs PS_PRCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::PS_PRCondition* const PS_PRs, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the PS_PRCondition at the given time stamp.

      \param t The current timestamp
      \param PS_PRs PS_PRCondition object giving the input parameters
      \param theVars The global variables
      \return A PS_PRCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::PS_PRCondition* _MB_con(const double t,
                                               const conditions::PS_PRCondition* const PS_PRs,
                                               Variables *theVars);

    /**
      Calculate the Rates of PS_PR based on the input PS_PRCondition.

      \param t The current timestamp
      \param PS_PRs PS_PRCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::PS_PRCondition* const PS_PRs, Variables *theVars) {
        (void)t;
        (void)PS_PRs;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
};

}  // namespace modules
}  // namespace ePhotosynthesis
