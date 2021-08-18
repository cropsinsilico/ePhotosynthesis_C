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
 Class for RuACT_RC data
 */
class RuACTRC {
public:
    RuACTRC() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTRC object to copy
      */
    RuACTRC(const RuACTRC &other) {

        k1 = other.k1;
        kn1 = other.kn1;
        km1 = other.km1;
        Ke2 = other.Ke2;
        Ke3 = other.Ke3;
        k6 = other.k6;
        kc = other.kc;
        ko = other.ko;
        k7 = other.k7;
        kr = other.kr;
    }

    double k1 = 0;
    double kn1 = 0; ///< The rate constant of E inactivation by binding of RuBP; Lazar 1999, with a lifetime of 5 ns at closed reaction center
    double km1 = 0;
    double Ke2 = 0; ///< Data from Mate et al 1996. Unit: micormolar; Reference needed, a guess
    double Ke3 = 0; ///< Data from Mate et al 1996. Unit: micormolar;
    double k6 = 0;  ///< micromolar per meter square per second, transfered to unit
    double kc = 0;  ///< Michaelis menton constant for CO2
    double ko = 0;  ///< Michaelis menton constant for O2
    double k7 = 0;  ///< The rate constant for ecm to ecmr
    double kr = 0;
};

}  // namespace RC
}  // namespace ePhotosynthesis
