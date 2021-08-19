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
namespace RC {

/**
 Class for RROEA_RC data
 */
class RROEARC {
public:
    RROEARC() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEARC object to copy
      */
    RROEARC(const RROEARC &other) {
        ke2GAPDH = other.ke2GAPDH;
        ke2MDH = other.ke2MDH;
        ke2FBPase = other.ke2FBPase;
        ke2SBPase = other.ke2SBPase;
        ke2PRK = other.ke2PRK;
        ke2RubACT = other.ke2RubACT;
        ke2Fd = other.ke2Fd;
        keFd2Thio = other.keFd2Thio;
        keFd2Calvin = other.keFd2Calvin;
        ke2ATPGPP = other.ke2ATPGPP;
    }

    double ke2GAPDH = 0;
    double ke2MDH = 0;
    double ke2FBPase = 0;
    double ke2SBPase = 0;
    double ke2PRK = 0;
    double ke2RubACT = 0;
    double ke2Fd = 0;
    double keFd2Thio = 0;
    double keFd2Calvin = 0;
    double ke2ATPGPP = 0;
};

}  // namespace RC
}  // namespace ePhotosynthesis
