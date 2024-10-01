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
#include "conditions/DynaPSCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class DynaPSModuleTest;
}
#endif
namespace drivers {
class DynaPSDriver;
}
namespace modules {
class trDynaPS;

/**
 Class for the DynaPS Module functions.
 */
class DynaPS : public MODULE_BASE(DynaPS) {
public:
    DECLARE_MODULE_COMPOSITE(DynaPS)
private:
    friend ModuleBase;
    friend class modules::trDynaPS;
    friend class drivers::DynaPSDriver;
#ifdef TESTING
    friend class test::DynaPSModuleTest;
#endif

    /**
      Initialize the variables

      \param theVars The global variables
      \return A DynaPSCon object for input into calculations
      */
    static conditions::DynaPSCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the DynaPSCondition at the given time stamp.

      \param t The current timestamp
      \param DynaPS_con DynaPSCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::DynaPSCondition* const DynaPS_con,
                   Variables *theVars);

    /**
      Function to calculate the dy/dt values for the DynaPSCondition at the given time stamp.

      \param t The current timestamp
      \param DynaPS_con DynaPSCondition object giving the input parameters
      \param theVars The global variables
      \return A DynaPSCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::DynaPSCondition* _MB_con(const double t,
                                                const conditions::DynaPSCondition* const DynaPS_con,
                                                Variables *theVars);

    /**
      Calculate the Rates of DynaPS based on the input DynaPSCondition.

      \param t The current timestamp
      \param DynaPS_con DynaPSCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::DynaPSCondition* const DynaPS_con,
                      Variables *theVars) {
        (void)t;
        (void)DynaPS_con;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
};

}  // namespace modules
}  // namespace ePhotosynthesis
