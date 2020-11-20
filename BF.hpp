#pragma once

#include <vector>

const int BF_CON_SIZE = 2;
const int BF_VEL_SIZE = 31;

struct Variables;
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
/*
class BFRC {
public:
    BFRC(){}
    BFRC(const BFRC &other) {
        ISP1 = other.ISP1;
        ISP2 = other.ISP2;
        QH = other.QH;
        CytbL = other.CytbL;
        CytbH1 = other.CytbH1;
        CytbH2 = other.CytbH2;
        Q_Qi = other.Q_Qi;
        ISPH = other.ISPH;
        eTransc1 = other.eTransc1;
        eTransc2 = other.eTransc2;
        maxATP = other.maxATP;
        protUp = other.protUp;
        permK = other.permK;
        permMg = other.permMg;
        permCl = other.permCl;
        exTransp = other.exTransp;
        exTransa = other.exTransa;
        flouresc = other.flouresc;
        heatD = other.heatD;
        KE8 = other.KE8;
        KE9 = other.KE9;
        chargeSep = other.chargeSep;
        etrans = other.etrans;
        memcap = other.memcap;
        ltt = other.ltt;
        beqS = other.beqS;
        beqL = other.beqL;
        mmATP_ATP = other.mmATP_ATP;
        mmATP_ADP = other.mmATP_ADP;
        mmADP_Pi = other.mmADP_Pi;
        mmNADPH = other.mmNADPH;
        mNADPH = other.mNADPH;
        eConst = other.eConst;

    }
    double ISP1 = 0.;  //  The rate constant for formation of ISP.QH2 complex; unit:  per second
    double ISP2 = 0.;  //  The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
    double QH = 0.;  //  The rate constant for QH. + cytbL --> Q + cytbL- + H+   Unit: s-1
    double CytbL = 0.;  //  The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
    double CytbH1 = 0.;  //  The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
    double CytbH2 = 0.;  //  The rate constant  for CytbH- + Q- --> cytbH + Q2-  Unit: s-1
    double Q_Qi = 0.;  //  The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website    Unit: s-1
    double ISPH = 0.;  //  The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+    Unit: s-1
    double eTransc1 = 0.;  //  The rate constant for the electron transport from cytc1 to cytc2    Unit: s-1
    double eTransc2 = 0.;  //  The rate constant for the electron transport from cytc2 to P700 Unit: s-1
    double maxATP = 0.;  //  The maximum rate of ATP synthesis   Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
    double protUp = 0.;  //  The rate constant for uptake of two protons from the stroma to Q2-  s-1
    double permK = 0.;  //  The permeability constant for K Unit: cm s-1
    double permMg = 0.;  //  The permeability constant for Mg    Unit: cm s-1
    double permCl = 0.;  //  The permeability constant for Cl    Unit: cm s-1
    double exTransp = 0.;  //  The rate constant for exciton transfer from perpheral antenna to core antenna, see FI   Unit: s-1
    double exTransa = 0.;  //  The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI  Unit: s-1
    double flouresc = 0.;  //  The rate constant for fluorescence emission, see the note in FI Unit: s-1
    double heatD = 0.;  //  The rate constant for heat dissipation; see the note for FI Unit: s-1
    double KE8 = 0.;  //  ISPHr + cytc1 --> ISPHox + cytc1-   Unit: s-1
    double KE9 = 0.;  //  cytc1- + cytc2 --> cytc1 + cytc2-   Unit: s-1
    double chargeSep = 0.;  //  The rate constant for primary charge separation in PSI  Unit: s-1
    double etrans = 0.;  //  The rate constant for electron tranfer from electron acceptor of PSI to Fd  Unit: s-1
    double memcap = 0.;  //  The membrane capacity
    double ltt = 0.;  //  The ratio of lumen volume to thylakoid membrane area
    double beqS = 0.;  //  The buffer equilibrium constant in stroma
    double beqL = 0.;  //  The buffer equilibrium constant in lumen
    double mmATP_ATP = 0.;  //  The michaelis menton constant for ATP for ATP synthesis
    double mmATP_ADP = 0.;  //  The michaelis menton constant for ATP for ADP synthesis
    double mmADP_Pi = 0.;  //  The michaelis menton constant for ATP for PI synthesis
    double mmNADP = 0.;  //  The michaelis menten constant for NADP  Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1, From Fridlyand and Scheibe 1999
    double mmNADPH = 0.;  //  The michaelis menten constant for NADPH Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1, From Fridlyand and Scheibe 1999
    double mNADPH = 0.;  //  The maximum rate of NADPH formation Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1, Calcualted based on 6.4 mmol (mg chl)-1h-1; Unit: mmol/l/s;
    double eConst = 0.;  //  Equilibrium constant, From Fridlyand paper , 1999, BBA, 1413, 1, 31-42
};

class BFCON {
public:
    BFCON() {}
    BFCON(const BFCON &other){
        ISPH = other.ISPH;
        oCytc1 = other.oCytc1;
        ISP = other.ISP;
        cISP = other.cISP;
        semq = other.semq;
        oCytbl = other.oCytbl;
        bQ = other.bQ;
        Q = other.Q;
        oCytbH = other.oCytbH;
        Qi = other.Qi;
        Q2 = other.Q2;
        PQH2 = other.PQH2;
        oCytc2 = other.oCytc2;
        rP700 = other.rP700;
        ADP = other.ADP;
        P = other.P;
        ATP = other.ATP;
        K = other.K;
        Mg = other.Mg;
        Cl = other.Cl;
        paPhot = other.paPhot;
        caPhot = other.caPhot;
        eacc = other.eacc;
        ferrox = other.ferrox;
        PBSs = other.PBSs;
        PBSl = other.PBSl;
        PHs = other.PHs;
        PHl = other.PHl;
        NADPH = other.NADPH;

    }
    BFCON(const std::vector<double> &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const std::vector<double> &vec, const size_t offset = 0) {
        ISPH = vec[offset];
        oCytc1 = vec[offset + 1];
        ISP = vec[offset + 2];
        cISP = vec[offset + 3];
        semq = vec[offset + 4];
        oCytbl = vec[offset + 5];
        bQ = vec[offset + 6];
        Q = vec[offset + 7];
        oCytbH = vec[offset + 8];
        Qi = vec[offset + 9];
        Q2 = vec[offset + 10];
        PQH2 = vec[offset + 11];
        oCytc2 = vec[offset + 12];
        rP700 = vec[offset + 13];
        ADP = vec[offset + 14];
        P = vec[offset + 15];
        ATP = vec[offset + 16];
        K = vec[offset + 17];
        Mg = vec[offset + 18];
        Cl = vec[offset + 19];
        paPhot = vec[offset + 20];
        caPhot = vec[offset + 21];
        eacc = vec[offset + 22];
        ferrox = vec[offset + 23];
        PBSs = vec[offset + 24];
        PBSl = vec[offset + 25];
        PHs = vec[offset + 26];
        PHl = vec[offset + 27];
        NADPH = vec[offset + 28];
    }
    std::vector<double> toArray() {
        std::vector<double> vec = {ISPH, oCytc1, ISP, cISP, semq, oCytbl, bQ, Q, oCytbH, Qi, Q2, PQH2, oCytc2, rP700, ADP, P, ATP, K, Mg, Cl, paPhot, caPhot, eacc, ferrox, PBSs, PBSl, PHs, PHl, NADPH};
        return vec;
    }
    size_t size() {
        return count;
    }
    double ISPH = 0.;  //  The reduced ion sulfer protein (ISPH); unit: micromole per m2
    double oCytc1 = 0.;  //  The oxidized state of cytc1; unit: micromole per meter square
    double ISP = 0.;  //  The oxidized ion sulfer protein (ISP); unit: micromole per meter square
    double cISP = 0.;  //  The complex of oxidized ion sulfer protein and reduced quinone; unit: micromole per meter square
    double semq = 0.;  //  Semiquinone; micromole per meter square
    double oCytbl = 0.;  //  The oxidized cytbL; micromole per meter square
    double bQ = 0.;  //  The binding Quinone; micromole per meter square
    double Q = 0.;  //  Quinone; micromole per meter square
    double oCytbH = 0.;  //  The oxidized form of cytbH; micromole per meter square
    double Qi = 0.;  //  Q-; unit: micromole per meter square
    double Q2 = 0.;  //  The reduced quinone Q2-; micromole per meter square
    double PQH2 = 0.;  //  The reduced quinone PQH2; micromole per meter square
    double oCytc2 = 0.;  //  oxidized cytc2; micromole per meter square
    double rP700 = 0.;  //  The reduced state of P700, including both P700 and excited P700; micromole per meter square
    double ADP = 0.;  //  ADP in stroma, from the earlier photorespiration model; mmol l-1
    double P = 0.;  //  Phosphate in stroma, from the photorespiration model; mmol l-1
    double ATP = 0.;  //  ATP in stroma, from the photorespiration model; mmol l-1
    double K = 0.;  //  K ions in stroma, mM, from the literature; mmol l-1; 90 might be an default;
    double Mg = 0.;  //  Mg ions in stroma, mM, from the literature of the ion estimate
    double Cl = 0.;  //  Cl ions in stroma, mM, from the literature of the ion estimate
    double paPhot = 0.;  //  The number of photons in peripheral antenna; micromole per meter square
    double caPhot = 0.;  //  The number of photons in core antenna; micromole per meter square
    double eacc = 0.;  //  The reduced electron acceptor in PSI; micromole per meter square
    double ferrox = 0.;  //  The reduced ferrodoxin; micromole per meter square leaf area
    double PBSs = 0.;  //  The protonated buffer species  and free proton together in stroma; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number.; default 25
    double PBSl = 0.;  //  The protonated buffer species and free proton together in lumen; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number. ; default 5
    double PHs = 0.;  //  The PH value of the stroma
    double PHl = 0.;  //  The PH value of the lumen
    double NADPH = 0.;  //  The NADPH concentration in stroma, Unit: mmol l-1;
private:
    size_t count = 29;
};

void BF_Ini(Variables *myVars);

std::vector<double> BF_Mb(double t, Variables *myVars);

void BF_Rate(double t, Variables *myVars);
*/
