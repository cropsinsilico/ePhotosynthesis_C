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

#include "definitions.hpp"
#include "Variables.hpp"

namespace ePhotosynthesis {
namespace modules {

template<class T, class U>
class ModuleBase {
public:
    static U* init(Variables *theVars) {return T::_init(theVars);}
    static arr MB(const double t, const U* constraints, Variables *theVars) {
        DEBUG_MESSAGE(constraints)
        return T::_MB(t, constraints, theVars);}
    static U* MB_con(const double t, const U* constraints, Variables *theVars) {
        DEBUG_MESSAGE(constraints)
        return T::_MB_con(t, constraints, theVars);}
protected:
    ModuleBase() {}
    static void Rate(const double t, const U* constraints, Variables *theVars) {
        T::_Rate(t, constraints, theVars);
    }
};

}  // namespace modules
}  // namespace ePhotosynthesis
