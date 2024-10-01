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
namespace RC {

// class for holding FIRC data
class FIRC : public ValueSet<MODULE_FI, PARAM_TYPE_RC> {
public:
    DECLARE_VALUE_SET(FIRC, ValueSet<MODULE_FI, PARAM_TYPE_RC>)
    FIRC() : ValueSet<MODULE_FI, PARAM_TYPE_RC>() {
        initMembers();
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FIRC object to copy
      */
    FIRC(const FIRC &other) : ValueSet<MODULE_FI, PARAM_TYPE_RC>(other) {
      initMembers();
      *this = other;
    }
    FIRC& operator=(const FIRC &other) {
        kA_d = other.kA_d;  //  The rate constant of heat dissipation from peripheral antenna  Lazar (1999), 0.25~1 *10^(9)
        kA_f = other.kA_f;  //  The rate constant of fluorescence emission from peripheral antenna  Lazar 1999, with a lifetime of 5 ns at closed reaction center
        kA_U = other.kA_U;  //  The rate constant of exciton transfer from periphral antenna to core antenna  Reference needed, a guess
        kU_A = other.kU_A;  //  The rate constant of exciton transfer from core antenna to peripheral antenna  Reference needed, a guess
        kU_d = other.kU_d;  //  The rate constant of  heat emission from core antenna
        kU_f = other.kU_f;  //  The rate constant of fluorescence emission from core antenna
        k1 = other.k1;      //  The rate constant of primary charge separation for open reaction center
        k_r1 = other.k_r1;  //  The rate constant of charge recombination for open reactoin center
        kz = other.kz;      //  The rate constant of the Tyrosine oxidation  Lazar (1999); 3.8~50 * 10^6
        k12 = other.k12;    //  The rate constant of the S1 to S2 transition  Lazar (1999); 0.667~33.3 * 10^3
        k23 = other.k23;    //  The rate constant of the S2 to S3 transition  Lazar (1999); 0.667~33.3 * 10^3
        k30 = other.k30;    //  The rate constant of the S3 to S0 transition  Lazar (1999); 0.667~33.3 * 10^3
        k01 = other.k01;    //  The rate constant of the S0 to S1 transition  Lazar (1999); 0.667~33.3 * 10^3
        k2 = other.k2;      //  The rate constant of the QA reduction by Pheo-  Lazar (1999); 2~2.3 * 10^9
        kAB1 = other.kAB1;  //  The rate constant of QAQB-->QAQB-  Lazar (1999); 2.5~5 * 10^3
        kBA1 = other.kBA1;  //  The rate constant of the QAQB- -->QAQB  Lazar (1999)
        kAB2 = other.kAB2;  //  The rate constant of the QAQB- --> QAQB2-  Lazar (1999); 1.25~3.33 * 10^3
        kBA2 = other.kBA2;  //  The rate constant of the QAQB2- --> QAQB-   Lazar (1999), or same as kAB2 depend on the equilibium constant
        k3 = other.k3;      //  The rate constant of the exchange of PQ and QBH2  Lazar (1999),0.12~1 for the fast PQ pool,  or 3~8 for the slow recycling PQ pool
        k_r3 = other.k_r3;  //  The rate constant of the exchange of QB and PQH2  Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
        k_pq_oxy = other.k_pq_oxy;//  The rate constant of the PQH2 oxidation  Lazar (1999),50~500
	copyMembers(other);
	return *this;
    }

};

  DEFINE_VALUE_SET_HEADER(FIRC);

}  // namespace RC
}  // namespace ePhotosynthesis
