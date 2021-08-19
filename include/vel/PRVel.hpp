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

namespace ePhotosynthesis {
namespace vel {

/**
 Class for holding the result of the PR_Rate calculations
 */
class PRVel {
public:
    PRVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PRVel object to copy
      */
    PRVel(const PRVel &other) {
        v111 = other.v111;
        v112 = other.v112;
        v113 = other.v113;
        v121 = other.v121;
        v122 = other.v122;
        v123 = other.v123;
        v124 = other.v124;
        v131 = other.v131;
        v1in = other.v1in;
        v2out = other.v2out;
    }
    /**
      Overload of the multiplication-equals operator to multiply all members of
      the class by a constant

      @param val The constant to multiply all the members by
      */
    PRVel& operator*=(const double val) {
        this->v111 *= val;
        this->v112 *= val;
        this->v113 *= val;
        this->v121 *= val;
        this->v122 *= val;
        this->v123 *= val;
        this->v124 *= val;
        this->v131 *= val;
        this->v1in *= val;
        this->v2out *= val;
        return *this;
    }

    double v111 = 0.;
    double v112 = 0.;
    double v113 = 0.;
    double v121 = 0.;
    double v122 = 0.;
    double v123 = 0.;
    double v124 = 0.;
    double v131 = 0.;
    double v1in = 0.;
    double v2out = 0.;

    friend std::ostream& operator<<(std::ostream& out, const PRVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
