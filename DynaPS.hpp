#pragma once
#include "RA.hpp"
#include "XanCycle.hpp"
#include "driver.hpp"

class DynaPSCon {
public:
    DynaPSCon() {}
    DynaPSCon(const DynaPSCon &other) {
        RA_con = other.RA_con;
        XanCycle_con = other.XanCycle_con;
    }
    DynaPSCon(realtype *x) {
        fromArray(x);
    }
    DynaPSCon(const RACon &rother, const XanCycleCon &xother) {
        RA_con = rother;
        XanCycle_con = xother;
    }
    DynaPSCon(const arr &vec, size_t offset = 0){
            fromArray(vec, offset);
    }
    void fromArray(realtype *x) {
        arr vec(120);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    void fromArray(const arr &vec, size_t offset = 0) {
            RA_con.fromArray(vec, offset);
            XanCycle_con.fromArray(vec, offset + RA_con.size());
    }
    arr toArray() {
        arr rvec = RA_con.toArray();
        arr xvec = XanCycle_con.toArray();
        rvec.insert(rvec.end(), xvec.begin(), xvec.end());
        return rvec;
    }
    size_t size() {
        return RA_con.size() + XanCycle_con.size();
    }
    RACon RA_con;
    XanCycleCon XanCycle_con;
};

DynaPSCon DynaPS_Init(Variables *myVars);
arr DynaPSmb(double t, DynaPSCon &DynaPS_con, Variables *myVars);
class DynaPS {
public:
    DynaPS(Variables *myVars) { this->myVars = myVars; }
    static Variables *myVars;
    arr DynaPS_Drive(size_t ParaNum, double Ratio);

    static int DynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
    DynaPSCon DynaPS_Ini();
};
