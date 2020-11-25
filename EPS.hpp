#pragma once
#include "FIBF.hpp"
#include "CM.hpp"

class EPSCon {
public:
    EPSCon() {}
    EPSCon(const EPSCon &other) {
        CM_con = other.CM_con;
        FIBF_con = other.FIBF_con;
    }
    EPSCon(const FIBFCon &fother, const CMCon &cother) {
        CM_con = cother;
        FIBF_con = fother;
    }
    EPSCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr &vec, const size_t offset = 0) {
        FIBF_con.fromArray(vec, offset);
        CM_con.fromArray(vec, offset + FIBF_con.size());
    }
    arr toArray() {
        arr fvec = FIBF_con.toArray();
        arr cvec = CM_con.toArray();
        fvec.reserve(size());
        fvec.insert(fvec.end(), cvec.begin(), cvec.end());
        return fvec;
    }
    size_t size() {
        return CM_con.size() + FIBF_con.size();
    }
    CMCon CM_con;
    FIBFCon FIBF_con;
};

EPSCon EPS_Ini(Variables *myVars);

arr EPS_mb(double t, EPSCon &EPS_Con, Variables *myVars);
