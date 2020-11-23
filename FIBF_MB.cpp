#include "globals.hpp"
#include "FIBF.hpp"
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



// FIBF_MB.m
// This function calculate the mass balance equation for the complete model of the light reactions.

arr FIBF_MB(double t, FIBFCon &FIBF_Con, varptr *myVars) {
    
    // First Get the variables needed for the calcualtion step
    
    //arr BF_con = zeros(29);
    //arr BF_Con = zeros(29);
    //for (int m = 0; m < 29; m++)
    //    BF_Con[m] = FIBF_Con[m];
    BFCon BF_con(FIBF_Con.BF_con);
    
    
    //arr FI_con = zeros(22);
    //for (int m = 0; m < 22; m++)
    //    FI_con[m] = FIBF_Con[m + 29];
    FICon FI_Con(FIBF_Con.FI_con);
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //       Calculate auxilary variable, PQ             //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //global BF_Pool;
    //global FI_Pool;
    
    const double PQn = FI_Con.PQn;	//	The concentration of reduced PQ, i.e. PQH2;
    const double Qi = BF_con.Qi;	//	The binding Quinone
    const double Qn = BF_con.Qn;	//	Q-
    const double Qr = BF_con.Qr;	//	Q2-
    const double ISPoQH2 = BF_con.ISPoQH2;	//	The complex of oxidized ion sulfer protein and reduced quinone
    const double QHsemi = BF_con.QHsemi;	//	Semiquinone
    
    double TQ = myVars->BF_Pool.k_r1;	//	The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
    
    //global FIBF_Pool;       // Since in this file, the FIBF is combined, therefore, we can use the common variable directly.
    TQ = myVars->FIBF_Pool.PQT;
    
    const double QBt = myVars->FI_Pool.QBt;      //   The total concentration of QB site.
    
    //global FIBF_AUX;
    const double PQ = TQ - QBt - PQn - Qi - Qn - Qr - ISPoQH2 - QHsemi;
    const double PQa = QBt + Qi + Qn + Qr + ISPoQH2 + QHsemi;
    
    myVars->FIBF_AUX[0] = PQ;
    myVars->FIBF_AUX[1] = PQa;
    BF_con.Q = PQ;
    
    //global FIBF2FI_PQ;
    myVars->FIBF2FI_PQ = PQ;
    
    //global FI_RC;
    myVars->FI_RC.kA_d = FIBF_Con.kd;
    myVars->FI_RC.kU_d = FIBF_Con.kd;
    
    //global BF_RC;
    myVars->BF_RC.Kd = FIBF_Con.kd;
    
    //arr BF_mb = zeros(28);
    arr BF_mb = BF_Mb(t, BF_con, myVars);
    arr FI_mb = FI_Mb(t, FI_Con, myVars);
    
    // Assign the value of the calcualted BF_mb and FI_mb to FIBF_MB variable
    arr FIBF_mb = zeros(52);
    
    for (int m = 0; m < 29; m++)
        FIBF_mb[m] = BF_mb[m];
    
    
    for (int m = 0; m < 22; m++)
        FIBF_mb[m + 29] = FI_mb[m];
    
    
    // Now specially calcualte the mass balance equation for the rate constant of the heat dissipation
    
    const double kd = FIBF_Con.kd;          // The initialization of the initial rate constant for heat dissipation
    double PHl = BF_con.PHl;           // Get the PH value of the lumen
    const double Hl = pow(10, PHl);
    const double QH = pow(10, (5.5)) / (Hl + pow(10, (5.5)));
    
    const double RC = 0.1;                   // RC is the relaxation constant, which is one term borrowed from Laisk et al., 1997;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the section implementing the nonphotochemical quenching.
    // ////////////////////////////////////////////////////////////////////////////////////////////////
    //global XanCycle_BF_com;
    //global BF2FIBFMB_PHl;
    PHl = myVars->BF2FIBFMB_PHl;
    
    double dmax = 5 * pow(10, 8) * QH;
    
    if (myVars->XanCycle_BF_com) {
        
        //global myVars->XanCycle2FIBF_Xstate;
        if (myVars->XanCycle2FIBF_Xstate <= 0.3) {
            dmax = dmax;
    } else {
            dmax = dmax * myVars->XanCycle2FIBF_Xstate / 0.3;
        }
    }
    FIBF_mb[51] = RC * (dmax - kd);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the proton generation rate from the model of FI and use that to calculate the lumen PH //            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //global AVR;
    const double CoeffVol = myVars->AVR;      // The conversion factor between micromole per meter square per second and milimole per liter per second
    
    
    //global FI_Vel;
    
    const double Vs3s0 = myVars->FI_Vel.vS3_S0 ;     // This is the rate of state transition from S3 state to So state. This conversion is accompanied by splitting
    // one molecular water molecules to release four protons.
    const double Hroe = 4 * Vs3s0 / CoeffVol;// 27 is the conversion of unit from micromole per meter squre leaf area to mmol per liter.
    
    //global BF_Vel;
    const double Vbf3 = myVars->BF_Vel.Vbf3 ;
    const double Vbf8 = myVars->BF_Vel.Vbf8 ;
    const double Vbf11 = myVars->BF_Vel.Vbf11 ;
    
    const double Hvqo1 = Vbf8 / CoeffVol;//	The rate of release of protons into lumen through Qo site
    const double Hvqo2 = Vbf3 / CoeffVol;//	The rate of proton release into lumen through Qo site
    
    //Hroe;
    //global HPR;
    
    BF_mb[25] = (Hvqo1 + Hvqo2 + Hroe - myVars->HPR * Vbf11);//	BFHl	The proton and protonated buffer species in lumen, similarly, we can only use the buff concentration, but, the proton concentration can not be used here.
    BF_mb[27] = - (Hvqo1 + Hvqo2 + Hroe - myVars->HPR * Vbf11) / 1000 / 0.015;//   PHl  The changes in PH of lumen, 0.03 is from Curz et al., 2001, Biochemistry.
    
    FIBF_mb[25] = BF_mb[25];
    FIBF_mb[27] = BF_mb[27];
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    //          Calculate the PH of stroma        //
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    const double v3 = myVars->FI_Vel.v3 ;	//	v3	The rate of exchange of QAQBH2 with PQ; There is two proton uptake from stroma
    const double v_r3 = myVars->FI_Vel.v_r3 ;	//	v_r3	The rate of exchange of QAQB with PQH2; there is two proton release into stroma
    const double v3_n = myVars->FI_Vel.v3_n ;	//	v3_n	The rate of exchange of QAnQBH2 with PQ; there is two protons uptake from stroma
    const double v_r3_n = myVars->FI_Vel.v_r3_n ;	//	v_r3_n	The rate of exchange of QAnQB with PQH2; there is two protons release into stroma
    
    const double GPQH2_qb = v3 - v_r3 + v3_n - v_r3_n;
    const double vqb = 2 * GPQH2_qb;//   The total rate of proton uptake at the QB site of PSII.
    
    const double Vqi = myVars->BF_Vel.Vqi ;   //   The rate of proton uptake from the stroma side
    const double Hvqi = Vqi / CoeffVol;//	The rate of proton uptake from stroma at Qi site of cytbc1 complex
    const double vbfn2 = myVars->BF_Vel.vbfn2 ;   //   The rate of proton consumption by formation of NADPH
    const double Hrqb = vqb / CoeffVol;//	Convert the unit of vqb from micormole per meter square per second to mM s-1; vqb is the rate of QB2- reduction in thylakoid membrane.
    
    BF_mb[24] = (myVars->HPR * Vbf11 - Hrqb - Hvqi - vbfn2);//	BFHs	The proton and protonated buffer species in stroma. The proton concentration is not used in the MB procedure. The reason is that the proton concentration is buffered and therefore did not changed linerly with the generation of the protons.
    BF_mb[26] = - (myVars->HPR * Vbf11 - Hrqb - Hvqi - vbfn2) / 1000 / 0.015;//	PHs, The changes of PH in stoma, 0.03 mol /PH from Laisk et al.
    
    FIBF_mb[24] = BF_mb[24];
    FIBF_mb[26] = BF_mb[26];
    const double Vbf1 = myVars->BF_Vel.Vbf1 ;          // The rate of PQH2 utilization when forming the PQH2.ISP complex
    const double GPQH2_t = GPQH2_qb - Vbf1 + Vqi / 2;// This is the total rate of PQH2 generation
    //const double Vbf7 = myVars->BF_Vel.Vbf7 ;               // The rate of consumption of PQ at the Qi site.
    // CPQ = - GPQH2_qb + Vbf3 - Vbf7;// --unused
    FIBF_mb[7] = 0;	        //	Q	Quinone in thylakoid membrane in free form
    FIBF_mb[11] = GPQH2_t;	//	QH2	The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.
    FIBF_mb[50] = GPQH2_t;	//	QH2	The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.
    return FIBF_mb;
}
