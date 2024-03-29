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

#include "conditions/RACondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

std::size_t RACondition::count = 0;

RACondition::RACondition(const RACondition* const other) {
    RuACT_con = new RuACTCondition(other->RuACT_con);
    EPS_con = new EPSCondition(other->EPS_con);
    RuACT_con->setParent(this);
    EPS_con->setParent(this);
}

RACondition::RACondition(EPSCondition* eother, RuACTCondition* rother) {
    if (rother->parent == nullptr) {
        RuACT_con = rother;
    } else {
        RuACT_con = new RuACTCondition(rother);
    }
    if (eother->parent == nullptr) {
        EPS_con = eother;
    } else {
        EPS_con = new EPSCondition(eother);
    }
    RuACT_con->setParent(this);
    EPS_con->setParent(this);
}

RACondition::RACondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}


void RACondition::_clear() {
    if (RuACT_con != nullptr) {
        delete RuACT_con;
        RuACT_con = nullptr;
    }
    if (EPS_con != nullptr) {
        delete EPS_con;
        EPS_con = nullptr;
    }
    count = 0;
}

void RACondition::_fromArray(const arr &vec, const std::size_t offset) {
    if (EPS_con == nullptr)
        EPS_con = new EPSCondition(this);
    if (RuACT_con == nullptr)
        RuACT_con = new RuACTCondition(this);
    EPS_con->fromArray(vec, offset);
    RuACT_con->fromArray(vec, offset + EPSCondition::size());
}

arr RACondition::_toArray() const {
    arr evec = EPS_con->toArray();
    arr rvec = RuACT_con->toArray();
    evec.reserve(size());
    evec.insert(evec.end(), rvec.begin(), rvec.end());
    return evec;
}
