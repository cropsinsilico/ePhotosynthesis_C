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
#include "modules/RuACT.hpp"

const size_t RuACTContainer::count = 4;

RuACTContainer* RuACT::_init(Variables *theVars) {
    theVars->activase = 80 * theVars->RacRatio[10];

    // The rate constant used in the model
    theVars->RuACT_RC.k1 = 0.006 * theVars->RacRatio[0];                // The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco theVars->activase content or activity;
    theVars->RuACT_RC.kn1 = 1.6 * pow(10, (-3)) * theVars->RacRatio[1]; // The rate constant of E inactivation by binding of RuBP;
    theVars->RuACT_RC.km1 = 20 * pow(10, (-6)) * theVars->RacRatio[2];  // The michaelis menton constant for RuBP with E.
    theVars->RuACT_RC.Ke2 = 0.1 * theVars->RacRatio[3];                 // Data from Mate et al 1996. Unit: micormolar;
    theVars->RuACT_RC.Ke3 = 1.600 * theVars->RacRatio[4];               // Data from Mate et al 1996. Unit: micormolar;
    theVars->RuACT_RC.k6 = 2.5 * theVars->RacRatio[5] * 4 / 3;          // micromolar per meter square per second, transfered to unit
    theVars->RuACT_RC.kc = 0.016 * theVars->RacRatio[6];                // Michaelis menton constant for CO2
    theVars->RuACT_RC.ko = 0.448 * theVars->RacRatio[7];                // Michaelis menton constant for O2
    theVars->RuACT_RC.k7 = theVars->RuACT_RC.k6 * 10 * theVars->RacRatio[8]; // The rate constant for ecm to ecmr
    theVars->RuACT_RC.kr = 20 * pow(10, (-3)) * theVars->RacRatio[9];   // The apparaent michaelis menton constant for RuBP

    //factor = 0.224/0.3;
    const double factor = 1;

    // Assign value to a variable that is transferred to the program
    RuACTContainer* RuACT_Con = new RuACTContainer();
    RuACT_Con->ER = 0.05 * 4 * factor;  // The concentration of inactive ER
    RuACT_Con->Eaf = 0.05 * 4 * factor; // The total concentration of E, EC, AND ECM
    RuACT_Con->ECMR = 0.2 * 4 * factor; // The concentration of ECMR
    RuACT_Con->RuBP = 2.;             // The concentration of ECMR

    theVars->RuACT_Pool.ET = 0.3 * 4 * factor * theVars->RacRatio[11]; //  The total concentraiton of E, ER, EC, ECM, ECMR , mM;
    theVars->RuACT_Pool.Rac = 0.0056 * theVars->RacRatio[12];          // The concentration of the activase, mM
    theVars->RuACT_Pool.C = 0.012 * theVars->RacRatio[13];             // mM
    theVars->RuACT_Pool.O = 0.260 * theVars->RacRatio[14];             // mM
    theVars->RuACT_Pool.M = 5 * theVars->RacRatio[15];

    return RuACT_Con;
}
