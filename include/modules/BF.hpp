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

FORWARD_DECLARE_MODULE(BF);
  
namespace modules {

/**
 Class for the BF Module functions.
 */
class BF : public MODULE_BASE(BF) {
public:
    DECLARE_MODULE(BF)
    SET_GET(_Pi)
    SET_GET(EPS_ATP_Rate)
    SET_GET_BOOL_MODULE(FI_connect, conditions::BF)
    SET_GET_BOOL_MODULE(PS_connect, conditions::BF)
    SET_GET_BOOL_MODULE(RROEA_connect, conditions::BF)
private:

    SET_GET(cATPsyn)
    SET_GET(CPSi)
    SET_GET(cNADPHsyn)

    /**
      Reset the static member variables to their default values.
      */
    static void _reset(const bool noChildren = false) {
        setcATPsyn(0.);
        setCPSi(0.);
        setcNADPHsyn(0.);
        _Pi = 0.;
        EPS_ATP_Rate = 0.;
        setFI_connect(false);
        setPS_connect(false);
        setRROEA_connect(false);
        N = 1;
        TIME = 0.;
	ParentClass::_reset(noChildren);
    }
};

  DEFINE_MODULE_HEADER(BF);

}  // namespace modules
}  // namespace ePhotosynthesis
