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
#include "definitions.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE_COMPOSITE(FIBF);
  
namespace modules {

/**
 Class for the FIBF Module functions.
 */
class FIBF : public MODULE_BASE(FIBF) {
public:
    DECLARE_MODULE_COMPOSITE(FIBF)
private:
    SET_GET(ChlPSI)
    SET_GET(ChlT)
    SET_GET(ChlT2)
    SET_GET(FIBF2FI_PQ)
    SET_GET(FIBF2FI_PQa)

    /**
      Function to calculate the dy/dt values for the FIBFCondition at the given time stamp.

      \param t The current timestamp
      \param FIBF_Con FIBFCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::FIBFCondition* const FIBF_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the FIBFCondition at the given time stamp.

      \param t The current timestamp
      \param FIBF_Con FIBFCondition object giving the input parameters
      \param theVars The global variables
      \return A FIBFCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::FIBFCondition* _MB_con(const double t,
                                              const conditions::FIBFCondition* const FIBF_Con,
                                              Variables *theVars);

    /**
      Calculate the Rates of FIBF based on the input FIBFCondition.

      \param t The current timestamp
      \param FIBF_Con FIBFCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::FIBFCondition* const FIBF_Con,
                      Variables *theVars) {
        (void)t;
        (void)FIBF_Con;
        (void)theVars;
    }

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
};

  DEFINE_MODULE_COMPOSITE_HEADER(FIBF);
  
}  // namespace modules
}  // namespace ePhotosynthesis
