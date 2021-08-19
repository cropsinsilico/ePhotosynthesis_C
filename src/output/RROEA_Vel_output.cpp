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

#include "vel/RROEAVel.hpp"

namespace ePhotosynthesis {
namespace vel {

std::ostream& operator<<(std::ostream& out, const RROEAVel &in) {
    out << "RROEAVel" << std::endl;
    out << "  ve2GAPDH = " << in.ve2GAPDH << std::endl;
    out << "  ve2FBPase = " << in.ve2FBPase << std::endl;
    out << "  ve2SBPase = " << in.ve2SBPase << std::endl;
    out << "  ve2PRK = " << in.ve2PRK << std::endl;
    out << "  ve2ATPase = " << in.ve2ATPase << std::endl;
    out << "  ve2ATPGPP = " << in.ve2ATPGPP << std::endl;
    out << "  ve2MDH = " << in.ve2MDH << std::endl;
    out << "  ve2Fd = " << in.ve2Fd << std::endl;
    out << "  veFd2Thio = " << in.veFd2Thio << std::endl;
    out << "  veFd2Calvin = " << in.veFd2Calvin << std::endl;
    out << "  ve2RuACT = " << in.ve2RuACT << std::endl;
    return out;
}

}  // namespace vel
}  // namespace ePhotosynthesis
