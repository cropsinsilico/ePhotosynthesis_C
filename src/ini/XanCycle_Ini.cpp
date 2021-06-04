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
#include "modules/XanCycle.hpp"

double XanCycle::XanCycle_kav = 0.;
double XanCycle::XanCycle_kaz = 0.;
double XanCycle::XanCycle_kva = 0.;
double XanCycle::XanCycle_kza = 0.;

const size_t XanCycleCon::count = 4;

XanCycleCon* XanCycle::_init(Variables *theVars) {

    XanCycle_kva = 0.163 / 60 * theVars->XanRatio[0]; // Ruth Frommolt et a; 2001; Planta
    XanCycle_kaz = 0.691 / 60 * theVars->XanRatio[1]; // Ruth Frommolt et a; 2001; Planta
    XanCycle_kza = 0.119 / 60 * theVars->XanRatio[2]; // Ruth Frommolt et a; 2001; Planta
    XanCycle_kav = 0.119 / 60 * theVars->XanRatio[3]; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.

    const double Vx = 160;
    const double Ax = 10;
    const double Zx = 5;
    const double ABA = 1;

    XanCycleCon* XanCycle_con = new XanCycleCon();
    XanCycle_con->Vx = Vx * 0.37;
    XanCycle_con->Ax = Ax * 0.37;
    XanCycle_con->Zx = Zx * 0.37;
    XanCycle_con->ABA = ABA;

    theVars->XanCycle_OLD_TIME = 0;
    theVars->XanCycle_TIME_N = 1;

    theVars->XanCycle2FIBF_Xstate = Zx / (Ax + Vx + Zx);

    return XanCycle_con;
}
