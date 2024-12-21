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
#include "globals.hpp"

using namespace ePhotosynthesis;

double ePhotosynthesis::TargetFunVal(const Variables *theVars) {
    if (!theVars->record) {
      if (theVars->RuACT_EPS_com)
	return (theVars->RuACT_Vel.v6_1 - theVars->PR_Vel.v131) * theVars->AVR;
      return (theVars->PS_Vel.v1 - theVars->PR_Vel.v131) * theVars->AVR;
    }


    const double PSVCOEFF = theVars->AVR;

    double ratio;
    if (theVars->VolRatioStCyto) {
        ratio = PSVCOEFF;
    } else {
        ratio = PSVCOEFF * 4. / 9.;
    }

    const double a = theVars->PS_VEL.getLastData().v1 * PSVCOEFF;
    const double b = theVars->PR_VEL.getLastData().v1in * ratio;

    const double CO2AR = a - b;

    return CO2AR;
}
