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
#include "modules/FIBF.hpp"
#include "modules/FI.hpp"
#include "modules/BF.hpp"
// This is the function to initialize some global variable used in FIBF MODEL

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

std::size_t FIBFCondition::count = 0;

DEFINE_MODULE_COMPOSITE(FIBF);

void FIBF::_initOrig(Variables *theVars, FIBFCondition* FIBF_con) {

    const double FIBF_PQT = 8.;
    theVars->FIBF_Pool.PQT = FIBF_PQT;
    _initCalc(theVars, FIBF_con);

}

void FIBF::_initCalc(Variables *theVars, FIBFCondition* FIBF_con) {
    UNUSED(FIBF_con);
    theVars->FI_Pool.PQT = theVars->FIBF_Pool.PQT;
    theVars->BF_Pool.k_r1 = theVars->FIBF_Pool.PQT;
}

// void FIBF::_initAlt(Variables *theVars, FIBFCondition* FIBF_con) {
// #ifdef CHECK_VALUE_SET_ALTS
//     theVars->initParamStatic<FIBF>();
//     theVars->initParam(theVars->FIBF_Pool);
//     theVars->initParam(*FIBF_con);
//     theVars->FI_Pool[POOL::FI::PQT] = theVars->FIBF_Pool[POOL::FIBF::PQT];
//     theVars->BF_Pool[POOL::BF::k_r1] = theVars->FIBF_Pool[POOL::FIBF::PQT];
// #else // CHECK_VALUE_SET_ALTS
//     UNUSED(theVars);
//     UNUSED(FIBF_con);
// #endif // CHECK_VALUE_SET_ALTS
// }

void FIBF::_reset()  {
    ChlPSI = 0.;
    ChlT = 0.;
    ChlT2 = 0.;
    FIBF2FI_PQ = 0.;
    FIBF2FI_PQa = 0.;
}
