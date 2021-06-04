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

#include "con/DynaPSCon.hpp"

DynaPSCon::DynaPSCon(const DynaPSCon* other) {
    _clear();
    RA_con = other->RA_con;
    XanCycle_con = other->XanCycle_con;
    RA_con->setParent(this);
    XanCycle_con->setParent(this);
}

DynaPSCon::DynaPSCon(realtype *x) {
    fromArray(x);
}

DynaPSCon::DynaPSCon(RACon* rother, XanCycleCon* xother) {
    _clear();
    RA_con = rother;
    XanCycle_con = xother;
    RA_con->setParent(this);
    XanCycle_con->setParent(this);
}

DynaPSCon::DynaPSCon(const arr &vec, size_t offset){
    _fromArray(vec, offset);
}

arr DynaPSCon::_toArray() {
    arr rvec = RA_con->toArray();
    arr xvec = XanCycle_con->toArray();
    rvec.insert(rvec.end(), xvec.begin(), xvec.end());
    return rvec;
}

void DynaPSCon::_fromArray(const arr &vec, size_t offset) {
    if (RA_con == nullptr)
        RA_con = new RACon(this);
    if (XanCycle_con == nullptr)
        XanCycle_con = new XanCycleCon(this);
    RA_con->fromArray(vec, offset);
    XanCycle_con->fromArray(vec, offset + RA_con->size());
}

void DynaPSCon::_clear() {
    if (RA_con != nullptr) {
        delete RA_con;
        RA_con = nullptr;
    }
    if (XanCycle_con != nullptr) {
        delete XanCycle_con;
        XanCycle_con = nullptr;
    }
}
