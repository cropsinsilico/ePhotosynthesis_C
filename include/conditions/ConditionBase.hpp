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
#include "enums/enums_utils.hpp"
#include "ValueSet.hpp"
#include <stdexcept>
#include <sundials/sundials_types.h>

#ifndef realtype
#define realtype sunrealtype
#endif

#define MODULES_COND BF, CM, DynaPS, EPS, FIBF, FI, PR, PS, PS_PR, RA, RROEA, RedoxReg, RuACT, SUCS, XanCycle, trDynaPS

namespace ePhotosynthesis {
namespace conditions {

/**
  This template class provides a common set of funtions for every Condition subclass. This makes
  calling the underlying functions more straight forward.

  \tparam T The class type to work with
  \tparam U The parent Condition class
  */
template<class T, class U, MODULE ID = MODULE_NONE>
class ConditionBase : public VALUE_SET_PARENT(T, U, ID, PARAM_TYPE_COND) {
public:
    DECLARE_VALUE_SET_BASE(ConditionBase, VALUE_SET_PARENT(T, U, ID, PARAM_TYPE_COND))
    ~ConditionBase() override {}

    /**
      Common, public interface for the private _size function.

      \returns The number of active data members in this class.
      */
    static std::size_t size() {
	size_t out = T::_size();
        if (T::module != MODULE_NONE) {
	    size_t alt = T::memberCount();
	    if (alt != out) {
		ERROR_VALUE_SET("Size of default_values (", alt,
				") does not match expected count (", out,
				") ", T::memberState());
	    }
	}
        return out;
    }

    /**
      Converts a realtype* to a vector and then copies the data into this class.

      \param x Array of realtype's
      */
    void fromArray(realtype *x) {
        arr vec(size());
        for (std::size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }

    /**
      Common, puiblic interface for the private _clear function.
      */
    void clear() {
        static_cast<T*>(this)->_clear();
    }

    /**
      Set the parent class pointer to the given value.

      \param par Pointer to the parent class instance.
      */
    void setParent(U* par) override {parent = par;}

    U* parent = nullptr;  //!< The parent Condition instance.
#ifdef INCDEBUG
    Debug::DebugLevel debugLevel() const {return static_cast<const T*>(this)->_dlevel;}
#endif

protected:
    ConditionBase() : VALUE_SET_PARENT(T, U, ID, PARAM_TYPE_COND)() {}
    ConditionBase(const ConditionBase* const other) :
      VALUE_SET_PARENT(T, U, ID, PARAM_TYPE_COND)(*other) {}

#ifdef INCDEBUG
    EPHOTO_API static Debug::DebugLevel _dlevel;
 public:
    static void setTop() {
	T::_dlevel = Debug::Top;
    }
 protected:
#endif // INCDEBUG

    static void _initStaticMembers() {
	ParentClass::_initStaticMembers();
#ifdef INCDEBUG
	if (ParentClass::child_classes.size() > 0)
	    T::_dlevel = Debug::Middle;
#endif // INCDEBUG
    }

    /**
      Serialize the data members of this object to a vector of double's. This is the opposite of
      _fromArray().

      \returns std::vector<double> containing the data members.
      */
    arr _toArray() const override { return arr(); }

    /**
      Reset any child instances.
      */
    virtual void _clear() {}

    /**
      De-serialize the given vector into this object. This is the opposite of _toArray()

      \param vec A vector containing the serialized version of this object.
      \param offset The index of the first data member for the de-serialization
      */
    void _fromArray(const arr &vec, const std::size_t offset = 0) override {}
};

#ifdef INCDEBUG
template<class T, class U, MODULE ID>
Debug::DebugLevel ConditionBase<T, U, ID>::_dlevel = Debug::Low;
#endif

}  // namespace conditions
}  // namespace ePhotosynthesis


/**
   Macro for boiler plate forward declaring classes required to declare a
     singular or composite module class
   \param name Name of module
 */
#define FORWARD_DECLARE_CONDITION_BASE(name)				\
  namespace modules {							\
    class VARS_CLASS_VAR_LOCAL(name, MOD);				\
  }
/**
   Macro for boiler plate forward declaring classes required to declare a
     singular module class
   \param name Name of module
 */
#define FORWARD_DECLARE_CONDITION(name)					\
  FORWARD_DECLARE_CONDITION_BASE(name)					\
  namespace conditions {						\
    class VARS_CLASS_VAR_LOCAL(PARENT_ ## name, COND);			\
  }
/**
   Macro for boiler plate forward declaring classes required to declare a
     composite module class w/o a parent module
   \param name Name of module
 */
#define FORWARD_DECLARE_CONDITION_TOP(name)				\
  FORWARD_DECLARE_CONDITION_BASE(name)
/**
   Boiler plate for singular & composite condition classes.
   \param name Name of the module the condition class is associated with.
 */
#define DECLARE_CONDITION_BASE(name)					\
private:								\
 friend ValueSetBase;							\
 friend ConditionBase;							\
 friend class modules::name;						\
 /** \copydoc ConditionBase::_fromArray */				\
 void _fromArray(const arr &vec, const std::size_t offset = 0) override; \
 /** \copydoc ConditionBase::_toArray */				\
 arr _toArray() const override;						\
public:                                                                 \
 /** Assignment operator */                                             \
 /** @param other The object to copy */                                 \
 /** @returns Updated condition */                                      \
 VARS_CLASS_VAR_LOCAL(name, COND)& operator=(const VARS_CLASS_VAR_LOCAL(name, COND)& other); \
 /** Copy constructor that makes a deep copy of the given object */	\
 /** @param other The object to copy */                                 \
 VARS_CLASS_VAR_LOCAL(name, COND)(const VARS_CLASS_VAR_LOCAL(name, COND)* const other) { \
     *this = *other;                                                    \
 }                                                                      \
 /** Copy constructor that makes a deep copy of the given object */	\
 /** @param other The object to copy */                                 \
 VARS_CLASS_VAR_LOCAL(name, COND)(const VARS_CLASS_VAR_LOCAL(name, COND)& other) : \
   VARS_CLASS_VAR_LOCAL(name, COND)(&other) {}
  
#define DECLARE_CONDITION_BASE2(name)					\
  public:								\
  /** Constructor to create an object from the input vector, starting */ \
  /**   at the given offset.*/						\
  /** @param vec Vector to create the object from */			\
  /** @param offset The index in vec to start creating the object from */ \
  VARS_CLASS_VAR_LOCAL(name, COND)(const arr &vec, const std::size_t offset = 0); \
  /** Constructor to create an object from a pointer to an array */	\
  /** @param x Pointer to an array (C array) */				\
  VARS_CLASS_VAR_LOCAL(name, COND)(realtype *x);			\
  ~VARS_CLASS_VAR_LOCAL(name, COND)() override {			\
    _clear();								\
  }									\
private:								\
 /** \copydoc ValueSet::_print */					\
 std::ostream& _print(std::ostream &out,				\
		      const uint tab = 0,				\
		      bool includePrefixes = false,			\
		      bool noChildren = false) const override;		\
 /** \copydoc ConditionBase::_reset */					\
 static void _reset(const bool noChildren = false);			\
 /** \copydoc ConditionBase::_size */					\
 static std::size_t _size();						\
public:

/**
   Utility macro to decrement the size based on a control parameter.
   \param name Name of control parameter.
   \param var Name of variable that control parameter disables.
 */
#define DECLARE_CONDITION_CONTROL_SIZE(name, var)	\
  if (name) c--
#define DECLARE_CONDITION_CONTROL_SIZE_PACKED(args)	\
  DECLARE_CONDITION_CONTROL_SIZE args
/**
   Boiler plate for declaring members of a singular condition class.
   \param name Name of the module the condition class is associated with.
*/
#define DECLARE_CONDITION(name)						\
  DECLARE_VALUE_SET(VARS_CLASS_VAR_LOCAL(name, COND),			\
		    ConditionBase<VARS_CLASS_VAR_LOCAL(name, COND),	\
		    VARS_CLASS_VAR_LOCAL(PARENT_ ## name, COND),	\
		    MODULE_ ## name>)					\
  DECLARE_CONDITION_BASE(name)						\
  /** Construct from parent */						\
  /** @param par Pointer to parent instance */				\
  VARS_CLASS_VAR_LOCAL(name, COND)(VARS_CLASS_VAR_LOCAL(PARENT_ ## name, COND)* par = nullptr) { \
      setParent(par);							\
      initMembers();							\
  }									\
private:								\
  /** The Maximum size of the serialized vector */			\
  EPHOTO_API static const std::size_t count;				\
public:

/**
   Utility macro for getting the set of control parameters in a condition
     class.
   \param name Name of the module the condition class is associated with.
 */
#define CONDITION_CONTROL_NAMES(name)				\
  FOR_EACH_COMMA(FIRST_ARG_PACKED, EXPAND(CONTROL_ ## name))
/**
   Utility macro for getting the set of control parameter regulated
     variables in a condition class.
   \param name Name of the module the condition class is associated with.
 */
#define CONDITION_CONTROL_VARS(name)				\
  FOR_EACH_COMMA(LAST_ARG_PACKED, EXPAND(CONTROL_ ## name))
/**
   Utility macro for getting the set of boolean parameters in a condition
     class.
   \param name Name of the module the condition class is associated with.
 */
#define CONDITION_BOOL_MEMBERS(name)					\
  JOIN_MACROS(SEP_COMMA, BOOL_MEMBERS_BF, CONDITION_CONTROL_NAMES(name))

#define DECLARE_CONDITION_COMPOSITE_CHILD(name)				\
  VARS_CLASS_VAR_LOCAL(name, COND)* name ## _con = nullptr; /**< Child condition */
#define DECLARE_CONDITION_COMPOSITE_CHILD_VARS(name)	\
  VARS_CLASS_VAR_LOCAL(name, COND)* name ## _other
#define DECLARE_CONDITION_COMPOSITE_CHILD_INIT(name)	\
  name ## _con(new VARS_CLASS_VAR_LOCAL(name, COND)(this))
/**
   Boiler plate for declaring members of a composite condition class.
   \param name Name of the module the condition class is associated with.
*/
#define DECLARE_CONDITION_COMPOSITE_BASE(name)				\
  DECLARE_VALUE_SET_COMPOSITE(VARS_CLASS_VAR_LOCAL(name, COND),		\
			      (SUFFIX_EACH(Condition, CHILDREN_ ## name)), \
			      (SUFFIX_EACH(_con, CHILDREN_ ## name)),	\
			      ConditionBase<				\
			      VARS_CLASS_VAR_LOCAL(name, COND),		\
			      VARS_CLASS_VAR_LOCAL(PARENT_ ## name, COND), \
			      MODULE_ ## name>)				\
  DECLARE_CONDITION_BASE(name)						\
  private:								\
  /** \copydoc ConditionBase::_clear */					\
  void _clear() override;						\
  /** \copydoc ConditionBase::_reset */					\
  static void _reset(const bool noChildren = false);			\
  /** \copydoc ValueSet::_createChildren */				\
  void _createChildren() override;					\
  /** Number of elements in the current array version of this condition */ \
  EPHOTO_API static std::size_t count;					\
  public:
#define DECLARE_CONDITION_COMPOSITE(name)	\
  DECLARE_CONDITION_COMPOSITE_BASE(name)
  
#define DECLARE_CONDITION_COMPOSITE2(name)				\
  /** Construct from parent */						\
  /** @param par Pointer to parent instance */				\
  VARS_CLASS_VAR_LOCAL(name, COND)(VARS_CLASS_VAR_LOCAL(PARENT_ ## name, COND)* par = nullptr) : \
  FOR_EACH_COMMA(DECLARE_CONDITION_COMPOSITE_CHILD_INIT,		\
		 EXPAND(CHILDREN_ ## name)) {				\
      setParent(par);							\
      initMembers();							\
  }									\
  /** Constructor to create an object from the contained classes*/	\
  VARS_CLASS_VAR_LOCAL(name, COND)(FOR_EACH_COMMA(DECLARE_CONDITION_COMPOSITE_CHILD_VARS, \
				   EXPAND(CHILDREN_ ## name)));		\
  FOR_EACH(DECLARE_CONDITION_COMPOSITE_CHILD, EXPAND(CHILDREN_ ## name));
/**
   Macro for boiler plate declaring a composite condition class w/o a
       parent module
   \param name Name of the module the condition class is associated with.
 */
#define DECLARE_CONDITION_TOP(name)					\
  DECLARE_CONDITION_COMPOSITE_BASE(name)				\
  /** \copydoc ModuleBase::setParent */					\
  void setParent(VARS_CLASS_VAR_LOCAL(name, COND)* par) override {	\
      UNUSED(par);							\
  }
/**
   Boiler plate for defining members of a singular or composite condition
     class.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_BASE(name)					\
  DEFINE_VALUE_SET_SRC(VARS_CLASS_VAR_LOCAL(name, COND))
#define DEFINE_CONDITION_BASE2(name)					\
  VARS_CLASS_VAR_LOCAL(name, COND)::					\
    VARS_CLASS_VAR_LOCAL(name, COND)(const arr &vec,			\
				     const std::size_t offset) {	\
      initMembers();							\
      fromArray(vec, offset);						\
  }									\
  VARS_CLASS_VAR_LOCAL(name, COND)::					\
    VARS_CLASS_VAR_LOCAL(name, COND)(realtype *x) {			\
      initMembers();							\
      fromArray(x);							\
  }
/**
   Boiler plate for defining members of a singular condition class.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION(name)						\
  DEFINE_CONDITION_BASE(name)                                           \
  VARS_CLASS_VAR_LOCAL(name, COND)& VARS_CLASS_VAR_LOCAL(name, COND)::  \
  operator=(const VARS_CLASS_VAR_LOCAL(name, COND)& other) {            \
      initMembers();							\
      copyMembers(other);                                               \
      return *this;                                                     \
  }
#define DEFINE_CONDITION2(name)						\
  std::size_t VARS_CLASS_VAR_LOCAL(name, COND)::_size() {		\
      std::size_t c = count;						\
      FOR_EACH(DECLARE_CONDITION_CONTROL_SIZE_PACKED,			\
               EXPAND(CONTROL_ ## name));                               \
      return c;								\
  }
/**
   Utility macro for initializing static boolean class members for a
     condition class.
   \param name Name of the module the condition class is associated with.
   \param var Name of the boolean member.
 */
#define DEFINE_CONDITION_BOOL_MEMBER(name, var)		\
  bool VARS_CLASS_VAR_LOCAL(name, COND)::var = false
/**
   Boiler plate for defining members of a singular condition class in
     the ini compilation unit.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_INI(name)					\
  FOR_EACH_WITH_PREFIX_ARGS(DEFINE_CONDITION_BOOL_MEMBER, (name),	\
			    CONDITION_BOOL_MEMBERS(name))
  
/**
   Boiler plate for defining members of a singular condition class in the
     header.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_HEADER(name)				\
  DEFINE_VALUE_SET_HEADER(VARS_CLASS_VAR_LOCAL(name, COND))

/**
   Utility to initialize a child condition.
   \param name Name of the module the child condition is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_CHILD_CREATE(name)			\
  if (!name ## _con) {							\
    name ## _con = new VARS_CLASS_VAR_LOCAL(name, COND)(this);		\
    /* name ## _con->setParent(this); */				\
  }
/**
   Utility to initialize a child condition by copying the child of
     another condition.
   \param name Name of the module the child condition is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_CHILD_CREATE_OTHER(name)		\
  name ## _con = new VARS_CLASS_VAR_LOCAL(name, COND)(other.name ## _con); \
  name ## _con->setParent(this)
/**
   Utility to initialize a child condition from another child condition.
   \param name Name of the module the child condition is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_CHILD_CREATE_OTHER_CHILD(name)       \
  if (name ## _other->parent == nullptr) {				\
    name ## _con = name ## _other;					\
  } else {								\
    name ## _con = new VARS_CLASS_VAR_LOCAL(name, COND)(name ## _other); \
  }									\
  name ## _con->setParent(this)

/**
   Utility to clear a child condition.
   \param name Name of the module the child condition is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_CHILD_CLEAR(name)			\
  if (name ## _con != nullptr) {					\
    delete name ## _con;						\
    name ## _con = nullptr;						\
  }
/**
   Utility to get the size of a child condition.
   \param name Name of the module the child condition is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_CHILD_SIZE(name)			\
  VARS_CLASS_VAR_LOCAL(name, COND)::size()
/**
   Boiler plate for defining members of a composite condition class
     including those with specialized methods (RedoxReg).
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_BASE(name)                           \
  DEFINE_CONDITION_BASE(name);                                          \
  VARS_CLASS_VAR_LOCAL(name, COND)& VARS_CLASS_VAR_LOCAL(name, COND)::  \
    operator=(const VARS_CLASS_VAR_LOCAL(name, COND)& other) {          \
      initMembers();							\
      FOR_EACH(DEFINE_CONDITION_COMPOSITE_CHILD_CREATE_OTHER,		\
               EXPAND(CHILDREN_ ## name));				\
      copyMembers(other);						\
      return *this;                                                     \
  }
/**
   Boiler plate for defining members of a composite condition class.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE(name)                                \
  DEFINE_CONDITION_COMPOSITE_BASE(name);                                \
  void VARS_CLASS_VAR_LOCAL(name, COND)::                               \
    _createChildren() {                                                 \
      FOR_EACH(DEFINE_CONDITION_COMPOSITE_CHILD_CREATE,                 \
               EXPAND(CHILDREN_ ## name));                              \
  }                                                                     \
  void VARS_CLASS_VAR_LOCAL(name, COND)::_reset(const bool) {           \
      count = 0;							\
  }                                                                     \
  void VARS_CLASS_VAR_LOCAL(name, COND)::_clear() {                     \
      FOR_EACH(DEFINE_CONDITION_COMPOSITE_CHILD_CLEAR,                  \
               EXPAND(CHILDREN_ ## name));                              \
      count = 0;                                                        \
  }
  
#define DEFINE_CONDITION_COMPOSITE2(name)				\
  VARS_CLASS_VAR_LOCAL(name, COND)::					\
    VARS_CLASS_VAR_LOCAL(name, COND)(FOR_EACH_COMMA(DECLARE_CONDITION_COMPOSITE_CHILD_VARS, \
						    EXPAND(CHILDREN_ ## name))) { \
      initMembers();							\
      FOR_EACH(DEFINE_CONDITION_COMPOSITE_CHILD_CREATE_OTHER_CHILD,	\
               EXPAND(CHILDREN_ ## name));				\
  }									\
  std::size_t VARS_CLASS_VAR_LOCAL(name, COND)::_size() {		\
      count = FOR_EACH_JOIN(DEFINE_CONDITION_COMPOSITE_CHILD_SIZE, +,	\
                            CHILDREN_ ## name);				\
      return count;							\
  }
/**
   Boiler plate for defining members of a composite condition class in the
     header.
   \param name Name of the module the condition class is associated with.
 */
#define DEFINE_CONDITION_COMPOSITE_HEADER(name)			\
  DEFINE_VALUE_SET_HEADER(VARS_CLASS_VAR_LOCAL(name, COND))
