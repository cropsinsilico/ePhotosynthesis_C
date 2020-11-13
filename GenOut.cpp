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

// This is a function to generate output from the program.

void GenOut(double t, varptr *myVars) {
    
    //global PS2OUT;
    //global PR2OUT;
    //global BF2OUT;
    //global SUCS2OUT;
    
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
    
    //global O2_cond;
    O2 = myVars->O2_cond;
    
    //global FIBF_PSPR_com
    if (myVars->FIBF_PSPR_com) {
        Fdn = myVars->BF2OUT[0];
        PHs = myVars->BF2OUT[1];
        PHl = myVars->BF2OUT[2];
        NADPH = myVars->BF2OUT[3];
        ATP = myVars->BF2OUT[4];
    }
    
    
    //global EPS_SUCS_com;
    
    if (!myVars->EPS_SUCS_com)
        myVars->SUCS2OUT = zeros(12);
    
    
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
    
    //global XanCycle_BF_com;
    //global XanCycle2OUT;
    
    if (!myVars->XanCycle_BF_com)
        myVars->XanCycle2OUT = zeros(5);
    
    
    const double V = myVars->XanCycle2OUT[0];
    const double A = myVars->XanCycle2OUT[1];
    const double Z = myVars->XanCycle2OUT[2];
    const double ABA = myVars->XanCycle2OUT[3];
    
    //global StomCond_TrDynaPS_com;
    //global StomCon2OUT;
    
    if (!myVars->StomCond_TrDynaPS_com)
         myVars->StomCon2OUT = zeros(3);
    
    const double TurgorPressure = myVars->StomCon2OUT[0];
    const double Gs = myVars->StomCon2OUT[1];
    const double Posm = myVars->StomCon2OUT[2];
    
    //global CO2A;
    //global GLight;
    //global FIBF_PSPR_com;
    
    //global PS_TIME_N
    
    if (myVars->CO2A.shape()[0] < myVars->PS_TIME_N) {
        myVars->CO2A.resize(boost::extents[myVars->PS_TIME_N][CO2A_SIZE]);
        myVars->    CO2A.resize(boost::extents[myVars->PS_TIME_N][    CO2A_SIZE]);
    }

    myVars->CO2A[myVars->PS_TIME_N - 1][0] = CO2 * 3 * pow(10, 4);
    myVars->CO2A[myVars->PS_TIME_N - 1][1] = O2 / 1.26;
    
    //global V123;
    //global V16;
    
    if (myVars->FIBF_PSPR_com) {
        myVars->CO2A[myVars->PS_TIME_N - 1][2] = myVars->GLight;
    } else {
        myVars->CO2A[myVars->PS_TIME_N - 1][2] = myVars->V16;
    }
    
    //global CytoPi;
    
    myVars->CO2A[myVars->PS_TIME_N - 1][3] = ABA;
    myVars->CO2A[myVars->PS_TIME_N - 1][4] = ATP;
    myVars->CO2A[myVars->PS_TIME_N - 1][5] = Gs;
    
    
    //global AVR;
    
    myVars->CO2A[myVars->PS_TIME_N - 1][6] = (v1 - v131) * myVars->AVR;//
    //global RuACT2RA_v61;
    //global RuACT_EPS_com;
    
    if (myVars->RuACT_EPS_com)
        myVars->CO2A[myVars->PS_TIME_N - 1][6] = (myVars->RuACT2RA_v61 - v131) * myVars->AVR;//
    
    
    
    myVars->CO2A[myVars->PS_TIME_N - 1][7] = PGA;//
    myVars->CO2A[myVars->PS_TIME_N - 1][8] = DPGA;//
    myVars->CO2A[myVars->PS_TIME_N - 1][9] = T3P;//
    myVars->CO2A[myVars->PS_TIME_N - 1][10] = FBP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][11] = E4P;//
    myVars->CO2A[myVars->PS_TIME_N - 1][12] = S7P;//
    myVars->CO2A[myVars->PS_TIME_N - 1][13] = SBP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][14] = ATP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][15] = NADPH;//
    myVars->CO2A[myVars->PS_TIME_N - 1][16] = HexP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][17] = PenP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][18] = Pi;//
    myVars->CO2A[myVars->PS_TIME_N - 1][19] = ADP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][20] = RuBP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][21] = Gcea;//
    myVars->CO2A[myVars->PS_TIME_N - 1][22] = Gca;//
    myVars->CO2A[myVars->PS_TIME_N - 1][23] = Pga;//
    myVars->CO2A[myVars->PS_TIME_N - 1][24] = Pgca;//
    myVars->CO2A[myVars->PS_TIME_N - 1][25] = Gcac;//
    myVars->CO2A[myVars->PS_TIME_N - 1][26] = Goac;//
    myVars->CO2A[myVars->PS_TIME_N - 1][27] = Serc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][28] = Glyc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][29] = Hprc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][30] = Gceac;//
    myVars->CO2A[myVars->PS_TIME_N - 1][31] = Rubp;//
    myVars->CO2A[myVars->PS_TIME_N - 1][32] = Fdn;//      Fdn
    myVars->CO2A[myVars->PS_TIME_N - 1][33] = PHs;//      PhS
    myVars->CO2A[myVars->PS_TIME_N - 1][34] = PHl;//      PHl
    myVars->CO2A[myVars->PS_TIME_N - 1][35] = T3Pc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][36] = FBPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][37] = HexPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][38] = F26BPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][39] = ATPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][40] = ADPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][41] = OPOPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][42] = UDPGc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][43] = UTPc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][44] = SUCP;//
    myVars->CO2A[myVars->PS_TIME_N - 1][45] = SUC;   //
    myVars->CO2A[myVars->PS_TIME_N - 1][46] = PGAc;//
    myVars->CO2A[myVars->PS_TIME_N - 1][47] = V;//
    myVars->CO2A[myVars->PS_TIME_N - 1][48] = A;//
    myVars->CO2A[myVars->PS_TIME_N - 1][49] = Z;   //
    myVars->CO2A[myVars->PS_TIME_N - 1][50] = ABA;//
    myVars->CO2A[myVars->PS_TIME_N - 1][51] = TurgorPressure;
    myVars->CO2A[myVars->PS_TIME_N - 1][52] = Gs;
    myVars->CO2A[myVars->PS_TIME_N - 1][53] = Posm;
    
    
    myVars->CO2A[myVars->PS_TIME_N - 1][99] = t;
    //const double OutSuc = 1;
    //return OutSuc;
}
