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

FORWARD_DECLARE_MODULE(FI);
  
namespace modules {

/**
 Class for FI related functions
 */
class FI : public MODULE_BASE(FI) {
public:
    DECLARE_MODULE(FI)
    SET_GET_BOOL_MODULE(BF_connect, conditions::FI)
    SET_GET(cpsii)
private:

    /**
      Function to set the initial state of the FICondition class.

      \param theVars Pointer to the global variables
      \return A FICondition object with values set based on the input
      */
    static conditions::FICondition* _init(Variables *theVars);

    /** \copydoc ValueSetBase::_reset */
    static void _reset(const bool noChildren = false) {
        BF_connect = false;
        cpsii = 0.;
        N = 1;
        TIME = 0.;
	ParentClass::_reset(noChildren);
    }
};

  DEFINE_MODULE_HEADER(FI);
  
}  // namespace modules
}  // namespace ePhotosynthesis
