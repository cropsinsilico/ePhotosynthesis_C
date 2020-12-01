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
        myVars->SUCS2OUT.clear();

    if (!myVars->XanCycle_BF_com)
        myVars->XanCycle2OUT.clear();

    if (myVars->record) {
        double ATP = myVars->PS2OUT.ATP;
        double NADPH = myVars->PS2OUT.NADPH;
        double O2 = myVars->PS2OUT.O2;
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
        co2a[0] = myVars->PS2OUT.CO2 * 3 * pow(10, 4);
        co2a[1] = O2 / 1.26;
        if (myVars->FIBF_PSPR_com) {
            co2a[2] = myVars->GLight;
        } else {
            co2a[2] = myVars->V16;
        }
        co2a[3] = myVars->XanCycle2OUT.ABA;
        co2a[4] = ATP;
        co2a[5] = 0.; // Gs;
        co2a[6] = (myVars->PS2OUT._v1 - myVars->PR2OUT._v131) * myVars->AVR;//
        if (myVars->RuACT_EPS_com)
            co2a[6] = (myVars->RuACT_Vel.v6_1 - myVars->PR2OUT._v131) * myVars->AVR;//
        co2a[7] = myVars->PS2OUT.PGA;//
        co2a[8] = myVars->PS2OUT.DPGA;//
        co2a[9] = myVars->PS2OUT.T3P;//
        co2a[10] = myVars->PS2OUT.FBP;//
        co2a[11] = myVars->PS2OUT.E4P;//
        co2a[12] = myVars->PS2OUT.S7P;//
        co2a[13] = myVars->PS2OUT.SBP;//
        co2a[14] = ATP;//
        co2a[15] = NADPH;//
        co2a[16] = myVars->PS2OUT.HexP;//
        co2a[17] = myVars->PS2OUT.PenP;//
        co2a[18] = myVars->PS2OUT._Pi;//
        co2a[19] = myVars->PS2OUT._ADP;//
        co2a[20] = myVars->PS2OUT.RuBP;//
        co2a[21] = myVars->PR2OUT.GCEA;//
        co2a[22] = myVars->PR2OUT.GCA;//
        co2a[23] = myVars->PR2OUT.PGA;//
        co2a[24] = myVars->PR2OUT.PGCA;//
        co2a[25] = myVars->PR2OUT.GCAc;//
        co2a[26] = myVars->PR2OUT.GOAc;//
        co2a[27] = myVars->PR2OUT.SERc;//
        co2a[28] = myVars->PR2OUT.GLYc;//
        co2a[29] = myVars->PR2OUT.HPRc;//
        co2a[30] = myVars->PR2OUT.GCEAc;//
        co2a[31] = myVars->PR2OUT.RUBP;//
        co2a[32] = Fdn;//      Fdn
        co2a[33] = PHs;//      PhS
        co2a[34] = PHl;//      PHl
        co2a[35] = myVars->SUCS2OUT.T3Pc;//
        co2a[36] = myVars->SUCS2OUT.FBPc;//
        co2a[37] = myVars->SUCS2OUT.HexPc;//
        co2a[38] = myVars->SUCS2OUT.F26BPc;//
        co2a[39] = myVars->SUCS2OUT.ATPc;//
        co2a[40] = myVars->SUCS2OUT.ADPc;//
        co2a[41] = myVars->SUCS2OUT.OPOPc;//
        co2a[42] = myVars->SUCS2OUT.UDPGc;//
        co2a[43] = myVars->SUCS2OUT.UTPc;//
        co2a[44] = myVars->SUCS2OUT.SUCP;//
        co2a[45] = myVars->SUCS2OUT.SUC;   //
        co2a[46] = myVars->SUCS2OUT.PGAc;//
        co2a[47] = myVars->XanCycle2OUT.Vx;//
        co2a[48] = myVars->XanCycle2OUT.Ax;//
        co2a[49] = myVars->XanCycle2OUT.Zx;   //
        co2a[50] = myVars->XanCycle2OUT.ABA;//
        co2a[51] = 0.; // TurgorPressure;
        co2a[52] = 0.; // Gs;
        co2a[53] = 0.; // Posm;


        co2a[99] = t;
        myVars->CO2A.insert(myVars->PS_TIME_N - 1, t, co2a);
    }
}

void makeFluxTR(Variables *myVars) {
    myVars->FluxTR = zeros(120);
    myVars->FluxTR[0] = myVars->RuACT_VEL.getLastData().v6_1;//PS
    myVars->FluxTR[1] = myVars->RuACT_VEL.getLastData().v6_2;//PR
    myVars->FluxTR[2] = myVars->PS_VEL.getLastData().v2;// v2
    myVars->FluxTR[3] = myVars->PS_VEL.getLastData().v3;// v3
    myVars->FluxTR[4] = myVars->PS_VEL.getLastData().v5;// v5
    myVars->FluxTR[5] = myVars->PS_VEL.getLastData().v6;// v6
    myVars->FluxTR[6] = myVars->PS_VEL.getLastData().v7;// v7
    myVars->FluxTR[7] = myVars->PS_VEL.getLastData().v8;// v8
    myVars->FluxTR[8] = myVars->PS_VEL.getLastData().v9;// v9
    myVars->FluxTR[9] = myVars->PS_VEL.getLastData().v10;// v10
    myVars->FluxTR[10] = myVars->PS_VEL.getLastData().v13;// v13
    myVars->FluxTR[11] = myVars->PS_VEL.getLastData().v23;// v23
    myVars->FluxTR[12] = myVars->PS_VEL.getLastData().v24;// v24
    myVars->FluxTR[13] = myVars->PS_VEL.getLastData().v25;// v25
    myVars->FluxTR[14] = myVars->PR_VEL.getLastData().v112;//v112
    myVars->FluxTR[15] = myVars->PR_VEL.getLastData().v113;//v113
    myVars->FluxTR[16] = myVars->PR_VEL.getLastData().v121;//v121
    myVars->FluxTR[17] = myVars->PR_VEL.getLastData().v122;//v122
    myVars->FluxTR[18] = myVars->PR_VEL.getLastData().v123;//v123
    myVars->FluxTR[19] = myVars->PR_VEL.getLastData().v124;//v124
    myVars->FluxTR[20] = myVars->PR_VEL.getLastData().v131;//v131
    myVars->FluxTR[21] = myVars->PR_VEL.getLastData().v1in;//vlin
    myVars->FluxTR[22] = myVars->PR_VEL.getLastData().v2out;//v2out
    myVars->FluxTR[23] = myVars->SUCS_VEL.getLastData().v51;//v51	;//	DHAP+GAP --FBP
    myVars->FluxTR[24] = myVars->SUCS_VEL.getLastData().v52;//v52	;//	FBP --F6P + Pi
    myVars->FluxTR[25] = myVars->SUCS_VEL.getLastData().v55;//v55	;//	G1P+UTP --OPOP+UDPG
    myVars->FluxTR[26] = myVars->SUCS_VEL.getLastData().v56;//v56	;//	UDPG+F6P--SUCP + UDP
    myVars->FluxTR[27] = myVars->SUCS_VEL.getLastData().v57;//v57	;//	SUCP--Pi + SUC
    myVars->FluxTR[28] = myVars->SUCS_VEL.getLastData().v58;//v58	;//	F26BP--F6P + Pi
    myVars->FluxTR[29] = myVars->SUCS_VEL.getLastData().v59;//v59	;//	F6P + ATP --ADP + F26BP
    myVars->FluxTR[30] = myVars->SUCS_VEL.getLastData().v60;//v60	;//	ATP+UDP --UTP + ADP
    myVars->FluxTR[31] = myVars->SUCS_VEL.getLastData().v62;//v62	;//	SUC SINK
    myVars->FluxTR[32] = myVars->SUCS_VEL.getLastData().vdhap_in;//vdhap_in	;//	DHAP export from chloroplast
    myVars->FluxTR[33] = myVars->SUCS_VEL.getLastData().vgap_in;//vgap_in	;//	GAP Export from chloroplast
    myVars->FluxTR[34] = myVars->SUCS_VEL.getLastData().vpga_in;//vpga_in	;//	PGA export from chloroplast
    myVars->FluxTR[35] = myVars->SUCS_VEL.getLastData().vpga_use;//vpga_use	;//	PGA utilisation in cytosol

    myVars->FluxTR[36] = myVars->BF_VEL.getLastData().Vbf1;
    myVars->FluxTR[37] = myVars->BF_VEL.getLastData().Vbf2;
    myVars->FluxTR[38] = myVars->BF_VEL.getLastData().Vbf3;
    myVars->FluxTR[39] = myVars->BF_VEL.getLastData().Vbf4;
    myVars->FluxTR[40] = myVars->BF_VEL.getLastData().Vbf5;
    myVars->FluxTR[41] = myVars->BF_VEL.getLastData().Vbf6;
    myVars->FluxTR[42] = myVars->BF_VEL.getLastData().Vbf7;
    myVars->FluxTR[43] = myVars->BF_VEL.getLastData().Vbf8;
    myVars->FluxTR[44] = myVars->BF_VEL.getLastData().Vbf9;
    myVars->FluxTR[45] = myVars->BF_VEL.getLastData().Vbf10;
    myVars->FluxTR[46] = myVars->BF_VEL.getLastData().Vbf11;
    myVars->FluxTR[47] = myVars->BF_VEL.getLastData().Vqi;
    myVars->FluxTR[48] = myVars->BF_VEL.getLastData().Vipc;
    myVars->FluxTR[49] = myVars->BF_VEL.getLastData().Vicp;
    myVars->FluxTR[50] = myVars->BF_VEL.getLastData().Vinc;
    myVars->FluxTR[51] = myVars->BF_VEL.getLastData().Vinp;
    myVars->FluxTR[52] = myVars->BF_VEL.getLastData().Vdp;
    myVars->FluxTR[53] = myVars->BF_VEL.getLastData().Vdc;
    myVars->FluxTR[54] = myVars->BF_VEL.getLastData().Vfc;
    myVars->FluxTR[55] = myVars->BF_VEL.getLastData().Vsfd;
    myVars->FluxTR[56] = myVars->BF_VEL.getLastData().VsATP;
    myVars->FluxTR[57] = myVars->BF_VEL.getLastData().VgPQH2;
    myVars->FluxTR[58] = myVars->BF_VEL.getLastData().JKc;
    myVars->FluxTR[59] = myVars->BF_VEL.getLastData().JMgc;
    myVars->FluxTR[60] = myVars->BF_VEL.getLastData().JClc;
    myVars->FluxTR[61] = myVars->BF_VEL.getLastData().Vbf15;
    myVars->FluxTR[62] = myVars->BF_VEL.getLastData().Vbf16;
    myVars->FluxTR[63] = myVars->BF_VEL.getLastData().vbfn2;
    myVars->FluxTR[64] = myVars->BF_VEL.getLastData().VsNADPH;
    myVars->FluxTR[65] = myVars->BF_VEL.getLastData().vcet;

    myVars->FluxTR[66] = myVars->FI_VEL.getLastData().vA_d;
    myVars->FluxTR[67] = myVars->FI_VEL.getLastData().vA_f;
    myVars->FluxTR[68] = myVars->FI_VEL.getLastData().vA_U;
    myVars->FluxTR[69] = myVars->FI_VEL.getLastData().vU_A;
    myVars->FluxTR[70] = myVars->FI_VEL.getLastData().vU_f;
    myVars->FluxTR[71] = myVars->FI_VEL.getLastData().vU_d;
    myVars->FluxTR[72] = myVars->FI_VEL.getLastData().v1;
    myVars->FluxTR[73] = myVars->FI_VEL.getLastData().v_r1;
    myVars->FluxTR[74] = myVars->FI_VEL.getLastData().vS1_S2;
    myVars->FluxTR[75] = myVars->FI_VEL.getLastData().vS2_S3;
    myVars->FluxTR[76] = myVars->FI_VEL.getLastData().vS3_S0;
    myVars->FluxTR[77] = myVars->FI_VEL.getLastData().vS0_S1;
    myVars->FluxTR[78] = myVars->FI_VEL.getLastData().vz_1;
    myVars->FluxTR[79] = myVars->FI_VEL.getLastData().v1z_1;
    myVars->FluxTR[80] = myVars->FI_VEL.getLastData().v2z_1;
    myVars->FluxTR[81] = myVars->FI_VEL.getLastData().v3z_1;
    myVars->FluxTR[82] = myVars->FI_VEL.getLastData().v0z_1;
    myVars->FluxTR[83] = myVars->FI_VEL.getLastData().vz_2;
    myVars->FluxTR[84] = myVars->FI_VEL.getLastData().v1z_2;
    myVars->FluxTR[85] = myVars->FI_VEL.getLastData().v2z_2;
    myVars->FluxTR[86] = myVars->FI_VEL.getLastData().v3z_2;
    myVars->FluxTR[87] = myVars->FI_VEL.getLastData().v0z_2;
    myVars->FluxTR[88] = myVars->FI_VEL.getLastData().v1z;
    myVars->FluxTR[89] = myVars->FI_VEL.getLastData().v2z;
    myVars->FluxTR[90] = myVars->FI_VEL.getLastData().v3z;
    myVars->FluxTR[91] = myVars->FI_VEL.getLastData().v0z;
    myVars->FluxTR[92] = myVars->FI_VEL.getLastData().vAB1;
    myVars->FluxTR[93] = myVars->FI_VEL.getLastData().vBA1;
    myVars->FluxTR[94] = myVars->FI_VEL.getLastData().vAB2;
    myVars->FluxTR[95] = myVars->FI_VEL.getLastData().vBA2;
    myVars->FluxTR[96] = myVars->FI_VEL.getLastData().v3;
    myVars->FluxTR[97] = myVars->FI_VEL.getLastData().v_r3;
    myVars->FluxTR[98] = myVars->FI_VEL.getLastData().v3_n;
    myVars->FluxTR[99] = myVars->FI_VEL.getLastData().v_r3_n;
    myVars->FluxTR[100] = myVars->FI_VEL.getLastData().v_pq_ox;
    myVars->FluxTR[101] = myVars->FI_VEL.getLastData().Ic;
    myVars->FluxTR[102] = myVars->FI_VEL.getLastData().Ia;
    myVars->FluxTR[103] = myVars->FI_VEL.getLastData().v2_1;
    myVars->FluxTR[104] = myVars->FI_VEL.getLastData().v2_2;
    myVars->FluxTR[105] = myVars->FI_VEL.getLastData().v2_00_1;
    myVars->FluxTR[106] = myVars->FI_VEL.getLastData().v2_01_1;
    myVars->FluxTR[107] = myVars->FI_VEL.getLastData().v2_02_1;
    myVars->FluxTR[108] = myVars->FI_VEL.getLastData().v2_00_2;
    myVars->FluxTR[109] = myVars->FI_VEL.getLastData().v2_01_2;
    myVars->FluxTR[110] = myVars->FI_VEL.getLastData().v2_02_2;
    myVars->FluxTR[111] = myVars->FI_VEL.getLastData().vr2_00_1;
    myVars->FluxTR[112] = myVars->FI_VEL.getLastData().vr2_01_1;
    myVars->FluxTR[113] = myVars->FI_VEL.getLastData().vr2_02_1;
    myVars->FluxTR[114] = myVars->FI_VEL.getLastData().vr2_1;
    myVars->FluxTR[115] = myVars->FI_VEL.getLastData().vr2_00_2;
    myVars->FluxTR[116] = myVars->FI_VEL.getLastData().vr2_01_2;
    myVars->FluxTR[117] = myVars->FI_VEL.getLastData().vr2_02_2;
    myVars->FluxTR[118] = myVars->FI_VEL.getLastData().vr2_2;
    myVars->FluxTR[119] = myVars->FI_VEL.getLastData().vP680qU;
    myVars->FluxTR[120] = myVars->FI_VEL.getLastData().vP680qA;
    myVars->FluxTR[121] = myVars->FI_VEL.getLastData().vU_P680;
    myVars->FluxTR[122] = myVars->FI_VEL.getLastData().vP680_d;
    myVars->FluxTR[123] = myVars->FI_VEL.getLastData().vP680_f;

    myVars->FluxTR[124] = myVars->XanCycle_VEL.getLastData().Vva;
    myVars->FluxTR[125] = myVars->XanCycle_VEL.getLastData().Vaz;
    myVars->FluxTR[126] = myVars->XanCycle_VEL.getLastData().Vza;
    myVars->FluxTR[127] = myVars->XanCycle_VEL.getLastData().Vav;
    myVars->FluxTR[128] = myVars->XanCycle_VEL.getLastData().Vvf;
    myVars->FluxTR[129] = myVars->XanCycle_VEL.getLastData().Vv2ABA;
    myVars->FluxTR[130] = myVars->XanCycle_VEL.getLastData().VABAdg;

    myVars->FluxTR[131] = myVars->RROEA_VEL.getLastData().ve2GAPDH;
    myVars->FluxTR[132] = myVars->RROEA_VEL.getLastData().ve2FBPase;
    myVars->FluxTR[133] = myVars->RROEA_VEL.getLastData().ve2SBPase;
    myVars->FluxTR[134] = myVars->RROEA_VEL.getLastData().ve2PRK;
    myVars->FluxTR[135] = myVars->RROEA_VEL.getLastData().ve2ATPase;
    myVars->FluxTR[136] = myVars->RROEA_VEL.getLastData().ve2ATPGPP;
    myVars->FluxTR[137] = myVars->RROEA_VEL.getLastData().ve2MDH;
    myVars->FluxTR[138] = myVars->RROEA_VEL.getLastData().ve2Fd;
    myVars->FluxTR[139] = myVars->RROEA_VEL.getLastData().veFd2Thio;
    myVars->FluxTR[140] = myVars->RROEA_VEL.getLastData().veFd2Calvin;
    myVars->FluxTR[141] = myVars->RROEA_VEL.getLastData().ve2RuACT;
    for (size_t x = 0; x < 36; x++)
        myVars->FluxTR[x] = myVars->FluxTR[x] * myVars->AVR;
    myVars->FluxTR[46] = myVars->FluxTR[46] * myVars->AVR;
    myVars->FluxTR[64] = myVars->FluxTR[64] * myVars->AVR / 2;

}
