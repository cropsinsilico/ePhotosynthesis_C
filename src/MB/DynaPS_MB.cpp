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

#include "globals.hpp"
#include "modules/DynaPS.hpp"
#include "Variables.hpp"
#include "modules/RA.hpp"
#include "modules/XanCycle.hpp"

// This model includes the mass balance equations for the full model of photosynthesis.

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DynaPSCondition* DynaPS::_MB_con(const double t, const DynaPSCondition* DynaPS_con, Variables *theVars) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    //// Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.

    // This is a sensitivity test to show that the model is stable udner fluctuating light

    const double light = 1;
    Condition(t, theVars);

    theVars->FI_Param[0] = light;
    theVars->BF_Param[0] = light;

    RACondition* RAdydt = RA::MB_con(t, DynaPS_con->RA_con, theVars);
    XanCycleCondition* XanCycledydt = XanCycle::MB_con(t, DynaPS_con->XanCycle_con, theVars);

    // Here get the rate of Thioredoxin reduction and oxidation and use it to construct the differential equation for both thio and fd.
    DynaPSCondition* dydt = new DynaPSCondition(RAdydt, XanCycledydt);
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr DynaPS::_MB(const double t, const DynaPSCondition* DynaPS_con, Variables *theVars) {
    DynaPSCondition* dydt = _MB_con(t, DynaPS_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
