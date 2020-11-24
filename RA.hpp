#pragma once
#include "EPS.hpp"
#include "RuACT.hpp"

class RACon {
public:
    RACon() {}
    RACon(const RACon &other) {
        RuACT_con = other.RuACT_con;
        EPS_con = other.EPS_con;
    }
    RACon(const EPSCon &eother, const RuACTCon &rother) {
        RuACT_con = rother;
        EPS_con = eother;
    }
    RACon(const std::vector<double> &vec, size_t offset = 0){
        fromArray(vec, offset);
    }
    void fromArray(const std::vector<double> &vec, size_t offset = 0) {
        EPS_con.fromArray(vec, offset);
        RuACT_con.fromArray(vec, offset + EPS_con.size());
    }
    std::vector<double> toArray() {
        std::vector<double> evec = EPS_con.toArray();
        std::vector<double> rvec = RuACT_con.toArray();
        evec.reserve(size());
        evec.insert(evec.end(), rvec.begin(), rvec.end());
        return evec;
    }
    size_t size() {
        return EPS_con.size() + RuACT_con.size();
    }
    RuACTCon RuACT_con;
    EPSCon EPS_con;
};

RACon RA_Ini(Variables *myVars);

std::vector<double> RA_mb(double t, RACon &RA_Con, Variables *myVars);
