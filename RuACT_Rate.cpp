#include "globals.hpp"

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



void RuACT_Rate(double t, arr &RuACT_Con, varptr *myVars) {
    
    //global RuACT_RC;
    // k1 = myVars->RuACT_RC[0];	//	The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco myVars->activase content or activity; i.e. 	Lazar (1999), 0.25~1 *10^(9)// --unused
    const double kn1 = myVars->RuACT_RC[1];	//	The rate constant of E inactivation by binding of RuBP;	Lazar 1999, with a lifetime of 5 ns at closed reaction center
    // km1 = myVars->RuACT_RC[2];	//	The michaelis menton constant for RuBP with E.	Reference needed, a guess// --unused
    const double Ke2 = myVars->RuACT_RC[3];	//	Data from Mate et al 1996. Unit: micormolar;	Reference needed, a guess
    const double Ke3 = myVars->RuACT_RC[4];	//	Data from Mate et al 1996. Unit: micormolar;
    const double k6 = myVars->RuACT_RC[5];	//	micromolar per meter square per second, transfered to unit
    const double kc = myVars->RuACT_RC[6];	//	Michaelis menton constant for CO2
    const double ko = myVars->RuACT_RC[7];	//	Michaelis menton constant for O2
    const double k7 = myVars->RuACT_RC[8];	//	The rate constant for ecm to ecmr
    // kr = myVars->RuACT_RC[9];	//	The apparaent michaelis menton constant for RuBP// --unused
    
    const double ER = RuACT_Con[0];	//	The concentration of inactive ER
    const double Eaf = RuACT_Con[1];	//	The total concentration of  E, EC, AND ECM
    const double ECMR = RuACT_Con[2];	//	The concentration of ECMR
    const double RuBP = RuACT_Con[3];	//	The concentration of ECMR
    
    //global RuACT_Pool;
    //const double ET = myVars->RuACT_Pool[0];
    //double Rac = myVars->RuACT_Pool[1];
    double C = myVars->RuACT_Pool[2];
    double O = myVars->RuACT_Pool[3];
    double MT = myVars->RuACT_Pool[4];
    
    //global activase;
    // activase = 80  ;
    
    //global RROEA_EPS_com;
    
    if (myVars->RROEA_EPS_com) {
        
        //global myVars->RROEA_Pool;
        //Rac = myVars->RROEA_Pool.RuACTT;
        
        //global myVars->RROEA2RuACT_RuAC;
        myVars->activase = myVars->RROEA2RuACT_RuAC * 14364;
    }
    
    //global RuACT_EPS_com;
    //global PSPR_RA_O2;
    //global PSPR_RA_CO2;
    //global FIBF_RA_Mg;
    //global PSPR_RA_CA;
    //global PS2RA_ATP;
    
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
        
        
        //global myVars->StomCond_TrDynaPS_com;
        if (myVars->StomCond_TrDynaPS_com) {
            //global myVars->PS2RubACC;
            //global myVars->PS2RubACO;
            
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
    
    
    //global RuACT_OLD_TIME;
    //global RuACT_TIME_N;
    //global RuACT_VEL;
    //global RuACT_CON;
    
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
        //if (myVars->RuACT_TIME_N == 0)
        //    myVars->RuACT_TIME_N = 1;
        myVars->RuACT_VEL.insert(myVars->RuACT_TIME_N - 1, t, myVars->RuACT_Vel);
    
        
        /*
        if (myVars->RuACT_VEL.shape()[0] < myVars->RuACT_TIME_N) {
            myVars->RuACT_VEL.resize(boost::extents[myVars->RuACT_TIME_N][RuACT_VEL_SIZE]);
            myVars->RuACT_CON.resize(boost::extents[myVars->RuACT_TIME_N][RuACT_CON_SIZE]);
        }

        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][0] = t;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][1] = v1;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][2] = vn1;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][3] = v7;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][4] = vn7;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][5] = v6_1;
        myVars->RuACT_VEL[myVars->RuACT_TIME_N - 1][6] = v6_2;
        */
    }
    //myVars->RuACT_CON[myVars->RuACT_TIME_N - 1][0] = t;  // --unused
    //myVars->RuACT_CON[myVars->RuACT_TIME_N - 1][1] = E;  // --unused
    //myVars->RuACT_CON[myVars->RuACT_TIME_N - 1][2] = EC;  // --unused
    //myVars->RuACT_CON[myVars->RuACT_TIME_N - 1][3] = ECM;  // --unused
    
    
    //arr RuACT_Vel = zeros(6);
    
    //global RuACT2RA_v61;
    //global RuACT2RA_v62;
    //global RuACT2RA_v1;
    //global RuACT2RA_vn1;
    //global RuACT2RA_v7;
    //global RuACT2RA_vn7;
    
    myVars->RuACT2RA_v61 = v6_1;
    myVars->RuACT2RA_v62 = v6_2;
    myVars->RuACT2RA_v1 = v1;
    myVars->RuACT2RA_vn1 = vn1;
    myVars->RuACT2RA_vn7 = vn7;
    myVars->RuACT2RA_v7 = v7;
    
    //const double Percent = ECM / ET;
    
    
    //global RuACT2PS_Percent;
    //myVars->RuACT2PS_Percent = Percent;  // --unused
    //return RuACT_Vel;
}
