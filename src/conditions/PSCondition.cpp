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

#include "conditions/PSCondition.hpp"

PSCondition::PSCondition(const PSCondition* other)  {
    RuBP = other->RuBP;
    PGA = other->PGA;
    DPGA = other->DPGA;
    T3P = other->T3P;
    ADPG = other->ADPG;
    FBP = other->FBP;
    E4P = other->E4P;
    S7P = other->S7P;
    SBP = other->SBP;
    ATP = other->ATP;
    HexP = other->HexP;
    PenP = other->PenP;
    _Pi = other->_Pi;
    _ADP = other->_ADP;
    _v1 = other->_v1;
}

PSCondition::PSCondition(const arr &vec, size_t offset) {
    _fromArray(vec, offset);
}

void PSCondition::_fromArray(const arr &vec, size_t offset) {
    RuBP = vec[offset];
    PGA = vec[offset + 1];
    DPGA = vec[offset + 2];
    T3P = vec[offset + 3];
    ADPG = vec[offset + 4];
    FBP = vec[offset + 5];
    E4P = vec[offset + 6];
    S7P = vec[offset + 7];
    SBP = vec[offset + 8];
    ATP = vec[offset + 9];
    HexP = vec[offset + 10];
    PenP = vec[offset + 11];
}

arr PSCondition::_toArray()  {
    arr array = {RuBP, PGA, DPGA, T3P, ADPG, FBP, E4P, S7P, SBP, ATP, HexP, PenP};
    return array;
}

