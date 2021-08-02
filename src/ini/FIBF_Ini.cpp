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
#include "modules/FIBF.hpp"
#include "modules/FI.hpp"
#include "modules/BF.hpp"
// This is the function to initialize some global variable used in FIBF MODEL

size_t FIBFCondition::count = 0;
double FIBF::ChlPSI = 0.;
double FIBF::ChlT = 0.;
double FIBF::ChlT2 = 0.;
double FIBF::FIBF2FI_PQ = 0.;
double FIBF::FIBF2FI_PQa = 0.;

FIBFCondition* FIBF::_init(Variables *theVars) {

    const double FIBF_PQT = 8;
    theVars->FIBF_Pool.PQT = FIBF_PQT;
    return nullptr;
}
