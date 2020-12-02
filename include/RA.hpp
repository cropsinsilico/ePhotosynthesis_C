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

#include "EPS.hpp"
#include "RuACT.hpp"

// class for holding input for RA_mb
class RACon {
public:
    RACon() {}
    RACon(const RACon &other) {
        RuACT_con = other.RuACT_con;
        EPS_con = other.EPS_con;
    }
    RACon(const EPSCon &eother, const RuACTCon &rother) {
        RuACT_con = rother;
        EPS_con = eother;
    }
    RACon(const arr &vec, size_t offset = 0){
        fromArray(vec, offset);
    }
    void fromArray(const arr &vec, size_t offset = 0) {
        EPS_con.fromArray(vec, offset);
        RuACT_con.fromArray(vec, offset + EPS_con.size());
    }
    arr toArray() {
        arr evec = EPS_con.toArray();
        arr rvec = RuACT_con.toArray();
        evec.reserve(size());
        evec.insert(evec.end(), rvec.begin(), rvec.end());
        return evec;
    }
    size_t size() {
        return EPS_con.size() + RuACT_con.size();
    }
    RuACTCon RuACT_con;
    EPSCon EPS_con;
};

RACon RA_Ini(Variables *theVars);

arr RA_Mb(double t, RACon &RA_Con, Variables *theVars);
