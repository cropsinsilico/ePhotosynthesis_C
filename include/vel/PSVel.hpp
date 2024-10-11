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
 Class for holding the results of PS_Rate calculations
 */
class PSVel : public VelBase<PSVel, MODULE_PS> {
public:
    DECLARE_VALUE_SET(PSVel, VelBase<PSVel, MODULE_PS>)
    PSVel() : VelBase<PSVel, MODULE_PS>() {
	initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PSVel object to copy
      */
    PSVel(const PSVel &other) : VelBase<PSVel, MODULE_PS>(other) {
	initMembers();
        v1 = other.v1;
        v2 = other.v2;
        v3 = other.v3;
        v4 = other.v4;
        v5 = other.v5;
        v6 = other.v6;
        v7 = other.v7;
        v8 = other.v8;
        v9 = other.v9;
        v10 = other.v10;
        v13 = other.v13;
        v16 = other.v16;
        v23 = other.v23;
        v31 = other.v31;
        v32 = other.v32;
        v33 = other.v33;
        Pi = other.Pi;
        v24 = other.v24;
        v25 = other.v25;
	copyMembers(other);
    }
    /**
      Overloaded time-equal operator to multiply all data mambers by a constant value

      @param val The value to multiply the data members by.
      */
    PSVel& operator*=(const double val) {
        this->v1 *= val;
        this->v2 *= val;
        this->v3 *= val;
        this->v4 *= val;
        this->v5 *= val;
        this->v6 *= val;
        this->v7 *= val;
        this->v8 *= val;
        this->v9 *= val;
        this->v10 *= val;
        this->v13 *= val;
        this->v16 *= val;
        this->v23 *= val;
        this->v31 *= val;
        this->v32 *= val;
        this->v33 *= val;
        this->Pi *= val;
        this->v24 *= val;
        this->v25 *= val;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const PSVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
