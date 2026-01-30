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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;


void XanCycle::update_shared(const XanCycleCondition* const XanCycle_con,
                             Variables *theVars) {
  XanCycle::XanCycle2FIBF_Xstate = XanCycle_con->Zx /
    (XanCycle_con->Ax + XanCycle_con->Vx + XanCycle_con->Zx);
  if (!theVars->UseZaksNPQ)
    return;
  // Protonation and enzyme activation
  const double Zx_Ax = (XanCycle_con->Ax + XanCycle_con->Zx)
    / (XanCycle_con->Ax + XanCycle_con->Vx + XanCycle_con->Zx);
  // Total quenching according to Zaks et al. 2012
  const double Q_zaks = theVars->XanCycle_RC.Fpsbs
    * XanCycle_con->PsbSQ * Zx_Ax;
  double kd = 2.0 * pow(10., 8.0) * Q_zaks;
  XanCycle::XanCycle2FIBF_Kd_NPQ = kd;
}

void XanCycle::_Rate(const double t, const XanCycleCondition* const XanCycle_Con, Variables *theVars) {

    Condition(t, theVars);

    double pH;
    if (theVars->XanCycle_BF_com) {
        pH = XanCycle_Con->parent->RA_con->EPS_con->FIBF_con->BF_con->PHl;
    } else {
        pH = 6.;
    }

    double RegCof;
    if (theVars->UseZaksNPQ) {
        // Protonation and enzyme activation
        const double PsbSQ = XanCycle_Con->PsbSQ;
        const double expr_psbs = pow(10., (theVars->XanCycle_RC.hill_psbs * (pH - theVars->XanCycle_RC.pK_psbs)));
        const double QH = 1. / (1 + expr_psbs);
        const double one_minus_QH = expr_psbs / (1 + expr_psbs);
        theVars->XanCycle_Vel.vpsbs_act = theVars->XanCycle_RC.psbsQ_converRate * (1 - PsbSQ) * QH;  // Activation rate
        theVars->XanCycle_Vel.vpsbs_deact = theVars->XanCycle_RC.psbsQ_converRate * PsbSQ * one_minus_QH;  // Deactivation rate
        const double expr_vde = pow(10., (theVars->XanCycle_RC.hill_vde * (pH - theVars->XanCycle_RC.pK_vde)));
        RegCof = 1.0 / (1 + expr_vde);
    } else if (pH <= 5.8) {
        RegCof = 1.;
    } else if (pH > 5.8 && pH < 6.5){
        RegCof = (6.5 - pH) / 0.7;
    } else {
        RegCof = 0.;
    }

    theVars->XanCycle_Vel.Vva = XanCycle_Con->Vx * theVars->XanCycle_RC.kva * RegCof; // The velocity of v to a conversion
    theVars->XanCycle_Vel.Vaz = XanCycle_Con->Ax * theVars->XanCycle_RC.kaz * RegCof; // The rate of A to z
    theVars->XanCycle_Vel.Vza = XanCycle_Con->Zx * theVars->XanCycle_RC.kza; // THe rate of z to a
    theVars->XanCycle_Vel.Vav = XanCycle_Con->Ax * theVars->XanCycle_RC.kav; // The rate of A to V
    theVars->XanCycle_Vel.Vvf = VVF; // The rate of V formation
    theVars->XanCycle_Vel.Vv2ABA = VV2ABA; // The rate of conversion from v to XanCycle_Con.ABA.
    theVars->XanCycle_Vel.VABAdg = VABADG; // The rate of XanCycle_Con.ABA degradation

    // The Xstate part of this was missing in the original C++
    // translation & was disabled by a typo in the MATLAB version...
    XanCycle::update_shared(XanCycle_Con, theVars);
    
#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->XanCycle_Vel)
#endif
    if (theVars->record) {
        if (t > XanCycle::TIME) {
            XanCycle::N++;
            XanCycle::TIME = t;
        }

        theVars->XanCycle_VEL.insert(XanCycle::N, t, theVars->XanCycle_Vel);

        theVars->XanCycle2OUT.Vx = XanCycle_Con->Vx;
        theVars->XanCycle2OUT.Ax = XanCycle_Con->Ax;
        theVars->XanCycle2OUT.Zx = XanCycle_Con->Zx;
        theVars->XanCycle2OUT.ABA = XanCycle_Con->Vx / (XanCycle_Con->Vx + XanCycle_Con->Ax +
                                                        XanCycle_Con->Zx);
    }

}
