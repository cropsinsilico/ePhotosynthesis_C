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

#include "conditions/BFCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

DEFINE_VALUE_SET_SRC(BFCondition);

BFCondition::BFCondition(const BFCondition* const other) :
  ConditionBase<BFCondition, FIBFCondition, MODULE_BF>(other) {
    initMembers();
    ISPHr = other->ISPHr;
    cytc1 = other->cytc1;
    ISPo = other->ISPo;
    ISPoQH2 = other->ISPoQH2;
    QHsemi = other->QHsemi;
    cytbL = other->cytbL;
    Qi = other->Qi;
    if (!FI_connect)
        Q = other->Q;
    cytbH = other->cytbH;
    Qn = other->Qn;
    Qr = other->Qr;
    QH2 = other->QH2;
    cytc2 = other->cytc2;
    P700 = other->P700;
    ADP = other->ADP;
    if (!PS_connect)
        ATP = other->ATP;
    Ks = other->Ks;
    Mgs = other->Mgs;
    Cls = other->Cls;
    Aip = other->Aip;
    U = other->U;
    An = other->An;
    if (!RROEA_connect)
        Fdn = other->Fdn;
    BFHs = other->BFHs;
    BFHl = other->BFHl;
    PHs = other->PHs;
    PHl = other->PHl;
    NADPH = other->NADPH;
    copyMembers(*other);
}

BFCondition::BFCondition(const arr &vec, const std::size_t offset) {
    initMembers();
    fromArray(vec, offset);
}

void BFCondition::_fromArray(const arr &vec, const std::size_t offset) {
    std::size_t count = 0;
    ISPHr = vec[offset + count++];
    cytc1 = vec[offset + count++];
    ISPo = vec[offset + count++];
    ISPoQH2 = vec[offset + count++];
    QHsemi = vec[offset + count++];
    cytbL = vec[offset + count++];
    Qi = vec[offset + count++];
    if (!FI_connect)
        Q = vec[offset + count++];
    cytbH = vec[offset + count++];
    Qn = vec[offset + count++];
    Qr = vec[offset + count++];
    QH2 = vec[offset + count++];
    cytc2 = vec[offset + count++];
    P700 = vec[offset + count++];
    ADP = vec[offset + count++];
    if (!PS_connect)
        ATP = vec[offset + count++];
    Ks = vec[offset + count++];
    Mgs = vec[offset + count++];
    Cls = vec[offset + count++];
    Aip = vec[offset + count++];
    U = vec[offset + count++];
    An = vec[offset + count++];
    if (!RROEA_connect)
        Fdn = vec[offset + count++];
    BFHs = vec[offset + count++];
    BFHl = vec[offset + count++];
    PHs = vec[offset + count++];
    PHl = vec[offset + count++];
    NADPH = vec[offset + count++];
}

arr BFCondition::_toArray() const {
    arr output = {ISPHr, cytc1, ISPo, ISPoQH2, QHsemi, cytbL, Qi};
    arr output2 = {cytbH, Qn, Qr, QH2, cytc2, P700, ADP};
    arr output3 = {Ks, Mgs, Cls, Aip, U, An};
    arr output4 = {BFHs, BFHl, PHs, PHl, NADPH};
    if (!FI_connect)
        output.push_back(Q);
    output.insert(output.end(),output2.begin(), output2.end());
    if (!PS_connect)
        output.push_back(ATP);
    output.insert(output.end(), output3.begin(), output3.end());
    if (!RROEA_connect)
        output.push_back(Fdn);
    output.insert(output.end(), output4.begin(), output4.end());
    return output;
}
