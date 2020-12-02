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
#include "globals.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>
#include "trDynaPS.hpp"

Variables *trDynaPS::theVars = nullptr;

arr trDynaPS::trDynaPS_Drive(size_t ParaNum, double Ratio) {

    if (ParaNum <= 103) {
        theVars->PSRatio[ParaNum] = Ratio;
    } else if (ParaNum <= 169) {
        theVars->SUCRatio[ParaNum - 103] = Ratio;
    } else if (ParaNum <= 217) {
        theVars->PRRatio[ParaNum - 169] = Ratio;
    } else if (ParaNum <= 233) {
        theVars->RacRatio[ParaNum - 217] = Ratio;
    } else if (ParaNum <= 256) {
        theVars->FIRatio[ParaNum - 233] = Ratio;
    } else if (ParaNum <= 305) {
        theVars->BFRatio[ParaNum - 256] = Ratio;
    } else if (ParaNum <= 309) {
        theVars->XanRatio[ParaNum - 305] = Ratio;
    }
    SYSInitial(theVars);

    // Indicate in the beginning there is no ATP synthesis activity.
    theVars->EPS_ATP_Rate = 0;

    IniModelCom(theVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    theVars->BF_FI_com = true;

    // true means that the overall EPS model is used. false means partial model of FIBF is used.
    theVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    theVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    theVars->RedoxReg_RA_com = false;        // This means that the connection is there.

    theVars->XanCycle_BF_com = true;

    theVars->RROEA_EPS_com = true;

    theVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    theVars->PSPR_SUCS_com = true;

    // Next is to initialize the vector.
    trDynaPSCon trDynaPS_con = trDynaPS_Ini();
    ParamSet(theVars);
    arr trDynaPS_Con = trDynaPS_con.toArray();

    UserData *data = alloc_user_data();
    int flag;
    realtype abstol = 1e-5;
    realtype reltol = 1e-4;
    sunindextype N =  static_cast<long>(trDynaPS_Con.size());
    N_Vector y;
    y = N_VNew_Serial(N);

    for (size_t i = 0; i < trDynaPS_Con.size(); i++)
        NV_Ith_S(y, i) =  trDynaPS_Con[i];

    void *cvode_mem = nullptr;
    cvode_mem = CVodeCreate(CV_BDF);
    realtype t0 = 0;
    flag = CVodeInit(cvode_mem, trDynaPS_mb, t0, y);

    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;
    flag = CVodeSetUserData(cvode_mem, data);
    flag = CVodeSetMaxNumSteps(cvode_mem, 750);

    SUNMatrix A = SUNDenseMatrix(N, N);

    SUNLinearSolver LS = SUNLinSol_Dense(y, A);

    flag = CVDlsSetLinearSolver(cvode_mem, LS, A);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;

    realtype end_time = 250;
    realtype step_length = 1.;
    realtype t = 0;

    for (realtype tout = step_length; tout <= end_time; tout += step_length)
        flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);

    // call the functions one last time to get the correct values we need
    realtype *results = N_VGetArrayPointer(y);
    trDynaPSCon trDynaPS_res(results);
    arr temp = trDynaPS_Mb(t, trDynaPS_res, theVars);
    delete data;
    double CarbonRate = theVars->RuACT_Vel.v6_1 * theVars->AVR;
    double VPR = theVars->RuACT_Vel.v6_2 * theVars->AVR;
    double Vpgasink = theVars->SUCS_Vel.vpga_use * theVars->AVR;
    double VStarch = (theVars->PS_Vel.v23 - theVars->PS_Vel.v25) * theVars->AVR;
    double Vt3p = (theVars->PS_Vel.v31 + theVars->PS_Vel.v33) * theVars->AVR;
    arr Resulta = zeros(7);

    Resulta[0] = CarbonRate; //Vc
    Resulta[1] = VPR;        //Vo
    Resulta[2] = Vpgasink;   //PGA
    Resulta[3] = Vt3p;       //VT3P
    Resulta[4] = VStarch;    //Vstarch
    Resulta[5] = theVars->PR_Vel.v1in * theVars->AVR;  //Vt_glycerate
    Resulta[6] = theVars->PR_Vel.v2out * theVars->AVR; //Vt_glycolate

    if(theVars->record) {
        makeFluxTR(theVars);
    }

    IniModelCom(theVars);
    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);
    SUNMatDestroy(A);
    return Resulta;
}
