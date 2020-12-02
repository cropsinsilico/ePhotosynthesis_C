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

#include <math.h>
#include "FI.hpp"
#include "BF.hpp"

// class for FIBF_Pool data
class FIBFPool {
  public:
  FIBFPool() {}
  FIBFPool(const FIBFPool &other) {
      PQT = other.PQT;
  }
  double PQT;
};

// class for input to FIBF_mb
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
    FIBFCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr &vec, const size_t offset = 0) {
        BF_con.fromArray(vec, offset);
        FI_con.fromArray(vec, offset + BF_con.size());
        kd = vec[offset + BF_con.size() + FI_con.size()];
    }
    arr toArray() {
        arr bvec = BF_con.toArray();
        arr fvec = FI_con.toArray();
        bvec.reserve(size());
        bvec.insert(bvec.end(), fvec.begin(), fvec.end());
        arr fivec = {kd};
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

void FIBF_Ini(Variables *theVars);

arr FIBF_Mb(const double t, const FIBFCon &FIBF_Con, Variables *theVars);
