#pragma once
#include "RROEA.hpp"
#include "DynaPS.hpp"

class trDynaPSCon {
public:
    trDynaPSCon() {}
    trDynaPSCon(const trDynaPSCon &other) {
        RROEA_con = other.RROEA_con;
        DynaPS_con = other.DynaPS_con;
    }
    trDynaPSCon(const DynaPSCon &dother, const RROEACon &rother) {
        DynaPS_con = dother;
        RROEA_con = rother;
    }
    trDynaPSCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    trDynaPSCon(realtype *x) {
        fromArray(x);
    }
    void fromArray(realtype *x) {
        arr vec(size());
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    void fromArray(const arr &vec, size_t offset = 0) {
        DynaPS_con.fromArray(vec, offset);
        RROEA_con.fromArray(vec, offset + 110);//DynaPS_con.size());
    }
    arr toArray() {
        arr dyvec = DynaPS_con.toArray();
        arr rrvec = RROEA_con.toArray();
        arr vec = zeros(120);
        for (size_t i = 0; i < 96; i++)
            vec[i] = dyvec[i];
        for (size_t i = 0; i < 10; i++)
            vec[i+110] = rrvec[i];
        return vec;

    }
    size_t size() {
        return 120;//RROEA_con.size() + DynaPS_con.size();
    }
    RROEACon RROEA_con;
    DynaPSCon DynaPS_con;
};


class trDynaPS {
public:
  trDynaPS(Variables *myVars) { this->myVars = myVars; }
  static Variables *myVars;
  arr trDynaPS_Drive(double ParaNum, double Ratio);

  static int trDynaPS_mb(realtype t, N_Vector u, N_Vector u_dot,
                         void *user_data);
  trDynaPSCon trDynaPS_Ini();
};
