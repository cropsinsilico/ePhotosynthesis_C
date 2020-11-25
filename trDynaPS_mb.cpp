#include "Variables.hpp"
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

    myVars->trDynaPS2RedReg_cal = 0;

    trDynaPSCon trDynaPS_con(x);

    Condition(t, myVars);
    const double light = 1.0;
    myVars->FI_Param[0] = light;
    myVars->BF_Param[0] = light;

    myVars->RROEA_Param[1] = 1;
    arr RROEA_DYDT = RROEA_Mb(t, trDynaPS_con.RROEA_con, myVars);

    arr DynaPS_DYDT = DynaPSmb(t, trDynaPS_con.DynaPS_con, myVars);

    for (size_t index = 0; index < 96; index++)
        dxdt[index] = DynaPS_DYDT[index];


    for (size_t index = 0; index < 10; index++)
        dxdt[index + 110] = RROEA_DYDT[index];


    //////WY201804
    const double Temp = RROEA_DYDT[8] - myVars->RROEA2trDynaPS_ve2Fd + myVars->BF2RROEA_Vbf16 / myVars->AVR + myVars->RROEA2trDynaPS_veFd2Calvin - myVars->BF2trDynaPS_vbfn2 - myVars->BF2TrDynaPSMB_vcet / myVars->AVR;

    dxdt[118] = Temp * myVars->AVR;
    dxdt[23] = Temp * myVars->AVR;

    GenOut(t, myVars);

    return 0;
}
