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

EPSCondition::EPSCondition(const EPSCondition* other) {
    _clear();
    CM_con = other->CM_con;
    FIBF_con = other->FIBF_con;
    CM_con->setParent(this);
    FIBF_con->setParent(this);
}

EPSCondition::EPSCondition(realtype *x, const uint adjust) {
    fromArray(x, adjust);
}

EPSCondition::EPSCondition(FIBFCondition* fother, CMCondition* cother) {
    _clear();
    CM_con = cother;
    FIBF_con = fother;
    CM_con->setParent(this);
    FIBF_con->setParent(this);
}

EPSCondition::EPSCondition(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void EPSCondition::_fromArray(const arr &vec, const size_t offset) {
    if (FIBF_con == nullptr)
        FIBF_con = new FIBFCondition(this);
    if (CM_con == nullptr)
        CM_con = new CMCondition(this);
    FIBF_con->fromArray(vec, offset);
    CM_con->fromArray(vec, offset + FIBF_con->size());
}

void EPSCondition::fromArray(realtype *x, const uint adjust) {
    arr vec = zeros(size());
    for (size_t i = 0; i < size() - adjust; i++)
        vec[i] = x[i];
    _fromArray(vec);
}

arr EPSCondition::_toArray() {
    arr fvec = FIBF_con->toArray();
    arr cvec = CM_con->toArray();
    fvec.reserve(size());
    fvec.insert(fvec.end(), cvec.begin(), cvec.end());
    return fvec;
}

void EPSCondition::_clear() {
    if (CM_con != nullptr) {
        delete CM_con;
        CM_con = nullptr;
    }
    if (FIBF_con != nullptr) {
        delete FIBF_con;
        FIBF_con = nullptr;
    }
}
