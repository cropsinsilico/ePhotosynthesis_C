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

#include "vel/PRVel.hpp"

std::ostream& operator<<(std::ostream &out, const PRVel &in) {
    out << "PRVel" << std::endl;
    out << "  v111 = " << in.v111 << std::endl;
    out << "  v112 = " << in.v112 << std::endl;
    out << "  v113 = " << in.v113 << std::endl;
    out << "  v121 = " << in.v121 << std::endl;
    out << "  v122 = " << in.v122 << std::endl;
    out << "  v123 = " << in.v123 << std::endl;
    out << "  v124 = " << in.v124 << std::endl;
    out << "  v131 = " << in.v131 << std::endl;
    out << "  v1in = " << in.v1in << std::endl;
    out << "  v2out = " << in.v2out << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream &out, const PRVel* in) {
    out << *in;
    return out;
}

