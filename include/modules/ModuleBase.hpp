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
#include "ValueSet.hpp"

namespace ePhotosynthesis {
namespace modules {

#define MODULES_MOD BF, FI, PR, PS, RROEA, RedoxReg, RuACT, SUCS, XanCycle
  
#define MODULE_BASE(name)						\
  ModuleBase<name, conditions::name ## Condition, MODULE_ ## name>
#define DECLARE_MODULE_BASE(name)					\
  static void _initCalc(Variables *theVars,				\
			conditions::name ## Condition* name ## _Con);	\
  static void _initOrig(Variables *theVars,				\
			conditions::name ## Condition* name ## _Con);	\
  static void _initAlt(Variables *theVars,				\
		       conditions::name ## Condition* name ## _Con);	\
  static void _updateAlts(Variables *theVars,				\
			  const std::string& context = "");		\
  static void _updateAlts(Variables *theVars,				\
			  conditions::name ## Condition* name ## _Con = nullptr, \
			  const std::string& context = "") {		\
      _updateAlts(theVars, context);					\
      if (name ## _Con) {						\
	name ## _Con->updateAlts(context);				\
      }									\
      ValueSetClass::updateAlts(context);				\
  }									\
  static void _checkAlts(Variables *theVars,				\
			 const std::string& context = "");		\
  static void _checkAlts(Variables *theVars,				\
			 const conditions::name ## Condition* name ## _Con = nullptr, \
			 const std::string& context = "") {		\
    if (name ## _Con) {							\
      name ## _Con->checkAlts(context + #name + "Condition: ");		\
    }									\
    _checkAlts(theVars, context);					\
    ValueSetClass::checkAlts(context + #name + ": ");			\
  }
#define DECLARE_MODULE(name)						\
  DECLARE_VALUE_SET_STATIC(name, ModuleBase<name,			\
			   conditions::name ## Condition,		\
			   MODULE_ ## name>)				\
  DECLARE_MODULE_BASE(name)
#define DECLARE_MODULE_COMPOSITE(name)					\
  DECLARE_VALUE_SET_STATIC_COMPOSITE(name, ModuleBase<name,		\
				     conditions::name ## Condition,	\
				     MODULE_ ## name>)			\
  DECLARE_MODULE_BASE(name)

#define DEFINE_DEFAULT_CHECKALT(name)					\
  void name::_checkAlts(Variables *theVars,				\
			const std::string& context) {			\
    UNUSED(theVars);							\
    UNUSED(context);							\
  }									\
  void name::_updateAlts(Variables *theVars,				\
			 const std::string& context) {			\
    UNUSED(theVars);							\
    UNUSED(context);							\
  }
  
#define DEFINE_DEFAULT_INITALT(name)					\
  void name::_initCalc(Variables *theVars,				\
		      name ## Condition* name ## _Con) {		\
    UNUSED(theVars);							\
    UNUSED(name ## _Con);						\
  }									\
  void name::_initOrig(Variables *theVars,				\
		      name ## Condition* name ## _Con) {		\
    UNUSED(theVars);							\
    UNUSED(name ## _Con);						\
  }									\
  void name::_initAlt(Variables *theVars,				\
		      name ## Condition* name ## _Con) {		\
    UNUSED(theVars);							\
    UNUSED(name ## _Con);						\
  }									\
  DEFINE_DEFAULT_CHECKALT(name)

/**
  This template class provides a common set of static funtions for every Module subclass. This makes
  calling the underlying functions more straight forward.

  \tparam T The class type to work with
  \tparam U The condition class associated with this Module
  */
template<class T, class U, MODULE ID = MODULE_NONE>
class ModuleBase : public ValueSetStatic<ID, PARAM_TYPE_MOD> {
public:
    DECLARE_VALUE_SET_STATIC_BASE(ModuleBase, ValueSetStatic<ID, PARAM_TYPE_MOD>)
    typedef T ModType;
    typedef U CondType;
  
    /**
      Common, public interface for the private init function.

      \param theVars The main Variables instance.
      \returns A pointer to the associated Condition class.
      */
    static U* init(Variables *theVars) {
      U* out = T::_init(theVars);
#ifdef CHECK_VALUE_SET_ALTS
      T::_initAlt(theVars, out);
      T::_checkAlts(theVars, out, "ModuleBase::init::");
#endif // CHECK_VALUE_SET_ALTS
      return out;
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
	arr out = T::_MB(t, condition, theVars);
	return out;
    }

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
#ifdef CHECK_VALUE_SET_ALTS
	T::_checkAlts(theVars, condition, "ModuleBase::MB_con::");
#endif // CHECK_VALUE_SET_ALTS
	U* out = T::_MB_con(t, condition, theVars);
#ifdef CHECK_VALUE_SET_ALTS
	T::_updateAlts(theVars, out, "ModuleBase::MB_con::");
#endif // CHECK_VALUE_SET_ALTS
	return out;
    }
    /**
      Common, public interface for the private function which resets any static Module parameters
      to their default values.
      */
    static void reset() {
        T::_reset();
        ValueSetClass::reset();
    }
protected:
    ModuleBase() {}
  // ValueSet<ID, PARAM_TYPE_MODULE>() {}

    /**
      Common interface for the private rate calculation function.

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      */
    static void Rate(const double t, const U* condition, Variables *theVars) {
#ifdef CHECK_VALUE_SET_ALTS
	T::_checkAlts(theVars, condition, "ModuleBase::Rate::");
#endif // CHECK_VALUE_SET_ALTS
        T::_Rate(t, condition, theVars);
#ifdef CHECK_VALUE_SET_ALTS
	T::_updateAlts(theVars, nullptr, "ModuleBase::Rate::");
#endif // CHECK_VALUE_SET_ALTS
    }
};

}  // namespace modules
}  // namespace ePhotosynthesis
