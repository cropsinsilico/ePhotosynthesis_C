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

#include "conditions/RuACTCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_VALUE_SET_SRC(RuACTCondition);

RuACTCondition::RuACTCondition(const RuACTCondition* const other) :
  ConditionBase<RuACTCondition, RACondition, MODULE_RuACT>(other) {
    initMembers();
    ER = other->ER;
    Eaf = other->Eaf;
    ECMR = other->ECMR;
    if (!EPS_connect)
        RuBP = other->RuBP;
    copyMembers(*other);
}

RuACTCondition::RuACTCondition(const arr vec, const std::size_t offset) {
    initMembers();
    fromArray(vec, offset);
}

void RuACTCondition::_fromArray(const arr &vec, const std::size_t offset ){
    ER = vec[offset];
    Eaf = vec[offset + 1];
    ECMR = vec[offset + 2];
    if (!EPS_connect)
        RuBP = vec[offset + 3];
}

arr RuACTCondition::_toArray() const {
    if (EPS_connect)
        return {ER, Eaf, ECMR};
    return {ER, Eaf, ECMR, RuBP};
}
