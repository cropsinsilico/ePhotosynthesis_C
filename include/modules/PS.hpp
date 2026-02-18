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
#include "../conditions/PSCondition.hpp"
#include "../definitions.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE(PS);
  
namespace modules {

/**
 Class to for PS related functions and common variables
 */
class PS : public MODULE_BASE(PS) {
public:
    DECLARE_MODULE(PS)
    /**
      Set the initial parameters.

      \param param The initial parameters.
      */
    static void setParam(const arr &param) {
        Param = param;
    }

    /**
      Get the current size of the PS TimeSeries.

      \returns The current size of the TimeSeries.
      */
    static std::size_t getN() {return N;}
private:

    /**
      Reset the static member variables to their default values.
      */
    static void _reset(const bool noChildren = false);
    SET_GET_BOOL_MODULE(C3, conditions::PS)

    static arr Param;
};

  DEFINE_MODULE_HEADER(PS);

}  // namespace modules
}  // namespace ePhotosynthesis
