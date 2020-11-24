#pragma once
#include <math.h>
#include "FI.hpp"
#include "BF.hpp"

class FIBFPool {
  public:
  FIBFPool() {}
  FIBFPool(const FIBFPool &other) {
      PQT = other.PQT;
  }
  double PQT;  
};

class FIBFCon {
public:
    FIBFCon() {}
    FIBFCon(const FIBFCon &other) {
        BF_con = other.BF_con;
        FI_con = other.FI_con;
        kd = other.kd;
    }
    FIBFCon(const BFCon &bother, const FICon &fother) {
        BF_con = bother;
        FI_con = fother;
        kd = pow(10, 8) * 0.5;
    }
    FIBFCon(const std::vector<double> &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const std::vector<double> &vec, const size_t offset = 0) {
        BF_con.fromArray(vec, offset);
        FI_con.fromArray(vec, offset + BF_con.size());
        kd = vec[offset + BF_con.size() + FI_con.size()];
    }
    std::vector<double> toArray() {
        std::vector<double> bvec = BF_con.toArray();
        std::vector<double> fvec = FI_con.toArray();
        bvec.reserve(size());
        bvec.insert(bvec.end(), fvec.begin(), fvec.end());
        std::vector<double> fivec = {kd};
        //bvec[size() - 1] = kd;
        bvec.insert(bvec.end(), fivec.begin(), fivec.end());
        return bvec;
    }
    size_t size() {
        return BF_con.size() + FI_con.size() + 1;
    }
    BFCon BF_con;
    FICon FI_con;
    double kd = 0; // The initialization of the initial rate constant for heat dissipation

};

void FIBF_Ini(Variables *myVars);

std::vector<double> FIBF_MB(double t, FIBFCon &FIBF_Con, Variables *myVars);
