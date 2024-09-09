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
#include "ValueSet.hpp"

namespace ePhotosynthesis {
namespace pool {

/**
 Class for RROEA_Pool data
 */
class RROEAPool : public ValueSet<MODULE_RROEA, PARAM_TYPE_POOL> {
public:
    RROEAPool() :
      ValueSet<MODULE_RROEA, PARAM_TYPE_POOL>() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEAPool object to copy
      */
    RROEAPool(const RROEAPool &other) :
      ValueSet<MODULE_RROEA, PARAM_TYPE_POOL>(other) {

        GAPDH = other.GAPDH;
        FBPase = other.FBPase;
        SBPase = other.SBPase;
        PRK = other.PRK;
        ATPase = other.ATPase;
        ATPGPP = other.ATPGPP;
        MDH = other.MDH;
        ThioT = other.ThioT;
        FdT = other.FdT;
        RuACTT = other.RuACTT;
    }

    double GAPDH = 0.;
    double FBPase = 0.;
    double SBPase = 0.;
    double PRK = 0.;
    double ATPase = 0.;
    double ATPGPP = 0.;
    double MDH = 0.;
    double ThioT = 0.;
    double FdT = 0.;
    double RuACTT = 0.;
};

}  // namespace pool
}  // namespace ePhotosynthesis
