#include "Variables.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This model includes the mass balance equations for the full model of the light reactions.

arr EPS_mb(double t, EPSCon &EPS_Con, Variables *myVars) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    //// Step One: Get the initialization of the concentrations for the PSPR model which will be used in the calculation of mb of CM.
    EPSCon EPSc(EPS_Con);
    arr CM_DYDT = CM_Mb(t, EPSc.CM_con, myVars);
    arr FIBF_DYDT = FIBF_MB(t, EPSc.FIBF_con, myVars);

    // Step III: Calculate the mass balanec equation for the EPS model. This basically need to make sure that the variables
    // used in the mass balance equation should be in exact sequence with the sequence used in the inialization.

    arr EPS_DYDT = zeros(96);
    for (size_t m = 0; m < 52; m++)
        EPS_DYDT[m] = FIBF_DYDT[m];

    for (size_t m = 0; m < 36; m++)
        EPS_DYDT[m + 52] = CM_DYDT[m];

    EPS_DYDT[60] = CM_DYDT[8] - myVars->PS2EPS_V16 + myVars->EPS_ATP_Rate - myVars->PRGlu;//WY 201804
    EPS_DYDT[16] = EPS_DYDT[60];

    EPS_DYDT[61] = myVars->BF2EPS_vbfn2 / 2 - myVars->PS2EPS_v3 - 2 * myVars->PRGlu;//WY 201804
    EPS_DYDT[28] = EPS_DYDT[61];
    return EPS_DYDT;
}
