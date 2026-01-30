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
    
    XanCycle_con->Vx = 160.0 * 0.37;
    XanCycle_con->Ax = 10.0 * 0.37;
    XanCycle_con->Zx = 5.0 * 0.37;
    XanCycle_con->ABA = 1.0;

    XanCycle::update_shared(XanCycle_con, theVars);

}

void XanCycle::_initCalc(Variables *theVars,
			 XanCycleCondition* XanCycle_con) {
    XanCycle::setNPQ_connect(theVars->UseZaksNPQ);
    if (theVars->UseZaksNPQ) {
      theVars->XanCycle_RC.kva = theVars->XanCycle_RC.kvde_max;
      theVars->XanCycle_RC.kaz = theVars->XanCycle_RC.kvde_max;
      theVars->XanCycle_RC.kza = theVars->XanCycle_RC.k_ze;
      theVars->XanCycle_RC.kav = theVars->XanCycle_RC.k_ze;
      // theVars->XanCycle_RC.kva = theVars->XanCycleRatio[0] * theVars->XanCycle_RC.kvde_max;
      // theVars->XanCycle_RC.kaz = theVars->XanCycleRatio[1] * theVars->XanCycle_RC.kvde_max;
      // theVars->XanCycle_RC.kza = theVars->XanCycleRatio[2] * theVars->XanCycle_RC.k_ze;
      // theVars->XanCycle_RC.kav = theVars->XanCycleRatio[3] * theVars->XanCycle_RC.k_ze;
      if (XanCycle_con->Vx > 1) {
        const double Tot = XanCycle_con->Vx + XanCycle_con->Ax + XanCycle_con->Zx;
        XanCycle_con->Vx /= Tot;
        XanCycle_con->Ax /= Tot;
        XanCycle_con->Zx /= Tot;
      }
    } else {
      // TODO: Move factors from ini functions into param files
      theVars->XanCycle_RC.kva *= theVars->XanCycleRatio[0] / 60.; // Ruth Frommolt et a; 2001; Planta
      theVars->XanCycle_RC.kaz *= theVars->XanCycleRatio[1] / 60.; // Ruth Frommolt et a; 2001; Planta
      theVars->XanCycle_RC.kza *= theVars->XanCycleRatio[2] / 60.; // Ruth Frommolt et a; 2001; Planta
      theVars->XanCycle_RC.kav *= theVars->XanCycleRatio[3] / 60; // Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
    }
    XanCycle::update_shared(XanCycle_con, theVars);
}

DEFINE_DEFAULT_CHECKALT(XanCycle)
