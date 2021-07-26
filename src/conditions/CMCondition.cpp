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

#include "conditions/CMCondition.hpp"

CMCondition::CMCondition(const CMCondition* other) {
    _clear();
    PS_PR_con = other->PS_PR_con;
    SUCS_con = other->SUCS_con;
    PS_PR_con->setParent(this);
    SUCS_con->setParent(this);
}

CMCondition::CMCondition(const arr &vec, size_t offset) {
    fromArray(vec, offset);
}

CMCondition::CMCondition(realtype *x) {
    fromArray(x);
}

CMCondition::CMCondition(PS_PRCondition* pother, SUCSCondition* sother) {
    _clear();
    PS_PR_con = pother;
    SUCS_con = sother;
    PS_PR_con->setParent(this);
    SUCS_con->setParent(this);
}

void CMCondition::_fromArray(const arr &vec, size_t offset) {
    if (PS_PR_con == nullptr)
        PS_PR_con = new PS_PRCondition(this);
    if (SUCS_con == nullptr)
        SUCS_con = new SUCSCondition(this);
    arr pvec(PS_PR_con->size());
    std::copy(vec.begin() + offset, vec.begin() + PS_PR_con->size() + offset, pvec.begin());
    pvec[23] = vec[35 + offset];
    pvec[23] = vec[size() - 1 + offset];

    PS_PR_con->fromArray(pvec);
    SUCS_con->fromArray(vec, offset + PS_PR_con->size() - 1);
}

arr CMCondition::_toArray() {
    arr psprvec = PS_PR_con->toArray();
    arr svec = SUCS_con->toArray();
    psprvec.reserve(size());
    double temp = psprvec[23];
    psprvec.insert(psprvec.end() - 1, svec.begin(), svec.end());
        psprvec[size() - 1] = temp;

    return psprvec;
}

void CMCondition::_clear() {
    if (PS_PR_con != nullptr) {
        delete PS_PR_con;
        PS_PR_con = nullptr;
    }
    if (SUCS_con != nullptr) {
        delete SUCS_con;
        SUCS_con = nullptr;
    }
}
