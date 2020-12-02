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

#include "RA.hpp"
#include "driver.hpp"

// class for holding the results of RedoxReg_Rate calculations
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

// class for holding the inputs to RedoxReg_mb
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

// class for RedoxReg related calculations and common variables
class RedoxReg{
public:
    static RedoxRegCon RedoxReg_Ini(Variables *theVars);

    static void RedoxReg_Rate(double t, RedoxRegCon &RedoxReg_Con, Variables *theVars);

    static arr RedoxReg_Mb(double t, RedoxRegCon &RedoxReg_Con, Variables *theVars);

    static int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);
private:
    static double RedoxReg_VMAX13;
    static double RedoxReg_VMAX16;
    static double RedoxReg_VMAX6;
    static double RedoxReg_VMAX9;
};
