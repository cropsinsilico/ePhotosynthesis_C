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
#include "BF.hpp"

BFCon BF_Ini(Variables *myVars) {
    const double K1 = pow(10, 6) * myVars->BFRatio[0];
    const double K2 = 500 * myVars->BFRatio[1];//	The rate constant for ISP.QH2-->QH(semi) + ISPH(red)
    const double K3 = 5 * pow(10, 7) * myVars->BFRatio[2];
    const double K4 = 5 * pow(10, 7) * myVars->BFRatio[3];
    const double K5 = 5 * pow(10, 7) * myVars->BFRatio[4];

    const double K6 = 5 * pow(10, 7) * myVars->BFRatio[5];
    const double K7 = pow(10, 4) * myVars->BFRatio[6];
    const double K8 = 1000 * myVars->BFRatio[7];//	The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+
    const double K9 = 8.3 * pow(10, 6) * myVars->BFRatio[8];
    const double K10 = 8 * pow(10, 8) * myVars->BFRatio[9];

    const double Vmax11 = 6 * myVars->BFRatio[10];//	The maximum rate of ATP synthesis; unit: mmol l-1 s-1; default 1.47;
    const double Kqi = pow(10, 3) * myVars->BFRatio[11];

    const double PMODTEM = 1;
    const double PK = 3.6 * pow(10, (-8)) * PMODTEM * myVars->BFRatio[12];
    const double PMg = 3.6 * pow(10, (-8)) * PMODTEM * myVars->BFRatio[13];
    const double PCl = 1.8 * pow(10, (-8)) * PMODTEM * myVars->BFRatio[14];

    const double Kau = pow(10, 10) * myVars->BFRatio[15];
    const double Kua = pow(10, 10) * myVars->BFRatio[16];
    const double Kf = 6.3 * pow(10, 6) * myVars->BFRatio[17];
    const double Kd = 2 * pow(10, 8) * myVars->BFRatio[18];

    const double K15 = pow(10, 10) * myVars->BFRatio[19];
    const double K16 = pow(10, 5) * myVars->BFRatio[20];

    const double Em_ISP = 0.31 * myVars->BFRatio[21];//	The midpoint potential fo ISP; unit: V
    const double Em_CytC1 = 0.27 * myVars->BFRatio[22];//	The midpoint potential for cytc1; unit: V
    const double Em_CytC2 = 0.35 * myVars->BFRatio[25];//	The midpoint potential for cytc2, unit: V

    // ISPHr + cytc1 --> ISPHox + cytc1-
    double DeltaEm = Em_CytC1 - Em_ISP;
    double DeltaG = DeltaEm * (-9.649) * pow(10, 4);
    double RT = 8.314 * 298;
    const double KE8 = exp(-DeltaG / RT);

    // cytc1- + cytc2 --> cytc1 + cytc2-
    DeltaEm = Em_CytC2 - Em_CytC1;
    DeltaG = DeltaEm * (-9.649) * pow(10, 4);
    RT = 8.314 * 298;
    const double KE9 = exp(-DeltaG / RT);

    const double MemCap = 0.6 * pow(10, (-6)) * myVars->BFRatio[26];
    const double RVA = 8 * pow(10, (-10)) * myVars->BFRatio[27];

    const double KBs = 1.1 * pow(10, (-8)) * myVars->BFRatio[28];
    const double KBl = 5.1 * pow(10, (-6)) * myVars->BFRatio[29];

    const double KM1ATP = 0.12 * myVars->BFRatio[30];
    const double KM1ADP = 0.014 * myVars->BFRatio[31];// Originally
    const double KM1PI = 0.3 * myVars->BFRatio[32];// Originally


    const double KM2NADP = 0.05 * myVars->BFRatio[33];// From Fridlyand and Scheibe 1999
    const double KM2NADPH = 0.035 * myVars->BFRatio[34];// From Fridlyand and Scheibe 1999
    const double V2M = 27.8 * myVars->BFRatio[35];// Calcualted based on 6.4 mmol (mg chl)-1h-1; Unit: mmol/l/s;
    const double KE2 = 495 * myVars->BFRatio[36];// From Fridlyand paper , 1999, BBA, 1413, 1, 31-42

    //	Assign values to the array for rate constant

    myVars->BF_RC.K1 = K1;	//	The rate constant for formation of ISP.QH2 complex; unit:  per second
    myVars->BF_RC.K2 = K2;	//	The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
    myVars->BF_RC.K3 = K3;	//	The rate constant for QH. + cytbL --> Q + cytbL- + H+	Unit: s-1
    myVars->BF_RC.K4 = K4;	//	The rate constant for cytbL- + cytbH --> cytbL + cytbH-	Unit: s-1
    myVars->BF_RC.K5 = K5;	//	The rate constant for CytbH- + Q --> cytbH + Q-	Unit: s-1

    myVars->BF_RC.K6 = K6;	//	The rate constant  for CytbH- + Q- --> cytbH + Q2-	Unit: s-1
    myVars->BF_RC.K7 = K7;	//	The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website	Unit: s-1
    myVars->BF_RC.K8 = K8;	//	The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+	Unit: s-1
    myVars->BF_RC.K9 = K9;	//	The rate constant for the electron transport from cytc1 to cytc2	Unit: s-1
    myVars->BF_RC.K10 = K10;	//	The rate constant for the electron transport from cytc2 to P700	Unit: s-1

    myVars->BF_RC.Vmax11 = Vmax11;	//	The maximum rate of ATP synthesis	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    myVars->BF_RC.Kqi = Kqi;	//	The rate constant for uptake of two protons from the stroma to Q2-	s-1
    myVars->BF_RC.PK = PK;	//	The permeability constant for K	Unit: cm s-1
    myVars->BF_RC.PMg = PMg;	//	The permeability constant for Mg	Unit: cm s-1
    myVars->BF_RC.PCl = PCl;	//	The permeability constant for Cl	Unit: cm s-1

    myVars->BF_RC.Kau = Kau;	//	The rate constant for exciton transfer from perpheral antenna to core antenna, see FI	Unit: s-1
    myVars->BF_RC.Kua = Kua;	//	The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI	Unit: s-1
    myVars->BF_RC.Kf = Kf;	//	The rate constant for fluorescence emission, see the note in FI	Unit: s-1
    myVars->BF_RC.Kd = Kd;	//	The rate constant for heat dissipation; see the note for FI	Unit: s-1
    myVars->BF_RC.KE8 = KE8;	//	ISPHr + cytc1 --> ISPHox + cytc1-	Unit: s-1

    myVars->BF_RC.KE9 = KE9;	//	cytc1- + cytc2 --> cytc1 + cytc2-	Unit: s-1
    myVars->BF_RC.K15 = K15;	//	The rate constant for primary charge separation in PSI	Unit: s-1
    myVars->BF_RC.K16 = K16;	//	The rate constant for electron tranfer from electron acceptor of PSI to Fd	Unit: s-1
    myVars->BF_RC.MemCap = MemCap;	//	The membrane capacity
    myVars->BF_RC.RVA = RVA;	//	The ratio of lumen volume to thylakoid membrane area

    myVars->BF_RC.KBs = KBs;	//	The buffer equilibrium constant in stroma
    myVars->BF_RC.KBl = KBl;	//	The buffer equilibrium constant in lumen
    myVars->BF_RC.KM1ATP = KM1ATP;	//	The michaelis menton constant for ATP for ATP synthesis
    myVars->BF_RC.KM1ADP = KM1ADP;	//	The michaelis menton constant for ATP for ADP synthesis
    myVars->BF_RC.KM1PI = KM1PI;	//	The michaelis menton constant for ATP for PI synthesis

    myVars->BF_RC.KM2NADP = KM2NADP;	//	The michaelis menten constant for NADP	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    myVars->BF_RC.KM2NADPH = KM2NADPH;	//	The michaelis menten constant for NADPH	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    myVars->BF_RC.V2M = V2M;	//	The maximum rate of NADPH formation	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    myVars->BF_RC.KE2 = KE2;	    //	Equilibrium constatn

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization of the initial concentration of the different component  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize the leaves for a dark adapted leaves;
    // Unit		micro mol per m2	or mmol l-2 stroma volume

    //	The initialization of the model with concentration of each substrate in dark-adapted leaves
    //	The total concentration of PSII is assumed to be 1 micromole per meter square

    const double ISPHr = 0;	//	The reduced ion sulfer protein (ISPH); unit: micromole per m2
    const double cytc1 = 1;	//	The oxidized state of cytc1; unit: micromole per meter square
    const double ISPo = 1;	//	The oxidized ion sulfer protein (ISP); unit: micromole per meter square
    const double ISPoQH2 = 0;	//	The complex of oxidized ion sulfer protein and reduced quinone; unit: micromole per meter square
    const double QHsemi = 0;	//	Semiquinone; micromole per meter square
    const double cytbL = 1;	//	The oxidized cytbL; micromole per meter square
    const double Qi = 0;	    //	The binding quinone on the quinone site; micromole per meter square
    const double Q = 1;	    //	Quinone; micromole per meter square
    const double cytbH = 1;	    //	The oxidized form of cytbH; micromole per meter square
    const double Qn = 0;	    //	Q- ; unit: micromole per meter square
    const double Qr = 0;	    //	The reduced quinone Q2- ; micromole per meter square
    const double QH2 = 5;	    //	The reduced quinone PQH2; micromole per meter square
    const double cytc2 = 1;	//	oxidized cytc2; micromole per meter square
    const double P700 = 0.5;	//	The reduced state of P700, including both P700 and excited P700; micromole per meter square
    const double ADP = 0.82;	//	ADP in stroma, from the earlier photorespiration model; mmol l-1
    const double Pi = 0.9;	    //	Phosphate in stroma, from the photorespiration model; mmol l-1
    const double ATP = 0.68;	//	ATP in stroma, from the photorespiration model; mmol l-1

    const double Ks = 10;	    //	K ions in stroma, mM, from the literature; mmol l-1; 90 might be an default;
    const double Mgs = 5;//2.5	;	    //	Mg ions in stroma, mM, from the literature of the ion estimate
    const double Cls = 1;	    //	Cl ions in stroma, mM, from the literature of the ion estimate


    const double Aip = 0;	    //	The number of photons in peripheral antenna; micromole per meter square
    const double U = 0;	    //	The number of photons in core antenna; micromole per meter square
    const double An = 0;	    //	The reduced electron acceptor in PSI; micromole per meter square
    const double Fdn = 0.3;	//	The reduced ferrodoxin; micromole per meter square leaf area

    const double BFHs = 19.0001; //50.0001	;	//	The protonated buffer species  and free proton together in stroma; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number.; default 25
    const double BFHl = 19.0001;	//	The protonated buffer species and free proton together in lumen; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number. ; default 5

    const double PHs = 7;   //   PH of stroma
    const double PHl = 7;   //   PH of lumen
    const double NADPH = 0.21;   //   The NADPH concentration in stroma at dark


    // 	This is the initialization step for the module of the Q cycle, and ATP synthesis steps

    BFCon BF_con;
    BF_con.ISPHr = ISPHr;	//	The reduced ion sulfer protein (ISPH)
    BF_con.cytc1 = cytc1;	//	The oxidized state of cytc1
    BF_con.ISPo = ISPo;	//	The oxidized ion sulfer protein (ISP)
    BF_con.ISPoQH2 = ISPoQH2;	//	The complex of oxidized ion sulfer protein and reduced quinone
    BF_con.QHsemi = QHsemi;	//	Semiquinone

    BF_con.cytbL = cytbL;	//	The oxidized cytbL
    BF_con.Qi = Qi;	//	The binding Quinone
    BF_con.Q = Q;	//	Quinone
    BF_con.cytbH = cytbH;	//	The oxidized form of cytbH
    BF_con.Qn = Qn;	//	Q-

    BF_con.Qr = Qr;	//	Q2-
    BF_con.QH2 = QH2;	//	QH2
    BF_con.cytc2 = cytc2;	//	oxidized cytc2
    BF_con.P700 = P700;	//	The reduced state of P700, including both P700 and excited P700
    BF_con.ADP = ADP;	//	ADP in stroma

    BF_con.Pi = Pi;	//	Phosphate in stroma
    BF_con.ATP = ATP;	//	ATP in stroma
    BF_con.Ks = Ks;	//	K ions in stroma
    BF_con.Mgs = Mgs;	//	Mg ions in stroma
    BF_con.Cls = Cls;	//	Cl ions in stroma

    BF_con.Aip = Aip;	//	The number of photons in peripheral antenna
    BF_con.U = U;	//	The number of photons in core antenna
    BF_con.An = An;	//	The reduced electron acceptor in PSI
    BF_con.Fdn = Fdn;	//	The reduced ferrodoxin
    BF_con.BFHs = BFHs;	//	The total concentration of proton and protonated buffer species in stroma, put in unit: mmol l-1
    BF_con.BFHl = BFHl;	//	The total concentration of proton and protonated buffer species in lumen, unit: mmol l-1

    BF_con.PHs = PHs;	//	The PH value of the stroma
    BF_con.PHl = PHl;	//	The PH value of the lumen
    BF_con.NADPH = NADPH;	//	The NADPH concentration in stroma, Unit: mmol l-1;

    // Assigning the pool variables
    //	The sizes of different pools in the model
    const double Tcyt = 1 * myVars->BFRatio[37];//	Unit: micromole m-2	The total concentration of cytochrome. It is assumed that the concentration of cytbL, cytbH, and cytc1 is equal as Tcyt.
    const double Tcytc2 = 1 * myVars->BFRatio[38];//	Unit: micromole m-2	The total concentration of cytc2, as Tcyt , the unit is micromole per meter square leaf area
    const double TK = 20 * myVars->BFRatio[39];//	Unit: mmol l-1	The total concentration of potassium, 180 mM, in the bigining assuming the concentration of ions in stroma and lumen are same
    const double TMg = 10 * myVars->BFRatio[40];//	Unit: mmol l-1	The total concentraiton of Mg2+, 18 mM, in the beginning assuming the concentration of the ions in the stroma and lumen are same
    const double TCl = 2 * myVars->BFRatio[41];//1		;	//	Unit: mmol l-1	The total concentraiton of Cl-, 1 mM, assuming equal concentrations of ion concentrations in stroma and lumen in the beginning.
    const double TFd = 1 * myVars->BFRatio[42];//	Unit: micromole m-2	The total concentration of Ferrodoxin, this assumed that only 1 Fd is associated with one PSI unit.
    const double TA = 1 * myVars->BFRatio[43];//	Unit: micromole m-2	The total concentration of electron acceptor of PSI
    const double TQ = 8 * myVars->BFRatio[44];//	Unit: micromole m-2	The total concentration of quinone at all different states
    const double BFTs = 38 * myVars->BFRatio[45];//	Unit: mmol l-1	The total concentration of buffer in stroma;
    const double BFTl = 38 * myVars->BFRatio[46];//	Unit: mmol l-1	The total concentration of buffer in lumen


    const double P700T = 1 * myVars->BFRatio[47];//   //   The total number of P700 unit: micromole m-2 leaf area
    const double NADPHT = 1 * myVars->BFRatio[48];//   The total concentration of NADPH in stroma; 1 is an guessed value;

    //	Assign the pools to the global pool variables

    myVars->BF_Pool.kA_d = Tcyt;	//	The total amount of cytbH or cytbL; Unit: micromole m-2 leaf area
    myVars->BF_Pool.kA_f = Tcytc2;	//	The total amount of cytc; Unit: micromole m-2 leaf area
    myVars->BF_Pool.kA_U = TK;	//	The total concentration of K in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of K, and Mg and Cl as well, is constant.
    myVars->BF_Pool.kU_A = TMg;	//	The total concentration of Mg in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Mg, and K and Cl as well, is constant.
    myVars->BF_Pool.kU_d = TCl;	//	The total concentration of Cl in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Cl in both stroma and lumen is constant.
    myVars->BF_Pool.kU_f = TFd;	//	The total concentration of Ferrodoxin
    myVars->BF_Pool.k1 = TA;	//	The total concentration of the primary electron acceptor of PSI; Unit: micromole m-2 leaf area
    myVars->BF_Pool.k_r1 = TQ;	//	The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
    myVars->BF_Pool.kz = BFTs;	//	The total concentration of buffer in stroma; unit: mmol per liter
    myVars->BF_Pool.k12 = BFTl;	//	The total concentration of buffer in lumen; unit: mmol per liter
    myVars->BF_Pool.k23 = P700T;	//	The total number of P700; unit: micromole m-2 leaf area
    myVars->BF_Pool.k30 = NADPHT;	//   The total concentration of NADPH in stroma; 1 is an guessed value;

    myVars->BF2RROEA_Fdn = Fdn;
    myVars->BF2RROEA_FdT = TFd;
    return BF_con;
}
