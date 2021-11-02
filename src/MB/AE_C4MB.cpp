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
#include "modules/AE_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

AECondition* AE::_MB_con(const double t, const AECondition* AE_con, Variables *theVars) {


    //arr LM_v = RAC4LeafMetaVel(t, LM_con, myVars);
    Rate(t, AE_con, theVars);

    AECondition* dydt = new AECondition();

    dydt->Mchl_ActATPsynthase = theVars->AE_Vel.vATPsynthase_Act_Mchl;
    dydt->Mchl_ActGAPDH = theVars->AE_Vel.vGAPDH_Act_Mchl;
    dydt->Mchl_ActNADPMDH = theVars->AE_Vel.vNADPMDH_Act;
    dydt->Bchl_ActATPsynthase = theVars->AE_Vel.vATPsynthase_Act_Bchl;
    dydt->Bchl_ActPEPC = theVars->AE_Vel.vPEPC_Act;
    dydt->Bchl_ActGAPDH = theVars->AE_Vel.vGAPDH_Act_Bchl;
    dydt->Bchl_ActFBPase = theVars->AE_Vel.vFBPase_Act;
    dydt->Bchl_ActSBPase = theVars->AE_Vel.vSBPase_Act;
    dydt->Bchl_ActPRK = theVars->AE_Vel.vPRK_Act;
    dydt->Bchl_ActRubisco = theVars->AE_Vel.vRubisco_Act;
    dydt->Bchl_ActRCA = theVars->AE_Vel.vRCA_Act;

    return dydt;
}

arr AE::_MB(const double t, const AECondition* const AE_con, Variables *theVars) {
    AECondition* dydt = _MB_con(t, AE_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
