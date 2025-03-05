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

#include "modules/ModuleBase.hpp"
#include "conditions/PRCondition.hpp"
#include "definitions.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE(PR);

namespace modules {

/**
  Class for grouping PR related functions and common variables
  */
class PR : public MODULE_BASE(PR) {
public:
    DECLARE_MODULE(PR)
private:

#ifdef MAKE_EQUIVALENT_TO_MATLAB
    static void _initDefaults();
#endif // MAKE_EQUIVALENT_TO_MATLAB
    
    /**
      Reset the static member variables to their default values.
      */
    static void _reset(const bool noChildren = false);
    SET_GET(KC)
    SET_GET(KE113)
    SET_GET(KE122)
    SET_GET(KE123)
    SET_GET(KE124)
    SET_GET(KGc)
    SET_GET(KI1011)
    SET_GET(KI1012)
    SET_GET(KI1121)
    SET_GET(KI1122)
    SET_GET(KI113)
    SET_GET(KI1221)
    SET_GET(KI123)
    SET_GET(KI1311)
    SET_GET(KM1312)
    SET_GET(KI1312)
    SET_GET(KM1011)
    SET_GET(KM1012)
    SET_GET(KM112)
    SET_GET(KM1131)
    SET_GET(KM1132)
    SET_GET(KM121)
    SET_GET(KM1221)
    SET_GET(KM1222)
    SET_GET(KM123)
    SET_GET(KM1241)
    SET_GET(KM1242)
    SET_GET(KI124)
    SET_GET(KM1311)
    SET_GET(KO)
    SET_GET(KR)
    SET_GET(NADHc)
    SET_GET(NADc)
    SET_GET(PR_ADP)
    SET_GET(PR_ATP)
    SET_GET(V111)
    SET_GET(V112)
    SET_GET(V113)
    SET_GET(V121)
    SET_GET(V122)
    SET_GET(V123)
    SET_GET(V124)
    SET_GET(V131)
    SET_GET(V2T)
    SET_GET(Vfactor112)
    SET_GET(Vfactor113)
    SET_GET(Vfactor121)
    SET_GET(Vfactor122)
    SET_GET(Vfactor123)
    SET_GET(Vfactor124)
    SET_GET(Vfactor131)
    SET_GET(Vf_T131)
    SET_GET(Vf_T113)
    SET_GET(Vf_T123)
    SET_GET(Vf_T121)
    SET_GET(Vf_T122)
    SET_GET(Vf_T112)
    SET_GET(V1T)
    SET_GET(PGA)
    SET_GET(GLUc)
    SET_GET_BOOL_MODULE(PS_connect, conditions::PR)
    SET_GET_BOOL_MODULE(PS_RuBP, conditions::PR)
    SET_GET(RUBISCOTOTAL)

};

  DEFINE_MODULE_HEADER(PR);

}  // namespace modules
}  // namespace ePhotosynthesis
