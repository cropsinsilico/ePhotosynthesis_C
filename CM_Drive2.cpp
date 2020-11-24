#include "globals.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>
#include "CM.hpp"

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

varptr *CM::myVars = new varptr();
double CM::CM_Drive2(double pop, double currentPop) {
    
    //double Begin = 1;
    // fin = SysInitial(Begin);// --unused
    //global options1;
    //global tglobal;
    //const double time = myVars->tglobal;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Global variables used for obtaining flux and concentration data //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //global PS_PR_OLDTIME;
    //global PS_PR_TIME_N;
    //global PS_PR_VEL;
    
    myVars->PS_PR_OLDTIME = 0;
    myVars->PS_PR_TIME_N = 1;
    // PS_PR_VEL = zeros(27, 1);        // Store the flux value
    
    //global PS_OLD_TIME;
    //global PS_TIME_N;
    //global PS_VEL;
    myVars->PS_OLD_TIME = 0;
    myVars->PS_TIME_N = 0;
    // PS_VEL = zeros(1, 1);
    
    //global PR_OLD_TIME;
    //global PR_TIME_N;
    //global PR_VEL;
    myVars->PR_OLD_TIME = 0;
    myVars->PR_TIME_N = 1;
    // PR_VEL = zeros(1, 1);
    
    ////////////////////////////////////////////////
    //   Initialation step //
    ////////////////////////////////////////////////
    //Begin = 1;
    //arr PS_PRs = zeros(0);
    IniModelCom(myVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.
    //global PR_PS_com;    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    myVars->PR_PS_com = true;
    //global PSPR_SUCS_com;    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    myVars->PSPR_SUCS_com = true;
    
    //global ATPActive;
    myVars->ATPActive = 0;
    
    CMCon CM_con = CM_Ini();
    arr CMs = CM_con.toArray();
    ////////////////////////////////////////////////
    //   Calculation  step //
    ////////////////////////////////////////////////
    
    // CM_Param = 0;// --unused
    
    //global CO2A;
    // CO2A = zeros(5, 1);
    
    myVars->SUCS_Param[0] = 1;
    myVars->SUCS_Param[1] = 1;
    
    myVars->PS_PR_Param = 0;
    
    // suc = AssignVelocity(pop, currentPop);// --unused
    
//[Tt, d] = ode15s(@CM_mb, [0, time], CMs, options1, PS_PR_Param, SUCS_Param);

    UserData *data = alloc_user_data();
    int flag;
    realtype abstol = 1e-5;
    realtype reltol = 1e-4;
    sunindextype N =  CMs.size();
    N_Vector y;
    y = N_VNew_Serial(N);

    for (int i = 0; i < N; i++)
        NV_Ith_S(y, i) =  CMs[i];

    void *cvode_mem = NULL;
    cvode_mem = CVodeCreate(CV_BDF);
    realtype t0 = 0;
    flag = CVodeInit(cvode_mem, CM_mb, t0, y);

    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSetUserData(cvode_mem, data);

    SUNMatrix A = SUNDenseMatrix(N, N);


    SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

    flag = CVDlsSetLinearSolver(cvode_mem, LS, A);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    realtype tout;
    realtype end_time = 250;
    realtype step_length = 1.;
    realtype t = 0;

    for (tout = step_length; tout <= end_time; tout += step_length)
        flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);

    
    //global d_plot;
    //myVars->d_plot = d;
    
    //global Tt_plot;
    //myVars->Tt_plot = Tt;
    
    //////////////////////////////////////////////
    //   output  step      //
    //////////////////////////////////////////////
    
    // success = CM_Out(Tt, d);// --unused
    
    // Reinitialize some values of global variables.
    myVars->PSPR_SUCS_com = false;
    IniModelCom(myVars);
    
    const double CO2AR = TargetFunVal(myVars);

    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

                return CO2AR;
}
