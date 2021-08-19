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
 Class for holding the results of RROEA_Rate calculations
 */
class RROEAVel {
public:
    RROEAVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEAVel object to copy
      */
    RROEAVel(const RROEAVel &other) {
        ve2GAPDH = other.ve2GAPDH;
        ve2FBPase = other.ve2FBPase;
        ve2SBPase = other.ve2SBPase;
        ve2PRK = other.ve2PRK;
        ve2ATPase = other.ve2ATPase;
        ve2ATPGPP = other.ve2ATPGPP;
        ve2MDH = other.ve2MDH;
        ve2Fd = other.ve2Fd;
        veFd2Thio = other.veFd2Thio;
        veFd2Calvin = other.veFd2Calvin;
        ve2RuACT = other.ve2RuACT;
    }
    double ve2GAPDH = 0.;
    double ve2FBPase = 0.;
    double ve2SBPase = 0.;
    double ve2PRK = 0.;
    double ve2ATPase = 0.;
    double ve2ATPGPP = 0.;
    double ve2MDH = 0.;
    double ve2Fd = 0.;
    double veFd2Thio = 0.;
    double veFd2Calvin = 0.;
    double ve2RuACT = 0.;

    friend std::ostream& operator<<(std::ostream& out, const RROEAVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
