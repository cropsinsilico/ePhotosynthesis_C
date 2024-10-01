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
#include "conditions/EPSCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class EPSModuleTest;
}
#endif
namespace drivers {
class EPSDriver;
}
namespace modules {
class RA;

/**
 Class for the EPS Module functions.
 */
class EPS : public MODULE_BASE(EPS) {
public:
    DECLARE_MODULE_COMPOSITE(EPS);
private:
    friend ModuleBase;
    friend class modules::RA;
    friend class drivers::EPSDriver;
#ifdef TESTING
    friend class test::EPSModuleTest;
#endif

    /**
      Function to set the initial state of the EPSCondition class.

      \param theVars Pointer to the global variables
      \return A EPSCondition object with values set based on the input
      */
    static conditions::EPSCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the EPSCondition at the given time stamp.

      \param t The current timestamp
      \param EPS_Con EPSCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::EPSCondition* const EPS_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the EPSCondition at the given time stamp.

      \param t The current timestamp
      \param EPS_Con EPSCondition object giving the input parameters
      \param theVars The global variables
      \return A EPSCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::EPSCondition* _MB_con(const double t,
                                             const conditions::EPSCondition* const EPS_Con,
                                             Variables *theVars);

    /**
      Calculate the Rates of EPS based on the input EPSCondition.

      \param t The current timestamp
      \param EPS_Con EPSCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::EPSCondition* const EPS_Con, Variables *theVars) {
        (void)t;
        (void)EPS_Con;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
};

}  // namespace modules
}  // namespace ePhotosynthesis
