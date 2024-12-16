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

#include "conditions/PS_PRCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_CONDITION_COMPOSITE(PS_PR);

std::size_t PS_PRCondition::count = 0;

PS_PRCondition::PS_PRCondition(PSCondition* sother, PRCondition* rother) {
    initMembers();
    if (sother->parent == nullptr) {
        PS_con = sother;
    } else {
        PS_con = new PSCondition(sother);
    }
    if (rother->parent == nullptr) {
        PR_con = rother;
    } else {
        PR_con = new PRCondition(rother);
    }
    PS_con->setParent(this);
    PR_con->setParent(this);
}

PS_PRCondition::PS_PRCondition(const arr &vec, const std::size_t offset) {
    initMembers();
    fromArray(vec, offset);
}

void PS_PRCondition::_fromArray(const arr &vec, const std::size_t offset) {
    if (PS_con == nullptr)
        PS_con = new PSCondition(this);
    if (PR_con  == nullptr)
        PR_con = new PRCondition(this);
    PS_con->fromArray(vec, offset);
    PR_con->fromArray(vec, offset + PS_con->size());
}

arr PS_PRCondition::_toArray() const {
    arr psvec = PS_con->toArray();
    arr prvec = PR_con->toArray();
    psvec.insert(psvec.end(), prvec.begin(), prvec.end());
    return psvec;
}
