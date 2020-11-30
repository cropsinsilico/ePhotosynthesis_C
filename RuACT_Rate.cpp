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

void RuACT::RuACT_Rate(double t, RuACTCon &RuACT_Con, Variables *myVars) {

    double C = myVars->RuACT_Pool.C;
    double O = myVars->RuACT_Pool.O;
    double MT = myVars->RuACT_Pool.M;

    if (myVars->RROEA_EPS_com) {
        myVars->activase = myVars->RROEA2RuACT_RuAC * 14364;
    }

    double ADP;
    double ATP;
    if (!myVars->RuACT_EPS_com) {
        ATP = 1.45;
        ADP = 1.5 - ATP;
    } else {
        C = myVars->CO2_cond;
        O = myVars->O2_cond;

        MT = myVars->FIBF_RA_Mg;
        ATP = myVars->PS2RA_ATP;
        ADP = myVars->PSPR_RA_CA - ATP;
    }
    double RatioDT = ADP / ATP;


    const double CA = 1;
    const double CB = myVars->RuACT_RC.Ke3 + myVars->RuACT_RC.Ke2 * myVars->RuACT_RC.Ke3 / C + RuACT_Con.Eaf - MT;
    const double CC = - MT * (myVars->RuACT_RC.Ke3 + myVars->RuACT_RC.Ke2 * myVars->RuACT_RC.Ke3 / C);

    const double M = (-CB + pow(( pow(CB, 2) - 4 * CA * CC), 0.5)) / (2 * CA);
    const double EC = RuACT_Con.Eaf *  pow((1 + myVars->RuACT_RC.Ke2 / C + M / myVars->RuACT_RC.Ke3), -1);
    const double E = EC / C * myVars->RuACT_RC.Ke2;
    const double ECM = EC * M / myVars->RuACT_RC.Ke3;

    double LT;
    double RCA;
    if (myVars->activase < pow(10, -6)) {
        RCA = 0;
    } else {
        LT = 216.9 / myVars->activase;//	The lifetime of the activation; UNIT: MIN;
        RCA = 1 / (LT * 60);// 	The rate constant of the activation reaction
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

    if (t > myVars->RuACT_OLD_TIME) {
        myVars->RuACT_TIME_N = myVars->RuACT_TIME_N + 1;
        myVars->RuACT_OLD_TIME = t;
    }

    myVars->RuACT_Vel.v1 = RCA * RuACT_Con.ER * FATP;
    myVars->RuACT_Vel.vn1 = myVars->RuACT_RC.kn1 * E * RuACT_Con.RuBP;
    myVars->RuACT_Vel.v7 = myVars->RuACT_RC.k7 * ECM * RuACT_Con.RuBP;
    myVars->RuACT_Vel.vn7 = RuACT_Con.ECMR * 0.5 * factor_n7;
    myVars->RuACT_Vel.v6_1 = RuACT_Con.ECMR * myVars->RuACT_RC.k6 * C / (C + myVars->RuACT_RC.kc * (1 + O / myVars->RuACT_RC.ko));
    myVars->RuACT_Vel.v6_2 = RuACT_Con.ECMR * myVars->RuACT_RC.k6 / 3 * O / (O + myVars->RuACT_RC.ko * (1 + C / myVars->RuACT_RC.kc));

    if (myVars->record) {
        myVars->RuACT_VEL.insert(myVars->RuACT_TIME_N - 1, t, myVars->RuACT_Vel);
    }
}
