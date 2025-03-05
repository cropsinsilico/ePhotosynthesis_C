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
 Class for holding the results of SUCS_Rate calculations
 */
class SUCSVel : public VelBase<SUCSVel, MODULE_SUCS> {
public:
    DECLARE_VALUE_SET(SUCSVel, VelBase<SUCSVel, MODULE_SUCS>)
    SUCSVel() : VelBase<SUCSVel, MODULE_SUCS>() {
	initMembers();
    }
    SUCSVel(const SUCSVel &other) : VelBase<SUCSVel, MODULE_SUCS>(other) {
	initMembers();
        v51 = other.v51;
        v52 = other.v52;
        v55 = other.v55;
        v56 = other.v56;
        v57 = other.v57;
        v58 = other.v58;
        v59 = other.v59;
        v60 = other.v60;
        v61 = other.v61;
        v62 = other.v62;
        vdhap_in = other.vdhap_in;
        vgap_in = other.vgap_in;
        vpga_in = other.vpga_in;
        vpga_use = other.vpga_use;
        vatpf = other.vatpf;
	copyMembers(other);
    }

    friend std::ostream& operator<<(std::ostream& out, const SUCSVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
