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
const double KI583 = 1.55;

void SUCS_Rate(double t, SUCSCon &SUCS_Con, Variables *myVars) {
    ////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    ////////////////////////////////////////////////////////////

    // HexP
    const double TEMP = 1 + myVars->KE541 + 1 / myVars->KE531;

    const double F6Pc = (SUCS_Con.HexPc / TEMP) / myVars->KE531;
    const double G1Pc = SUCS_Con.HexPc * myVars->KE541 / TEMP;

    // T3P
    const double GAPc = SUCS_Con.T3Pc / (1 + myVars->KE501);
    const double DHAPc = SUCS_Con.T3Pc * myVars->KE501 / (1 + myVars->KE501);

    // UDP
    const double UDPc = myVars->SUCS_Pool.UTc - SUCS_Con.UTPc - SUCS_Con.UDPGc;
    const double ADPc = myVars->SUCS_Pool.ATc - SUCS_Con.ATPc;

    // OP
    const double PiTc = myVars->SUCS_Pool.PTc - 2 * (SUCS_Con.FBPc + SUCS_Con.F26BPc) - (SUCS_Con.PGAc + SUCS_Con.T3Pc + SUCS_Con.HexPc + SUCS_Con.SUCP + SUCS_Con.UTPc + SUCS_Con.ATPc);
    const double Pic = (pow((pow(myVars->KE61, 2) + 4 * myVars->KE61 * PiTc), 0.5) - myVars->KE61) / 2;
    const double OPOPc = PiTc - Pic;

    ////// Calculate the rate equations

    // Here the regulation of FBPase activity via the F26BP need to be implemented.
    const double Km521AP = myVars->Km521 * (1 + SUCS_Con.F26BPc / myVars->KI523);
    const double temp51 = myVars->Km512 * myVars->Km513 * (1 + GAPc / myVars->Km512 + DHAPc / myVars->Km513 + SUCS_Con.FBPc / myVars->Km511 + GAPc * DHAPc / (myVars->Km512 * myVars->Km513));
    const double temp52 = Km521AP * (1 + SUCS_Con.FBPc / Km521AP + Pic / myVars->KI522 + F6Pc / myVars->KI521 + Pic * F6Pc / (myVars->KI521 * myVars->KI522));
    const double temp55 = myVars->Km551 * myVars->Km552 * (1 + SUCS_Con.UTPc / myVars->Km552 + G1Pc / myVars->Km551 + SUCS_Con.UDPGc / myVars->Km554 + OPOPc / myVars->Km553 + SUCS_Con.UTPc * G1Pc / (myVars->Km551 * myVars->Km552) + SUCS_Con.UDPGc * OPOPc / (myVars->Km553 * myVars->Km554));
    const double temp56 = (F6Pc + myVars->Km561 * (1 + SUCS_Con.FBPc / myVars->KI562)) * (SUCS_Con.UDPGc + myVars->Km562 * (1 + UDPc / myVars->KI561) * (1 + SUCS_Con.SUCP / myVars->KI563) * (1 + Pic / myVars->KI564) * (1 + SUCS_Con.SUC / myVars->KI565));
    const double temp57 = SUCS_Con.SUCP + myVars->Km571 * (1 + SUCS_Con.SUC / myVars->Ki572);
    const double temp58 = SUCS_Con.F26BPc + myVars->Km581 * (1 + F6Pc / myVars->KI581) * (1 + SUCS_Con.FBPc / 0.08);

    myVars->Km591 = 5 * myVars->SUCRatio[60];
    myVars->Km593 = 0.55 * myVars->SUCRatio[61];
    const double temp59 = (F6Pc + myVars->Km593) * (SUCS_Con.ATPc + myVars->Km591 * (1 + ADPc / myVars->KI591));// This is the orginal equation

    const double Km_in = 0.6 * myVars->SUCRatio[62];

    double Kmpga_in;
    double Kmpga_u;
    double Vpga_u;
    double vpga_use;
    double vpga_in;
    if (!myVars->PSPR_SUCS_com) {
        vpga_in = 0;
        vpga_use = 0;
    } else {
        if (myVars->PS2SUCSV32 == 0) {
            vpga_in = 0;
            vpga_use = 0;
    } else {
            Vpga_u = 1.05 * myVars->SUCRatio[63];
            Kmpga_u = 0.6 * myVars->SUCRatio[64];
            Kmpga_in = 0.6 * myVars->SUCRatio[65];
            vpga_use = SUCS_Con.PGAc * Vpga_u / (SUCS_Con.PGAc + Kmpga_u);// WY201803
            vpga_in = myVars->Vpga_in * Pic / (Pic + Kmpga_in);// WY201803
        }
    }


    if (t > myVars->SUCS_OLD_TIME) {
        myVars->SUCS_TIME_N = myVars->SUCS_TIME_N + 1;
        myVars->SUCS_OLD_TIME = t;
    }
    ////////////////////////////////////////////////////////////////////////////
    // Assign table
    ////////////////////////////////////////////////////////////////////////////////
    myVars->SUCS_Vel.v51 = myVars->V51 * (GAPc * DHAPc - SUCS_Con.FBPc / myVars->KE51) / temp51;//	DHAP+GAP --FBP
    myVars->SUCS_Vel.v52 = myVars->V52 * (SUCS_Con.FBPc - F6Pc * Pic / myVars->KE52) / temp52;//	FBP --F6P + Pi
    myVars->SUCS_Vel.v55 = myVars->V55 * (SUCS_Con.UTPc * G1Pc - SUCS_Con.UDPGc * OPOPc / myVars->KE55) / temp55;//	G1P+UTP --OPOP+UDPG
    myVars->SUCS_Vel.v56 = myVars->V56 * (F6Pc * SUCS_Con.UDPGc - SUCS_Con.SUCP * UDPc / myVars->KE56) / temp56 * 2 * (SUCS_Con.HexPc / (SUCS_Con.HexPc + 2));//	UDPG+F6P--SUCP + UDP
    myVars->SUCS_Vel.v57 = myVars->V57 * (SUCS_Con.SUCP - SUCS_Con.SUC * Pic / myVars->KE57) / temp57;//	SUCP--Pi + SUC
    myVars->SUCS_Vel.v58 = myVars->V58 * SUCS_Con.F26BPc / (temp58 * (1 + Pic / myVars->KI582) * (1 + F6Pc / KI583));//	F26BP--F6P + Pi
    myVars->SUCS_Vel.v59 = myVars->V59 * (SUCS_Con.ATPc * F6Pc - ADPc * SUCS_Con.F26BPc / myVars->KE59) / temp59;//	F6P + ATP --ADP + F26BP
    myVars->SUCS_Vel.v60 = 0.;//	ATP+UDP --UTP + ADP
    myVars->SUCS_Vel.v61 = 0.;//	POPO --2PO
    myVars->SUCS_Vel.v62 = myVars->V62 * SUCS_Con.SUC / (SUCS_Con.SUC + myVars->Km621);//	SUC SINK
    myVars->SUCS_Vel.vdhap_in = myVars->Vdhap_in * Pic / (Pic + Km_in);//	DHAP IN
    myVars->SUCS_Vel.vgap_in = myVars->Vgap_in * Pic / (Pic + Km_in);//	GAP Export from chloroplast
    myVars->SUCS_Vel.vpga_in = vpga_in;//	PGA export from chloroplast
    myVars->SUCS_Vel.vpga_use = vpga_use;//	PGA utilisation in cytosol
    myVars->SUCS_Vel.vatpf = 0.;//	ATP synthesis rate

    if (myVars->record) {
        myVars->SUCS_VEL.insert(myVars->SUCS_TIME_N - 1, t, myVars->SUCS_Vel);
        myVars->SUCS2OUT[0] = SUCS_Con.T3Pc;
        myVars->SUCS2OUT[1] = SUCS_Con.FBPc;
        myVars->SUCS2OUT[2] = SUCS_Con.HexPc;
        myVars->SUCS2OUT[3] = SUCS_Con.F26BPc;
        myVars->SUCS2OUT[4] = SUCS_Con.ATPc;
        myVars->SUCS2OUT[5] = ADPc;
        myVars->SUCS2OUT[6] = OPOPc;
        myVars->SUCS2OUT[7] = SUCS_Con.UDPGc;
        myVars->SUCS2OUT[8] = SUCS_Con.UTPc;
        myVars->SUCS2OUT[9] = SUCS_Con.SUCP;
        myVars->SUCS2OUT[10] = SUCS_Con.SUC;
        myVars->SUCS2OUT[11] = SUCS_Con.PGAc;
    }

    myVars->SUCS2PS_Pic = Pic;                // This is the original code.
}
