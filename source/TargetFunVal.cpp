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

double TargetFunVal(Variables *theVars) {
    if (!theVars->record)
        return 0.;


    const double PSVCOEFF = 30;

    for (size_t i = 1; i < theVars->PS_VEL.size(); i++)
        theVars->PS_VEL[i] *= PSVCOEFF;

    //PSVEL = PS_VEL';// --unused
    double ratio;
    if (theVars->VolRatioStCyto == 1) {
        ratio = PSVCOEFF;
    } else {
        ratio = PSVCOEFF * 4 / 9;
    }
    for (size_t i = 0; i < theVars->PR_VEL.size(); i++)
        theVars->PR_VEL[i] *= ratio;



    //const size_t n = theVars->PS_VEL.size();
    const double a = theVars->PS_VEL.getLastData().v1; // [1][n];
    const double b = theVars->PR_VEL.getLastData().v1in;

    const double CO2AR = a - b;

    return CO2AR;
}
