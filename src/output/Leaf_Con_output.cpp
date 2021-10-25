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

#include "conditions/LeafCondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& LeafCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "LeafCondition" << std::endl;
    out << space << "  Ci = " << Ci << std::endl;
    out << space << "  Cb = " << Cb << std::endl;
    out << space << "  Eb = " << Eb << std::endl;
    out << space << "  Gs = " << Gs << std::endl;
    out << space << "  Tleaf = " << Tleaf << std::endl;
    out << space << "  H2Oou = " << H2Oou << std::endl;
    out << space << "  CO2in = " << CO2in << std::endl;

    return out;
}
