#pragma once
#include "PS_PR.hpp"
#include "SUCS.hpp"
#include "driver.hpp"

class CMCon {
public:
    CMCon() {}
    CMCon(const CMCon &other) {
        PS_PR_con = other.PS_PR_con;
        SUCS_con = other.SUCS_con;
    }
    CMCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    CMCon(realtype *x) {
        fromArray(x);
    }
    CMCon(const PS_PRCon &pother, const SUCSCon &sother) {
        PS_PR_con = pother;
        SUCS_con = sother;
    }
    void fromArray(const arr &vec, size_t offset = 0) {

        arr pvec(PS_PR_con.size());
        std::copy(vec.begin() + offset, vec.begin() + PS_PR_con.size() + offset, pvec.begin());
        pvec[23] = vec[35 + offset];

        PS_PR_con.fromArray(pvec);
        SUCS_con.fromArray(vec, offset + PS_PR_con.size() - 1);
    }
    void fromArray(realtype *x) {
        arr vec(36);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    arr toArray() {
        arr psprvec = PS_PR_con.toArray();
        arr svec = SUCS_con.toArray();
        psprvec.reserve(size());
        double temp = psprvec[23];
        psprvec.insert(psprvec.end() - 1, svec.begin(), svec.end());
        psprvec[35] = temp;

        return psprvec;
    }

    size_t size() {
        return PS_PR_con.size() + SUCS_con.size();
    }
    PS_PRCon PS_PR_con;
    SUCSCon SUCS_con;
};
struct Variables;
class CM {
public:
    CM(Variables *myVars) {
        this->myVars = myVars;
    }
    static Variables *myVars;
    double CM_Drive2(double pop, double currentPop);
    CMCon CM_Ini();
    static int CM_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
};

CMCon CMInit(Variables *myVars);
arr CM_Mb(double t, CMCon &CM_con, Variables *myVars);
