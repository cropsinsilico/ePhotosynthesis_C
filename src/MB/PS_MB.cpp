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

#include "Variables.hpp"
#include "globals.hpp"
#include "modules/PS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

PSCondition* PS::_MB_con(const double t, const PSCondition* PS_con, Variables *theVars) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifying KM, KI, KE VMAX for different reactions as the regulation//
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef INCDEBUG
    DEBUG_MESSAGE(PS_con)
#endif

    // Regulations first.
    Condition(t, theVars);

    // Get the rate for the reactions in the photosynthesis sytem
    //PS::setParam(Param);
    Rate(t, PS_con, theVars);

    // Get the rate

    // Implement the mass balance equations

    PSCondition *dydt = new PSCondition();
    dydt->RuBP = theVars->PS_Vel.v13 - theVars->PS_Vel.v1;
    dydt->PGA = 2 * theVars->PS_Vel.v1 - theVars->PS_Vel.v2 - theVars->PS_Vel.v32;
    dydt->DPGA = theVars->PS_Vel.v2 - theVars->PS_Vel.v3;
    dydt->T3P = theVars->PS_Vel.v3 - 2 * theVars->PS_Vel.v5 - theVars->PS_Vel.v7 - theVars->PS_Vel.v8 - theVars->PS_Vel.v10 - theVars->PS_Vel.v31 - theVars->PS_Vel.v33;
    dydt->ADPG = theVars->PS_Vel.v23 - theVars->PS_Vel.v24;
    dydt->FBP = theVars->PS_Vel.v5 - theVars->PS_Vel.v6;
    dydt->E4P = theVars->PS_Vel.v7 - theVars->PS_Vel.v8;
    dydt->S7P = theVars->PS_Vel.v9 - theVars->PS_Vel.v10;
    dydt->SBP = theVars->PS_Vel.v8 - theVars->PS_Vel.v9;
    dydt->ATP = theVars->PS_Vel.v16 - theVars->PS_Vel.v2 - theVars->PS_Vel.v23 - theVars->PS_Vel.v13 - theVars->PS_Vel.v25;
    dydt->HexP = theVars->PS_Vel.v6 - theVars->PS_Vel.v7 - theVars->PS_Vel.v23 + theVars->PS_Vel.v25;
    dydt->PenP = theVars->PS_Vel.v7 + theVars->PS_Vel.v10 * 2 - theVars->PS_Vel.v13;
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr PS::_MB(const double t, const PSCondition* PS_con, Variables *theVars) {
    PSCondition *dydt = _MB_con(t, PS_con, theVars);
    arr dydtarr = dydt->toArray();
    delete dydt;
    return dydtarr;
}
