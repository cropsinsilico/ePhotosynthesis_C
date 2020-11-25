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

void XanCycle_Rate(double t, XanCycleCon &XanCycle_Con, Variables *myVars) {

    Condition(t, myVars);

    double pH;
    if (myVars->XanCycle_BF_com) {
        pH = myVars->BF2XanCycle_pHl;
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


    const double Vva = XanCycle_Con.Vx * myVars->XanCycle_kva * RegCof;
    const double Vaz = XanCycle_Con.Ax * myVars->XanCycle_kaz * RegCof;
    const double Vza = XanCycle_Con.Zx * myVars->XanCycle_kza;
    const double Vav = XanCycle_Con.Ax * myVars->XanCycle_kav;

    const double Vvf = 0;
    const double Vv2ABA = 0;
    const double VABAdg = 0;

    if (t > myVars->XanCycle_OLD_TIME) {
            myVars->XanCycle_TIME_N = myVars->XanCycle_TIME_N + 1;
            myVars->XanCycle_OLD_TIME = t;
        }
    myVars->XanCycle_Vel.Vva = Vva;	//	The velocity of v to a conversion
    myVars->XanCycle_Vel.Vaz = Vaz;	//	The rate of A to z
    myVars->XanCycle_Vel.Vza = Vza;	//	THe rate of z to a
    myVars->XanCycle_Vel.Vav = Vav;	//	The rate of A to V
    myVars->XanCycle_Vel.Vvf = Vvf;	//	The rate of V formation
    myVars->XanCycle_Vel.Vv2ABA = Vv2ABA;	//	The rate of conversion from v to XanCycle_Con.ABA.
    myVars->XanCycle_Vel.VABAdg = VABAdg;	//	The rate of XanCycle_Con.ABA degradation

    if (myVars->record) {
        myVars->XanCycle_VEL.insert(myVars->XanCycle_TIME_N, t, myVars->XanCycle_Vel);
    }

    myVars->XanCycle2OUT[0] = XanCycle_Con.Vx;
    myVars->XanCycle2OUT[1] = XanCycle_Con.Ax;
    myVars->XanCycle2OUT[2] = XanCycle_Con.Zx;
    myVars->XanCycle2OUT[3] = XanCycle_Con.Vx / (XanCycle_Con.Vx + XanCycle_Con.Ax + XanCycle_Con.Zx);
}
