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
#include <math.h>
#include "Variables.hpp"
#include "modules/DynaPS.hpp"
#include "modules/trDynaPS.hpp"
#include "modules/RuACT.hpp"
#include "modules/PS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void RuACT::_Rate(const double t, const RuACTCondition* RuACT_Con, Variables *theVars) {

    double C = theVars->RuACT_Pool.C;
    double O = theVars->RuACT_Pool.O;
    double MT = theVars->RuACT_Pool.M;
    double RuBP;
    double ADP;
    double ATP;
    if (!theVars->RuACT_EPS_com) {
        ATP = 1.45;
        ADP = 1.5 - ATP;
        RuBP = RuACT_Con->RuBP;
    } else {
        RuACT::activase = RuACT_Con->parent->parent->parent->RROEA_con->RuACT * 14364;
        RuBP = RuACT_Con->parent->EPS_con->CM_con->PS_PR_con->PS_con->RuBP;
        C = theVars->CO2_cond;
        O = theVars->O2_cond;

        MT = RuACT_Con->parent->EPS_con->FIBF_con->BF_con->Mgs;
        ATP = RuACT_Con->parent->EPS_con->CM_con->PS_PR_con->PS_con->ATP;
        ADP = PS::getPS_C_CA() - ATP;
    }
    double RatioDT = ADP / ATP;

    const double CA = 1;
    const double CB = theVars->RuACT_RC.Ke3 + theVars->RuACT_RC.Ke2 * theVars->RuACT_RC.Ke3 / C + RuACT_Con->Eaf - MT;
    const double CC = - MT * (theVars->RuACT_RC.Ke3 + theVars->RuACT_RC.Ke2 * theVars->RuACT_RC.Ke3 / C);

    const double M = (-CB + pow(( pow(CB, 2) - 4 * CA * CC), 0.5)) / (2 * CA);
    const double EC = RuACT_Con->Eaf *  pow((1 + theVars->RuACT_RC.Ke2 / C + M / theVars->RuACT_RC.Ke3), -1);
    const double E = EC / C * theVars->RuACT_RC.Ke2;
    const double ECM = EC * M / theVars->RuACT_RC.Ke3;

    double LT;
    double RCA;
    if (RuACT::activase < pow(10, -6)) {
        RCA = 0;
    } else {
        LT = 216.9 / RuACT::activase; // The lifetime of the activation; UNIT: MIN;
        RCA = 1 / (LT * 60);            // The rate constant of the activation reaction
    }

    if (RatioDT > 3) {
        RatioDT = 3;
    } else if (RatioDT < 0.25){
        RatioDT = 0;
    }

    double FATP = 1 - RatioDT / 3;

    if (FATP < 0.6)
        FATP = 0.6;

    const double factor_n7 = 1;

    theVars->RuACT_Vel.v1 = RCA * RuACT_Con->ER * FATP;
    theVars->RuACT_Vel.vn1 = theVars->RuACT_RC.kn1 * E * RuBP;
    theVars->RuACT_Vel.v7 = theVars->RuACT_RC.k7 * ECM * RuBP;
    theVars->RuACT_Vel.vn7 = RuACT_Con->ECMR * 0.5 * factor_n7;
    theVars->RuACT_Vel.v6_1 = RuACT_Con->ECMR * theVars->RuACT_RC.k6 * C / (C + theVars->RuACT_RC.kc * (1 + O / theVars->RuACT_RC.ko));
    theVars->RuACT_Vel.v6_2 = RuACT_Con->ECMR * theVars->RuACT_RC.k6 / 3 * O / (O + theVars->RuACT_RC.ko * (1 + C / theVars->RuACT_RC.kc));
#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->RuACT_Vel)
#endif
    if (theVars->record) {
        if (t > RuACT::TIME) {
            RuACT::N++;
            RuACT::TIME = t;
        }
        theVars->RuACT_VEL.insert(RuACT::N - 1, t, theVars->RuACT_Vel);
    }
}
