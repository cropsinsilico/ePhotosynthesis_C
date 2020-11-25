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
#include "CM.hpp"

arr CM_Mb(realtype t, CMCon &CM_con, Variables *myVars) {
    arr dxdt;
    dxdt.reserve(36);
    arr SUCS_DYDT = SUCS_Mb(t, CM_con.SUCS_con, myVars);

    arr PSPR_DYDT = PS_PRmb(t, CM_con.PS_PR_con, myVars);

    dxdt.insert(dxdt.end(), PSPR_DYDT.begin(), PSPR_DYDT.begin() + 23);
    dxdt.insert(dxdt.end(), SUCS_DYDT.begin(), SUCS_DYDT.begin() + 12);

    dxdt[35] = PSPR_DYDT[23];

    // The rate of import into the cytosol
    if (myVars->TestSucPath == 1)
        SUCS_DYDT[0] = SUCS_DYDT[0] + myVars->PS_Vel.v31 + myVars->PS_Vel.v33 - (myVars->SUCS_Vel.vdhap_in + myVars->SUCS_Vel.vgap_in);

    //	T3Pc WY1905
    dxdt[23] = SUCS_DYDT[0];

    SUCS_DYDT[11] = SUCS_DYDT[11] - myVars->SUCS_Vel.vpga_in + myVars->PS_Vel.v32;//	pgaC
    dxdt[34] = SUCS_DYDT[11];
    return dxdt;
}
