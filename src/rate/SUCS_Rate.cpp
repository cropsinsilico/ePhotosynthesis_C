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
#include "modules/SUCS.hpp"
#include "modules/PS.hpp"
#define Vmatpf 0.25


const double KI583 = 1.55;

void SUCS::_Rate(const double t, const SUCSCon* SUCS_Con, Variables *theVars) {
    ////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    ////////////////////////////////////////////////////////////

    const double Pic = (pow(pow(KE61, 2) + 4 * KE61 * theVars->PiTc, 0.5) - KE61) / 2;
    const double OPOPc = theVars->PiTc - Pic;

    // HexP
    const double TEMP = 1 + KE541 + 1 / KE531;

    const double F6Pc = (SUCS_Con->HexPc / TEMP) / KE531;
    const double G1Pc = SUCS_Con->HexPc * KE541 / TEMP;

    // T3P
    const double GAPc = SUCS_Con->T3Pc / (1 + KE501);
    const double DHAPc = SUCS_Con->T3Pc * KE501 / (1 + KE501);

    // UDP
    const double UDPc = theVars->SUCS_Pool.UTc - SUCS_Con->UTPc - SUCS_Con->UDPGc;
    const double ADPc = theVars->SUCS_Pool.ATc - SUCS_Con->ATPc;

    if (theVars->useC3) {
        const double SUCSV51 = V51*Vfactor51*Vf_T51 * pow(Q10_51, (theVars->Tp - 25) / 10);//	;		DHAP+GAP --FBP
        const double SUCSV52 = V52*Vfactor52*Vf_T52 * pow(Q10_52, (theVars->Tp - 25) / 10);//	;		FBP --F6P + Pi
        const double SUCSV55 = V55 * pow(Q10_55, (theVars->Tp - 25) / 10);//	;		G1P+UTP --OPOP+UDPG
        const double SUCSV56 = V56*Vfactor56*Vf_T56 * pow(Q10_56, (theVars->Tp - 25) / 10);//	;		UDPG+F6P--SUCP + UDP
        const double SUCSV57 = V57*Vfactor57*Vf_T57 * pow(Q10_57, (theVars->Tp - 25) / 10);//	;		SUCP--Pi + SUC
        const double SUCSV58 = V58 * pow(Q10_58, (theVars->Tp - 25) / 10);//	;		F26BP--F6P + Pi

        // Calculate the rate equations

        const double temp51 =Km512 * Km513 * ( 1 + GAPc/Km512 + DHAPc/Km513 + SUCS_Con->FBPc/Km511 + GAPc * DHAPc/(Km512 * Km513));
        theVars->SUCS_Vel.v51 = SUCSV51 * (GAPc * DHAPc - SUCS_Con->FBPc/KE51)/temp51;
        const double Km521AP = Km521 * (1 + SUCS_Con->F26BPc/KI523);
        const double temp52 = Km521AP * (1 + SUCS_Con->FBPc/Km521AP + Pic/KI522 + F6Pc/KI521 + Pic * F6Pc /(KI521 * KI522));
        theVars->SUCS_Vel.v52 = SUCSV52 * (SUCS_Con->FBPc - F6Pc  * Pic/KE52)/temp52;

        const double temp55 = Km551 * Km552 * ( 1 + SUCS_Con->UTPc/Km551 + G1Pc/Km552 + SUCS_Con->UDPGc/Km553 + OPOPc/Km554 + SUCS_Con->UTPc * G1Pc/(Km551 * Km552) + SUCS_Con->UDPGc * OPOPc/(Km553 * Km554));
        theVars->SUCS_Vel.v55 = SUCSV55 * (SUCS_Con->UTPc * G1Pc - SUCS_Con->UDPGc * OPOPc/KE55)/temp55;

        const double temp56 = (F6Pc + Km561 * (1 + SUCS_Con->FBPc/KI562))*(SUCS_Con->UDPGc + Km562 * ( 1 + UDPc/KI561)*(1+SUCS_Con->SUCP/KI563)*(1+Pic/KI564)*(1+SUCS_Con->SUC/KI565));
        theVars->SUCS_Vel.v56 = SUCSV56 * (F6Pc * SUCS_Con->UDPGc - SUCS_Con->SUCP * UDPc/KE56)/temp56;

        const double temp57 = SUCS_Con->SUCP + Km571 * (1 + SUCS_Con->SUC/Ki572);
        theVars->SUCS_Vel.v57 = SUCSV57 * (SUCS_Con->SUCP - SUCS_Con->SUC * Pic/KE57)/temp57;

        const double temp58 = Km581 * (1 + SUCS_Con->F26BPc/Km581) * (1 + Pic/KI582) * (1+F6Pc/KI581);
        theVars->SUCS_Vel.v58 = SUCSV58 * SUCS_Con->F26BPc/temp58;

        const double temp59 = (F6Pc + Km593 * ( 1 + SUCS_Con->F26BPc/Km592)*(1+DHAPc/KI592)) * (SUCS_Con->ATPc + Km591 * (1 + ADPc/KI591));
        theVars->SUCS_Vel.v59 = V59*Vfactor59*Vf_T59 * (SUCS_Con->ATPc * F6Pc - ADPc * SUCS_Con->F26BPc/KE59)/temp59;

        const double temp60 = Km602 * Km603 * (1 + SUCS_Con->ATPc/Km602 + UDPc/Km603 + SUCS_Con->ATPc * UDPc/(Km602 * Km603) + ADPc/Km601 + SUCS_Con->UTPc/Km604 + ADPc * SUCS_Con->UTPc/(Km601 * Km604));
        theVars->SUCS_Vel.v60 = V60 * (SUCS_Con->ATPc * UDPc - ADPc * SUCS_Con->UTPc/KE60 )/temp60;

        theVars->SUCS_Vel.v62 = V62 * SUCS_Con->SUC/(SUCS_Con->SUC + Km621);


        theVars->SUCS_Vel.vatpf = Vmatpf * ADPc * Pic /((ADPc + 0.014)*(Pic + 0.3));

        theVars->SUCS_Vel.vdhap_in = 2 * Pic/(Pic + 2) ;
        theVars->SUCS_Vel.vgap_in  = 2 * Pic/(Pic + 2);

        theVars->SUCS_Vel.vpga_in = 0;
        theVars->SUCS_Vel.vpga_use = SUCS_Con->PGAc * 0.5/(SUCS_Con->PGAc + 1);


    } else {


        ////// Calculate the rate equations

        // Here the regulation of FBPase activity via the F26BP need to be implemented.
        const double Km521AP = Km521 * (1 + SUCS_Con->F26BPc / KI523);
        const double temp51 = Km512 * Km513 * (1 + GAPc / Km512 + DHAPc / Km513 + SUCS_Con->FBPc / Km511 + GAPc * DHAPc / (Km512 * Km513));
        const double temp52 = Km521AP * (1 + SUCS_Con->FBPc / Km521AP + Pic / KI522 + F6Pc / KI521 + Pic * F6Pc / (KI521 * KI522));
        const double temp55 = Km551 * Km552 * (1 + SUCS_Con->UTPc / Km552 + G1Pc / Km551 + SUCS_Con->UDPGc / Km554 + OPOPc / Km553 + SUCS_Con->UTPc * G1Pc / (Km551 * Km552) + SUCS_Con->UDPGc * OPOPc / (Km553 * Km554));
        const double temp56 = (F6Pc + Km561 * (1 + SUCS_Con->FBPc / KI562)) * (SUCS_Con->UDPGc + Km562 * (1 + UDPc / KI561) * (1 + SUCS_Con->SUCP / KI563) * (1 + Pic / KI564) * (1 + SUCS_Con->SUC / KI565));
        const double temp57 = SUCS_Con->SUCP + Km571 * (1 + SUCS_Con->SUC / Ki572);
        const double temp58 = SUCS_Con->F26BPc + Km581 * (1 + F6Pc / KI581) * (1 + SUCS_Con->FBPc / 0.08);

        Km591 = 5 * theVars->SUCRatio[60];
        Km593 = 0.55 * theVars->SUCRatio[61];
        const double temp59 = (F6Pc + Km593) * (SUCS_Con->ATPc + Km591 * (1 + ADPc / KI591));// This is the orginal equation

        const double Km_in = 0.6 * theVars->SUCRatio[62];

        double vpga_in, vpga_use;
        if (!theVars->PSPR_SUCS_com) {
            vpga_in = 0;
            vpga_use = 0;
        } else {
            if (PS::V32 == 0.) {
                vpga_in = 0;
                vpga_use = 0;
            } else {
                const double Vpga_u = 1.05 * theVars->SUCRatio[63];
                const double Kmpga_u = 0.6 * theVars->SUCRatio[64];
                const double Kmpga_in = 0.6 * theVars->SUCRatio[65];
                vpga_use = SUCS_Con->PGAc * Vpga_u / (SUCS_Con->PGAc + Kmpga_u);// WY201803
                vpga_in = Vpga_in * Pic / (Pic + Kmpga_in);// WY201803
            }
        }


        if (t > theVars->SUCS_OLD_TIME) {
            theVars->SUCS_TIME_N = theVars->SUCS_TIME_N + 1;
            theVars->SUCS_OLD_TIME = t;
        }
        ////////////////////////////////////////////////////////////////////////////
        // Assign table
        ////////////////////////////////////////////////////////////////////////////////
        theVars->SUCS_Vel.v51 = V51 * (GAPc * DHAPc - SUCS_Con->FBPc / KE51) / temp51;  // DHAP+GAP --FBP
        theVars->SUCS_Vel.v52 = V52 * (SUCS_Con->FBPc - F6Pc * Pic / KE52) / temp52;    // FBP --F6P + Pi
        theVars->SUCS_Vel.v55 = V55 * (SUCS_Con->UTPc * G1Pc - SUCS_Con->UDPGc * OPOPc / KE55) / temp55; // G1P+UTP --OPOP+UDPG
        theVars->SUCS_Vel.v56 = V56 * (F6Pc * SUCS_Con->UDPGc - SUCS_Con->SUCP * UDPc / KE56) / temp56 * 2 * (SUCS_Con->HexPc / (SUCS_Con->HexPc + 2));// UDPG+F6P--SUCP + UDP
        theVars->SUCS_Vel.v57 = V57 * (SUCS_Con->SUCP - SUCS_Con->SUC * Pic / KE57) / temp57; // SUCP--Pi + SUC
        theVars->SUCS_Vel.v58 = V58 * SUCS_Con->F26BPc / (temp58 * (1 + Pic / KI582) * (1 + F6Pc / KI583)); // F26BP--F6P + Pi
        theVars->SUCS_Vel.v59 = V59 * (SUCS_Con->ATPc * F6Pc - ADPc * SUCS_Con->F26BPc / KE59) / temp59;     // F6P + ATP --ADP + F26BP
        theVars->SUCS_Vel.v60 = 0.;  // ATP+UDP --UTP + ADP
        theVars->SUCS_Vel.v61 = 0.;  // POPO --2PO
        theVars->SUCS_Vel.v62 = V62 * SUCS_Con->SUC / (SUCS_Con->SUC + Km621); // SUC SINK
        theVars->SUCS_Vel.vdhap_in = Vdhap_in * Pic / (Pic + Km_in);         // DHAP IN
        theVars->SUCS_Vel.vgap_in = Vgap_in * Pic / (Pic + Km_in);           // GAP Export from chloroplast
        theVars->SUCS_Vel.vpga_in = vpga_in;   // PGA export from chloroplast
        theVars->SUCS_Vel.vpga_use = vpga_use; // PGA utilisation in cytosol
        theVars->SUCS_Vel.vatpf = 0.;          // ATP synthesis rate
    }
    if (theVars->record) {
        theVars->SUCS_VEL.insert(theVars->SUCS_TIME_N - 1, t, theVars->SUCS_Vel);
        theVars->SUCS2OUT.T3Pc = SUCS_Con->T3Pc;
        theVars->SUCS2OUT.FBPc = SUCS_Con->FBPc;
        theVars->SUCS2OUT.HexPc = SUCS_Con->HexPc;
        theVars->SUCS2OUT.F26BPc = SUCS_Con->F26BPc;
        theVars->SUCS2OUT.ATPc = SUCS_Con->ATPc;
        theVars->SUCS2OUT.ADPc = SUCS_Con->ADPc;
        theVars->SUCS2OUT.OPOPc = SUCS_Con->OPOPc;
        theVars->SUCS2OUT.UDPGc = SUCS_Con->UDPGc;
        theVars->SUCS2OUT.UTPc = SUCS_Con->UTPc;
        theVars->SUCS2OUT.SUCP = SUCS_Con->SUCP;
        theVars->SUCS2OUT.SUC = SUCS_Con->SUC;
        theVars->SUCS2OUT.PGAc = SUCS_Con->PGAc;
    }

    theVars->SUCS2PS_Pic = Pic;                // This is the original code.
}
