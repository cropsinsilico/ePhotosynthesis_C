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

#include "conditions/trDynaPSCondition.hpp"

const size_t trDynaPSCondition::count = 120;

trDynaPSCondition::trDynaPSCondition(const trDynaPSCondition* other) {
    _clear();
    RROEA_con = other->RROEA_con;
    DynaPS_con = other->DynaPS_con;
    RROEA_con->setParent(this);
    DynaPS_con->setParent(this);
}

trDynaPSCondition::trDynaPSCondition(DynaPSCondition* dother, RROEACondition* rother) {
    _clear();
    DynaPS_con = dother;
    RROEA_con = rother;
    RROEA_con->setParent(this);
    DynaPS_con->setParent(this);
}

trDynaPSCondition::trDynaPSCondition(const arr &vec, size_t offset) {
    _fromArray(vec, offset);
}

trDynaPSCondition::trDynaPSCondition(realtype *x) {
    fromArray(x);
}

void trDynaPSCondition::_fromArray(const arr &vec, size_t offset) {
    if (DynaPS_con == nullptr)
        DynaPS_con = new DynaPSCondition(this);
    DynaPS_con->fromArray(vec, offset);
    if (RROEA_con == nullptr)
        RROEA_con = new RROEACondition(this);
    RROEA_con->fromArray(vec, offset + 110);
}

arr trDynaPSCondition::_toArray() {
    arr dyvec = DynaPS_con->toArray();
    arr rrvec = RROEA_con->toArray();
    arr vec = zeros(120);
    for (size_t i = 0; i < 96; i++)
        vec[i] = dyvec[i];
    for (size_t i = 0; i < 10; i++)
        vec[i+110] = rrvec[i];
    return vec;
}

void trDynaPSCondition::_clear() {
    if (RROEA_con != nullptr) {
        delete RROEA_con;
        RROEA_con = nullptr;
    }
    if (DynaPS_con != nullptr) {
        delete DynaPS_con;
        DynaPS_con = nullptr;
    }
}
