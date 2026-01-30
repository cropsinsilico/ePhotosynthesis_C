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
#include "PoolBase.hpp"

namespace ePhotosynthesis {
namespace pool {

/**
 Class for holding BF_Pool data
 */
class BFPool : public PoolBase<BFPool, MODULE_BF> {
public:
    DECLARE_VALUE_SET(BFPool, PoolBase<BFPool, MODULE_BF>)
    BFPool() :
      PoolBase<BFPool, MODULE_BF>() {
        initMembers();
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFPool object to copy
      */
    BFPool(const BFPool &other) : PoolBase<BFPool, MODULE_BF>(other) {
      initMembers();
      *this = other;
    }
    BFPool& operator=(const BFPool &other) {
        // The commented names were the names incorrectly used when the
        //   MATLAB code was first translated (they are actually values in
        //   FI::RC).
        Tcyt = other.Tcyt;     // kA_d
        Tcytc2 = other.Tcytc2; // kA_f
        TK = other.TK;         // kA_U
        TMg = other.TMg;       // kU_A
        TCl = other.TCl;       // kU_d
        TFd = other.TFd;       // kU_f
        TA = other.TA;         // k1
        TQ = other.TQ;         // k_r1
        BFTs = other.BFTs;     // kz
        BFTl = other.BFTl;     // k12
        P700T = other.P700T;   // k23
        NADPHT = other.NADPHT; // k30
	copyMembers(other);
	return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(BFPool);

}  // namespace pool
}  // namespace ePhotosynthesis
