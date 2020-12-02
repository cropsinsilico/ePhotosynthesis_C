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

// This model includes the mass balance equations for the full model of the light reactions.

arr EPS_Mb(const double t, const EPSCon &EPS_Con, Variables *theVars) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    //// Step One: Get the initialization of the concentrations for the PSPR model which will be used in the calculation of mb of CM.
    EPSCon EPSc(EPS_Con);
    arr CM_DYDT = CM_Mb(t, EPSc.CM_con, theVars);
    arr FIBF_DYDT = FIBF_Mb(t, EPSc.FIBF_con, theVars);

    // Step III: Calculate the mass balanec equation for the EPS model. This basically need to make sure that the variables
    // used in the mass balance equation should be in exact sequence with the sequence used in the inialization.

    arr EPS_DYDT;
    EPS_DYDT.reserve(96);
    EPS_DYDT.insert(EPS_DYDT.end(), FIBF_DYDT.begin(), FIBF_DYDT.begin() + 52);
    EPS_DYDT.insert(EPS_DYDT.end(), CM_DYDT.begin(), CM_DYDT.begin() + 36);

    EPS_DYDT[60] = CM_DYDT[8] - theVars->PS_Vel.v16 + theVars->EPS_ATP_Rate - theVars->PR_Vel.v124; //WY 201804
    EPS_DYDT[16] = EPS_DYDT[60];

    EPS_DYDT[61] = theVars->BF_Vel.vbfn2 / 2 - theVars->PS_Vel.v3 - 2 * theVars->PR_Vel.v124; //WY 201804
    EPS_DYDT[28] = EPS_DYDT[61];
    return EPS_DYDT;
}
