#pragma once
#include <vector>

class RROEAVel {
public:
    RROEAVel() {}
    RROEAVel(const RROEAVel &other) {
        ve2GAPDH = other.ve2GAPDH;
        ve2FBPase = other.ve2FBPase;
ve2SBPase = other.ve2SBPase;
ve2PRK = other.ve2PRK;
ve2ATPase = other.ve2ATPase;
ve2ATPGPP = other.ve2ATPGPP;
ve2MDH = other.ve2MDH;
ve2Fd = other.ve2Fd;
veFd2Thio = other.veFd2Thio;
veFd2Calvin = other.veFd2Calvin;
ve2RuACT = other.ve2RuACT;

    }
    double ve2GAPDH = 0.;  
double ve2FBPase = 0.;  
double ve2SBPase = 0.;  
double ve2PRK = 0.;  
double ve2ATPase = 0.;  
double ve2ATPGPP = 0.;  
double ve2MDH = 0.;  
double ve2Fd = 0.;  
double veFd2Thio = 0.;  
double veFd2Calvin = 0.;  
double ve2RuACT = 0.;  

};
/*
struct Variables;
class RROEACon {
public:
    RROEACon() {}
    RROEACon(const RROEACon &other) {
        GAPDH = other.GAPDH;
        FBPase = other.FBPase;
        SBPase = other.SBPase;
        PRK = other.PRK;
        ATPase = other.ATPase;
        ATPGPP = other.ATPGPP;
        MDH = other.MDH;
        Thio = other.Thio;
        Fd = other.Fd;
        RuACT = other.RuACT;
    }

    RROEACon(const std::vector<double> &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const std::vector<double> &vec, size_t offset = 0) {
        GAPDH = vec[offset];
        FBPase = vec[offset + 1];
        SBPase = vec[offset + 2];
        PRK = vec[offset + 3];
        ATPase = vec[offset + 4];
        ATPGPP = vec[offset + 5];
        MDH = vec[offset + 6];
        Thio = vec[offset + 7];
        Fd = vec[offset + 8];
        RuACT = vec[offset + 9];
    }
    size_t size() {
        return count;
    }

    std::vector<double> toArray() {
        std::vector<double> array = {GAPDH, FBPase, SBPase, PRK, ATPase, ATPGPP, MDH, Thio, Fd, RuACT};
        return array;
    }

    double GAPDH = 0.;  //  The initial concentration of active GAPDH
    double FBPase = 0.;  //  The initial concentration of active FBPase
    double SBPase = 0.;  //  The initial concentration of active SBPase
    double PRK = 0.;  //  The initial concentration of actove PRK
    double ATPase = 0.;  //  The initial concentration of actove ATPase
    double ATPGPP = 0.;  //  The initial concentration of actove ATPGPP
    double MDH = 0.;  //  The initial concentration of actove MDH
    double Thio = 0.;  //  The initial concentration of reduced thioredoxin
    double Fd = 0.;  //  The initial concentration of reduced ferrodoxin
    double RuACT = 0.;  //  The initial concentration of active Rubisco activase

private:
    size_t count = 10;

};

RROEACon RROEA_Ini(Variables *myVars);

std::vector<double> RROEA_Mb(double t, RROEACon &RROEA_Con, Variables *myVars);

void RROEA_Rate(double t, RROEACon &RROEA_Con, Variables *myVars);
*/
