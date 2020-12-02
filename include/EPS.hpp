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

#include "FIBF.hpp"
#include "CM.hpp"

// class for holding the inputs to EPS_mb
class EPSCon {
public:
    EPSCon() {}
    EPSCon(const EPSCon &other) {
        CM_con = other.CM_con;
        FIBF_con = other.FIBF_con;
    }
    EPSCon(const FIBFCon &fother, const CMCon &cother) {
        CM_con = cother;
        FIBF_con = fother;
    }
    EPSCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const arr &vec, const size_t offset = 0) {
        FIBF_con.fromArray(vec, offset);
        CM_con.fromArray(vec, offset + FIBF_con.size());
    }
    arr toArray() {
        arr fvec = FIBF_con.toArray();
        arr cvec = CM_con.toArray();
        fvec.reserve(size());
        fvec.insert(fvec.end(), cvec.begin(), cvec.end());
        return fvec;
    }
    size_t size() {
        return CM_con.size() + FIBF_con.size();
    }
    CMCon CM_con;
    FIBFCon FIBF_con;
};

EPSCon EPS_Ini(Variables *theVars);

arr EPS_Mb(double t, EPSCon &EPS_Con, Variables *theVars);
