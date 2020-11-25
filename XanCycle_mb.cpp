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

#include "globals.hpp"
#include "Variables.hpp"

arr XanCycle_Mb(double t, XanCycleCon &XanCycle_Con, Variables *myVars) {
    Condition(t, myVars);

    XanCycle_Rate(t, XanCycle_Con, myVars);

    const double Vva = myVars->XanCycle_Vel.Vva;	//	The velocity of v to a conversion
    const double Vaz = myVars->XanCycle_Vel.Vaz;	//	The rate of A to z
    const double Vza = myVars->XanCycle_Vel.Vza;	//	THe rate of z to a
    const double Vav = myVars->XanCycle_Vel.Vav;	//	The rate of A to V
    const double Vvf = myVars->XanCycle_Vel.Vvf;	//	The rate of V formation
    const double Vv2ABA = myVars->XanCycle_Vel.Vv2ABA;	//	The rate of conversion from v to ABA.
    const double VABAdg = myVars->XanCycle_Vel.VABAdg;	//	The rate of ABA degradation

    arr XanCycle_mb = zeros(4);

    XanCycle_mb[0] = Vvf + Vav - Vva - Vv2ABA;
    XanCycle_mb[1] = Vva - Vav + Vza - Vaz;
    XanCycle_mb[2] = Vaz - Vza;
    XanCycle_mb[3] = Vv2ABA - VABAdg;

    return XanCycle_mb;
}
