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
#include "conditions/EnzymeCondition.hpp"
#include "definitions.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class EnzymeModuleTest;
}
#endif
namespace modules {
class RAC4leafMeta;
/**
  Class for grouping Enzyme related functions and common variables
  */
class Enzyme : public ModuleBase<Enzyme, conditions::EnzymeCondition> {
private:
    friend ModuleBase;
    friend class modules::RAC4leafMeta;
#ifdef TESTING
    friend class test::EnzymeModuleTest;
#endif
    /**
      Function to set the initial state of the EnzymeCondition class.

      \param theVars Pointer to the global variables
      \return A EnzymeCondition object with values set based on the input
      */
    static conditions::EnzymeCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the EnzymeCondition at the given time stamp.

      \param t The current timestamp
      \param Enzyme_con EnzymeCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::EnzymeCondition* const Enzyme_con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the EnzymeCondition at the given time stamp.

      \param t The current timestamp
      \param Enzyme_con EnzymeCondition object giving the input parameters
      \param theVars The global variables
      \return A EnzymeCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::EnzymeCondition* _MB_con(const double t,
                                            const conditions::EnzymeCondition* const Enzyme_con,
                                            Variables *theVars);
    /**
      Calculate the Rates of Enzyme based on the input EnzymeCondition.

      \param t The current timestamp
      \param Enzyme_con EnzymeCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::EnzymeCondition* const Enzyme_con, Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
    SET_GET(Bchl_CA)
    SET_GET(Bchl_CN)
    SET_GET(MC_CU)
    SET_GET(MC_CA)
    SET_GET(Mchl_CA)
    SET_GET(Mchl_CN)
    SET_GET(MC_UTP)
    SET_GET(Bper_GLU)
    SET_GET(Bper_KG)
    SET_GET(Bper_NADH)
    SET_GET(Bper_NAD)
    SET_GET(Bchl_CP)
    SET_GET(vNAE)
    SET_GET(V6sen)
    static int pathway_option;
    ////// 0=Normol NADP-ME type
    ////// 1=Asp+Mal transport and MDH type
    ////// 2=Asp+Mal and PCK type
    ////// 3 Asp+Mal and PCK+MDH type
    ////// 4 Asp and PCK only type
    ////// 6 DiT2 mutant
    ////// 7 NAD-ME type
    ////// 8 NAD-ME+PCK type


    static std::vector<arr> KValue;
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the Enzyme TimeSeries
};

}  // namespace modules
}  // namespace ePhotosynthesis
