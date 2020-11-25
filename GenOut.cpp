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

// This is a function to generate output from the program.

void GenOut(double t, Variables *myVars) {
    if (!myVars->EPS_SUCS_com)
        myVars->SUCS2OUT = zeros(12);

    if (!myVars->XanCycle_BF_com)
        myVars->XanCycle2OUT = zeros(5);

    if (myVars->record) {
        const double RuBP = myVars->PS2OUT[0];
        const double PGA = myVars->PS2OUT[1];
        const double DPGA = myVars->PS2OUT[2];
        const double T3P = myVars->PS2OUT[3];
        const double FBP = myVars->PS2OUT[5];
        const double E4P = myVars->PS2OUT[6];
        const double S7P = myVars->PS2OUT[7];
        const double SBP = myVars->PS2OUT[8];
        double ATP = myVars->PS2OUT[9];
        double NADPH = myVars->PS2OUT[10];
        const double CO2 = myVars->PS2OUT[11];
        double O2 = myVars->PS2OUT[12];
        const double HexP = myVars->PS2OUT[13];
        const double PenP = myVars->PS2OUT[14];
        const double Pi = myVars->PS2OUT[15];
        const double ADP = myVars->PS2OUT[16];
        const double v1 = myVars->PS2OUT[17];

        const double Gcea = myVars->PR2OUT[0];
        const double Gca = myVars->PR2OUT[1];
        const double Pga = myVars->PR2OUT[2];
        const double Pgca = myVars->PR2OUT[3];
        const double Gcac = myVars->PR2OUT[4];
        const double Goac = myVars->PR2OUT[5];
        const double Serc = myVars->PR2OUT[6];
        const double Glyc = myVars->PR2OUT[7];
        const double Hprc = myVars->PR2OUT[8];
        const double Gceac = myVars->PR2OUT[9];
        const double Rubp = myVars->PR2OUT[10];
        const double v131 = myVars->PR2OUT[11];
        double Fdn = 0;
        double PHs = 0;
        double PHl = 0;

        O2 = myVars->O2_cond;

        if (myVars->FIBF_PSPR_com) {
            Fdn = myVars->BF2OUT[0];
            PHs = myVars->BF2OUT[1];
            PHl = myVars->BF2OUT[2];
            NADPH = myVars->BF2OUT[3];
            ATP = myVars->BF2OUT[4];
        }
        const double T3Pc = myVars->SUCS2OUT[0];
        const double FBPc = myVars->SUCS2OUT[1];
        const double HexPc = myVars->SUCS2OUT[2];
        const double F26BPc = myVars->SUCS2OUT[3];
        const double ATPc = myVars->SUCS2OUT[4];
        const double ADPc = myVars->SUCS2OUT[5];
        const double OPOPc = myVars->SUCS2OUT[6];
        const double UDPGc = myVars->SUCS2OUT[7];
        const double UTPc = myVars->SUCS2OUT[8];
        const double SUCP = myVars->SUCS2OUT[9];
        const double SUC = myVars->SUCS2OUT[10];
        const double PGAc = myVars->SUCS2OUT[11];

        const double V = myVars->XanCycle2OUT[0];
        const double A = myVars->XanCycle2OUT[1];
        const double Z = myVars->XanCycle2OUT[2];
        const double ABA = myVars->XanCycle2OUT[3];

        arr co2a = zeros(100);
        co2a[0] = CO2 * 3 * pow(10, 4);
        co2a[1] = O2 / 1.26;
        if (myVars->FIBF_PSPR_com) {
            co2a[2] = myVars->GLight;
        } else {
            co2a[2] = myVars->V16;
        }
        co2a[3] = ABA;
        co2a[4] = ATP;
        co2a[5] = 0.; // Gs;
        co2a[6] = (v1 - v131) * myVars->AVR;//
        if (myVars->RuACT_EPS_com)
            co2a[6] = (myVars->RuACT_Vel.v6_1 - v131) * myVars->AVR;//
        co2a[7] = PGA;//
        co2a[8] = DPGA;//
        co2a[9] = T3P;//
        co2a[10] = FBP;//
        co2a[11] = E4P;//
        co2a[12] = S7P;//
        co2a[13] = SBP;//
        co2a[14] = ATP;//
        co2a[15] = NADPH;//
        co2a[16] = HexP;//
        co2a[17] = PenP;//
        co2a[18] = Pi;//
        co2a[19] = ADP;//
        co2a[20] = RuBP;//
        co2a[21] = Gcea;//
        co2a[22] = Gca;//
        co2a[23] = Pga;//
        co2a[24] = Pgca;//
        co2a[25] = Gcac;//
        co2a[26] = Goac;//
        co2a[27] = Serc;//
        co2a[28] = Glyc;//
        co2a[29] = Hprc;//
        co2a[30] = Gceac;//
        co2a[31] = Rubp;//
        co2a[32] = Fdn;//      Fdn
        co2a[33] = PHs;//      PhS
        co2a[34] = PHl;//      PHl
        co2a[35] = T3Pc;//
        co2a[36] = FBPc;//
        co2a[37] = HexPc;//
        co2a[38] = F26BPc;//
        co2a[39] = ATPc;//
        co2a[40] = ADPc;//
        co2a[41] = OPOPc;//
        co2a[42] = UDPGc;//
        co2a[43] = UTPc;//
        co2a[44] = SUCP;//
        co2a[45] = SUC;   //
        co2a[46] = PGAc;//
        co2a[47] = V;//
        co2a[48] = A;//
        co2a[49] = Z;   //
        co2a[50] = ABA;//
        co2a[51] = 0.; // TurgorPressure;
        co2a[52] = 0.; // Gs;
        co2a[53] = 0.; // Posm;


        co2a[99] = t;
        myVars->CO2A.insert(myVars->PS_TIME_N - 1, t, co2a);
    }
}
