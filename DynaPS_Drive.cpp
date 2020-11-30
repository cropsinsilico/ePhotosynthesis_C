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
#include "DynaPS.hpp"
#include "globals.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>

Variables *DynaPS::myVars = new Variables();

arr DynaPS::DynaPS_Drive(size_t ParaNum, double Ratio) {
    // This part include the function to begin the simulation.

    // The time information is set in a global variable called tglobal in SYSInitial.
    if (ParaNum <= 103)
        myVars->PSRatio[ParaNum] = Ratio;

    if (ParaNum > 103 && ParaNum <= 169)
        myVars->SUCRatio[ParaNum - 103] = Ratio;

    if (ParaNum > 169 && ParaNum <= 217)
        myVars->PRRatio[ParaNum - 169] = Ratio;

    if (ParaNum > 217 && ParaNum <= 233)
        myVars->RacRatio[ParaNum - 217] = Ratio;

    if (ParaNum > 233 && ParaNum <= 256)
        myVars->FIRatio[ParaNum - 233] = Ratio;

    if (ParaNum > 256 && ParaNum <= 305)
        myVars->BFRatio[ParaNum - 256] = Ratio;

    if (ParaNum > 305 && ParaNum <= 309)
        myVars->XanRatio[ParaNum - 305] = Ratio;

    // This part include the function to begin the simulation.
    // The time information is set in a global variable called tglobal in SYSInitial.

    SYSInitial(myVars);

    ////////////////////////////////////////////////
    //   Calculation  step //
    ////////////////////////////////////////////////

    // Indicate in the beginning there is no ATP synthesis activity.
    myVars->EPS_ATP_Rate = 0;

    IniModelCom(myVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    myVars->BF_FI_com = true;

    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    myVars->PR_PS_com = true;

    // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    myVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    myVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    myVars->RedoxReg_RA_com = false;        // This means that the connection is not provided there.

    myVars->XanCycle_BF_com = true;

    myVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    myVars->PSPR_SUCS_com = true;

    myVars->SUCS_Param = zeros(2);

    // Next is to initialize the vector.

    DynaPSCon DynaPS_con = DynaPS_Ini();

    const double va1 = 0;
    // The ratio of the PSI unit to the PSII unit
    myVars->BF_Param[0] = va1;
    myVars->BF_Param[1] = myVars->PS12ratio;

    myVars->FI_Param[0] = va1;
    myVars->FI_Param[1] = myVars->PS12ratio;

    myVars->RuACT_Param[0] = va1;
    myVars->RuACT_Param[1] = myVars->PS12ratio;

    myVars->XanCycle_Param[0] = va1;
    myVars->XanCycle_Param[1] = myVars->PS12ratio;

    myVars->RedoxReg_Param = 0; // This parameter is just used here as a future storage tool. NOt used now.

    arr DynaPS_Con = zeros(120);
    arr temp = DynaPS_con.toArray();
    for (size_t i = 0; i < DynaPS_con.size(); i++)
        DynaPS_Con[i] = temp[i];
    UserData *data = alloc_user_data();
    int flag;
    realtype abstol = 1e-5;
    realtype reltol = 1e-4;
    sunindextype N =  static_cast<long>(DynaPS_Con.size());
    N_Vector y;
    y = N_VNew_Serial(N);

    for (size_t i = 0; i < DynaPS_Con.size(); i++)
        NV_Ith_S(y, i) =  DynaPS_Con[i];

    void *cvode_mem = nullptr;
    cvode_mem = CVodeCreate(CV_BDF);
    realtype t0 = 0;
    flag = CVodeInit(cvode_mem, DynaPS_mb, t0, y);

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


    double PSIIabs = myVars->FI_Vel.vP680_d;
    double PSIabs = myVars->BF_Vel.Vbf11;
    double CarbonRate = myVars->RuACT_Vel.v6_1 * myVars->AVR;
    double VPR = myVars->RuACT_Vel.v6_2 * myVars->AVR;
    double Vpgasink = myVars->SUCS_Vel.vpga_use * myVars->AVR;
    double VStarch = (myVars->PS_Vel.v23 - myVars->PS_Vel.v25) * myVars->AVR;
    double Vsucrose = myVars->SUCS_Vel.vdhap_in * myVars->AVR;

    arr Resulta = zeros(7);
    //Resulta = [0;0;0;0;0;0;0];
    Resulta[0] = PSIIabs;
    Resulta[1] = PSIabs;
    //Resulta(3)=PSIabs2(row);
    Resulta[2] = CarbonRate;
    Resulta[3] = VPR;
    Resulta[4] = Vpgasink;
    Resulta[5] = Vsucrose;
    Resulta[6] = VStarch;

    if(myVars->record) {
        makeFluxTR(myVars);
    }
    // This is to set the regualtions to be as beginning.
    //myVars->ATPActive = 0;
    myVars->BF_FI_com = false;
    myVars->PR_PS_com = false;
    myVars->FIBF_PSPR_com = false;
    myVars->RuACT_EPS_com = false;
    myVars->RedoxReg_RA_com = false;
    myVars->XanCycle_BF_com = false;

    IniModelCom(myVars);
    //save FDC2

    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

    return Resulta;
}
