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

#include "conditions/PRCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

PRCondition::PRCondition(const PRCondition* const other)  {
    GCEA = other->GCEA;
    GCA = other->GCA;
    PGCA = other->PGCA;
    GCAc = other->GCAc;
    GOAc = other->GOAc;
    SERc = other->SERc;
    GLYc = other->GLYc;
    HPRc = other->HPRc;
    GCEAc = other->GCEAc;
    if (!PS_RuBP)
        RuBP = other->RuBP;
}

PRCondition::PRCondition(const arr vec, const std::size_t offset)  {
    fromArray(vec, offset);
}

void PRCondition::_fromArray(const arr &vec, const std::size_t offset)  {
    GCEA= vec[offset];
    GCA= vec[offset + 1];
    PGCA= vec[offset + 2];
    GCAc= vec[offset + 3];
    GOAc= vec[offset + 4];
    SERc= vec[offset + 5];
    GLYc= vec[offset + 6];
    HPRc= vec[offset + 7];
    GCEAc= vec[offset + 8];
    if (!PS_RuBP)
        RuBP= vec[offset + 9];
}

arr PRCondition::_toArray() const {
    if (PS_RuBP)
        return {GCEA, GCA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc};
    return {GCEA, GCA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc, RuBP};
}
