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

#include "conditions/Leaf_C4Condition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

const std::size_t LeafCondition::count = 7;

LeafCondition::LeafCondition(const LeafCondition* const other) {
    Ci = other->Ci;
    Cb = other->Cb;
    Eb = other->Eb;
    Gs = other->Gs;
    Tleaf = other->Tleaf;
    H2Oou = other->H2Oou;
    CO2in = other->CO2in;
}

LeafCondition::LeafCondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}

void LeafCondition::_fromArray(const arr &vec, const std::size_t offset) {
    std::size_t counter = 0;
    Ci = vec[offset + counter++];
    Cb = vec[offset + counter++];
    Eb = vec[offset + counter++];
    Gs = vec[offset + counter++];
    Tleaf = vec[offset + counter++];
    H2Oou = vec[offset + counter++];
    CO2in = vec[offset + counter++];
}

arr LeafCondition::_toArray() const {
    return {Ci, Cb, Eb, Gs, Tleaf, H2Oou, CO2in};
}
