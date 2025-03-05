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

#include "modules/ModuleBase.hpp"
#include <nvector/nvector_serial.h>
#include "conditions/RedoxRegCondition.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE_COMPOSITE(RedoxReg);
  
namespace modules {

/**
 Class for RedoxReg related calculations and common variables
 */
class RedoxReg : public MODULE_BASE(RedoxReg) {
public:
    DECLARE_MODULE_TOP(RedoxReg) // RedoxReg is special case
    SET_GET_BOOL_NOSKIP(trDynaPS2RedReg_cal)
private:

    /** \copydoc ModuleBase::_Rate */
    static void _Rate(const double t, const conditions::RedoxRegCondition* RedoxReg_Con,
                      Variables *theVars);

    static int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);

    /** \copydoc ValueSetBase::_reset */
    static void _reset(const bool noChildren = false) {
        RedoxReg_VMAX13 = 0.;
        RedoxReg_VMAX16 = 0.;
        RedoxReg_VMAX6 = 0.;
        RedoxReg_VMAX9 = 0.;
        TIME = 0.;
        N = 1;
	ParentClass::_reset(noChildren);
    }
    SET_GET(RedoxReg_VMAX13)
    SET_GET(RedoxReg_VMAX16)
    SET_GET(RedoxReg_VMAX6)
    SET_GET(RedoxReg_VMAX9)
    EPHOTO_API static double TIME;    // The timestamp of the most recent call to _Rate
    EPHOTO_API static std::size_t N;  // The current size of the RedoxReg TimeSeries
};

  DEFINE_MODULE_COMPOSITE_HEADER(RedoxReg);
  
}  // namespace modules
}  // namespace ePhotosynthesis
