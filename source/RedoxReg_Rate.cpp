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
#include <kinsol/kinsol.h>             /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix       */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver */
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */

#define ONE    RCONST(1.0)
#define ZERO   RCONST(0.0)

void RedoxReg::RedoxReg_Rate(double t, RedoxRegCon &RedoxReg_Con, Variables *myVars) {
    const double Thio = myVars->ThioT - RedoxReg_Con.Thion;

    myVars->RedoxReg_MP[0][2] = RedoxReg_Con.Thion / myVars->ThioT;

    double TEMP = myVars->RedoxReg_MP[0][2];

    if (myVars->RROEA_EPS_com)
        TEMP = 0.5;

    if (myVars->trDynaPS2RedReg_cal == 1) {
        UserData *data = alloc_user_data();
        data->coeffs.resize(2);

        data->coeffs[0] = myVars->RedoxReg_MP[0][1] - 0.03 * log10(TEMP / (1 - TEMP));

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

            NV_Ith_S(y, 0) = myVars->RedoxReg_MP[index][2];
            data->coeffs[1] = myVars->RedoxReg_MP[index][1];
            KINSetUserData(kmem, data);
            KINSetConstraints(kmem, constraints);
            KINSetFuncNormTol(kmem, abstol);
            KINSetScaledStepTol(kmem, abstol);
            KINInit(kmem, RedoxReg_FPercent, y);
            KINSol(kmem, y, KIN_LINESEARCH, scaling, scaling);
            realtype *retvals = N_VGetArrayPointer(y);
            //pr = fsolve(@RedoxReg_FPercent, RedPercent, optimset('Display', 'off'), RedP, MPE);
            myVars->RedoxReg_MP[index][2] = retvals[0];


            if (myVars->RedoxReg_MP[index][0] == 6) {
                myVars->Redox2PS_V6 = RedoxReg_VMAX6 * myVars->RedoxReg_MP[index][2];
            } else if (myVars->RedoxReg_MP[index][0] == 9) {
                myVars->Redox2PS_V9 = RedoxReg_VMAX9 * myVars->RedoxReg_MP[index][2];
            } else if (myVars->RedoxReg_MP[index][0] == 13) {
                myVars->Redox2PS_V13 = RedoxReg_VMAX13 * myVars->RedoxReg_MP[index][2];
            } else if (myVars->RedoxReg_MP[index][0] == 16) {
                myVars->Redox2PS_V16 = RedoxReg_VMAX16 * myVars->RedoxReg_MP[index][2];
            }
        }
        N_VDestroy(y);
        N_VDestroy(constraints);
        N_VDestroy(scaling);
        KINFree(&kmem);
        SUNLinSolFree(LS);
    }

    myVars->RedoxReg_Vel.Vred = RedoxReg_Con.RA_con.EPS_con.FIBF_con.BF_con.Fdn * myVars->Fd_Thio_ET * Thio / myVars->ThioT;
    myVars->RedoxReg_Vel.Vox = RedoxReg_Con.Thion * myVars->Thio_Oxidation;

    if (myVars->RedoxReg_TIME_N == 0)
        myVars->RedoxReg_TIME_N = 1;

    if (t > myVars->RedoxReg_TIME_N) {
        myVars->RedoxReg_TIME_N = myVars->RedoxReg_TIME_N + 1;
        myVars->RedoxReg_OLD_TIME = t;
    }

    if (myVars->record)
        myVars->RedoxReg_VEL.insert(myVars->RedoxReg_TIME_N - 1, t, myVars->RedoxReg_Vel);
}
