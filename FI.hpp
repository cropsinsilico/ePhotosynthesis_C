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

class FI {
public:
    FI() {}
    FI(const FI &other) {
        vA_d = other.vA_d;
        vA_f = other.vA_f;
        vA_U = other.vA_U;
        vU_A = other.vU_A;
        vU_f = other.vU_f;
        vU_d = other.vU_d;
        v1 = other.v1;
        v_r1 = other.v_r1;
        vS1_S2 = other.vS1_S2;
        vS2_S3 = other.vS2_S3;
        vS3_S0 = other.vS3_S0;
        vS0_S1 = other.vS0_S1;
        vz_1 = other.vz_1;
        v1z_1 = other.v1z_1;
        v2z_1 = other.v2z_1;
        v3z_1 = other.v3z_1;
        v0z_1 = other.v0z_1;
        vz_2 = other.vz_2;
        v1z_2 = other.v1z_2;
        v2z_2 = other.v2z_2;
        v3z_2 = other.v3z_2;
        v0z_2 = other.v0z_2;
        v1z = other.v1z;
        v2z = other.v2z;
        v3z = other.v3z;
        v0z = other.v0z;
        vAB1 = other.vAB1;
        vBA1 = other.vBA1;
        vAB2 = other.vAB2;
        vBA2 = other.vBA2;
        v3 = other.v3;
        v_r3 = other.v_r3;
        v3_n = other.v3_n;
        v_r3_n = other.v_r3_n;
        v_pq_ox = other.v_pq_ox;
        Ic = other.Ic;
        Ia = other.Ia;
        v2_1 = other.v2_1;
        v2_2 = other.v2_2;
        v2_00_1 = other.v2_00_1;
        v2_01_1 = other.v2_01_1;
        v2_02_1 = other.v2_02_1;
        v2_00_2 = other.v2_00_2;
        v2_01_2 = other.v2_01_2;
        v2_02_2 = other.v2_02_2;
        vr2_00_1 = other.vr2_00_1;
        vr2_01_1 = other.vr2_01_1;
        vr2_02_1 = other.vr2_02_1;
        vr2_1 = other.vr2_1;
        vr2_00_2 = other.vr2_00_2;
        vr2_01_2 = other.vr2_01_2;
        vr2_02_2 = other.vr2_02_2;
        vr2_2 = other.vr2_2;
        vP680qU = other.vP680qU;
        vP680qA = other.vP680qA;
        vU_P680 = other.vU_P680;
        vP680_d = other.vP680_d;
        vP680_f = other.vP680_f;
    }
    double vA_d = 0.;//	vA_d	The rate of heat dissipation from peripheral antenna
    double vA_f = 0.;	//	vA_f	The rate of fluorescence emission from peripheral antenna
    double vA_U = 0.;	//	vA_U	The rate of exciton transfer from peripheral antenna to core antenna in open reaction center
    double vU_A = 0.;	//	vU_A	The rate of exciton transfer from core antenna to perpheral antenna in open center
    double vU_f = 0.;	//	vU_f	The rate of fluorescence emission from core antenna
    double vU_d = 0.;	//	vU_d	The rate of heat dissipation from core antenna
    double v1 = 0.;	//	v1	The rate of primary charge separation
    double v_r1 = 0.;//	v_r1	The rate of charge recombination
    double vS1_S2 = 0.;	//	vS1_S2	The rate of transition from S1 to S2
    double vS2_S3 = 0.;	//	vS2_S3	The rate of transition from S2 to S3
    double vS3_S0 = 0.;	//	vS3_S0	The rate of transition from S3 to S0
    double vS0_S1 = 0.;	//	vS0_S1	The rate of transition from S0 to S1
    double vz_1 = 0.;	//	vz_1	The rate of P680p reduction
    double v1z_1 = 0.;	//	v1z_1	The rate of oxidation of S1T by P680pPheon
    double v2z_1 = 0.;	//	v2z_1	The rate of oxidation of S2T  by P680pPheon
    double v3z_1 = 0.;	//	v3z_1	The rate of oxidation of S3T  by P680pPheon
    double v0z_1 = 0.;	//	v0z_1	The rate of oxidation of S0T  by P680pPheon
    double vz_2 = 0.;	//	vz_2	The rate of P680pPheon reduction
    double v1z_2 = 0.;	//	v1z_2	The rate of oxidation of S1T by P680pPheo
    double v2z_2 = 0.;	//	v2z_2	The rate of oxidation of S2T  by P680pPheo
    double v3z_2 = 0.;	//	v3z_2	The rate of oxidation of S3T  by P680pPheo
    double v0z_2 = 0.;	//	v0z_2	The rate of oxidation of S0T  by P680pPheo
    double v1z = 0.;
    double v2z = 0.;
    double v3z = 0.;
    double v0z = 0.;
    double vAB1 = 0.;//	vAB1	The rate of electron transfer from QA- to QB
    double vBA1 = 0.;//	vBA1	The rate of electron transfer from QB- to QA
    double vAB2 = 0.;//	vAB2	The rate of electron transfer from QA- to QB-
    double vBA2 = 0.;//	vBA2	The rate of electron transfer from QB2- TO QA
    double v3 = 0.;//	v3	The rate of exchange of QAQBH2 with PQ
    double v_r3 = 0.;//	v_r3	The rate of exchange of QAQB with PQH2
    double v3_n = 0.;//	v3_n	The rate of exchange of QAnQBH2 with PQ
    double v_r3_n = 0.;//	v_r3_n	The rate of exchange of QAnQB with PQH2
    double v_pq_ox = 0.;//	v_pq_ox	The rate of PQH2 oxidation
    double Ic = 0.; //	Ic	The incident light on the core antenna
    double Ia = 0.;//	Ia	The incident light on the peripheral antenna
    double v2_1 = 0.;//	v2_1	The rate of P680pPheon oxidation
    double v2_2 = 0.;//	v2_1	The rate of P680pPheon oxidation
    double v2_00_1 = 0.;//	v2_00_1	The rate of reduction of QAQB by P680pPheon
    double v2_01_1 = 0.;//	v2_01_1	The rate of reduction of QAQBn by P680pPheon
    double v2_02_1 = 0.;//	v2_02_1	The rate of reduction of QAQB2n by P680pPheon
    double v2_00_2 = 0.;//	v2_00_2	The rate of reduction of QAQB by P680Pheon
    double v2_01_2 = 0.;//	v2_01_2	The rate of reduction of QAQBn by P680Pheon
    double v2_02_2 = 0.;//	v2_02_2	The rate of reduction of QAQB2n by P680Pheon
    double vr2_00_1 = 0.;//	vr2_00_1	The reverse reaction of The rate of reduction of QAQB by P680pPheon
    double vr2_01_1 = 0.;//	vr2_01_1	The reverse reaction of The rate of reduction of QAQBn by P680pPheon
    double vr2_02_1 = 0.;//	vr2_02_1	The reverse reaction of The rate of reduction of QAQB2n by P680pPheon
    double vr2_1 = 0.;
    double vr2_00_2 = 0.;//	vr2_00_2	The reverse reaction of The rate of reduction of QAQB by P680Pheon
    double vr2_01_2 = 0.;//	vr2_01_2	The reverse reaction of The rate of reduction of QAQBn by P680Pheon
    double vr2_02_2 = 0.;//	vr2_02_2	The reverse reaction of The rate of reduction of QAQB2n by P680Pheon
    double vr2_2 = 0.;
    double vP680qU = 0.;
    double vP680qA = 0.;
    double vU_P680 = 0.;
    double vP680_d = 0.;
    double vP680_f = 0.;
};

class FIRC {
public:
    FIRC() {}
    FIRC(const FIRC &other){
        kA_d = other.kA_d;//	The rate constant of heat dissipation from peripheral antenna	Lazar (1999), 0.25~1 *10^(9)
        kA_f = other.kA_f;//	The rate constant of fluorescence emission from peripheral antenna	Lazar 1999, with a lifetime of 5 ns at closed reaction center
        kA_U = other.kA_U;//	The rate constant of exciton transfer from periphral antenna to core antenna	Reference needed, a guess
        kU_A = other.kU_A;//	The rate constant of exciton transfer from core antenna to peripheral antenna	Reference needed, a guess
        kU_d = other.kU_d;//	The rate constant of  heat emission from core antenna
        kU_f = other.kU_f;//	The rate constant of fluorescence emission from core antenna
        k1 = other.k1;	//	The rate constant of primary charge separation for open reaction center
        k_r1 = other.k_r1;	//	The rate constant of charge recombination for open reactoin center
        kz = other.kz;//	The rate constant of the Tyrosine oxidation	Lazar (1999); 3.8~50 * 10^6
        k12 = other.k12;	//	The rate constant of the S1 to S2 transition	Lazar (1999); 0.667~33.3 * 10^3
        k23 = other.k23;	//	The rate constant of the S2 to S3 transition	Lazar (1999); 0.667~33.3 * 10^3
        k30 = other.k30;	//	The rate constant of the S3 to S0 transition	Lazar (1999); 0.667~33.3 * 10^3
        k01 = other.k01;	//	The rate constant of the S0 to S1 transition	Lazar (1999); 0.667~33.3 * 10^3
        k2 = other.k2;//	The rate constant of the QA reduction by Pheo-	Lazar (1999); 2~2.3 * 10^9
        kAB1 = other.kAB1;//	The rate constant of QAQB-->QAQB-	Lazar (1999); 2.5~5 * 10^3
        kBA1 = other.kBA1;//	The rate constant of the QAQB- -->QAQB	Lazar (1999)
        kAB2 = other.kAB2;	//	The rate constant of the QAQB- --> QAQB2-	Lazar (1999); 1.25~3.33 * 10^3
        kBA2 = other.kBA2;	//	The rate constant of the QAQB2- --> QAQB- 	Lazar (1999), or same as kAB2 depend on the equilibium constant
        k3 = other.k3;//	The rate constant of the exchange of PQ and QBH2	Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        k_r3 = other.k_r3;	//	The rate constant of the exchange of QB and PQH2	Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        k_pq_oxy = other.k_pq_oxy;//	The rate constant of the PQH2 oxidation	Lazar (1999),50~500
    }

    double kA_d = 0.;  //  The rate constant of heat dissipation from peripheral antenna   Lazar (1999), 0.25~1 *10^(9)
    double kA_f = 0.;  //  The rate constant of fluorescence emission from peripheral antenna  Lazar 1999, with a lifetime of 5 ns at closed reaction center
    double kA_U = 0.;  //  The rate constant of exciton transfer from periphral antenna to core antenna    Reference needed, a guess
    double kU_A = 0.;  //  The rate constant of exciton transfer from core antenna to peripheral antenna   Reference needed, a guess
    double kU_d = 0.;  //  The rate constant of  heat emission from core antenna
    double kU_f = 0.;  //  The rate constant of fluorescence emission from core antenna
    double k1 = 0.;  //  The rate constant of primary charge separation for open reaction center
    double k_r1 = 0.;  //  The rate constant of charge recombination for open reactoin center
    double kz = 0.;  //  The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
    double k12 = 0.;  //  The rate constant of the S1 to S2 transition    Lazar (1999); 0.667~33.3 * 10^3
    double k23 = 0.;  //  The rate constant of the S2 to S3 transition    Lazar (1999); 0.667~33.3 * 10^3
    double k30 = 0.;  //  The rate constant of the S3 to S0 transition    Lazar (1999); 0.667~33.3 * 10^3
    double k01 = 0.;  //  The rate constant of the S0 to S1 transition    Lazar (1999); 0.667~33.3 * 10^3
    double k2 = 0.;  //  The rate constant of the QA reduction by Pheo-  Lazar (1999); 2~2.3 * 10^9
    double kAB1 = 0.;  //  The rate constant of QAQB-->QAQB-   Lazar (1999); 2.5~5 * 10^3
    double kBA1 = 0.;  //  The rate constant of the QAQB- -->QAQB  Lazar (1999)
    double kAB2 = 0.;  //  The rate constant of the QAQB- --> QAQB2-   Lazar (1999); 1.25~3.33 * 10^3
    double kBA2 = 0.;  //  The rate constant of the QAQB2- --> QAQB-   Lazar (1999), or same as kAB2 depend on the equilibium constant
    double k3 = 0.;  //  The rate constant of the exchange of PQ and QBH2    Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
    double k_r3 = 0.;  //  The rate constant of the exchange of QB and PQH2    Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
    double k_pq_oxy = 0.;  //  The rate constant of the PQH2 oxidation Lazar (1999),50~500

};

class FIPool {
public:
    FIPool() {}
    FIPool(const FIPool &other) {
        QBt = other.QBt;
        PQT = other.PQT;
    }
    double QBt = 0;
    double PQT = 0;
};


class FICon {
public:
    FICon() {}
    FICon(const FICon &other){
        A = other.A;
        U = other.U;
        P680ePheo = other.P680ePheo;
        P680pPheon = other.P680pPheon;
        P680pPheo = other.P680pPheo;
        P680Pheon = other.P680Pheon;
        Yz = other.Yz;
        S1T = other.S1T;
        S2T = other.S2T;
        S3T = other.S3T;
        S0T = other.S0T;
        S1Tp = other.S1Tp;
        S2Tp = other.S2Tp;
        S3Tp = other.S3Tp;
        S0Tp = other.S0Tp;
        QAQB = other.QAQB;
        QAnQB = other.QAnQB;
        QAQBn = other.QAQBn;
        QAnQBn = other.QAnQBn;
        QAQB2n = other.QAQB2n;
        QAnQB2n = other.QAnQB2n;
        PQn = other.PQn;
    }

    FICon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const arr &vec, const size_t offset = 0) {
        A = vec[offset];
        U = vec[offset + 1];
        P680ePheo = vec[offset + 2];
        P680pPheon = vec[offset + 3];
        P680pPheo = vec[offset + 4];
        P680Pheon = vec[offset + 5];
        Yz = vec[offset + 6];
        S1T = vec[offset + 7];
        S2T = vec[offset + 8];
        S3T = vec[offset + 9];
        S0T = vec[offset + 10];
        S1Tp = vec[offset + 11];
        S2Tp = vec[offset + 12];
        S3Tp = vec[offset + 13];
        S0Tp = vec[offset + 14];
        QAQB = vec[offset + 15];
        QAnQB = vec[offset + 16];
        QAQBn = vec[offset + 17];
        QAnQBn = vec[offset + 18];
        QAQB2n = vec[offset + 19];
        QAnQB2n = vec[offset + 20];
        PQn = vec[offset + 21];
    }

    arr toArray() {
        arr vec = {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp, S2Tp, S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n, PQn};
        return vec;
    }
    size_t size() {
        return count;
    }
    double A = 0.;  //  The concentration of excitons in the peripheral antenna
    double U = 0.;  //  The concentration fo excitons in the core antenna
    double P680ePheo = 0.;  //QF add
    double P680pPheon = 0.;  //  The concentration for the P680+ Pheo-
    double P680pPheo = 0.;  //  The concentration of P680+ Pheo
    double P680Pheon = 0.;  //  The concentration of P680Pheo-
    double Yz = 0.;  //  The concentration of reduced tyrosine// --unused
    double S1T = 0.;  //  The concentration of S1 in combination with reduced tyrosine
    double S2T = 0.;  //  The concentration of S2 in combination with reduced tyrosine
    double S3T = 0.;  //  The concentration of S3 in combination with reduced tyrosine
    double S0T = 0.;  //  The concentration of S0 in combination with reduced tyrosine
    double S1Tp = 0.;  //  The concentration of S1 in combination with oxidized tyrosine
    double S2Tp = 0.;  //  The concentration of S2 in combination with oxidized tyrosine
    double S3Tp = 0.;  //  The concentration of S3 in combination with oxidized tyrosine
    double S0Tp = 0.;  //  The concentration of S0 in combination with oxidized tyrosine
    double QAQB = 0.;  //  The concentration of [QAQB]
    double QAnQB = 0.;  //  The concentration of [QA-QB]
    double QAQBn = 0.;  //  The concentration of [QAQB-]
    double QAnQBn = 0.;  //  The concentration of [QA-QB-]
    double QAQB2n = 0.;  //  The concentration of [QAQB2-]
    double QAnQB2n = 0.;  //  The concentration of [QA-QB2-]
    double PQn = 0.;  //  The concentration of reduced PQ, i.e. PQH2;
private:
    size_t count = 22;
};

FICon FI_Ini(Variables *myVars);
arr FI_Mb(double t, FICon &FI_Con, Variables *myVars);
void FI_Rate(double t, FICon &FI_Con, Variables *myVars);
