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
#include <nvector/nvector_serial.h>
#include "conditions/RedoxRegCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class RedoxRegModuleTest;
}
#endif
namespace modules {

#define MEMBERS_RedoxReg_CONSTANT		\
  Fd_Thio_ET,					\
    ThioT,					\
    Thio_Oxidation
#define MEMBERS_RedoxReg_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_RedoxReg_NOT_IN_ARRAY EMPTY_MEMBER_LIST

/**
 Class for RedoxReg related calculations and common variables
 */
class RedoxReg : public ModuleBase<RedoxReg, conditions::RedoxRegCondition, MODULE_RedoxReg> {
public:
    DECLARE_VALUE_SET_STATIC(RedoxReg, ModuleBase<RedoxReg, conditions::RedoxRegCondition, MODULE_RedoxReg>);
    SET_GET_BOOL_NOSKIP(trDynaPS2RedReg_cal)
private:
    friend ModuleBase;
#ifdef TESTING
    friend class test::RedoxRegModuleTest;
#endif
    /**
      Function to set the initial state of the RedoxRegCondition class.

      \param theVars Pointer to the global variables
      \return A RedoxRegCondition object with values set based on the input
      */
    static conditions::RedoxRegCondition* _init(Variables *theVars);

    /**
      Calculate the Rates of RedoxReg based on the input RedoxRegCondition.

      \param t The current timestamp
      \param RedoxReg_Con RedoxRegCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::RedoxRegCondition* RedoxReg_Con,
                      Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RedoxRegCondition at the given time stamp.

      \param t The current timestamp
      \param RedoxReg_Con RedoxRegCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::RedoxRegCondition* RedoxReg_Con,
                   Variables *theVars);

    /**
      Function to calculate the dy/dt values for the RedoxRegCondition at the given time stamp.

      \param t The current timestamp
      \param RedoxReg_Con RedoxRegCondition object giving the input parameters
      \param theVars The global variables
      \return A RedoxRegCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::RedoxRegCondition* _MB_con(const double t,
                                                  const conditions::RedoxRegCondition* RedoxReg_Con,
                                                  Variables *theVars);

    static int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        RedoxReg_VMAX13 = 0.;
        RedoxReg_VMAX16 = 0.;
        RedoxReg_VMAX6 = 0.;
        RedoxReg_VMAX9 = 0.;
        TIME = 0.;
        N = 1;
        conditions::RedoxRegCondition::reset();
    }
    SET_GET(RedoxReg_VMAX13)
    SET_GET(RedoxReg_VMAX16)
    SET_GET(RedoxReg_VMAX6)
    SET_GET(RedoxReg_VMAX9)
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the RedoxReg TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(RedoxReg);
  
}  // namespace modules
}  // namespace ePhotosynthesis
