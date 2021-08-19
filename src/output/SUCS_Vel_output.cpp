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

#include "vel/SUCSVel.hpp"

namespace ePhotosynthesis {
namespace vel {

std::ostream& operator<<(std::ostream& out, const SUCSVel &in) {
    out << "  v51 = " << in.v51 << std::endl;
    out << "  v52 = " << in.v52 << std::endl;
    out << "  v55 = " << in.v55 << std::endl;
    out << "  v56 = " << in.v56 << std::endl;
    out << "  v57 = " << in.v57 << std::endl;
    out << "  v58 = " << in.v58 << std::endl;
    out << "  v59 = " << in.v59 << std::endl;
    out << "  v60 = " << in.v60 << std::endl;
    out << "  v61 = " << in.v61 << std::endl;
    out << "  v62 = " << in.v62 << std::endl;
    out << "  vdhap_in = " << in.vdhap_in << std::endl;
    out << "  vgap_in = " << in.vgap_in << std::endl;
    out << "  vpga_in = " << in.vpga_in << std::endl;
    out << "  vpga_use = " << in.vpga_use << std::endl;
    out << "  vatpf = " << in.vatpf << std::endl;
    return out;
}

}  // namespace vel
}  // namespace ePhotosynthesis
