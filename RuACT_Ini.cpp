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

RuACTCon RuACT::RuACT_Ini(Variables *myVars) {
    myVars->activase = 80 * myVars->RacRatio[10];

    // The rate constant used in the model
    myVars->RuACT_RC.k1 = 0.006 * myVars->RacRatio[0];	//	The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco myVars->activase content or activity;
    myVars->RuACT_RC.kn1 = 1.6 * pow(10, (-3)) * myVars->RacRatio[1];	//	The rate constant of E inactivation by binding of RuBP;
    myVars->RuACT_RC.km1 = 20 * pow(10, (-6)) * myVars->RacRatio[2];	//	The michaelis menton constant for RuBP with E.
    myVars->RuACT_RC.Ke2 = 0.1 * myVars->RacRatio[3];	//	Data from Mate et al 1996. Unit: micormolar;
    myVars->RuACT_RC.Ke3 = 1.600 * myVars->RacRatio[4];	//	Data from Mate et al 1996. Unit: micormolar;
    myVars->RuACT_RC.k6 = 2.5 * myVars->RacRatio[5] * 4 / 3;	//	micromolar per meter square per second, transfered to unit
    myVars->RuACT_RC.kc = 0.016 * myVars->RacRatio[6];	//	Michaelis menton constant for CO2
    myVars->RuACT_RC.ko = 0.448 * myVars->RacRatio[7];	//	Michaelis menton constant for O2
    myVars->RuACT_RC.k7 = myVars->RuACT_RC.k6 * 10 * myVars->RacRatio[8];	//	The rate constant for ecm to ecmr
    myVars->RuACT_RC.kr = 20 * pow(10, (-3)) * myVars->RacRatio[9];	//	The apparaent michaelis menton constant for RuBP

    //factor = 0.224/0.3;
    const double factor = 1;
    double RuBP = 2;

    if (myVars->RuACT_EPS_com)
        RuBP = myVars->PS2RA_RuBP_ini;


    // Assign value to a variable that is transferred to the program
    RuACTCon RuACT_Con;
    RuACT_Con.ER = 0.05 * 4 * factor;	//	The concentration of inactive ER
    RuACT_Con.Eaf = 0.05 * 4 * factor;	//	The total concentration of E, EC, AND ECM
    RuACT_Con.ECMR = 0.2 * 4 * factor;	//	The concentration of ECMR
    RuACT_Con.RuBP = RuBP;	//	The concentration of ECMR

    myVars->RuACT_Pool.ET = 0.3 * 4 * factor * myVars->RacRatio[11];// 	The total concentraiton of E, ER, EC, ECM, ECMR	, mM;
    myVars->RuACT_Pool.Rac = 0.0056 * myVars->RacRatio[12];//	The concentration of the activase, mM
    myVars->RuACT_Pool.C = 0.012 * myVars->RacRatio[13];//   mM
    myVars->RuACT_Pool.O = 0.260 * myVars->RacRatio[14];//   mM
    myVars->RuACT_Pool.M = 5 * myVars->RacRatio[15];

    return RuACT_Con;
}
