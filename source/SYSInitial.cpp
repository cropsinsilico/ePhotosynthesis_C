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

// This routine initialze the parameters used for all the routines.

void SYSInitial(Variables *theVars) {

    theVars->PS12ratio = theVars->input_PSI / theVars->input_PSIIcore;

    theVars->ChlT2 = theVars->input_PSIIcore * (theVars->PSIIantennaSize + 13 * theVars->input_LHCII); // U and A, PSII and LHCII
    theVars->ChlT = theVars->PSIIantennaSize * theVars->input_PSIIcore;  // U , PSII
    theVars->ChlPSI = theVars->input_PSI * (theVars->PSIantennaSize + 13 * theVars->input_LHCI);       // U and A of PSI, total Chl in PSI
}
