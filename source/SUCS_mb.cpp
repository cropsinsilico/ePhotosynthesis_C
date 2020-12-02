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

#include "globals.hpp"
#include "Variables.hpp"

arr SUCS::SUCS_Mb(double t, SUCSCon &SUCS_Con, Variables *theVars) {
    Condition(t, theVars);

    theVars->SUCS_Param[0] = theVars->GLight;

    arr SUCS_Vel = zeros(15);
    SUCS_Rate(t, SUCS_Con, theVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation//
    ////////////////////////////////////////////////////////////////

    // The Major Variables
    arr SUCS_mb = zeros(12);
    SUCS_mb[0] = theVars->SUCS_Vel.vdhap_in + theVars->SUCS_Vel.vgap_in - 2 * theVars->SUCS_Vel.v51;//	T3Pc
    SUCS_mb[1] = theVars->SUCS_Vel.v51 - theVars->SUCS_Vel.v52;//	FBPc
    SUCS_mb[2] = theVars->SUCS_Vel.v52 - theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v59 + theVars->SUCS_Vel.v58 - theVars->SUCS_Vel.v56;//	HexPc
    SUCS_mb[3] = theVars->SUCS_Vel.v59 - theVars->SUCS_Vel.v58;//	F26BPc
    SUCS_mb[4] = 0;                                      //   vatpf - theVars->SUCS_Vel.v59 - v60;   //	ATPc
    SUCS_mb[5] = 0;                       //	ADPc
    SUCS_mb[7] = theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v56;//	UDPGc
    SUCS_mb[8] = 0;//  v60 - v55		;   //	UTPc
    SUCS_mb[9] = theVars->SUCS_Vel.v56 - theVars->SUCS_Vel.v57;//	SUCP
    SUCS_mb[10] = theVars->SUCS_Vel.v57 - theVars->SUCS_Vel.v62;//	SUC
    SUCS_mb[11] = theVars->SUCS_Vel.vpga_in - theVars->SUCS_Vel.vpga_use;//	pgaC
    return SUCS_mb;
}
