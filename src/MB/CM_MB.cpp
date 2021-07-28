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
#include <sundials/sundials_types.h>
#include "Variables.hpp"
#include "modules/CM.hpp"
#include "modules/SUCS.hpp"
#include "modules/PS_PR.hpp"

arr CM::_MB(const realtype t, const CMCondition* CM_con, Variables *theVars) {
    arr dxdt;
    dxdt.reserve(36);
    arr PSPR_DYDT;
    arr SUCS_DYDT;

    if (theVars->useC3) {
        PSPR_DYDT = PS_PR::MB(t, CM_con->PS_PR_con, theVars);
        SUCS_DYDT = SUCS::MB(t, CM_con->SUCS_con, theVars);
    } else {
        SUCS_DYDT = SUCS::MB(t, CM_con->SUCS_con, theVars);
        PSPR_DYDT = PS_PR::MB(t, CM_con->PS_PR_con, theVars);
    }

    dxdt.insert(dxdt.end(), PSPR_DYDT.begin(), PSPR_DYDT.begin() + 23);
    dxdt.insert(dxdt.end(), SUCS_DYDT.begin(), SUCS_DYDT.begin() + 12);

    if (!theVars->useC3) {
        dxdt[35] = PSPR_DYDT[23];

        // The rate of import into the cytosol
        if (theVars->TestSucPath == 1)
            SUCS_DYDT[0] = SUCS_DYDT[0] + theVars->PS_Vel.v31 + theVars->PS_Vel.v33 - (theVars->SUCS_Vel.vdhap_in + theVars->SUCS_Vel.vgap_in);

        //	T3Pc WY1905
        dxdt[23] = SUCS_DYDT[0];
        SUCS_DYDT[11] = SUCS_DYDT[11] - theVars->SUCS_Vel.vpga_in + theVars->PS_Vel.v32;//	pgaC
        dxdt[34] = SUCS_DYDT[11];
    }
    DEBUG_DELTA(dxdt)
    return dxdt;
}
