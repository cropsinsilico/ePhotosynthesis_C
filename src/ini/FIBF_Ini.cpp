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
bool FIBFCondition::NPQ_kd = false;
bool FIBF::NPQ_kd = false;

DEFINE_MODULE_COMPOSITE(FIBF);

void FIBF::_initOrig(Variables *theVars, FIBFCondition* FIBF_con) {

    const double FIBF_PQT = 8.;
    theVars->FIBF_Pool.PQT = FIBF_PQT;
    FIBF_con->kd = pow(10, 8) * 0.5;
    theVars->FIBF_RC.kdm0 = 5. * pow(10., 8.);
    _initCalc(theVars, FIBF_con);

}

void FIBF::_initCalc(Variables *theVars, FIBFCondition* FIBF_con) {
    UNUSED(FIBF_con);
    theVars->FI_Pool.PQT = theVars->FIBF_Pool.PQT;
    theVars->BF_Pool.TQ = theVars->FIBF_Pool.PQT;
    FIBF::setNPQ_kd(theVars->UseZaksNPQ);
    
    BFCondition* BF_con = FIBF_con->BF_con;
    FICondition* FI_con = FIBF_con->FI_con;
    const double PQ = theVars->FIBF_Pool.PQT - theVars->FI_Pool.QBt - BF_con->QH2 - BF_con->Qi -
                      BF_con->Qn - BF_con->Qr - BF_con->ISPoQH2 - BF_con->QHsemi;

    FIBF2FI_PQa = theVars->FI_Pool.QBt + BF_con->Qi + BF_con->Qn + BF_con->Qr + BF_con->ISPoQH2 +
                  BF_con->QHsemi;
    BF_con->Q = PQ;
    FIBF2FI_PQ = PQ;

    theVars->FI_RC.kA_d = FIBF_con->kd;
    theVars->FI_RC.kU_d = FIBF_con->kd;
    theVars->BF_RC.Kd = FIBF_con->kd;
}

void FIBF::_reset(const bool noChildren)  {
#define DEFINE_FIBF_VAR(name) FIBF::name = 0.
    FOR_EACH(DEFINE_FIBF_VAR, EXPAND(MEMBERS_FIBF));
#undef DEFINE_FIBF_VAR
    setNPQ_kd(false);
    ParentClass::_reset(noChildren);
}
