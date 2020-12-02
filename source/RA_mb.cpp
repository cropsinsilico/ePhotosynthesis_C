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

arr RA_Mb(const double t, const RACon &RA_Con, Variables *theVars) {
    arr EPS_DYDT = EPS_Mb(t, RA_Con.EPS_con, theVars);
    arr RuACT_DYDT = RuACT::RuACT_Mb(t, RA_Con.RuACT_con, theVars);

    arr RA_DYDT = zeros(92);

    for (size_t m = 0; m < 88; m++)
        RA_DYDT[m] = EPS_DYDT[m];


    for (size_t m = 0; m < 4; m++)
        RA_DYDT[m + 88] = RuACT_DYDT[m];

    const double DYDT_RuBP = theVars->RuACT_Vel.v1 + theVars->PS_Vel.v13 - theVars->RuACT_Vel.vn1 + theVars->RuACT_Vel.vn7 - theVars->RuACT_Vel.v7;
    RA_DYDT[52] = DYDT_RuBP;
    RA_DYDT[91] = DYDT_RuBP;

    const double DYDT_PGA = EPS_DYDT[53] - 2 * theVars->PS_Vel.v1 + 2 * theVars->RuACT_Vel.v6_1 - theVars->PR_Vel.v111 + theVars->RuACT_Vel.v6_2;// Originally it is pspr(2), now use EPS_DYDT[53].
    RA_DYDT[53] = DYDT_PGA;


    const double DYDT_PGCA = EPS_DYDT[68] - theVars->PR_Vel.v111 + theVars->RuACT_Vel.v6_2;
    RA_DYDT[68] = DYDT_PGCA;
    return RA_DYDT;
}
