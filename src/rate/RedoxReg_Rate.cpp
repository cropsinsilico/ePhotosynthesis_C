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
#include <math.h>
#include "Variables.hpp"
#include "modules/RedoxReg.hpp"
#include "drivers/driver.hpp"
#include <kinsol/kinsol.h>             /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix       */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver */
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */

#define ONE    RCONST(1.0)
#define ZERO   RCONST(0.0)

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void RedoxReg::_Rate(const double t, const RedoxRegCondition* RedoxReg_Con, Variables *theVars) {
    const double Thio = ThioT - RedoxReg_Con->Thion;

    theVars->RedoxReg_MP[0][2] = RedoxReg_Con->Thion / ThioT;

    double TEMP = theVars->RedoxReg_MP[0][2];

    if (theVars->RROEA_EPS_com)
        TEMP = 0.5;

    if (trDynaPS2RedReg_cal) {
        drivers::UserData *data = drivers::alloc_user_data();
        data->coeffs.resize(2);

        data->coeffs[0] = theVars->RedoxReg_MP[0][1] - 0.03 * log10(TEMP / (1. - TEMP));

        N_Vector y, constraints, scaling;
        y = N_VNew_Serial(1);
        constraints = N_VNew_Serial(1);
        scaling = N_VNew_Serial(1);
        N_VConst(ONE, scaling);
        realtype abstol = 1e-5;
        //realtype reltol = 1e-4;
        NV_Ith_S(constraints, 0) = ZERO;
        void *kmem = nullptr;
        kmem = KINCreate();

        SUNMatrix A = SUNDenseMatrix(1, 1);
        SUNLinearSolver LS = SUNDenseLinearSolver(y, A);
        KINSetLinearSolver(kmem, LS, A);

        for (size_t index = 1; index < 5; index++) {

            NV_Ith_S(y, 0) = theVars->RedoxReg_MP[index][2];
            data->coeffs[1] = theVars->RedoxReg_MP[index][1];
            KINSetUserData(kmem, data);
            KINSetConstraints(kmem, constraints);
            KINSetFuncNormTol(kmem, abstol);
            KINSetScaledStepTol(kmem, abstol);
            KINInit(kmem, RedoxReg_FPercent, y);
            KINSol(kmem, y, KIN_LINESEARCH, scaling, scaling);
            realtype *retvals = N_VGetArrayPointer(y);
            //pr = fsolve(@RedoxReg_FPercent, RedPercent, optimset('Display', 'off'), RedP, MPE);
            theVars->RedoxReg_MP[index][2] = retvals[0];


            if (static_cast<int>(theVars->RedoxReg_MP[index][0]) == 6) {
                RedoxRegCondition::setV6(RedoxReg_VMAX6 * theVars->RedoxReg_MP[index][2]);
            } else if (static_cast<int>(theVars->RedoxReg_MP[index][0]) == 9) {
                RedoxRegCondition::setV9(RedoxReg_VMAX9 * theVars->RedoxReg_MP[index][2]);
            } else if (static_cast<int>(theVars->RedoxReg_MP[index][0]) == 13) {
                RedoxRegCondition::setV13(RedoxReg_VMAX13 * theVars->RedoxReg_MP[index][2]);
            } else if (static_cast<int>(theVars->RedoxReg_MP[index][0]) == 16) {
                RedoxRegCondition::setV16(RedoxReg_VMAX16 * theVars->RedoxReg_MP[index][2]);
            }
        }
        N_VDestroy(y);
        N_VDestroy(constraints);
        N_VDestroy(scaling);
        KINFree(&kmem);
        SUNLinSolFree(LS);
    }

    theVars->RedoxReg_Vel.Vred = RedoxReg_Con->RA_con->EPS_con->FIBF_con->BF_con->Fdn * Fd_Thio_ET * Thio / ThioT;
    theVars->RedoxReg_Vel.Vox = RedoxReg_Con->Thion * Thio_Oxidation;

#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->RedoxReg_Vel)
#endif
    if (theVars->record) {
        if (t > RedoxReg::TIME) {
            RedoxReg::N++;
            RedoxReg::TIME = t;
        }

        theVars->RedoxReg_VEL.insert(RedoxReg::N - 1, t, theVars->RedoxReg_Vel);
    }
}
