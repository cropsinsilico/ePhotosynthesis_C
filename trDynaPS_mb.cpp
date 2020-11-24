#include "globals.hpp"
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include "trDynaPS.hpp"
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


// trDynaPS_mb.m  This model includes the mass balance equations for the full model of photosynthesis.

int trDynaPS::trDynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *x = N_VGetArrayPointer(u);
    realtype *dxdt = N_VGetArrayPointer(u_dot);

    //global trDynaPS2RedReg_cal
    myVars->trDynaPS2RedReg_cal = 0;
    
    //fidw = fopen('Mymatrix2.txt','a');
    //fprintf(fidw, '//g   ', t);
    //fprintf(fidw, '//g  ', trDynaPS_Con);
    //fprintf(fidw, '\n');
    //fclose(fidw);
    //N_Vector dy, ddxdt;
    //dy = N_VNew_Serial(96);
    //ddxdt = N_VNew_Serial(96);
    //arr DynaPS_Con = zeros(96);
    
    //for (int m = 0; m < 96; m++)
    //    DynaPS_Con[m] = x[m];
        //NV_Ith_S(dy, m) = x[m];
    trDynaPSCon trDynaPS_con(x);
    //DynaPSCon DynaPS_con = trDynaPS_con.DynaPS_con;//(x);
    
    //arr RROEA_Con = zeros(10);
    //for (int m = 0; m < 10; m++)
    //    RROEA_Con[m] = x[m + 110];
    //RROEACon RROEA_con(RROEA_Con);
    //sRROEACon RROEA_con = trDynaPS_con.RROEA_con;
    
    Condition(t, myVars);
    const double light = 1.0;
    myVars->FI_Param[0] = light;
    myVars->BF_Param[0] = light;
    
    myVars->RROEA_Param[1] = 1;
    arr RROEA_DYDT = RROEA_Mb(t, trDynaPS_con.RROEA_con, myVars);
    //fprintf(" //g    ", DynaPS_Con);
    //fprintf("\n");
    //DynaPS dps = DynaPS(myVars);
    
    arr DynaPS_DYDT = DynaPSmb(t, trDynaPS_con.DynaPS_con, myVars);
    
    //realtype *DynaPS_DYDT = N_VGetArrayPointer(ddxdt);
    //arr trDynaPS_DYDT = zeros(119);
    
    for (int index = 0; index < 96; index++)
        dxdt[index] = DynaPS_DYDT[index];
    
    
    for (int index = 0; index < 10; index++)
        dxdt[index + 110] = RROEA_DYDT[index];
    
    
    //global RROEA2trDynaPS_ve2Fd;
    //global BF2RROEA_Vbf16;
    //global RROEA2trDynaPS_veFd2Calvin;
    //global BF2trDynaPS_vbfn2;
    
    //global BF2TrDynaPSMB_vcet;
    //global AVR;
    //global PRGlu;
    //////WY201804
    const double Temp = RROEA_DYDT[8] - myVars->RROEA2trDynaPS_ve2Fd + myVars->BF2RROEA_Vbf16 / myVars->AVR + myVars->RROEA2trDynaPS_veFd2Calvin - myVars->BF2trDynaPS_vbfn2 - myVars->BF2TrDynaPSMB_vcet / myVars->AVR;
    //Temp = RROEA_DYDT(9) - RROEA2trDynaPS_ve2Fd + BF2RROEA_Vbf16/AVR + RROEA2trDynaPS_veFd2Calvin - BF2trDynaPS_vbfn2 - BF2TrDynaPSMB_vcet/AVR-PRGlu/AVR;
    
    dxdt[118] = Temp * myVars->AVR;
    dxdt[23] = Temp * myVars->AVR;
    //fid = fopen('Mymatrix.txt','a');
    //fprintf(fid, '//e   ', t);
    //fprintf(fid, '//f  ', trDynaPS_DYDT);
    //fprintf(fid, '\n');
    //fclose(fid);
    
    GenOut(t, myVars);
    //N_VDestroy(dy);
    //N_VDestroy(ddxdt);
    return 0;
}
