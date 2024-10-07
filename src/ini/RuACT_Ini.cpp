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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

bool RuACT::EPS_connect = false;
bool RuACTCondition::EPS_connect = false;
double RuACT::TIME = 0.;
std::size_t RuACT::N = 1;

const std::size_t RuACTCondition::count = 4;

DEFINE_VALUE_SET_STATIC(RuACT);
DEFINE_VALUE_SET(RuACTCondition);
DEFINE_VALUE_SET_NS(RC::, RuACTRC);
DEFINE_VALUE_SET_NS(pool::, RuACTPool);

RuACTCondition* RuACT::_init(Variables *theVars) {
    RuACT::setEPS_connect(theVars->RuACT_EPS_com);
    RuACT::activase = 80. * theVars->RacRatio[10];

    // The rate constant used in the model
    theVars->RuACT_RC.k1 = 0.006 * theVars->RacRatio[0];                // The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco theVars->activase content or activity;
    theVars->RuACT_RC.kn1 = 1.6 * pow(10., -3.) * theVars->RacRatio[1]; // The rate constant of E inactivation by binding of RuBP;
    theVars->RuACT_RC.km1 = 20 * pow(10., -6.) * theVars->RacRatio[2];  // The michaelis menton constant for RuBP with E.
    theVars->RuACT_RC.Ke2 = 0.1 * theVars->RacRatio[3];                 // Data from Mate et al 1996. Unit: micormolar;
    theVars->RuACT_RC.Ke3 = 1.600 * theVars->RacRatio[4];               // Data from Mate et al 1996. Unit: micormolar;
    theVars->RuACT_RC.k6 = 2.5 * theVars->RacRatio[5] * 4. / 3.;          // micromolar per meter square per second, transfered to unit
    theVars->RuACT_RC.kc = 0.016 * theVars->RacRatio[6];                // Michaelis menton constant for CO2
    theVars->RuACT_RC.ko = 0.448 * theVars->RacRatio[7];                // Michaelis menton constant for O2
    theVars->RuACT_RC.k7 = theVars->RuACT_RC.k6 * 10 * theVars->RacRatio[8]; // The rate constant for ecm to ecmr
    theVars->RuACT_RC.kr = 20. * pow(10., -3.) * theVars->RacRatio[9];   // The apparaent michaelis menton constant for RuBP

    //factor = 0.224/0.3;
    const double factor = 1;
    RuACT::factor = factor;
    
    // Assign value to a variable that is transferred to the program
    RuACTCondition* RuACT_Con = new RuACTCondition();
    RuACT_Con->ER = 0.05 * 4. * factor;  // The concentration of inactive ER
    RuACT_Con->Eaf = 0.05 * 4. * factor; // The total concentration of E, EC, AND ECM
    RuACT_Con->ECMR = 0.2 * 4. * factor; // The concentration of ECMR
    RuACT_Con->RuBP = 2.;             // The concentration of ECMR

    theVars->RuACT_Pool.ET = 0.3 * 4. * factor * theVars->RacRatio[11]; //  The total concentraiton of E, ER, EC, ECM, ECMR , mM;
    theVars->RuACT_Pool.Rac = 0.0056 * theVars->RacRatio[12];          // The concentration of the activase, mM
    theVars->RuACT_Pool.C = 0.012 * theVars->RacRatio[13];             // mM
    theVars->RuACT_Pool.O = 0.260 * theVars->RacRatio[14];             // mM
    theVars->RuACT_Pool.M = 5. * theVars->RacRatio[15];

    return RuACT_Con;
}

void RuACT::_initAlt(Variables *theVars, RuACTCondition* RuACT_Con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<RuACT>();
    theVars->initParam(theVars->RuACT_RC);
    theVars->initParam(theVars->RuACT_Pool);
    theVars->initParam(*RuACT_Con);

    RuACT::set(MOD::RuACT::activase,
	       RuACT::get(MOD::RuACT::activase) *
	       theVars->RacRatio[10]);
    size_t i = 0;
    
    for (typename RC::RuACTRC::iterator it = theVars->RuACT_RC.begin();
	 it != theVars->RuACT_RC.end(); it++) {
      if (i == 10)
	break;
      switch (it->first) {
      case (RC::RuACT::k6) : {
	it->second = it->second * theVars->RacRatio[i] * 4.0 / 3.0;
	i++;
	break;
      }
      case (RC::RuACT::k7) : {
	it->second = it->second * theVars->RacRatio[i] * theVars->RuACT_RC[RC::RuACT::k6];
	i++;
	break;
      }
      default : {
	it->second = it->second * theVars->RacRatio[i];
	i++;
      }
      }
    }

    (*RuACT_Con)[COND::RuACT::ER] *= RuACT::get(MOD::RuACT::factor);
    (*RuACT_Con)[COND::RuACT::Eaf] *= RuACT::get(MOD::RuACT::factor);
    (*RuACT_Con)[COND::RuACT::ECMR] *= RuACT::get(MOD::RuACT::factor);

    i = 11;
    for (typename pool::RuACTPool::iterator it = theVars->RuACT_Pool.begin();
	 it != theVars->RuACT_Pool.end(); it++) {
      if (i == 16)
	break;
      switch (it->first) {
      case (POOL::RuACT::ET) : {
	it->second = it->second * theVars->RacRatio[i] * RuACT::get(MOD::RuACT::factor);
	i++;
	break;
      }
      default : {
	it->second = it->second * theVars->RacRatio[i];
	i++;
      }
      }
    }

    theVars->RuACT_RC.checkAlts("RuACT::_init::RuACT_RC: ");
    theVars->RuACT_Pool.checkAlts("RuACT::_init::RuACT_Pool: ");
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(RuACT_Con);
#endif // CHECK_VALUE_SET_ALTS
}

void RuACT::_checkAlts(Variables *theVars, const std::string& context) {
    theVars->RuACT_RC.checkAlts(context + "RuACT_RC:");
    theVars->RuACT_Pool.checkAlts(context + "RuACT_Pool:");
}

void RuACT::_updateAlts(Variables *theVars, const std::string& context) {
    theVars->RuACT_RC.updateAlts(context + "RuACT_RC:");
    theVars->RuACT_Pool.updateAlts(context + "RuACT_Pool:");
}
