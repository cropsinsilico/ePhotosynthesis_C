#pragma once
#include <vector>

struct Variables;

class PSCon {
public:
    PSCon() {}
    PSCon(const PSCon &other) {
        RuBP = other.RuBP;
        PGA = other.PGA;
        DPGA = other.DPGA;
        T3P = other.T3P;
        ADPG = other.ADPG;
        FBP = other.FBP;
        E4P = other.E4P;
        S7P = other.S7P;
        SBP = other.SBP;
        ATP = other.ATP;
        NADPH = other.NADPH;
        CO2 = other.CO2;
        O2 = other.O2;
        HexP = other.HexP;
        PenP = other.PenP;
    }

    PSCon(const std::vector<double> &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const std::vector<double> &vec, size_t offset = 0) {
        RuBP = vec[offset];
        PGA = vec[offset + 1];
        DPGA = vec[offset + 2];
        T3P = vec[offset + 3];
        ADPG = vec[offset + 4];
        FBP = vec[offset + 5];
        E4P = vec[offset + 6];
        S7P = vec[offset + 7];
        SBP = vec[offset + 8];
        ATP = vec[offset + 9];
        NADPH = vec[offset + 10];
        CO2 = vec[offset + 11];
        O2 = vec[offset + 12];
        HexP = vec[offset + 13];
        PenP = vec[offset + 14];
    }

    std::vector<double> toArray() {
        std::vector<double> array = {RuBP, PGA, DPGA, T3P, ADPG, FBP, E4P, S7P, SBP, ATP, NADPH, CO2, O2, HexP, PenP};
        return array;
    }
    size_t size() {
        return count;
    }
    double RuBP = 0.;
    double PGA = 0.;
    double DPGA = 0.;
    double T3P = 0.;
    double ADPG = 0.;
    double FBP = 0.;
    double E4P = 0.;
    double S7P = 0.;
    double SBP = 0.;
    double ATP = 0.;
    double NADPH = 0.;
    double CO2 = 0.;
    double O2 = 0.;
    double HexP = 0.;
    double PenP = 0.;
private:
    size_t count = 15;
};

class PSVel {
public:
    PSVel() {}
    PSVel(const PSVel &other) {
        v1 = other.v1;
        v2 = other.v2;
        v3 = other.v3;
        v4 = other.v4;
        v5 = other.v5;
        v6 = other.v6;
        v7 = other.v7;
        v8 = other.v8;
        v9 = other.v9;
        v10 = other.v10;
        v13 = other.v13;
        v16 = other.v16;
        v23 = other.v23;
        v31 = other.v31;
        v32 = other.v32;
        v33 = other.v33;
        Pi = other.Pi;
        v24 = other.v24;
        v25 = other.v25;
    }
    PSVel& operator*=(const double val) {


        //t
        this->v1 *= val;
        this->v2 *= val;
        this->v3 *= val;
        this->v4 *= val;
        this->v5 *= val;
        this->v6 *= val;
        this->v7 *= val;
        this->v8 *= val;
        this->v9 *= val;
        this->v10 *= val;
        this->v13 *= val;
        this->v16 *= val;
        this->v23 *= val;
        this->v31 *= val;
        this->v32 *= val;
        this->v33 *= val;
        this->Pi *= val;
        this->v24 *= val;
        this->v25 *= val;
        return *this;
    }
    double v1 = 0.;
    double v2 = 0.;
    double v3 = 0.;
    double v4 = 0.;
    double v5 = 0.;
    double v6 = 0.;
    double v7 = 0.;
    double v8 = 0.;
    double v9 = 0.;
    double v10 = 0.;
    double v13 = 0.;
    double v16 = 0.;
    double v23 = 0.;
    double v31 = 0.;
    double v32 = 0.;
    double v33 = 0.;
    double Pi = 0.;
    double v24 = 0.;
    double v25 = 0.;

};

std::vector<double> PSmb(double t, PSCon &PSs, std::vector<double> &Param, Variables *myVars);
//std::vector<double> PSI(Variables *myVars);

PSCon PSInitial(Variables *myVars);

void PSRate(double t, PSCon &PSs, std::vector<double> &Param, Variables *myVars);
