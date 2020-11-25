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
#include "CM.hpp"

arr CM_Mb(realtype t, CMCon &CM_con, Variables *myVars) {
    arr dxdt = zeros(36);

    arr SUCS_DYDT = zeros(12);
    SUCS_DYDT = SUCS_Mb(t, CM_con.SUCS_con, myVars);
    //PS_PRCon PS_PR_con(PSPR_Con);
    arr PSPR_DYDT = PS_PRmb(t, CM_con.PS_PR_con, myVars);

    for (size_t m = 0; m < 23; m++)
        dxdt[m] = PSPR_DYDT[m];


    for (size_t m = 0; m < 12; m++)
        dxdt[m + 23] = SUCS_DYDT[m];


    dxdt[35] = PSPR_DYDT[23];

    const double vdhap = myVars->PS2CM_vdhap;        // The rate of export out of chloroplast

    // The rate of export out of chloroplast
    const double vgap = myVars->PS2CM_vgap;

    // The rate of import into the cytosol
    const double vdhap_ins = myVars->SUCS2CM_vdhap;   //	DHAP IN
    const double vgap_ins = myVars->SUCS2CM_vgap;   //	GAP IN
    if (myVars->TestSucPath == 1)
        SUCS_DYDT[0] = SUCS_DYDT[0] + vdhap + vgap - (vdhap_ins + vgap_ins);

    //	T3Pc WY1905
    dxdt[23] = SUCS_DYDT[0];

    const double vpga = myVars->PS2CM_vpga;

    const double vpga_ins = myVars->SUCS2CM_vpga;                                       //	PGA export from chloroplast

    SUCS_DYDT[11] = SUCS_DYDT[11] - vpga_ins + vpga;//	pgaC
    dxdt[34] = SUCS_DYDT[11];
    return dxdt;
}
