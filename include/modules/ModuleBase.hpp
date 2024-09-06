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

/**
  This template class provides a common set of static funtions for every Module subclass. This makes
  calling the underlying functions more straight forward.

  \tparam T The class type to work with
  \tparam U The condition class associated with this Module
  */
template<class T, class U>
class ModuleBase {
public:
    typedef T ModType;
    typedef U CondType;
  
    /**
      Common, public interface for the private init function.

      \param theVars The main Variables instance.
      \returns A pointer to the associated Condition class.
      */
    static U* init(Variables *theVars) {
      // CondType* out = new CondType();
      // out.init(theVars);
      return T::_init(theVars);
    }

    /**
      Common, public interface for the private differential calculation function

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      \returns A vector of doubles containing the dy/dt values for each member of the input Condition.
      */
    static arr MB(const double t, const U* condition, Variables *theVars) {
#ifdef INCDEBUG
        DEBUG_MESSAGE(condition)
#endif
        return T::_MB(t, condition, theVars);}

    /**
      Common, public interface for the private differential calculation function

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      \returns A pointer to an associated Condition class containing the dy/dt values for each member of the input Condition.
      */
    static U* MB_con(const double t, const U* condition, Variables *theVars) {
#ifdef INCDEBUG
        DEBUG_MESSAGE(condition)
#endif
        return T::_MB_con(t, condition, theVars);}

    /**
      Common, public interface for the private function which resets any static Module parameters
      to their default values.
      */
    static void reset() {
        T::_reset();
    }
protected:
    ModuleBase() {}

    /**
      Common interface for the private rate calculation function.

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      */
    static void Rate(const double t, const U* condition, Variables *theVars) {
        T::_Rate(t, condition, theVars);
    }
};

}  // namespace modules
}  // namespace ePhotosynthesis
