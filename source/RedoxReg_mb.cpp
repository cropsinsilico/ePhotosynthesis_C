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

arr RedoxReg::RedoxReg_mb(double t, RedoxRegCon &RedoxReg_Con, Variables *theVars) {
    theVars->trDynaPS2RedReg_cal = 1;

    RedoxReg_Rate(t, RedoxReg_Con, theVars);

    arr RA_DYDT = RA_mb(t, RedoxReg_Con.RA_con, theVars);

    arr RedoxReg_DYDT;
    RedoxReg_DYDT.reserve(93);

    RedoxReg_DYDT.insert(RedoxReg_DYDT.end(), RA_DYDT.begin(), RA_DYDT.end());

    //RedoxReg_DYDT[92] = theVars->RedoxReg_Vel.Vred - theVars->RedoxReg_Vel.Vox;
    RedoxReg_DYDT[92] = 0;

    //const double Temp = RedoxReg_DYDT[23];
    //RedoxReg_DYDT[23] = Temp;
    return RedoxReg_DYDT;
}
