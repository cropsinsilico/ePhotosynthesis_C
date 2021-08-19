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
#include "modules/RuACT.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

RuACTCondition* RuACT::_MB_con(const double t, const RuACTCondition* RuACT_Con, Variables *theVars) {

    Condition(t, theVars);
    const double light = theVars->GLight;

    theVars->RuACT_Param[0] = light;

    Rate(t, RuACT_Con, theVars);

    const double v1 = theVars->RuACT_Vel.v1;     // v1 The rate of ER activation due to Rubisco activase
    const double vn1 = theVars->RuACT_Vel.vn1;   // vn1 The rate of E inactiavtion due to binding of RuBP
    const double v7 = theVars->RuACT_Vel.v7;     // v7 The rate of formation of ECMR from ECM by binding of RuBP
    const double vn7 = theVars->RuACT_Vel.vn7;   // vn7 The rate of actiavtion of ECMR by Rubisco activase
    const double v6_1 = theVars->RuACT_Vel.v6_1; // v6_1 The rate of RuBP carboxylation
    const double v6_2 = theVars->RuACT_Vel.v6_2; // v6_2 The rate of RuBP oxygenation

    RuACTCondition* dydt = new RuACTCondition();
    dydt->ER = vn1 - v1;                          // ER
    dydt->Eaf = v1 - v7 + vn7 + v6_1 + v6_2 - vn1; // EAF
    dydt->ECMR = v7 - vn7 - v6_1 - v6_2;            // ECMR
    if (!theVars->RuACT_EPS_com)
        dydt->RuBP = v6_1 + v6_2 + v1 - vn1 + vn7 - v7; // RuBP
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr RuACT::_MB(const double t, const RuACTCondition* RuACT_Con, Variables *theVars) {
    RuACTCondition* dydt = _MB_con(t, RuACT_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
