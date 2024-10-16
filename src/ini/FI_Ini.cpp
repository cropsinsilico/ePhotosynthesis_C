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
 *    MERCHANTABILITY or FITNESS FOR 0 PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include <math.h>
#include "Variables.hpp"
#include "modules/FI.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double FI::TIME = 0.;
std::size_t FI::N = 1;
const std::size_t FICondition::count = 22;
bool FICondition::BF_connect = false;
bool FI::BF_connect = false;

DEFINE_VALUE_SET_STATIC(FI);
DEFINE_VALUE_SET(FICondition);
DEFINE_VALUE_SET_NS(RC::, FIRC);
DEFINE_VALUE_SET_NS(pool::, FIPool);

// This is the routine that initialize the parameters, initial conditions for simulation of fluorescence induction curve.
// The following information is initialized sequentially 1) Rate constants; 2) Initial concentration ( or conditions); 3) THe maximum
// concentration of components of photosystems.

void FI::_initCalc(Variables *theVars, FICondition* FI_Con) {
    if (theVars->useC3) {
        if (theVars->lightParam == 0.) {
            const double light_scaler = theVars->alfa * (1 - theVars->fc);
            theVars->lightParam = theVars->TestLi * 30 * light_scaler;
        }
        theVars->FI_RC.kA_d = theVars->EnzymeAct.at("kA_d");         // The rate constant of heat dissipation from peripheral antenna Lazar (1999), 0.25~1 *10^(9)
        theVars->FI_RC.kA_f = theVars->EnzymeAct.at("kA_f");         // The rate constant of fluorescence emission from peripheral antenna Lazar 1999, with a lifetime of 5 ns at closed reaction center
        theVars->FI_RC.kA_U = theVars->EnzymeAct.at("kA_U");         // The rate constant of exciton transfer from periphral antenna to core antenna Reference needed, a guess
        theVars->FI_RC.kU_A = theVars->EnzymeAct.at("kU_A");         // The rate constant of exciton transfer from core antenna to peripheral antenna Reference needed, a guess
        theVars->FI_RC.kU_d = theVars->EnzymeAct.at("kU_d");         // The rate constant of  heat emission from core antenna
        theVars->FI_RC.kU_f = theVars->EnzymeAct.at("kU_f");         // The rate constant of fluorescence emission from core antenna
        theVars->FI_RC.k1 = theVars->EnzymeAct.at("k1") * FI::cpsii;        // WY201803  The rate constant of primary charge separation for open reaction center
        theVars->FI_RC.k_r1 = theVars->EnzymeAct.at("k_r1");         // The rate constant of charge recombination for open reactoin center
        theVars->FI_RC.kz = theVars->EnzymeAct.at("kz");           // The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
        theVars->FI_RC.k12 = theVars->EnzymeAct.at("k12");          // The rate constant of the S1 to S2 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k23 = theVars->EnzymeAct.at("k23");         // The rate constant of the S2 to S3 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k30 = theVars->EnzymeAct.at("k30");         // The rate constant of the S3 to S0 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k01 = theVars->EnzymeAct.at("k01");         // The rate constant of the S0 to S1 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k2 = theVars->EnzymeAct.at("k2");          // The rate constant of the QA reduction by Pheo- Lazar (1999); 2~2.3 * 10^9
        theVars->FI_RC.kAB1 = theVars->EnzymeAct.at("kAB1");        // The rate constant of QAQB-->QAQB- Lazar (1999); 2.5~5 * 10^3
        theVars->FI_RC.kBA1 = theVars->EnzymeAct.at("kBA1");        // The rate constant of the QAQB- -->QAQB Lazar (1999)
        theVars->FI_RC.kAB2 = theVars->EnzymeAct.at("kAB2");        // The rate constant of the QAQB- --> QAQB2- Lazar (1999); 1.25~3.33 * 10^3
        theVars->FI_RC.kBA2 = theVars->EnzymeAct.at("kBA2");        // The rate constant of the QAQB2- --> QAQB-  Lazar (1999), or same as 3300 * theVars->FIRatio[16] depend on the equilibium constant
        theVars->FI_RC.k3 = theVars->EnzymeAct.at("k3");          // The rate constant of the exchange of PQ and QBH2 Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        theVars->FI_RC.k_r3 = theVars->EnzymeAct.at("k_r3");        // The rate constant of the exchange of QB and PQH2 Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        theVars->FI_RC.k_pq_oxy = theVars->EnzymeAct.at("k_pq_oxy");    // The rate constant of the PQH2 oxidation Lazar (1999),50~500
    } else {
        theVars->FI_RC.kA_d *= theVars->FIRatio[0];         // The rate constant of heat dissipation from peripheral antenna Lazar (1999), 0.25~1 *10^(9)
        theVars->FI_RC.kA_f *= theVars->FIRatio[1]; // The rate constant of fluorescence emission from peripheral antenna Lazar 1999, with a lifetime of 5 ns at closed reaction center
        theVars->FI_RC.kA_U *= theVars->FIRatio[2];            // The rate constant of exciton transfer from periphral antenna to core antenna Reference needed, a guess
        theVars->FI_RC.kU_A *= theVars->FIRatio[3];            // The rate constant of exciton transfer from core antenna to peripheral antenna Reference needed, a guess
        theVars->FI_RC.kU_d *= theVars->FIRatio[4];         // The rate constant of  heat emission from core antenna
        theVars->FI_RC.kU_f *= theVars->FIRatio[5]; // The rate constant of fluorescence emission from core antenna
        theVars->FI_RC.k1 *= theVars->FIRatio[6];        // The rate constant of primary charge separation for open reaction center
        theVars->FI_RC.k_r1 *= theVars->FIRatio[7];         // The rate constant of charge recombination for open reactoin center
        theVars->FI_RC.kz *= theVars->FIRatio[8];           // The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
        theVars->FI_RC.k12 *= theVars->FIRatio[9];                   // The rate constant of the S1 to S2 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k23 *= theVars->FIRatio[10];                  // The rate constant of the S2 to S3 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k30 *= theVars->FIRatio[11];                   // The rate constant of the S3 to S0 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k01 *= theVars->FIRatio[12];                    // The rate constant of the S0 to S1 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k2 *= theVars->FIRatio[13];          // The rate constant of the QA reduction by Pheo- Lazar (1999); 2~2.3 * 10^9
        theVars->FI_RC.kAB1 *= theVars->FIRatio[14];                  // The rate constant of QAQB-->QAQB- Lazar (1999); 2.5~5 * 10^3
        theVars->FI_RC.kBA1 *= theVars->FIRatio[15];                   // The rate constant of the QAQB- -->QAQB Lazar (1999)
        theVars->FI_RC.kAB2 *= theVars->FIRatio[16];                  // The rate constant of the QAQB- --> QAQB2- Lazar (1999); 1.25~3.33 * 10^3
        theVars->FI_RC.kBA2 *= theVars->FIRatio[17];                   // The rate constant of the QAQB2- --> QAQB-  Lazar (1999), or same as 3300 * theVars->FIRatio[16] depend on the equilibium constant
        theVars->FI_RC.k3 *= theVars->FIRatio[18];                     // The rate constant of the exchange of PQ and QBH2 Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        theVars->FI_RC.k_r3 *= theVars->FIRatio[19];                    // The rate constant of the exchange of QB and PQH2 Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        theVars->FI_RC.k_pq_oxy *= theVars->FIRatio[20];               // The rate constant of the PQH2 oxidation Lazar (1999),50~500

        theVars->FI_Pool.QBt *= theVars->FIRatio[21]; // The total concentration of Qb site;
        theVars->FI_Pool.PQT *= theVars->FIRatio[22]; // The total concentration of PQ;
    }
}

FICondition* FI::_init(Variables *theVars) {
    //////////////////////////////////////////////////////////////////////////
    // Initilization of the rate constant //
    ////////////////////////////////////////////////////////////////////////////
    FI::setBF_connect(theVars->BF_FI_com);
    // The rate constant used in the model
    // Reference
    // The rate constant used in the model

    if (theVars->useC3) {
        FI::cpsii = 1.;
        if (theVars->lightParam == 0.) {
            const double light_scaler = theVars->alfa * (1 - theVars->fc);
            theVars->lightParam = theVars->TestLi * 30 * light_scaler;
        }
        theVars->FI_RC.kA_d = theVars->EnzymeAct.at("kA_d");         // The rate constant of heat dissipation from peripheral antenna Lazar (1999), 0.25~1 *10^(9)
        theVars->FI_RC.kA_f = theVars->EnzymeAct.at("kA_f");         // The rate constant of fluorescence emission from peripheral antenna Lazar 1999, with a lifetime of 5 ns at closed reaction center
        theVars->FI_RC.kA_U = theVars->EnzymeAct.at("kA_U");         // The rate constant of exciton transfer from periphral antenna to core antenna Reference needed, a guess
        theVars->FI_RC.kU_A = theVars->EnzymeAct.at("kU_A");         // The rate constant of exciton transfer from core antenna to peripheral antenna Reference needed, a guess
        theVars->FI_RC.kU_d = theVars->EnzymeAct.at("kU_d");         // The rate constant of  heat emission from core antenna
        theVars->FI_RC.kU_f = theVars->EnzymeAct.at("kU_f");         // The rate constant of fluorescence emission from core antenna
        theVars->FI_RC.k1 = theVars->EnzymeAct.at("k1") * FI::cpsii;        // WY201803  The rate constant of primary charge separation for open reaction center
        theVars->FI_RC.k_r1 = theVars->EnzymeAct.at("k_r1");         // The rate constant of charge recombination for open reactoin center
        theVars->FI_RC.kz = theVars->EnzymeAct.at("kz");           // The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
        theVars->FI_RC.k12 = theVars->EnzymeAct.at("k12");          // The rate constant of the S1 to S2 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k23 = theVars->EnzymeAct.at("k23");         // The rate constant of the S2 to S3 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k30 = theVars->EnzymeAct.at("k30");         // The rate constant of the S3 to S0 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k01 = theVars->EnzymeAct.at("k01");         // The rate constant of the S0 to S1 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k2 = theVars->EnzymeAct.at("k2");          // The rate constant of the QA reduction by Pheo- Lazar (1999); 2~2.3 * 10^9
        theVars->FI_RC.kAB1 = theVars->EnzymeAct.at("kAB1");        // The rate constant of QAQB-->QAQB- Lazar (1999); 2.5~5 * 10^3
        theVars->FI_RC.kBA1 = theVars->EnzymeAct.at("kBA1");        // The rate constant of the QAQB- -->QAQB Lazar (1999)
        theVars->FI_RC.kAB2 = theVars->EnzymeAct.at("kAB2");        // The rate constant of the QAQB- --> QAQB2- Lazar (1999); 1.25~3.33 * 10^3
        theVars->FI_RC.kBA2 = theVars->EnzymeAct.at("kBA2");        // The rate constant of the QAQB2- --> QAQB-  Lazar (1999), or same as 3300 * theVars->FIRatio[16] depend on the equilibium constant
        theVars->FI_RC.k3 = theVars->EnzymeAct.at("k3");          // The rate constant of the exchange of PQ and QBH2 Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        theVars->FI_RC.k_r3 = theVars->EnzymeAct.at("k_r3");        // The rate constant of the exchange of QB and PQH2 Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        theVars->FI_RC.k_pq_oxy = theVars->EnzymeAct.at("k_pq_oxy");    // The rate constant of the PQH2 oxidation Lazar (1999),50~500
    } else {
        theVars->FI_RC.kA_d = 2 * pow(10, 8) * theVars->FIRatio[0];         // The rate constant of heat dissipation from peripheral antenna Lazar (1999), 0.25~1 *10^(9)
        theVars->FI_RC.kA_f = 6.3 * pow(10, 6) * 0.2 * theVars->FIRatio[1]; // The rate constant of fluorescence emission from peripheral antenna Lazar 1999, with a lifetime of 5 ns at closed reaction center
        theVars->FI_RC.kA_U = pow(10, 10) * theVars->FIRatio[2];            // The rate constant of exciton transfer from periphral antenna to core antenna Reference needed, a guess
        theVars->FI_RC.kU_A = pow(10, 10) * theVars->FIRatio[3];            // The rate constant of exciton transfer from core antenna to peripheral antenna Reference needed, a guess
        theVars->FI_RC.kU_d = 2 * pow(10, 8) * theVars->FIRatio[4];         // The rate constant of  heat emission from core antenna
        theVars->FI_RC.kU_f = 6.3 * pow(10, 6) * 0.2 * theVars->FIRatio[5]; // The rate constant of fluorescence emission from core antenna
        theVars->FI_RC.k1 = 2.5 * pow(10, 11) * theVars->FIRatio[6];        // The rate constant of primary charge separation for open reaction center
        theVars->FI_RC.k_r1 = 3 * pow(10, 8) * theVars->FIRatio[7];         // The rate constant of charge recombination for open reactoin center
        theVars->FI_RC.kz = 5 * pow(10, 6) * theVars->FIRatio[8];           // The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
        theVars->FI_RC.k12 = 30000 * theVars->FIRatio[9];                   // The rate constant of the S1 to S2 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k23 = 10000 * theVars->FIRatio[10];                  // The rate constant of the S2 to S3 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k30 = 3000 * theVars->FIRatio[11];                   // The rate constant of the S3 to S0 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k01 = 500 * theVars->FIRatio[12];                    // The rate constant of the S0 to S1 transition Lazar (1999); 0.667~33.3 * 10^3
        theVars->FI_RC.k2 = 2 * pow(10, 9) * theVars->FIRatio[13];          // The rate constant of the QA reduction by Pheo- Lazar (1999); 2~2.3 * 10^9
        theVars->FI_RC.kAB1 = 2500 * theVars->FIRatio[14];                  // The rate constant of QAQB-->QAQB- Lazar (1999); 2.5~5 * 10^3
        theVars->FI_RC.kBA1 = 200 * theVars->FIRatio[15];                   // The rate constant of the QAQB- -->QAQB Lazar (1999)
        theVars->FI_RC.kAB2 = 3300 * theVars->FIRatio[16];                  // The rate constant of the QAQB- --> QAQB2- Lazar (1999); 1.25~3.33 * 10^3
        theVars->FI_RC.kBA2 = 250 * theVars->FIRatio[17];                   // The rate constant of the QAQB2- --> QAQB-  Lazar (1999), or same as 3300 * theVars->FIRatio[16] depend on the equilibium constant
        theVars->FI_RC.k3 = 800 * theVars->FIRatio[18];                     // The rate constant of the exchange of PQ and QBH2 Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        theVars->FI_RC.k_r3 = 80 * theVars->FIRatio[19];                    // The rate constant of the exchange of QB and PQH2 Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        theVars->FI_RC.k_pq_oxy = 500 * theVars->FIRatio[20];               // The rate constant of the PQH2 oxidation Lazar (1999),50~500
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves for a dark adapted leaves;
    // Unit  micro mol per m2
    // Initialize the leaves for a dark adapted leaves;
    //  mircomol per m2

    // Initialize the leaves for a dark adapted leaves;
    //  Micro mol m2


    // Assign the value to a array
    // This is the program that initialize the major variables used in the fluorescence induction system.In this file, the n represent negative charges, _red represent that the components are associated with the closed reaction center; while _ox represent a system with open reaction center.
    FICondition* FI_Con = new FICondition();
    FI_Con->A = 0;          // The concentration of excitons in the peripheral antenna
    FI_Con->U = 0;          // The concentration fo excitons in the core antenna
    FI_Con->P680ePheo = 1;  // The concentration of the P680Pheo
    FI_Con->P680pPheon = 0; // The concentration for the P680+ Pheo-
    FI_Con->P680pPheo = 0;  // The concentration of P680+ Pheo
    FI_Con->P680Pheon = 0;  // The concentration of P680Pheo-
    FI_Con->Yz = 1;         // The concentration of reduced tyrosine
    FI_Con->S1T = 0.8;      // The concentration of S1 in combination with reduced tyrosine
    FI_Con->S2T = 0;        // The concentration of S2 in combination with reduced tyrosine
    FI_Con->S3T = 0;        // The concentration of S3 in combination with reduced tyrosine
    FI_Con->S0T = 0.2;      // The concentration of S0 in combination with reduced tyrosine
    FI_Con->S1Tp = 0;       // The concentration of S1 in combination with oxidized tyrosine
    FI_Con->S2Tp = 0;       // The concentration of S2 in combination with oxidized tyrosine
    FI_Con->S3Tp = 0;       // The concentration of S3 in combination with oxidized tyrosine
    FI_Con->S0Tp = 0;       // The concentration of S0 in combination with oxidized tyrosine
    FI_Con->QAQB = 1;       // The concentration of [QAQB]
    FI_Con->QAnQB = 0;      // The concentration of [QA-QB];
    FI_Con->QAQBn = 0;      // The concentration of [QAQB-]
    FI_Con->QAnQBn = 0;     // The concentration of [QA-QB-];
    FI_Con->QAQB2n = 0;     // The concentration of [QAQB2-]
    FI_Con->QAnQB2n = 0;    // The concentration of [QA-QB2-];
    FI_Con->PQn = 5;        // The concentration of reduced PQ, i.e. PQH2;
    if (theVars->useC3) {
        theVars->FI_Pool.QBt = 1; // The total concentration of Qb site;
        theVars->FI_Pool.PQT = 8; // The total concentration of PQ;
    } else {
        theVars->FI_Pool.QBt = 1 * theVars->FIRatio[21]; // The total concentration of Qb site;
        theVars->FI_Pool.PQT = 8 * theVars->FIRatio[22]; // The total concentration of PQ;
    }

    return FI_Con;
}

void FI::_initAlt(Variables *theVars, FICondition* FI_Con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<FI>();
    theVars->initParam(theVars->FI_RC);
    theVars->initParam(theVars->FI_Pool);
    theVars->initParam(*FI_Con);

    if (theVars->useC3) {
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kA_d, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kA_f, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kA_U, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kA_d, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kU_A, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kU_d, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kU_f, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k1, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k_r1, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kz, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k12, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k23, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k30, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k01, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k2, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kAB1, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kBA1, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kAB2, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::kBA2, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k3, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k_r3, theVars->EnzymeAct);
      theVars->FI_RC.setFromEnzymeAct(RC::FI::k_pq_oxy, theVars->EnzymeAct);
      theVars->FI_RC[RC::FI::k1] *= FI::get(MOD::FI::cpsii);
    } else {
      size_t i = 0;
      for (RC::FIRC::iterator it = theVars->FI_RC.begin();
	   it != theVars->FI_RC.end(); it++, i++) {
	if (i == 21)
	  break;
	CHECK_RATIO_IDX(i, 6, RC::FI::k1);
	it->second *= theVars->FIRatio[i];
      }
      theVars->FI_Pool[POOL::FI::QBt] *= theVars->FIRatio[21];
      theVars->FI_Pool[POOL::FI::PQT] *= theVars->FIRatio[22];
    }
    
    theVars->FI_RC.checkAlts("FI::_init::FI_RC: ");
    theVars->FI_Pool.checkAlts("FI::_init::FI_Pool: ");
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(FI_Con);
#endif // CHECK_VALUE_SET_ALTS
}

void FI::_checkAlts(Variables *theVars, const std::string& context) {
    theVars->FI_RC.checkAlts(context + "FI_RC:");
    theVars->FI_Pool.checkAlts(context + "FI_RC:");
}

void FI::_updateAlts(Variables *theVars, const std::string& context) {
    theVars->FI_RC.updateAlts(context + "FI_RC:");
    theVars->FI_Pool.updateAlts(context + "FI_RC:");
}
