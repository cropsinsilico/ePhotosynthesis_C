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
#include "../conditions/XanCycleCondition.hpp"
#include "../definitions.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE(XanCycle);

namespace modules {

/**
 Class for XanCycle code and internal variables
 */
class XanCycle : public MODULE_BASE(XanCycle) {
public:
    DECLARE_MODULE(XanCycle)

    /**
      Method for updating the shared static variables.
      \param condition The input Condition class.
      \param theVars Pointer to the global variables
      */
      static void update_shared(const conditions::XanCycleCondition* const XanCycle_Con,
                                Variables *theVars);

private:
    /**
      Function to set the initial state of the XanCycleCondition class.

      \param theVars Pointer to the global variables
      \return A XanCycleCondition object with values set based on the input
      */
    static conditions::XanCycleCondition* _init(Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset(const bool noChildren = false) {
        setXanCycle2FIBF_Xstate(0.);
        setXanCycle2FIBF_Kd_NPQ(0.);
        setNPQ_connect(false);
        TIME = 0.;
        N = 1;
        ParentClass::_reset(noChildren);
    }

    SET_GET(XanCycle2FIBF_Xstate)
    SET_GET(XanCycle2FIBF_Kd_NPQ)
    SET_GET_BOOL_MODULE(NPQ_connect, conditions::XanCycle)

};

  DEFINE_MODULE_HEADER(XanCycle);
  
}  // namespace modules
}  // namespace ePhotosynthesis
