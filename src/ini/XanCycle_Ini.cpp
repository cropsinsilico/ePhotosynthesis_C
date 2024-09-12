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

#ifdef CHECK_VALUE_SET_ALTS
const double Vx_ = 160.;
const double Ax_ = 10.;
const double Zx_ = 5.;
const double ABA_ = 1.;
#endif // CHECK_VALUE_SET_ALTS

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DEFINE_VALUE_SET_STATIC(XanCycle);

INIT_MEMBER_STATIC(XanCycle, kav);
INIT_MEMBER_STATIC(XanCycle, kaz);
INIT_MEMBER_STATIC(XanCycle, kva);
INIT_MEMBER_STATIC(XanCycle, kza);
INIT_MEMBER_STATIC(XanCycle, XanCycle2FIBF_Xstate);

double XanCycle::TIME = 0.;
std::size_t XanCycle::N = 1;

const std::size_t XanCycleCondition::count = 4;

XanCycleCondition* XanCycle::_init(Variables *theVars) {

    theVars->initParamStatic<XanCycle>();
    XanCycleCondition* XanCycle_con = new XanCycleCondition();
    theVars->initParam(*XanCycle_con);

    int i = 0;
    for (XanCycle::iterator it = XanCycle::begin(); it != XanCycle::end(); i++) {
      it->second = it->second / 60.0 * theVars->XanRatio[i];
      if (i == 3)
	break;
    }

    // XanCycle2FIBF_Xstate set before multiplying Vx, Ax, & Zx by 0.37
    XanCycle::set(MOD::XanCycle::XanCycle2FIBF_Xstate,
		  (*XanCycle_con)[COND::XanCycle::Zx] /
		  ((*XanCycle_con)[COND::XanCycle::Ax] +
		   (*XanCycle_con)[COND::XanCycle::Vx] +
		   (*XanCycle_con)[COND::XanCycle::Zx]));

    (*XanCycle_con)[COND::XanCycle::Vx] = 0.37 * (*XanCycle_con)[COND::XanCycle::Vx];
    (*XanCycle_con)[COND::XanCycle::Ax] = 0.37 * (*XanCycle_con)[COND::XanCycle::Ax];
    (*XanCycle_con)[COND::XanCycle::Zx] = 0.37 * (*XanCycle_con)[COND::XanCycle::Zx];
    
#ifdef CHECK_VALUE_SET_ALTS
    XanCycle::kva = 0.163 / 60. * theVars->XanRatio[0]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kaz = 0.691 / 60. * theVars->XanRatio[1]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kza = 0.119 / 60. * theVars->XanRatio[2]; // Ruth Frommolt et a; 2001; Planta
    XanCycle::kav = 0.119 / 60. * theVars->XanRatio[3]; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
    
    XanCycle_con->Vx = Vx_ * 0.37;
    XanCycle_con->Ax = Ax_ * 0.37;
    XanCycle_con->Zx = Zx_ * 0.37;
    XanCycle_con->ABA = ABA_;

    SET_VALUE_STATIC(XanCycle, XanCycle2FIBF_Xstate,
		     Zx_ / (Ax_ + Vx_ + Zx_));

    XanCycle_con->checkAlts();
#endif // CHECK_VALUE_SET_ALTS

    return XanCycle_con;
}
