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
 Class to hold the results of the XanCycleRate calculations
 */
class XanCycleVel {
public:
    XanCycleVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The XanCycleVel object to copy
      */
    XanCycleVel(const XanCycleVel &other) {
        Vva = other.Vva;
        Vaz = other.Vaz;
        Vza = other.Vza;
        Vav = other.Vav;
        Vvf = other.Vvf;
        Vv2ABA = other.Vv2ABA;
        VABAdg = other.VABAdg;
    }
    double Vva = 0.;    ///< The velocity of v to a conversion
    double Vaz = 0.;    ///< The rate of A to z
    double Vza = 0.;    ///< THe rate of z to a
    double Vav = 0.;    ///< The rate of A to V
    double Vvf = 0.;    ///< The rate of V formation
    double Vv2ABA = 0.; ///< The rate of conversion from v to ABA.
    double VABAdg = 0.; ///< The rate of ABA degradation

    friend std::ostream& operator<<(std::ostream& out, const XanCycleVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
