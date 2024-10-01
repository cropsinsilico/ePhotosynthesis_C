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
#include "conditions/RACondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class RAModuleTest;
}
#endif
namespace modules {
class DynaPS;

/**
 Class for the RA Module functions.
 */
class RA : public MODULE_BASE(RA) {
public:
    DECLARE_MODULE_COMPOSITE(RA)
private:
    friend ModuleBase;
    friend class modules::DynaPS;
#ifdef TESTING
    friend class test::RAModuleTest;
#endif

    /**
      Function to set the initial state of the RACondition class.

      \param theVars Pointer to the global variables
      \return A RACondition object with values set based on the input
      */
    static conditions::RACondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RACondition at the given time stamp.

      \param t The current timestamp
      \param RA_Con RACondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::RACondition* const RA_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RACondition at the given time stamp.

      \param t The current timestamp
      \param RA_Con RACondition object giving the input parameters
      \param theVars The global variables
      \return A RACondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::RACondition* _MB_con(const double t, const conditions::RACondition* const RA_Con,
                                            Variables *theVars);

    /**
      Calculate the Rates of RA based on the input RACondition.

      \param t The current timestamp
      \param RA_Con RACondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::RACondition* const RA_Con, Variables *theVars) {
        (void)t;
        (void)RA_Con;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
};

}  // namespace modules
}  // namespace ePhotosynthesis
