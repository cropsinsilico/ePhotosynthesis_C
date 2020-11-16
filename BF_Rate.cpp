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

void BF_Rate(double t, varptr *myVars) {
    
    //global BF_RC;
    
    const double K1 = myVars->BF_RC[0];	//	The rate constant for formation of ISP.QH2 complex; unit:  per second
    const double K2 = myVars->BF_RC[1];	//	The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
    const double K3 = myVars->BF_RC[2];	//	The rate constant for QH. + cytbL --> Q + cytbL- + H+	Unit: s-1
    const double K4 = myVars->BF_RC[3];	//	The rate constant for cytbL- + cytbH --> cytbL + cytbH-	Unit: s-1
    const double K5 = myVars->BF_RC[4];	//	The rate constant for CytbH- + Q --> cytbH + Q-	Unit: s-1
    
    const double K6 = myVars->BF_RC[5];	//	The rate constant  for CytbH- + Q- --> cytbH + Q2-	Unit: s-1
    const double K7 = myVars->BF_RC[6];	//	The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website	Unit: s-1
    const double K8 = myVars->BF_RC[7];	//	The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+	Unit: s-1
    const double K9 = myVars->BF_RC[8];	//	The rate constant for the electron transport from cytc1 to cytc2	Unit: s-1
    const double K10 = myVars->BF_RC[9];	//	The rate constant for the electron transport from cytc2 to P700	Unit: s-1
    
    const double Vmax11 = myVars->BF_RC[10];	//	The maximum rate of ATP synthesis	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    const double Kqi = myVars->BF_RC[11];	//	The rate constant for uptake of two protons from the stroma to Q2-	s-1
    const double PK = myVars->BF_RC[12];	//	The permeability constant for K	Unit: cm s-1
    const double PMg = myVars->BF_RC[13];	//	The permeability constant for Mg	Unit: cm s-1
    const double PCl = myVars->BF_RC[14];	//	The permeability constant for Cl	Unit: cm s-1
    
    const double Kau = myVars->BF_RC[15];	//	The rate constant for exciton transfer from perpheral antenna to core antenna, see FI	Unit: s-1
    const double Kua = myVars->BF_RC[16];	//	The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI	Unit: s-1
    const double Kf = myVars->BF_RC[17];	//	The rate constant for fluorescence emission, see the note in FI	Unit: s-1
    const double Kd = myVars->BF_RC[18];	//	The rate constant for heat dissipation; see the note for FI	Unit: s-1
    const double KE8 = myVars->BF_RC[19];	//	ISPHr + cytc1 --> ISPHox + cytc1-	Unit: s-1
    
    const double KE9 = myVars->BF_RC[20];	//	cytc1- + cytc2 --> cytc1 + cytc2-	Unit: s-1
    const double K15 = myVars->BF_RC[21];	//	The rate constant for primary charge separation in PSI	Unit: s-1
    const double K16 = myVars->BF_RC[22];	//	The rate constant for electron tranfer from electron acceptor of PSI to Fd	Unit: s-1
    // MemCap = myVars->BF_RC[23];	//	The membrane capacity// --unused
    const double RVA = myVars->BF_RC[24];	//	The ratio of lumen volume to thylakoid membrane area
    
    // KBs = myVars->BF_RC[25];	//	The buffer equilibrium constant in stroma// --unused
    // KBl = myVars->BF_RC[26];	//	The buffer equilibrium constant in lumen// --unused
    // KmATP = myVars->BF_RC[27];	//	The michaelis menton constant for ATP for ATP synthesis// --unused
    const double KmADP = myVars->BF_RC[28];	//	The michaelis menton constant for ATP for ADP synthesis
    const double KmPi = myVars->BF_RC[29];	//	The michaelis menton constant for ATP for PI synthesis
    
    const double KM2NADP = myVars->BF_RC[30];	//	The michaelis menten constant for NADP	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    const double KM2NADPH = myVars->BF_RC[31];	//	The michaelis menten constant for NADPH	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    const double V2M = myVars->BF_RC[32];	//	The maximum rate of NADPH formation	Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    const double KE2 = myVars->BF_RC[33];	//	Equilibrium constatn
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Step II get the concentration of differnet component //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // 	This is the initialization step for the module of the Q cycle, and ATP synthesis steps
    
    const double ISPHr = myVars->BF_con[0];	//	The reduced ion sulfer protein (ISPH)
    const double cytc1 = myVars->BF_con[1];	//	The oxidized state of cytc1
    const double ISPo = myVars->BF_con[2];	//	The oxidized ion sulfer protein (ISP)
    const double ISPoQH2 = myVars->BF_con[3];	//	The complex of oxidized ion sulfer protein and reduced quinone
    const double QHsemi = myVars->BF_con[4];	//	Semiquinone
    
    const double cytbL = myVars->BF_con[5];	//	The oxidized cytbL
    const double Qi = myVars->BF_con[6];	//	The binding Quinone
    const double Q = myVars->BF_con[7];	//	Quinone
    const double cytbH = myVars->BF_con[8];	//	The oxidized form of cytbH
    const double Qn = myVars->BF_con[9];	//	Q-
    
    const double Qr = myVars->BF_con[10];	//	Q2-
    const double QH2 = myVars->BF_con[11];	//	QH2
    const double cytc2 = myVars->BF_con[12];	//	oxidized cytc2
    const double P700 = myVars->BF_con[13];	//	The reduced state of P700, including both P700 and excited P700
    double ADP = myVars->BF_con[14];	//	ADP in stroma
    
    double Pi = myVars->BF_con[15];	//	Phosphate in stroma
    const double ATP = myVars->BF_con[16];	//	ATP in stroma
    const double Ks = myVars->BF_con[17];	//	K ions in stroma
    const double Mgs = myVars->BF_con[18];	//	Mg ions in stroma
    const double Cls = myVars->BF_con[19];	//	Cl ions in stroma
    
    const double Aip = myVars->BF_con[20];	//	The number of photons in peripheral antenna
    const double U = myVars->BF_con[21];	//	The number of photons in core antenna
    const double An = myVars->BF_con[22];	//	The reduced electron acceptor in PSI
    const double Fdn = myVars->BF_con[23];	//	The reduced ferrodoxin
    const double BFHs = myVars->BF_con[24];	//	The proton concentration in stroma, put in unit: mol l-1
    
    // BFHl = BF_con[25];	//	The proton concentration in lumen; put in unit: mol l-1// --unused
    const double PHs = myVars->BF_con[26];	//	The PH value of stroma
    const double PHl = myVars->BF_con[27];	//	The PH value of lumen
    const double NADPH = myVars->BF_con[28];	//	The NADPH concentratin in stroma, Unit: mmol l-1
    
    
    //global BF_Pool;
    
    const double Tcyt = myVars->BF_Pool[0];	//	The total amount of cytbH or cytbL; Unit: micromole m-2 leaf area
    const double Tcytc2 = myVars->BF_Pool[1];	//	The total amount of cytc; Unit: micromole m-2 leaf area
    const double TK = myVars->BF_Pool[2];	//	The total concentration of K in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of K, and Mg and Cl as well, is constant.
    const double TMg = myVars->BF_Pool[3];	//	The total concentration of Mg in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Mg, and K and Cl as well, is constant.
    const double TCl = myVars->BF_Pool[4];	//	The total concentration of Cl in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Cl in both stroma and lumen is constant.
    const double TFd = myVars->BF_Pool[5];	//	The total concentration of Ferrodoxin
    const double TA = myVars->BF_Pool[6];	//	The total concentration of the primary electron acceptor of PSI; Unit: micromole m-2 leaf area
    const double TQ = myVars->BF_Pool[7];	//	The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
    const double BFTs = myVars->BF_Pool[8];	//	The total concentration of buffer in stroma; unit: mmol per liter
    // BFTl = myVars->BF_Pool[9];	//	The total concentration of buffer in lumen; unit: mmol per liter// --unused
    const double P700T = myVars->BF_Pool[10];	//	The total concentration of P700; unit: micromole m-2 leaf area
    const double NADPHT = myVars->BF_Pool[11];   //   The total concentration of NADPH, Unit: mmol l-1.
    
    //////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    //////////////////////////////////////////////////////////////
    
    const double cytc1n = Tcyt - cytc1;//	The concentration of reduced cytc1
    const double cytbLn = Tcyt - cytbL;//	The concentration of reduced cytbL
    const double cytbHn = Tcyt - cytbH;//	The concentration of reduced cytbH
    const double cytc2n = Tcytc2 - cytc2;//	The concentration of reduced cytc2
    const double Kl = TK - Ks;//	The concentration of K in lumen
    const double Mgl = TMg - Mgs;//	The concentration of Mg in lumen
    const double Cll = TCl - Cls;//	The concentration of Cl in lumen
    const double Fd = TFd - Fdn;//	The conncentration of oxidized Fd in stroma
    const double A = TA - An;//	The concentration of oxidized electron acceptor in PSI
    const double QST = Tcyt;   // Assuming that the total number of cytochrome is equal to the total number of quinone binding site;
    // QSe     =   QST - Qi ;
    const double QSe = QST - Qi - Qn - Qr;
    const double P700p = P700T - P700;// The number of positive P700;
    const double NADP = NADPHT - NADPH;
    
    //global FIBF_PSPR_com;
    //global PS2BF_ADP;
    //global PS2BF_Pi;
    
    if (myVars->FIBF_PSPR_com) {
        ADP = myVars->PS2BF_ADP;
        Pi = myVars->PS2BF_Pi;
    }
    
    //global AVR;
    const double CoeffVol = myVars->AVR;
    
    const double Hfs = pow(10, (-PHs)) * 1000;
    // Hfl = pow(10, (-PHl)) * 1000;// --unused
    const double OHs = pow(10, (-14)) / (Hfs / 1000) * 1000;
    
    const double BFs = BFHs - Hfs;// The concentration of the protonated buffer speices. Notice here the variable BFHs represent the total concentration of both proton and the protonated buffer species in stroma
    const double BFns = BFTs - BFs;// The total concentration of deprotonated buffer species in stroma;
    
    // RegPHATP = 1;// --unused
    const double RegPHs = 1;
    
    const double Temp1 = pow(10, 5.5 - 8);
    const double a = Temp1 / (1 + Temp1);
    const double Temp2 = pow(10, 5.5 - PHl);
    const double b = Temp2 / (1 + Temp2);
    const double RegPHl = 1 - (b - a);
    const double Iin = myVars->BF_Param[0];
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the rate of different reactions //
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    const double Vmax = K1 * (ISPo + ISPHr);//   The maximum rate of formation of enzyme substrate complex
    const double Vbf1 = Vmax * ISPo / (ISPo + ISPHr) * QH2 / TQ;;//	Unit: micromole s-1 m-2 leaf area
    const double Vbf2 = K2 * ISPoQH2 * RegPHl;//	Unit: micromole s-1 m-2 leaf area
    const double Vbf3 = K3 * QHsemi * cytbL / Tcyt * RegPHl;//	Unit: micromole s-1 m-2 leaf area
    const double Vbf4 = K4 * cytbLn * cytbH / Tcyt;//	Unit: micromole s-1 m-2 leaf area
    // Vbf4	=	K4 * cytbLn * cytbH/TcytK3 * QHsemi * cytbL /Tc	;	//	Unit: micromole s-1 m-2 leaf area
    const double Vbf5 = K5 * cytbHn * Qi / Tcyt;//	;Unit: micromole s-1 m-2 leaf area
    const double Vbf6 = K6 * cytbHn * Qn / Tcyt;//	Unit: micromole s-1 m-2 leaf area
    const double Vbf7 = K7 * Tcyt * QSe / QST * Q / TQ;//	QSe ans QST represent the empty quinone binding site and the total number of quinone binding site respectively. Unit: micromole s-1 per meter squareleaf area;
    const double Vbf8 = K8 * Tcyt * (ISPHr / Tcyt * cytc1 / Tcyt - ISPo / Tcyt * cytc1n / Tcyt / KE8) * RegPHl;//	Unit: micromole s-1 m-2 leaf area
    const double Vbf9 = K9 * Tcyt * (cytc1n / Tcyt * cytc2 / Tcyt - cytc1 / Tcyt * cytc2n / Tcyt / KE9);//	Unit: micromole s-1 m-2 leaf area
    const double KE10 = 10;
    const double Vbf10 = K10 * P700p * cytc2n / P700T - K10 * cytc2 * P700 / P700T / KE10;//	Unit: micromole s-1 m-2 leaf area
    
    double Ytemp;
    if (Hfs < 0) {
        Ytemp = 0;
    } else {
        Ytemp = 25.359 - 0.0425 * pow(PHs, 3) + 1.0986 * pow(PHs, 2) - 9.1831 * PHs;
    }
    
    const double Vqi = Kqi * Qr * 2 / 0.72 * Ytemp;//	The rate of proton uptake at Qi site of bc1 complex; Unit: micromole s-1 m-2 leaf area; the coefficient 2 represents that two protons were taken up at the same time.0.72 represents the calculated proton uptake rate at Qi site when PH is 9.
    
    // 		Calculation of the different excition transfer reactions occurred in PSI
    
    const double Vipc = Aip * Kau;//	The rate of exciton transfer from peripheral antenna to core antenna; unit: micromole m-2 s-1	Unit: micromole m-2 leaf area per second
    const double Vicp = U * Kua;//	The rate of exciton transfer from core antenna to peripheral antenna	Unit: micromole m-2 leaf area per second
    
    //global ChlT2;   // total Chl in PSII
    // global ChlT;
    //global ChlPSI;  // total Chl in PSI
    // Iin is the total absorbed light
    
    const double Vinc = Iin * myVars->ChlPSI / (myVars->ChlT2 + myVars->ChlPSI) * 95 / 184;//   PPFD absorbed by core antenna of PSI    Unit: micromole m-2 leaf area per second
    const double Vinp = Iin * myVars->ChlPSI / (myVars->ChlT2 + myVars->ChlPSI) * 105 / 184;//   PPFD absorbed by peripheral antenna of PSI  Unit: micromole m-2 leaf area per second
    
    // Vinc	=	Iin * (200/(200+290))*80/200	;	//	PPFD absorbed by core antenna of PSI	Unit: micromole m-2 leaf area per second
    // Vinp	=	Iin * (200/(200+290))*120/200	;	//	PPFD absorbed by peripheral antenna of PSI	Unit: micromole m-2 leaf area per second
    
    const double Vdp = Aip * Kd;//	The rate of heat dissipation from peripheral antenna	Unit: micromole m-2 leaf area per second
    const double Vdc = U * Kd;//	The rate heat dissipation from core antenna	Unit: micromole m-2 leaf area per second
    const double Vfp = Aip * Kf;//	The fluorescence emission from peripheral antenna	Unit: micromole m-2 leaf area per second
    const double Vfc = U * Kf;//	The fluorescence emission from core antenna	Unit: micromole m-2 leaf area per second
    
    // 		The other empirical rates calculations
    const double Vsfd = 0;	//	The sink for Fd utilization; unit: mmol l-1 s-1;
    
    //global CO2_cond;
    const double CO2 = myVars->CO2_cond * 3 * pow(10, 4);
    
    const double MaxCO2Rate = 100 * CO2 / (CO2 + 460);
    
    const double VsATP = MaxCO2Rate * 1.5 / CoeffVol * ATP / 1.5;//(ADP + ATP);	//	The sink for ATP utilizaiton, 20 represent the of CO2 assimilation, since 1 meter square amount to 27 ml, therefore, the sink capacity should be 20 * 1.5 * 1.5 mmol / 27 l-1 s-1. The 1.5 represents the 1.5 ATP consumption per CO2 fixation. 	Unit: mmol l-2 s-1
    const double VsNADPH = MaxCO2Rate / CoeffVol * 1 * NADPH / NADPHT;// For 6 C6 = 5 C6 + 1C6 ;
    const double VgPQH2 = Q * 800 * RegPHs;// Assuming that the rate of generation of PQH2 through QB site only depend on the PQ and PQH2 exchange capacity.
    // FC = 9.6 * pow(10, 4);// --unused
    // R = 8.314;	    //	J K-1 mol-1// --unused
    // T = 298;	        //	K// --unused
    double NetCharge = Hfs + Ks + 2 * Mgs - OHs - Cls - BFns;//	The difference between the positive and negative charge in stroma. It was assumed that the charge is in equilibrium state in the beginning of the model, therefore, the difference in the positive and negative charges reflect the charges forming electrical potential cross the membrane. The unit is mmol l-1.
    NetCharge = NetCharge / 1000;//	The unit conversion. Convert from mmol l-1 to mol l-1.
    NetCharge = NetCharge * RVA;//	Convert from mol l-1 to mol cm-2.
    const double AfC = 6.022 * pow(10, 23);
    const double UnitCharge = 1.6 * pow(10, -19);
    NetCharge = NetCharge * AfC * UnitCharge;//	Unit: Coulomb cm-2
    const double MPotential = 2 * NetCharge / 6 * pow(10, 6);
    const double temp = - MPotential / 0.026;//	0.026 pools together the RT/F; With R as 8.314 J K-1 mol-1; T: 298; F: 9.648*10^4 C mol-1.
    
    double JMg;
    double JCl;
    double JK;
    if (temp == 0) {
        JK = PK * (Kl - Ks) / 10;//	The flux of K from lumen to stroma; unit: mol/dm2/s; the unit of permeability is: cm s-1; 10 represent the conversion from cm s-1 to dm s-1; Remember that 1 liter is 1 dm3.
        JCl = PCl * (Cll - Cls) / 10;//	The flux of Cl from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion from cm s-1 to dm s-1; remember that 1 liter amount to 1 decimeter cube
        JMg = PMg * (Mgl - Mgs) / 10;//	The flux of Mg from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion of cm s-1 to dm s-1. Remember that 1 liter amount to 1 decimeter cube.
    } else {
        JK = PK * temp * (Kl - Ks * exp(-temp)) / (1 - exp(-temp)) / 10;//	The flux of K from lumen to stroma; unit: mol/dm2/s; the unit of permeability is: cm s-1; 10 represent the conversion from cm s-1 to dm s-1; Remember that 1 liter is 1 dm3.
        JCl = - PCl * temp * (Cll - Cls * exp(temp)) / (1 - exp(temp)) / 10;//	The flux of Cl from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion from cm s-1 to dm s-1; remember that 1 liter amount to 1 decimeter cube
        JMg = PMg * temp / 2 * (Mgl - Mgs * exp(-temp / 2)) / (1 - exp(-temp / 2)) / 10;//	The flux of Mg from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion of cm s-1 to dm s-1. Remember that 1 liter amount to 1 decimeter cube.
    }
    
    //	The ion flux needed to converted to convert to the changes in the substrate concentration changes in the stroma.
    const double JKc = JK / RVA / 100;//	100 represent the conversion to cm-2;  RVA is the ratio between the lumen volume and thylakoid membrane area.
    const double JClc = JCl / RVA / 100;//	100 represent the conversion to cm-2;  RVA is the ratio between the lumen volume and thylakoid membrane area.
    const double JMgc = JMg / RVA / 100;//	100 represent the conversion to cm-2;  RVA is the ratio between the lumen volume and thylakoid membrane area.
    
    const double Vbf12 = JKc;	//	Assign the variable
    const double Vbf13 = JMgc;	//	Assign the variable
    const double Vbf14 = JClc;	//	Assign the variable
    
    const double P700e = U * (P700T - P700p) / 120;//	The amount of excited P700; micromole m-2 leaf area
    const double Vbf15 = P700e * K15 * A / TA;//	The rate of PSI primary charge separation; unit: micromole m-2 leaf area per second
    const double Vbf16 = An * K16 * Fd / TFd;//	The rate of electron transport from the electron acceptor of PSI to Fd; Unit: micromole m-2 leaf area s-1;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //global HPR;
    const double RT = 298 * 8.314;//	Gas constnat and the temperature
    const double DeltaGo = 7.3 * 4184;//   The free energy change for ATP synthesis from ADP and Pi
    const double DiffPH = PHs - PHl;
    const double DeltaG11 = DeltaGo - 2.3 * RT * myVars->HPR * DiffPH + myVars->HPR * MPotential * 9.6 * pow(10, 4);
    const double KE11 = exp(-DeltaG11 / (RT));//	The equilibrium constant of ATP synthesis
    
    //global DPH;
    
    //myVars->DPH = DiffPH - MPotential / 0.059;  // --unused
    const double Temp = Vmax11 * (ADP * Pi - ATP / KE11) / ((KmADP * KmPi) * (1 + ADP / KmADP + Pi / KmPi + ADP * Pi / (KmADP * KmPi)));//	Unit: mmol l- s-1; The stroma volume is used as a basis for the volume
    double Vbf11 = Temp;
    
    if (Vbf11 < 0)
        Vbf11 = 0;
    
    
    //global EPS_ATP_Rate;   // The EPS_ATP_Rate is used in the overall model for the calculation of the mass balance equation of ATP.
    myVars->EPS_ATP_Rate = Vbf11;
    
    const double FD = Fd;
    const double FD_N = Fdn;
    const double T_FD = TFd;
    
    const double vbfn2 = 2 * V2M * (FD_N * NADP / T_FD - FD * NADPH / (T_FD * KE2)) / (KM2NADP * (1 + NADP / KM2NADP + NADPH / KM2NADPH));// mmol/l/s  //QF add 2*
    
    const double vcet = V2M * Qi * FD_N / T_FD * CoeffVol;
    
    //global BF2EPS_vbfn2;
    myVars->BF2EPS_vbfn2 = vbfn2;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Transfer infroamtion between models                     //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //global FIBF_RA_Mg;
    //global FIBF_RA_ATP;
    //global BF2XanCycle_pHl;
    
    myVars->FIBF_RA_Mg = Mgs;
    //myVars->FIBF_RA_ATP = ATP;  // --unused
    myVars->BF2XanCycle_pHl = PHl;
    
    
    //global BF2RROEA_Vbf16;
    myVars->BF2RROEA_Vbf16 = Vbf16;
    
    //global BF2trDynaPS_vbfn2;
    myVars->BF2trDynaPS_vbfn2 = vbfn2;
    
    //global BF2Stom_ATP;
    //myVars->BF2Stom_ATP = ATP;  // --unused
    
    ////////////////////////////////////////////////////////////////////////////////
    //   Part V Output of Velocity for plot //
    ////////////////////////////////////////////////////////////////////////////////
    
    //global BF_OLD_TIME;
    //global BF_TIME_N;
    //global BF_VEL;
    //global BF_CON;
    
    if (myVars->BF_TIME_N == 0)
        myVars->BF_TIME_N = 1;
    
    
    if (t > myVars->BF_OLD_TIME) {
            myVars->BF_TIME_N = myVars->BF_TIME_N + 1;
            myVars->BF_OLD_TIME = t;
        }
        myVars->BF_Vel[0] = Vbf1;
    myVars->BF_Vel[1] = Vbf2;
    myVars->BF_Vel[2] = Vbf3;
    myVars->BF_Vel[3] = Vbf4;
    myVars->BF_Vel[4] = Vbf5;
    
    myVars->BF_Vel[5] = Vbf6;
    myVars->BF_Vel[6] = Vbf7;
    myVars->BF_Vel[7] = Vbf8;
    myVars->BF_Vel[8] = Vbf9;
    myVars->BF_Vel[9] = Vbf10;
    
    myVars->BF_Vel[10] = Vbf11;
    myVars->BF_Vel[11] = Vqi;
    myVars->BF_Vel[12] = Vipc;
    myVars->BF_Vel[13] = Vicp;
    myVars->BF_Vel[14] = Vinc;
    
    myVars->BF_Vel[15] = Vinp;
    myVars->BF_Vel[16] = Vdp;
    myVars->BF_Vel[17] = Vdc;
    myVars->BF_Vel[18] = Vfp;
    myVars->BF_Vel[19] = Vfc;
    
    myVars->BF_Vel[20] = Vsfd;
    myVars->BF_Vel[21] = VsATP;
    myVars->BF_Vel[22] = VgPQH2;
    myVars->BF_Vel[23] = Vbf12;
    myVars->BF_Vel[24] = Vbf13;
    
    myVars->BF_Vel[25] = Vbf14;
    myVars->BF_Vel[26] = Vbf15;
    myVars->BF_Vel[27] = Vbf16;
    myVars->BF_Vel[28] = vbfn2;
    myVars->BF_Vel[29] = VsNADPH;
    myVars->BF_Vel[30] = vcet;

    if (myVars->record) {
        myVars->BF_VEL.insert(myVars->BF_TIME_N - 1, t, myVars->BF_Vel);
        //if (myVars->BF_TIME_N == 0)
        //    myVars->BF_TIME_N = 1;
        /*
    
        
    
        if (myVars->BF_VEL.shape()[0] < myVars->BF_TIME_N) {
            myVars->BF_VEL.resize(boost::extents[myVars->BF_TIME_N][BF_VEL_SIZE]);
            myVars->BF_CON.resize(boost::extents[myVars->BF_TIME_N][BF_CON_SIZE]);
        }

        myVars->BF_VEL[myVars->BF_TIME_N - 1][0] = t;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][1] = Vbf1;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][2] = Vbf2;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][3] = Vbf3;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][4] = Vbf4;
    
        myVars->BF_VEL[myVars->BF_TIME_N - 1][5] = Vbf5;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][6] = Vbf6;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][7] = Vbf7;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][8] = Vbf8;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][9] = Vbf9;
    
        myVars->BF_VEL[myVars->BF_TIME_N - 1][10] = Vbf10;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][11] = Vbf11;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][12] = Vqi;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][13] = Vipc;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][14] = Vicp;
    
        myVars->BF_VEL[myVars->BF_TIME_N - 1][15] = Vinc;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][16] = Vinp;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][17] = Vdp;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][18] = Vdc;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][19] = Vfp;
    
        myVars->BF_VEL[myVars->BF_TIME_N - 1][20] = Vfc;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][21] = Vsfd;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][22] = VsATP;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][23] = VgPQH2;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][24] = Vbf12;
    
        myVars->BF_VEL[myVars->BF_TIME_N - 1][25] = Vbf13;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][26] = Vbf14;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][27] = Vbf15;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][28] = Vbf16;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][29] = vbfn2;
        myVars->BF_VEL[myVars->BF_TIME_N - 1][30] = VsNADPH;
        */
        //myVars->BF_CON[myVars->BF_TIME_N - 1][0] = t;  // --unused
        //myVars->BF_CON[myVars->BF_TIME_N - 1][1] = MPotential;  // --unused
    }
    //global BF_Vel;
    
    //global BF2OUT;
    // BF2OUT = zeros(5, 1);
    myVars->BF2OUT[0] = Fdn;
    myVars->BF2OUT[1] = PHs;
    myVars->BF2OUT[2] = PHl;
    myVars->BF2OUT[3] = NADPH;
    myVars->BF2OUT[4] = ATP;
    
    //global BF2FIBFMB_PHl;
    
    myVars->BF2FIBFMB_PHl = PHl;
    
    //global BF2TrDynaPSMB_vcet;
    myVars->BF2TrDynaPSMB_vcet = vcet;
}
