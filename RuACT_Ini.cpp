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



arr RuACT_Ini(varptr *myVars) {
    //global RacRatio;
    const double k1 = 0.006 * myVars->RacRatio[0];//	The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco myVars->activase content or activity;
    
    const double kn1 = 1.6 * pow(10, (-3)) * myVars->RacRatio[1];
    
    
    const double km1 = 20 * pow(10, (-6)) * myVars->RacRatio[2];
    
    const double Ke2 = 0.1 * myVars->RacRatio[3];
    const double Ke3 = 1.600 * myVars->RacRatio[4];
    
    const double k6 = 2.5 * myVars->RacRatio[5] * 4 / 3;//WY201803 //Rubisco activity initial=80; max=120
    //k6	=	15.5/4  *RacRatio(6);	 		//WY201806
    
    const double kc = 0.016 * myVars->RacRatio[6];//	Michaelis menton constant for CO2, 0.016 mM
    const double ko = 0.448 * myVars->RacRatio[7];//	Michaelis menton constant for O2, .448 mM
    
    const double k7 = k6 * 10 * myVars->RacRatio[8];//	The rate constant for ecm to ecmr		mM-1 s-1
    const double kr = 20 * pow(10, (-3)) * myVars->RacRatio[9];
    
    //global activase;
    myVars->activase = 80 * myVars->RacRatio[10];
    
    //global RuACT_RC;
    // RuACT_RC = zeros(5, 1);
    
    // The rate constant used in the model
    myVars->RuACT_RC.k1 = k1;	//	The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco myVars->activase content or activity;
    myVars->RuACT_RC.kn1 = kn1;	//	The rate constant of E inactivation by binding of RuBP;
    myVars->RuACT_RC.km1 = km1;	//	The michaelis menton constant for RuBP with E.
    myVars->RuACT_RC.Ke2 = Ke2;	//	Data from Mate et al 1996. Unit: micormolar;
    myVars->RuACT_RC.Ke3 = Ke3;	//	Data from Mate et al 1996. Unit: micormolar;
    myVars->RuACT_RC.k6 = k6;	//	micromolar per meter square per second, transfered to unit
    myVars->RuACT_RC.kc = kc;	//	Michaelis menton constant for CO2
    myVars->RuACT_RC.ko = ko;	//	Michaelis menton constant for O2
    myVars->RuACT_RC.k7 = k7;	//	The rate constant for ecm to ecmr
    myVars->RuACT_RC.kr = kr;	//	The apparaent michaelis menton constant for RuBP
    
    //factor = 0.224/0.3;
    const double factor = 1;
    const double ER = 0.05 * 4 * factor;// 	The concentration of inactive ER
    const double Eaf = 0.05 * 4 * factor;//	The total concentration of E, EC, AND ECM
    const double ECMR = 0.2 * 4 * factor;// 	The concentration of ECMR
    double RuBP = 2;
    
    //global RuACT_EPS_com;
    //global PS2RA_RuBP_ini;
    
    if (myVars->RuACT_EPS_com)
        RuBP = myVars->PS2RA_RuBP_ini;
    
    
    // Assign value to a variable that is transferred to the program
    arr RuACT_Con = zeros(4);
    
    RuACT_Con[0] = ER;	//	The concentration of inactive ER
    RuACT_Con[1] = Eaf;	//	The total concentration of E, EC, AND ECM
    RuACT_Con[2] = ECMR;	//	The concentration of ECMR
    RuACT_Con[3] = RuBP;	//	The concentration of ECMR
    
    const double ET = 0.3 * 4 * factor * myVars->RacRatio[11];// 	The total concentraiton of E, ER, EC, ECM, ECMR	, mM
    const double Rac = 0.0056 * myVars->RacRatio[12];//	The concentration of the activase, mM
    const double C = 0.012 * myVars->RacRatio[13];//   mM
    const double O = 0.260 * myVars->RacRatio[14];//   mM
    const double M = 5 * myVars->RacRatio[15];
    
    //global RuACT_Pool;
    // RuACT_Pool = zeros(2, 1);
    myVars->RuACT_Pool.ET = ET;
    myVars->RuACT_Pool.Rac = Rac;
    myVars->RuACT_Pool.C = C;
    myVars->RuACT_Pool.O = O;
    myVars->RuACT_Pool.M = M;
    return RuACT_Con;
}
