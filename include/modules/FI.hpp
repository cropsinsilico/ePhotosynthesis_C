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
#include "conditions/FIBFCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class FIModuleTest;
}
#endif
namespace modules {

#define MEMBERS_FI_CONSTANT EMPTY_MEMBER_LIST
#define MEMBERS_FI_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_FI_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for FI related functions
 */
class FI : public ModuleBase<FI, conditions::FICondition, MODULE_FI> {
public:
    DECLARE_VALUE_SET_STATIC_SINGLE(FI, ModuleBase<FI, conditions::FICondition, MODULE_FI>)
    SET_GET_BOOL_MODULE(BF_connect, conditions::FI)
private:
    friend ModuleBase;
    friend class modules::FIBF;
#ifdef TESTING
    friend class test::FIModuleTest;
#endif

    /**
      Function to set the initial state of the FICondition class.

      \param theVars Pointer to the global variables
      \return A FICondition object with values set based on the input
      */
    static conditions::FICondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the FICondition at the given time stamp.

      \param t The current timestamp
      \param FI_Con FICondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::FICondition* const FI_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the FICondition at the given time stamp.

      \param t The current timestamp
      \param FI_Con FICondition object giving the input parameters
      \param theVars The global variables
      \return A FICondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::FICondition* _MB_con(const double t,
                                            const conditions::FICondition* FI_Con,
                                            Variables *theVars);
    /**
      Calculate the Rates of FI based on the input FICondition.

      \param t The current timestamp
      \param FI_Con FICondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::FICondition* const FI_Con, Variables *theVars);
    SET_GET(cpsii)

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        BF_connect = false;
        cpsii = 0.;
        N = 1;
        TIME = 0.;
        conditions::FICondition::reset();
    }
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the FI TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER_SINGLE(FI);
  
}  // namespace modules
}  // namespace ePhotosynthesis
