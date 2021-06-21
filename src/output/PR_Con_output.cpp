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

#include "containers/PRContainer.hpp"

std::ostream& PRContainer::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "PRContainer" << std::endl;
    out << space << "  GCEA = " << GCEA<< std::endl;
    out << space << "  GCA = " << GCA << std::endl;
    out << space << "  PGA = " << PGA << std::endl;
    out << space << "  PGCA = " << PGCA<< std::endl;
    out << space << "  GCAc = " << GCAc<< std::endl;
    out << space << "  GOAc = " << GOAc<< std::endl;
    out << space << "  SERc = " << SERc<< std::endl;
    out << space << "  GLYc = " << GLYc<< std::endl;
    out << space << "  HPRc = " << HPRc<< std::endl;
    out << space << "  GCEAc = " << GCEAc << std::endl;
    out << space << "  RUBP = " << RuBP<< std::endl;
    out << space << "  CO2 = " << CO2 << std::endl;
    out << space << "  O2 = " << O2<< std::endl;
    return out;
}

