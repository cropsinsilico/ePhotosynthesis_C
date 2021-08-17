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

#include "drivers/driver.hpp"
#include "Variables.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <sunnonlinsol/sunnonlinsol_newton.h>
#include <cvode/cvode_direct.h>
#include "drivers/CVodeMem.hpp"

bool Driver::showWarnings = false;

arr Driver::run() {
    origVars = new Variables(theVars);
    uint count = 0;
    while (count < 10){
        maxStep = 20. * step;

        setup();

        sunindextype N =  static_cast<long>(constraints.size());
        N_Vector y;
        y = N_VNew_Serial(N);

        for (size_t i = 0; i < constraints.size(); i++)
            NV_Ith_S(y, i) =  constraints[i];
        realtype t0 = 0;

        CVodeMem *cmem = &CVodeMem::create();
        cmem->cvode_mem_init(this, t0, y);

        data->drv = this;

        SUNMatrix A = SUNDenseMatrix(N, N);
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y);
        SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

        if (CVodeSetNonlinearSolver(cvode_mem, NLS) != CV_SUCCESS) {
            std::cout << "CVodeSetNonlinearSolver failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (CVDlsSetLinearSolver(cvode_mem, LS, A) != CV_SUCCESS) {
            std::cout << "CVDlsSetLinearSolver failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        realtype t = 0;
        bool runOK = true;
        realtype tout = start + step;
        while (tout <= endtime) {
        //for (realtype tout = start + step; tout <= endtime; tout += step)
            if (CVode(cvode_mem, tout, y, &t, CV_NORMAL) != CV_SUCCESS) {
                std::cout << "CVode failed at t=" << tout << std::endl;
                //exit(EXIT_FAILURE);
                runOK = false;
                break;
            }
            tout += step;
        }
        if (runOK) {
            intermediateRes = N_VGetArrayPointer(y);
            time = t;
            getResults();
        }

        SUNNonlinSolFree(NLS);
        SUNLinSolFree(LS);
        SUNMatDestroy(A);
        N_VDestroy(y);
        if (runOK)
            return results;

        theVars = origVars;
        count++;
        step = initialStep / (count + 1);
        std::cout << "Retrying with smaller step size: " << step << std::endl;
    }
    throw std::runtime_error("No valid solution found");
}

Driver::~Driver() {
    if (origVars != nullptr)
        delete origVars;
}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    CalcData *data = static_cast<CalcData*>(user_data);
    arr ddxdt = data->drv->MB(t, u);
    for (size_t index = 0; index < ddxdt.size(); index++)
        dxdt[index] = ddxdt[index];
    return 0;
}
