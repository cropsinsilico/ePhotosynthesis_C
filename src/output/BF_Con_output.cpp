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
using namespace ePhotosynthesis::conditions;

std::ostream& BFCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "BFCon" << std::endl;
    out << space << "  ISPHr = " << ISPHr << std::endl;
    out << space << "  cytc1 = " << cytc1 << std::endl;
    out << space << "  ISPo = " << ISPo<< std::endl;
    out << space << "  ISPoQH2 = " << ISPoQH2 << std::endl;
    out << space << "  QHsemi = " << QHsemi<< std::endl;
    out << space << "  cytbL = " << cytbL << std::endl;
    out << space << "  Qi = " << Qi<< std::endl;
    if (!BFCondition::FI_connect)
        out << space << "  Q = " << Q << std::endl;
    out << space << "  cytbH = " << cytbH << std::endl;
    out << space << "  Qn = " << Qn<< std::endl;
    out << space << "  Qr = " << Qr<< std::endl;
    out << space << "  QH2 = " << QH2 << std::endl;
    out << space << "  cytc2 = " << cytc2 << std::endl;
    out << space << "  P700 = " << P700<< std::endl;
    out << space << "  ADP = " << ADP << std::endl;
    if (!PS_connect)
        out << space << "  ATP = " << ATP << std::endl;
    out << space << "  Ks = " << Ks<< std::endl;
    out << space << "  Mgs = " << Mgs << std::endl;
    out << space << "  Cls = " << Cls << std::endl;
    out << space << "  Aip = " << Aip << std::endl;
    out << space << "  U = " << U << std::endl;
    out << space << "  An = " << An<< std::endl;
    out << space << "  Fdn = " << Fdn << std::endl;
    out << space << "  BFHs = " << BFHs<< std::endl;
    out << space << "  BFHl = " << BFHl<< std::endl;
    out << space << "  PHs = " << PHs << std::endl;
    out << space << "  PHl = " << PHl << std::endl;
    out << space << "  NADPH = " << NADPH << std::endl;
    return out;
}
