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

#include "conditions/PSCondition.hpp"

using namespace ePhotosynthesis::conditions;

std::ostream& PSCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "PSContainer" << std::endl;
    out << space << "  RuBP = " << RuBP<< std::endl;
    out << space << "  PGA = " << PGA << std::endl;
    out << space << "  DPGA = " << DPGA<< std::endl;
    out << space << "  T3P = " << T3P << std::endl;
    out << space << "  ADPG = " << ADPG<< std::endl;
    out << space << "  FBP = " << FBP << std::endl;
    out << space << "  E4P = " << E4P << std::endl;
    out << space << "  S7P = " << S7P << std::endl;
    out << space << "  SBP = " << SBP << std::endl;
    out << space << "  ATP = " << ATP << std::endl;
    out << space << "  HexP = " << HexP<< std::endl;
    out << space << "  PenP = " << PenP<< std::endl;

    return out;
}

