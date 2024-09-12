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
#include "ValueSet.hpp"
#include "modules/BF.hpp"
#define PMODTEM 1.
#define RT (8.314 * 298.)

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DEFINE_VALUE_SET_STATIC(BF);

INIT_MEMBER_STATIC(BF, cATPsyn);
INIT_MEMBER_STATIC(BF, CPSi);
INIT_MEMBER_STATIC(BF, cNADPHsyn);
INIT_MEMBER_STATIC(BF, EPS_ATP_Rate);

const std::size_t BFCondition::count = 28;
bool BFCondition::FI_connect = false;
bool BF::FI_connect = false;
bool BF::PS_connect = false;
bool BFCondition::PS_connect = false;
bool BF::RROEA_connect = false;
bool BFCondition::RROEA_connect = false;
std::size_t BF::N = 1;
double BF::TIME = 0.;

double BF::_Pi = 0.;

BFCondition* BF::_init(Variables *theVars) {

    BF::setFI_connect(theVars->BF_FI_com);
    BF::setPS_connect(theVars->FIBF_PSPR_com);
    BF::setRROEA_connect(theVars->RROEA_EPS_com);
    // Indicate in the beginning there is no ATP synthesis activity.
    BF::EPS_ATP_Rate = 0.;
    theVars->initParamStatic<BF>();
    theVars->initParam(theVars->BF_RC);
    theVars->initParam(theVars->BF_Pool);
    BFCondition* BF_con = new BFCondition();
    theVars->initParam(*BF_con);

    theVars->BF_RC[RC::BF::PK] *= PMODTEM;
    theVars->BF_RC[RC::BF::PMg] *= PMODTEM;
    theVars->BF_RC[RC::BF::PCl] *= PMODTEM;

    if (theVars->useC3) {
      if (theVars->lightParam == 0.) {
	const double light_scaler = theVars->alfa * (1. - theVars->fc);
	theVars->lightParam = theVars->TestLi * 30. * light_scaler;
      }
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K1, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K2, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K3, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K4, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K5, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K6, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K7, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K8, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K9, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K10, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::Vmax11, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::Kau, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::Kua, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::Kf, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::Kd, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K15, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::K16, theVars->EnzymeAct);
      theVars->BF_RC.setFromEnzymeAct(RC::BF::V2M, theVars->EnzymeAct);

      theVars->BF_RC[RC::BF::Vmax11] *= BF::get(MOD::BF::cATPsyn);
      theVars->BF_RC[RC::BF::K15] *= BF::get(MOD::BF::CPSi);
      theVars->BF_RC[RC::BF::V2M] *= BF::get(MOD::BF::cNADPHsyn);
      
    } else {
      
      int i = 0;
      for (typename RC::BFRC::iterator it = theVars->BF_RC.begin();
           it != theVars->BF_RC.end(); it++) {
	if (i >= 37)
	  break;
        switch(it->first) {
	case (RC::BF::KE8) :
	case (RC::BF::KE9) : {
	  // Not scalled by BFRatio because calculated from
	  //   Em_IPS, Em_Cytf, & Em_PG
	  break;
	}
        case (RC::BF::Em_Cytf) : {
	  if (i != 22)
	    throw std::runtime_error("Mismatch at Em_IPS: 22 vs "
				     + std::to_string(i));
          it->second *= theVars->BFRatio[i];
	  // BFRatio 23 & 24 not used?
	  i = 25;
          break;
        }
        default : {
          it->second *= theVars->BFRatio[i];
	  i++;
        }
        }
      }
      
      i = 37;
      for (typename pool::BFPool::iterator it = theVars->BF_Pool.begin();
           it != theVars->BF_Pool.end(); it++, i++) {
        if (i >= 49)
          break;
        it->second *= theVars->BFRatio[i];
      }

      i = 49;

      // ISPHr + cytc1 --> ISPHox + cytc1-
      double DeltaEm = theVars->BF_RC[RC::BF::Em_Cytf]
	- theVars->BF_RC[RC::BF::Em_IPS];
      double DeltaG = DeltaEm * -9.649 * pow(10., 4.);
      theVars->BF_RC[RC::BF::KE8] = exp(-DeltaG / RT);
      // cytc1- + cytc2 --> cytc1 + cytc2-
      DeltaEm = theVars->BF_RC[RC::BF::Em_PG]
	- theVars->BF_RC[RC::BF::Em_Cytf];
      DeltaG = DeltaEm * -9.649 * pow(10., 4.);
      theVars->BF_RC[RC::BF::KE9] = exp(-DeltaG / RT);
      
    }


#ifdef CHECK_VALUE_SET_ALTS
    double Em_IPS = 0.31;
    double Em_Cytf = 0.27;
    double Em_PG = 0.35;
    theVars->BF_RC.K1 = pow(10., 6.);
    theVars->BF_RC.K2 = 500.;
    theVars->BF_RC.K3 = 5. * pow(10., 7.);
    theVars->BF_RC.K4 = 5. * pow(10., 7.);
    theVars->BF_RC.K5 = 5. * pow(10., 7.);
    theVars->BF_RC.K6 = 5. * pow(10., 7.);
    theVars->BF_RC.K7 = pow(10., 4.);
    theVars->BF_RC.K8 = 1000.;
    theVars->BF_RC.K9 = 8.3 * pow(10., 6.);
    theVars->BF_RC.K10 = 8. * pow(10., 8.);
    theVars->BF_RC.Vmax11 = 6.;
    theVars->BF_RC.Kqi = pow(10., 3.);
    theVars->BF_RC.PK = 3.6 * pow(10., -8.);
    theVars->BF_RC.PMg = 3.6 * pow(10., -8.);
    theVars->BF_RC.PCl = 1.8 * pow(10., -8.);
    theVars->BF_RC.Kau = pow(10., 10.);
    theVars->BF_RC.Kua = pow(10., 10.);
    theVars->BF_RC.Kf = 6.3 * pow(10., 6.);
    theVars->BF_RC.Kd = 2. * pow(10., 8.);
    theVars->BF_RC.K15 = pow(10., 10.);
    theVars->BF_RC.K16 = pow(10., 5.);
    theVars->BF_RC.MemCap = 0.6 * pow(10., -6.);
    theVars->BF_RC.RVA = 8. * pow(10., -10.);
    theVars->BF_RC.KBs = 1.1 * pow(10., -8.);
    theVars->BF_RC.KBl = 5.1 * pow(10., -6.);
    theVars->BF_RC.KM1ATP = 0.12;
    theVars->BF_RC.KM1ADP = 0.014;
    theVars->BF_RC.KM1PI = 0.3;
    theVars->BF_RC.KM2NADP = 0.05;
    theVars->BF_RC.KM2NADPH = 0.035;
    theVars->BF_RC.V2M = 27.8;
    theVars->BF_RC.KE2 = 495.;
    
    theVars->BF_RC.PK *= PMODTEM;
    theVars->BF_RC.PMg *= PMODTEM;
    theVars->BF_RC.PCl *= PMODTEM;
    
    if (theVars->useC3) {
      // Set values from loaded EnzymeAct
      // This may be deprecated infavor of setting via RC parameter file
      BF::cNADPHsyn = 1.;
      BF::CPSi = 1.;
      BF::cATPsyn = 1.;
      // cATPsyn=1.0447;%1.01866 WY201803
      // CPSi=1.0131;% 1.0237 WY201803
      // cNADPHsyn=1.094468408;%1.0388 WY201803
      if (theVars->lightParam == 0.) {
	const double light_scaler = theVars->alfa * (1. - theVars->fc);
	theVars->lightParam = theVars->TestLi * 30. * light_scaler;
      }
      
      // Assign values to the array for rate constant
      theVars->BF_RC.K1 = theVars->EnzymeAct.at("K1");     // The rate constant for formation of ISP.QH2 complex; unit:  per second
      theVars->BF_RC.K2 = theVars->EnzymeAct.at("K2");     // The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
      theVars->BF_RC.K3 = theVars->EnzymeAct.at("K3");     // The rate constant for QH. + cytbL --> Q + cytbL- + H+ Unit: s-1
      theVars->BF_RC.K4 = theVars->EnzymeAct.at("K4");     // The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
      theVars->BF_RC.K5 = theVars->EnzymeAct.at("K5");     // The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
      theVars->BF_RC.K6 = theVars->EnzymeAct.at("K6");     // The rate constant  for CytbH- + Q- --> cytbH + Q2- Unit: s-1
      theVars->BF_RC.K7 = theVars->EnzymeAct.at("K7");     // The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website Unit: s-1
      theVars->BF_RC.K8 = theVars->EnzymeAct.at("K8");     // The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+ Unit: s-1
      theVars->BF_RC.K9 = theVars->EnzymeAct.at("K9");     // The rate constant for the electron transport from cytc1 to cytc2 Unit: s-1
      theVars->BF_RC.K10 = theVars->EnzymeAct.at("K10");    // The rate constant for the electron transport from cytc2 to P700 Unit: s-1
      theVars->BF_RC.Vmax11 = theVars->EnzymeAct.at("Vmax11") * BF::cATPsyn;  // The maximum rate of ATP synthesis Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
      theVars->BF_RC.Kau = theVars->EnzymeAct.at("Kau");   // The rate constant for exciton transfer from perpheral antenna to core antenna, see FI Unit: s-1
      theVars->BF_RC.Kua = theVars->EnzymeAct.at("Kua");   // The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI Unit: s-1
      theVars->BF_RC.Kf = theVars->EnzymeAct.at("Kf");    // The rate constant for fluorescence emission, see the note in FI Unit: s-1
      theVars->BF_RC.Kd = theVars->EnzymeAct.at("Kd");    // The rate constant for heat dissipation; see the note for FI Unit: s-1
      theVars->BF_RC.K15 = theVars->EnzymeAct.at("K15") * BF::CPSi;     // The rate constant for primary charge separation in PSI Unit: s-1
      theVars->BF_RC.K16 = theVars->EnzymeAct.at("K16");    // The rate constant for electron tranfer from electron acceptor of PSI to Fd Unit: s-1
      theVars->BF_RC.V2M = theVars->EnzymeAct.at("V2M") * BF::cNADPHsyn;     // The maximum rate of NADPH formation Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    } else {
      theVars->BF_RC.K1 *= theVars->BFRatio[0];
      theVars->BF_RC.K2 *= theVars->BFRatio[1];
      theVars->BF_RC.K3 *= theVars->BFRatio[2];
      theVars->BF_RC.K4 *= theVars->BFRatio[3];
      theVars->BF_RC.K5 *= theVars->BFRatio[4];
      theVars->BF_RC.K6 *= theVars->BFRatio[5];
      theVars->BF_RC.K7 *= theVars->BFRatio[6];
      theVars->BF_RC.K8 *= theVars->BFRatio[7];
      theVars->BF_RC.K9 *= theVars->BFRatio[8];
      theVars->BF_RC.K10 *= theVars->BFRatio[9];
      theVars->BF_RC.Vmax11 *= theVars->BFRatio[10];
      theVars->BF_RC.Kqi *= theVars->BFRatio[11];
      theVars->BF_RC.PK *= theVars->BFRatio[12];
      theVars->BF_RC.PMg *= theVars->BFRatio[13];
      theVars->BF_RC.PCl *= theVars->BFRatio[14];
      theVars->BF_RC.Kau *= theVars->BFRatio[15];
      theVars->BF_RC.Kua *= theVars->BFRatio[16];
      theVars->BF_RC.Kf *= theVars->BFRatio[17];
      theVars->BF_RC.Kd *= theVars->BFRatio[18];
      // KE8 & KE9 skipped for BFRatio as they are not parametrized directly
      theVars->BF_RC.K15 *= theVars->BFRatio[19];
      theVars->BF_RC.K16 *= theVars->BFRatio[20];
      Em_IPS *= theVars->BFRatio[21];
      Em_Cytf *= theVars->BFRatio[22];
      // BFRatio 23 & 24 not used?
      Em_PG *= theVars->BFRatio[25];
      theVars->BF_RC.MemCap *= theVars->BFRatio[26];
      theVars->BF_RC.RVA *= theVars->BFRatio[27];
      theVars->BF_RC.KBs *= theVars->BFRatio[28];
      theVars->BF_RC.KBl *= theVars->BFRatio[29];
      theVars->BF_RC.KM1ATP *= theVars->BFRatio[30];
      theVars->BF_RC.KM1ADP *= theVars->BFRatio[31];
      theVars->BF_RC.KM1PI *= theVars->BFRatio[32];
      theVars->BF_RC.KM2NADP *= theVars->BFRatio[33];
      theVars->BF_RC.KM2NADPH *= theVars->BFRatio[34];
      theVars->BF_RC.V2M *= theVars->BFRatio[35];
      theVars->BF_RC.KE2 *= theVars->BFRatio[36];
    }
    // ISPHr + cytc1 --> ISPHox + cytc1-
    double DeltaEm = Em_Cytf - Em_IPS;
    double DeltaG = DeltaEm * -9.649 * pow(10., 4.);
    theVars->BF_RC.KE8 = exp(-DeltaG / RT);  // ISPHr + cytc1 --> ISPHox + cytc1- Unit: s-1
    // cytc1- + cytc2 --> cytc1 + cytc2-
    DeltaEm = Em_PG - Em_Cytf;
    DeltaG = DeltaEm * -9.649 * pow(10., 4.);
    theVars->BF_RC.KE9 = exp(-DeltaG / RT);  // cytc1- + cytc2 --> cytc1 + cytc2- Unit: s-1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves for a dark adapted leaves;
    // Unit  micro mol per m2 or mmol l-2 stroma volume

    // The initialization of the model with concentration of each substrate in dark-adapted leaves
    // The total concentration of PSII is assumed to be 1 micromole per meter square

    //  This is the initialization step for the module of the Q cycle, and ATP synthesis steps

    BF_con->ISPHr = 0.;     // The reduced ion sulfer protein (ISPH)
    BF_con->cytc1 = 1.;     // The oxidized state of cytc1
    BF_con->ISPo = 1.;      // The oxidized ion sulfer protein (ISP)
    BF_con->ISPoQH2 = 0.;   // The complex of oxidized ion sulfer protein and reduced quinone
    BF_con->QHsemi = 0.;    // Semiquinone

    BF_con->cytbL = 1.;     // The oxidized cytbL
    BF_con->Qi = 0.;        // The binding Quinone
    BF_con->Q = 1.;         // Quinone
    BF_con->cytbH = 1.;     // The oxidized form of cytbH
    BF_con->Qn = 0.;        // Q-

    BF_con->Qr = 0.;        // Q2-
    BF_con->QH2 = 5.;       // QH2
    BF_con->cytc2 = 1.;     // oxidized cytc2
    BF_con->P700 = 0.5;     // The reduced state of P700, including both P700 and excited P700
    BF_con->ADP = 0.82;     // ADP in stroma

    BF::_Pi = 0.9;       // Phosphate in stroma
    BF_con->ATP = 0.68;     // ATP in stroma
    BF_con->Ks = 10.;       // K ions in stroma
    BF_con->Mgs = 5.;       // Mg ions in stroma
    BF_con->Cls = 1.;       // Cl ions in stroma

    BF_con->Aip = 0.;       // The number of photons in peripheral antenna
    BF_con->U = 0.;         // The number of photons in core antenna
    BF_con->An = 0.;        // The reduced electron acceptor in PSI
    BF_con->Fdn = 0.3;      // The reduced ferrodoxin
    BF_con->BFHs = 19.0001; // The total concentration of proton and protonated buffer species in stroma, put in unit: mmol l-1
    BF_con->BFHl = 19.0001; // The total concentration of proton and protonated buffer species in lumen, unit: mmol l-1

    BF_con->PHs = 7.;       // The PH value of the stroma
    BF_con->PHl = 7.;       // The PH value of the lumen
    BF_con->NADPH = 0.21;   // The NADPH concentration in stroma, Unit: mmol l-1;

    // Assigning the pool variables
    // The sizes of different pools in the model

    // Assign the pools to the global pool variables
    theVars->BF_Pool.kA_d = 1.;
    theVars->BF_Pool.kA_f = 1.;
    theVars->BF_Pool.kA_U = 20.;
    theVars->BF_Pool.kU_A = 10.;
    theVars->BF_Pool.kU_d = 2.;
    theVars->BF_Pool.kU_f = 1.;
    theVars->BF_Pool.k1 = 1.;
    theVars->BF_Pool.k_r1 = 8.;
    theVars->BF_Pool.kz = 38.;
    theVars->BF_Pool.k12 = 38.;
    theVars->BF_Pool.k23 = 1.;
    theVars->BF_Pool.k30 = 1.;
    if (!theVars->useC3) {
      theVars->BF_Pool.kA_d *= theVars->BFRatio[37];
      theVars->BF_Pool.kA_f *= theVars->BFRatio[38];
      theVars->BF_Pool.kA_U *= theVars->BFRatio[39];
      theVars->BF_Pool.kU_A *= theVars->BFRatio[40];
      theVars->BF_Pool.kU_d *= theVars->BFRatio[41];
      theVars->BF_Pool.kU_f *= theVars->BFRatio[42];
      theVars->BF_Pool.k1 *= theVars->BFRatio[43];
      theVars->BF_Pool.k_r1 *= theVars->BFRatio[44];
      theVars->BF_Pool.kz *= theVars->BFRatio[45];
      theVars->BF_Pool.k12 *= theVars->BFRatio[46];
      theVars->BF_Pool.k23 *= theVars->BFRatio[47];
      theVars->BF_Pool.k30 *= theVars->BFRatio[48];
    }

    theVars->BF_RC.checkAlts();
    theVars->BF_Pool.checkAlts();
    BF_con->checkAlts();
#endif // CHECK_VALUE_SET_ALTS
    
    return BF_con;
}
