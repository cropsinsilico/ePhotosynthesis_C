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

Variables *trDynaPS::myVars = new Variables();

arr trDynaPS::trDynaPS_Drive(size_t ParaNum, double Ratio) {

    if (ParaNum <= 103)
        myVars->PSRatio[ParaNum] = Ratio;

    if (ParaNum > 103&&ParaNum <= 169)
        myVars->SUCRatio[ParaNum - 103] = Ratio;

    if (ParaNum > 169&&ParaNum <= 217)
        myVars->PRRatio[ParaNum - 169] = Ratio;

    if (ParaNum > 217&&ParaNum <= 233)
        myVars->RacRatio[ParaNum - 217] = Ratio;

    if (ParaNum > 233&&ParaNum <= 256)
        myVars->FIRatio[ParaNum - 233] = Ratio;

    if (ParaNum > 256&&ParaNum <= 305)
        myVars->BFRatio[ParaNum - 256] = Ratio;

    if (ParaNum > 305&&ParaNum <= 309)
        myVars->XanRatio[ParaNum - 305] = Ratio;

    PreProcess(myVars);

    SYSInitial(myVars);

    myVars->ATPActive = 0;

    // Indicate in the beginning there is no ATP synthesis activity.
    myVars->EPS_ATP_Rate = 0;

    IniModelCom(myVars);        // Initialize the structure of the model, i.e. Is this model separate or combined with others.

    // The combination of BF and FI model
    myVars->BF_FI_com = true;

    // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    myVars->FIBF_PSPR_com = true;

    // A global variable to indicate whether the RuACT is run by itself or combined with others.
    myVars->RuACT_EPS_com = true;        // Since this is run within this program, it is combinbed, therefore, it is assigned value true, otherwise, assign value false.

    // This is the connection between Redox and RA.
    myVars->RedoxReg_RA_com = false;        // This means that the connection is there.

    myVars->XanCycle_BF_com = true;

    myVars->RROEA_EPS_com = true;

    myVars->EPS_SUCS_com = true;

    // This is a variable indicating whether the PSPR model is actually need to be combined with SUCS or not. If 1 then means combined; 0 means not.
    myVars->PSPR_SUCS_com = true;

    // Next is to initialize the vector.
    trDynaPSCon trDynaPS_con = trDynaPS_Ini();
    ParamSet(myVars);
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

    SUNMatrix A = SUNDenseMatrix(N, N);


    SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

    flag = CVDlsSetLinearSolver(cvode_mem, LS, A);
    if (flag != 0)
        std::cout << "FAIL" << std::endl;

    realtype end_time = 250;
    realtype step_length = 1.;
    realtype t = 0;

    for (realtype tout = step_length; tout <= end_time; tout += step_length)
        flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);

    myVars->ATPActive = 0;

    double CarbonRate = myVars->RuACT_Vel.v6_1 * myVars->AVR;
    double VPR = myVars->RuACT_Vel.v6_2 * myVars->AVR;
    double Vpgasink = myVars->SUCS_Vel.vpga_use * myVars->AVR;
    double VStarch = (myVars->PS_Vel.v23 - myVars->PS_Vel.v25) * myVars->AVR;
    double Vt3p = (myVars->PS_Vel.v31 + myVars->PS_Vel.v33) * myVars->AVR;
    arr Resulta = zeros(7);

    Resulta[0] = CarbonRate; //Vc
    Resulta[1] = VPR;        //Vo
    Resulta[2] = Vpgasink;   //PGA
    Resulta[3] = Vt3p;       //VT3P
    Resulta[4] = VStarch;    //Vstarch
    Resulta[5] = myVars->PR_Vel.v1in * myVars->AVR;//Vt_glycerate
    Resulta[6] = myVars->PR_Vel.v2out * myVars->AVR;//Vt_glycolate

    if(myVars->record) {
        myVars->FluxTR[0] = myVars->RuACT_VEL.getLastData().v6_1;//PS
        myVars->FluxTR[1] = myVars->RuACT_VEL.getLastData().v6_2;//PR
        myVars->FluxTR[2] = myVars->PS_VEL.getLastData().v2;// v2
        myVars->FluxTR[3] = myVars->PS_VEL.getLastData().v3;// v3
        myVars->FluxTR[4] = myVars->PS_VEL.getLastData().v5;// v5
        myVars->FluxTR[5] = myVars->PS_VEL.getLastData().v6;// v6
        myVars->FluxTR[6] = myVars->PS_VEL.getLastData().v7;// v7
        myVars->FluxTR[7] = myVars->PS_VEL.getLastData().v8;// v8
        myVars->FluxTR[8] = myVars->PS_VEL.getLastData().v9;// v9
        myVars->FluxTR[9] = myVars->PS_VEL.getLastData().v10;// v10
        myVars->FluxTR[10] = myVars->PS_VEL.getLastData().v13;// v13
        myVars->FluxTR[11] = myVars->PS_VEL.getLastData().v23;// v23
        myVars->FluxTR[12] = myVars->PS_VEL.getLastData().v24;// v24
        myVars->FluxTR[13] = myVars->PS_VEL.getLastData().v25;// v25
        myVars->FluxTR[14] = myVars->PR_VEL.getLastData().v112;//v112
        myVars->FluxTR[15] = myVars->PR_VEL.getLastData().v113;//v113
        myVars->FluxTR[16] = myVars->PR_VEL.getLastData().v121;//v121
        myVars->FluxTR[17] = myVars->PR_VEL.getLastData().v122;//v122
        myVars->FluxTR[18] = myVars->PR_VEL.getLastData().v123;//v123
        myVars->FluxTR[19] = myVars->PR_VEL.getLastData().v124;//v124
        myVars->FluxTR[20] = myVars->PR_VEL.getLastData().v131;//v131
        myVars->FluxTR[21] = myVars->PR_VEL.getLastData().v1in;//vlin
        myVars->FluxTR[22] = myVars->PR_VEL.getLastData().v2out;//v2out
        myVars->FluxTR[23] = myVars->SUCS_VEL.getLastData().v51;//v51	;//	DHAP+GAP --FBP
        myVars->FluxTR[24] = myVars->SUCS_VEL.getLastData().v52;//v52	;//	FBP --F6P + Pi
        myVars->FluxTR[25] = myVars->SUCS_VEL.getLastData().v55;//v55	;//	G1P+UTP --OPOP+UDPG
        myVars->FluxTR[26] = myVars->SUCS_VEL.getLastData().v56;//v56	;//	UDPG+F6P--SUCP + UDP
        myVars->FluxTR[27] = myVars->SUCS_VEL.getLastData().v57;//v57	;//	SUCP--Pi + SUC
        myVars->FluxTR[28] = myVars->SUCS_VEL.getLastData().v58;//v58	;//	F26BP--F6P + Pi
        myVars->FluxTR[29] = myVars->SUCS_VEL.getLastData().v59;//v59	;//	F6P + ATP --ADP + F26BP
        myVars->FluxTR[30] = myVars->SUCS_VEL.getLastData().v60;//v60	;//	ATP+UDP --UTP + ADP
        myVars->FluxTR[31] = myVars->SUCS_VEL.getLastData().v62;//v62	;//	SUC SINK
        myVars->FluxTR[32] = myVars->SUCS_VEL.getLastData().vdhap_in;//vdhap_in	;//	DHAP export from chloroplast
        myVars->FluxTR[33] = myVars->SUCS_VEL.getLastData().vgap_in;//vgap_in	;//	GAP Export from chloroplast
        myVars->FluxTR[34] = myVars->SUCS_VEL.getLastData().vpga_in;//vpga_in	;//	PGA export from chloroplast
        myVars->FluxTR[35] = myVars->SUCS_VEL.getLastData().vpga_use;//vpga_use	;//	PGA utilisation in cytosol

        /*for (int x = 36; x < 66; x++)
            for (int y = 1; y < 31; y++)
                myVars->FluxTR[x] = myVars->BF_VEL.getLastData()[y];

        for (int x = 66; x < 124; x++)
            for (int y = 1; y < 59; y++)
                myVars->FluxTR[x] = myVars->FI_VEL.getLastData()[y];

        for (int x = 124; x < 131; x++)
            for (int y = 1; y < 8; y++)
                myVars->FluxTR[x] = myVars->XanCycle_VEL.getLastData()[y];

        for (int x = 131; x < 142; x++)
            for (int y = 1; y < 12; y++)
                myVars->FluxTR[x] = myVars->RROEA_VEL.getLastData()[y];

        for (int x = 0; x < 36; x++)
            myVars->FluxTR[x] = myVars->FluxTR[x] * myVars->AVR;
        */
        myVars->FluxTR[46] = myVars->FluxTR[46] * myVars->AVR;
        myVars->FluxTR[64] = myVars->FluxTR[64] * myVars->AVR / 2;
    }


    IniModelCom(myVars);
    //save LRC
    N_VDestroy(y);
    CVodeFree(&cvode_mem);
    SUNLinSolFree(LS);

    return Resulta;
}
