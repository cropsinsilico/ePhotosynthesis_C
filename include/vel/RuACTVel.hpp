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

/**
 Class for holding the results of RuACT_Rate calculations
 */
class RuACTVel {
public:
    RuACTVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTVel object to copy
      */
    RuACTVel(const RuACTVel &other) {
        v1 = other.v1;
        vn1 = other.vn1;
        v7 = other.v7;
        vn7 = other.vn7;
        v6_1 = other.v6_1;
        v6_2 = other.v6_2;
    }
    double v1 = 0.;
    double vn1 = 0.;
    double v7 = 0.;
    double vn7 = 0.;
    double v6_1 = 0.;
    double v6_2 = 0.;
    friend std::ostream& operator<<(std::ostream& out, const RuACTVel &in);
    friend std::ostream& operator<<(std::ostream& out, const RuACTVel *in);
};
