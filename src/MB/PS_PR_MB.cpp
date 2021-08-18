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
#include "modules/PS_PR.hpp"
#include "modules/PS.hpp"
#include "modules/PR.hpp"
#include "globals.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

PS_PRCondition* PS_PR::_MB_con(const double t, const PS_PRCondition* PS_PR_con, Variables *theVars) {

    const double vATPcost = theVars->TestATPCost / theVars->AVR;

    PRCondition* PR_con = PS_PR_con->PR_con;
    PSCondition* PS_con = PS_PR_con->PS_con;
    PR::PGA = PS_con->PGA;
    const double PR2PS_Pgca = PS_PR_con->PR_con->PGCA;  // FOr transfering information between PR to PS.

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 2. Add exprimental conditions here; Conditions like light, temperature, CO2, O2 concentration should be added here //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Condition(t, theVars);

    arr PS_Param = zeros(2);
    PS_Param[0] = theVars->PS_PR_Param;
    PS_Param[1] = PR2PS_Pgca;

    PS::setParam(PS_Param);
    PSCondition* PSdydt = PS::MB_con(t, PS_con, theVars);

    arr PR_Param = zeros(2);
    PR_Param[0] = theVars->PS_PR_Param;      // To indicate that the calcualtion is using the combined model
    // for the PS-PR combined model. 0: Combined model; 1: Separate model
    PR_Param[1] = theVars->Pi;

    PRCondition* PRdydt = PR::MB_con(t, PR_con, theVars);

    // Assign the rate of reaction that is calculated from the photosynthesis and photorespiration routine.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 5.  Calculation of the mass balance equations ////
    //////////////////////////////////////////////////////////////////////////////////////////////$

    PSdydt->RuBP = theVars->PS_Vel.v13 - theVars->PS_Vel.v1 - theVars->PR_Vel.v111;
    PSdydt->PGA = 2 * theVars->PS_Vel.v1 - theVars->PS_Vel.v2 - theVars->PS_Vel.v32 + theVars->PR_Vel.v113 + theVars->PR_Vel.v111;

    if (theVars->useC3) {
        PSdydt->ATP = theVars->PS_Vel.v16 - theVars->PS_Vel.v2 - theVars->PS_Vel.v23 - theVars->PS_Vel.v13 - theVars->PR_Vel.v113;
    } else {
        PSdydt->ATP = theVars->PS_Vel.v16 - theVars->PS_Vel.v2 - theVars->PS_Vel.v23 - theVars->PS_Vel.v13 - theVars->PR_Vel.v113 - theVars->PS_Vel.v25 - theVars->PR_Vel.v131 - vATPcost;//WY202003 extra ATP cost
    }

    if (theVars->useC3)
        PSdydt->HexP = theVars->PS_Vel.v6 - theVars->PS_Vel.v7 - theVars->PS_Vel.v23;

    PS_PRCondition *dydt = new PS_PRCondition(PSdydt, PRdydt);
    //DEBUG_DELTA(PS_PR_DYDT)
    return dydt;
}

arr PS_PR::_MB(const double t, const PS_PRCondition* PS_PRs, Variables *theVars) {
    PS_PRCondition *dydt = _MB_con(t, PS_PRs, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}

