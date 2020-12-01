#pragma once

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

#include "definitions.hpp"

class BFVEL {
public:
    BFVEL() {}
    BFVEL(const BFVEL &other) {
        Vbf1 = other.Vbf1;
        Vbf2 = other.Vbf2;
        Vbf3 = other.Vbf3;
        Vbf4 = other.Vbf4;
        Vbf5 = other.Vbf5;
        Vbf6 = other.Vbf6;
        Vbf7 = other.Vbf7;
        Vbf8 = other.Vbf8;
        Vbf9 = other.Vbf9;
        Vbf10 = other.Vbf10;
        Vbf11 = other.Vbf11;
        Vqi = other.Vqi;
        Vipc = other.Vipc;
        Vicp = other.Vicp;
        Vinc = other.Vinc;
        Vinp = other.Vinp;
        Vdp = other.Vdp;
        Vdc = other.Vdc;
        Vfp = other.Vfp;
        Vfc = other.Vfc;
        Vsfd = other.Vsfd;
        VsATP = other.VsATP;
        VgPQH2 = other.VgPQH2;
        JKc = other.JKc;
        JMgc = other.JMgc;
        JClc = other.JClc;
        Vbf15 = other.Vbf15;
        Vbf16 = other.Vbf16;
        vbfn2 = other.vbfn2;
        VsNADPH = other.VsNADPH;
        vcet = other.vcet;
    }

    double Vbf1 = 0.;
    double Vbf2 = 0.;
    double Vbf3 = 0.;
    double Vbf4 = 0.;
    double Vbf5 = 0.;
    double Vbf6 = 0.;
    double Vbf7 = 0.;
    double Vbf8 = 0.;
    double Vbf9 = 0.;
    double Vbf10 = 0.;
    double Vbf11 = 0.;
    double Vqi = 0.;
    double Vipc = 0.;
    double Vicp = 0.;
    double Vinc = 0.;
    double Vinp = 0.;
    double Vdp = 0.;
    double Vdc = 0.;
    double Vfp = 0.;
    double Vfc = 0.;
    double Vsfd = 0.;
    double VsATP = 0.;
    double VgPQH2 = 0.;
    double JKc = 0.;
    double JMgc = 0.;
    double JClc = 0.;
    double Vbf15 = 0.;
    double Vbf16 = 0.;
    double vbfn2 = 0.;
    double VsNADPH = 0.;
    double vcet = 0.;

};

class BFRC {
public:
    BFRC(){}
    BFRC(const BFRC &other) {
        K1 = other.K1;
        K2 = other.K2;
        K3 = other.K3;
        K4 = other.K4;
        K5 = other.K5;
        K6 = other.K6;
        K7 = other.K7;
        K8 = other.K8;
        K9 = other.K9;
        K10 = other.K10;
        Vmax11 = other.Vmax11;
        Kqi = other.Kqi;
        PK = other.PK;
        PMg = other.PMg;
        PCl = other.PCl;
        Kau = other.Kau;
        Kua = other.Kua;
        Kf = other.Kf;
        Kd = other.Kd;
        KE8 = other.KE8;
        KE9 = other.KE9;
        K15 = other.K15;
        K16 = other.K16;
        MemCap = other.MemCap;
        RVA = other.RVA;
        KBs = other.KBs;
        KBl = other.KBl;
        KM1ATP = other.KM1ATP;
        KM1ADP = other.KM1ADP;
        KM1PI = other.KM1PI;
        KM2NADP = other.KM2NADP;
        KM2NADPH = other.KM2NADPH;
        V2M = other.V2M;
        KE2 = other.KE2;
    }
    double K1 = 0.;  //      The rate constant for formation of ISP.QH2 complex
    double K2 = 0.;  //      The rate constant for ISP.QH2-->QH(semi) + ISPH(red)
    double K3 = 0.;  //      The rate constant for QH. + cytbL --> Q + cytbL- + H+   Unit: s-1
    double K4 = 0.;  //      The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
    double K5 = 0.;  //      The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
    double K6 = 0.;  //      The rate constant  for CytbH- + Q- --> cytbH + Q2-      Unit: s-1
    double K7 = 0.;  //      The rate constant for Q binding to Qi site
    double K8 = 0.;  //      The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+        Unit: s-1
    double K9 = 0.;  //      The rate constant for the electron transport from cytc1 to cytc2        Unit: s-1
    double K10 = 0.;  //      The rate constant for the electron transport from cytc2 to P700 Unit: s-1
    double Vmax11 = 0.;  //      The maximum rate of ATP synthesis       Unit: mmol l-1 s-1
    double Kqi = 0.;  //      The rate constant for uptake of two protons from the stroma to Q2-      s-1
    double PK = 0.;  //      The permeability constant for K Unit: cm s-1
    double PMg = 0.;  //      The permeability constant for Mg        Unit: cm s-1
    double PCl = 0.;  //      The permeability constant for Cl        Unit: cm s-1
    double Kau = 0.;  //      The rate constant for exciton transfer from perpheral antenna to core antenna, see FI   Unit: s-1
    double Kua = 0.;  //      The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI  Unit: s-1
    double Kf = 0.;  //      The rate constant for fluorescence emission, see the note in FI Unit: s-1
    double Kd = 0.;  //      The rate constant for heat dissipation
    double KE8 = 0.;  //      ISPHr + cytc1 --> ISPHox + cytc1-       Unit: s-1
    double KE9 = 0.;  //      cytc1- + cytc2 --> cytc1 + cytc2-       Unit: s-1
    double K15 = 0.;  //      The rate constant for primary charge separation in PSI  Unit: s-1
    double K16 = 0.;  //      The rate constant for electron tranfer from electron acceptor of PSI to Fd      Unit: s-1
    double MemCap = 0.;  //      The membrane capacity
    double RVA = 0.;  //      The ratio of lumen volume to thylakoid membrane area
    double KBs = 0.;  //      The buffer equilibrium constant in stroma
    double KBl = 0.;  //      The buffer equilibrium constant in lumen
    double KM1ATP = 0.;  //      The michaelis menton constant for ATP for ATP synthesis
    double KM1ADP = 0.;  //      The michaelis menton constant for ATP for ADP synthesis
    double KM1PI = 0.;  //      The michaelis menton constant for ATP for PI synthesis
    double KM2NADP = 0.;  //      The michaelis menten constant for NADP  Unit: mmol l-1 s-1
    double KM2NADPH = 0.;  //      The michaelis menten constant for NADPH Unit: mmol l-1 s-1
    double V2M = 0.;  //      The maximum rate of NADPH formation     Unit: mmol l-1 s-1
    double KE2 = 0.;  //  Equilibrium constant
};

class BFPool {
public:
    BFPool() {}
    BFPool(const BFPool &other) {

        kA_d = other.kA_d;
        kA_f = other.kA_f;
        kA_U = other.kA_U;
        kU_A = other.kU_A;
        kU_d = other.kU_d;
        kU_f = other.kU_f;
        k1 = other.k1;
        k_r1 = other.k_r1;
        kz = other.kz;
        k12 = other.k12;
        k23 = other.k23;
        k30 = other.k30;
        k01 = other.k01;
        k2 = other.k2;
        kAB1 = other.kAB1;
        kBA1 = other.kBA1;
        kAB2 = other.kAB2;
        kBA2 = other.kBA2;
        k3 = other.k3;
        k_r3 = other.k_r3;
        k_pq_oxy = other.k_pq_oxy;
    }

    double kA_d = 0;
    double kA_f = 0;
    double kA_U = 0;
    double kU_A = 0;
    double kU_d = 0;
    double kU_f = 0;
    double k1 = 0;
    double k_r1 = 0;
    double kz = 0;
    double k12 = 0;
    double k23 = 0;
    double k30 = 0;
    double k01 = 0;
    double k2 = 0;
    double kAB1 = 0;
    double kBA1 = 0;
    double kAB2 = 0;
    double kBA2 = 0;
    double k3 = 0;
    double k_r3 = 0;
    double k_pq_oxy = 0;
};


class BFCon {
public:
    BFCon() {}
    BFCon(const BFCon &other){
        ISPHr = other.ISPHr;
        cytc1 = other.cytc1;
        ISPo = other.ISPo;
        ISPoQH2 = other.ISPoQH2;
        QHsemi = other.QHsemi;
        cytbL = other.cytbL;
        Qi = other.Qi;
        Q = other.Q;
        cytbH = other.cytbH;
        Qn = other.Qn;
        Qr = other.Qr;
        QH2 = other.QH2;
        cytc2 = other.cytc2;
        P700 = other.P700;
        ADP = other.ADP;
        Pi = other.Pi;
        ATP = other.ATP;
        Ks = other.Ks;
        Mgs = other.Mgs;
        Cls = other.Cls;
        Aip = other.Aip;
        U = other.U;
        An = other.An;
        Fdn = other.Fdn;
        BFHs = other.BFHs;
        BFHl = other.BFHl;
        PHs = other.PHs;
        PHl = other.PHl;
        NADPH = other.NADPH;

    }
    BFCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const arr &vec, const size_t offset = 0) {
        ISPHr = vec[offset];
        cytc1 = vec[offset + 1];
        ISPo = vec[offset + 2];
        ISPoQH2 = vec[offset + 3];
        QHsemi = vec[offset + 4];
        cytbL = vec[offset + 5];
        Qi = vec[offset + 6];
        Q = vec[offset + 7];
        cytbH = vec[offset + 8];
        Qn = vec[offset + 9];
        Qr = vec[offset + 10];
        QH2 = vec[offset + 11];
        cytc2 = vec[offset + 12];
        P700 = vec[offset + 13];
        ADP = vec[offset + 14];
        Pi = vec[offset + 15];
        ATP = vec[offset + 16];
        Ks = vec[offset + 17];
        Mgs = vec[offset + 18];
        Cls = vec[offset + 19];
        Aip = vec[offset + 20];
        U = vec[offset + 21];
        An = vec[offset + 22];
        Fdn = vec[offset + 23];
        BFHs = vec[offset + 24];
        BFHl = vec[offset + 25];
        PHs = vec[offset + 26];
        PHl = vec[offset + 27];
        NADPH = vec[offset + 28];
    }
    arr toArray() {
        arr vec = {ISPHr, cytc1, ISPo, ISPoQH2, QHsemi, cytbL, Qi, Q, cytbH, Qn, Qr, QH2, cytc2, P700, ADP, Pi, ATP, Ks, Mgs, Cls, Aip, U, An, Fdn, BFHs, BFHl, PHs, PHl, NADPH};
        return vec;
    }
    size_t size() {
        return count;
    }
    double ISPHr = 0.;  //  The reduced ion sulfer protein (ISPH); unit: micromole per m2
    double cytc1 = 0.;  //  The oxidized state of cytc1; unit: micromole per meter square
    double ISPo = 0.;  //  The oxidized ion sulfer protein (ISP); unit: micromole per meter square
    double ISPoQH2 = 0.;  //  The complex of oxidized ion sulfer protein and reduced quinone; unit: micromole per meter square
    double QHsemi = 0.;  //  Semiquinone; micromole per meter square
    double cytbL = 0.;  //  The oxidized cytbL; micromole per meter square
    double Qi = 0.;  //  The binding Quinone; micromole per meter square
    double Q = 0.;  //  Quinone; micromole per meter square
    double cytbH = 0.;  //  The oxidized form of cytbH; micromole per meter square
    double Qn = 0.;  //  Q-; unit: micromole per meter square
    double Qr = 0.;  //  The reduced quinone Q2-; micromole per meter square
    double QH2 = 0.;  //  The reduced quinone PQH2; micromole per meter square
    double cytc2 = 0.;  //  oxidized cytc2; micromole per meter square
    double P700 = 0.;  //  The reduced state of P700, including both P700 and excited P700; micromole per meter square
    double ADP = 0.;  //  ADP in stroma, from the earlier photorespiration model; mmol l-1
    double Pi = 0.;  //  Phosphate in stroma, from the photorespiration model; mmol l-1
    double ATP = 0.;  //  ATP in stroma, from the photorespiration model; mmol l-1
    double Ks = 0.;  //  K ions in stroma, mM, from the literature; mmol l-1; 90 might be an default;
    double Mgs = 0.;  //  Mg ions in stroma, mM, from the literature of the ion estimate
    double Cls = 0.;  //  Cl ions in stroma, mM, from the literature of the ion estimate
    double Aip = 0.;  //  The number of photons in peripheral antenna; micromole per meter square
    double U = 0.;  //  The number of photons in core antenna; micromole per meter square
    double An = 0.;  //  The reduced electron acceptor in PSI; micromole per meter square
    double Fdn = 0.;  //  The reduced ferrodoxin; micromole per meter square leaf area
    double BFHs = 0.;  //  The protonated buffer species  and free proton together in stroma; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number.; default 25
    double BFHl = 0.;  //  The protonated buffer species and free proton together in lumen; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number. ; default 5
    double PHs = 0.;  //  The PH value of the stroma
    double PHl = 0.;  //  The PH value of the lumen
    double NADPH = 0.;  //  The NADPH concentration in stroma, Unit: mmol l-1;
private:
    size_t count = 29;
};

class BF {
public:
    static BFCon BF_Ini(Variables *myVars);

    static arr BF_Mb(double t, BFCon &BF_con, Variables *myVars);

    static void BF_Rate(double t, BFCon &BF_con, Variables *myVars);
};
