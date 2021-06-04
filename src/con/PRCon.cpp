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

#include "con/PRCon.hpp"

PRCon::PRCon(const PRCon *other)  {
    GCEA = other->GCEA;
    GCA = other->GCA;
    PGA = other->PGA;
    PGCA = other->PGCA;
    GCAc = other->GCAc;
    GOAc = other->GOAc;
    SERc = other->SERc;
    GLYc = other->GLYc;
    HPRc = other->HPRc;
    GCEAc = other->GCEAc;
    RuBP = other->RuBP;
    CO2 = other->CO2;
    O2 = other->O2;
}

PRCon::PRCon(const arr vec, size_t offset)  {
    _fromArray(vec, offset);
}

void PRCon::_fromArray(const arr &vec, size_t offset)  {
    GCEA= vec[offset];
    GCA= vec[offset + 1];
    PGA= vec[offset + 2];
    PGCA= vec[offset + 3];
    GCAc= vec[offset + 4];
    GOAc= vec[offset + 5];
    SERc= vec[offset + 6];
    GLYc= vec[offset + 7];
    HPRc= vec[offset + 8];
    GCEAc= vec[offset + 9];
    RuBP= vec[offset + 10];
    CO2= vec[offset + 11];
    O2= vec[offset + 12];
}

arr PRCon::_toArray()  {
    arr array = {GCEA, GCA, PGA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc, RuBP, CO2, O2};
    return array;
}
