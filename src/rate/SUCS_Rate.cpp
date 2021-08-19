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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void SUCS::_Rate(const double t, const SUCSCondition* SUCS_Con, Variables *theVars) {
    ////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    ////////////////////////////////////////////////////////////
    double SUC = SUCS_Con->SUC;
    double Pic, OPOPc;

    if (theVars->useC3) {
        Pic = (pow(pow(SUCS::KE61, 2) + 4 * SUCS::KE61 * PS::getPiTc(), 0.5) - SUCS::KE61) / 2;
        OPOPc = PS::getPiTc() - Pic;
    } else {
        const double PiTc = theVars->SUCS_Pool.PTc - 2 * (SUCS_Con->FBPc + SUCS_Con->F26BPc) - (SUCS_Con->PGAc + SUCS_Con->T3Pc + SUCS_Con->HexPc + SUCS_Con->SUCP + SUCS::UTPc + SUCS::ATPc);
        Pic = (pow(pow(SUCS::KE61, 2) + 4 * SUCS::KE61 * PiTc, 0.5) - SUCS::KE61) / 2;
        OPOPc = PiTc - Pic;
    }
    // HexP
    const double TEMP = 1 + SUCS::KE541 + 1 / SUCS::KE531;

    const double F6Pc = (SUCS_Con->HexPc / TEMP) / SUCS::KE531;
    const double G1Pc = SUCS_Con->HexPc * SUCS::KE541 / TEMP;

    // T3P
    const double GAPc = SUCS_Con->T3Pc / (1 + SUCS::KE501);
    const double DHAPc = SUCS_Con->T3Pc * SUCS::KE501 / (1 + SUCS::KE501);

    // UDP
    const double UDPc = theVars->SUCS_Pool.UTc - SUCS::UTPc - SUCS_Con->UDPGc;
    const double ADPc = theVars->SUCS_Pool.ATc - SUCS::ATPc;

    if (theVars->useC3) {
        const double SUCSV51 = SUCS::V51 * SUCS::Vfactor51 * SUCS::Vf_T51 * pow(Q10_51, (theVars->Tp - 25) / 10);//	;		DHAP+GAP --FBP
        const double SUCSV52 = SUCS::V52 * SUCS::Vfactor52 * SUCS::Vf_T52 * pow(Q10_52, (theVars->Tp - 25) / 10);//	;		FBP --F6P + Pi
        const double SUCSV55 = SUCS::V55 * pow(Q10_55, (theVars->Tp - 25) / 10);//	;		G1P+UTP --OPOP+UDPG
        const double SUCSV56 = SUCS::V56 * SUCS::Vfactor56 * SUCS::Vf_T56 * pow(Q10_56, (theVars->Tp - 25) / 10);//	;		UDPG+F6P--SUCP + UDP
        const double SUCSV57 = SUCS::V57 * SUCS::Vfactor57 * SUCS::Vf_T57 * pow(Q10_57, (theVars->Tp - 25) / 10);//	;		SUCP--Pi + SUC
        const double SUCSV58 = SUCS::V58 * pow(Q10_58, (theVars->Tp - 25) / 10);//	;		F26BP--F6P + Pi

        // Calculate the rate equations

        const double temp51 = SUCS::Km512 * SUCS::Km513 * ( 1 + GAPc / SUCS::Km512 + DHAPc / SUCS::Km513 + SUCS_Con->FBPc / SUCS::Km511 + GAPc * DHAPc / (SUCS::Km512 * SUCS::Km513));
        theVars->SUCS_Vel.v51 = SUCSV51 * (GAPc * DHAPc - SUCS_Con->FBPc / SUCS::KE51) / temp51;
        const double Km521AP = SUCS::Km521 * (1 + SUCS_Con->F26BPc / SUCS::KI523);
        const double temp52 = Km521AP * (1 + SUCS_Con->FBPc / Km521AP + Pic / SUCS::KI522 + F6Pc / SUCS::KI521 + Pic * F6Pc / (SUCS::KI521 * SUCS::KI522));
        theVars->SUCS_Vel.v52 = SUCSV52 * (SUCS_Con->FBPc - F6Pc * Pic / SUCS::KE52) / temp52;

        const double temp55 = SUCS::Km551 * SUCS::Km552 * ( 1 + SUCS::UTPc / SUCS::Km551 + G1Pc / SUCS::Km552 + SUCS_Con->UDPGc / SUCS::Km553 + OPOPc / SUCS::Km554 + SUCS::UTPc * G1Pc / (SUCS::Km551 * SUCS::Km552) + SUCS_Con->UDPGc * OPOPc / (SUCS::Km553 * SUCS::Km554));
        theVars->SUCS_Vel.v55 = SUCSV55 * (SUCS::UTPc * G1Pc - SUCS_Con->UDPGc * OPOPc / SUCS::KE55)/temp55;

        const double temp56 = (F6Pc + SUCS::Km561 * (1 + SUCS_Con->FBPc / SUCS::KI562))*(SUCS_Con->UDPGc + SUCS::Km562 * ( 1 + UDPc / SUCS::KI561) * (1 + SUCS_Con->SUCP / SUCS::KI563) * (1 + Pic / SUCS::KI564) * (1 + SUC / SUCS::KI565));
        theVars->SUCS_Vel.v56 = SUCSV56 * (F6Pc * SUCS_Con->UDPGc - SUCS_Con->SUCP * UDPc / SUCS::KE56) / temp56;

        const double temp57 = SUCS_Con->SUCP + SUCS::Km571 * (1 + SUC / SUCS::Ki572);
        theVars->SUCS_Vel.v57 = SUCSV57 * (SUCS_Con->SUCP - SUC * Pic / SUCS::KE57) / temp57;

        const double temp58 = SUCS::Km581 * (1 + SUCS_Con->F26BPc / SUCS::Km581) * (1 + Pic / SUCS::KI582) * (1 + F6Pc / SUCS::KI581);
        theVars->SUCS_Vel.v58 = SUCSV58 * SUCS_Con->F26BPc / temp58;

        const double temp59 = (F6Pc + SUCS::Km593 * ( 1 + SUCS_Con->F26BPc / SUCS::Km592) * (1 + DHAPc / SUCS::KI592)) * (SUCS::ATPc + SUCS::Km591 * (1 + ADPc/SUCS::KI591));
        theVars->SUCS_Vel.v59 = SUCS::V59 * SUCS::Vfactor59 * SUCS::Vf_T59 * (SUCS::ATPc * F6Pc - ADPc * SUCS_Con->F26BPc / SUCS::KE59) / temp59;

        const double temp60 = SUCS::Km602 * SUCS::Km603 * (1 + SUCS::ATPc / SUCS::Km602 + UDPc / SUCS::Km603 + SUCS::ATPc * UDPc / (SUCS::Km602 * SUCS::Km603) + ADPc / SUCS::Km601 + SUCS::UTPc / SUCS::Km604 + ADPc * SUCS::UTPc / (SUCS::Km601 * SUCS::Km604));
        theVars->SUCS_Vel.v60 = V60 * (SUCS::ATPc * UDPc - ADPc * SUCS::UTPc / SUCS::KE60 )/temp60;

        theVars->SUCS_Vel.v62 = V62 * SUC/(SUC + SUCS::Km621);


        theVars->SUCS_Vel.vatpf = Vmatpf * ADPc * Pic /((ADPc + 0.014)*(Pic + 0.3));

        theVars->SUCS_Vel.vdhap_in = 2 * Pic/(Pic + 2) ;
        theVars->SUCS_Vel.vgap_in  = 2 * Pic/(Pic + 2);

        theVars->SUCS_Vel.vpga_in = 0;
        theVars->SUCS_Vel.vpga_use = SUCS_Con->PGAc * 0.5/(SUCS_Con->PGAc + 1);


    } else {


        ////// Calculate the rate equations

        // Here the regulation of FBPase activity via the F26BP need to be implemented.
        const double Km521AP = SUCS::Km521 * (1 + SUCS_Con->F26BPc / SUCS::KI523);
        const double temp51 = SUCS::Km512 * SUCS::Km513 * (1 + GAPc / SUCS::Km512 + DHAPc / SUCS::Km513 + SUCS_Con->FBPc / SUCS::Km511 + GAPc * DHAPc / (SUCS::Km512 * SUCS::Km513));
        const double temp52 = Km521AP * (1 + SUCS_Con->FBPc / Km521AP + Pic / SUCS::KI522 + F6Pc / SUCS::KI521 + Pic * F6Pc / (SUCS::KI521 * SUCS::KI522));
        const double temp55 = SUCS::Km551 * SUCS::Km552 * (1 + SUCS::UTPc / SUCS::Km552 + G1Pc / SUCS::Km551 + SUCS_Con->UDPGc / SUCS::Km554 + OPOPc / SUCS::Km553 + SUCS::UTPc * G1Pc / (SUCS::Km551 * SUCS::Km552) + SUCS_Con->UDPGc * OPOPc / (SUCS::Km553 * SUCS::Km554));
        const double temp56 = (F6Pc + SUCS::Km561 * (1 + SUCS_Con->FBPc / SUCS::KI562)) * (SUCS_Con->UDPGc + SUCS::Km562 * (1 + UDPc / SUCS::KI561) * (1 + SUCS_Con->SUCP / SUCS::KI563) * (1 + Pic / SUCS::KI564) * (1 + SUC / SUCS::KI565));

        const double temp57 = SUCS_Con->SUCP + SUCS::Km571 * (1 + SUC / SUCS::Ki572);
        const double temp58 = SUCS_Con->F26BPc + SUCS::Km581 * (1 + F6Pc / SUCS::KI581) * (1 + SUCS_Con->FBPc / 0.08);

        SUCS::Km591 = 5 * theVars->SUCRatio[60];
        SUCS::Km593 = 0.55 * theVars->SUCRatio[61];
        const double temp59 = (F6Pc + SUCS::Km593) * (SUCS::ATPc + SUCS::Km591 * (1 + ADPc / SUCS::KI591));// This is the orginal equation

        const double Km_in = 0.6 * theVars->SUCRatio[62];

        double vpga_in, vpga_use;
        if (!theVars->PSPR_SUCS_com) {
            vpga_in = 0;
            vpga_use = 0;
        } else {
            if (PS::getV32() == 0.) {
                vpga_in = 0;
                vpga_use = 0;
            } else {
                const double Vpga_u = 1.05 * theVars->SUCRatio[63];
                const double Kmpga_u = 0.6 * theVars->SUCRatio[64];
                const double Kmpga_in = 0.6 * theVars->SUCRatio[65];
                vpga_use = SUCS_Con->PGAc * Vpga_u / (SUCS_Con->PGAc + Kmpga_u);// WY201803
                vpga_in = SUCS::Vpga_in * Pic / (Pic + Kmpga_in);// WY201803
            }
        }


        ////////////////////////////////////////////////////////////////////////////
        // Assign table
        ////////////////////////////////////////////////////////////////////////////////
        theVars->SUCS_Vel.v51 = SUCS::V51 * (GAPc * DHAPc - SUCS_Con->FBPc / SUCS::KE51) / temp51;  // DHAP+GAP --FBP
        theVars->SUCS_Vel.v52 = SUCS::V52 * (SUCS_Con->FBPc - F6Pc * Pic / SUCS::KE52) / temp52;    // FBP --F6P + Pi
        theVars->SUCS_Vel.v55 = SUCS::V55 * (SUCS::UTPc * G1Pc - SUCS_Con->UDPGc * OPOPc / SUCS::KE55) / temp55; // G1P+UTP --OPOP+UDPG
        theVars->SUCS_Vel.v56 = SUCS::V56 * (F6Pc * SUCS_Con->UDPGc - SUCS_Con->SUCP * UDPc / SUCS::KE56) / temp56 * 2 * (SUCS_Con->HexPc / (SUCS_Con->HexPc + 2));// UDPG+F6P--SUCP + UDP
        theVars->SUCS_Vel.v57 = SUCS::V57 * (SUCS_Con->SUCP - SUC * Pic / SUCS::KE57) / temp57; // SUCP--Pi + SUC
        theVars->SUCS_Vel.v58 = SUCS::V58 * SUCS_Con->F26BPc / (temp58 * (1 + Pic / SUCS::KI582) * (1 + F6Pc / KI583)); // F26BP--F6P + Pi
        theVars->SUCS_Vel.v59 = SUCS::V59 * (SUCS::ATPc * F6Pc - ADPc * SUCS_Con->F26BPc / SUCS::KE59) / temp59;     // F6P + ATP --ADP + F26BP
        theVars->SUCS_Vel.v60 = 0.;  // ATP+UDP --UTP + ADP
        theVars->SUCS_Vel.v61 = 0.;  // POPO --2PO
        theVars->SUCS_Vel.v62 = SUCS::V62 * SUC / (SUC + SUCS::Km621); // SUC SINK
        theVars->SUCS_Vel.vdhap_in = SUCS::Vdhap_in * Pic / (Pic + Km_in);         // DHAP IN
        theVars->SUCS_Vel.vgap_in = SUCS::Vgap_in * Pic / (Pic + Km_in);           // GAP Export from chloroplast
        theVars->SUCS_Vel.vpga_in = vpga_in;   // PGA export from chloroplast
        theVars->SUCS_Vel.vpga_use = vpga_use; // PGA utilisation in cytosol
        theVars->SUCS_Vel.vatpf = 0.;          // ATP synthesis rate
    }
    if (theVars->record) {
        if (t > SUCS::TIME) {
            SUCS::N++;
            SUCS::TIME = t;
        }
        theVars->SUCS_VEL.insert(SUCS::N - 1, t, theVars->SUCS_Vel);
        theVars->SUCS2OUT.T3Pc = SUCS_Con->T3Pc;
        theVars->SUCS2OUT.FBPc = SUCS_Con->FBPc;
        theVars->SUCS2OUT.HexPc = SUCS_Con->HexPc;
        theVars->SUCS2OUT.F26BPc = SUCS_Con->F26BPc;
        theVars->SUCS2OUT.UDPGc = SUCS_Con->UDPGc;
        theVars->SUCS2OUT.SUCP = SUCS_Con->SUCP;
        theVars->SUCS2OUT.SUC = SUC;
        theVars->SUCS2OUT.PGAc = SUCS_Con->PGAc;
    }
#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->SUCS_Vel)
#endif
    SUCS::SUCS2PS_Pic = Pic;                // This is the original code.
}
