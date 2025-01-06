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
#include "definitions.hpp"
#include "conditions/BFCondition.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class BFModuleTest;
}
#endif
namespace modules {
class FIBF;
/**
 Class for the BF Module functions.
 */
class BF : public ModuleBase<BF, conditions::BFCondition> {
public:
    SET_GET(_Pi)
    SET_GET(EPS_ATP_Rate)
    SET_GET_BOOL_MODULE(FI_connect, conditions::BF)
    SET_GET_BOOL_MODULE(PS_connect, conditions::BF)
    SET_GET_BOOL_MODULE(RROEA_connect, conditions::BF)
private:
    friend ModuleBase;
    friend class modules::FIBF;
#ifdef TESTING
    friend class test::BFModuleTest;
#endif
    /**
      Function to set the initial state of the BFCondition class.

      \param theVars Pointer to the global variables
      \return A BFCondition object with values set based on the input
      */
    static conditions::BFCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the BFCondition at the given time stamp.

      \param t The current timestamp
      \param BF_con BFCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::BFCondition* const BF_con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the BFCondition at the given time stamp.

      \param t The current timestamp
      \param BF_con BFCondition object giving the input parameters
      \param theVars The global variables
      \return A BFCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::BFCondition* _MB_con(const double t, const conditions::BFCondition* const BF_con,
                                            Variables *theVars);

    /**
      Calculate the Rates of BF based on the input BFCondition.

      \param t The current timestamp
      \param BF_con BFCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::BFCondition* const BF_con, Variables *theVars);

    SET_GET(cATPsyn)
    SET_GET(CPSi)
    SET_GET(cNADPHsyn)
    EPHOTO_API static double TIME;    // The timestamp of the most recent call to _Rate
    EPHOTO_API static std::size_t N;  // The current size of the BF TimeSeries

    /**
      Reset the static member variables to their default values.
      */
    static void _reset() {
        cATPsyn = 0.;
        CPSi = 0.;
        cNADPHsyn = 0.;
        _Pi = 0.;
        EPS_ATP_Rate = 0.;
        FI_connect = false;
        PS_connect = false;
        RROEA_connect = false;
        N = 1;
        TIME = 0.;
        conditions::BFCondition::reset();
    }
};

}  // namespace modules
}  // namespace ePhotosynthesis
