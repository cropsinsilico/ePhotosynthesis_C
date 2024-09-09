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

#include "conditions/RROEACondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

RROEACondition::RROEACondition(const RROEACondition* const other) :
  ConditionBase<RROEACondition, trDynaPSCondition, MODULE_RROEA>(other) {
    GAPDH = other->GAPDH;
    FBPase = other->FBPase;
    SBPase = other->SBPase;
    PRK = other->PRK;
    ATPase = other->ATPase;
    ATPGPP = other->ATPGPP;
    MDH = other->MDH;
    Thio = other->Thio;
    Fd = other->Fd;
    RuACT = other->RuACT;
}

RROEACondition:: RROEACondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}

void RROEACondition::_fromArray(const arr &vec, const std::size_t offset) {
    GAPDH = vec[offset];
    FBPase = vec[offset + 1];
    SBPase = vec[offset + 2];
    PRK = vec[offset + 3];
    ATPase = vec[offset + 4];
    ATPGPP = vec[offset + 5];
    MDH = vec[offset + 6];
    Thio = vec[offset + 7];
    Fd = vec[offset + 8];
    RuACT = vec[offset + 9];
}

arr RROEACondition::_toArray() const {
    arr array = {GAPDH, FBPase, SBPase, PRK, ATPase, ATPGPP, MDH, Thio, Fd, RuACT};
    return array;
}
