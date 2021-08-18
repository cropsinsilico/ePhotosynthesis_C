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

#include "vel/BFVel.hpp"

using namespace ePhotosynthesis::vel;

std::ostream& operator<<(std::ostream &out, const BFVel &in) {
    out << "BFVel" << std::endl;
    out << "  Vbf1 = " << in.Vbf1 << std::endl;
    out << "  Vbf2 = " << in.Vbf2 << std::endl;
    out << "  Vbf3 = " << in.Vbf3 << std::endl;
    out << "  Vbf4 = " << in.Vbf4 << std::endl;
    out << "  Vbf5 = " << in.Vbf5 << std::endl;
    out << "  Vbf6 = " << in.Vbf6 << std::endl;
    out << "  Vbf7 = " << in.Vbf7 << std::endl;
    out << "  Vbf8 = " << in.Vbf8 << std::endl;
    out << "  Vbf9 = " << in.Vbf9 << std::endl;
    out << "  Vbf10= " << in.Vbf10<< std::endl;
    out << "  Vbf11= " << in.Vbf11<< std::endl;
    out << "  Vqi= " << in.Vqi<< std::endl;
    out << "  Vipc = " << in.Vipc << std::endl;
    out << "  Vicp = " << in.Vicp << std::endl;
    out << "  Vinc = " << in.Vinc << std::endl;
    out << "  Vinp = " << in.Vinp << std::endl;
    out << "  Vdp= " << in.Vdp<< std::endl;
    out << "  Vdc= " << in.Vdc<< std::endl;
    out << "  Vfp= " << in.Vfp<< std::endl;
    out << "  Vfc= " << in.Vfc<< std::endl;
    out << "  Vsfd = " << in.Vsfd << std::endl;
    out << "  VsATP= " << in.VsATP<< std::endl;
    out << "  VgPQH2 = " << in.VgPQH2 << std::endl;
    out << "  JKc= " << in.JKc<< std::endl;
    out << "  JMgc = " << in.JMgc << std::endl;
    out << "  JClc = " << in.JClc << std::endl;
    out << "  Vbf15= " << in.Vbf15<< std::endl;
    out << "  Vbf16= " << in.Vbf16<< std::endl;
    out << "  vbfn2= " << in.vbfn2<< std::endl;
    out << "  VsNADPH= " << in.VsNADPH<< std::endl;
    out << "  vcet = " << in.vcet << std::endl;

    return out;
}
