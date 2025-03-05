#pragma once

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

#include "definitions.hpp"
#include "vel/VelBase.hpp"

namespace ePhotosynthesis {
namespace vel {

/**
 Class for holding the result of the BF_Rate calculations
 */
class BFVel : public VelBase<BFVel, MODULE_BF> {
public:
    DECLARE_VALUE_SET(BFVel, VelBase<BFVel, MODULE_BF>)
    BFVel() : VelBase<BFVel, MODULE_BF>() {
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFVel object to copy
      */
    BFVel(const BFVel &other) : VelBase<BFVel, MODULE_BF>(other) {
	initMembers();
        Vbf1 = other.Vbf1;
        Vbf2 = other.Vbf2;
        Vbf3 = other.Vbf3;
        Vbf4 = other.Vbf4;
        Vbf5 = other.Vbf5;
        Vbf6 = other.Vbf6;
        Vbf7 = other.Vbf7;
        Vbf8 = other.Vbf8;
        Vbf9 = other.Vbf9;
        Vbf10 = other.Vbf10;
        Vbf11 = other.Vbf11;
        Vqi = other.Vqi;
        Vipc = other.Vipc;
        Vicp = other.Vicp;
        Vinc = other.Vinc;
        Vinp = other.Vinp;
        Vdp = other.Vdp;
        Vdc = other.Vdc;
        Vfp = other.Vfp;
        Vfc = other.Vfc;
        Vsfd = other.Vsfd;
        VsATP = other.VsATP;
        VgPQH2 = other.VgPQH2;
        JKc = other.JKc;
        JMgc = other.JMgc;
        JClc = other.JClc;
        Vbf15 = other.Vbf15;
        Vbf16 = other.Vbf16;
        vbfn2 = other.vbfn2;
        VsNADPH = other.VsNADPH;
        vcet = other.vcet;
	copyMembers(other);
    }

    friend std::ostream& operator<<(std::ostream& out, const BFVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
