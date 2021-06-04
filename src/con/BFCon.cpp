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

#include "con/BFCon.hpp"

BFCon:: BFCon(const BFCon* other) {
    ISPHr = other->ISPHr;
    cytc1 = other->cytc1;
    ISPo = other->ISPo;
    ISPoQH2 = other->ISPoQH2;
    QHsemi = other->QHsemi;
    cytbL = other->cytbL;
    Qi = other->Qi;
    Q = other->Q;
    cytbH = other->cytbH;
    Qn = other->Qn;
    Qr = other->Qr;
    QH2 = other->QH2;
    cytc2 = other->cytc2;
    P700 = other->P700;
    ADP = other->ADP;
    Pi = other->Pi;
    ATP = other->ATP;
    Ks = other->Ks;
    Mgs = other->Mgs;
    Cls = other->Cls;
    Aip = other->Aip;
    U = other->U;
    An = other->An;
    Fdn = other->Fdn;
    BFHs = other->BFHs;
    BFHl = other->BFHl;
    PHs = other->PHs;
    PHl = other->PHl;
    NADPH = other->NADPH;
}

BFCon::BFCon(const arr &vec, const size_t offset) {
    fromArray(vec, offset);
}

void BFCon::_fromArray(const arr &vec, const size_t offset) {
    ISPHr = vec[offset];
    cytc1 = vec[offset + 1];
    ISPo = vec[offset + 2];
    ISPoQH2 = vec[offset + 3];
    QHsemi = vec[offset + 4];
    cytbL = vec[offset + 5];
    Qi = vec[offset + 6];
    Q = vec[offset + 7];
    cytbH = vec[offset + 8];
    Qn = vec[offset + 9];
    Qr = vec[offset + 10];
    QH2 = vec[offset + 11];
    cytc2 = vec[offset + 12];
    P700 = vec[offset + 13];
    ADP = vec[offset + 14];
    Pi = vec[offset + 15];
    ATP = vec[offset + 16];
    Ks = vec[offset + 17];
    Mgs = vec[offset + 18];
    Cls = vec[offset + 19];
    Aip = vec[offset + 20];
    U = vec[offset + 21];
    An = vec[offset + 22];
    Fdn = vec[offset + 23];
    BFHs = vec[offset + 24];
    BFHl = vec[offset + 25];
    PHs = vec[offset + 26];
    PHl = vec[offset + 27];
    NADPH = vec[offset + 28];
}

arr BFCon::_toArray() {
    arr vec = {ISPHr, cytc1, ISPo, ISPoQH2, QHsemi, cytbL, Qi, Q, cytbH, Qn, Qr, QH2, cytc2, P700, ADP, Pi, ATP, Ks, Mgs, Cls, Aip, U, An, Fdn, BFHs, BFHl, PHs, PHl, NADPH};
    return vec;
}
