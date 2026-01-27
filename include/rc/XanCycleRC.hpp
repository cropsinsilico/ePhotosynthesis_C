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

#include "../definitions.hpp"
#include "RCBase.hpp"

namespace ePhotosynthesis {
namespace RC {

/**
 Class for XanCycle_RC data
 */
class XanCycleRC : public RCBase<XanCycleRC, MODULE_XanCycle> {
public:
    DECLARE_VALUE_SET(XanCycleRC, RCBase<XanCycleRC, MODULE_XanCycle>)
    XanCycleRC() : RCBase<XanCycleRC, MODULE_XanCycle>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The XanCycleRC object to copy
      */
    XanCycleRC(const XanCycleRC &other) : RCBase<XanCycleRC, MODULE_XanCycle>(other) {
      initMembers();
      *this = other;
    }
    XanCycleRC& operator=(const XanCycleRC &other) {
        kav = other.kav;
        kaz = other.kaz;
        kva = other.kva;
        kza = other.kza;
        hill_psbs = other.hill_psbs;
        pK_psbs = other.pK_psbs;
        hill_vde = other.hill_vde;
        pK_vde = other.pK_vde;
        kvde_max = other.kvde_max;
        k_ze = other.k_ze;
        Fpsbs = other.Fpsbs;
        psbsQ_converRate = other.psbsQ_converRate;
	copyMembers(other);
	return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(XanCycleRC);

}  // namespace RC
}  // namespace ePhotosynthesis
