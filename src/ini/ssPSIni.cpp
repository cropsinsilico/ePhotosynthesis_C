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

/*
  langmm: This module does not appear to be used anywhere else in the
    code and so is disabled for now until tests can be designed to verify
    that it is behaving correctly (however that may be).
*/

#ifdef ENABLE_ssPS

#include "Variables.hpp"
#include <math.h>
#include "modules/ssPS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double ssPS::GammaStar = 0.;
double ssPS::kmO2 = 0.;
double ssPS::kmCO2 = 0.;
void ssPS::ssPSIni(const double t, Variables *theVars) {
    const double RT = 0.0083 * (273. + t);

    GammaStar = exp(19.02 - 37.83 / RT);
    kmCO2 = exp(38.05 - 79.43 / RT);
    kmO2 = exp(20.30 - 36.38 / RT);
}

#endif
