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
#include "XanCycle.hpp"
#include "driver.hpp"

// class for holding inputs to DynaPS_mb
class DynaPSCon {
public:
    DynaPSCon() {}
    DynaPSCon(const DynaPSCon &other) {
        RA_con = other.RA_con;
        XanCycle_con = other.XanCycle_con;
    }
    DynaPSCon(realtype *x) {
        fromArray(x);
    }
    DynaPSCon(const RACon &rother, const XanCycleCon &xother) {
        RA_con = rother;
        XanCycle_con = xother;
    }
    DynaPSCon(const arr &vec, size_t offset = 0){
            fromArray(vec, offset);
    }
    void fromArray(realtype *x) {
        arr vec(120);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    void fromArray(const arr &vec, size_t offset = 0) {
            RA_con.fromArray(vec, offset);
            XanCycle_con.fromArray(vec, offset + RA_con.size());
    }
    arr toArray() {
        arr rvec = RA_con.toArray();
        arr xvec = XanCycle_con.toArray();
        rvec.insert(rvec.end(), xvec.begin(), xvec.end());
        return rvec;
    }
    size_t size() {
        return RA_con.size() + XanCycle_con.size();
    }
    RACon RA_con;
    XanCycleCon XanCycle_con;
};

DynaPSCon DynaPS_Init(Variables *theVars);
arr DynaPS_Mb(double t, DynaPSCon &DynaPS_con, Variables *theVars);

//class for running DynaPS through an ODE solver
class DynaPS {
public:
    DynaPS(Variables *theVars) { this->theVars = theVars; }
    static Variables *theVars;
    arr DynaPS_Drive(size_t ParaNum, double Ratio);

    static int DynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
    DynaPSCon DynaPS_Ini();
};
