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
#include "modules/XanCycle.hpp"

arr XanCycle::XanCycle_Mb(const double t, const XanCycleCon* XanCycle_Con, Variables *theVars) {
    Condition(t, theVars);

    XanCycle_Rate(t, XanCycle_Con, theVars);

    arr XanCycle_mb = zeros(4);

    XanCycle_mb[0] = theVars->XanCycle_Vel.Vvf + theVars->XanCycle_Vel.Vav - theVars->XanCycle_Vel.Vva - theVars->XanCycle_Vel.Vv2ABA;
    XanCycle_mb[1] = theVars->XanCycle_Vel.Vva - theVars->XanCycle_Vel.Vav + theVars->XanCycle_Vel.Vza - theVars->XanCycle_Vel.Vaz;
    XanCycle_mb[2] = theVars->XanCycle_Vel.Vaz - theVars->XanCycle_Vel.Vza;
    XanCycle_mb[3] = theVars->XanCycle_Vel.Vv2ABA - theVars->XanCycle_Vel.VABAdg;

    return XanCycle_mb;
}
