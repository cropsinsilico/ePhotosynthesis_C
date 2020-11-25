#include "globals.hpp"
#include "DynaPS.hpp"
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

DynaPSCon DynaPS::DynaPS_Ini() {
    return DynaPS_Init(myVars);
}

// DynaPS_mb.m  This model includes the mass balance equations for the full model of photosynthesis.

int DynaPS::DynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {

    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.

    //// Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.
    realtype *x = N_VGetArrayPointer(u);
    realtype *dxdt = N_VGetArrayPointer(u_dot);

    DynaPSCon DynaPS_con(x);

    arr ddxdt = DynaPSmb(t, DynaPS_con, myVars);
    for (size_t index = 0; index < 96; index++)
        dxdt[index] = ddxdt[index];

    return 0;
}
