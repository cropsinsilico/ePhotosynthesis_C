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

#include "conditions/FICondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_CONDITION(FI);

FICondition::FICondition(const arr &vec, const std::size_t offset) {
    initMembers();
    fromArray(vec, offset);
}

void FICondition::_fromArray(const arr &vec, const std::size_t offset) {
    std::size_t count = 0;
    A = vec[offset + count++];
    U = vec[offset + count++];
    P680ePheo = vec[offset + count++];
    P680pPheon = vec[offset + count++];
    P680pPheo = vec[offset + count++];
    P680Pheon = vec[offset + count++];
    Yz = vec[offset + count++];
    S1T = vec[offset + count++];
    S2T = vec[offset + count++];
    S3T = vec[offset + count++];
    S0T = vec[offset + count++];
    S1Tp = vec[offset + count++];
    S2Tp = vec[offset + count++];
    S3Tp = vec[offset + count++];
    S0Tp = vec[offset + count++];
    QAQB = vec[offset + count++];
    QAnQB = vec[offset + count++];
    QAQBn = vec[offset + count++];
    QAnQBn = vec[offset + count++];
    QAQB2n = vec[offset + count++];
    QAnQB2n = vec[offset + count++];
    if (!FICondition::BF_connect)
        PQn = vec[offset + count++];
}

arr FICondition::_toArray() const {
    if (FICondition::BF_connect)
        return {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp,
                    S2Tp, S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n};
    return {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp, S2Tp,
                S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n, PQn};
}
