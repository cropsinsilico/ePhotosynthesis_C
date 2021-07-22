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

#include "modules/DynaPS.hpp"
#include "modules/RA.hpp"
#include "modules/XanCycle.hpp"

#ifdef INCDEBUG
Debug::DebugLevel DynaPSCondition::_dlevel = Debug::Middle;
#endif
DynaPSCondition* DynaPS::_init(Variables *theVars) {

    //////////////////////////////////////////////////////////////////////////////////
    //   Clear up memory for simulation       //
    //////////////////////////////////////////////////////////////////////////////////

    RACondition* RA_con = RA::init(theVars);
    XanCycleCondition* XanCycle_con = XanCycle::init(theVars);
    DynaPSCondition* DynaPS_con = new DynaPSCondition(RA_con, XanCycle_con);
    return DynaPS_con;
}
