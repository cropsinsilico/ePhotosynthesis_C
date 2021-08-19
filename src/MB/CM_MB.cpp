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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

CMCondition* CM::_MB_con(const realtype t, const CMCondition* CM_con, Variables *theVars) {
#ifdef INCDEBUG
    DEBUG_MESSAGE(CM_con)
#endif

    PS_PRCondition* PSPRdydt;
    SUCSCondition* SUCSdydt;

    if (theVars->useC3) {
        PSPRdydt = PS_PR::MB_con(t, CM_con->PS_PR_con, theVars);
        SUCSdydt = SUCS::MB_con(t, CM_con->SUCS_con, theVars);
    } else {
        SUCSdydt = SUCS::MB_con(t, CM_con->SUCS_con, theVars);
        PSPRdydt = PS_PR::MB_con(t, CM_con->PS_PR_con, theVars);
    }

    if (!theVars->useC3) {
        // The rate of import into the cytosol
        if (CM::TestSucPath)
            SUCSdydt->T3Pc = SUCSdydt->T3Pc + theVars->PS_Vel.v31 + theVars->PS_Vel.v33 - (theVars->SUCS_Vel.vdhap_in + theVars->SUCS_Vel.vgap_in);

        //	T3Pc WY1905
        SUCSdydt->PGAc = SUCSdydt->PGAc - theVars->SUCS_Vel.vpga_in + theVars->PS_Vel.v32;//	pgaC
    }

    CMCondition *dydt = new CMCondition(PSPRdydt, SUCSdydt);
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

arr CM::_MB(const realtype t, const CMCondition *CM_con, Variables *theVars) {
    CMCondition *dydt = _MB_con(t, CM_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
