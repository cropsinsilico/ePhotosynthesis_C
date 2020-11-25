#pragma once
#include "definitions.hpp"

class XanCycleVel {
public:
    XanCycleVel() {}
    XanCycleVel(const XanCycleVel &other) {
        Vva = other.Vva;
        Vaz = other.Vaz;
        Vza = other.Vza;
        Vav = other.Vav;
        Vvf = other.Vvf;
        Vv2ABA = other.Vv2ABA;
        VABAdg = other.VABAdg;
    }
    double Vva = 0.;  //  The velocity of v to a conversion
    double Vaz = 0.;  //  The rate of A to z
    double Vza = 0.;  //  THe rate of z to a
    double Vav = 0.;  //  The rate of A to V
    double Vvf = 0.;  //  The rate of V formation
    double Vv2ABA = 0.;  //  The rate of conversion from v to ABA.
    double VABAdg = 0.;  //  The rate of ABA degradation
};

struct Variables;
class XanCycleCon {
public:
    XanCycleCon() {}
    XanCycleCon(const XanCycleCon &other) {
        Vx = other.Vx;
        Ax = other.Ax;
        Zx = other.Zx;
        ABA = other.ABA;
    }
    XanCycleCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr &vec, const size_t offset = 0) {
        Vx = vec[offset];
        Ax = vec[offset + 1];
        Zx = vec[offset + 2];
        ABA = vec[offset + 3];
    }
    arr toArray() {
        arr array = {Vx, Ax, Zx, ABA};
        return array;
    }
    size_t size() {
        return count;
    }
    double Vx = 0.;
    double Ax = 0.;
    double Zx = 0.;
    double ABA = 0.;
private:
    size_t count = 4;
};

XanCycleCon XanCycle_Ini(Variables *myVars);

arr XanCycle_Mb(double t, XanCycleCon &XanCycle_Con, Variables *myVars);

void XanCycle_Rate(double t, XanCycleCon &XanCycle_Con, Variables *myVars);
