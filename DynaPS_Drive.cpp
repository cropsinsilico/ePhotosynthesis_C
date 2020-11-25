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

    myVars->ATPActive = 0;

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
    sunindextype N =  DynaPS_Con.size();
    N_Vector y;
    y = N_VNew_Serial(N);

    for (size_t i = 0; i < N; i++)
        NV_Ith_S(y, i) =  DynaPS_Con[i];

    void *cvode_mem = NULL;
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

    /*
    const int row = myVars->RuACT_VEL.size() - 1;
    arr PSIIabs = zeros(FI_VEL_SIZE);
    for (int x = 0; x < FI_VEL_SIZE; x++)
        PSIIabs[x] = myVars->FI_VEL[x][56];

    arr PSIabs = zeros(BF_VEL_SIZE);
    for (int x = 0; x < BF_VEL_SIZE; x++)
        PSIabs[x] = myVars->BF_VEL[x][10];

    //PSIabs2=BF_VEL(:, 14)+BF_VEL(:, 16);
    arr CarbonRate = zeros(RuACT_VEL_SIZE);
    for (int x = 0; x < RuACT_VEL_SIZE; x++)
        CarbonRate[x] = myVars->RuACT_VEL[x][5] * myVars->AVR;

    arr VPR = zeros(RuACT_VEL_SIZE);
    for (int x = 0; x < RuACT_VEL_SIZE; x++)
        VPR[x] = myVars->RuACT_VEL[x][6] * myVars->AVR;

    //     CO2Release = PR_VEL(:,9) * AVR;
    //     Assim = CarbonRate - CO2Release;
    arr Vpgasink = zeros(SUCS_VEL_SIZE);
    for (int x = 0; x < SUCS_VEL_SIZE; x++)
        Vpgasink[x] = myVars->SUCS_VEL[x][14] * myVars->AVR;

    arr VStarch = zeros(PS_VEL_SIZE);
    for (int y = 0; y < PS_VEL_SIZE; y++)
        VStarch[y] = (myVars->PS_VEL[13][y] - myVars->PS_VEL[19][y]) * myVars->AVR;

    arr Vsucrose = zeros(SUCS_VEL_SIZE);
    for (int x = 0; x < SUCS_VEL_SIZE; x++)
        Vsucrose[x] = myVars->SUCS_VEL[x][10] * myVars->AVR;

    arr Resulta = zeros(7);
    //Resulta = [0;0;0;0;0;0;0];
    Resulta[0] = PSIIabs[row];
    Resulta[1] = PSIabs[row];
    //Resulta(3)=PSIabs2(row);
    Resulta[2] = CarbonRate[row];
    Resulta[3] = VPR[row];
    Resulta[4] = Vpgasink[row];
    Resulta[5] = Vsucrose[row];
    Resulta[6] = VStarch[row];
    //global FluxTR;
    // FluxTR = zeros(81, 1);
    myVars->FluxTR[0] = myVars->RuACT_VEL[row][5];//PS
    myVars->FluxTR[1] = myVars->RuACT_VEL[row][6];//PR
    myVars->FluxTR[2] = myVars->PS_VEL[2][row];// v2
    myVars->FluxTR[3] = myVars->PS_VEL[3][row];// v3
    myVars->FluxTR[4] = myVars->PS_VEL[5][row];// v5
    myVars->FluxTR[5] = myVars->PS_VEL[6][row];// v6
    myVars->FluxTR[6] = myVars->PS_VEL[7][row];// v7
    myVars->FluxTR[7] = myVars->PS_VEL[8][row];// v8
    myVars->FluxTR[8] = myVars->PS_VEL[9][row];// v9
    myVars->FluxTR[9] = myVars->PS_VEL[10][row];// v10
    myVars->FluxTR[10] = myVars->PS_VEL[11][row];// v13
    myVars->FluxTR[11] = myVars->PS_VEL[13][row];// v23
    myVars->FluxTR[12] = myVars->PS_VEL[18][row];// v24
    myVars->FluxTR[13] = myVars->PS_VEL[19][row];// v25
    myVars->FluxTR[14] = myVars->PR_VEL[row][2];//v112
    myVars->FluxTR[15] = myVars->PR_VEL[row][3];//v113
    myVars->FluxTR[16] = myVars->PR_VEL[row][4];//v121
    myVars->FluxTR[17] = myVars->PR_VEL[row][5];//v122
    myVars->FluxTR[18] = myVars->PR_VEL[row][6];//v123
    myVars->FluxTR[19] = myVars->PR_VEL[row][7];//v124
    myVars->FluxTR[20] = myVars->PR_VEL[row][8];//v131
    myVars->FluxTR[21] = myVars->PR_VEL[row][9];//vlin
    myVars->FluxTR[22] = myVars->PR_VEL[row][10];//v2out
    myVars->FluxTR[23] = myVars->SUCS_VEL[row][1];//v51	;//	DHAP+GAP --FBP
    myVars->FluxTR[24] = myVars->SUCS_VEL[row][2];//v52	;//	FBP --F6P + Pi
    myVars->FluxTR[25] = myVars->SUCS_VEL[row][3];//v55	;//	G1P+UTP --OPOP+UDPG
    myVars->FluxTR[26] = myVars->SUCS_VEL[row][4];//v56	;//	UDPG+F6P--SUCP + UDP
    myVars->FluxTR[27] = myVars->SUCS_VEL[row][5];//v57	;//	SUCP--Pi + SUC
    myVars->FluxTR[28] = myVars->SUCS_VEL[row][6];//v58	;//	F26BP--F6P + Pi
    myVars->FluxTR[29] = myVars->SUCS_VEL[row][7];//v59	;//	F6P + ATP --ADP + F26BP
    myVars->FluxTR[30] = myVars->SUCS_VEL[row][8];//v60	;//	ATP+UDP --UTP + ADP
    myVars->FluxTR[31] = myVars->SUCS_VEL[row][10];//v62	;//	SUC SINK
    myVars->FluxTR[32] = myVars->SUCS_VEL[row][11];//vdhap_in	;//	DHAP export from chloroplast
    myVars->FluxTR[33] = myVars->SUCS_VEL[row][12];//vgap_in	;//	GAP Export from chloroplast
    myVars->FluxTR[34] = myVars->SUCS_VEL[row][13];//vpga_in	;//	PGA export from chloroplast
    myVars->FluxTR[35] = myVars->SUCS_VEL[row][14];//vpga_use	;//	PGA utilisation in cytosol
    for (int x = 36; x < 66; x++) {
        for (int y = 1; y < 31; y++)
            myVars->FluxTR[x] = myVars->BF_VEL[row][y];

    }
    for (int x = 66; x < 124; x++) {
        for (int y = 1; y < 59; y++)
            myVars->FluxTR[x] = myVars->FI_VEL[row][y];

    }
    for (int x = 124; x < 131; x++) {
        for (int y = 1; y < 8; y++)
            myVars->FluxTR[x] = myVars->XanCycle_VEL[row][y];

    }
    //FluxTR(132:142)=RROEA_VEL(row,2:12);
    for (int x = 0; x < 36; x++)
        myVars->FluxTR[x] = myVars->FluxTR[x] * myVars->AVR;

    myVars->FluxTR[46] = myVars->FluxTR[46] * myVars->AVR;
    myVars->FluxTR[64] = myVars->FluxTR[64] * myVars->AVR / 2;
    // This is to set the regualtions to be as beginning.
    myVars->ATPActive = 0;
    myVars->BF_FI_com = false;
    myVars->PR_PS_com = false;
    myVars->FIBF_PSPR_com = false;
    myVars->RuACT_EPS_com = false;
    myVars->RedoxReg_RA_com = false;
    myVars->XanCycle_BF_com = false;
    // global BF_VEL;
    // global FI_VEL;
    // global PS_VEL;
    */
    arr Resulta;
    IniModelCom(myVars);
    //save FDC2

    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

    return Resulta;
}
