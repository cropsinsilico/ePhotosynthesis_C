#include "Variables.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RuACT_Rate(double t, RuACTCon &RuACT_Con, Variables *myVars) {

    const double kn1 = myVars->RuACT_RC.kn1;	//	The rate constant of E inactivation by binding of RuBP;	Lazar 1999, with a lifetime of 5 ns at closed reaction center

    const double Ke2 = myVars->RuACT_RC.Ke2;	//	Data from Mate et al 1996. Unit: micormolar;	Reference needed, a guess
    const double Ke3 = myVars->RuACT_RC.Ke3;	//	Data from Mate et al 1996. Unit: micormolar;
    const double k6 = myVars->RuACT_RC.k6;	//	micromolar per meter square per second, transfered to unit
    const double kc = myVars->RuACT_RC.kc;	//	Michaelis menton constant for CO2
    const double ko = myVars->RuACT_RC.ko;	//	Michaelis menton constant for O2
    const double k7 = myVars->RuACT_RC.k7;	//	The rate constant for ecm to ecmr

    const double ER = RuACT_Con.ER;	//	The concentration of inactive ER
    const double Eaf = RuACT_Con.Eaf;	//	The total concentration of  E, EC, AND ECM
    const double ECMR = RuACT_Con.ECMR;	//	The concentration of ECMR
    const double RuBP = RuACT_Con.RuBP;	//	The concentration of ECMR

    double C = myVars->RuACT_Pool.C;
    double O = myVars->RuACT_Pool.O;
    double MT = myVars->RuACT_Pool.M;

    if (myVars->RROEA_EPS_com) {
        myVars->activase = myVars->RROEA2RuACT_RuAC * 14364;
    }

    double RatioDT;
    double ADP;
    double ATP;
    if (!myVars->RuACT_EPS_com) {
        ATP = 1.45;
        ADP = 1.5 - ATP;
        RatioDT = ADP / ATP;
    } else {
        C = myVars->PSPR_RA_CO2;
        O = myVars->PSPR_RA_O2;

        if (myVars->StomCond_TrDynaPS_com) {
            C = myVars->PS2RubACC;
            O = myVars->PS2RubACO;
        }

        MT = myVars->FIBF_RA_Mg;
        ATP = myVars->PS2RA_ATP;
        ADP = myVars->PSPR_RA_CA - ATP;
        RatioDT = ADP / ATP;
    }


    const double CA = 1;
    const double CB = Ke3 + Ke2 * Ke3 / C + Eaf - MT;
    const double CC = - MT * (Ke3 + Ke2 * Ke3 / C);

    const double M = (-CB + pow(( pow(CB, 2) - 4 * CA * CC), 0.5)) / (2 * CA);
    const double EC = Eaf *  pow((1 + Ke2 / C + M / Ke3), -1);
    const double E = EC / C * Ke2;
    const double ECM = EC * M / Ke3;

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

    const double v1 = RCA * ER * FATP;
    const double vn1 = kn1 * E * RuBP;
    const double v7 = k7 * ECM * RuBP;

    const double factor_n7 = 1;
    const double kn7 = 0.5 * factor_n7;

    const double vn7 = ECMR * kn7;

    const double v6_1 = ECMR * k6 * C / (C + kc * (1 + O / ko));
    const double v6_2 = ECMR * k6 / 3 * O / (O + ko * (1 + C / kc));

    if (t > myVars->RuACT_OLD_TIME) {
        myVars->RuACT_TIME_N = myVars->RuACT_TIME_N + 1;
        myVars->RuACT_OLD_TIME = t;
    }

    myVars->RuACT_Vel.v1 = v1;
    myVars->RuACT_Vel.vn1 = vn1;
    myVars->RuACT_Vel.v7 = v7;
    myVars->RuACT_Vel.vn7 = vn7;
    myVars->RuACT_Vel.v6_1 = v6_1;
    myVars->RuACT_Vel.v6_2 = v6_2;

    if (myVars->record) {
        myVars->RuACT_VEL.insert(myVars->RuACT_TIME_N - 1, t, myVars->RuACT_Vel);
    }

    myVars->RuACT2RA_v61 = v6_1;
    myVars->RuACT2RA_v62 = v6_2;
    myVars->RuACT2RA_v1 = v1;
    myVars->RuACT2RA_vn1 = vn1;
    myVars->RuACT2RA_vn7 = vn7;
    myVars->RuACT2RA_v7 = v7;
}
