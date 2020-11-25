#include "Variables.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This routine initialze the parameters used for all the routines.

void SYSInitial(Variables *myVars) {

    myVars->PS12ratio = myVars->input_PSI / myVars->input_PSIIcore;

    myVars->ChlT2 = myVars->input_PSIIcore * (myVars->PSIIantennaSize + 13 * myVars->input_LHCII);// U and A, PSII and LHCII
    myVars->ChlT = myVars->PSIIantennaSize * myVars->input_PSIIcore;// U , PSII
    myVars->ChlPSI = myVars->input_PSI * (myVars->PSIantennaSize + 13 * myVars->input_LHCI);// U and A of PSI, total Chl in PSI
}
