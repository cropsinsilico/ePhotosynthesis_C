#pragma once

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

#include "ModuleBase.hpp"
#include "conditions/PS_PRCondition.hpp"

class PS_PR : public ModuleBase<PS_PR, PS_PRCondition> {
private:
    friend ModuleBase;
/**
  Initialize the variables

  @param theVars The global variables
  @return A PS_PRCon object for input into calculations
  */
    static PS_PRCondition* _init(Variables *theVars);

/**
  Calculate the output values based on the inputs

  @param PS_PRs PS_PRCon object giving the input parameters
  @param theVars The global variables
  @return A vector containing the updated values
  */
    static arr _MB(const double t, const PS_PRCondition* PS_PRs, Variables *theVars);

    static void _Rate(const double t, const PS_PRCondition* PS_PRs, Variables *theVars) {
        (void)t;
        (void)PS_PRs;
        (void)theVars;
    }
};
