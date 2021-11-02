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

#include <math.h>
#include "Variables.hpp"
#include "globals.hpp"
#include "modules/Leaf_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double Cpwater = 4.184;//Jg-1c-1
const double Vandp = 198.;//gm-2 leaf thickness 200um leaf density 0.7*10^3kg m-3
//leaf volume leaf thickness 200um~0.2L
//bundary layer
//leaf air space 20//
const double Vol_airspace = 0.04;//L

LeafCondition* Leaf::_MB_con(const double t, const LeafCondition* Leaf_con, Variables *theVars) {

    Rate(t, Leaf_con, theVars);

    const double Molar_Volume = 22.4 / 273. * (theVars->Tp + 273.);

    LeafCondition* dydt = new LeafCondition();
    dydt->Ci = (theVars->Leaf_Vel.vCO2total - theVars->Leaf_Vel.NetAssimilation) / (Vol_airspace / Molar_Volume);
    dydt->Cb = 0.;
    dydt->Eb = 0.;
    dydt->Gs = theVars->Leaf_Vel.vgs;
    dydt->Tleaf = theVars->Leaf_Vel.EnergyBalanceResidual / Cpwater / Vandp;
    dydt->H2Oou = theVars->Leaf_Vel.vH2Ototal / pow(10., 6.);
    dydt->CO2in = theVars->Leaf_Vel.vCO2total / pow(10., 6.);

    return dydt;
}

arr Leaf::_MB(const double t, const LeafCondition* const Leaf_con, Variables *theVars) {
    LeafCondition* dydt = _MB_con(t, Leaf_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
