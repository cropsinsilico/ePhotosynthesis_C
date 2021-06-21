#pragma once
#include "definitions.hpp"

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
#include "containers/XanCycleContainer.hpp"

/**
 Class for XanCycle code and internal variables
 */
class XanCycle : public ModuleBase<XanCycle, XanCycleContainer> {
private:
    friend ModuleBase;
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A XanCycleCon object with values set base on the input
      */
    static XanCycleContainer* _init(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param XanCycle_Con XanCycleCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const XanCycleContainer* XanCycle_Con, Variables *theVars);

    /**
      Calculate the Rates of XanCycle based on the inputs

      @param t The current timestamp
      @param XanCycle_Con XanCycleCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const XanCycleContainer* XanCycle_Con, Variables *theVars);

    static double XanCycle_kav;
    static double XanCycle_kaz;
    static double XanCycle_kva;
    static double XanCycle_kza;
};

