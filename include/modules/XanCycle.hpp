#pragma once
#include "definitions.hpp"

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
#include "conditions/XanCycleCondition.hpp"
#include "definitions.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class XanCycleModuleTest;
}
#endif
namespace modules {
class DynaPS;

#define MEMBERS_XanCycle			\
  kav,						\
    kaz,					\
    kva,					\
    kza,					\
    XanCycle2FIBF_Xstate
#define MEMBERS_XanCycle_CONSTANT EMPTY_MEMBER_LIST
#define MEMBERS_XanCycle_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_XanCycle_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for XanCycle code and internal variables
 */
class XanCycle : public ModuleBase<XanCycle, conditions::XanCycleCondition, MODULE_XanCycle> {
public:
    DECLARE_VALUE_SET_STATIC(XanCycle, ModuleBase<XanCycle, conditions::XanCycleCondition, MODULE_XanCycle>)
private:
    friend ModuleBase;
    friend class modules::DynaPS;
#ifdef TESTING
    friend class test::XanCycleModuleTest;
#endif
    /**
      Function to set the initial state of the XanCycleCondition class.

      \param theVars Pointer to the global variables
      \return A XanCycleCondition object with values set based on the input
      */
    static conditions::XanCycleCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the XanCycleCondition at the given time stamp.

      \param t The current timestamp
      \param XanCycle_Con XanCycleCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::XanCycleCondition* XanCycle_Con,
                   Variables *theVars);

    /**
      Function to calculate the dy/dt values for the XanCycleCondition at the given time stamp.

      \param t The current timestamp
      \param XanCycle_Con XanCycleCondition object giving the input parameters
      \param theVars The global variables
      \return A XanCycleCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::XanCycleCondition* _MB_con(const double t,
                                                  const conditions::XanCycleCondition* XanCycle_Con,
                                                  Variables *theVars);

    /**
      Calculate the Rates of XanCycle based on the input XanCycleCondition.

      \param t The current timestamp
      \param XanCycle_Con XanCycleCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::XanCycleCondition* XanCycle_Con,
                      Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        kav = 0.;
        kaz = 0.;
        kva = 0.;
        kza = 0.;
        TIME = 0.;
        N = 1;
        conditions::XanCycleCondition::reset();
    }

    SET_GET(kav)
    SET_GET(kaz)
    SET_GET(kva)
    SET_GET(kza)
    SET_GET(XanCycle2FIBF_Xstate)

    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the XanCycle TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(XanCycle);
  
}  // namespace modules
}  // namespace ePhotosynthesis
