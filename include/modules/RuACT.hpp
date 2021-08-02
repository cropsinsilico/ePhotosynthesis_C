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
#include "conditions/RuACTCondition.hpp"

/**
 Class for RuACT related functions
 */
class RuACT : public ModuleBase<RuACT, RuACTCondition> {
public:
    static void setEPS(const bool val) {
        EPS_con = val;
        RuACTCondition::setEPS(val);
    }
private:
    friend ModuleBase;
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A RuACTCon object with values set base on the input
      */
    static RuACTCondition* _init(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param RuACT_Con RuACTCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const RuACTCondition* RuACT_Con, Variables *theVars);
    static RuACTCondition* _MB_con(const double t, const RuACTCondition* RuACT_Con, Variables *theVars);
    /**
      Calculate the Rates of RuACT based on the inputs

      @param t The current timestamp
      @param RuACT_Con RuACTCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const RuACTCondition* RuACT_Con, Variables *theVars);
    static bool EPS_con;
    static double activase;
    static double TIME;
    static size_t N;
};
