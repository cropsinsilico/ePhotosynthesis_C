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


void XanCycle_Rate(double t, arr &XanCycle_Con, varptr *myVars) {
    
    //global XanCycle_kva;
    //global XanCycle_kaz;
    //global XanCycle_kza;
    //global XanCycle_kav;
    //global XanCycle_kvf;
    //global XanCycle_kv2ABA;
    //global XanCycle_kABAdg;
    
    const double Vx = XanCycle_Con[0];	// The concentration of Violozanthin
    const double Ax = XanCycle_Con[1];	//	The concentration of Anthrozanthin
    const double Zx = XanCycle_Con[2];	//	The concentration of Zeaznthin
    double ABA = XanCycle_Con[3];	//	The concentration of ABA
    
    
    Condition(t, myVars);
    //global Glight;
    
    // light = myVars->Glight;// --unused
    
    //global XanCycle_BF_com;
    double pH;
    if (myVars->XanCycle_BF_com) {
        //global myVars->BF2XanCycle_pHl;
        pH = myVars->BF2XanCycle_pHl;
    } else {
        pH = 6;
    }
    
    double RegCof;
    if (pH <= 5.8) {
        RegCof = 1;
    } else if (pH > 5.8 && pH < 6.5){
        RegCof = (6.5 - pH) / 0.7;
    } else {
        RegCof = 0;
    }
    
    
    const double Vva = Vx * myVars->XanCycle_kva * RegCof;
    const double Vaz = Ax * myVars->XanCycle_kaz * RegCof;
    const double Vza = Zx * myVars->XanCycle_kza;
    const double Vav = Ax * myVars->XanCycle_kav;
    
    const double Vvf = 0;
    const double Vv2ABA = 0;
    const double VABAdg = 0;
    
    //global XanCycle_OLD_TIME;
    //global XanCycle_TIME_N;
    //global XanCycle_VEL;
    //global XanCycle_CON;
    
    //if (myVars->XanCycle_TIME_N == 0)
    //    myVars->XanCycle_TIME_N = 1;
    if (t > myVars->XanCycle_OLD_TIME) {
            myVars->XanCycle_TIME_N = myVars->XanCycle_TIME_N + 1;
            myVars->XanCycle_OLD_TIME = t;
        }
    myVars->XanCycle_Vel.Vva = Vva;	//	The velocity of v to a conversion
    myVars->XanCycle_Vel.Vaz = Vaz;	//	The rate of A to z
    myVars->XanCycle_Vel.Vza = Vza;	//	THe rate of z to a
    myVars->XanCycle_Vel.Vav = Vav;	//	The rate of A to V
    myVars->XanCycle_Vel.Vvf = Vvf;	//	The rate of V formation
    myVars->XanCycle_Vel.Vv2ABA = Vv2ABA;	//	The rate of conversion from v to ABA.
    myVars->XanCycle_Vel.VABAdg = VABAdg;	//	The rate of ABA degradation

    if (myVars->record) {
        myVars->XanCycle_VEL.insert(myVars->XanCycle_TIME_N, t, myVars->XanCycle_Vel);
        /*
        if (myVars->XanCycle_VEL.shape()[0] < myVars->XanCycle_TIME_N) {
            myVars->XanCycle_VEL.resize(boost::extents[myVars->XanCycle_TIME_N][XanCycle_VEL_SIZE]);
            myVars->XanCycle_CON.resize(boost::extents[myVars->XanCycle_TIME_N][XanCycle_CON_SIZE]);
        }

        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][0] = t;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][1] = Vva;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][2] = Vaz;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][3] = Vza;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][4] = Vav;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][5] = Vvf;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][6] = Vv2ABA;
        myVars->XanCycle_VEL[myVars->XanCycle_TIME_N - 1][7] = VABAdg;
        */
    }
    //myVars->XanCycle_CON[myVars->XanCycle_TIME_N - 1][0] = t;  // --unused
    //myVars->XanCycle_CON[myVars->XanCycle_TIME_N - 1][1] = Vx;  // --unused
    
    
    //arr XanCycle_Vel = zeros(7);
    
    
    //const double Xstate = Zx / (Vx + Ax + Zx);
    ABA = Vx / (Vx + Ax + Zx);
    
    //global XanCycle2FIBF_Xstate;
    // XanCycle2FI_Xstate = Xstate;// --unused
    
    //global Xan2Stom_ABA;
    
    //myVars->Xan2Stom_ABA = ABA;  // --unused
    
    //global XanCycle2OUT;
    // XanCycle2OUT = zeros(5, 1);
    myVars->XanCycle2OUT[0] = Vx;
    myVars->XanCycle2OUT[1] = Ax;
    myVars->XanCycle2OUT[2] = Zx;
    myVars->XanCycle2OUT[3] = ABA;
    //return XanCycle_Vel;
}
