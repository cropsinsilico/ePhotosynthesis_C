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

#include "RROEA.hpp"
#include "DynaPS.hpp"
#include "driver.hpp"

// class for holding inputs to trDynaPS_mb
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

trDynaPSCon trDynaPS_Init(Variables *theVars);
arr trDynaPS_Mb(const double t, const trDynaPSCon &trDynaPS_con, Variables *theVars);
// class for running trDynaPS with an ODE solver
class trDynaPS {
public:
  trDynaPS(Variables *theVars) { this->theVars = theVars; }
  ~trDynaPS();
  static Variables *theVars;
  arr trDynaPS_Drive(size_t ParaNum, double Ratio);

  static int trDynaPS_mb(realtype t, N_Vector u, N_Vector u_dot,
                         void *user_data);
  trDynaPSCon trDynaPS_Ini();
};
