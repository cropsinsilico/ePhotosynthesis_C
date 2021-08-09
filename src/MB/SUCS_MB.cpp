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
#include "modules/SUCS.hpp"

SUCSCondition* SUCS::_MB_con(const double t, const SUCSCondition* SUCS_Con, Variables *theVars) {
    Condition(t, theVars);

    theVars->SUCS_Param[0] = theVars->GLight;

    //arr SUCS_Vel = zeros(15);
    Rate(t, SUCS_Con, theVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation//
    ////////////////////////////////////////////////////////////////
    // The Major Variables
    SUCSCondition *dydt = new SUCSCondition();
    if (theVars->useC3) {
        dydt->T3Pc = theVars->PS_Vel.v31 + theVars->PS_Vel.v33 - 2 * theVars->SUCS_Vel.v51; // T3Pc
    } else {
        dydt->T3Pc = theVars->SUCS_Vel.vdhap_in + theVars->SUCS_Vel.vgap_in - 2 * theVars->SUCS_Vel.v51; // T3Pc
    }
    dydt->FBPc = theVars->SUCS_Vel.v51 - theVars->SUCS_Vel.v52; // FBPc
    dydt->HexPc = theVars->SUCS_Vel.v52 - theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v59 + theVars->SUCS_Vel.v58 - theVars->SUCS_Vel.v56;// HexPc
    dydt->F26BPc = theVars->SUCS_Vel.v59 - theVars->SUCS_Vel.v58; // F26BPc
    dydt->UDPGc = theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v56; // UDPGc
    //dydt->UTPc = 0; //  v60 - v55  ;   // UTPc
    dydt->SUCP = theVars->SUCS_Vel.v56 - theVars->SUCS_Vel.v57; // SUCP
    dydt->SUC = theVars->SUCS_Vel.v57 - theVars->SUCS_Vel.v62; // SUC
    if (theVars->useC3) {
        dydt->PGAc = theVars->PS_Vel.v32 - theVars->SUCS_Vel.vpga_use;
    } else {
        dydt->PGAc = theVars->SUCS_Vel.vpga_in - theVars->SUCS_Vel.vpga_use; // pgaC
    }
    //DEBUG_DELTA(SUCS_mb)
    return dydt;
}

arr SUCS::_MB(const double t, const SUCSCondition *SUCS_Con, Variables *theVars) {
    SUCSCondition* dydt = _MB_con(t, SUCS_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
