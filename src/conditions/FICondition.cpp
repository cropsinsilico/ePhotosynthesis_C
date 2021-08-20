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

FICondition::FICondition(const FICondition* other) {
    A = other->A;
    U = other->U;
    P680ePheo = other->P680ePheo;
    P680pPheon = other->P680pPheon;
    P680pPheo = other->P680pPheo;
    P680Pheon = other->P680Pheon;
    Yz = other->Yz;
    S1T = other->S1T;
    S2T = other->S2T;
    S3T = other->S3T;
    S0T = other->S0T;
    S1Tp = other->S1Tp;
    S2Tp = other->S2Tp;
    S3Tp = other->S3Tp;
    S0Tp = other->S0Tp;
    QAQB = other->QAQB;
    QAnQB = other->QAnQB;
    QAQBn = other->QAQBn;
    QAnQBn = other->QAnQBn;
    QAQB2n = other->QAQB2n;
    QAnQB2n = other->QAnQB2n;
    if (!FICondition::BF_connect)
        PQn = other->PQn;
}

FICondition::FICondition(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void FICondition::_fromArray(const arr &vec, const size_t offset) {
    size_t count = 0;
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

arr FICondition::_toArray() {
    if (FICondition::BF_connect)
        return {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp,
                    S2Tp, S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n};
    return {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp, S2Tp,
                S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n, PQn};
}
