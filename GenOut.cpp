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
 *    MERCHANTABILITY or FITNESS FOR myVars->XanCycle2OUT[1] PARTICULAR PURPOSE.  See the
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
        double ATP = myVars->PS2OUT[9];
        double NADPH = myVars->PS2OUT[10];
        double O2 = myVars->PS2OUT[12];
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

        arr co2a = zeros(100);
        co2a[0] = myVars->PS2OUT[11] * 3 * pow(10, 4);
        co2a[1] = O2 / 1.26;
        if (myVars->FIBF_PSPR_com) {
            co2a[2] = myVars->GLight;
        } else {
            co2a[2] = myVars->V16;
        }
        co2a[3] = myVars->XanCycle2OUT[3];
        co2a[4] = ATP;
        co2a[5] = 0.; // Gs;
        co2a[6] = (myVars->PS2OUT[17] - myVars->PR2OUT[11]) * myVars->AVR;//
        if (myVars->RuACT_EPS_com)
            co2a[6] = (myVars->RuACT_Vel.v6_1 - myVars->PR2OUT[11]) * myVars->AVR;//
        co2a[7] = myVars->PS2OUT[1];//
        co2a[8] = myVars->PS2OUT[2];//
        co2a[9] = myVars->PS2OUT[3];//
        co2a[10] = myVars->PS2OUT[5];//
        co2a[11] = myVars->PS2OUT[6];//
        co2a[12] = myVars->PS2OUT[7];//
        co2a[13] = myVars->PS2OUT[8];//
        co2a[14] = ATP;//
        co2a[15] = NADPH;//
        co2a[16] = myVars->PS2OUT[13];//
        co2a[17] = myVars->PS2OUT[14];//
        co2a[18] = myVars->PS2OUT[15];//
        co2a[19] = myVars->PS2OUT[16];//
        co2a[20] = myVars->PS2OUT[0];//
        co2a[21] = myVars->PR2OUT[0];//
        co2a[22] = myVars->PR2OUT[1];//
        co2a[23] = myVars->PR2OUT[2];//
        co2a[24] = myVars->PR2OUT[3];//
        co2a[25] = myVars->PR2OUT[4];//
        co2a[26] = myVars->PR2OUT[5];//
        co2a[27] = myVars->PR2OUT[6];//
        co2a[28] = myVars->PR2OUT[7];//
        co2a[29] = myVars->PR2OUT[8];//
        co2a[30] = myVars->PR2OUT[9];//
        co2a[31] = myVars->PR2OUT[10];//
        co2a[32] = Fdn;//      Fdn
        co2a[33] = PHs;//      PhS
        co2a[34] = PHl;//      PHl
        co2a[35] = myVars->SUCS2OUT[0];//
        co2a[36] = myVars->SUCS2OUT[1];//
        co2a[37] = myVars->SUCS2OUT[2];//
        co2a[38] = myVars->SUCS2OUT[3];//
        co2a[39] = myVars->SUCS2OUT[4];//
        co2a[40] = myVars->SUCS2OUT[5];//
        co2a[41] = myVars->SUCS2OUT[6];//
        co2a[42] = myVars->SUCS2OUT[7];//
        co2a[43] = myVars->SUCS2OUT[8];//
        co2a[44] = myVars->SUCS2OUT[9];//
        co2a[45] = myVars->SUCS2OUT[10];   //
        co2a[46] = myVars->SUCS2OUT[11];//
        co2a[47] = myVars->XanCycle2OUT[0];//
        co2a[48] = myVars->XanCycle2OUT[1];//
        co2a[49] = myVars->XanCycle2OUT[2];   //
        co2a[50] = myVars->XanCycle2OUT[3];//
        co2a[51] = 0.; // TurgorPressure;
        co2a[52] = 0.; // Gs;
        co2a[53] = 0.; // Posm;


        co2a[99] = t;
        myVars->CO2A.insert(myVars->PS_TIME_N - 1, t, co2a);
    }
}
