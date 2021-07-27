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
    if (!useC3)
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
    size_t current = 0;
    RuBP = vec[offset + current++];
    PGA = vec[offset + current++];
    DPGA = vec[offset + current++];
    T3P = vec[offset + current++];
    if (!useC3)
        ADPG = vec[offset + current++];
    FBP = vec[offset + current++];
    E4P = vec[offset + current++];
    S7P = vec[offset + current++];
    SBP = vec[offset + current++];
    ATP = vec[offset + current++];
    HexP = vec[offset + current++];
    PenP = vec[offset + current++];
}

arr PSCondition::_toArray()  {
    if (useC3)
        return {RuBP, PGA, DPGA, T3P, FBP, E4P, S7P, SBP, ATP, HexP, PenP};
    return {RuBP, PGA, DPGA, T3P, ADPG, FBP, E4P, S7P, SBP, ATP, HexP, PenP};
}

