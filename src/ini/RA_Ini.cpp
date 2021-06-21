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
#include "modules/RA.hpp"
#include "modules/EPS.hpp"
#include "modules/RuACT.hpp"

RAContainer* RA::_init(Variables *theVars) {
    EPSContainer* EPS_con = EPS::init(theVars);
    RuACTContainer* RuACT_con = RuACT::init(theVars);

    if (theVars->RuACT_EPS_com)
        RuACT_con->RuBP = EPS_con->CM_con->PS_PR_con->PS_con->RuBP;
    theVars->RuACT_OLD_TIME = 0;
    theVars->RuACT_TIME_N = 1;

    // Now get the combined total concentration of different concentration variables.
    RAContainer* RA_con = new RAContainer(EPS_con, RuACT_con);
    return RA_con;
}
