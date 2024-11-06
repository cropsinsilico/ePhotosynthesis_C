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

#include "conditions/EPSCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_CONDITION_COMPOSITE(EPS);

EPSCondition::EPSCondition(const EPSCondition* const other) {
    initMembers();
    CM_con = new CMCondition(other->CM_con);
    FIBF_con = new FIBFCondition(other->FIBF_con);
    CM_con->setParent(this);
    FIBF_con->setParent(this);
    copyMembers(*other);
}

EPSCondition::EPSCondition(realtype *x) {
    initMembers();
    fromArray(x);
}

EPSCondition::EPSCondition(FIBFCondition* fother, CMCondition* cother) {
    initMembers();
    if (cother->parent == nullptr) {
        CM_con = cother;
    } else {
        CM_con = new CMCondition(cother);
    }
    if (fother->parent == nullptr) {
        FIBF_con = fother;
    } else {
        FIBF_con = new FIBFCondition(fother);
    }
    CM_con->setParent(this);
    FIBF_con->setParent(this);
}

EPSCondition::EPSCondition(const arr &vec, const std::size_t offset) {
    initMembers();
    fromArray(vec, offset);
}

void EPSCondition::_fromArray(const arr &vec, const std::size_t offset) {
    if (FIBF_con == nullptr)
        FIBF_con = new FIBFCondition(this);
    if (CM_con == nullptr)
        CM_con = new CMCondition(this);
    FIBF_con->fromArray(vec, offset);
    CM_con->fromArray(vec, offset + FIBF_con->size());
}

arr EPSCondition::_toArray() const {
    arr fvec = FIBF_con->toArray();
    arr cvec = CM_con->toArray();
    fvec.insert(fvec.end(), cvec.begin(), cvec.end());
    return fvec;
}
