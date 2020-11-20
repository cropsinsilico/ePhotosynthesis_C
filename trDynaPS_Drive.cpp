#include "globals.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis
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
varptr *trDynaPS::myVars = new varptr();

arr trDynaPS::trDynaPS_Drive(double ParaNum, double Ratio) {
    //global TestCa;
    //global TestLi;
    //global TestSucPath;
    //global TestATPCost;
    //myVars->TestCa = Cai;
    //myVars->TestLi = Lii;
    //myVars->TestSucPath = SucPath;
    //myVars->TestATPCost = ATPCost;
    // trDynaPS_Drive.m
    // This part include the function to begin the simulation.
    
    // The time information is set in a global variable called tglobal in SYSInitial.
    //global PSRatio;
    // PSRatio = ones(103, 1);
    if (ParaNum <= 103)
        myVars->PSRatio[ParaNum] = Ratio;
    
    //global SUCRatio;
    // SUCRatio = ones(66, 1);
    if (ParaNum > 103&&ParaNum <= 169)
        myVars->SUCRatio[ParaNum - 103] = Ratio;
    
    //global PRRatio;
    // PRRatio = ones(48, 1);
    if (ParaNum > 169&&ParaNum <= 217)
        myVars->PRRatio[ParaNum - 169] = Ratio;
    
    
    //global RacRatio;
    // RacRatio = ones(16, 1);
    if (ParaNum > 217&&ParaNum <= 233)
        myVars->RacRatio[ParaNum - 217] = Ratio;
    
    
    //global FIRatio;
    // FIRatio = ones(23, 1);
    if (ParaNum > 233&&ParaNum <= 256)
        myVars->FIRatio[ParaNum - 233] = Ratio;
    
    
    //global BFRatio;
    // BFRatio = ones(49, 1);
    if (ParaNum > 256&&ParaNum <= 305)
        myVars->BFRatio[ParaNum - 256] = Ratio;
    
    
    
    //global XanRatio;
    // XanRatio = ones(4, 1);
    if (ParaNum > 305&&ParaNum <= 309)
        myVars->XanRatio[ParaNum - 305] = Ratio;
    
    
    //clear;
    // time1 = clock;// --unused
    PreProcess(myVars);
    
    // Begin = 1;// --unused
    SYSInitial(myVars);
    //global options1;
    //global tglobal;
    //const double time = myVars->tglobal;
    
    
    //global ATPActive;
    myVars->ATPActive = 0;
    
    //global EPS_ATP_Rate;        // Indicate in the beginning there is no ATP synthesis activity.
    myVars->EPS_ATP_Rate = 0;
    
    IniModelCom(myVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.
    
    //global BF_FI_com;            // The combination of BF and FI model
    myVars->BF_FI_com = true;
    
    //global PR_PS_com;    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    // PR_PS_Com = 1;// --unused
    
    //global FIBF_PSPR_com; // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    myVars->FIBF_PSPR_com = true;
    
    //global RuACT_EPS_com;     // A global variable to indicate whether the RuACT is run by itself or combined with others.
    myVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.
    
    //global RedoxReg_RA_com;     // This is the connection between Redox and RA.
    myVars->RedoxReg_RA_com = false;        // This means that the connection is there.
    
    //global XanCycle_BF_com;
    myVars->XanCycle_BF_com = true;
    
    //global RROEA_EPS_com;
    myVars->RROEA_EPS_com = true;
    
    //global EPS_SUCS_com;
    myVars->EPS_SUCS_com = true;
    
    //global PSPR_SUCS_com;    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    myVars->PSPR_SUCS_com = true;
    
    //global CO2A;
    // CO2A = zeros(5, 1);
    
    // Next is to initialize the vector.
    //global d;
    arr trDynaPS_Con = trDynaPS_Ini();
    //fprintf("//f   //f", trDynaPS_Con(12), trDynaPS_Con(13));
    //fprintf("\n");
    ParamSet(myVars);
    
    //[FI_Param, BF_Param, PS_PR_Param, SUCS_Param, EPS_Param, RuACT_Param, XanCycle_Param, RROEA_Param, RedoxReg_Param] = ParamSet(myVars);
    
//[Tt, d] = ode15s(@trDynaPS_mb, [0, time], trDynaPS_Con, options1, BF_Param, FI_Param, PS_PR_Param, SUCS_Param, RuACT_Param, RedoxReg_Param, XanCycle_Param, RROEA_Param);

    UserData *data = alloc_user_data();
    int flag;
    realtype abstol = 1e-5;
    realtype reltol = 1e-4;
    sunindextype N =  trDynaPS_Con.size();
    N_Vector y;
    y = N_VNew_Serial(N);

    for (int i = 0; i < N; i++)
        NV_Ith_S(y, i) =  trDynaPS_Con[i];

    void *cvode_mem = NULL;
    cvode_mem = CVodeCreate(CV_BDF);
    realtype t0 = 0;
    flag = CVodeInit(cvode_mem, trDynaPS_mb, t0, y);

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
    //realtype tout;
    realtype end_time = 250;
    realtype step_length = 1.;
    realtype t = 0;

    for (realtype tout = step_length; tout <= end_time; tout += step_length)
        flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);

    //dff = diff(Tt);
    //fids = fopen("timesteps.txt", 'w');
    //fprintf(fids, "//f   ", dff);
    //fclose(fids);
    //fileid1 = fopen("times.txt", 'w');
    //fileid2 = fopen("outdata.txt", 'w');
    //fprintf(fileid1, '//f  ', Tt);
    //fprintf(fileid2, '//f  ', d);
    //fclose(fileid1);
    //fclose(fileid2);
    myVars->ATPActive = 0;
    //global BF_VEL;
    //global FI_VEL;
    //global BF_CON;
    //global PS_VEL;
    //global PR_VEL;
    //global FI_CON;
    //global PS_CON;
    //global PR_CON;
    //global SUCS_VEL;
    //global RuACT_VEL;
    //global XanCycle_VEL;
    //global RedoxReg_VEL;
    //global RROEA_VEL;
    //global AVR;
    const size_t row = myVars->RuACT_VEL.size();
    
    //arr PSIIabs = zeros(FI_VEL_SIZE);
    //for (int x = 0; x < FI_VEL_SIZE; x++)
    //    PSIIabs[x] = myVars.FI_VEL[x][56];
    
    //arr PSIabs = zeros(BF_VEL_SIZE);
    //for (int x = 0; x < BF_VEL_SIZE; x++)
    //    PSIabs[x] = myVars.BF_VEL[x][10];
    
    //PSIabs2=BF_VEL(:, 14)+BF_VEL(:, 16);
    
    //arr CarbonRate = zeros(row);
    //arr VPR = zeros(row);
    double CarbonRate = myVars->RuACT_Vel.v6_1 * myVars->AVR;
    double VPR = myVars->RuACT_Vel.v6_2 * myVars->AVR;
    //arr temp = zeros(RuACT_VEL_SIZE);
    //for (int x = 0; x < row; x++) {
    //    CarbonRate[x] = myVars->RuACT_VEL[x][5] * myVars->AVR;
    //    VPR[x] = myVars->RuACT_VEL[x][6] * myVars->AVR;
    //}
    
    //const double VPR;
    //for (int x = 0; x < RuACT_VEL_SIZE1; x++)
    //    VPR[x] = RuACT_VEL[x][6] * AVR;
    
    //     CO2Release = PR_VEL(:,9) * AVR;
    //     Assim = CarbonRate - CO2Release;
    double Vpgasink = myVars->SUCS_Vel[13] * myVars->AVR;
    //arr Vpgasink = zeros(myVars->SUCS_VEL.shape()[0]);
    //for (int x = 0; x < row; x++)
    //    Vpgasink[x] = myVars->SUCS_VEL[x][14] * myVars->AVR;
    
    
    //arr VStarch = zeros(myVars->PS_VEL.shape()[1]);
    //arr Vt3p = zeros(myVars->PS_VEL.shape()[1]);
    double VStarch = (myVars->PS_Vel.v23 - myVars->PS_Vel.v25) * myVars->AVR;
    double Vt3p = (myVars->PS_Vel.v31 + myVars->PS_Vel.v33) * myVars->AVR;
    //for (int y = 0; y < row; y++) {
    //    VStarch[y] = (myVars->PS_VEL[13][y] - myVars->PS_VEL[19][y]) * myVars->AVR;
    //    Vt3p[y] = (myVars->PS_VEL[14][y] + myVars->PS_VEL[16][y]) * myVars->AVR;
    //}
    //Vsucrose=SUCS_VEL(:,11)*AVR;
    
    // Vt3p=(SUCS_VEL(:,2))'*AVR*2;//+SUCS_VEL(:,13)
    arr Resulta = zeros(7);
    //     Resulta(1)=PSIIabs(row);
    //     Resulta(2)=PSIabs(row);
    //Resulta(3)=PSIabs2(row);
    
    Resulta[0] = CarbonRate;//[row - 1];//Vc
    Resulta[1] = VPR;//[row - 1];//Vo
    Resulta[2] = Vpgasink;//[row - 1];//PGA
    Resulta[3] = Vt3p;//[row - 1];//VT3P
    Resulta[4] = VStarch;//[row - 1];//Vstarch
    Resulta[5] = myVars->PR_Vel.v1in * myVars->AVR;//Vt_glycerate
    Resulta[6] = myVars->PR_Vel.v2out * myVars->AVR;//Vt_glycolate
    //global FluxTR;
    // FluxTR = zeros(81, 1);
    /*
    myVars->FluxTR[0] = myVars->RuACT_VEL[row - 1][5];//PS
    myVars->FluxTR[1] = myVars->RuACT_VEL[row - 1][6];//PR
    myVars->FluxTR[2] = myVars->PS_VEL[2][row - 1];// v2
    myVars->FluxTR[3] = myVars->PS_VEL[3][row - 1];// v3
    myVars->FluxTR[4] = myVars->PS_VEL[5][row - 1];// v5
    myVars->FluxTR[5] = myVars->PS_VEL[6][row - 1];// v6
    myVars->FluxTR[6] = myVars->PS_VEL[7][row - 1];// v7
    myVars->FluxTR[7] = myVars->PS_VEL[8][row - 1];// v8
    myVars->FluxTR[8] = myVars->PS_VEL[9][row - 1];// v9
    myVars->FluxTR[9] = myVars->PS_VEL[10][row - 1];// v10
    myVars->FluxTR[10] = myVars->PS_VEL[11][row - 1];// v13
    myVars->FluxTR[11] = myVars->PS_VEL[13][row - 1];// v23
    myVars->FluxTR[12] = myVars->PS_VEL[18][row - 1];// v24
    myVars->FluxTR[13] = myVars->PS_VEL[19][row - 1];// v25
    myVars->FluxTR[14] = myVars->PR_VEL[row - 1][2];//v112
    myVars->FluxTR[15] = myVars->PR_VEL[row - 1][3];//v113
    myVars->FluxTR[16] = myVars->PR_VEL[row - 1][4];//v121
    myVars->FluxTR[17] = myVars->PR_VEL[row - 1][5];//v122
    myVars->FluxTR[18] = myVars->PR_VEL[row - 1][6];//v123
    myVars->FluxTR[19] = myVars->PR_VEL[row - 1][7];//v124
    myVars->FluxTR[20] = myVars->PR_VEL[row - 1][8];//v131
    myVars->FluxTR[21] = myVars->PR_VEL[row - 1][9];//vlin
    myVars->FluxTR[22] = myVars->PR_VEL[row - 1][10];//v2out
    myVars->FluxTR[23] = myVars->SUCS_VEL[row - 1][1];//v51	;//	DHAP+GAP --FBP
    myVars->FluxTR[24] = myVars->SUCS_VEL[row - 1][2];//v52	;//	FBP --F6P + Pi
    myVars->FluxTR[25] = myVars->SUCS_VEL[row - 1][3];//v55	;//	G1P+UTP --OPOP+UDPG
    myVars->FluxTR[26] = myVars->SUCS_VEL[row - 1][4];//v56	;//	UDPG+F6P--SUCP + UDP
    myVars->FluxTR[27] = myVars->SUCS_VEL[row - 1][5];//v57	;//	SUCP--Pi + SUC
    myVars->FluxTR[28] = myVars->SUCS_VEL[row - 1][6];//v58	;//	F26BP--F6P + Pi
    myVars->FluxTR[29] = myVars->SUCS_VEL[row - 1][7];//v59	;//	F6P + ATP --ADP + F26BP
    myVars->FluxTR[30] = myVars->SUCS_VEL[row - 1][8];//v60	;//	ATP+UDP --UTP + ADP
    myVars->FluxTR[31] = myVars->SUCS_VEL[row - 1][10];//v62	;//	SUC SINK
    myVars->FluxTR[32] = myVars->SUCS_VEL[row - 1][11];//vdhap_in	;//	DHAP export from chloroplast
    myVars->FluxTR[33] = myVars->SUCS_VEL[row - 1][12];//vgap_in	;//	GAP Export from chloroplast
    myVars->FluxTR[34] = myVars->SUCS_VEL[row - 1][13];//vpga_in	;//	PGA export from chloroplast
    myVars->FluxTR[35] = myVars->SUCS_VEL[row - 1][14];//vpga_use	;//	PGA utilisation in cytosol
    
    std::cout << "BF " << myVars->BF_VEL.shape()[0] << "," << myVars->BF_VEL.shape()[1] << std::endl;
    std::cout << "FI " << myVars->FI_VEL.shape()[0] << "," << myVars->FI_VEL.shape()[1] << std::endl;
    std::cout << "Xa " << myVars->XanCycle_VEL.shape()[0] << "," << myVars->XanCycle_VEL.shape()[1] << std::endl;
    std::cout << "RR " << myVars->RROEA_VEL.shape()[0] << "," << myVars->RROEA_VEL.shape()[1] << std::endl;
    std::cout << "Fl " << myVars->FluxTR.size() << std::endl;
    std::cout << "Row " << row << std::endl; 
    
    for (int x = 36; x < 66; x++)
        for (int y = 1; y < 31; y++)
            myVars->FluxTR[x] = myVars->BF_VEL[row - 1][y];
        
    for (int x = 66; x < 124; x++)
        for (int y = 1; y < 59; y++)
            myVars->FluxTR[x] = myVars->FI_VEL[row - 1][y];
        
    for (int x = 124; x < 131; x++)
        for (int y = 1; y < 8; y++)
            myVars->FluxTR[x] = myVars->XanCycle_VEL[row - 1][y];
        
    for (int x = 131; x < 142; x++)
        for (int y = 1; y < 12; y++)
            myVars->FluxTR[x] = myVars->RROEA_VEL[row - 1][y];
        
    for (int x = 0; x < 36; x++)
        myVars->FluxTR[x] = myVars->FluxTR[x] * myVars->AVR;
    
    myVars->FluxTR[46] = myVars->FluxTR[46] * myVars->AVR;
    myVars->FluxTR[64] = myVars->FluxTR[64] * myVars->AVR / 2;
    */
    
    //FluxTR(132:133)=RedoxReg_VEL(row,2:3);
    //RedoxReg_VEL(2,RedoxReg_TIME_N) = Vred;
    //RedoxReg_VEL(3,RedoxReg_TIME_N) = Vox;
    // RROEA_VEL	(	RROEA_TIME_N	,   2	)	=	ve2GAPDH	;
    // RROEA_VEL	(	RROEA_TIME_N	,   3	)	=	ve2FBPase	;
    // RROEA_VEL	(	RROEA_TIME_N	,   4	)	=	ve2SBPase	;
    // RROEA_VEL	(	RROEA_TIME_N	,   5	)	=	ve2PRK	;
    // RROEA_VEL	(	RROEA_TIME_N	,   6	)	=	ve2ATPase	;
    // RROEA_VEL	(	RROEA_TIME_N	,   7	)	=	ve2ATPGPP	;
    // RROEA_VEL	(	RROEA_TIME_N	,   8	)	=	ve2MDH	;
    // RROEA_VEL	(	RROEA_TIME_N	,   9	)	=	ve2Fd	;
    // RROEA_VEL	(	RROEA_TIME_N	,   10	)	=	veFd2Thio	;
    // RROEA_VEL	(	RROEA_TIME_N	,   11	)	=	veFd2Calvin	;
    // RROEA_VEL	(	RROEA_TIME_N	,   12	)	=	ve2RuACT	;
    
    // FI_VEL	(	FI_TIME_N	,	2	)	=	vA_d;
    // FI_VEL	(	FI_TIME_N	,	3	)	=	vA_f;
    // FI_VEL	(	FI_TIME_N	,	4	)	=	vA_U;
    // FI_VEL	(	FI_TIME_N	,	5	)	=	vU_A;
    // FI_VEL	(	FI_TIME_N	,	6	)	=	vU_f;
    // FI_VEL	(	FI_TIME_N	,	7	)	=	vU_d;
    // FI_VEL	(	FI_TIME_N	,	8	)	=	v1;
    // FI_VEL	(	FI_TIME_N	,	9	)	=	v_r1;
    // FI_VEL	(	FI_TIME_N	,	10	)	=	vS1_S2;
    // FI_VEL	(	FI_TIME_N	,	11	)	=	vS2_S3;
    // FI_VEL	(	FI_TIME_N	,	12	)	=	vS3_S0;
    // FI_VEL	(	FI_TIME_N	,	13	)	=	vS0_S1;
    // FI_VEL	(	FI_TIME_N	,	14	)	=	vz_1;
    // FI_VEL	(	FI_TIME_N	,	15	)	=	v1z_1;
    // FI_VEL	(	FI_TIME_N	,	16	)	=	v2z_1;
    // FI_VEL	(	FI_TIME_N	,	17	)	=	v3z_1;
    // FI_VEL	(	FI_TIME_N	,	18	)	=	v0z_1;
    // FI_VEL	(	FI_TIME_N	,	19	)	=	vz_2;
    // FI_VEL	(	FI_TIME_N	,	20	)	=	v1z_2;
    // FI_VEL	(	FI_TIME_N	,	21	)	=	v2z_2;
    // FI_VEL	(	FI_TIME_N	,	22	)	=	v3z_2;
    // FI_VEL	(	FI_TIME_N	,	23	)	=	v0z_2;
    // FI_VEL	(	FI_TIME_N	,	24	)	=	v1z;
    // FI_VEL	(	FI_TIME_N	,	25	)	=	v2z;
    // FI_VEL	(	FI_TIME_N	,	26	)	=	v3z;
    // FI_VEL	(	FI_TIME_N	,	27	)	=	v0z;
    // FI_VEL	(	FI_TIME_N	,	28	)	=	vAB1;
    // FI_VEL	(	FI_TIME_N	,	29	)	=	vBA1;
    // FI_VEL	(	FI_TIME_N	,	30	)	=	vAB2;
    // FI_VEL	(	FI_TIME_N	,	31	)	=	vBA2;
    // FI_VEL	(	FI_TIME_N	,	32	)	=	v3;
    // FI_VEL	(	FI_TIME_N	,	33	)	=	v_r3;
    // FI_VEL	(	FI_TIME_N	,	34	)	=	v3_n;
    // FI_VEL	(	FI_TIME_N	,	35	)	=	v_r3_n;
    // FI_VEL	(	FI_TIME_N	,	36	)	=	v_pq_ox;
    // FI_VEL	(	FI_TIME_N	,	37	)	=	Ic ;
    // FI_VEL	(	FI_TIME_N	,	38	)	=	Ia;
    // FI_VEL	(	FI_TIME_N	,	39	)	=	v2_1;
    // FI_VEL	(	FI_TIME_N	,	40	)	=	v2_2;
    // FI_VEL	(	FI_TIME_N	,	41	)	=	v2_00_1;
    // FI_VEL	(	FI_TIME_N	,	42	)	=	v2_01_1;
    // FI_VEL	(	FI_TIME_N	,	43	)	=	v2_02_1;
    // FI_VEL	(	FI_TIME_N	,	44	)	=	v2_00_2;
    // FI_VEL	(	FI_TIME_N	,	45	)	=	v2_01_2;
    // FI_VEL	(	FI_TIME_N	,	46	)	=	v2_02_2;
    // FI_VEL	(	FI_TIME_N	,	47	)	=	vr2_00_1;
    // FI_VEL	(	FI_TIME_N	,	48	)	=	vr2_01_1;
    // FI_VEL	(	FI_TIME_N	,	49	)	=	vr2_02_1;
    // FI_VEL	(	FI_TIME_N	,	50	)	=	vr2_1;
    // FI_VEL	(	FI_TIME_N	,	51	)	=	vr2_00_2;
    // FI_VEL	(	FI_TIME_N	,	52	)	=	vr2_01_2;
    // FI_VEL	(	FI_TIME_N	,	53	)	=	vr2_02_2;
    // FI_VEL	(	FI_TIME_N	,	54	)	=	vr2_2;
    // FI_VEL	(	FI_TIME_N	,	55	)		=	vP680qU	;	//	vr2_2
    // FI_VEL	(	FI_TIME_N	,	56	)	=	vP680qA	;	//	vr2_2
    // FI_VEL	(	FI_TIME_N	,	57	)	 =   vU_P680;
    // FI_VEL	(	FI_TIME_N	,	58	)	  =   vP680_d;
    // FI_VEL	(	FI_TIME_N	,	59	)	   =   vP680_f;
    // BF_VEL	(	BF_TIME_N	,	2	)	=	Vbf1	;
    // BF_VEL	(	BF_TIME_N	,	3	)	=	Vbf2	;
    // BF_VEL	(	BF_TIME_N	,	4	)	=	Vbf3	;
    // BF_VEL	(	BF_TIME_N	,	5	)	=	Vbf4	;
    // BF_VEL	(	BF_TIME_N	,	6	)	=	Vbf5	;
    // BF_VEL	(	BF_TIME_N	,	7	)	=	Vbf6	;
    // BF_VEL	(	BF_TIME_N	,	8	)	=	Vbf7	;
    // BF_VEL	(	BF_TIME_N	,	9	)	=	Vbf8	;
    // BF_VEL	(	BF_TIME_N	,	10	)	=	Vbf9	;
    // BF_VEL	(	BF_TIME_N	,	11	)	=	Vbf10	;
    // BF_VEL	(	BF_TIME_N	,	12	)	=	Vbf11	;
    // BF_VEL	(	BF_TIME_N	,	13	)	=	Vqi	    ;
    // BF_VEL	(	BF_TIME_N	,	14	)	=	Vipc	;
    // BF_VEL	(	BF_TIME_N	,	15	)	=	Vicp	;
    // BF_VEL	(	BF_TIME_N	,	16	)	=	Vinc	;
    // BF_VEL	(	BF_TIME_N	,	17	)	=	Vinp	;
    // BF_VEL	(	BF_TIME_N	,	18	)	=	Vdp	    ;
    // BF_VEL	(	BF_TIME_N	,	19	)	=	Vdc	    ;
    // BF_VEL	(	BF_TIME_N	,	20	)	=	Vfp	    ;
    // BF_VEL	(	BF_TIME_N	,	21	)	=	Vfc	    ;
    // BF_VEL	(	BF_TIME_N	,	22	)	=	Vsfd	;
    // BF_VEL	(	BF_TIME_N	,	23	)	=	VsATP	;
    // BF_VEL	(	BF_TIME_N	,	24	)	=	VgPQH2	;
    // BF_VEL	(	BF_TIME_N	,	25	)	=	Vbf12	;
    // BF_VEL	(	BF_TIME_N	,	26	)	=	Vbf13	;
    // BF_VEL	(	BF_TIME_N	,	27	)	=	Vbf14	;
    // BF_VEL	(	BF_TIME_N	,	28	)	=	Vbf15	;
    // BF_VEL	(	BF_TIME_N	,	29	)	=	Vbf16	;
    // BF_VEL	(	BF_TIME_N	,	30	)	=	vbfn2	;
    // BF_VEL	(	BF_TIME_N	,	31	)	=	VsNADPH	;
    
    // PS_VEL(13,PS_TIME_N) = v16;
    // PS_VEL(15,PS_TIME_N) = v31;
    // PS_VEL(16,PS_TIME_N) = v32;
    // PS_VEL(17,PS_TIME_N) = v33;
    // PS_VEL(18,PS_TIME_N) = Pi;
    
    //     TSave = PR_VEL(:,1);
    // time2 = clock;
    // TotalRunTime = time2-time1
    
    IniModelCom(myVars);
    //save LRC
    

    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

    return Resulta;
}
