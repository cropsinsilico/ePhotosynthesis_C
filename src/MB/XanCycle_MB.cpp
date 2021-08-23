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
#include "Variables.hpp"
#include "modules/XanCycle.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

XanCycleCondition* XanCycle::_MB_con(const double t, const XanCycleCondition* const XanCycle_Con, Variables *theVars) {
    Condition(t, theVars);
    Rate(t, XanCycle_Con, theVars);

    XanCycleCondition* dydt = new XanCycleCondition();

    dydt->Vx = theVars->XanCycle_Vel.Vvf + theVars->XanCycle_Vel.Vav - theVars->XanCycle_Vel.Vva -
               theVars->XanCycle_Vel.Vv2ABA;
    dydt->Ax = theVars->XanCycle_Vel.Vva - theVars->XanCycle_Vel.Vav + theVars->XanCycle_Vel.Vza -
               theVars->XanCycle_Vel.Vaz;
    dydt->Zx = theVars->XanCycle_Vel.Vaz - theVars->XanCycle_Vel.Vza;
    dydt->ABA = theVars->XanCycle_Vel.Vv2ABA - theVars->XanCycle_Vel.VABAdg;
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr XanCycle::_MB(const double t, const XanCycleCondition* const XanCycle_Con, Variables *theVars) {
    XanCycleCondition* dydt = XanCycle::_MB_con(t, XanCycle_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
