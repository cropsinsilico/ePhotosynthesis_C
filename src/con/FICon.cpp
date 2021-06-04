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

#include "con/FICon.hpp"

FICon::FICon(const FICon* other) {
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
    PQn = other->PQn;
}

FICon::FICon(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void FICon::_fromArray(const arr &vec, const size_t offset) {
     A = vec[offset];
     U = vec[offset + 1];
     P680ePheo = vec[offset + 2];
     P680pPheon = vec[offset + 3];
     P680pPheo = vec[offset + 4];
     P680Pheon = vec[offset + 5];
     Yz = vec[offset + 6];
     S1T = vec[offset + 7];
     S2T = vec[offset + 8];
     S3T = vec[offset + 9];
     S0T = vec[offset + 10];
     S1Tp = vec[offset + 11];
     S2Tp = vec[offset + 12];
     S3Tp = vec[offset + 13];
     S0Tp = vec[offset + 14];
     QAQB = vec[offset + 15];
     QAnQB = vec[offset + 16];
     QAQBn = vec[offset + 17];
     QAnQBn = vec[offset + 18];
     QAQB2n = vec[offset + 19];
     QAnQB2n = vec[offset + 20];
     PQn = vec[offset + 21];
 }

arr FICon::_toArray() {
    arr vec = {A, U, P680ePheo, P680pPheon, P680pPheo, P680Pheon, Yz, S1T, S2T, S3T, S0T, S1Tp, S2Tp, S3Tp, S0Tp, QAQB, QAnQB, QAQBn, QAnQBn, QAQB2n, QAnQB2n, PQn};
    return vec;
}
