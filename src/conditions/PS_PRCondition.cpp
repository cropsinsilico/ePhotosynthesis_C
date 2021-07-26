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

#include "conditions/PS_PRCondition.hpp"

const size_t PS_PRCondition::count = 21;
PS_PRCondition::PS_PRCondition(const PS_PRCondition* other) {
    _clear();
    PS_con = other->PS_con;
    PR_con = other->PR_con;
    PS_con->setParent(this);
    PR_con->setParent(this);
}

PS_PRCondition::PS_PRCondition(PSCondition* sother, PRCondition* rother) {
    _clear();
    PS_con = sother;
    PR_con = rother;
    PS_con->setParent(this);
    PR_con->setParent(this);
}

PS_PRCondition::PS_PRCondition(const arr &vec, const size_t offset) {
    _fromArray(vec, offset);
}

void PS_PRCondition::_fromArray(const arr &vec, const size_t offset) {
    if (PS_con == nullptr)
        PS_con = new PSCondition(this);
    if (PR_con  == nullptr)
        PR_con = new PRCondition(this);
    size_t counter = 0;
    PS_con->RuBP = vec[offset + counter++];
    PS_con->PGA = vec[offset + counter++];
    PS_con->DPGA = vec[offset + counter++];
    PS_con->T3P = vec[offset + counter++];
    PS_con->FBP = vec[offset + counter++];
    PS_con->E4P = vec[offset + counter++];
    PS_con->S7P = vec[offset + counter++];
    PS_con->SBP = vec[offset + counter++];
    PS_con->ATP = vec[offset + counter++];
    PS_con->HexP = vec[offset + counter++];
    PS_con->PenP = vec[offset + counter++];
    PR_con->GCEA = vec[offset + counter++];
    PR_con->GCA = vec[offset + counter++];
    PR_con->PGCA = vec[offset + counter++];
    PR_con->GCAc = vec[offset + counter++];
    PR_con->GOAc = vec[offset + counter++];
    PR_con->SERc = vec[offset + counter++];
    PR_con->GLYc = vec[offset + counter++];
    PR_con->HPRc = vec[offset + counter++];
    PR_con->GCEAc = vec[offset + counter++];
    //if (!useC3)
        PS_con->ADPG = vec[offset + counter++];
}

arr PS_PRCondition::_toArray() {
    /*if (useC3)
        return {PS_con->RuBP,
                  PS_con->PGA,
                  PS_con->DPGA,
                  PS_con->T3P,
                  PS_con->FBP,
                  PS_con->E4P,
                  PS_con->S7P,
                  PS_con->SBP,
                  PS_con->ATP,
                  PS_con->NADPH,
                  PS_con->CO2,
                  PS_con->O2,
                  PS_con->HexP,
                  PS_con->PenP,
                  PR_con->GCEA,
                  PR_con->GCA,
                  PR_con->PGCA,
                  PR_con->GCAc,
                  PR_con->GOAc,
                  PR_con->SERc,
                  PR_con->GLYc,
                  PR_con->HPRc,
                  PR_con->GCEAc};
                  */
    return {PS_con->RuBP,
                PS_con->PGA,
                PS_con->DPGA,
                PS_con->T3P,
                PS_con->FBP,
                PS_con->E4P,
                PS_con->S7P,
                PS_con->SBP,
                PS_con->ATP,
                PS_con->HexP,
                PS_con->PenP,
                PR_con->GCEA,
                PR_con->GCA,
                PR_con->PGCA,
                PR_con->GCAc,
                PR_con->GOAc,
                PR_con->SERc,
                PR_con->GLYc,
                PR_con->HPRc,
                PR_con->GCEAc,
                PS_con->ADPG};

}

void PS_PRCondition::_clear() {
    if (PS_con != nullptr) {
        delete PS_con;
        PS_con = nullptr;
    }
    if (PR_con != nullptr) {
        delete PR_con;
        PR_con = nullptr;
    }
}
