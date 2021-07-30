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
 *    MERCHANTABILITY or FITNESS FOR theVars->XanCycle2OUT[1] PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/
#include <math.h>
#include "Variables.hpp"
#include "modules/PS.hpp"

// This is a function to generate output from the program.

void GenOut(double t, Variables *theVars) {
    if (!theVars->EPS_SUCS_com)
        theVars->SUCS2OUT.clear();

    if (!theVars->XanCycle_BF_com)
        theVars->XanCycle2OUT.clear();

    if (theVars->record) {
        double ATP = theVars->PS2OUT.ATP;
        double NADPH = PS::_NADPH;
        double O2 = theVars->O2_cond;
        double Fdn = 0;
        double PHs = 0;
        double PHl = 0;

        O2 = theVars->O2_cond;

        if (theVars->FIBF_PSPR_com) {
            Fdn = theVars->BF2OUT[0];
            PHs = theVars->BF2OUT[1];
            PHl = theVars->BF2OUT[2];
            NADPH = theVars->BF2OUT[3];
            ATP = theVars->BF2OUT[4];
        }

        arr co2a = zeros(100);
        co2a[0] = theVars->CO2_cond * 3 * pow(10, 4);
        co2a[1] = O2 / 1.26;
        if (theVars->FIBF_PSPR_com) {
            co2a[2] = theVars->GLight;
        } else {
            co2a[2] = theVars->V16;
        }
        co2a[3] = theVars->XanCycle2OUT.ABA;
        co2a[4] = ATP;
        co2a[5] = 0.; // Gs;
        co2a[6] = (theVars->PS2OUT._v1 - theVars->PR2OUT._v131) * theVars->AVR;//
        if (theVars->RuACT_EPS_com)
            co2a[6] = (theVars->RuACT_Vel.v6_1 - theVars->PR2OUT._v131) * theVars->AVR;//
        co2a[7] = theVars->PS2OUT.PGA;//
        co2a[8] = theVars->PS2OUT.DPGA;//
        co2a[9] = theVars->PS2OUT.T3P;//
        co2a[10] = theVars->PS2OUT.FBP;//
        co2a[11] = theVars->PS2OUT.E4P;//
        co2a[12] = theVars->PS2OUT.S7P;//
        co2a[13] = theVars->PS2OUT.SBP;//
        co2a[14] = ATP;//
        co2a[15] = NADPH;//
        co2a[16] = theVars->PS2OUT.HexP;//
        co2a[17] = theVars->PS2OUT.PenP;//
        co2a[18] = theVars->PS2OUT._Pi;//
        co2a[19] = theVars->PS2OUT._ADP;//
        co2a[20] = theVars->PS2OUT.RuBP;//
        co2a[21] = theVars->PR2OUT.GCEA;//
        co2a[22] = theVars->PR2OUT.GCA;//
        co2a[24] = theVars->PR2OUT.PGCA;//
        co2a[25] = theVars->PR2OUT.GCAc;//
        co2a[26] = theVars->PR2OUT.GOAc;//
        co2a[27] = theVars->PR2OUT.SERc;//
        co2a[28] = theVars->PR2OUT.GLYc;//
        co2a[29] = theVars->PR2OUT.HPRc;//
        co2a[30] = theVars->PR2OUT.GCEAc;//
        co2a[31] = theVars->PR2OUT.RuBP;//
        co2a[32] = Fdn;//      Fdn
        co2a[33] = PHs;//      PhS
        co2a[34] = PHl;//      PHl
        co2a[35] = theVars->SUCS2OUT.T3Pc;//
        co2a[36] = theVars->SUCS2OUT.FBPc;//
        co2a[37] = theVars->SUCS2OUT.HexPc;//
        co2a[38] = theVars->SUCS2OUT.F26BPc;//
        co2a[42] = theVars->SUCS2OUT.UDPGc;//
        co2a[44] = theVars->SUCS2OUT.SUCP;//
        co2a[45] = theVars->SUCS2OUT.SUC;   //
        co2a[46] = theVars->SUCS2OUT.PGAc;//
        co2a[47] = theVars->XanCycle2OUT.Vx;//
        co2a[48] = theVars->XanCycle2OUT.Ax;//
        co2a[49] = theVars->XanCycle2OUT.Zx;   //
        co2a[50] = theVars->XanCycle2OUT.ABA;//
        co2a[51] = 0.; // TurgorPressure;
        co2a[52] = 0.; // Gs;
        co2a[53] = 0.; // Posm;


        co2a[99] = t;
        theVars->CO2A.insert(PS::getN() - 1, t, co2a);
    }
}

void makeFluxTR(Variables *theVars) {
    theVars->FluxTR = zeros(120);
    theVars->FluxTR[0] = theVars->RuACT_VEL.getLastData().v6_1;//PS
    theVars->FluxTR[1] = theVars->RuACT_VEL.getLastData().v6_2;//PR
    theVars->FluxTR[2] = theVars->PS_VEL.getLastData().v2;// v2
    theVars->FluxTR[3] = theVars->PS_VEL.getLastData().v3;// v3
    theVars->FluxTR[4] = theVars->PS_VEL.getLastData().v5;// v5
    theVars->FluxTR[5] = theVars->PS_VEL.getLastData().v6;// v6
    theVars->FluxTR[6] = theVars->PS_VEL.getLastData().v7;// v7
    theVars->FluxTR[7] = theVars->PS_VEL.getLastData().v8;// v8
    theVars->FluxTR[8] = theVars->PS_VEL.getLastData().v9;// v9
    theVars->FluxTR[9] = theVars->PS_VEL.getLastData().v10;// v10
    theVars->FluxTR[10] = theVars->PS_VEL.getLastData().v13;// v13
    theVars->FluxTR[11] = theVars->PS_VEL.getLastData().v23;// v23
    theVars->FluxTR[12] = theVars->PS_VEL.getLastData().v24;// v24
    theVars->FluxTR[13] = theVars->PS_VEL.getLastData().v25;// v25
    theVars->FluxTR[14] = theVars->PR_VEL.getLastData().v112;//v112
    theVars->FluxTR[15] = theVars->PR_VEL.getLastData().v113;//v113
    theVars->FluxTR[16] = theVars->PR_VEL.getLastData().v121;//v121
    theVars->FluxTR[17] = theVars->PR_VEL.getLastData().v122;//v122
    theVars->FluxTR[18] = theVars->PR_VEL.getLastData().v123;//v123
    theVars->FluxTR[19] = theVars->PR_VEL.getLastData().v124;//v124
    theVars->FluxTR[20] = theVars->PR_VEL.getLastData().v131;//v131
    theVars->FluxTR[21] = theVars->PR_VEL.getLastData().v1in;//vlin
    theVars->FluxTR[22] = theVars->PR_VEL.getLastData().v2out;//v2out
    theVars->FluxTR[23] = theVars->SUCS_VEL.getLastData().v51;//v51	;//	DHAP+GAP --FBP
    theVars->FluxTR[24] = theVars->SUCS_VEL.getLastData().v52;//v52	;//	FBP --F6P + Pi
    theVars->FluxTR[25] = theVars->SUCS_VEL.getLastData().v55;//v55	;//	G1P+UTP --OPOP+UDPG
    theVars->FluxTR[26] = theVars->SUCS_VEL.getLastData().v56;//v56	;//	UDPG+F6P--SUCP + UDP
    theVars->FluxTR[27] = theVars->SUCS_VEL.getLastData().v57;//v57	;//	SUCP--Pi + SUC
    theVars->FluxTR[28] = theVars->SUCS_VEL.getLastData().v58;//v58	;//	F26BP--F6P + Pi
    theVars->FluxTR[29] = theVars->SUCS_VEL.getLastData().v59;//v59	;//	F6P + ATP --ADP + F26BP
    theVars->FluxTR[30] = theVars->SUCS_VEL.getLastData().v60;//v60	;//	ATP+UDP --UTP + ADP
    theVars->FluxTR[31] = theVars->SUCS_VEL.getLastData().v62;//v62	;//	SUC SINK
    theVars->FluxTR[32] = theVars->SUCS_VEL.getLastData().vdhap_in;//vdhap_in	;//	DHAP export from chloroplast
    theVars->FluxTR[33] = theVars->SUCS_VEL.getLastData().vgap_in;//vgap_in	;//	GAP Export from chloroplast
    theVars->FluxTR[34] = theVars->SUCS_VEL.getLastData().vpga_in;//vpga_in	;//	PGA export from chloroplast
    theVars->FluxTR[35] = theVars->SUCS_VEL.getLastData().vpga_use;//vpga_use	;//	PGA utilisation in cytosol

    theVars->FluxTR[36] = theVars->BF_VEL.getLastData().Vbf1;
    theVars->FluxTR[37] = theVars->BF_VEL.getLastData().Vbf2;
    theVars->FluxTR[38] = theVars->BF_VEL.getLastData().Vbf3;
    theVars->FluxTR[39] = theVars->BF_VEL.getLastData().Vbf4;
    theVars->FluxTR[40] = theVars->BF_VEL.getLastData().Vbf5;
    theVars->FluxTR[41] = theVars->BF_VEL.getLastData().Vbf6;
    theVars->FluxTR[42] = theVars->BF_VEL.getLastData().Vbf7;
    theVars->FluxTR[43] = theVars->BF_VEL.getLastData().Vbf8;
    theVars->FluxTR[44] = theVars->BF_VEL.getLastData().Vbf9;
    theVars->FluxTR[45] = theVars->BF_VEL.getLastData().Vbf10;
    theVars->FluxTR[46] = theVars->BF_VEL.getLastData().Vbf11;
    theVars->FluxTR[47] = theVars->BF_VEL.getLastData().Vqi;
    theVars->FluxTR[48] = theVars->BF_VEL.getLastData().Vipc;
    theVars->FluxTR[49] = theVars->BF_VEL.getLastData().Vicp;
    theVars->FluxTR[50] = theVars->BF_VEL.getLastData().Vinc;
    theVars->FluxTR[51] = theVars->BF_VEL.getLastData().Vinp;
    theVars->FluxTR[52] = theVars->BF_VEL.getLastData().Vdp;
    theVars->FluxTR[53] = theVars->BF_VEL.getLastData().Vdc;
    theVars->FluxTR[54] = theVars->BF_VEL.getLastData().Vfc;
    theVars->FluxTR[55] = theVars->BF_VEL.getLastData().Vsfd;
    theVars->FluxTR[56] = theVars->BF_VEL.getLastData().VsATP;
    theVars->FluxTR[57] = theVars->BF_VEL.getLastData().VgPQH2;
    theVars->FluxTR[58] = theVars->BF_VEL.getLastData().JKc;
    theVars->FluxTR[59] = theVars->BF_VEL.getLastData().JMgc;
    theVars->FluxTR[60] = theVars->BF_VEL.getLastData().JClc;
    theVars->FluxTR[61] = theVars->BF_VEL.getLastData().Vbf15;
    theVars->FluxTR[62] = theVars->BF_VEL.getLastData().Vbf16;
    theVars->FluxTR[63] = theVars->BF_VEL.getLastData().vbfn2;
    theVars->FluxTR[64] = theVars->BF_VEL.getLastData().VsNADPH;
    theVars->FluxTR[65] = theVars->BF_VEL.getLastData().vcet;

    theVars->FluxTR[66] = theVars->FI_VEL.getLastData().vA_d;
    theVars->FluxTR[67] = theVars->FI_VEL.getLastData().vA_f;
    theVars->FluxTR[68] = theVars->FI_VEL.getLastData().vA_U;
    theVars->FluxTR[69] = theVars->FI_VEL.getLastData().vU_A;
    theVars->FluxTR[70] = theVars->FI_VEL.getLastData().vU_f;
    theVars->FluxTR[71] = theVars->FI_VEL.getLastData().vU_d;
    theVars->FluxTR[72] = theVars->FI_VEL.getLastData().v1;
    theVars->FluxTR[73] = theVars->FI_VEL.getLastData().v_r1;
    theVars->FluxTR[74] = theVars->FI_VEL.getLastData().vS1_S2;
    theVars->FluxTR[75] = theVars->FI_VEL.getLastData().vS2_S3;
    theVars->FluxTR[76] = theVars->FI_VEL.getLastData().vS3_S0;
    theVars->FluxTR[77] = theVars->FI_VEL.getLastData().vS0_S1;
    theVars->FluxTR[78] = theVars->FI_VEL.getLastData().vz_1;
    theVars->FluxTR[79] = theVars->FI_VEL.getLastData().v1z_1;
    theVars->FluxTR[80] = theVars->FI_VEL.getLastData().v2z_1;
    theVars->FluxTR[81] = theVars->FI_VEL.getLastData().v3z_1;
    theVars->FluxTR[82] = theVars->FI_VEL.getLastData().v0z_1;
    theVars->FluxTR[83] = theVars->FI_VEL.getLastData().vz_2;
    theVars->FluxTR[84] = theVars->FI_VEL.getLastData().v1z_2;
    theVars->FluxTR[85] = theVars->FI_VEL.getLastData().v2z_2;
    theVars->FluxTR[86] = theVars->FI_VEL.getLastData().v3z_2;
    theVars->FluxTR[87] = theVars->FI_VEL.getLastData().v0z_2;
    theVars->FluxTR[88] = theVars->FI_VEL.getLastData().v1z;
    theVars->FluxTR[89] = theVars->FI_VEL.getLastData().v2z;
    theVars->FluxTR[90] = theVars->FI_VEL.getLastData().v3z;
    theVars->FluxTR[91] = theVars->FI_VEL.getLastData().v0z;
    theVars->FluxTR[92] = theVars->FI_VEL.getLastData().vAB1;
    theVars->FluxTR[93] = theVars->FI_VEL.getLastData().vBA1;
    theVars->FluxTR[94] = theVars->FI_VEL.getLastData().vAB2;
    theVars->FluxTR[95] = theVars->FI_VEL.getLastData().vBA2;
    theVars->FluxTR[96] = theVars->FI_VEL.getLastData().v3;
    theVars->FluxTR[97] = theVars->FI_VEL.getLastData().v_r3;
    theVars->FluxTR[98] = theVars->FI_VEL.getLastData().v3_n;
    theVars->FluxTR[99] = theVars->FI_VEL.getLastData().v_r3_n;
    theVars->FluxTR[100] = theVars->FI_VEL.getLastData().v_pq_ox;
    theVars->FluxTR[101] = theVars->FI_VEL.getLastData().Ic;
    theVars->FluxTR[102] = theVars->FI_VEL.getLastData().Ia;
    theVars->FluxTR[103] = theVars->FI_VEL.getLastData().v2_1;
    theVars->FluxTR[104] = theVars->FI_VEL.getLastData().v2_2;
    theVars->FluxTR[105] = theVars->FI_VEL.getLastData().v2_00_1;
    theVars->FluxTR[106] = theVars->FI_VEL.getLastData().v2_01_1;
    theVars->FluxTR[107] = theVars->FI_VEL.getLastData().v2_02_1;
    theVars->FluxTR[108] = theVars->FI_VEL.getLastData().v2_00_2;
    theVars->FluxTR[109] = theVars->FI_VEL.getLastData().v2_01_2;
    theVars->FluxTR[110] = theVars->FI_VEL.getLastData().v2_02_2;
    theVars->FluxTR[111] = theVars->FI_VEL.getLastData().vr2_00_1;
    theVars->FluxTR[112] = theVars->FI_VEL.getLastData().vr2_01_1;
    theVars->FluxTR[113] = theVars->FI_VEL.getLastData().vr2_02_1;
    theVars->FluxTR[114] = theVars->FI_VEL.getLastData().vr2_1;
    theVars->FluxTR[115] = theVars->FI_VEL.getLastData().vr2_00_2;
    theVars->FluxTR[116] = theVars->FI_VEL.getLastData().vr2_01_2;
    theVars->FluxTR[117] = theVars->FI_VEL.getLastData().vr2_02_2;
    theVars->FluxTR[118] = theVars->FI_VEL.getLastData().vr2_2;
    theVars->FluxTR[119] = theVars->FI_VEL.getLastData().vP680qU;
    theVars->FluxTR[120] = theVars->FI_VEL.getLastData().vP680qA;
    theVars->FluxTR[121] = theVars->FI_VEL.getLastData().vU_P680;
    theVars->FluxTR[122] = theVars->FI_VEL.getLastData().vP680_d;
    theVars->FluxTR[123] = theVars->FI_VEL.getLastData().vP680_f;

    theVars->FluxTR[124] = theVars->XanCycle_VEL.getLastData().Vva;
    theVars->FluxTR[125] = theVars->XanCycle_VEL.getLastData().Vaz;
    theVars->FluxTR[126] = theVars->XanCycle_VEL.getLastData().Vza;
    theVars->FluxTR[127] = theVars->XanCycle_VEL.getLastData().Vav;
    theVars->FluxTR[128] = theVars->XanCycle_VEL.getLastData().Vvf;
    theVars->FluxTR[129] = theVars->XanCycle_VEL.getLastData().Vv2ABA;
    theVars->FluxTR[130] = theVars->XanCycle_VEL.getLastData().VABAdg;

    theVars->FluxTR[131] = theVars->RROEA_VEL.getLastData().ve2GAPDH;
    theVars->FluxTR[132] = theVars->RROEA_VEL.getLastData().ve2FBPase;
    theVars->FluxTR[133] = theVars->RROEA_VEL.getLastData().ve2SBPase;
    theVars->FluxTR[134] = theVars->RROEA_VEL.getLastData().ve2PRK;
    theVars->FluxTR[135] = theVars->RROEA_VEL.getLastData().ve2ATPase;
    theVars->FluxTR[136] = theVars->RROEA_VEL.getLastData().ve2ATPGPP;
    theVars->FluxTR[137] = theVars->RROEA_VEL.getLastData().ve2MDH;
    theVars->FluxTR[138] = theVars->RROEA_VEL.getLastData().ve2Fd;
    theVars->FluxTR[139] = theVars->RROEA_VEL.getLastData().veFd2Thio;
    theVars->FluxTR[140] = theVars->RROEA_VEL.getLastData().veFd2Calvin;
    theVars->FluxTR[141] = theVars->RROEA_VEL.getLastData().ve2RuACT;
    for (size_t x = 0; x < 36; x++)
        theVars->FluxTR[x] = theVars->FluxTR[x] * theVars->AVR;
    theVars->FluxTR[46] = theVars->FluxTR[46] * theVars->AVR;
    theVars->FluxTR[64] = theVars->FluxTR[64] * theVars->AVR / 2;

}
