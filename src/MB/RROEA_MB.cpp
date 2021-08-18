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
#include "modules/RROEA.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

RROEACondition* RROEA::_MB_con(const double t, const RROEACondition* RROEA_Con, Variables *theVars) {
    Condition(t, theVars);
    theVars->RROEA_Param[0] = theVars->GLight;

    Rate(t, RROEA_Con, theVars);

    RROEACondition* dydt = new RROEACondition();
    dydt->GAPDH = theVars->RROEA_Vel.ve2GAPDH;  // GAPDH
    dydt->FBPase = theVars->RROEA_Vel.ve2FBPase; // FBPase
    dydt->SBPase = theVars->RROEA_Vel.ve2SBPase; // SBPase
    dydt->PRK = theVars->RROEA_Vel.ve2PRK;    // PRK
    dydt->ATPase = theVars->RROEA_Vel.ve2ATPase; // ATPase
    dydt->ATPGPP = theVars->RROEA_Vel.ve2ATPGPP; // ATPGPP
    dydt->MDH = theVars->RROEA_Vel.ve2MDH;    // MDH
    dydt->Thio = theVars->RROEA_Vel.veFd2Thio - theVars->RROEA_Vel.ve2GAPDH - theVars->RROEA_Vel.ve2FBPase - theVars->RROEA_Vel.ve2SBPase - theVars->RROEA_Vel.ve2PRK - theVars->RROEA_Vel.ve2ATPGPP - theVars->RROEA_Vel.ve2RuACT; // Thio
    dydt->Fd = theVars->RROEA_Vel.ve2Fd - theVars->RROEA_Vel.veFd2Thio - theVars->RROEA_Vel.veFd2Calvin; // Fd
    dydt->RuACT = theVars->RROEA_Vel.ve2RuACT;  // RuACT;
    //DEBUG_DELTA(dydt->)
    return dydt;
}

arr RROEA::_MB(const double t, const RROEACondition* RROEA_Con, Variables *theVars) {
    RROEACondition* dydt = _MB_con(t, RROEA_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
