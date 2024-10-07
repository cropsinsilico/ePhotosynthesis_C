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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double XanCycle::TIME = 0.;
std::size_t XanCycle::N = 1;

const std::size_t XanCycleCondition::count = 4;

DEFINE_VALUE_SET_STATIC(XanCycle);
DEFINE_VALUE_SET(XanCycleCondition);

XanCycleCondition* XanCycle::_init(Variables *theVars) {

    XanCycle::kva = 0.163 / 60. * theVars->XanRatio[0]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kaz = 0.691 / 60. * theVars->XanRatio[1]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kza = 0.119 / 60. * theVars->XanRatio[2]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kav = 0.119 / 60. * theVars->XanRatio[3]; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
    
    XanCycleCondition* XanCycle_con = new XanCycleCondition();

    XanCycle::Vx_ = 160.;
    XanCycle::Ax_ = 10.;
    XanCycle::Zx_ = 5.;
    XanCycle::ABA_ = 1.;
    XanCycle_con->Vx = XanCycle::Vx_ * 0.37;
    XanCycle_con->Ax = XanCycle::Ax_ * 0.37;
    XanCycle_con->Zx = XanCycle::Zx_ * 0.37;
    XanCycle_con->ABA = XanCycle::ABA_;

    XanCycle::XanCycle2FIBF_Xstate = XanCycle::Zx_ /
      (XanCycle::Ax_ + XanCycle::Vx_ + XanCycle::Zx_);

    return XanCycle_con;
}

void XanCycle::_initAlt(Variables *theVars,
			XanCycleCondition* XanCycle_con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<XanCycle>();
    theVars->initParam(*XanCycle_con);
  
    int i = 0;
    for (XanCycle::iterator it = XanCycle::begin(); it != XanCycle::end(); it++, i++) {
      it->second *= (theVars->XanRatio[i] / 60.0);
      if (i == 3)
	break;
    }

    (*XanCycle_con)[COND::XanCycle::Vx] = 0.37 * XanCycle::get(MOD::XanCycle::Vx_);
    (*XanCycle_con)[COND::XanCycle::Ax] = 0.37 * XanCycle::get(MOD::XanCycle::Ax_);
    (*XanCycle_con)[COND::XanCycle::Zx] = 0.37 * XanCycle::get(MOD::XanCycle::Zx_);
    
    XanCycle::set(MOD::XanCycle::XanCycle2FIBF_Xstate,
		  XanCycle::get(MOD::XanCycle::Zx_) /
		  (XanCycle::get(MOD::XanCycle::Ax_) +
		   XanCycle::get(MOD::XanCycle::Vx_) +
		   XanCycle::get(MOD::XanCycle::Zx_)));

#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(XanCycle_con);
#endif // CHECK_VALUE_SET_ALTS
}

DEFINE_DEFAULT_CHECKALT(XanCycle)
