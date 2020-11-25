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

struct Variables;
class SUCSCon {
public:
    SUCSCon() {}
    SUCSCon(const SUCSCon &other) {
        T3Pc = other.T3Pc;
        FBPc = other.FBPc;
        HexPc = other.HexPc;
        F26BPc = other.F26BPc;
        ATPc = other.ATPc;
        ADPc = other.ADPc;
        OPOPc = other.OPOPc;
        UDPGc = other.UDPGc;
        UTPc = other.UTPc;
        SUCP = other.SUCP;
        SUC = other.SUC;
        PGAc = other.PGAc;
    }
    SUCSCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const arr &vec, const size_t offset = 0) {
        T3Pc = vec[offset];
        FBPc = vec[offset + 1];
        HexPc = vec[offset + 2];
        F26BPc = vec[offset + 3];
        ATPc = vec[offset + 4];
        ADPc = vec[offset + 5];
        OPOPc = vec[offset + 6];
        UDPGc = vec[offset + 7];
        UTPc = vec[offset + 8];
        SUCP = vec[offset + 9];
        SUC = vec[offset + 10];
        PGAc = vec[offset + 11];

    }

    arr toArray() {
        arr vec = {T3Pc, FBPc, HexPc, F26BPc, ATPc, ADPc, OPOPc, UDPGc, UTPc, SUCP, SUC, PGAc};
        return vec;
    }
    size_t size() {
        return count;
    }
    double T3Pc = 0.;
    double FBPc = 0.;
    double HexPc = 0.;
    double F26BPc = 0.;
    double ATPc = 0.;
    double ADPc = 0.;
    double OPOPc = 0.;
    double UDPGc = 0.;
    double UTPc = 0.;
    double SUCP = 0.;
    double SUC = 0.;
    double PGAc = 0.;
private:
    size_t count = 12;
};

class SUCSVel {
public:
    SUCSVel() {}
    SUCSVel(const SUCSVel &other) {
        v51 = other.v51;
        v52 = other.v52;
        v55 = other.v55;
        v56 = other.v56;
        v57 = other.v57;
        v58 = other.v58;
        v59 = other.v59;
        v60 = other.v60;
        v61 = other.v61;
        v62 = other.v62;
        vdhap_in = other.vdhap_in;
        vgap_in = other.vgap_in;
        vpga_in = other.vpga_in;
        vpga_use = other.vpga_use;
        vatpf = other.vatpf;
    }
    double v51 = 0.;  //   DHAP+GAP --FBP
    double v52 = 0.;  //   FBP --F6P + Pi
    double v55 = 0.;  //   G1P+UTP --OPOP+UDPG
    double v56 = 0.;  //   UDPG+F6P--SUCP + UDP
    double v57 = 0.;  //   SUCP--Pi + SUC
    double v58 = 0.;  //   F26BP--F6P + Pi
    double v59 = 0.;  //   F6P + ATP --ADP + F26BP
    double v60 = 0.;  // ATP+UDP --UTP + ADP
    double v61 = 0.;  //  POPO --2PO
    double v62 = 0.;  // SUC SINK
    double vdhap_in = 0.;  //  DHAP IN
    double vgap_in = 0.;  //    GAP Export from chloroplast
    double vpga_in = 0.;  //   PGA export from chloroplast
    double vpga_use = 0.;  //  PGA utilisation in chloroplast
    double vatpf = 0.;  //   ATP synthesis rate

};
class SUCSPool {
public:
    SUCSPool() {}
    SUCSPool(const SUCSPool &other) {

        ATc = other.ATc;
        UTc = other.UTc;
        PTc = other.PTc;
}

    double ATc = 0;
    double UTc = 0;
    double PTc = 0;
};

SUCSCon SUCS_Ini(Variables *myVars);

arr SUCS_Mb(double t, SUCSCon &SUCS_Con, Variables *myVars);

void SUCS_Rate(double t, SUCSCon &SUCS_Con, Variables *myVars);
