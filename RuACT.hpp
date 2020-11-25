#pragma once
#include "definitions.hpp"

class RuACTVel {
public:
    RuACTVel() {}
    RuACTVel(const RuACTVel &other) {
        v1 = other.v1;
        vn1 = other.vn1;
        v7 = other.v7;
        vn7 = other.vn7;
        v6_1 = other.v6_1;
        v6_2 = other.v6_2;
    }
    double v1 = 0.;
    double vn1 = 0.;
    double v7 = 0.;
    double vn7 = 0.;
    double v6_1 = 0.;
    double v6_2 = 0.;
};

class RuACTRC {
public:
    RuACTRC() {}
    RuACTRC(const RuACTRC &other) {

        k1 = other.k1;
        kn1 = other.kn1;
        km1 = other.km1;
        Ke2 = other.Ke2;
        Ke3 = other.Ke3;
        k6 = other.k6;
        kc = other.kc;
        ko = other.ko;
        k7 = other.k7;
        kr = other.kr;
}

    double k1 = 0;
    double kn1 = 0;
    double km1 = 0;
    double Ke2 = 0;
    double Ke3 = 0;
    double k6 = 0;
    double kc = 0;
    double ko = 0;
    double k7 = 0;
    double kr = 0;
};
class RuACTPool {
public:
    RuACTPool() {}
    RuACTPool(const RuACTPool &other) {

        ET = other.ET;
        Rac = other.Rac;
        C = other.C;
        O = other.O;
        M = other.M;
}

    double ET = 0;
    double Rac = 0;
    double C = 0;
    double O = 0;
    double M = 0;
};

class RuACTCon {
public:
    RuACTCon() {}
    RuACTCon(const RuACTCon &other) {
        ER = other.ER;
        Eaf = other.Eaf;
        ECMR = other.ECMR;
        RuBP = other.RuBP;
    }
    RuACTCon(const arr vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr vec, size_t offset = 0){
        ER = vec[offset];
        Eaf = vec[offset + 1];
        ECMR = vec[offset + 2];
        RuBP = vec[offset + 3];
    }
    arr toArray() {
        arr array = {ER, Eaf, ECMR, RuBP};
        return array;
    }
    size_t size() {
        return count;
    }
    double ER = 0.;	//	The concentration of inactive ER
    double Eaf = 0.;	//	The total concentration of E, EC, AND ECM
    double ECMR = 0.;	//	The concentration of ECMR
    double RuBP = 0.;	//	The concentration of ECMR
private:
    size_t count = 4;
};

RuACTCon RuACT_Ini(Variables *myVars);

arr RuACT_Mb(double t, RuACTCon &RuACT_Con, Variables *myVars);

void RuACT_Rate(double t, RuACTCon &RuACT_Con, Variables *myVars);
