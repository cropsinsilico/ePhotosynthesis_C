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
#include "modules/EPS.hpp"
#include "modules/FIBF.hpp"
#include "modules/CM.hpp"
#include "modules/BF.hpp"

// This model includes the mass balance equations for the full model of the light reactions.

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

EPSCondition* EPS::_MB_con(const double t, const EPSCondition* EPS_Con, Variables *theVars) {
    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.
    //// Step One: Get the initialization of the concentrations for the PSPR model which will be used in the calculation of mb of CM.

    CMCondition* CMdydt = CM::MB_con(t, EPS_Con->CM_con, theVars);
    FIBFCondition* FIBFdydt = FIBF::MB_con(t, EPS_Con->FIBF_con, theVars);

    // Step III: Calculate the mass balanec equation for the EPS model. This basically need to make sure that the variables
    // used in the mass balance equation should be in exact sequence with the sequence used in the inialization.

    if (theVars->useC3) {
        CMdydt->PS_PR_con->PS_con->ATP = CMdydt->PS_PR_con->PS_con->ATP - theVars->PS_Vel.v16 + BF::getEPS_ATP_Rate();
        FIBFdydt->BF_con->NADPH = theVars->BF_Vel.vbfn2/2 - theVars->PS_Vel.v3;// - 1 * PS2EPS_NADPH/(PS2EPS_NADPH + 0.5) ;  // QF changed /2 and ;// - 1 * PS2EPS_NADPH/(PS2EPS_NADPH + 0.5)

    } else {
        CMdydt->PS_PR_con->PS_con->ATP = CMdydt->PS_PR_con->PS_con->ATP - theVars->PS_Vel.v16 + BF::getEPS_ATP_Rate() - theVars->PR_Vel.v124; //WY 201804
        FIBFdydt->BF_con->NADPH = theVars->BF_Vel.vbfn2 / 2 - theVars->PS_Vel.v3 - 2 * theVars->PR_Vel.v124; //WY 201804
    }

    EPSCondition* dydt = new EPSCondition(FIBFdydt, CMdydt);
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr EPS::_MB(const double t, const EPSCondition* EPS_Con, Variables *theVars) {
    EPSCondition* dydt = _MB_con(t, EPS_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
