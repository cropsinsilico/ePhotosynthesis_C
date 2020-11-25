#pragma once
#include "RA.hpp"
#include "driver.hpp"

class RedoxRegVel {
  public:
  RedoxRegVel() {}
  RedoxRegVel(const RedoxRegVel &other) {
      Vred = other.Vred;
Vox = other.Vox;

  }
    double Vred = 0.;
double Vox = 0.;

};

class RedoxRegCon {
public:
    RedoxRegCon() {}
    RedoxRegCon(const RedoxRegCon &other) {
        RA_con = other.RA_con;
        Thion = other.Thion;
    }
    RedoxRegCon(const RACon &rother, double thio = 0.) {
        RA_con = rother;
        Thion = thio;
    }
    RedoxRegCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const arr &vec, size_t offset = 0) {
        RA_con.fromArray(vec, offset);
        Thion = vec[offset + 92];
    }
    arr toArray() {
        arr rvec = RA_con.toArray();
        rvec.push_back(Thion);
        return rvec;
    }
    size_t size() {
        return RA_con.size() + 1;
    }
    RACon RA_con;
    double Thion = 0;
};

RedoxRegCon RedoxReg_Ini(Variables *myVars);

void RedoxReg_Rate(double t, RedoxRegCon &RedoxReg_Con, Variables *myVars);

arr RedoxReg_mb(double t, RedoxRegCon &RedoxReg_Con, Variables *myVars);

int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);
