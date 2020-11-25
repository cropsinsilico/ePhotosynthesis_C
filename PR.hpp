#pragma once
#include "definitions.hpp"

class PRCon {
public:
    PRCon() {}
    PRCon(const PRCon &other) {
        GCEA = other.GCEA;
        GCA = other.GCA;
        PGA = other.PGA;
        PGCA = other.PGCA;
        GCAc = other.GCAc;
        GOAc = other.GOAc;
        SERc = other.SERc;
        GLYc = other.GLYc;
        HPRc = other.HPRc;
        GCEAc = other.GCEAc;
        RUBP = other.RUBP;
        CO2 = other.CO2;
        O2 = other.O2;
    }
    PRCon(const arr vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr vec, size_t offset = 0){
        GCEA= vec[offset];
        GCA= vec[offset + 1];
        PGA= vec[offset + 2];
        PGCA= vec[offset + 3];
        GCAc= vec[offset + 4];
        GOAc= vec[offset + 5];
        SERc= vec[offset + 6];
        GLYc= vec[offset + 7];
        HPRc= vec[offset + 8];
        GCEAc= vec[offset + 9];
        RUBP= vec[offset + 10];
        CO2= vec[offset + 11];
        O2= vec[offset + 12];
    }
    arr toArray() {
        arr array = {GCEA, GCA, PGA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc, RUBP, CO2, O2};
        return array;
    }
    size_t size() {
        return count;
    }
    double GCEA = 0.;
    double GCA = 0.;
    double PGA = 0.;
    double PGCA = 0.;
    double GCAc = 0.;
    double GOAc = 0.;
    double SERc = 0.;
    double GLYc = 0.;
    double HPRc = 0.;
    double GCEAc = 0.;
    double RUBP = 0.;
    double CO2 = 0.;
    double O2 = 0.;
private:
    size_t count = 13;
};

class PRVel {
public:
    PRVel() {}
    PRVel(const PRVel &other) {
        v111 = other.v111;
        v112 = other.v112;
        v113 = other.v113;
        v121 = other.v121;
        v122 = other.v122;
        v123 = other.v123;
        v124 = other.v124;
        v131 = other.v131;
        v1in = other.v1in;
        v2out = other.v2out;
    }
    PRVel& operator*=(const double val) {
        this->v111 *= val;
        this->v112 *= val;
        this->v113 *= val;
        this->v121 *= val;
        this->v122 *= val;
        this->v123 *= val;
        this->v124 *= val;
        this->v131 *= val;
        this->v1in *= val;
        this->v2out *= val;
        return *this;
    }
    double v111 = 0.;
    double v112 = 0.;
    double v113 = 0.;
    double v121 = 0.;
    double v122 = 0.;
    double v123 = 0.;
    double v124 = 0.;
    double v131 = 0.;
    double v1in = 0.;
    double v2out = 0.;
};

PRCon PRinitial(Variables *myVars);

arr PRmb(double t, PRCon &PR_con, Variables *myVars);

void PRrate(double t, PRCon &PR_con, Variables *myVars);
