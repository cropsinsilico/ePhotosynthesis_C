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

#include "driver.hpp"
#include "Variables.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>
#include "CVodeMem.hpp"


arr Driver::run() {
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

    SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

    if (CVDlsSetLinearSolver(cvode_mem, LS, A) != 0) {
        std::cout << "CVDlsSetLinearSolver failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    realtype t = 0;

    for (realtype tout = start + step; tout <= endtime; tout += step)
        if (CVode(cvode_mem, tout, y, &t, CV_NORMAL) != 0) {
            std::cout << "CVode failed at t=" << tout << std::endl;
            exit(EXIT_FAILURE);
        }

    intermediateRes = N_VGetArrayPointer(y);
    time = t;
    getResults();

    SUNLinSolFree(LS);
    SUNMatDestroy(A);
    N_VDestroy(y);
    return results;
}

Driver::~Driver() {

}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    CalcData *data = static_cast<CalcData*>(user_data);
    arr ddxdt = data->drv->MB(t, u);
    uint adjust = 0;
    if (theVars->useC3)
        adjust = 1;
    for (size_t index = 0; index < ddxdt.size() - adjust; index++)
        dxdt[index] = ddxdt[index];
    return 0;
}
