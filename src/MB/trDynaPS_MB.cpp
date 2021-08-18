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
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include "modules/trDynaPS.hpp"
#include "modules/RROEA.hpp"
#include "modules/DynaPS.hpp"
#include "modules/RedoxReg.hpp"
// This model includes the mass balance equations for the full model of photosynthesis.

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

trDynaPSCondition* trDynaPS::_MB_con(const double t, const trDynaPSCondition* trDynaPS_con, Variables *theVars) {
    RedoxReg::settrDynaPS2RedReg_cal(false);

    Condition(t, theVars);
    const double light = 1.0;
    theVars->FI_Param[0] = light;
    theVars->BF_Param[0] = light;

    theVars->RROEA_Param[1] = 1;
    RROEACondition* RROEAdydt = RROEA::MB_con(t, trDynaPS_con->RROEA_con, theVars);

    DynaPSCondition* DynaPSdydt = DynaPS::MB_con(t, trDynaPS_con->DynaPS_con, theVars);


    //WY201804
    const double Temp = RROEAdydt->Fd - theVars->RROEA_Vel.ve2Fd + theVars->BF_Vel.Vbf16 / theVars->AVR + theVars->RROEA_Vel.veFd2Calvin - theVars->BF_Vel.vbfn2 - theVars->BF_Vel.vcet / theVars->AVR;

    RROEAdydt->Fd = Temp * theVars->AVR;
    DynaPSdydt->RA_con->EPS_con->FIBF_con->BF_con->Fdn = RROEAdydt->Fd;

    GenOut(t, theVars);
    //DEBUG_DELTA(dxdt)
    trDynaPSCondition* dydt = new trDynaPSCondition(DynaPSdydt, RROEAdydt);
    return dydt;
}

arr trDynaPS::_MB(const double t, const trDynaPSCondition* trDynaPS_con, Variables *theVars) {
    trDynaPSCondition* dydt = _MB_con(t, trDynaPS_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
