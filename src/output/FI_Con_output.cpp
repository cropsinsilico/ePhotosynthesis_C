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

std::ostream& FICondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');

    out << space << "FICon" << std::endl;
    out << space << "  A = " << A << std::endl;
    out << space << "  U = " << U << std::endl;
    out << space << "  P680ePheo = " << P680ePheo << std::endl;
    out << space << "  P680pPheon = " << P680pPheon<< std::endl;
    out << space << "  P680pPheo = " << P680pPheo << std::endl;
    out << space << "  P680Pheon = " << P680Pheon << std::endl;
    out << space << "  Yz = " << Yz<< std::endl;
    out << space << "  S1T = " << S1T << std::endl;
    out << space << "  S2T = " << S2T << std::endl;
    out << space << "  S3T = " << S3T << std::endl;
    out << space << "  S0T = " << S0T << std::endl;
    out << space << "  S1Tp = " << S1Tp<< std::endl;
    out << space << "  S2Tp = " << S2Tp<< std::endl;
    out << space << "  S3Tp = " << S3Tp<< std::endl;
    out << space << "  S0Tp = " << S0Tp<< std::endl;
    out << space << "  QAQB = " << QAQB<< std::endl;
    out << space << "  QAnQB = " << QAnQB << std::endl;
    out << space << "  QAQBn = " << QAQBn << std::endl;
    out << space << "  QAnQBn = " << QAnQBn<< std::endl;
    out << space << "  QAQB2n = " << QAQB2n<< std::endl;
    out << space << "  QAnQB2n = " << QAnQB2n << std::endl;
    out << space << "  PQn = " << PQn << std::endl;
    return out;
}
