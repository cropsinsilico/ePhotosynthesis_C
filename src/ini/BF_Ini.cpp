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
#include "modules/BF.hpp"

const size_t BFCondition::count = 28;
bool BFCondition::FI_connect = false;
bool BF::FI_connect = false;
bool BF::PS_connect = false;
bool BFCondition::PS_connect = false;
bool BF::RROEA_connect = false;
bool BFCondition::RROEA_connect = false;
double BF::TIME = 0.;
size_t BF::N = 1;

double BF::_Pi = 0.;

BFCondition* BF::_init(Variables *theVars) {

    const double PMODTEM = 1;
    BF::setFI_connect(theVars->BF_FI_com);
    BF::setPS_connect(theVars->FIBF_PSPR_com);
    BF::setRROEA_connect(theVars->RROEA_EPS_com);
    if (theVars->useC3) {
        theVars->cNADPHsyn = 1.;
        theVars->CPSi = 1.;
        theVars->cATPsyn = 1.;
        theVars->cpsii = 1.;
        // cATPsyn=1.0447;%1.01866 WY201803
        // CPSi=1.0131;% 1.0237 WY201803
        // cNADPHsyn=1.094468408;%1.0388 WY201803


        // ISPHr + cytc1 --> ISPHox + cytc1-
        double DeltaEm = 0.27 - 0.31;
        double DeltaG = DeltaEm * (-9.649) * pow(10, 4);
        double RT = 8.314 * 298;
        const double KE8 = exp(-DeltaG / RT);

        // cytc1- + cytc2 --> cytc1 + cytc2-
        DeltaEm = 0.35 - 0.27;
        DeltaG = DeltaEm * (-9.649) * pow(10, 4);
        RT = 8.314 * 298;
        const double KE9 = exp(-DeltaG / RT);

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
        theVars->BF_RC.Vmax11 = theVars->EnzymeAct.at("Vmax11") * theVars->cATPsyn;  // The maximum rate of ATP synthesis Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.Kqi = pow(10, 3);            // The rate constant for uptake of two protons from the stroma to Q2- s-1
        theVars->BF_RC.PK = 3.6 * pow(10, (-8)) * PMODTEM;  // The permeability constant for K Unit: cm s-1
        theVars->BF_RC.PMg = 3.6 * pow(10, (-8)) * PMODTEM; // The permeability constant for Mg Unit: cm s-1
        theVars->BF_RC.PCl = 1.8 * pow(10, (-8)) * PMODTEM; // The permeability constant for Cl Unit: cm s-1
        theVars->BF_RC.Kau = theVars->EnzymeAct.at("Kau");   // The rate constant for exciton transfer from perpheral antenna to core antenna, see FI Unit: s-1
        theVars->BF_RC.Kua = theVars->EnzymeAct.at("Kua");   // The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI Unit: s-1
        theVars->BF_RC.Kf = theVars->EnzymeAct.at("Kf");    // The rate constant for fluorescence emission, see the note in FI Unit: s-1
        theVars->BF_RC.Kd = theVars->EnzymeAct.at("Kd");    // The rate constant for heat dissipation; see the note for FI Unit: s-1
        theVars->BF_RC.KE8 = KE8;                   // ISPHr + cytc1 --> ISPHox + cytc1- Unit: s-1
        theVars->BF_RC.KE9 = KE9;                   // cytc1- + cytc2 --> cytc1 + cytc2- Unit: s-1
        theVars->BF_RC.K15 = theVars->EnzymeAct.at("K15") * theVars->CPSi;     // The rate constant for primary charge separation in PSI Unit: s-1
        theVars->BF_RC.K16 = theVars->EnzymeAct.at("K16");    // The rate constant for electron tranfer from electron acceptor of PSI to Fd Unit: s-1
        theVars->BF_RC.MemCap = 0.6 * pow(10, (-6)); // The membrane capacity
        theVars->BF_RC.RVA = 8 * pow(10, (-10));     // The ratio of lumen volume to thylakoid membrane area
        theVars->BF_RC.KBs = 1.1 * pow(10, (-8));    // The buffer equilibrium constant in stroma
        theVars->BF_RC.KBl = 5.1 * pow(10, (-6));    // The buffer equilibrium constant in lumen
        theVars->BF_RC.KM1ATP = 0.12;                // The michaelis menton constant for ATP for ATP synthesis
        theVars->BF_RC.KM1ADP = 0.014;               // The michaelis menton constant for ATP for ADP synthesis
        theVars->BF_RC.KM1PI = 0.3;                  // The michaelis menton constant for ATP for PI synthesis
        theVars->BF_RC.KM2NADP = 0.05;               // The michaelis menten constant for NADP Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.KM2NADPH = 0.035;             // The michaelis menten constant for NADPH Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.V2M = theVars->EnzymeAct.at("V2M") * theVars->cNADPHsyn;     // The maximum rate of NADPH formation Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.KE2 = 495;                    // Equilibrium constatn
    } else {


        // ISPHr + cytc1 --> ISPHox + cytc1-
        double DeltaEm = 0.27 * theVars->BFRatio[22] - 0.31 * theVars->BFRatio[21];
        double DeltaG = DeltaEm * (-9.649) * pow(10, 4);
        double RT = 8.314 * 298;
        const double KE8 = exp(-DeltaG / RT);

        // cytc1- + cytc2 --> cytc1 + cytc2-
        DeltaEm = 0.35 * theVars->BFRatio[25] - 0.27 * theVars->BFRatio[22];
        DeltaG = DeltaEm * (-9.649) * pow(10, 4);
        RT = 8.314 * 298;
        const double KE9 = exp(-DeltaG / RT);

        theVars->BF_RC.K1 = pow(10, 6) * theVars->BFRatio[0];     // The rate constant for formation of ISP.QH2 complex; unit:  per second
        theVars->BF_RC.K2 = 500 * theVars->BFRatio[1];            // The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
        theVars->BF_RC.K3 = 5 * pow(10, 7) * theVars->BFRatio[2]; // The rate constant for QH. + cytbL --> Q + cytbL- + H+ Unit: s-1
        theVars->BF_RC.K4 = 5 * pow(10, 7) * theVars->BFRatio[3]; // The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
        theVars->BF_RC.K5 = 5 * pow(10, 7) * theVars->BFRatio[4]; // The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
        theVars->BF_RC.K6 = 5 * pow(10, 7) * theVars->BFRatio[5]; // The rate constant  for CytbH- + Q- --> cytbH + Q2- Unit: s-1
        theVars->BF_RC.K7 = pow(10, 4) * theVars->BFRatio[6];     // The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website Unit: s-1
        theVars->BF_RC.K8 = 1000 * theVars->BFRatio[7];           // The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+ Unit: s-1
        theVars->BF_RC.K9 = 8.3 * pow(10, 6) * theVars->BFRatio[8]; // The rate constant for the electron transport from cytc1 to cytc2 Unit: s-1
        theVars->BF_RC.K10 = 8 * pow(10, 8) * theVars->BFRatio[9];  // The rate constant for the electron transport from cytc2 to P700 Unit: s-1
        theVars->BF_RC.Vmax11 = 6 * theVars->BFRatio[10];         // The maximum rate of ATP synthesis Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.Kqi = pow(10, 3) * theVars->BFRatio[11];   // The rate constant for uptake of two protons from the stroma to Q2- s-1
        theVars->BF_RC.PK = 3.6 * pow(10, (-8)) * PMODTEM * theVars->BFRatio[12];  // The permeability constant for K Unit: cm s-1
        theVars->BF_RC.PMg = 3.6 * pow(10, (-8)) * PMODTEM * theVars->BFRatio[13]; // The permeability constant for Mg Unit: cm s-1
        theVars->BF_RC.PCl = 1.8 * pow(10, (-8)) * PMODTEM * theVars->BFRatio[14]; // The permeability constant for Cl Unit: cm s-1
        theVars->BF_RC.Kau = pow(10, 10) * theVars->BFRatio[15];     // The rate constant for exciton transfer from perpheral antenna to core antenna, see FI Unit: s-1
        theVars->BF_RC.Kua = pow(10, 10) * theVars->BFRatio[16];     // The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI Unit: s-1
        theVars->BF_RC.Kf = 6.3 * pow(10, 6) * theVars->BFRatio[17]; // The rate constant for fluorescence emission, see the note in FI Unit: s-1
        theVars->BF_RC.Kd = 2 * pow(10, 8) * theVars->BFRatio[18];   // The rate constant for heat dissipation; see the note for FI Unit: s-1
        theVars->BF_RC.KE8 = KE8; // ISPHr + cytc1 --> ISPHox + cytc1- Unit: s-1
        theVars->BF_RC.KE9 = KE9; // cytc1- + cytc2 --> cytc1 + cytc2- Unit: s-1
        theVars->BF_RC.K15 = pow(10, 10) * theVars->BFRatio[19];     // The rate constant for primary charge separation in PSI Unit: s-1
        theVars->BF_RC.K16 = pow(10, 5) * theVars->BFRatio[20];      // The rate constant for electron tranfer from electron acceptor of PSI to Fd Unit: s-1
        theVars->BF_RC.MemCap = 0.6 * pow(10, (-6)) * theVars->BFRatio[26]; // The membrane capacity
        theVars->BF_RC.RVA = 8 * pow(10, (-10)) * theVars->BFRatio[27];     // The ratio of lumen volume to thylakoid membrane area
        theVars->BF_RC.KBs = 1.1 * pow(10, (-8)) * theVars->BFRatio[28];    // The buffer equilibrium constant in stroma
        theVars->BF_RC.KBl = 5.1 * pow(10, (-6)) * theVars->BFRatio[29];    // The buffer equilibrium constant in lumen
        theVars->BF_RC.KM1ATP = 0.12 * theVars->BFRatio[30];  // The michaelis menton constant for ATP for ATP synthesis
        theVars->BF_RC.KM1ADP = 0.014 * theVars->BFRatio[31]; // The michaelis menton constant for ATP for ADP synthesis
        theVars->BF_RC.KM1PI = 0.3 * theVars->BFRatio[32];    // The michaelis menton constant for ATP for PI synthesis
        theVars->BF_RC.KM2NADP = 0.05 * theVars->BFRatio[33]; // The michaelis menten constant for NADP Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.KM2NADPH = 0.035 * theVars->BFRatio[34]; // The michaelis menten constant for NADPH Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.V2M = 27.8 * theVars->BFRatio[35];     // The maximum rate of NADPH formation Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
        theVars->BF_RC.KE2 = 495 * theVars->BFRatio[36];      // Equilibrium constatn
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves for a dark adapted leaves;
    // Unit  micro mol per m2 or mmol l-2 stroma volume

    // The initialization of the model with concentration of each substrate in dark-adapted leaves
    // The total concentration of PSII is assumed to be 1 micromole per meter square

    //  This is the initialization step for the module of the Q cycle, and ATP synthesis steps

    BFCondition* BF_con = new BFCondition();
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
    if (theVars->useC3) {
        theVars->BF_Pool.kA_d = 1;  // The total amount of cytbH or cytbL; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kA_f = 1;  // The total amount of cytc; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kA_U = 20; // The total concentration of K in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of K, and Mg and Cl as well, is constant.
        theVars->BF_Pool.kU_A = 10; // The total concentration of Mg in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Mg, and K and Cl as well, is constant.
        theVars->BF_Pool.kU_d = 2;  // The total concentration of Cl in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Cl in both stroma and lumen is constant.
        theVars->BF_Pool.kU_f = 1;  // The total concentration of Ferrodoxin
        theVars->BF_Pool.k1 = 1;    // The total concentration of the primary electron acceptor of PSI; Unit: micromole m-2 leaf area
        theVars->BF_Pool.k_r1 = 8;  // The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kz = 38;   // The total concentration of buffer in stroma; unit: mmol per liter
        theVars->BF_Pool.k12 = 38;  // The total concentration of buffer in lumen; unit: mmol per liter
        theVars->BF_Pool.k23 = 1;   // The total number of P700; unit: micromole m-2 leaf area
        theVars->BF_Pool.k30 = 1;   //   The total concentration of NADPH in stroma; 1 is an guessed value;

    } else {
        theVars->BF_Pool.kA_d = 1 * theVars->BFRatio[37];  // The total amount of cytbH or cytbL; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kA_f = 1 * theVars->BFRatio[38];  // The total amount of cytc; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kA_U = 20 * theVars->BFRatio[39]; // The total concentration of K in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of K, and Mg and Cl as well, is constant.
        theVars->BF_Pool.kU_A = 10 * theVars->BFRatio[40]; // The total concentration of Mg in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Mg, and K and Cl as well, is constant.
        theVars->BF_Pool.kU_d = 2 * theVars->BFRatio[41];  // The total concentration of Cl in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Cl in both stroma and lumen is constant.
        theVars->BF_Pool.kU_f = 1 * theVars->BFRatio[42];  // The total concentration of Ferrodoxin
        theVars->BF_Pool.k1 = 1 * theVars->BFRatio[43];    // The total concentration of the primary electron acceptor of PSI; Unit: micromole m-2 leaf area
        theVars->BF_Pool.k_r1 = 8 * theVars->BFRatio[44];  // The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
        theVars->BF_Pool.kz = 38 * theVars->BFRatio[45];   // The total concentration of buffer in stroma; unit: mmol per liter
        theVars->BF_Pool.k12 = 38 * theVars->BFRatio[46];  // The total concentration of buffer in lumen; unit: mmol per liter
        theVars->BF_Pool.k23 = 1 * theVars->BFRatio[47];   // The total number of P700; unit: micromole m-2 leaf area
        theVars->BF_Pool.k30 = 1 * theVars->BFRatio[48];   //   The total concentration of NADPH in stroma; 1 is an guessed value;
    }

    return BF_con;
}
