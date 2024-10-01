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
#include "modules/trDynaPS.hpp"
#include "modules/DynaPS.hpp"
#include "modules/RROEA.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DEFINE_VALUE_SET(trDynaPSCondition);

trDynaPSCondition* trDynaPS::_init(Variables *theVars) {
    DynaPSCondition* DynaPS_con = DynaPS::init(theVars);

    RROEACondition* RROEA_con = RROEA::init(theVars);
    if (theVars->RROEA_EPS_com)
        RROEA_con->Fd = DynaPS_con->RA_con->EPS_con->FIBF_con->BF_con->Fdn;
    trDynaPSCondition* trDynaPS_Con = new trDynaPSCondition(DynaPS_con, RROEA_con);
    return trDynaPS_Con;
}

trDynaPSCondition* trDynaPS::_initAlt(Variables *theVars, trDynaPSCondition* trDynaPS_Con) {
#ifdef CHECK_VALUE_SET_ALTS
    if (theVars->RROEA_EPS_com)
	trDynaPS_Con->RROEA_con->set(COND::RROEA::Fd,
				     trDynaPS_Con->DynaPS_con->RA_con->EPS_con->FIBF_con->BF_con->Fdn);
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
#endif // CHECK_VALUE_SET_ALTS
    return trDynaPS_Con;
}

void trDynaPS::_updateAlts(Variables *theVars, trDynaPSCondition* trDynaPS_con) {
    UNUSED(theVars);
    UNUSED(trDynaPS_con);
}

void trDynaPS::_reset() {
    DynaPS::reset();
    RROEA::reset();
    conditions::trDynaPSCondition::reset();
}
