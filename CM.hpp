#pragma once
#include "PS_PR.hpp"
#include "SUCS.hpp"

class CMCon {
public:
    CMCon() {}
    CMCon(const CMCon &other) {
        
    }
    CMCon(const std::vector<double> &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    CMCon(realtype *x) {
        fromArray(x);
    }
    CMCon(const PS_PRCon &pother, const SUCSCon &sother) {
        PS_PR_con = pother;
        SUCS_con = sother;
    }
    void fromArray(const std::vector<double> &vec, size_t offset = 0) {

        std::vector<double> pvec(PS_PR_con.size());
        std::copy(vec.begin() + offset, vec.begin() + PS_PR_con.size() + offset, pvec.begin());
        pvec[23] = vec[35 + offset];

        PS_PR_con.fromArray(pvec);
        SUCS_con.fromArray(vec, offset + PS_PR_con.size() - 1);
    }
    void fromArray(realtype *x) {
        std::vector<double> vec(36);
        for (int i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    std::vector<double> toArray() {
        std::vector<double> psprvec = PS_PR_con.toArray();
        std::vector<double> svec = SUCS_con.toArray();
        psprvec.reserve(size());
        double temp = psprvec[23];
        psprvec.insert(psprvec.end() - 1, svec.begin(), svec.end());
        psprvec[35] = temp;
        
        return psprvec;
    }
    friend std::ostream &operator<<(std::ostream &out, const CMCon &cc) {
        out << cc.PS_PR_con << std::endl;
        out << cc.SUCS_con;
        return out;
    }
    size_t size() {
        return PS_PR_con.size() + SUCS_con.size();
    }
    PS_PRCon PS_PR_con;
    SUCSCon SUCS_con;
};

class CM {
public:
    CM(varptr *myVars) {
        this->myVars = myVars;
    }
    static varptr *myVars;
    double CM_Drive2(double pop, double currentPop);
    CMCon CM_Ini();
    static int CM_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
};

CMCon CMInit(varptr *myVars);
std::vector<double> CM_Mb(double t, CMCon &CM_con, varptr *myVars);
