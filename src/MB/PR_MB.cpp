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
\
#include "Variables.hpp"
#include "globals.hpp"
#include "modules/PR.hpp"

PRCondition* PR::_MB_con(const double t, const PRCondition* PR_con, Variables *theVars) {
    Condition(t, theVars);

    Rate(t, PR_con, theVars);

    PRCondition *dydt = new PRCondition();
    dydt->GCEA = theVars->PR_Vel.v1in - theVars->PR_Vel.v113;
    dydt->GCA = theVars->PR_Vel.v112 - theVars->PR_Vel.v2out;
    dydt->PGCA = theVars->PR_Vel.v111 - theVars->PR_Vel.v112;
    dydt->GCAc = theVars->PR_Vel.v2out - theVars->PR_Vel.v121;
    dydt->GOAc = theVars->PR_Vel.v121 - theVars->PR_Vel.v122 - theVars->PR_Vel.v124;
    dydt->SERc = theVars->PR_Vel.v131 - theVars->PR_Vel.v122;
    dydt->GLYc = theVars->PR_Vel.v122 + theVars->PR_Vel.v124 - 2 * theVars->PR_Vel.v131;
    dydt->HPRc = theVars->PR_Vel.v122 - theVars->PR_Vel.v123;
    dydt->GCEAc = theVars->PR_Vel.v123 - theVars->PR_Vel.v1in;
    if (!PR_PS_com)
        dydt->RuBP = 0.3 - theVars->PR_Vel.v111;
    //DEBUG_DELTA(PRdydt)
    return dydt;
}

arr PR::_MB(const double t, const PRCondition* const PR_con, Variables *theVars) {
    PRCondition* dydt = _MB_con(t, PR_con, theVars);
    arr dydtarr = dydt->toArray();
    delete dydt;
    return dydtarr;
}
