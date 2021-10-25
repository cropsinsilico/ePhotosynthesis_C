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

#include "vel/LeafVel.hpp"

namespace ePhotosynthesis {
namespace vel {
std::ostream& operator<<(std::ostream &out, const LeafVel &in) {
    out << "LeafVel" << std::endl;
    out << "  NetAssimilation = " << in.NetAssimilation << std::endl;
    out << "  vCO2b = " << in.vCO2b << std::endl;
    out << "  vCO2s = " << in.vCO2s << std::endl;
    out << "  vH2Ob = " << in.vH2Ob << std::endl;
    out << "  vH2Os = " << in.vH2Os << std::endl;
    out << "  EnergyBalanceResidual = " << in.EnergyBalanceResidual << std::endl;
    out << "  vCO2total = " << in.vCO2total << std::endl;
    out << "  vH2Ototal = " << in.vH2Ototal << std::endl;
    out << "  vgs = " << in.vgs << std::endl;
    out << "  vleak = " << in.vleak << std::endl;
    out << "  Gs = " << in.Gs << std::endl;
    out << "  Cb = " << in.Cb << std::endl;
    out << "  Ci = " << in.Ci << std::endl;
    out << "  Gbw = " << in.Gbw << std::endl;
    return out;
}

}
}
