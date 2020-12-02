#pragma once

/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include "definitions.hpp"

// class for holding the results of RROEA_Rate calculations
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

// class for RROEA_RC data
class RROEARC {
public:
    RROEARC() {}
    RROEARC(const RROEARC &other) {
        ke2GAPDH = other.ke2GAPDH;
        ke2MDH = other.ke2MDH;
        ke2FBPase = other.ke2FBPase;
        ke2SBPase = other.ke2SBPase;
        ke2PRK = other.ke2PRK;
        ke2RubACT = other.ke2RubACT;
        ke2Fd = other.ke2Fd;
        keFd2Thio = other.keFd2Thio;
        keFd2Calvin = other.keFd2Calvin;
        ke2ATPGPP = other.ke2ATPGPP;
    }

    double ke2GAPDH = 0;
    double ke2MDH = 0;
    double ke2FBPase = 0;
    double ke2SBPase = 0;
    double ke2PRK = 0;
    double ke2RubACT = 0;
    double ke2Fd = 0;
    double keFd2Thio = 0;
    double keFd2Calvin = 0;
    double ke2ATPGPP = 0;
};

// class for RROEA_KE data
class RROEAKE {
public:
    RROEAKE() {}
    RROEAKE(const RROEAKE &other) {

        KEe2FBPase = other.KEe2FBPase;
        KEe2SBPase = other.KEe2SBPase;
        KEe2PRK = other.KEe2PRK;
        KEe2ATPase = other.KEe2ATPase;
        KEe2RuACT = other.KEe2RuACT;
        KEe2GAPDH = other.KEe2GAPDH;
        KEe2MDH = other.KEe2MDH;
        KEe2ATPGPP = other.KEe2ATPGPP;
        KEeFd2Thio = other.KEeFd2Thio;
    }

    double KEe2FBPase = 0;
    double KEe2SBPase = 0;
    double KEe2PRK = 0;
    double KEe2ATPase = 0;
    double KEe2RuACT = 0;
    double KEe2GAPDH = 0;
    double KEe2MDH = 0;
    double KEe2ATPGPP = 0;
    double KEeFd2Thio = 0;
};

// class for RROEA_Pool data
class RROEAPool {
public:
    RROEAPool() {}
    RROEAPool(const RROEAPool &other) {

        GAPDH = other.GAPDH;
        FBPase = other.FBPase;
        SBPase = other.SBPase;
        PRK = other.PRK;
        ATPase = other.ATPase;
        ATPGPP = other.ATPGPP;
        MDH = other.MDH;
        ThioT = other.ThioT;
        FdT = other.FdT;
        RuACTT = other.RuACTT;
    }

    double GAPDH = 0;
    double FBPase = 0;
    double SBPase = 0;
    double PRK = 0;
    double ATPase = 0;
    double ATPGPP = 0;
    double MDH = 0;
    double ThioT = 0;
    double FdT = 0;
    double RuACTT = 0;
};


// class for holding the inputs to RROEA_mb
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

    RROEACon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    void fromArray(const arr &vec, size_t offset = 0) {
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

    arr toArray() {
        arr array = {GAPDH, FBPase, SBPase, PRK, ATPase, ATPGPP, MDH, Thio, Fd, RuACT};
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

// class for RROEA related functions
class RROEA {
public:
    static RROEACon RROEA_Ini(Variables *theVars);

    static arr RROEA_Mb(const double t, const RROEACon &RROEA_Con, Variables *theVars);

    static void RROEA_Rate(const double t, const RROEACon &RROEA_Con, Variables *theVars);
};
