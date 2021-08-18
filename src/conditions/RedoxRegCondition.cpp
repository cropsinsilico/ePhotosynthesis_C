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

#include "conditions/RedoxRegCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

RedoxRegCondition::RedoxRegCondition(const RedoxRegCondition* other) {
    _clear();
    RA_con = new RACondition(other->RA_con);
    Thion = other->Thion;
    //RA_con.setParent(this);
}

RedoxRegCondition::RedoxRegCondition(RACondition* rother, double thio) {
    _clear();
    if (rother->parent == nullptr) {
        RA_con = rother;
    } else {
        RA_con = new RACondition(rother);
    }
    //RA_con->setParent(this);
    Thion = thio;
}

RedoxRegCondition::RedoxRegCondition(const arr &vec, size_t offset) {
    fromArray(vec, offset);
}

void RedoxRegCondition::_fromArray(const arr &vec, size_t offset) {
    if (RA_con == nullptr)
        RA_con = new RACondition();
    RA_con->fromArray(vec, offset);
    Thion = vec[offset + RACondition::size()];
}

arr RedoxRegCondition::_toArray() {
    arr rvec = RA_con->toArray();
    rvec.push_back(Thion);
    return rvec;
}

void RedoxRegCondition::_clear() {
    if (RA_con != nullptr) {
        delete RA_con;
        RA_con = nullptr;
    }
}
