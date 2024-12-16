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

#include "conditions/DynaPSCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_CONDITION_COMPOSITE(DynaPS);

DynaPSCondition::DynaPSCondition(realtype *x) {
    initMembers();
    fromArray(x);
}

DynaPSCondition::DynaPSCondition(RACondition* rother, XanCycleCondition* xother) {
    initMembers();
    if (rother->parent == nullptr && rother->parentRedoxReg == nullptr) {
        RA_con = rother;
    } else {
        RA_con = new RACondition(rother);
    }
    if (xother->parent == nullptr) {
        XanCycle_con = xother;
    } else {
        XanCycle_con = new XanCycleCondition(xother);
    }
    RA_con->setParent(this);
    XanCycle_con->setParent(this);
}

DynaPSCondition::DynaPSCondition(const arr &vec, const std::size_t offset){
    initMembers();
    fromArray(vec, offset);
}

arr DynaPSCondition::_toArray() const {
    arr rvec = RA_con->toArray();
    arr xvec = XanCycle_con->toArray();
    rvec.insert(rvec.end(), xvec.begin(), xvec.end());
    return rvec;
}

void DynaPSCondition::_fromArray(const arr &vec, const std::size_t offset) {
    if (RA_con == nullptr)
        RA_con = new RACondition(this);
    if (XanCycle_con == nullptr)
        XanCycle_con = new XanCycleCondition(this);
    RA_con->fromArray(vec, offset);
    XanCycle_con->fromArray(vec, offset + RA_con->size());
}
