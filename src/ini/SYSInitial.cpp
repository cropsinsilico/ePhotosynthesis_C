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

#include "Variables.hpp"
#include "modules/FIBF.hpp"
#include "globals.hpp"

// This routine initialze the parameters used for all the routines.

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

void ePhotosynthesis::SYSInitial(Variables *theVars) {
    const double input_LHCI = 6.;
    const double input_LHCII = 13.;
    const double input_PSI = 1;
    const double input_PSIIcore = 1;
    const double PSIIantennaSize = 37;
    const double PSIantennaSize = 95;

    theVars->PS12ratio = input_PSI / input_PSIIcore;

    FIBF::setChlT2(input_PSIIcore * (PSIIantennaSize + 13 * input_LHCII)); // U and A, PSII and LHCII
    FIBF::setChlT(PSIIantennaSize * input_PSIIcore);  // U , PSII
    FIBF::setChlPSI(input_PSI * (PSIantennaSize + 13 * input_LHCI));       // U and A of PSI, total Chl in PSI
}
