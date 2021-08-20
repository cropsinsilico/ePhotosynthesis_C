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
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */
#include <math.h>
#include "drivers/driver.hpp"
#include "Variables.hpp"
#include "modules/RedoxReg.hpp"
#include "modules/RA.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

RedoxRegCondition* RedoxReg::_MB_con(const double t, const RedoxRegCondition* RedoxReg_Con,
                                     Variables *theVars) {

    trDynaPS2RedReg_cal = true;

    Rate(t, RedoxReg_Con, theVars);

    RACondition* RAdydt = RA::MB_con(t, RedoxReg_Con->RA_con, theVars);

    RedoxRegCondition* dydt = new RedoxRegCondition(RAdydt, 0.);

#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif
    return dydt;
}

int RedoxReg::RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data) {
    realtype *udata = N_VGetArrayPointer(u);
    realtype *fdata = N_VGetArrayPointer(f_val);
    UserData *data = static_cast<UserData*>(user_data);

    double Etr = data->coeffs[0];
    double Em = data->coeffs[1];
    realtype x = udata[0];
    fdata[0] = Etr - (Em - 0.03 * log10(x / (1. - x)));
    return 0;
}

arr RedoxReg::_MB(const double t, const RedoxRegCondition* RedoxReg_Con, Variables *theVars) {
    RedoxRegCondition* dydt = _MB_con(t, RedoxReg_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
