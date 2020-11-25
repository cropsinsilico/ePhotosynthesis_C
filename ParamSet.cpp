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

void ParamSet(Variables *myVars) {

    const double va1 = 0;

    myVars->BF_Param = zeros(2);
    myVars->BF_Param[0] = va1;
    myVars->BF_Param[1] = myVars->PS12ratio;

    myVars->FI_Param = zeros(2);
    myVars->FI_Param[0] = va1;
    myVars->FI_Param[1] = myVars->PS12ratio;

    myVars->PS_PR_Param = 0;
    myVars->EPS_Param = 0;

    myVars->SUCS_Param = zeros(2);


    myVars->RuACT_Param = zeros(2);
    myVars->RuACT_Param[0] = va1;
    myVars->RuACT_Param[1] = myVars->PS12ratio;

    myVars->XanCycle_Param = zeros(2);
    myVars->XanCycle_Param[0] = va1;
    myVars->XanCycle_Param[1] = myVars->PS12ratio;


    myVars->RROEA_Param = zeros(2);
    myVars->RROEA_Param[0] = va1;
    myVars->RROEA_Param[1] = myVars->PS12ratio;

    myVars->RedoxReg_Param = 0;

}
