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
#include "modules/RA.hpp"
#include "modules/EPS.hpp"
#include "modules/RuACT.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

RACondition* RA::_MB_con(const double t, const RACondition* const RA_Con, Variables *theVars) {

    EPSCondition* EPSdydt = EPS::MB_con(t, RA_Con->EPS_con, theVars);
    RuACTCondition* RuACTdydt = RuACT::MB_con(t, RA_Con->RuACT_con, theVars);

    EPSdydt->CM_con->PS_PR_con->PS_con->RuBP = theVars->RuACT_Vel.v1 + theVars->PS_Vel.v13 -
                                               theVars->RuACT_Vel.vn1 + theVars->RuACT_Vel.vn7 -
                                               theVars->RuACT_Vel.v7;

    EPSdydt->CM_con->PS_PR_con->PS_con->PGA = EPSdydt->CM_con->PS_PR_con->PS_con->PGA -
                                              2 * theVars->PS_Vel.v1 + 2 * theVars->RuACT_Vel.v6_1 -
                                              theVars->PR_Vel.v111 + theVars->RuACT_Vel.v6_2; // Originally it is pspr(2), now use EPS_DYDT[53].

    EPSdydt->CM_con->PS_PR_con->PR_con->PGCA = EPSdydt->CM_con->PS_PR_con->PR_con->PGCA -
                                               theVars->PR_Vel.v111 + theVars->RuACT_Vel.v6_2;

    RACondition* dydt = new RACondition(EPSdydt, RuACTdydt);
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr RA::_MB(const double t, const RACondition* const RA_Con, Variables *theVars) {
    RACondition* dydt = _MB_con(t, RA_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
