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
#include "modules/DynaPS.hpp"
#include "modules/XanCycle.hpp"

#define VVF 0.
#define VV2ABA 0.
#define VABADG 0.

void XanCycle::_Rate(const double t, const XanCycleCon* XanCycle_Con, Variables *theVars) {

    Condition(t, theVars);

    double pH;
    if (theVars->XanCycle_BF_com) {
        pH = XanCycle_Con->parent->RA_con->EPS_con->FIBF_con->BF_con->PHl;
    } else {
        pH = 6;
    }

    double RegCof;
    if (pH <= 5.8) {
        RegCof = 1;
    } else if (pH > 5.8 && pH < 6.5){
        RegCof = (6.5 - pH) / 0.7;
    } else {
        RegCof = 0;
    }


    const double Vva = XanCycle_Con->Vx * XanCycle_kva * RegCof;
    const double Vaz = XanCycle_Con->Ax * XanCycle_kaz * RegCof;
    const double Vza = XanCycle_Con->Zx * XanCycle_kza;
    const double Vav = XanCycle_Con->Ax * XanCycle_kav;


    if (t > theVars->XanCycle_OLD_TIME) {
            theVars->XanCycle_TIME_N = theVars->XanCycle_TIME_N + 1;
            theVars->XanCycle_OLD_TIME = t;
        }
    theVars->XanCycle_Vel.Vva = Vva; // The velocity of v to a conversion
    theVars->XanCycle_Vel.Vaz = Vaz; // The rate of A to z
    theVars->XanCycle_Vel.Vza = Vza; // THe rate of z to a
    theVars->XanCycle_Vel.Vav = Vav; // The rate of A to V
    theVars->XanCycle_Vel.Vvf = VVF; // The rate of V formation
    theVars->XanCycle_Vel.Vv2ABA = VV2ABA; // The rate of conversion from v to XanCycle_Con.ABA.
    theVars->XanCycle_Vel.VABAdg = VABADG; // The rate of XanCycle_Con.ABA degradation

    if (theVars->record) {
        theVars->XanCycle_VEL.insert(theVars->XanCycle_TIME_N, t, theVars->XanCycle_Vel);

        theVars->XanCycle2OUT.Vx = XanCycle_Con->Vx;
        theVars->XanCycle2OUT.Ax = XanCycle_Con->Ax;
        theVars->XanCycle2OUT.Zx = XanCycle_Con->Zx;
        theVars->XanCycle2OUT.ABA = XanCycle_Con->Vx / (XanCycle_Con->Vx + XanCycle_Con->Ax + XanCycle_Con->Zx);
    }

}
