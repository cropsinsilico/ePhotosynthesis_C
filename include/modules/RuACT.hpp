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
#include "../conditions/RuACTCondition.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE(RuACT);
  
namespace modules {

/**
 Class for RuACT related functions
 */
class RuACT : public MODULE_BASE(RuACT) {
public:
    DECLARE_MODULE(RuACT)
    SET_GET_BOOL_MODULE(EPS_connect, conditions::RuACT)
private:
  
    /** \copydoc ValueSetBase::_reset */
    static void _reset(const bool noChildren = false) {
        activase = 0.;
        TIME = 0.;
        N = 1;
	ParentClass::_reset(noChildren);
    }
    SET_GET(activase)
};

  DEFINE_MODULE_HEADER(RuACT);
  
}  // namespace modules
}  // namespace ePhotosynthesis
