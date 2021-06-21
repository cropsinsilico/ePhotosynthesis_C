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

#include "containers/FIBFContainer.hpp"

FIBFContainer::FIBFContainer(const FIBFContainer* other) {
    _clear();
    BF_con = other->BF_con;
    FI_con = other->FI_con;
    BF_con->setParent(this);
    FI_con->setParent(this);
    kd = other->kd;
}

FIBFContainer::FIBFContainer(BFContainer* bother, FIContainer* fother) {
    _clear();
    BF_con = bother;
    FI_con = fother;
    BF_con->setParent(this);
    FI_con->setParent(this);
    kd = pow(10, 8) * 0.5;
}

FIBFContainer::FIBFContainer(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void FIBFContainer::_fromArray(const arr &vec, const size_t offset) {
    if (BF_con == nullptr)
        BF_con = new BFContainer(this);
    if (FI_con == nullptr)
        FI_con = new FIContainer(this);
    BF_con->fromArray(vec, offset);
    FI_con->fromArray(vec, offset + BF_con->size());
    kd = vec[offset + BF_con->size() + FI_con->size()];
}

arr FIBFContainer::_toArray() {
    arr bvec = BF_con->toArray();
    arr fvec = FI_con->toArray();
    bvec.reserve(size());
    bvec.insert(bvec.end(), fvec.begin(), fvec.end());
    arr fivec = {kd};
    bvec.insert(bvec.end(), fivec.begin(), fivec.end());
    return bvec;
}

void FIBFContainer::_clear() {
    if (BF_con != nullptr){
        delete BF_con;
        BF_con = nullptr;
    }
    if (FI_con != nullptr) {
        delete FI_con;
        FI_con = nullptr;
    }
}
