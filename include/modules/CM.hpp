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
#include "conditions/CMCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class CMModuleTest;
}
#endif
namespace drivers {
class CMDriver;
}
namespace modules {
class EPS;
/**
Class for the CM Module functions.
  */
class CM : public MODULE_BASE(CM) {
public:
    DECLARE_MODULE_COMPOSITE(CM)
private:
    friend ModuleBase;
    friend class modules::EPS;
    friend class drivers::CMDriver;
#ifdef TESTING
    friend class test::CMModuleTest;
#endif

    /**
      Initialize the variables

      \param theVars The global variables
      \return A CMCondition object for input into calculations
      */
    static conditions::CMCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the CMCondition at the given time stamp.

      \param t The current timestamp
      \param CM_con CMCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::CMCondition* const CM_con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the CMCondition at the given time stamp.

      \param t The current timestamp
      \param CM_con CMCondition object giving the input parameters
      \param theVars The global variables
      \return A CMCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::CMCondition* _MB_con(const double t, const conditions::CMCondition* const CM_con,
                                            Variables *theVars);

    /**
      Calculate the Rates of CM based on the input CMCondition.

      \param t The current timestamp
      \param CM_con CMCondition object giving the input parameters
      \param theVars The global variables
      */

    static void _Rate(const double t, const conditions::CMCondition* const CM_con, Variables *theVars) {
        (void)t;
        (void)CM_con;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
    SET_GET_BOOL_NOSKIP(TestSucPath)
};

}  // namespace modules
}  // namespace ePhotosynthesis
