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

#include "conditions/SUCSCondition.hpp"

const size_t SUCSCondition::count = 12;

SUCSCondition::SUCSCondition(const SUCSCondition &other) {
    T3Pc = other.T3Pc;
    FBPc = other.FBPc;
    HexPc = other.HexPc;
    F26BPc = other.F26BPc;
    UDPGc = other.UDPGc;
    SUCP = other.SUCP;
    SUC = other.SUC;
    PGAc = other.PGAc;
}

SUCSCondition::SUCSCondition(const SUCSCondition *other) {
    T3Pc = other->T3Pc;
    FBPc = other->FBPc;
    HexPc = other->HexPc;
    F26BPc = other->F26BPc;
    UDPGc = other->UDPGc;
    SUCP = other->SUCP;
    SUC = other->SUC;
    PGAc = other->PGAc;
}

SUCSCondition::SUCSCondition(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void SUCSCondition::_fromArray(const arr &vec, const size_t offset) {
    T3Pc = vec[offset];
    FBPc = vec[offset + 1];
    HexPc = vec[offset + 2];
    F26BPc = vec[offset + 3];
    UDPGc = vec[offset + 4];
    SUCP = vec[offset + 5];
    SUC = vec[offset + 6];
    PGAc = vec[offset + 7];
}

void SUCSCondition::_clear() {
    T3Pc = 0.;
    FBPc = 0.;
    HexPc = 0.;
    F26BPc = 0.;
    UDPGc = 0.;
    SUCP = 0.;
    SUC = 0.;
    PGAc = 0.;
}

arr SUCSCondition::_toArray() {
    arr vec = {T3Pc, FBPc, HexPc, F26BPc, UDPGc, SUCP, SUC, PGAc};
    return vec;
}
