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

#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

std::size_t RAC4leafMetaCondition::count = 0;

RAC4leafMetaCondition::RAC4leafMetaCondition(const RAC4leafMetaCondition* const other) {
    Leaf_con = new LeafCondition(other->Leaf_con);
    Enzyme_con = new EnzymeCondition(other->Enzyme_con);
    AE_con = new AECondition(other->AE_con);
    Leaf_con->setParent(this);
    Enzyme_con->setParent(this);
    AE_con->setParent(this);
}

RAC4leafMetaCondition::RAC4leafMetaCondition(LeafCondition* lother, EnzymeCondition* eother, AECondition* aother) {
    if (lother->parent == nullptr) {
        Leaf_con = lother;
    } else {
        Leaf_con = new LeafCondition(lother);
    }
    if (eother->parent == nullptr) {
        Enzyme_con = eother;
    } else {
        Enzyme_con = new EnzymeCondition(eother);
    }
    if (aother->parent == nullptr) {
        AE_con = aother;
    } else {
        AE_con = new AECondition(aother);
    }
    Leaf_con->setParent(this);
    Enzyme_con->setParent(this);
    AE_con->setParent(this);
}

RAC4leafMetaCondition::RAC4leafMetaCondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}

RAC4leafMetaCondition::RAC4leafMetaCondition(realtype *x) {
    fromArray(x);
}

void RAC4leafMetaCondition::_fromArray(const arr &vec, const std::size_t offset) {
    if (Leaf_con == nullptr)
        Leaf_con = new LeafCondition(this);
    Leaf_con->fromArray(vec, offset);
    if (Enzyme_con == nullptr)
        Enzyme_con = new EnzymeCondition(this);
    Enzyme_con->fromArray(vec, offset + Leaf_con->size());
    if (AE_con == nullptr)
        AE_con = new AECondition(this);
    AE_con->fromArray(vec, offset + Leaf_con->size() + Enzyme_con->size());
}

arr RAC4leafMetaCondition::_toArray() const {
    arr lvec = Leaf_con->toArray();
    arr evec = Enzyme_con->toArray();
    arr avec = AE_con->toArray();
    lvec.insert(lvec.end(), evec.begin(), evec.end());
    lvec.insert(lvec.end(), avec.begin(), avec.end());
    return lvec;
}

void RAC4leafMetaCondition::_clear() {
    if (Leaf_con != nullptr) {
        delete Leaf_con;
        Leaf_con = nullptr;
    }
    if (Enzyme_con != nullptr) {
        delete Enzyme_con;
        Enzyme_con = nullptr;
    }
    if (AE_con != nullptr) {
        delete AE_con;
        AE_con = nullptr;
    }

}
