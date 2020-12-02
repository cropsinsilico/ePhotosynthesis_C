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

#include "PS_PR.hpp"
#include "SUCS.hpp"
#include "driver.hpp"

class CMCon {
public:
    CMCon() {}
    CMCon(const CMCon &other) {
        PS_PR_con = other.PS_PR_con;
        SUCS_con = other.SUCS_con;
    }
    CMCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    CMCon(realtype *x) {
        fromArray(x);
    }
    CMCon(const PS_PRCon &pother, const SUCSCon &sother) {
        PS_PR_con = pother;
        SUCS_con = sother;
    }
    void fromArray(const arr &vec, size_t offset = 0) {

        arr pvec(PS_PR_con.size());
        std::copy(vec.begin() + offset, vec.begin() + PS_PR_con.size() + offset, pvec.begin());
        pvec[23] = vec[35 + offset];

        PS_PR_con.fromArray(pvec);
        SUCS_con.fromArray(vec, offset + PS_PR_con.size() - 1);
    }
    void fromArray(realtype *x) {
        arr vec(36);
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    arr toArray() {
        arr psprvec = PS_PR_con.toArray();
        arr svec = SUCS_con.toArray();
        psprvec.reserve(size());
        double temp = psprvec[23];
        psprvec.insert(psprvec.end() - 1, svec.begin(), svec.end());
        psprvec[35] = temp;

        return psprvec;
    }

    size_t size() {
        return PS_PR_con.size() + SUCS_con.size();
    }
    PS_PRCon PS_PR_con;
    SUCSCon SUCS_con;
};
struct Variables;
class CM {
public:
    CM(Variables *theVars) {
        this->theVars = theVars;
    }
    static Variables *theVars;
    double CM_Drive2(double pop, double currentPop);
    CMCon CM_Ini();
    static int CM_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
};

CMCon CMInit(Variables *theVars);
arr CM_Mb(double t, CMCon &CM_con, Variables *theVars);
