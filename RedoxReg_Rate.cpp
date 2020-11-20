#include "globals.hpp"
#include <kinsol/kinsol.h>             /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix       */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver */
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */

#define ONE    RCONST(1.0)
#define ZERO   RCONST(0.0)

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

void RedoxReg_Rate(double t, arr &RedoxReg_Con, varptr *myVars) {
    //global RedoxReg_MP;
    
    //global RedoxReg_VMAX6;
    //global RedoxReg_VMAX9;
    //global RedoxReg_VMAX13;
    //global RedoxReg_VMAX16;
    
    //global BF2RedoxReg_Fdt;
    //global ThioT;
    
    //global Redox2PS_V6;
    //global Redox2PS_V9;
    //global Redox2PS_V13;
    //global Redox2PS_V16;
    
    const double Fdn = RedoxReg_Con[23];
    // Fd = myVars->BF2RedoxReg_Fdt - Fdn;// --unused
    
    const double Thion = RedoxReg_Con[92];
    const double Thio = myVars->ThioT - Thion;
    
    myVars->RedoxReg_MP[0][2] = Thion / myVars->ThioT;
    
    double TEMP = myVars->RedoxReg_MP[0][2];
    
    //global RROEA_EPS_com;
    if (myVars->RROEA_EPS_com)
        TEMP = 0.5;
    
    
    //global trDynaPS2RedReg_cal
    
    //double pr;
    if (myVars->trDynaPS2RedReg_cal == 1) {
        UserData *data = alloc_user_data();
        data->coeffs.resize(2);

        data->coeffs[0] = myVars->RedoxReg_MP[0][1] - 0.03 * log10(TEMP / (1 - TEMP));
        //double RedPercent, MPE;
        
        N_Vector y, constraints, scaling;
        y = N_VNew_Serial(1);
        constraints = N_VNew_Serial(1);
        scaling = N_VNew_Serial(1);
        N_VConst(ONE, scaling);
        realtype abstol = 1e-5;
        //realtype reltol = 1e-4;
        NV_Ith_S(constraints, 0) = ZERO;
        void *kmem = NULL;
        kmem = KINCreate();
        
        SUNMatrix A = SUNDenseMatrix(1, 1);
        SUNLinearSolver LS = SUNDenseLinearSolver(y, A);
        KINSetLinearSolver(kmem, LS, A);

        for (int index = 1; index < 5; index++) {
            
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
                myVars->Redox2PS_V6 = myVars->RedoxReg_VMAX6 * myVars->RedoxReg_MP[index][2];
                //myVars->Redox2PS_V6 = myVars->RedoxReg_VMAX6;
            } else if (myVars->RedoxReg_MP[index][0] == 9) {
                myVars->Redox2PS_V9 = myVars->RedoxReg_VMAX9 * myVars->RedoxReg_MP[index][2];
                //myVars->Redox2PS_V9 = myVars->RedoxReg_VMAX9;
            } else if (myVars->RedoxReg_MP[index][0] == 13) {
                myVars->Redox2PS_V13 = myVars->RedoxReg_VMAX13 * myVars->RedoxReg_MP[index][2];
                //myVars->Redox2PS_V13 = myVars->RedoxReg_VMAX13;
            } else if (myVars->RedoxReg_MP[index][0] == 16) {
                myVars->Redox2PS_V16 = myVars->RedoxReg_VMAX16 * myVars->RedoxReg_MP[index][2];
                //myVars->Redox2PS_V16 = myVars->RedoxReg_VMAX16;
            }
        }
        N_VDestroy(y);
        N_VDestroy(constraints);
        N_VDestroy(scaling);
        KINFree(&kmem);
        SUNLinSolFree(LS);
    }

    
    //global Thio_Oxidation;
    //global Fd_Thio_ET;
    
    const double Vred = Fdn * myVars->Fd_Thio_ET * Thio / myVars->ThioT;
    const double Vox = Thion * myVars->Thio_Oxidation;
    
    
    //arr RedoxReg_Vel = zeros(2);
    myVars->RedoxReg_Vel.Vred = Vred;
    myVars->RedoxReg_Vel.Vox = Vox;
    
    
    
    //global RedoxReg_OLD_TIME;
    //global RedoxReg_TIME_N;
    //global RedoxReg_VEL;
    //global RedoxReg_CON;
    
    if (myVars->RedoxReg_TIME_N == 0)
        myVars->RedoxReg_TIME_N = 1;
    
    
    if (t > myVars->RedoxReg_TIME_N) {
        myVars->RedoxReg_TIME_N = myVars->RedoxReg_TIME_N + 1;
        myVars->RedoxReg_OLD_TIME = t;
    }
    
    if (myVars->record)
        myVars->RedoxReg_VEL.insert(myVars->RedoxReg_TIME_N - 1, t, myVars->RedoxReg_Vel);
    //if (myVars->RedoxReg_VEL.shape()[1] < myVars->RedoxReg_TIME_N) {
    //    myVars->RedoxReg_VEL.resize(boost::extents[RedoxReg_VEL_SIZE][myVars->RedoxReg_TIME_N]);
    //}

    //myVars->RedoxReg_VEL[0][myVars->RedoxReg_TIME_N - 1] = t;  // --unused
    //myVars->RedoxReg_VEL[1][myVars->RedoxReg_TIME_N - 1] = Vred;  // --unused
    //myVars->RedoxReg_VEL[2][myVars->RedoxReg_TIME_N - 1] = Vox;  // --unused
    //return RedoxReg_Vel;
}
