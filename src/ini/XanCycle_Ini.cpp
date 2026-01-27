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
const std::size_t XanCycleCondition::count = COUNT_XanCycle;
bool XanCycle::NPQ_connect = false;
bool XanCycleCondition::NPQ_connect = false;

DEFINE_MODULE(XanCycle);

void XanCycle::_initOrig(Variables *theVars,
			 XanCycleCondition* XanCycle_con) {

    theVars->XanCycle_RC.kva = 0.163 / 60. * theVars->XanCycleRatio[0]; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kaz = 0.691 / 60. * theVars->XanCycleRatio[1]; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kza = 0.119 / 60. * theVars->XanCycleRatio[2]; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kav = 0.119 / 60. * theVars->XanCycleRatio[3]; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
    
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

}

void XanCycle::_initCalc(Variables *theVars,
			 XanCycleCondition* XanCycle_con) {
    XanCycle::setNPQ_connect(theVars->UseZaksNPQ);
    theVars->XanCycle_RC.kva *= theVars->XanCycleRatio[0] / 60.; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kaz *= theVars->XanCycleRatio[1] / 60.; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kza *= theVars->XanCycleRatio[2] / 60.; // Ruth Frommolt et a; 2001; Planta
    theVars->XanCycle_RC.kav *= theVars->XanCycleRatio[3] / 60; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.

    XanCycle_con->Vx = XanCycle::Vx_ * 0.37;
    XanCycle_con->Ax = XanCycle::Ax_ * 0.37;
    XanCycle_con->Zx = XanCycle::Zx_ * 0.37;
    XanCycle_con->ABA = XanCycle::ABA_;
 
    if (theVars->UseZaksNPQ) {
      theVars->XanCycle_RC.kva = theVars->XanCycle_RC.kvde_max;
      theVars->XanCycle_RC.kaz = theVars->XanCycle_RC.kvde_max;
      theVars->XanCycle_RC.kza = theVars->XanCycle_RC.k_ze;
      theVars->XanCycle_RC.kav = theVars->XanCycle_RC.k_ze;
      XanCycle::Vx_ = 0.7;
      XanCycle::Ax_ = 0.2;
      XanCycle::Zx_ = 0.1;
      XanCycle::ABA_ = 0.0;
      XanCycle_con->Vx = XanCycle::Vx_;
      XanCycle_con->Ax = XanCycle::Ax_;
      XanCycle_con->Zx = XanCycle::Zx_;
      XanCycle_con->ABA = XanCycle::ABA_;
      XanCycle::update_Kd_NPQ(XanCycle_con, theVars);
    } else {
      XanCycle_con->PsbSQ = 0.0;
    }
    
    XanCycle::XanCycle2FIBF_Xstate = XanCycle::Zx_ /
      (XanCycle::Ax_ + XanCycle::Vx_ + XanCycle::Zx_);
}

DEFINE_DEFAULT_CHECKALT(XanCycle)
