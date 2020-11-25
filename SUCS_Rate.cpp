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

#include "Variables.hpp"

void SUCS_Rate(double t, SUCSCon &SUCS_Con, Variables *myVars) {
    const double SUCSV51 = myVars->V51;	//	;		DHAP+GAP --FBP
    const double SUCSV52 = myVars->V52;	//	;		FBP --F6P + Pi
    const double SUCSV55 = myVars->V55;	//	;		G1P+UTP --OPOP+UDPG
    const double SUCSV56 = myVars->V56;	//	;		UDPG+F6P--SUCP + UDP
    const double SUCSV57 = myVars->V57;	//	;		SUCP--Pi + SUC
    const double SUCSV58 = myVars->V58;	//	;		F26BP--F6P + Pi
    const double SUCSV59 = myVars->V59;	//	;		F6P + ATP --ADP + F26BP
    const double SUCSV62 = myVars->V62;	//	;		SUC Sink
    const double SUCSVdhap_in = myVars->Vdhap_in;    //   DHAP export from chloroplast
    const double SUCSVgap_in = myVars->Vgap_in;    //   GAP export from chloroplast
    const double SUCSVpga_in = myVars->Vpga_in;    //   PGA export from chloropalst

    // The rate constant used in the model

    const double T3Pc = SUCS_Con.T3Pc;
    const double FBPc = SUCS_Con.FBPc;
    const double HexPc = SUCS_Con.HexPc;
    const double F26BPc = SUCS_Con.F26BPc;
    const double ATPc = SUCS_Con.ATPc;
    double ADPc = SUCS_Con.ADPc;
    double OPOPc = SUCS_Con.OPOPc;
    const double UDPGc = SUCS_Con.UDPGc;
    const double UTPc = SUCS_Con.UTPc;
    const double SUCP = SUCS_Con.SUCP;
    const double SUC = SUCS_Con.SUC;
    const double PGAc = SUCS_Con.PGAc;

    const double ATc = myVars->SUCS_Pool.ATc;
    const double UTc = myVars->SUCS_Pool.UTc;
    const double PTc = myVars->SUCS_Pool.PTc;

    ////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    ////////////////////////////////////////////////////////////

    // HexP
    const double TEMP = 1 + myVars->KE541 + 1 / myVars->KE531;

    const double G6Pc = HexPc / TEMP;
    const double F6Pc = G6Pc / myVars->KE531;
    const double G1Pc = HexPc * myVars->KE541 / TEMP;

    // T3P
    const double GAPc = T3Pc / (1 + myVars->KE501);
    const double DHAPc = T3Pc * myVars->KE501 / (1 + myVars->KE501);

    // UDP
    const double UDPc = UTc - UTPc - UDPGc;
    ADPc = ATc - ATPc;

    // OP
    const double PiTc = PTc - 2 * (FBPc + F26BPc) - (PGAc + T3Pc + HexPc + SUCP + UTPc + ATPc);
    const double Pic =( pow(( pow(myVars->KE61, 2) + 4 * myVars->KE61 * PiTc), 0.5) - myVars->KE61) / 2;
    OPOPc = PiTc - Pic;

    ////// Calculate the rate equations
    const double temp51 = myVars->Km512 * myVars->Km513 * (1 + GAPc / myVars->Km512 + DHAPc / myVars->Km513 + FBPc / myVars->Km511 + GAPc * DHAPc / (myVars->Km512 * myVars->Km513));
    const double v51 = SUCSV51 * (GAPc * DHAPc - FBPc / myVars->KE51) / temp51;

    // Here the regulation of FBPase activity via the F26BP need to be implemented.
    const double Km521AP = myVars->Km521 * (1 + F26BPc / myVars->KI523);
    const double temp52 = Km521AP * (1 + FBPc / Km521AP + Pic / myVars->KI522 + F6Pc / myVars->KI521 + Pic * F6Pc / (myVars->KI521 * myVars->KI522));
    const double v52 = SUCSV52 * (FBPc - F6Pc * Pic / myVars->KE52) / temp52;

    const double temp55 = myVars->Km551 * myVars->Km552 * (1 + UTPc / myVars->Km552 + G1Pc / myVars->Km551 + UDPGc / myVars->Km554 + OPOPc / myVars->Km553 + UTPc * G1Pc / (myVars->Km551 * myVars->Km552) + UDPGc * OPOPc / (myVars->Km553 * myVars->Km554));
    const double v55 = SUCSV55 * (UTPc * G1Pc - UDPGc * OPOPc / myVars->KE55) / temp55;

    const double temp56 = (F6Pc + myVars->Km561 * (1 + FBPc / myVars->KI562)) * (UDPGc + myVars->Km562 * (1 + UDPc / myVars->KI561) * (1 + SUCP / myVars->KI563) * (1 + Pic / myVars->KI564) * (1 + SUC / myVars->KI565));
    const double v56 = SUCSV56 * (F6Pc * UDPGc - SUCP * UDPc / myVars->KE56) / temp56 * 2 * (HexPc / (HexPc + 2));

    const double temp57 = SUCP + myVars->Km571 * (1 + SUC / myVars->Ki572);
    const double v57 = SUCSV57 * (SUCP - SUC * Pic / myVars->KE57) / temp57;


    const double temp58 = F26BPc + myVars->Km581 * (1 + F6Pc / myVars->KI581) * (1 + FBPc / 0.08);
    const double KI583 = 1.55;
    const double v58 = SUCSV58 * F26BPc / (temp58 * (1 + Pic / myVars->KI582) * (1 + F6Pc / KI583));

    const double SUCSV59n = SUCSV59;

    myVars->Km591 = 5 * myVars->SUCRatio[60];

    const double KmF6P = 0.55 * myVars->SUCRatio[61];
    myVars->Km593 = KmF6P;

    const double temp59 = (F6Pc + myVars->Km593) * (ATPc + myVars->Km591 * (1 + ADPc / myVars->KI591));// This is the orginal equation

    const double v59 = SUCSV59n * (ATPc * F6Pc - ADPc * F26BPc / myVars->KE59) / temp59;

    const double v60 = 0;

    const double v62 = SUCSV62 * SUC / (SUC + myVars->Km621);

    const double vatpf = 0;
    const double Km_in = 0.6 * myVars->SUCRatio[62];
    const double vdhap_in = SUCSVdhap_in * Pic / (Pic + Km_in);
    const double vgap_in = SUCSVgap_in * Pic / (Pic + Km_in);

    //global PSPR_SUCS_com;

    double Kmpga_in;
    double Kmpga_u;
    double Vpga_u;
    double vpga_use;
    double vpga_in;
    if (!myVars->PSPR_SUCS_com) {
        vpga_in = 0;
        vpga_use = 0;
    } else {
        //global myVars->PS2SUCSV32;
        if (myVars->PS2SUCSV32 == 0) {
            vpga_in = 0;
            vpga_use = 0;
    } else {
            Vpga_u = 1.05 * myVars->SUCRatio[63];
            Kmpga_u = 0.6 * myVars->SUCRatio[64];
            Kmpga_in = 0.6 * myVars->SUCRatio[65];
            vpga_use = PGAc * Vpga_u / (PGAc + Kmpga_u);// WY201803
            vpga_in = SUCSVpga_in * Pic / (Pic + Kmpga_in);// WY201803
        }
    }


    if (t > myVars->SUCS_OLD_TIME) {
            myVars->SUCS_TIME_N = myVars->SUCS_TIME_N + 1;
            myVars->SUCS_OLD_TIME = t;
        }
    ////////////////////////////////////////////////////////////////////////////
    // Assign table
    ////////////////////////////////////////////////////////////////////////////////
    myVars->SUCS_Vel.v51 = v51;//	DHAP+GAP --FBP
    myVars->SUCS_Vel.v52 = v52;//	FBP --F6P + Pi
    myVars->SUCS_Vel.v55 = v55;//	G1P+UTP --OPOP+UDPG
    myVars->SUCS_Vel.v56 = v56;//	UDPG+F6P--SUCP + UDP
    myVars->SUCS_Vel.v57 = v57;//	SUCP--Pi + SUC
    myVars->SUCS_Vel.v58 = v58;//	F26BP--F6P + Pi
    myVars->SUCS_Vel.v59 = v59;//	F6P + ATP --ADP + F26BP
    myVars->SUCS_Vel.v60 = v60;//	ATP+UDP --UTP + ADP
    myVars->SUCS_Vel.v61 = 0;//	POPO --2PO
    myVars->SUCS_Vel.v62 = v62;//	SUC SINK
    myVars->SUCS_Vel.vdhap_in = vdhap_in;//	DHAP IN
    myVars->SUCS_Vel.vgap_in = vgap_in;//	GAP Export from chloroplast
    myVars->SUCS_Vel.vpga_in = vpga_in;//	PGA export from chloroplast
    myVars->SUCS_Vel.vpga_use = vpga_use;//	PGA utilisation in cytosol
    myVars->SUCS_Vel.vatpf = vatpf;//	ATP synthesis rate

    if (myVars->record) {
        myVars->SUCS_VEL.insert(myVars->SUCS_TIME_N - 1, t, myVars->SUCS_Vel);
    }

    myVars->SUCS2PS_Pic = Pic;                // This is the original code.

    myVars->SUCS2CM_vdhap = vdhap_in;   //	DHAP IN
    myVars->SUCS2CM_vgap = vgap_in;   //	GAP Export from chloroplast
    myVars->SUCS2CM_vpga = vpga_in;   //	PGA export from chloroplast

    myVars->SUCS2OUT[0] = T3Pc;
    myVars->SUCS2OUT[1] = FBPc;
    myVars->SUCS2OUT[2] = HexPc;
    myVars->SUCS2OUT[3] = F26BPc;
    myVars->SUCS2OUT[4] = ATPc;
    myVars->SUCS2OUT[5] = ADPc;
    myVars->SUCS2OUT[6] = OPOPc;
    myVars->SUCS2OUT[7] = UDPGc;
    myVars->SUCS2OUT[8] = UTPc;
    myVars->SUCS2OUT[9] = SUCP;
    myVars->SUCS2OUT[10] = SUC;
    myVars->SUCS2OUT[11] = PGAc;
}
