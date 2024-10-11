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
#include "VelBase.hpp"

namespace ePhotosynthesis {
namespace vel {

/**
 Class to hold the results of the XanCycleRate calculations
 */
class XanCycleVel : VelBase<XanCycleVel, MODULE_XanCycle> {
public:
    DECLARE_VALUE_SET(XanCycleVel, VelBase<XanCycleVel, MODULE_XanCycle>)
    XanCycleVel() : VelBase<XanCycleVel, MODULE_XanCycle> () {
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The XanCycleVel object to copy
      */
    XanCycleVel(const XanCycleVel &other) : VelBase<XanCycleVel, MODULE_XanCycle>(other) {
	initMembers();
        Vva = other.Vva;
        Vaz = other.Vaz;
        Vza = other.Vza;
        Vav = other.Vav;
        Vvf = other.Vvf;
        Vv2ABA = other.Vv2ABA;
        VABAdg = other.VABAdg;
	copyMembers(other);
    }

    friend std::ostream& operator<<(std::ostream& out, const XanCycleVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
