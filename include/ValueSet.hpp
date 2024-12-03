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
#include <iomanip>

#define CHECK_RATIO_IDX(act, exp, key)					\
  if (it->first == key && act != exp) {					\
    throw std::runtime_error("CHECK[" + utils::enum_key2string(key)	\
			     + "]: expected = " + std::to_string(exp)	\
			     + ", actual = " + std::to_string(act));	\
  }

namespace ePhotosynthesis {

#define CALL_CHILD_METHOD(child, method, args)				\
  child->method args
#define DO_VALUE_SET_CHILD_CLASSES_MACRO(macro, iter, ...)		\
  if (!noChildren) {							\
    size_t iChild = 0;							\
    for (std::vector<ValueSetClass_t*>::iter it = child_classes.begin(); \
	 it != child_classes.end(); it++, iChild++) {			\
      if (*it) {							\
	macro((*it), __VA_ARGS__);					\
      }									\
    }									\
  }
#define DO_VALUE_SET_CHILDREN_MACRO(macro, iter, ...)			\
  if (!noChildren) {							\
    size_t iChild = 0;							\
    for (std::vector<ValueSet_t**>::iter it = children.begin();		\
	 it != children.end(); it++, iChild++) {			\
      if ((*it) && (**it)) {						\
	macro((**it), __VA_ARGS__);					\
      }									\
    }									\
  }
#define DO_VALUE_SET_CHILD_CLASSES(method, args)			\
  DO_VALUE_SET_CHILD_CLASSES_MACRO(CALL_CHILD_METHOD, iterator,		\
				   method, args)
#define DO_VALUE_SET_CHILD_CLASSES_CONST(method, args)			\
  DO_VALUE_SET_CHILD_CLASSES_MACRO(CALL_CHILD_METHOD, const_iterator,	\
				   method, args)
#define DO_VALUE_SET_CHILDREN(method, args)				\
  DO_VALUE_SET_CHILDREN_MACRO(CALL_CHILD_METHOD, iterator,		\
			      method, args)
#define DO_VALUE_SET_CHILDREN_CONST(method, args)			\
  DO_VALUE_SET_CHILDREN_MACRO(CALL_CHILD_METHOD, const_iterator,	\
			      method, args)

  inline bool __values_equal(const double& a, const double& b) {
    if (std::isnan(a) && std::isnan(b))
      return true;
    double relative_max = std::max(std::fabs(a), std::fabs(b));
    double relative = std::fabs(a - b);
    if (relative_max > COMPARE_RELATIVE_EPSILON)
      relative = relative / relative_max;
    double absolute = std::fabs(a - b);
    return ((relative <= COMPARE_RELATIVE_EPSILON) &&
	    (absolute <= COMPARE_ABSOLUTE_EPSILON));
  }
  inline double __extract_value(const double& x) { return x; }
  inline double __extract_value(const double* x) { return *x; }
  template<typename T>
  bool __contains(const std::vector<T>& v, const T& x) {
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); it++){
      if ((*(it)) == x) break;
    }
    return (it != v.end());
  }
  template<typename T>
  std::string __make_string(const T& x,
			    typename std::enable_if<!std::is_same<T, const char[]>::value >::type* = 0)
  { return std::to_string(x); }
  inline std::string __make_string(const std::string& x) { return x; }
  inline std::string __make_string(const char* x) { return std::string(x); }
  template<size_t N>
  inline std::string __make_string(const char x[N]) { return std::string(x); }

#define CONTEXT_VALUE_SET std::string(__func__) + ": "
#define ERROR_VALUE_SET(...)			\
    throw std::runtime_error(error_prefix() + std::string(__func__) + ": " + FOR_EACH_JOIN(__make_string, SEP_ADD, __VA_ARGS__))
#define ERROR_VALUE_SET_NESTED(err, msg)				\
  throw std::runtime_error(std::string(err.what()) + msg)
#define ERROR_VALUE_SET_NOARGS() ERROR_VALUE_SET("")
#define INFO_VALUE_SET_MSG(x)			\
  std::cerr << x
#define INFO_VALUE_SET_(...)			\
  FOR_EACH(INFO_VALUE_SET_MSG, __VA_ARGS__)
#define INFO_VALUE_SET(...)						\
  INFO_VALUE_SET_(error_prefix(), ": ", __func__, ": ",		\
		   __VA_ARGS__, std::endl)
#define INFO_VALUE_SET_NOARGS						\
  INFO_VALUE_SET_(error_prefix(), ": ", __func__, std::endl)
#define INFO_VALUE_SET_RAW(...)		\
  INFO_VALUE_SET_(__VA_ARGS__, "")
#define INFO_VALUE_SET_END(...)		\
  INFO_VALUE_SET_(__VA_ARGS__, std::endl)
#define INFO_VALUE_SET_NOEND(...)			\
  INFO_VALUE_SET_(error_prefix(), ": ", __func__, ": ", __VA_ARGS__)
  
#ifdef VERBOSE_VALUE_SET_DEBUG
#define DEBUG_VALUE_SET(...) INFO_VALUE_SET(__VA_ARGS__)
#define DEBUG_VALUE_SET_NOARGS INFO_VALUE_SET_NOARGS
#define DEBUG_VALUE_SET_RAW(...) INFO_VALUE_SET_RAW(__VA_ARGS__)
#define DEBUG_VALUE_SET_END(...) INFO_VALUE_SET_END(__VA_ARGS__)
#define DEBUG_VALUE_SET_NOEND(...) INFO_VALUE_SET_NOEND(__VA_ARGS__)
#else
#define DEBUG_VALUE_SET(...)
#define DEBUG_VALUE_SET_NOARGS
#define DEBUG_VALUE_SET_RAW(...)
#define DEBUG_VALUE_SET_END(...)
#define DEBUG_VALUE_SET_NOEND(...)
#endif // VERBOSE_VALUE_SET_DEBUG

#define VALUE_SET_PARENT(cls, par, mod, pt) ValueSet<cls, par, mod, pt>
#define VALUE_SET_STATIC_PARENT(cls, par, mod, pt) ValueSetStatic<cls, par, mod, pt>
  
#define DECLARE_VALUE_SET_MEMBER_RECORD			\
  static const StaticMemberClass _adjustments
#define DEFINE_VALUE_SET_MEMBER_RECORD(prefix, name)			\
  const typename prefix name::StaticMemberClass prefix name::_adjustments = \
    prefix name::StaticMemberClass()
#define DEFINE_VALUE_SET_MEMBER_RECORD_HEADER(prefix, name)

  // Non-static value set
#ifdef VALUE_SET_VAL_MEMBERS
#define INIT_VALUE_SET_MEMBER(name)				\
  ValueSetClass::insertOrig(EnumClass::name, name.getPtr(), "INIT_VALUE_SET_MEMBER: ")
#else // VALUE_SET_VAL_MEMBERS
#define INIT_VALUE_SET_MEMBER(name)				\
  ValueSetClass::insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_MEMBER: ")
#endif // VALUE_SET_VAL_MEMBERS
#define COPY_VALUE_SET_MEMBER(name)		\
  if (!isSkipped(EnumClass::name))		\
    name = other.name

#define DECLARE_VALUE_SET_CORE(name)				\
  typedef _valueSetStaticMember<name> StaticMemberClass;	\
  DECLARE_VALUE_SET_MEMBER_RECORD;
#define DECLARE_VALUE_SET_MEMBERS(name)				\
  FOR_EACH(DECLARE_VALUE_SET_MEMBER, EXPAND(MEMBERS_ ## name));	\
  void _initMemberPointers() override {				\
    FOR_EACH(INIT_VALUE_SET_MEMBER, EXPAND(MEMBERS_ ## name));	\
  }								\
  void _copyMembers(const name& other) override {		\
    FOR_EACH(COPY_VALUE_SET_MEMBER, EXPAND(MEMBERS_ ## name));	\
    __copyMembers(other);					\
  }								\
  DECLARE_VALUE_SET_CORE(name)
#define DECLARE_VALUE_SET(name, ...)		\
  DECLARE_VALUE_SET_BASE(name, __VA_ARGS__)	\
  friend ValueSetBase;				\
  friend ValueSetClass;				\
  using __VA_ARGS__::_initStaticMembers;	\
  using __VA_ARGS__::_initChildClasses;		\
  using __VA_ARGS__::_initChildren;		\
  DECLARE_VALUE_SET_MEMBERS(name)
#define DECLARE_VALUE_SET_COMPOSITE_CLASS_CHILD(child)			\
  addChildClass<child>()
#define DECLARE_VALUE_SET_COMPOSITE_CLASS_CHILDREN(...)			\
  FOR_EACH(DECLARE_VALUE_SET_COMPOSITE_CLASS_CHILD, __VA_ARGS__)
#define DECLARE_VALUE_SET_COMPOSITE_INIT_CHILD(child)			\
  addChild((ValueSet_t**)(&child))
#define DECLARE_VALUE_SET_COMPOSITE(name, children, childvars, ...)	\
  DECLARE_VALUE_SET_BASE(name, __VA_ARGS__)				\
  using __VA_ARGS__::_initStaticMembers;				\
  DECLARE_VALUE_SET_MEMBERS(name);					\
  static void _initChildClasses() {					\
    DECLARE_VALUE_SET_COMPOSITE_CLASS_CHILDREN children;		\
  }									\
  void _initChildren() override {					\
    FOR_EACH(DECLARE_VALUE_SET_COMPOSITE_INIT_CHILD, EXPAND childvars);	\
  }
  
#define DEFINE_VALUE_SET_MEMBERS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_NS(mod, name)			\
  DEFINE_VALUE_SET_MEMBERS(mod, name)
#define DEFINE_VALUE_SET(name)			\
  DEFINE_VALUE_SET_NS(, name)
#define DEFINE_VALUE_SET_HEADER(name)
#define DEFINE_VALUE_SET_SRC(name)  // Only used by conditions
  
  // Static value set
#define INIT_VALUE_SET_STATIC_MEMBER(name)				\
  ValueSetClass::insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_STATIC_MEMBER: ")
#define DEFINE_VALUE_SET_STATIC_MEMBERS_(name, ...)	\
  FOR_EACH_WITH_ARGS(DEFINE_VALUE_SET_STATIC_MEMBER,	\
		     (name), __VA_ARGS__)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_NS(mod, name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_(name, MEMBERS_ ## name);	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS(name)			\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NS(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name, children) \
  DEFINE_VALUE_SET_STATIC_MEMBERS_(name, MEMBERS_ ## name);		\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name);				\
  void mod name::_initChildClasses() {					\
    DECLARE_VALUE_SET_COMPOSITE_CLASS_CHILDREN children;		\
  }
  
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE(name, children)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(, name, children)
#define DEFINE_VALUE_SET_STATIC_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC(name)		\
  DEFINE_VALUE_SET_STATIC_NS(, name)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(mod, name, children)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name, children)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE(name, children)	\
  DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(, name, children)
#define DEFINE_VALUE_SET_STATIC_HEADER_NS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD_HEADER(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER(name)		\
  DEFINE_VALUE_SET_STATIC_HEADER_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE(name)	\
  DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(, name)
  
#define DECLARE_VALUE_SET_STATIC_MEMBERS(name)				\
  public:								\
  static void _initMemberPointers() {					\
    FOR_EACH(INIT_VALUE_SET_STATIC_MEMBER, EXPAND(MEMBERS_ ## name));	\
  }									\
  private:								\
  FOR_EACH(DECLARE_VALUE_SET_STATIC_MEMBER, EXPAND(MEMBERS_ ## name));	\
  public:
#define DECLARE_VALUE_SET_STATIC_CORE(name, ...)		\
  DECLARE_VALUE_SET_STATIC_BASE(name, __VA_ARGS__)		\
  typedef _valueSetStaticMember<name> StaticMemberClass;	\
  friend Value<name>;						\
  friend _valueSetStaticMember<name>;				\
  DECLARE_VALUE_SET_MEMBER_RECORD;
#define DECLARE_VALUE_SET_STATIC(name, ...)			\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)		\
  DECLARE_VALUE_SET_STATIC_MEMBERS(name)
#define DECLARE_VALUE_SET_STATIC_COMPOSITE(name, children, ...)		\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)			\
  DECLARE_VALUE_SET_STATIC_MEMBERS(name);				\
  static void _initChildClasses();

#define ENSURE_VALUE_POINTERS			\
  initMemberPointers()
#define ENSURE_VALUE_POINTERS_CONST
#define ENSURE_CHILD_POINTERS			\
  initChildren();				\
  createChildren()
#define ENSURE_CHILD_CLASS_POINTERS		\
  BaseClass::initChildClasses()

  template<typename T>
  class _valueSetStaticMember {
  public:
    _valueSetStaticMember() {
      T::initStaticMembers();
    }
    ~_valueSetStaticMember() {
      T::cleanupStaticMembers();
    }
  };

#define VALUE_X_PTR 1
  template<typename T>
  class Value {
  public:
    enum VALUE_FLAGS : int {
      VALUE_FLAG_INIT      = 0x00000001, //!< Value has been initialized
      VALUE_FLAG_SKIPPED   = 0x00000002, //!< Value has been skipped
      VALUE_FLAG_INIT_ONCE = 0x00000004, //!< Value should only be initialized once
      VALUE_FLAG_CONSTANT  = 0x00000008, //!< Value is constant and cannot be modified after init
      VALUE_FLAG_CALC      = 0x00000010, //!< Value is calculated so initializing the value directly has no effect
    };

#ifdef VALUE_X_PTR
    Value(typename T::EnumType k0, double& v,
	  const std::string& context="") : x(&v), k(k0), ctx(context) {}
    Value(typename T::EnumType k0, double* v,
	  const std::string& context="") : x(v), k(k0), ctx(context) {}
    double* getPtr() { return x; }
    const double* getPtr() const { return x; }
    double* x = nullptr;
#else // VALUE_X_PTR
    Value(typename T::EnumType k0=T::EnumClass::NONE,
	  const std::string& context="") : x(0.0), k(k0), ctx(context) {}
    Value(typename T::EnumType k0, const double& v,
	  const std::string& context="") : x(v), k(k0), ctx(context) {}
    double* getPtr() { return &x; }
    const double* getPtr() const { return &x; }
    double x = 0.0;
#endif // VALUE_X_PTR
    double x0 = 0.0;
    typename T::EnumType k;
    std::string ctx;
    int flags = 0;
    static std::string error_prefix() {
      return T::error_prefix();
    }
    double get(const std::string& context="") const {
#ifdef VALUE_X_PTR
      DEBUG_VALUE_SET(context, "(", ctx, "): get[", k, ", ", x,
		      "] = ", *x);
      return *x;
#else // VALUE_X_PTR
      return x;
#endif // VALUE_X_PTR
    }
    void set(const double& y, const std::string& context="") {
      T::checkNotConstant(k, context + "(" + ctx + "): set: ");
#ifdef VALUE_X_PTR
      DEBUG_VALUE_SET(context, "(", ctx, "): set[", k, ", ", x, "]: ",
		      *x, " -> ", y);
      x[0] = y;
#else // VALUE_X_PTR
      x = y;
#endif // VALUE_X_PTR
    }
#define CONV_OP(op)				\
    operator op() const {			\
      return op(get(#op));			\
    }
#define ASSIGN_OP(op)				\
    template<typename T2>			\
    Value& operator op(const Value<T2>& b) {	\
      double y = get(#op);			\
      y op b.get("rhs:" #op);			\
      set(y, #op);				\
      return *this;				\
    }						\
    template<typename T2>			\
    Value& operator op(const T2& b) {		\
      double y = get(#op);			\
      y op b;					\
      set(y, #op);				\
      return *this;				\
    }
#define BINARY_OP(ret, op)						\
    template<typename T2>						\
    friend ret operator op(const Value<T>& a, const Value<T2>& b) {	\
      return (a.get("lhs:" #op) op b.get("rhs:" #op));			\
    }									\
    template<typename T2>						\
    friend ret operator op(const Value<T>& a, const T2& b) {		\
      return (a.get("lhs:" #op) op b);					\
    }
#define ARITH_OP2(op)						\
    ASSIGN_OP(op ## =)						\
    BINARY_OP(double, op)

    CONV_OP(double)
    ASSIGN_OP(=)
    BINARY_OP(bool, <)
    BINARY_OP(bool, <=)
    BINARY_OP(bool, >)
    BINARY_OP(bool, >=)
    // BINARY_OP(bool, ==)
    // BINARY_OP(bool, !=)
    ARITH_OP2(+)
    ARITH_OP2(-)
    ARITH_OP2(*)
    ARITH_OP2(/)
    ARITH_OP2(%)
#undef CONV_OP
#undef ARITH_OP2
#undef BINARY_OP
#undef ASSIGN_OP
    template<typename T2>
    friend bool operator==(const Value<T>& a, const Value<T2>& b) {
      return __values_equal(a.get("lhs: =="), b.get("rhs: =="));
    }
    template<typename T2>
    friend bool operator==(const Value<T>& a, const T2& b) {
      return __values_equal(a.get("lhs: =="), b);
    }
    template<typename T2>
    friend bool operator!=(const Value<T>& a, const T2& b) {
      return !(a == b);
    }
  };

#define STUB_STATIC_VALUE_SET(name, retT, argsT, args, retV, suffix)	\
  static retT _static_ ## name argsT {					\
    return name args;							\
  }
#define STUB_VIRTUAL_VALUE_SET(name, retT, argsT, args, retV, suffix)	\
  retT _virtual_ ## name argsT suffix override {			\
    return T::name args;						\
  }

/**
   Use macro to define methods only in ValueSet
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_INST_ONLY(method)			\
  /** \copydoc ValueSet::copyMembers */					\
  method(copyMembers, void,						\
	 (const ValueSet_t& other, const bool noChildren=false),	\
	 (other, noChildren), , )					\
  /** \copydoc ValueSet::initChildren */				\
  method(initChildren, void,						\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::createChildren */				\
  method(createChildren, void,						\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::getChildren */					\
  method(getChildren, const std::vector<ValueSet_t**>&,			\
         (), (), {}, const)						\
  /** \copydoc ValueSet::resetInstance */				\
  method(resetInstance, void,						\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::setParent */					\
  method(setParent, void, (ValueSet_t* par), (par), , )

/**
   Use macro to define static methods only in ValueSet without duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC_NODUP(method)	\
  /** \copydoc ValueSet::newInstance */				\
  method(newInstance, ValueSet_t*,				\
	 (ValueSet_t* par=nullptr), (par), nullptr, )		\
  method(getValueSetClass, ValueSet_t*,				\
	 (), (), nullptr, )
/**
   Use macro to define static methods only in ValueSet with duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(method)	\
  ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC_NODUP(method)

/**
   Use macro to define methods only in ValueSetStatic
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_STATIC_ONLY(method)	\
  /** \copydoc ValueSetEnum::error_prefix */		\
  method(error_prefix, std::string, (), (), "", const)
  
/**
   Use macro to define methods static in ValueSet & ValueSetStatic
     without duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_STATIC_NODUP(method)			\
  /** \copydoc ValueSetBase::get_module */				\
  method(get_module, MODULE, (), (), MODULE_NONE, const)		\
  /** \copydoc ValueSetBase::get_param_type */				\
  method(get_param_type, PARAM_TYPE, (), (), PARAM_TYPE_NONE, const)	\
  /** \copydoc ValueSetBase::get_parameter_types */			\
  method(get_parameter_types,						\
	 const std::vector<PARAM_TYPE>&, (), (), {}, const)		\
  /** \copydoc ValueSetBase::initDefaults */				\
  method(initDefaults, void,						\
	 (const bool useC3=false, const std::string& filename="",	\
	  const bool force=false, const bool noChildren=false),		\
	 (useC3, filename, force, noChildren), , const)			\
  /** \copydoc ValueSetBase::memberCount */				\
  method(memberCount, std::size_t, (const bool noChildren=false),	\
	 (noChildren), 0, const)					\
  /** \copydoc ValueSetBase::memberState */				\
  method(memberState, std::string, (const bool noChildren=false),	\
	 (noChildren), "", const)					\
  /** \copydoc ValueSetBase::initChildClasses */			\
  method(initChildClasses, void, (const bool noChildren=false),		\
	 (noChildren), , const)						\
  /** \copydoc ValueSetBase::cleanupChildClasses */			\
  method(cleanupChildClasses, void, (const bool noChildren=false),	\
	 (noChildren), , const)						\
  /** \copydoc ValueSetBase::initStaticMembers */			\
  method(initStaticMembers, void, (const bool noChildren=false),	\
	 (noChildren), , const)						\
  /** \copydoc ValueSetBase::cleanupStaticMembers */			\
  method(cleanupStaticMembers, void, (const bool noChildren=false),	\
	 (noChildren), , const)						\
  /** \copydoc ValueSetBase:selected */					\
  method(selected, bool, (), (), false, const)				\
  /** \copydoc ValueSetBase::select */					\
  method(select, void,							\
	 (const bool x = true, const bool noChildren = false),		\
	 (x, noChildren), , const)					\
  /** \copydoc ValueSetBase::enableC3 */				\
  method(enableC3, void,						\
	 (const bool x = true, const bool noChildren = false),		\
	 (x, noChildren), , const)					\
  /** \copydoc ValueSetBase::reset */					\
  method(reset, void, (const bool noChildren = false),			\
	 (noChildren), , const)						\
  /** \copydoc ValueSetBase::size */					\
  method(size, std::size_t, (), (), 0, const)				\
  /** \copydoc ValueSetBase::max_field_width */				\
  method(max_field_width, std::size_t,					\
	 (bool noChildren = false), (noChildren), 0, const)		\
  /** \copydoc ValueSetBase::max_default_value_width */			\
  method(max_default_value_width, std::size_t,				\
	 (bool noChildren = false), (noChildren), 0, const)		\
  /** \copydoc ValueSetBase::has */					\
  method(has, bool,							\
	 (const std::string& name, const bool& isGlymaID = false),	\
	 (name, isGlymaID), false, const)				\
  /** \copydoc ValueSetBase::fromNameWithAliases */			\
  method(fromNameWithAliases, int,					\
	 (const std::string& name, const bool& isGlymaID = false),	\
	 (name, isGlymaID), -1, const)					\
  /** \copydoc ValueSetBase::setDefault */				\
  method(setDefault, void,						\
	 (const std::string& k, const double& v,			\
	  const bool& isGlymaID = false,				\
	  const bool dontPreserve = false),				\
	 (k, v, isGlymaID, dontPreserve), , const)			\
  /** \copydoc ValueSetBase::getDefault */				\
  method(getDefault, double,						\
	 (const std::string& x, const bool& isGlymaID = false),		\
	 (x, isGlymaID), 0.0, const)
/**
   Use macro to define methods static in ValueSet & ValueSetStatic
     with duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_STATIC(method)				\
  ADD_METHODS_VALUE_SET_T_STATIC_NODUP(method)				\
  /** \copydoc ValueSetBase::setDefault */				\
  method(setDefault, void,						\
	 (const int& k, const double& v,				\
	  const bool dontPreserve = false),				\
	 (k, v, dontPreserve), , const)					\
  /** \copydoc ValueSetBase::getDefault */				\
  method(getDefault, double,						\
	 (const int& k), (k), 0.0, const)

/**
   Use macros to define methods static in ValueSetStatic and not static
     in ValueSet without duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_BOTH_NODUP(method)			\
  /** \copydoc ValueSet::getValueSet */					\
  method(getValueSet, const ValueSet_t*, (), (),			\
	 nullptr, const)						\
  /** \copydoc ValueSet:getValueMap */					\
  method(getValueMap, std::map ADD_BRACKETS(int, double),		\
	 (const bool preinit=false), (preinit), {}, const)		\
  /** \copydoc ValueSet:setValueMap */					\
  method(setValueMap, void,						\
	 (const std::map<int, double>& map, const bool setinit=false,	\
	  const bool preinit=false),					\
	 (map, setinit, preinit), , )					\
  /** \copydoc ValueSet::max_value_width */				\
  method(max_value_width, std::size_t,					\
	 (bool noChildren = false), (noChildren), 0, const)		\
  /** \copydoc ValueSet::initMembers */					\
  method(initMembers, void,						\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::resetValues */					\
  method(resetValues, void,						\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::initialized */					\
  method(initialized, bool,						\
	 (), (), false, const)						\
  /** \copydoc ValueSet::initValues */					\
  method(initValues, void,						\
	 (const bool noDefaults=false, const bool force=false,		\
	  const bool noChildren=false),					\
	 (noDefaults, force, noChildren), , )				\
  /** \copydoc ValueSet::initMemberPointers */				\
  method(initMemberPointers, void,					\
	 (const bool noChildren=false), (noChildren), , )		\
  /** \copydoc ValueSet::diff */					\
  method(diff, std::string,						\
	 (const ValueSet_t& other, std::size_t padKeys=0,		\
	  std::size_t padVals=0, bool includePrefixes=false,		\
	  bool noChildren = false),					\
	 (other, padKeys, padVals, includePrefixes, noChildren),	\
	 "", const)							\
  /** \copydoc ValueSet::print */					\
  method(print, std::ostream&,						\
	 (std::ostream &out, const uint tab = 0, std::size_t pad = 0,	\
	  bool includePrefixes = false,					\
	  bool includeSkipped = false,					\
	  const std::vector<std::string>& skip_keys = {},		\
	  const std::map<std::string, std::string>& key_aliases = {},	\
	  bool noChildren = false),					\
	 (out, tab, pad, includePrefixes, includeSkipped,		\
	  skip_keys, key_aliases, noChildren),				\
	 out, const)							\
  /** \copydoc ValueSet::sizeArray */					\
  method(sizeArray, std::size_t, (), (), 0, const)			\
  /** \copydoc ValueSet::toArray */					\
  method(toArray, arr, (), (), {}, const)				\
  /** \copydoc ValueSet::fromArray */					\
  method(fromArray, void,						\
	 (const arr &vec, const std::size_t offset = 0),		\
	 (vec, offset), , )						\
  /** \copydoc ValueSetBase::set */					\
  method(set, void,							\
	 (const std::string& name, const double& v,			\
	  const bool& isGlymaID = false),				\
	 (name, v, isGlymaID), , )					\
  /** \copydoc ValueSetBase::get */					\
  method(get, double,							\
	 (const std::string& name, const bool& isGlymaID = false),	\
	 (name, isGlymaID), 0.0, const)
/**
   Use macros to define methods static in ValueSetStatic and not static
     in ValueSet with duplicates
   \param method Macro to define each method.
*/
#define ADD_METHODS_VALUE_SET_T_BOTH(method)				\
  ADD_METHODS_VALUE_SET_T_BOTH_NODUP(method)				\
  /** \copydoc ValueSetBase::set */					\
  method(set, void,							\
	 (const int& k, const double& v), (k, v), , )			\
  /** \copydoc ValueSetBase::get */					\
  method(get, double,							\
	 (const int& k), (k), 0.0, const)

  /**
     Untemplated base class to allow storage of mixed value sets in
       containers.
   */
  class ValueSet_t {
  public:

    virtual ~ValueSet_t() {}

    template<typename T>
    static const T& castValueSet(const ValueSet_t& x) {
      if (typeid(const T&) != typeid(x))
	std::runtime_error("Cannot cast between types");
      return static_cast<const T&>(x);
    }

    /**
       Check if another value set is equivalent to this one.
       \param b Value set to compare against this one.
       \param noChildren If true, child classes will not be checked.
     */
    virtual bool equals(const ValueSet_t& b0,
			const bool noChildren = false) const {
      UNUSED(noChildren);
      if(typeid(*this) != typeid(b0)) {
	std::cerr << "typeids not equal in ValueSet_t" << std::endl;
	return false;
      }
      return true;
    }
    bool operator==(const ValueSet_t& b) const {
      return equals(b);
    }
    bool operator!=(const ValueSet_t& b) const {
      return !(this->operator==(b));
    }
    
    
#define ADD_METHOD(name, retT, argsT, args, retV, suffix)	\
    virtual retT name argsT suffix {				\
      FOR_EACH(UNUSED, EXPAND args);				\
      IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
    }
#define ADD_METHOD_BOTH(name, retT, argsT, args, retV, suffix)		\
    ADD_METHOD(name, PACK_MACRO(retT), argsT, args, retV, suffix)	\
    static retT _static_ ## name argsT {				\
      FOR_EACH(UNUSED, EXPAND args);					\
      IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
    }
#define ADD_METHOD_STATIC(name, retT, argsT, args, retV, suffix)	\
    virtual retT _virtual_ ## name argsT suffix {			\
      FOR_EACH(UNUSED, EXPAND args);					\
      IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
    }
    
    // Methods specific to classes
    ADD_METHODS_VALUE_SET_T_INST_ONLY(ADD_METHOD)
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(ADD_METHOD_STATIC)
    
    // Methods static in ValueSetStatic and not static in ValueSet
    /** \copydoc ValueSet::getValueSet */
    virtual ValueSet_t* getValueSet() {
      return const_cast<ValueSet_t*>(const_cast<const ValueSet_t*>(this)->getValueSet());
    }
    /** \copydoc ValueSet getChildren */
    virtual std::vector<ValueSet_t**>& getChildren() {
      return const_cast<std::vector<ValueSet_t**>&>(const_cast<const ValueSet_t*>(this)->getChildren());
    }
    ADD_METHODS_VALUE_SET_T_BOTH(ADD_METHOD_BOTH)
    
    // Methods static in both classes
    ADD_METHODS_VALUE_SET_T_STATIC(ADD_METHOD_STATIC)
    
#undef ADD_METHOD_BOTH
#undef ADD_METHOD
#undef ADD_METHOD_STATIC
  };

  /**
     Template free base class to allow packaging of value set classes in
       containers.
   */
  class ValueSetClass_t : public ValueSet_t {
  public:
#define ADD_METHOD_BOTH(name, ...) using ValueSet_t::name;
#define ADD_METHOD(name, retT, argsT, args, retV, suffix)	\
    virtual retT name argsT suffix {				\
      FOR_EACH(UNUSED, EXPAND args);				\
      IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
    }
#define DISABLE_METHOD(name, retT, argsT, args, retV, suffix)	\
    retT name argsT suffix override {				\
      FOR_EACH(UNUSED, EXPAND args);				\
      throw std::runtime_error(std::string(__func__) +		\
			       ": Not a static method");	\
      IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
    }
#define ADD_METHOD_STATIC(name, ...) using ValueSet_t::_virtual_ ## name;
    
    // Methods specific to classes
    ADD_METHODS_VALUE_SET_T_INST_ONLY(DISABLE_METHOD)
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC_NODUP(ADD_METHOD_STATIC)
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(ADD_METHOD)
    ADD_METHODS_VALUE_SET_T_STATIC_ONLY(ADD_METHOD)

    // Methods static in ValueSetStatic and not static in ValueSet
    ADD_METHODS_VALUE_SET_T_BOTH_NODUP(ADD_METHOD_BOTH)
    
    // Methods static in both classes
    ADD_METHODS_VALUE_SET_T_STATIC_NODUP(ADD_METHOD_STATIC)
    ADD_METHODS_VALUE_SET_T_STATIC(ADD_METHOD)

#undef ADD_METHOD_STATIC
#undef DISABLE_METHOD
#undef ADD_METHOD_BOTH
#undef ADD_METHOD
  };

  /**
     Wrapper class to allow packaging of value set classes in containers.
     \tparam T Class being wrapped.
   */
  template<typename T>
  class ValueSetClassType : public ValueSetClass_t {
  public:
    ~ValueSetClassType() override {}
#define ADD_METHOD(name, retT, argsT, args, retV, suffix)		\
    retT name argsT suffix override {					\
      return T::name args;						\
    }									\
    STUB_VIRTUAL_VALUE_SET(name, retT, argsT, args, retV, suffix)
#define ADD_METHOD_BOTH(name, retT, argsT, args, retV, suffix)	\
    retT name argsT suffix override {				\
      return T::_static_ ## name args;				\
    }

    // Methods specific to classes
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(ADD_METHOD)
    
    // Methods virtual in ValueSet & static in ValueSetStatic
    ADD_METHODS_VALUE_SET_T_BOTH(ADD_METHOD_BOTH)

    // Methods static in both classes
    ADD_METHODS_VALUE_SET_T_STATIC(ADD_METHOD)

#undef ADD_METHOD_BOTH
#undef ADD_METHOD
  };

  /**
     Wrapper class to allow packaging of static value set classes in
       containers.
     \tparam T Class being wrapped.
   */
  template<typename T>
  class ValueSetStaticClassType : public ValueSetClassType<T> {
  public:
#define ADD_METHOD(name, retT, argsT, args, retV, suffix)	\
    retT name argsT suffix override {				\
      return T::name args;					\
    }
#define DISABLE_METHOD(name, retT, argsT, args, retV, suffix)		\
    retT name argsT suffix override {					\
      FOR_EACH(UNUSED, EXPAND args);					\
      throw std::runtime_error(std::string(__func__) +			\
			       ": Invalid for static value set");	\
      return retV;							\
    }

    // Methods specific to classes
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(DISABLE_METHOD)
    ADD_METHODS_VALUE_SET_T_STATIC_ONLY(ADD_METHOD)

#undef DISABLE_METHOD
#undef ADD_METHOD
  };
  
  /**
     Template class to provide core methods for manipulating sets of
     values used by a model run that can be set via input files.

     \tparam T Child class with virtual & static methods.
     \tparam U Parent class with virtual & static methods.
     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  class ValueSetBase : public ValueSetEnum<ID, PT> {
  public:
    typedef T ThisClass; /** Inheriting class */
    typedef U ThisParent; /** Parent class */
    typedef ValueSetBase<T, U, ID, PT> BaseClass; /**< Specialized value set base class */
    typedef ValueSetEnum<ID, PT> EnumBaseClass; /**< Base class containing enum utilities */
    typedef typename EnumBaseClass::Type EnumType; /**< Enumerator type specifying keys for values in the set */
#ifdef EPHOTO_USE_SCOPED_ENUM
    typedef typename EnumBaseClass::Type EnumClass; /**< Enumerator class specifying keys for values in the set */
#else // EPHOTO_USE_SCOPED_ENUM
    typedef EnumBaseClass EnumClass; /**< Enumerator class specifying keys for values in the set */
#endif // EPHOTO_USE_SCOPED_ENUM
    typedef Value<BaseClass> ValueType; /**< Class for storing values */
    typedef typename std::map<EnumType, double*>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double*>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
    INHERIT_METHOD_ENUM(EnumBaseClass);

    /**
       Get a ValueSetStaticClassType instance for this class.
       \returns Reference to ValueSetStaticClassType instance.
     */
    static ValueSetClass_t* getValueSetClass() {
      static ValueSetStaticClassType<T> out;
      return &out;
    }
    /**
       Convert an integer to an enum.
       \param[in] k Integer key.
       \returns Enum key.
    */
    static EnumType int2key(const int& k) {
      if (k < 0)
	ERROR_VALUE_SET("Cannot convert a negative value to an enum");
      if (k > ValueSetEnum<ID, PT>::all.size())
	ERROR_VALUE_SET("Integer (", k,
			") exceeds maximum value in enum (",
			std::to_string(ValueSetEnum<ID, PT>::all.size()),
			")");
      return ValueSetEnum<ID, PT>::all[k - 1]; // NONE not in all
    }
    /**
       Convert a map with integer keys to a map with enum keys.
       \tparam V Type of value in map.
       \param[in] map Value map with integer keys.
       \returns Value map with enum keys.
    */
    template<typename V>
    static std::map<EnumType, V> int2key(const std::map<int, V>& map) {
      std::map<EnumType, V> out;
      for (typename std::map<int, double>::const_iterator it = map.begin();
	   it != map.end(); it++)
	out[int2key(it->first)] = it->second;
      return out;
    }
    /**
       Determine if a key is included in arrays.
       \param k Key to check.
       \returns true if the key is included in arrays, false otherwise.
     */
    static bool inArrays(const EnumType k) {
      return (!(isNonvector(k) || isSkipped(k)));
    }

    enum VS_FLAGS : int {
      VS_FLAG_SELECTED        = 0x00000001, //!< Value set selected by driver
      VS_FLAG_DEFAULTS_C3     = 0x00000002, //!< Defaults initialized for C3 run
      VS_FLAG_DEFAULTS_EDITED = 0x00000004, //!< Defaults editted
      VS_FLAG_INIT_DEFAULTS   = 0x00000008, //!< Defaults initialized
      VS_FLAG_INIT_POINTERS   = 0x00000010, //!< Pointers initialized
      VS_FLAG_INIT_VALUES     = 0x00000020, //!< Values initialized
      VS_FLAG_INIT_STATIC     = 0x00000040, //!< Static members initialized
      VS_FLAG_INIT_CHILDREN   = 0x00000080, //!< Children initialized
    };

    // Inspection utilities
    /**
       Get the maximum width of the key names in a value map.
       \tparam K Key type.
       \tparam V Type of value in the provided map.
       \param vals Value map.
       \param includePrefixes If true, module & parameter type prefixes
         should be included.
       \returns Maximum key name width.
     */
    template <typename K, typename V>
    static std::size_t field_width_value_map(const std::map<K, V>& vals,
					     bool includePrefixes=false) {
      std::size_t pad = 0;
      std::string name;
      for (typename std::map<K, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	name = getName(it->first);
	if (name.size() > pad)
	  pad = name.size();
      }
      if (includePrefixes) {
	pad += 4; // "::" between param_type/module/key
	pad += get_enum_names<PARAM_TYPE>().find(param_type)->second.size();
	pad += get_enum_names<MODULE>().find(module)->second.size();
      }
      return pad;
    }
    /**
       Get the maximum width of the values in a value map.
       \tparam K Key type.
       \tparam V Type of value in the provided map.
       \param vals Value map.
       \returns Maximum value width.
     */
    template <typename K, typename V>
    static std::size_t value_width_value_map(const std::map<K, V>& vals) {
      std::size_t pad = 7; // for "MISSING"
      std::string name;
      for (typename std::map<K, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	name = to_string_with_precision(it->second);
	if (name.size() > pad)
	  pad = name.size();
      }
      return pad;
    }
    /**
       Get the maximum width of the key names in the defaults map.
       \param noChildren If true, children won't be included when
         determining the maximum field width.
       \returns Maximum key name width.
     */
    static std::size_t max_field_width(bool noChildren = false) {
      static std::size_t out = 0;
      static std::size_t outBase = 0;
      if (outBase == 0) {
	outBase = field_width_value_map(defaults, true);
      }
      if (noChildren) return outBase;
      if (out == 0) {
	out = outBase;
	std::size_t iout = 0;
#define DO_CHILD(child, dummy)						\
	iout = child->max_field_width();				\
	if (iout > out) out = iout
	DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      }
      return out;
    }
    /**
       Get the maximum width of the values in the defaults map.
       \param noChildren If true, children won't be included when
         determining the maximum value width.
       \returns Maximum default value width.
     */
    static std::size_t max_default_value_width(bool noChildren = false) {
      static std::size_t out = 0;
      static std::size_t outBase = 0;
      if (outBase == 0) {
	outBase = value_width_value_map(defaults);
      }
      if (noChildren) return outBase;
      if (out == 0) {
	out = outBase;
	std::size_t iout = 0;
#define DO_CHILD(child, dummy)						\
	iout = child->max_default_value_width();			\
	if (iout > out) out = iout
	DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      }
      return out;
    }
    /**
       Output a single value to an output stream.
       \param val Value to ouput.
       \param out Stream to output to.
       \param show_pointers If true, the value pointer is output. If false,
         the value is output.
       \returns Output stream.
     */
    static std::ostream& print_value(const double& val, std::ostream &out,
				     bool show_pointers = false) {
      if (show_pointers)
	out << &val;
      else
	out << val;
      return out;
    }
    static std::ostream& print_value(const double* val, std::ostream &out,
				     bool show_pointers = false) {
      if (show_pointers)
	out << val;
      else
	out << *val;
      return out;
    }
    /**
       Get a string version of a value.
       \param val Value to ouput.
       \param show_pointers If true, the value pointer is output. If false,
         the value is output.
       \returns String
     */
    template<typename V>
    static std::string string_value(const V val,
				    bool show_pointers = false) {
      std::ostringstream out;
      print_value(val, out, show_pointers);
      return std::move(out).str();
    }
    /**
       Display the values in a value map.
       \tparam V Type of value in value map.
       \param vals Value map to display.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \param pad Number of characters that key names should be padded
         to fill.
       \param includePrefixes If true, the module & parameter type
         prefixes will be added to the member names.
       \param includeSkipped If true, skipped keys will be output.
       \param skip_keys Key strings to skip in output.
       \param key_aliases String aliases to use for keys.
       \param show_pointers If true, the pointers will be displayed
         instead of the values.
       \returns Output stream.
     */
    template <typename V>
    static std::ostream& print_value_map(const std::map<EnumType, V>& vals,
					 std::ostream &out,
					 const uint tab=0,
					 std::size_t pad=0,
					 bool includePrefixes=false,
					 bool includeSkipped=false,
					 const std::vector<std::string>& skip_keys={},
					 const std::map<std::string, std::string>& key_aliases={},
					 bool show_pointers=false) {
      const std::string space(tab * tab_size, ' ');
      check_value_map(vals, "print_value_map: ");
      if (pad == 0)
	pad = field_width_value_map(vals, includePrefixes);
      for (typename std::map<EnumType, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	if ((!includeSkipped) && (isSkipped(it->first) ||
				  isConstant(it->first)))
	  continue;
	std::string iname;
	if (includePrefixes)
	  iname = getNameWithPrefix(it->first);
	else
	  iname = getName(it->first);
	std::map<std::string, std::string>::const_iterator it_alias = key_aliases.find(iname);
	if (!__contains(skip_keys, iname)) {
	  out << space << std::setw(pad + tab_size) << std::left;
	  if (it_alias != key_aliases.end())
	    iname = it_alias->second;
	  out << iname;
	  print_value(it->second, out, show_pointers);
	  out << std::endl;
	}
      }
      return out;
    }
    template <typename V>
    static std::string to_string_with_precision(const V a_value,
						const int n = COMPARE_PRECISION) {
      std::ostringstream out;
      out.precision(n);
      out << std::fixed << a_value;
      return std::move(out).str();
    }
    /**
       Check if two values are equal.
       \param a First value for comparison.
       \param b Second value for comparison.
     */
    static bool valuesEqual(const double& a, const double& b) {
      return __values_equal(a, b);
    }
    static bool valuesEqual(const double* a, const double* b) {
      return valuesEqual(*a, *b);
    }
    static bool valuesEqual(const double& a, const double* b) {
      return valuesEqual(a, *b);
    }
    static bool valuesEqual(const double* a, const double& b) {
      return valuesEqual(*a, b);
    }
    static bool valuesEqual(const Value<T>& a, const Value<T>& b) {
      return valuesEqual(a.x, b.x);
    }
    template<typename V>
    static bool valuesEqual(const Value<T>& a, const V& b) {
      return valuesEqual(a.x, b);
    }
    template<typename V>
    static bool valuesEqual(const V& a, const Value<T>& b) {
      return valuesEqual(a, b.x);
    }
    /**
       Convert a value to a string.
       \param a Value to make into a string.
       \returns String version of value.
     */
    static std::string valueString(const Value<T>& a) {
      return valueString(a.x);
    }
    static std::string valueString(const double& a) {
      return std::to_string(a);
    }
    static std::string valueString(const double* a) {
      if (!a)
	return "NULL";
      return std::to_string(*a);
    }
    /**
       Convert a value pointer to a string.
       \param a Value to make into a string.
       \returns String version of value.
     */
    static std::string valuePointerString(const double& a) {
      return "N/A";
    }
    static std::string valuePointerString(const double* a) {
      std::ostringstream oss;
      oss << a;
      return oss.str();
    }
    static std::string valuePointerString(const Value<T>& a) {
      return valuePointerString(a.x);
    }
    /**
       Set a value.
       \param dst Destination for value.
       \param src Source to copy into value.
       \param context Context to use for error messages.
     */
    static void valueSet(double& dst, const double& src,
			 const std::string& context = "") {
      UNUSED(context);
      dst = src;
    }
    static void valueSet(double& dst, const double* src,
			 const std::string& context = "") {
      if (!src)
	ERROR_VALUE_SET(context, "source pointer null");
      dst = src[0];
    }
    static void valueSet(double* dst, const double& src,
			 const std::string& context = "") {
      if (!dst)
	ERROR_VALUE_SET(context, "destination pointer null");
      dst[0] = src;
    }
    static void valueSet(double* dst, const double* src,
			 const std::string& context = "") {
      if (!dst)
	ERROR_VALUE_SET(context, "pointer null");
      if (!src)
	ERROR_VALUE_SET(context, "source pointer null");
      dst[0] = src[0];
    }
    template<typename V>
    static void valueSet(Value<T>& dst, const V& src,
			 const std::string& context = "") {
      UNUSED(context);
      dst = src;
    }
    /**
       Get value contents.
       \param[in] x Value to get contents of.
       \returns Value contents.
     */
    static double valueGet(const double* x) { return *x; }
    static double valueGet(const double& x) { return x; }
    static double valueGet(const Value<T>& x) { return valueGet(x.x); }
    /**
       Insert a value into a value set map.
       \param[in, out] valsDst Value set map to add value to.
       \param[in] k Key where value should be inserted.
       \param[in] src Value to insert.
       \param[in] context Context for error messages.
     */
    static void insertValue(std::map<EnumType, double*>& valsDst,
			    const EnumType& k, const double* src,
			    const std::string& context = "") {
      if (!src)
	ERROR_VALUE_SET(context, "source pointer null");
      valsDst[k] = const_cast<double*>(src);
    }
    static void insertValue(std::map<EnumType, double>& valsDst,
			    const EnumType& k, const double& src,
			    const std::string& context = "") {
      UNUSED(context);
      valsDst[k] = src;
    }
    static void insertValue(std::map<EnumType, double>& valsDst,
			    const EnumType& k, const double* src,
			    const std::string& context = "") {
      if (!src)
	ERROR_VALUE_SET(context, "source pointer null");
      valsDst[k] = src[0];
    }
    static void insertValue(std::map<EnumType, double*>&,
			    const EnumType& k, const double&,
			    const std::string& context = "") {
      ERROR_VALUE_SET(context, "No value for \'", getName(k), "\'");
    }
    template<typename V>
    static void insertValue(std::map<EnumType, Value<T> >& valsDst,
			    const EnumType& k, const V& src,
			    const std::string& context = "") {
      UNUSED(context);
      valsDst[k] = src;
    }
    /**
       Throw an error if two values are not equivalent.
       \param k Key that values correspond to.
       \param a First value for comparison.
       \param b Second value for comparison.
       \param context String to prefix the error message with.
     */
    static void compareValues(const EnumType k,
			      const double& a, const double& b,
			      const std::string& context = "") {
      if (std::isnan(a) && std::isnan(b))
	return;
      double relative_max = std::max(std::fabs(a), std::fabs(b));
      double relative = std::fabs(a - b);
      if (relative_max > COMPARE_RELATIVE_EPSILON)
	relative = relative / relative_max;
      double absolute = std::fabs(a - b);
      if (!((relative <= COMPARE_RELATIVE_EPSILON) &&
	    (absolute <= COMPARE_ABSOLUTE_EPSILON))) {
	std::string msg = "Values for " +
	  utils::enum_key2string(module) + " " +
	  utils::enum_key2string(param_type) + " " +
	  getName(k) +
	  " differ: " +
	  to_string_with_precision(a) + " vs " +
	  to_string_with_precision(b) +
	  " (absolute = " + to_string_with_precision(absolute) +
	  ", relative = " + to_string_with_precision(relative) + ")";
	if (!context.empty())
	  msg = context + ": " + msg;
	ERROR_VALUE_SET(msg);
      }
    }
    /**
       Throw an error if a map is empty;
       \tparam V Value map type.
       \param vals Value map to check.
       \param context String to prefix the error message with.
     */
    template<typename V>
    static void check_value_map(const std::map<EnumType, V>& vals,
				const std::string& context = "") {
      if (vals.empty() && !defaults.empty()) {
	ERROR_VALUE_SET(context, "Value map empty");
      }
    }
    /**
       Throw an error if a key is not present in a value map.
       \tparam V Value map type.
       \param vals Value map to check.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    template<typename V>
    static void check_value_map(const std::map<EnumType, V>& vals,
				const EnumType k,
				const std::string& context = "") {
      typename std::map<EnumType, V>::const_iterator it = vals.find(k);
      if (it == vals.end()) {
	std::string msg = "\'" +
	  getName(k) +
	  "\' not present";
	if (!context.empty())
	  msg = context + ": " + msg;
	ERROR_VALUE_SET(msg);
      }
    }
    // Value manipulation
    /**
       Remove skipped values from a value mpa.
       \tparam V Type of values in the map.
       \param val Map to remove skipped values from.
       \param context String providing context for error messages.
     */
    template<typename V>
    static void remove_skipped(std::map<EnumType, V>& vals,
			       const std::string& context="") {
      for (typename std::vector<EnumType>::const_iterator s = skipped.begin();
	   s != skipped.end(); s++) {
	typename std::map<EnumType, V>::iterator it = vals.find(*s);
	if (it != vals.end()) {
	  DEBUG_VALUE_SET(context, "erasing ", *s);
	  vals.erase(it);
	}
      }
    }
    /**
       Get a map from value name to value pointer.
       \param val Map from enum key to value pointer.
       \return Map between value names and value pointers.
     */
    static std::map<std::string, double*> get_pointer_map(std::map<EnumType, double*> vals) {
      std::map<std::string, double*> out;
      for (typename std::map<EnumType, double*>::iterator it = vals.begin();
	   it != vals.end(); it++) {
	out[getName(it->first)] = it->second;
      }
      return out;
    }
    /**
       Get the diff between two value maps.
       \tparam K Key type.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst First map for comparison.
       \param valsSrc Second map for comparison.
       \param padKeys Number of characters that key names should be padded
         to fill.
       \param padVals Number of characters that values should be padded to
         fill.
       \param includePrefixes If true, the module & parameter type
         prefixes will be added to the member names.
       \returns String diff.
     */
    template<typename K, typename VSrc, typename VDst>
    static std::string diff_value_maps(const std::map<K, VDst>& valsDst,
				       const std::map<K, VSrc>& valsSrc,
				       std::size_t padKeys=0,
				       std::size_t padVals=0,
				       bool includePrefixes=false) {
      std::map<K, std::string> valStrDst, valStrSrc;
#define GET_VAL(x) to_string_with_precision(__extract_value(x))
      if (padKeys == 0) {
	padKeys = field_width_value_map(valsDst, includePrefixes);
	std::size_t padKeys2 = field_width_value_map(valsSrc, includePrefixes);
	if (padKeys2 > padKeys)
	  padKeys = padKeys2;
      }
      if (padVals == 0) {
	padVals = value_width_value_map(valsDst);
	std::size_t padVals2 = value_width_value_map(valsSrc);
	if (padVals2 > padVals)
	  padVals = padVals2;
      }
      for (typename std::map<K, VSrc>::const_iterator itSrc = valsSrc.begin();
	   itSrc != valsSrc.end(); itSrc++) {
	typename std::map<K, VDst>::const_iterator itDst = valsDst.find(itSrc->first);
	if (itDst == valsDst.end()) {
	  valStrDst[itSrc->first] = "MISSING";
	  valStrSrc[itSrc->first] = GET_VAL(itSrc->second);
	} else if (!valuesEqual(itDst->second, itSrc->second)) {
	  valStrDst[itSrc->first] = GET_VAL(itDst->second);
	  valStrSrc[itSrc->first] = GET_VAL(itSrc->second);
	}
      }
      if (valsDst.size() != valsSrc.size()) {
	for (typename std::map<K, VDst>::const_iterator itDst = valsDst.begin();
	     itDst != valsDst.end(); itDst++) {
	  typename std::map<K, VSrc>::const_iterator itSrc = valsSrc.find(itDst->first);
	  if (itSrc == valsSrc.end()) {
	    valStrDst[itDst->first] = GET_VAL(itDst->second);
	    valStrSrc[itDst->first] = "MISSING";
	  }
	}
      }
      std::ostringstream out;
      for (typename std::map<K, std::string>::const_iterator itSrc = valStrSrc.begin();
	   itSrc != valStrSrc.end(); itSrc++) {
	typename std::map<K, std::string>::const_iterator itDst = valStrDst.find(itSrc->first);
	out << std::setw(padKeys + tab_size) << std::left;
	if (includePrefixes)
	  out << getNameWithPrefix(itSrc->first);
	else
	  out << getName(itSrc->first);
	out << std::setw(padVals + tab_size) << std::left << itDst->second;
	out << std::setw(padVals + tab_size) << std::left << itSrc->second << std::endl;
      }
#undef GET_VAL
      return std::move(out.str());
    }
    /**
       Check for equality between two value maps.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst First map for comparison.
       \param valsSrc Second map for comparison.
       \param context String providing context for error messages.
     */
    template<typename VSrc, typename VDst>
    static bool compare_value_maps(const std::map<EnumType, VDst>& valsDst,
				   const std::map<EnumType, VSrc>& valsSrc,
				   const std::string& context="") {
      for (typename std::map<EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
	   itSrc != valsSrc.end(); itSrc++) {
	typename std::map<EnumType, VDst>::const_iterator itDst = valsDst.find(itSrc->first);
	if (itDst == valsDst.end()) {
	  INFO_VALUE_SET(context, itSrc->first,
			 " value not present in a");
	  return false;
	}
	if (!valuesEqual(itDst->second, itSrc->second)) {
	  INFO_VALUE_SET(context, itDst->first,
			 " values are not equal: ",
			 valueString(itDst->second), ", ",
			 valueString(itSrc->second));
	  return false;
	}
      }
      if (valsDst.size() != valsSrc.size()) {
	for (typename std::map<EnumType, VDst>::const_iterator itDst = valsDst.begin();
	     itDst != valsDst.end(); itDst++) {
	  typename std::map<EnumType, VSrc>::const_iterator itSrc = valsSrc.find(itDst->first);
	  if (itSrc == valsSrc.end()) {
	    INFO_VALUE_SET(context, itDst->first,
			   " value not present in b");
	    return false;
	  }
	}
      }
      return true;
    }
    /**
       Check for equality between two value maps with integer keys.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst First map for comparison.
       \param valsSrc Second map for comparison.
       \param context String providing context for error messages.
     */
    template<typename VSrc, typename VDst>
    static bool compare_value_maps(const std::map<int, VDst>& valsDst,
				   const std::map<int, VSrc>& valsSrc,
				   const std::string& context="") {
      std::map<EnumType, VDst> kvalsDst = int2key(valsDst);
      std::map<EnumType, VSrc> kvalsSrc = int2key(valsSrc);
      return compare_value_maps(kvalsDst, kvalsSrc, context);
    }
    /**
       Compare two values maps, throwing an error if the keys do not
         match, valsSrc contains a constant value that is different than
	 the value in valsDst, or either map contains a skipped value.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst Map to add values to.
       \param valsSrc New values to add to the set.
       \param context String providing context for error messages.
       \param ignore_extras If true, extra values in valsSrc will be
         ignored.
       \param ignore_missing If true, missing values in valsSrc will be
         ignored.
       \param ignore_constants If true, constant values can be changed.
     */
    template<typename VSrc, typename VDst>
    static void check_value_maps(std::map<EnumType, VDst>& valsDst,
				 const std::map<EnumType, VSrc>& valsSrc,
				 const std::string& context="",
				 bool ignore_extras=false,
				 bool ignore_missing=false,
				 bool ignore_constants=false) {
      for (typename std::map<EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
	   itSrc != valsSrc.end(); itSrc++) {
	typename std::map<EnumType, VDst>::const_iterator itDst = valsDst.find(itSrc->first);
	if (itDst == valsDst.end()) {
	  if (ignore_extras)
	    continue;
	  ERROR_VALUE_SET(context, getName(itSrc->first),
			  " is not present in the destination");
	}
	if ((!ignore_constants) && isConstant(itSrc->first) &&
	    (!valuesEqual(itDst->second, itSrc->second))) {
	  std::string oldval = valueString(itDst->second);
	  std::string newval = valueString(itSrc->second);
	  checkNotConstant(itSrc->first, context +
			   "[" + oldval + ", " + newval + "]: ");
	}
      }
      if (valsDst.size() != valsSrc.size()) {
	for (typename std::map<EnumType, VDst>::const_iterator itDst = valsDst.begin();
	     itDst != valsDst.end(); itDst++) {
	  typename std::map<EnumType, VSrc>::const_iterator itSrc = valsSrc.find(itDst->first);
	  if (itSrc == valsSrc.end()) {
	    if (ignore_missing)
	      continue;
	    ERROR_VALUE_SET(context, getName(itDst->first),
			    " is not present in the source");
	  }
	}
      }
    }
    /**
       Copy values from one map to another.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst Map to add values to.
       \param valsSrc New values to add to the set.
       \param context String providing context for error messages.
       \param ignore_extras If true, extra values in valsSrc will be
         ignored.
       \param ignore_missing If true, missing values in valsSrc will be
         ignored.
       \param ignore_constants If true, constant values can be changed.
       \param ignore_skipped If true, skipped values can be changed.
       \param ignore_initonce If true, values initialized once can be
         changed;
     */
    template<typename VSrc, typename VDst>
    static void copy_value_map(std::map<EnumType, VDst>& valsDst,
			       const std::map<EnumType, VSrc>& valsSrc,
			       const std::string& context="",
			       bool ignore_extras=false,
			       bool ignore_missing=false,
			       bool ignore_constants=false,
			       bool ignore_skipped=false,
			       bool ignore_initonce=false) {
      bool check = false;
      if (valsDst.size() != valsSrc.size())
	check = true;
      if (check)
	check_value_maps(valsDst, valsSrc, context + "copy_value_map: ",
			 ignore_extras, ignore_missing, ignore_constants);
      for (typename std::map<EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
	   itSrc != valsSrc.end(); itSrc++) {
	typename std::map<EnumType, VDst>::iterator itDst = valsDst.find(itSrc->first);
	if (itDst == valsDst.end()) {
	  if (ignore_extras)
	    continue;
	  ERROR_VALUE_SET(context, getName(itSrc->first),
			  " is not present in the destination");
	}
	if (((!ignore_skipped) && isSkipped(itSrc->first)) ||
	    ((!ignore_initonce) && isInitonce(itSrc->first))) {
	  continue;
	}
#ifdef VERBOSE_VALUE_SET_DEBUG
	std::string srcval = valueString(itSrc->second);
	std::string dstval = valueString(itDst->second);
	std::string dstptr = valuePointerString(itDst->second);
#endif // VERBOSE_VALUE_SET_DEBUG
	valueSet(itDst->second, itSrc->second);
#ifdef VERBOSE_VALUE_SET_DEBUG
	DEBUG_VALUE_SET(context, itSrc->first, ": [", dstptr, "] ",
			dstval, " -> ", srcval, " [AFTER = ",
			valueString(itDst->second), "]");
#endif // VERBOSE_VALUE_SET_DEBUG
      }      
    }
    /**
       Add values to a value map by reading a parameter file.
       \tparam V Type of value in vals.
       \param vals Map to add values to.
       \param filename File containing values to add to the set.
       \param context String providing context for error messages.
       \param ignore_extras If true, extra values in valsSrc will be
         ignored.
       \param ignore_missing If true, missing values in valsSrc will be
         ignored.
       \param ignore_constants If true, constant values can be changed.
       \param ignore_skipped If true, skipped values can be changed.
       \param ignore_initonce If true, values initialized once can be
         changed;
     */
    template<typename V>
    static void copy_value_map(std::map<EnumType, V>& vals,
			       const std::string& filename,
			       const std::string& context="",
			       bool ignore_extras=false,
			       bool ignore_missing=false,
			       bool ignore_constants=false,
			       bool ignore_skipped=false,
			       bool ignore_initonce=false) {
      std::map<EnumType, double> new_values = utils::readEnumFile<EnumType, double>(filename);
      copy_value_map(vals, new_values, context + "[" + filename + "]: ",
		     ignore_extras, ignore_missing,
		     ignore_constants, ignore_skipped,
		     ignore_initonce);
    }
    /**
       Add values to a value map from another.
       \tparam VDst Destination value type.
       \tparam VSrc Source value type.
       \param valsDst Map to add values to.
       \param valsSrc New values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in valsSrc that are not
         in valsDst will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in valsDst that are not in valsSrc.
       \param no_constants If true, an error will be raised if there are
         constant values in valsSrc that differ from the values in
	 valsDst.
       \param no_skipped If true, an error will be raised if there are
         skipped values in valsSrc.
     */
    template<typename VSrc, typename VDst>
    static void update_value_map(std::map<EnumType, VDst>& valsDst,
				 const std::map<EnumType, VSrc>& valsSrc,
				 const std::string& context="",
				 bool allow_extras=false,
				 bool no_missing=false,
				 bool no_constants=false,
				 bool no_skipped=false) {
      copy_value_map(valsDst, valsSrc, context,
		     allow_extras, !no_missing,
		     !no_constants, !no_skipped);
    }
    /**
       Add values to a value map by reading a parameter file.
       \tparam V Type of value in vals.
       \param vals Map to add values to.
       \param filename File containing values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in the file that are not
         in vals will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in vals that are not in the file.
       \param no_constants If true, an error will be raised if there are
         constant values in the file that differ from the values in vals.
       \param no_skipped If true, an error will be raised if there are
         skipped values in the file.
     */
    template<typename V>
    static void update_value_map(std::map<EnumType, V>& vals,
				 const std::string& filename,
				 const std::string& context="",
				 bool allow_extras=false,
				 bool no_missing=false,
				 bool no_constants=false,
				 bool no_skipped=false) {
      copy_value_map(vals, filename, context,
		     allow_extras, !no_missing,
		     !no_constants, !no_skipped);
    }
    template<typename V>
    static std::map<int, double> export_value_map(const std::map<EnumType, V>& vals,
						  const std::string& context="") {
      std::map<int, double> out;
      for (typename std::map<EnumType, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	out[it->first] = valueGet(it->second);
      }
      return out;
    }
    template<typename V>
    static void import_value_map(std::map<EnumType, V>& dst,
				 const std::map<int, double>& src,
				 const std::string& context="") {
      std::map<EnumType, double> ksrc = BaseClass::int2key(src);
      copy_value_map(dst, ksrc, context,
		     false, false, true, true, true);
    }
    static void import_value_map(std::map<EnumType, double>& dst,
				 const std::map<int, double>& src,
				 const std::string& context) {
      std::map<EnumType, double> ksrc = BaseClass::int2key(src);
      dst.clear();
      dst.insert(ksrc.begin(), ksrc.end());
    }
    /**
       Re-initialize the values in a value map to the default values,
         minus any values that are skipped.
       \tparam V Value map type.
       \param vals Value map to initialize.
       \param isinit If true, the values are initialized as if they
         were already initialized.
     */
    template<typename V>
    static void init_value_map(std::map<EnumType, V>& vals,
			       const bool isinit=false) {
      // checkDefaults("init_value_map: ");
      initDefaults();
      copy_value_map(vals, defaults, "init_value_map: ",
		     true, false, !isinit, true, !isinit);
    }
    /**
       Throw an error if the defaults have not been initialized.
       \param context String providing context for error messages.
     */
    static void checkDefaults(const std::string& context="") {
      // initDefaults();
      if (defaults.empty() && !EnumBaseClass::defaults.empty())
	ERROR_VALUE_SET(context,
			"defaults not initialized, call ",
			"initDefaults first");
    }
    /**
       Get the value set module enum.
       \returns Module enum.
     */
    static MODULE get_module() { return module; }
    /**
       Get the value set parameter type enum.
       \returns Parameter type enum.
     */
    static PARAM_TYPE get_param_type() { return param_type; }
    /**
       Get a vector containing the parameter types that are associated
         with the module that this value set belongs to.
       \returns Parameter types.
     */
    static const std::vector<PARAM_TYPE>& get_parameter_types() {
      static std::vector<PARAM_TYPE> out = {};
      return out;
    }
    /**
       Initialize the defaults for the value set if they have not been
         initialized.
     */
    static void initDefaults() {
      initDefaults(static_flags & VS_FLAG_DEFAULTS_C3);
    }
    /**
       Initialize the defaults for the value set if they have not been
         initialized.
       \param useC3 If true, default values for a C3 will be used
       \param filename File containing values to add to the defaults.
       \param force If true, initialize the defaults even if the value
         set is not selected. [this paramter will be removed]
       \param noChildren If true, child classes will not be modified.
     */
    static void initDefaults(const bool useC3,
			     const std::string& filename="",
			     const bool force=false,
			     const bool noChildren=false) {
      if (!(static_flags & VS_FLAG_INIT_DEFAULTS)) {
	if (useC3) {
	  defaults.insert(EnumBaseClass::defaults_C3.begin(),
			  EnumBaseClass::defaults_C3.end());
	  static_flags |= VS_FLAG_DEFAULTS_C3;
	} else {
	  defaults.insert(EnumBaseClass::defaults.begin(),
			  EnumBaseClass::defaults.end());
	  static_flags &= ~VS_FLAG_DEFAULTS_C3;
	}
	static_flags |= VS_FLAG_INIT_DEFAULTS;
      }
      if (useC3 != bool(static_flags & VS_FLAG_DEFAULTS_C3)) {
	if (static_flags & VS_FLAG_DEFAULTS_EDITED)
	  ERROR_VALUE_SET("Defaults have been modified using "
			  "values from a file. Changing the "
			  "default value set to those for "
			  "useC3 = ", useC3,
			  " would overwrite those values.");
	if (useC3) {
	  copy_value_map(defaults, EnumBaseClass::defaults_C3,
			 "initDefaults: ",
			 false, false, true, true, true);
	  static_flags |= VS_FLAG_DEFAULTS_C3;
	} else {
	  copy_value_map(defaults, EnumBaseClass::defaults,
			 "initDefaults: ",
			 false, false, true, true, true);
	  static_flags &= ~VS_FLAG_DEFAULTS_C3;
	}
      }
      T::_initDefaults();
      DO_VALUE_SET_CHILD_CLASSES(initDefaults, (useC3, "", force));
      if (filename.empty())
	return;
      copy_value_map(defaults, filename, "initDefaults: ",
		     false, true, true, true, true);
      static_flags |= VS_FLAG_DEFAULTS_EDITED;
    }
    /** \copydoc ValueSetEnum::getName */
    static std::string getName(const int& k) {
      return getName(int2key(k));
    }
    /**
       Get the string associated with a variable including the module
         and parameter type as a prefix.
       \tparam K Key type.
       \param[in] k Key to get name for.
       \returns Key name with prefix.
     */
    template <typename K>
    static std::string getNameWithPrefix(const K& k) {
      std::ostringstream out;
      out << module << "::" << param_type << "::" << getName(k);
      return std::move(out).str();
    }
    /**
       Get the enum key corresponding to a name value also checking for
         aliases.
       \param[in] name Value to get key for
       \param[in] isGlymaID If true, name is a GlymaID.
       \return Key
    */
    static EnumType fromNameWithAliases(const std::string& name,
					bool isGlymaID = false) {
      if (isGlymaID)
	return fromGlymaid(name);
      // TODO: Check for module/param_type prefix
      if (aliases.size()) {
        typename std::map<std::string, EnumType>::const_iterator it = aliases.find(name);
        if (it != aliases.end())
          return it->second;
      }
      try {
        return fromName(name);
      } catch (...) {
        std::string split = "::";
        size_t idx1 = name.find(split);
        size_t idx2 = std::string::npos;
        if (idx1 != std::string::npos) {
          idx2 = name.find(split, idx1 + split.size());
          if (idx2 != std::string::npos) {
            std::string var1 = name.substr(0, idx1);
            if (var1 == utils::enum_key2string(module)) {
              std::string var2 = name.substr(idx1 + split.size(),
                                             idx2 - (idx1 + split.size()));
              if (var2 == utils::enum_key2string(param_type)) {
                std::string name_alt = name.substr(idx2 + split.size());
                return fromName(name_alt);
              }
            }
          }
        }
        throw;
      }
    }
    /**
       Check if a string names a member of the value set.
       \param name Key to check for.
       \param[in] isGlymaID If true, name is a GlymaID.
       \returns true if name is a member, false otherwise.
     */
    static bool has(const std::string& name,
		    const bool& isGlymaID = false) {
      try {
	if (isGlymaID)
	  fromGlymaid(name);
	else
	  fromNameWithAliases(name);
	return true;
      } catch (...) {
	return false;
      }
    }
    /**
       Check if a string names a GlymaID associated with a member of the
         value set.
       \param name GlymaID to check for.
       \returns true if name is a GlymaID associated with the value set,
         false otherwise.
     */
    static bool hasGlymaID(const std::string& name) {
      try {
	fromGlymaid(name);
	return true;
      } catch (...) {
	return false;
      }
    }
    /**
       Set the default value corresponding to an enum key
       \param[in] k Key to set value for.
       \param[in] v Value that default should be set to.
       \param[in] dontPreserve If true, the value will not be preserved
         if initDefaults is called again.
     */
    static void setDefault(const EnumType& k, const double& v,
			   const bool dontPreserve = false) {
      checkDefaults("setDefault: ");
      defaults[k] = v;
      if (!dontPreserve)
	static_flags |= VS_FLAG_DEFAULTS_EDITED;
    }
    /**
       Set the default value corresponding to an enum key
       \param[in] k Name of key to set value for.
       \param[in] v Value that default should be set to.
       \param[in] isGlymaID If true, k is a GlymaID.
       \param[in] dontPreserve If true, the value will not be preserved
         if initDefaults is called again.
     */
    static void setDefault(const std::string& k, const double& v,
			   const bool& isGlymaID = false,
			   const bool dontPreserve = false) {
      if (isGlymaID)
	return setDefault(fromGlymaid(k), v, dontPreserve);
      return setDefault(fromNameWithAliases(k), v, dontPreserve);
    }
    /**
       Set the default value corresponding to an enum key
       \param[in] k Key to set value for.
       \param[in] v Value that default should be set to.
       \param[in] dontPreserve If true, the value will not be preserved
         if initDefaults is called again.
     */
    static void setDefault(const int& k, const double& v,
			   const bool dontPreserve = false) {
      return setDefault(int2key(k), v, dontPreserve);
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static double getDefault(const EnumType& x) {
      checkDefaults("getDefault: ");
      typename std::map<EnumType, double>::const_iterator it;
      it = defaults.find(x);
      if (it == defaults.end()) {
	ERROR_VALUE_SET("Could not locate Default for '",
			names.find(x)->second, "'");
      }
      return it->second;
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] isGlymaID If true, x is a GlymaID.
      \return Value
    */
    static double getDefault(const std::string& x,
			     const bool& isGlymaID = false) {
      if (isGlymaID)
	return getDefault(fromGlymaid(x));
      return getDefault(fromNameWithAliases(x));
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] k Key to get value for
      \return Value
    */
    static double getDefault(const int& k) {
      return getDefault(int2key(k));
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static double getDefault(const EnumType& x, const double& defaultV) {
      checkDefaults("getDefault[optional]: ");
      typename std::map<EnumType, double>::const_iterator it;
      it = defaults.find(x);
      if (it == defaults.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
       Get the reference for the value associated with a key from a value
         map.
       \tparam T Type of values in map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Value reference.
     */
    template<typename V>
    static ValueType get_value(std::map<EnumType, V>& vals,
			       const EnumType k,
			       const std::string& context="[]: ") {
      check_value_map(vals, k, context);
      return ValueType(k, vals.find(k)->second,
		       context + "get_value[V]: "
		       + getName(k));
    }
    /**
       Get the constant reference for the value associated with a key from
         a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Constant value reference.
     */
    static const double& get_value_const(const std::map<EnumType, double>& vals,
					 const EnumType k,
					 const std::string& context="") {
      check_value_map(vals, k, context);
      DEBUG_VALUE_SET(context, "[CONST DOUBLE]: ", k, " = ",
		      vals.find(k)->second);
      return vals.find(k)->second;
    }
    /**
       Get the constant reference for the value associated with a key
         from a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Constant value reference.
     */
    static const double& get_value_const(const std::map<EnumType, double*>& vals,
					 const EnumType k,
					 const std::string& context="") {
      check_value_map(vals, k, context);
      DEBUG_VALUE_SET(context, "[CONST DOUBLE*]: ", k, " = ",
		      *(vals.find(k)->second));
      return *(vals.find(k)->second);
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
       \param context String providing context for error messages.
     */
    static void set_value(std::map<EnumType, double>& vals,
			  const EnumType k, const double v,
			  const std::string& context="") {
#ifdef VERBOSE_VALUE_SET_DEBUG
      DEBUG_VALUE_SET_NOEND(context, "[DOUBLE] ", k, "[",
			    std::to_string(k), "]: ");
      if (vals.find(k) == vals.end()) {
	DEBUG_VALUE_SET_RAW("NULL");
      } else {
	DEBUG_VALUE_SET_RAW(vals[k]);
      }
      DEBUG_VALUE_SET_RAW(" -> ", v);
#endif // VERBOSE_VALUE_SET_DEBUG
      checkNotConstant(k, context);
      vals[k] = v;
#ifdef VERBOSE_VALUE_SET_DEBUG
      DEBUG_VALUE_SET_END(" [AFTER = ", vals[k], "]");
#endif // VERBOSE_VALUE_SET_DEBUG
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
       \param context String providing context for error messages.
     */
    static void set_value(std::map<EnumType, double*>& vals,
			  const EnumType k, const double v,
			  const std::string& context="") {
      DEBUG_VALUE_SET_NOEND(context, "[DOUBLE*] ", k, "[",
			    std::to_string(k), "]: ");
#ifdef VERBOSE_VALUE_SET_DEBUG
      if (vals.find(k) == vals.end()) {
	DEBUG_VALUE_SET_RAW("[NULL] 0");
      } else {
	DEBUG_VALUE_SET_RAW("[", vals[k], "] ", *(vals[k]));
      }
      DEBUG_VALUE_SET_RAW(" -> ", v);
#endif // VERBOSE_VALUE_SET_DEBUG
      check_value_map(vals, k, context);
      *(vals[k]) = v;
      DEBUG_VALUE_SET_END(" [AFTER = ", *(vals[k]), "]");
    }
    /**
       Update the value associated with a key in a value map from a map
         containing enzyme activity values.
       \tparam V Value map type.
       \param vals Value map to update.
       \param k Key to update value for.
       \param EnzymeAct Enzyme activities.
     */
    template<typename V>
    static void set_value_from_EnzymeAct(std::map<EnumType, V>& vals,
					 const EnumType k,
					 const std::map<std::string, double>& EnzymeAct) {
      set_value(vals, k, EnzymeAct.at(getName(k)),
		"set_value_from_EnzymeAct: ");
    }
    /**
       Get the value pointer for the value associated with a key from a
         value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Value pointer.
     */
    static double*& get_value_orig(std::map<EnumType, double*>& vals,
				   const EnumType k,
				   const std::string& context="[]: ") {
      checkNotConstant(k, context);
      check_value_map(vals, k, context);
      DEBUG_VALUE_SET(context, k);
      return vals.find(k)->second;
    }
    /**
       Add a reference to an original value for the given key to a value
         map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
       \param context String providing context for error messages.
     */
    static void insert_value_orig(std::map<EnumType, double*>& vals,
				  const EnumType k, double* v,
				  const std::string& context="") {
      if (vals.find(k) == vals.end()) {
	vals.emplace(k, &(v[0]));
	if (isInitonce(k)) {
	  // TODO: this check may not be necessary if the init once
	  //   variables are reinitialized after a reset
	  compareValues(k, EnumBaseClass::defaults.find(k)->second,
			EnumBaseClass::defaults_C3.find(k)->second);
	  checkDefaults("insert_value_orig: ");
	  v[0] = defaults.find(k)->second;
	  DEBUG_VALUE_SET(context, "initialized ", k, " = ",
			  vals[k], " [", v, "]");
	}
	DEBUG_VALUE_SET(context, "inserted ", k, " = ",
			vals[k], " [", v, "]");
      }
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set_value_orig(std::map<EnumType, double*>& vals,
			       const EnumType k, double* v) {
      get_value_orig(vals, k, "set_orig") = v;
    }
    /**
       Reset all of the values in a value map that are not constant to 0.
       \tparam Type of value in map.
       \param vals Value map to reset.
     */
    template<typename V>
    static void reset_value_map(std::map<EnumType, V>& vals) {
      DEBUG_VALUE_SET_NOARGS;
      for (typename std::map<EnumType, V>::iterator it = vals.begin();
	   it != vals.end(); it++) {
	if (!isConstant(it->first)) {
	  if (isResetone(it->first)) {
	    set_value(vals, it->first, 1.0, "reset_value_map: ");
	  } else {
	    set_value(vals, it->first, 0.0, "reset_value_map: ");
	  }
	}
      }
    }
    /**
       Get the values from a value map in an array ordering based on
         key order.
       \tparam V Value map type.
       \param vals Value map.
       \param out Existing array that values should be added to.
     */
    template<typename V>
    static void value_map_toArray(const std::map<EnumType, V>& vals,
				  arr& out) {
      size_t i = 0;
      out.resize(out.size() + vals.size());
      for (typename std::map<EnumType, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	if (inArrays(it->first)) {
	  checkNotConstant(it->first);
	  out[i] = get_value_const(vals, it->first,
				   "value_map_toArray: ");
	  i++;
	}
      }
    }
    /**
       Update the values in a value map from an array.
       \tparam V Value map type.
       \param vals Value map to update.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    template<typename V>
    static void value_map_fromArray(std::map<EnumType, V>& vals,
				    const arr &vec,
				    const std::size_t offset = 0) {
      checkDefaults("value_map_fromArray: ");
      size_t i = 0;
      size_t nexp = memberCount();
      size_t nact = vec.size() - offset;
      if (vals.size() != defaults.size())
	ERROR_VALUE_SET(vals.size(), " provided, but ",
			defaults.size(), " expected");
      if (nexp > nact)
	ERROR_VALUE_SET(nexp, " expected, but only ",
			nact, " provided (vec.size() = ",
			vec.size(), ", offset = ",
			offset, ")");
      for (typename std::map<EnumType, V>::iterator it = vals.begin();
	   it != vals.end(); it++) {
	if (inArrays(it->first)) {
	  // checkNotConstant(it->first, "value_map_fromArray: ");
	  set_value(vals, it->first, vec[offset + i],
		    "value_map_fromArray: ");
	  i++;
	}
      }
    }
    /**
       Get the number of members that will be used to populated vectors.
       \param noChildren If true, child classes will not be included.
       \return Member counts.
     */
    static std::size_t memberCount(const bool noChildren = false) {
      std::size_t out = EnumBaseClass::defaults.size() - (skipped.size() + nonvector.size());
#define DO_CHILD(child, dummy) out += child->memberCount()
      DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return out;
    }
    /**
       Get a string representation of the class state.
       \param noChildren If true, child classes will not be included.
       \return String representation.
     */
    static std::string memberState(const bool noChildren = false) {
      std::string out = error_prefix() +
	", NCHILD = " + std::to_string(child_classes.size()) +
	", COUNT = " + std::to_string(memberCount()) +
	", SKIPPED = " + stringSkipped() +
	", NONVECT = " + stringNonvector();
      if (noChildren)
	return out;
      for (std::vector<ValueSetClass_t*>::const_iterator it = child_classes.begin();
	   it != child_classes.end(); it++)
	out += "\n\t" + (*it)->memberState();
      return out;
    }

    /**
       Add a child class to this classes set.
       \tparam C Child class.
     */
    template<typename C>
    static void addChildClass() {
      child_classes.push_back(new ValueSetClassType<C>());
    }

    static int static_flags; /**< Bitwise flags describing the value set  static state */
    static std::map<EnumType, double> defaults; /**< Default values */
    static std::map<EnumType, double*> static_values; /**< Static values */
    static std::vector<ValueSetClass_t*> child_classes; /**< Child classes */
    static const uint tab_size = 2;

    // Methods that call inheriting class methods
    /**
       Initialize the child classes.
       \param noChildren If true, child classes will not be initialized.
       Common, public interface for the private _initChildClasses method.
     */
    static void initChildClasses(const bool noChildren = false) {
      if (static_flags & VS_FLAG_INIT_CHILDREN) return;
      T::_initChildClasses();
      DO_VALUE_SET_CHILD_CLASSES(initChildClasses, ());
      static_flags |= VS_FLAG_INIT_CHILDREN;
    }
    /**
       Clean up the child classes.
       \param noChildren If true, children of child classes will not be
         finalized.
       Common, public interface for the private _cleanupChildClasses
         method.
     */
    static void cleanupChildClasses(const bool noChildren = false) {
      if (!(static_flags & VS_FLAG_INIT_CHILDREN)) return;
      T::_cleanupChildClasses();
      DO_VALUE_SET_CHILD_CLASSES(cleanupChildClasses, ());
      for (std::size_t i = 0; i < child_classes.size(); i++) {
	delete child_classes[i];
	child_classes[i] = nullptr;
      }
      child_classes.clear();
      static_flags &= ~VS_FLAG_INIT_CHILDREN;
    }
    /**
       Initialize the static class members including child classes.
       \param noChildren If true, child classes will not be initialized.
       Common, public interface for the private _initStaticMembers method.
     */
    static void initStaticMembers(const bool noChildren = false) {
      if (static_flags & VS_FLAG_INIT_STATIC) return;
      T::_initStaticMembers();
      initChildClasses(true); // Children will be called in initStaticMembers
      DO_VALUE_SET_CHILD_CLASSES(initStaticMembers, ());
      static_flags |= VS_FLAG_INIT_STATIC;
    }
    /**
       Finalize the static class members.
       Common, public interface for the private _cleanupStaticMembers
         method.
     */
    static void cleanupStaticMembers(const bool noChildren = false) {
      if (!(static_flags & VS_FLAG_INIT_STATIC)) return;
      T::_cleanupStaticMembers();
      DO_VALUE_SET_CHILD_CLASSES(cleanupStaticMembers, ());
      cleanupChildClasses(true);
      static_flags &= ~VS_FLAG_INIT_STATIC;
    }
    /**
       Check if the value set is selected by the current driver.
       \returns true if the value set is selected, false otherwise.
     */
    static bool selected() {
      return (static_flags & VS_FLAG_SELECTED);
    }
    /**
       Select the value set and any children.
       \param[in] x true if value set should be selected, false if it
         should be deselected.
       \param noChildren If true, child classes will not be selected.
     */
    static void select(const bool x = true, const bool noChildren = false) {
      if (x) {
	static_flags |= VS_FLAG_SELECTED;
      } else {
	static_flags &= ~VS_FLAG_SELECTED;
      }
      T::_select(x, noChildren);
      DO_VALUE_SET_CHILD_CLASSES(select, (x));
    }
    /**
       Throw an error if the value set is not selected.
     */
    static void checkSelected() {
      if (!(static_flags & VS_FLAG_SELECTED))
	ERROR_VALUE_SET("Value set has not been selected");
    }
    /**
       Check if the value set uses the C3 defaults.
       \returns true if the C3 defaults are being used, false otherwise.
     */
    static bool usesC3() {
      return (static_flags & VS_FLAG_DEFAULTS_C3);
    }
    /**
       Set a flag so that the value set uses or does not use C3 defaults.
       \param[in] x true if value set should use C3 defaults, false if it
         should use non-C3 defaults.
       \param noChildren If true, child classes will not be modified.
    */
    static void enableC3(const bool x = true,
			 const bool noChildren = false) {
      initDefaults(x, "", true);
      if (x)
	static_flags |= VS_FLAG_DEFAULTS_C3;
      else
	static_flags &= ~VS_FLAG_DEFAULTS_C3;
      T::_enableC3(x, noChildren);
      DO_VALUE_SET_CHILD_CLASSES(enableC3, (x));
    }
    /**
       Reset the static class members following the destruction of the
         current driver so that additional runs start with a clean slate.
       \param noChildren If true, child classes will not be reset.
       Common, public interface for the private _reset method.
     */
    static void reset(const bool noChildren = false) {
      int flags0 = static_flags;
      clearSkipped();
      T::_reset(noChildren);
      static_flags = 0;
      if (flags0 & VS_FLAG_INIT_CHILDREN)
	static_flags |= VS_FLAG_INIT_CHILDREN;
      if (flags0 & VS_FLAG_INIT_POINTERS)
	static_flags |= VS_FLAG_INIT_POINTERS;
      defaults.clear();
      DO_VALUE_SET_CHILD_CLASSES(reset, ());
    }
    /**
       Number of elements in the value set.
       \returns Element count.
     */
    static std::size_t size() {
      return T::_size();
    }
    // Methods that can be overridden
  protected:
    /** Perform class specific default initialization */
    static void _initDefaults() {}
    /** Perform class specific child class initialization */
    static void _initChildClasses() {}
    /** Perform class specific child class cleanup */
    static void _cleanupChildClasses() {}
    /** Perform class specific static member initialization */
    static void _initStaticMembers() {}
    /** Perform class specific static member cleanup */
    static void _cleanupStaticMembers() {}
    /** Perform class specific value set selection */
    static void _select(const bool = true, const bool = false) {}
    /** Perform class specific actions to enable C3 */
    static void _enableC3(const bool = true, const bool = false) {}
    /** Perform class specific class reset */
    static void _reset(const bool = false) {}
    /**
       Get class specific size
       \returns Number of members in the value set.
    */
    static std::size_t _size() {
      return memberCount();
    }
    
  };

  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  int ValueSetBase<T, U, ID, PT>::static_flags = 0;
  
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetBase<T, U, ID, PT>::EnumType, double>
  ValueSetBase<T, U, ID, PT>::defaults = {};
  
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetBase<T, U, ID, PT>::EnumType, double*>
  ValueSetBase<T, U, ID, PT>::static_values = {};
  
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  std::vector<ValueSetClass_t*>
  ValueSetBase<T, U, ID, PT>::child_classes = {};

  /**
     Template class to provide methods for manipulating a set of values
     used by a model run that can be set via input files.

     \tparam T Child class with virtual & static methods.
     \tparam U Parent class with virtual & static methods.
     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  class EPHOTO_API ValueSet :
    public ValueSet_t, public ValueSetBase<T, U, ID, PT> {
  public:
    typedef ValueSet<T, U, ID, PT> ValueSetClass;
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<T, U, ID, PT>)
    ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(STUB_VIRTUAL_VALUE_SET)
    ADD_METHODS_VALUE_SET_T_STATIC(STUB_VIRTUAL_VALUE_SET)

    /**
       Default constructor. Values will be initialized with default
         values, minus any values that are skipped.
     */
    ValueSet() : values() {}
    /**
       Copy constructor. Values will be updated from the values in the
         copied set.
       \param other Value set to copy values from.
     */
    ValueSet(const ValueSet& other) : ValueSet() {
      copyValueSet(other);
    }
    /**
       Copy ValueSet members from another value set to this one.
       \param other Value set to copy values from.
     */
    void copyValueSet(const ValueSet& other) {
      int flags0 = flags;
      flags = other.flags;
      preinit_values = other.preinit_values;
      if (!(flags0 & BaseClass::VS_FLAG_INIT_POINTERS))
	flags &= ~BaseClass::VS_FLAG_INIT_POINTERS;
      if (!(flags0 & BaseClass::VS_FLAG_INIT_CHILDREN))
	flags &= ~BaseClass::VS_FLAG_INIT_CHILDREN;
    }
    /**
       Copy another instance of this value set.
       \param other Value set to copy values from.
       \param noChildren If true, child classes will not be initialized.
     */
    void copyInstance(const T& other, const bool noChildren=false) {
      initMembers();
      static_cast<T*>(this)->_copyMembers(other);
      copyChildren(other, noChildren);
    }
    /**
       Copy children.
       \param other Value set to copy values from.
       \param noChildren If true, child classes will not be initialized.
     */
    void copyChildren(const T& other, const bool noChildren=false) {
      if (children.size() != other.children.size()) {
	ERROR_VALUE_SET("copyMembers: "
			"This instance has ", children.size(),
			" children, but the one being copied has ",
			other.children.size());
      }
      DO_VALUE_SET_CHILDREN(copyMembers, (**(other.children[iChild])));
    }
    /**
       Assignment operator. Values will be updated from the values in the
         copied set.
       \param other Value set to copy values from.
       \returns Updated value set.
     */
    ValueSet& operator=(const T& other) {
      copyMembers(other);
      return *this;
    }
    /**
       Copy values from an array.
       \param vec Vector to copy values from.
       \param offset Index into vec that copied values should start from.
     */
    ValueSet(const arr &vec, const std::size_t offset) : ValueSet() {}

    /**
       Get a constant ValueSet_t pointer for this instance/class.
       \returns Pointer.
     */
    const ValueSet_t* getValueSet() const override {
      return this;
    }

    /**
       Get a copy of the value in the set.
       \param[in] preinit If true, the values in preinit_values will be
         returned.
       \returns Map of values in the value set.
     */
    std::map<int, double> getValueMap(const bool preinit=false) const override {
      if (preinit)
	return BaseClass::export_value_map(preinit_values, CONTEXT_VALUE_SET);
      return BaseClass::export_value_map(values, CONTEXT_VALUE_SET);
    }
    /**
       Set the values from a map.
       \param[in] map Value map to update values from.
       \param[in] setinit If true, set the flag indicating that the value
         set is initialized.
       \param[in] preinit If true, the values in preinit_values will be
         updated. preinit & setinit cannot both be true.
     */
    void setValueMap(const std::map<int, double>& map,
		     const bool setinit=false,
		     const bool preinit=false) override {
      if (preinit) {
	if (setinit)
	  ERROR_VALUE_SET("preinit and setinit cannot both be true");
	BaseClass::import_value_map(preinit_values, map, CONTEXT_VALUE_SET);
	return;
      }
      BaseClass::import_value_map(values, map, CONTEXT_VALUE_SET);
      if (setinit)
	flags |= BaseClass::VS_FLAG_INIT_VALUES;
    }
    
    // Methods that call inheriting class methods
    /**
       Copy values from another value set to this one.
       Common, public interface for the private _copyMembers method.
       \param other Value set to copy values from.
       \param noChildren If true, child classes will not be initialized.
     */
    void copyMembers(const ValueSet_t& other0,
		     const bool noChildren=false) override {
      const T& other = castValueSet<T>(other0);
      copyValueSet(other);
      // TODO: Re-enable this only if more efficient than direct copy
      // copy_value_map(values, other.values, "copyMembers[vals]: ", true);
      copyInstance(other, noChildren);
    }
    /**
       Initialize the members in the instance. This should be overriden
         by child classes with a method that adds pointers to values.
       Common, public interface for the private _initMembers method.
       \param noChildren If true, child classes will not be initialized.
    */
    void initMembers(const bool noChildren=false) override {
      ParentClass::initStaticMembers(true);
      initMemberPointers(true);
      static_cast<T*>(this)->_initMembers();
      initChildren(true);
      DO_VALUE_SET_CHILDREN(initMembers, ());
    }
    /**
       Reset the values so that they can be re-initialized.
       \param noChildren If true, child classes will not be initialized.
     */
    void resetValues(const bool noChildren=false) override {
      ENSURE_VALUE_POINTERS;
      reset_value_map(values);
      static_cast<T*>(this)->_resetValues();
      DO_VALUE_SET_CHILDREN(resetValues, ());
      flags &= ~BaseClass::VS_FLAG_INIT_VALUES;
    }
    /**
       Check if the value set has been initialized.
       \returns true if the value set has been initialized, false otherwise.
     */
    bool initialized() const override {
      return (flags & BaseClass::VS_FLAG_INIT_VALUES);
    }
    /**
       Initialize the values to the default values if they have not
         already been initialized, minus any values that are skipped.
       Common, public interface for the private _initValues method.
       \param noDefaults If true, the value pointers will be initialized,
         but the default values will not be assigned to those values.
       \param force If true, initialize the values even if they have
         already been initialized.
       \param noChildren If true, child classes will not be initialized.
     */
    void initValues(const bool noDefaults=false,
		    const bool force=false,
		    const bool noChildren=false) override {
      if ((!force) && initialized()) return;
      initMembers(true);
      if (!noDefaults)
	init_value_map(values, initialized());
      static_cast<T*>(this)->_initValues();
      DO_VALUE_SET_CHILDREN(initValues, (noDefaults, force));
      if (!preinit_values.empty()) {
	copy_value_map(values, preinit_values, "initValues: ",
		       false, true, true, false, true);
      }
      flags |= BaseClass::VS_FLAG_INIT_VALUES;
      if (BaseClass::usesC3())
	flags |= BaseClass::VS_FLAG_DEFAULTS_C3;
      else
	flags &= ~BaseClass::VS_FLAG_DEFAULTS_C3;
    }
    /**
       Reset the instance so that it can be used again.
       \param noChildren If true, child classes will not be reset.
     */
    void resetInstance(const bool noChildren=false) override {
      T::reset(true);
      int preserve_flags = 0;
      if (flags & BaseClass::VS_FLAG_INIT_POINTERS)
	preserve_flags |= BaseClass::VS_FLAG_INIT_POINTERS;
      if (flags & BaseClass::VS_FLAG_INIT_CHILDREN)
	preserve_flags |= BaseClass::VS_FLAG_INIT_CHILDREN;
      flags = preserve_flags;
      DO_VALUE_SET_CHILDREN(resetInstance, ());
    }
    /**
       Initialize the map containing pointers to instance members.
       Common, public interface for the private _initMemberPointers method.
       \param noChildren If true, child classes will not be initialized.
     */
    void initMemberPointers(const bool noChildren=false) override {
      if (flags & BaseClass::VS_FLAG_INIT_POINTERS) return;
      static_cast<T*>(this)->_initMemberPointers();
      DO_VALUE_SET_CHILDREN(initMemberPointers, ());
      flags |= BaseClass::VS_FLAG_INIT_POINTERS;
    }
    /**
       Initialize the vector containing pointers to children.
       Common, public interface for the private _initChildren method.
     */
    void initChildren(const bool noChildren=false) override {
      if (flags & BaseClass::VS_FLAG_INIT_CHILDREN) return;
      static_cast<T*>(this)->_initChildren();
      DO_VALUE_SET_CHILDREN(initChildren, ());
      flags |= BaseClass::VS_FLAG_INIT_CHILDREN;
    } 
    /**
       Create any missing children.
       Common, public interface for the private _createChildren method.
     */
    void createChildren(const bool noChildren=false) override {
      BaseClass::initChildClasses(true);
      initChildren(true);
      static_cast<T*>(this)->_createChildren();
      // if (child_classes.size() != children.size())
      //        ERROR_VALUE_SET("Number of child classes (",
      //                        child_classes.size(),
      // 			") does not match the number of child pointers (",
      //                        children.size(), ")");
      // std::vector<ValueSetClass_t*>::iterator itCls = child_classes.begin();
      // for (std::vector<ValueSet_t**>::iterator it = children.begin();
      // 	   it != children.end(); it++, itCls++) {
      // 	if (**it) continue;
      // 	(*it)[0] = (*itCls)->newInstance(static_cast<ValueSet_t*>(this));
      // 	// (*it)[0]->setParent(static_cast<ValueSet_t*>(this));
      // }
      DO_VALUE_SET_CHILDREN(createChildren, ());
    }
    /**
       Get the vector of children.
       \returns References to the values set children.
     */
    const std::vector<ValueSet_t**>& getChildren() const override {
      return children;
    }
    /**
       Create a new instance of the wrapped class.
       \param par Pointer to parent instance.
       \returns New instance.
     */
    static ValueSet_t* newInstance(ValueSet_t* par = nullptr) {
      return T::_newInstance(par);
    }
    /**
       Set the parent class pointer to the given value.
       \param par Pointer to the parent class instance.
    */
    virtual void setParent(U* par) {
      UNUSED(par);
    }
    /**
       Set the parent class pointer to the given value.
       \param par Pointer to the parent class instance.
    */
    void setParent(ValueSet_t* par) override {
      // if (typeid(*par) != typeid(U))
      //   ERROR_VALUE_SET("Type IDs do not match");
      // static_cast<T*>(this)->setParent(static_cast<U*>(par));
      UNUSED(par);
    }

    /**
       Get the map containing pointers to the instance members.
       \return Map between member names and pointers to instance
         members.
     */
    std::map<std::string, double*> getPointerMap() {
      initMemberPointers();
      return get_pointer_map(values);
    }

    /** Get the iterator pointing to the start of the value set */
    iterator begin() {
      ENSURE_VALUE_POINTERS;
      return values.begin();
    }
    /** Get the iterator pointing to the end of the value set */
    iterator end() {
      ENSURE_VALUE_POINTERS;
      return values.end();
    }
    /** Get the constant iterator pointing to the start of the value set */
    const_iterator begin() const {
      ENSURE_VALUE_POINTERS_CONST;
      return values.begin();
    }
    /** Get the constant iterator pointing to the end of the value set */
    const_iterator end() const {
      ENSURE_VALUE_POINTERS_CONST;
      return values.end();
    }
    // Inspection utilities
    /**
       Get the maximum width of the values in the value set.
       \param noChildren If true, children won't be included when
         determining the maximum value width.
       \returns Maximum value width.
     */
    std::size_t max_value_width(bool noChildren = false) const override {
      std::size_t out = BaseClass::value_width_value_map(values);
      if (noChildren) return out;
      std::size_t iout = 0;
#define DO_CHILD(child, dummy)						\
      iout = child->max_value_width();					\
      if (iout > out) out = iout
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return out;
    }
    /**
       Get the diff between this an another value set.
       \param other Value set to compare against.
       \param padKeys Number of characters that key names should be
         padded to fill.
       \param padVals Number of characters that values should be padded
         to fill.
       \param includePrefixes If true, the module & parameter type
         prefixes will be added to the member names.
       \param noChildren If true, children of composite sets will not
         be displayed.
       \returns String diff.
     */
    std::string diff(const ValueSet_t& other,
		     std::size_t padKeys=0,
		     std::size_t padVals=0,
		     bool includePrefixes=false,
		     bool noChildren = false) const override {
      if (padKeys == 0) {
	padKeys = max_field_width(noChildren);
      }
      if (padVals == 0) {
	padVals = max_value_width(noChildren);
	std::size_t padVals2 = other.max_value_width(noChildren);
	if (padVals2 > padVals)
	  padVals = padVals2;
      }
      return static_cast<const T*>(this)->_diff(static_cast<const T&>(other),
						padKeys, padVals,
						includePrefixes,
						noChildren);
    }
    /**
       Display the values in the set.
       Common, public interface for the private _print method.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \param pad Number of characters that key names should be padded
         to fill.
       \param includePrefixes If true, the module & parameter type
         prefixes will be added to the member names.
       \param includeSkipped If true, skipped keys will be output.
       \param noChildren If true, children of composite sets will not
         be displayed.
       \param skip_keys Key strings to skip in output.
       \param key_aliases String aliases to use for keys.
       \returns Output stream.
     */
    std::ostream& print(std::ostream &out,
			const uint tab = 0, std::size_t pad = 0,
			bool includePrefixes = false,
			bool includeSkipped = false,
			const std::vector<std::string>& skip_keys = {},
			const std::map<std::string, std::string>& key_aliases = {},
			bool noChildren = false) const override {
      if (pad == 0) pad = max_field_width(noChildren);
      return static_cast<const T*>(this)->_print(out, tab, pad,
						 includePrefixes,
						 includeSkipped,
						 skip_keys,
						 key_aliases,
						 noChildren);
    }
    friend std::ostream& operator<<(std::ostream& out, const ValueSetClass& x) {
      return x.print(out);
    }
    friend std::ostream& operator<<(std::ostream& out, const ValueSetClass* x) {
      return x->print(out);
    }
    /**
       Check if another value set is equivalent to this one.
       \param b Value set to compare against this one.
       \param noChildren If true, child classes will not be checked.
     */
    bool equals(const ValueSet_t& b0,
		const bool noChildren = false) const override {
      if(typeid(*this) != typeid(b0)) {
	std::cerr << "typeids not equal in ValueSet_t" << std::endl;
	return false;
      }
      // if (!ValueSet_t::equals(b0, noChildren)) {
      // 	INFO_VALUE_SET("typeids not equal");
      // 	return false;
      // }
      const ValueSetClass& b = static_cast<const ValueSetClass&>(b0);
      if (!this->compare_value_maps(values, b.values, "values: "))
	return false;
      if (this->children.size() != b.children.size()) {
	INFO_VALUE_SET("number of children not equal: ",
		       this->children.size(), ", ", b.children.size());
	return false;
      }
#define DO_CHILD(child, dummy)					\
      if (*child != **(b.children[iChild])) return false
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return true;
    }
    bool operator==(const T& other) const {
      return static_cast<const T*>(this)->equals(static_cast<const ValueSet_t&>(other));
    }
    bool operator!=(const T& other) const {
      return !(operator==(other));
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    void check(const EnumType k, const std::string& context = "") const {
      ENSURE_VALUE_POINTERS_CONST;
      check_value_map(values, k, context);
    }
    // Value manipulation
    /**
       Add values to the set.
       \param new_values New values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in new_values that are
         not in the set will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in the set that are not in new_values.
       \param no_constants If true, an error will be raised if there are
         constant values in new_values that differ from the values in
	 the set.
       \param no_skipped If true, an error will be raised if there are
         skipped values in new_values.
     */
    void update_values(const std::map<EnumType, double>& new_values,
		       const std::string& context="",
		       bool allow_extras=false,
		       bool no_missing=false,
		       bool no_constants=false,
		       bool no_skipped=false) {
      ENSURE_VALUE_POINTERS;
      update_value_map(values, new_values,
		       context + "update_values: ",
		       allow_extras, no_missing,
		       no_constants, no_skipped);
    }
    /**
       Add values to the set by reading a parameter file.
       \param filename File containing values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in the file that are not
         in the set will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in the set that are not in the file.
       \param no_constants If true, an error will be raised if there are
         constant values in the file that differ from the values in the
	 set.
       \param no_skipped If true, an error will be raised if there are
         skipped values in the file.
     */
    void update_values(const std::string& filename,
		       const std::string& context="",
		       bool allow_extras=false,
		       bool no_missing=false,
		       bool no_constants=false,
		       bool no_skipped=false) {
      ENSURE_VALUE_POINTERS;
      update_value_map(values, filename,
		       context + "update_values[" + filename + "]: ",
		       allow_extras, no_missing,
		       no_constants, no_skipped);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    ValueType operator[](const EnumType k) {
      ENSURE_VALUE_POINTERS;
      return get_value(values, k);
    }
    /**
       Get the constant reference for the value associated with a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    const double& operator[](const EnumType k) const {
      ENSURE_VALUE_POINTERS_CONST;
      return get_value_const(values, k);
    }
    /**
       Record a value set before the value set is initialized so it can
         be ensured once the values are initialized.
       \param k Key to update value for.
       \param v New value for key.
     */
    void setPreInit(const EnumType& k, const double& v) {
      if (initialized()) return;
      // INFO_VALUE_SET(k, " = ", v);
      preinit_values[k] = v;
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    void set(const EnumType& k, const double& v) {
      ENSURE_VALUE_POINTERS;
      setPreInit(k, v);
      set_value(values, k, v);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
       \param[in] isGlymaID If true, k is a GlymaID.
     */
    void set(const std::string& name, const double& v,
	     const bool& isGlymaID = false) override {
      if (isGlymaID)
	return set(fromGlymaid(name), v);
      return set(fromNameWithAliases(name), v);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    void set(const int& k, const double& v) override {
      return set(BaseClass::int2key(k), v);
    }
    /**
       Update the value associated with a key in the value set from a map
         containing enzyme activity values.
       \param k Key to update value for.
       \param EnzymeAct Enzyme activities.
     */
    void setFromEnzymeAct(const EnumType k,
			  const std::map<std::string, double>& EnzymeAct) {
      ENSURE_VALUE_POINTERS;
      set_value_from_EnzymeAct(values, k, EnzymeAct);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value
     */
    ValueType get(const EnumType& k) {
      ENSURE_VALUE_POINTERS;
      return get_value(values, k);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value
     */
    double get(const EnumType& k) const {
      ENSURE_VALUE_POINTERS_CONST;
      return get_value_const(values, k);
    }
    /**
       Get the value associated with a key.
       \param name String representation of key to get value for.
       \param[in] isGlymaID If true, k is a GlymaID.
       \returns Value
     */
    double get(const std::string& name,
	       const bool& isGlymaID = false) const override {
      if (isGlymaID)
	return get(fromGlymaid(name));
      return get(fromNameWithAliases(name));
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value
     */
    double get(const int& k) const override {
      return get(BaseClass::int2key(k));
    }
    /**
       Add a value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    void insertOrig(const EnumType k, double* v,
		    const std::string& context="") {
      if (k == EnumClass::NONE || k == EnumClass::MAX)
	return;
      insert_value_orig(values, k, v, context);
    }
    /**
       Get the number of values in the set that would be included in an
         array produced by toArray.
       Common, public interface for the private _sizeArray function.
       \returns Value count.
     */
    std::size_t sizeArray() const override {
      // TODO: include children here instead of in overridden method?
      return static_cast<const T*>(this)->_sizeArray();
    }
    /**
       Get the values in an array ordering based on key order.
       Common, public interface for the private _toArray function.
       \returns Values array.
     */
    arr toArray() const override {
      // TODO: include children here instead of in overridden method?
      return static_cast<const T*>(this)->_toArray();
    }
    /**
       Update the values in the set from an array.
       Common, public interface for the private _fromArray method.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    void fromArray(const arr &vec, const std::size_t offset = 0) override {
      // TODO: include children here instead of in overridden method?
      static_cast<T*>(this)->_fromArray(vec, offset);
    }

    /**
       Add a child to this instance.
       \param[in] C Child.
     */
    void addChild(ValueSet_t** C) {
      children.push_back(C);
    }

    int flags = 0; /**< Bitwise flags describing the value set */
    std::map<EnumType, double*> values; /**< Values in the set. */
    std::map<EnumType, double> preinit_values; /**< Value set pre-initialization */
    std::vector<ValueSet_t**> children; /**< Child value sets. */

    // Methods that can be overridden by the inheriting class
  protected:
    virtual void _copyMembers(const T& other) {}
    virtual void __copyMembers(const T& other) {}
    virtual void _initMembers() {}
    virtual void _resetValues() {}
    virtual void _initValues() {}
    virtual void _initMemberPointers() {}
    virtual void _initChildren() {}
    virtual void _createChildren() {}
    static ValueSet_t* _newInstance(ValueSet_t* par=nullptr) {
      UNUSED(par);
      return nullptr;
    }
    virtual std::string _diff(const T& other,
			      std::size_t padKeys=0,
			      std::size_t padVals=0,
			      bool includePrefixes=false,
			      bool noChildren = false) const {
      ENSURE_VALUE_POINTERS_CONST;
      std::string out;
#define DO_CHILD(child, dummy)						\
      out += child->diff(**other.children[iChild], padKeys, padVals,	\
			 includePrefixes);
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      out += BaseClass::diff_value_maps(values, other.values,
					padKeys, padVals,
					includePrefixes);
      return out;
    }
    virtual std::ostream& _print(std::ostream &out,
				 const uint tab = 0, std::size_t pad = 0,
				 bool includePrefixes = false,
				 bool includeSkipped = false,
				 const std::vector<std::string>& skip_keys = {},
				 const std::map<std::string, std::string>& key_aliases = {},
				 bool noChildren = false) const {
      ENSURE_VALUE_POINTERS_CONST;
      uint child_tab = tab;
      if (!(noChildren || includePrefixes)) {
	const std::string space(tab * BaseClass::tab_size, ' ');
	out << space << error_prefix() << std::endl;
	child_tab++;
      }
#define DO_CHILD(child, dummy)						\
      child->print(out, child_tab, pad, includePrefixes, includeSkipped, \
		   skip_keys, key_aliases)
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return print_value_map(values, out, child_tab, pad,
			     includePrefixes, includeSkipped,
			     skip_keys, key_aliases);
    }
    virtual std::size_t _sizeArray() const {
      ENSURE_VALUE_POINTERS_CONST;
      bool noChildren = false;
      std::size_t out = values.size();
#define DO_CHILD(child, dummy) out += child->sizeArray()
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return out;
    }
    virtual arr _toArray() const {
      ENSURE_VALUE_POINTERS_CONST;
      bool noChildren = false;
      arr out;
#define DO_CHILD(child, dummy)						\
      arr ivec = child->toArray();					\
      out.insert(out.end(), ivec.begin(), ivec.end())
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      value_map_toArray(values, out);
      return out;
    }
    virtual void _fromArray(const arr &vec, const std::size_t offset = 0) {
      ENSURE_VALUE_POINTERS;
      ENSURE_CHILD_POINTERS;
      bool noChildren = false;
      std::size_t ioffset = offset;
#define DO_CHILD(child, dummy)						\
      child->fromArray(vec, ioffset);					\
      ioffset += child->sizeArray()
      DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
      value_map_fromArray(values, vec, ioffset);
    }
  };
  
  /**
     Template class to provide methods for manipulating a set of static
     class values used by a model run that can be set via input files.

     \tparam T Child class with virtual & static methods.
     \tparam U Parent class with virtual & static methods.
     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  class EPHOTO_API ValueSetStatic : public ValueSetBase<T, U, ID, PT> {
  public:
    typedef ValueSetStatic<T, U, ID, PT> ValueSetClass;
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<T, U, ID, PT>)
    ADD_METHODS_VALUE_SET_T_BOTH(STUB_STATIC_VALUE_SET)

    /** \copydoc ValueSet::getValueSet */
    static const ValueSet_t* getValueSet() {
      return getValueSetClass();
    }
    
    /** \copydoc ValueSet::getValueMap */
    static std::map<int, double> getValueMap(const bool preinit=false) {
      if (preinit)
	return BaseClass::export_value_map(preinit_values, CONTEXT_VALUE_SET);
      return BaseClass::export_value_map(values, CONTEXT_VALUE_SET);
    }
    /** \copydoc ValueSet::setValueMap */
    static void setValueMap(const std::map<int, double>& map,
			    const bool setinit=false,
			    const bool preinit=false) {
      if (preinit) {
	if (setinit)
	  ERROR_VALUE_SET("preinit and setinit cannot both be true");
	BaseClass::import_value_map(preinit_values, map, CONTEXT_VALUE_SET);
	return;
      }
      BaseClass::import_value_map(values, map, CONTEXT_VALUE_SET);
      if (setinit)
	static_flags |= BaseClass::VS_FLAG_INIT_VALUES;
    }
    
    /** \copydoc ValueSet::initMembers */
    static void initMembers(const bool noChildren=false) {
      initStaticMembers(true);
      // initMemberPointers(true); // Called by initStaticMembers
      T::_initMembers();
      DO_VALUE_SET_CHILD_CLASSES(initMembers, ());
    }
    /** \copydoc ValueSet::resetValues */
    static void resetValues(const bool noChildren=false) {
      ENSURE_VALUE_POINTERS;
      reset_value_map(values);
      T::_resetValues();
      DO_VALUE_SET_CHILD_CLASSES(resetValues, ());
      static_flags &= ~BaseClass::VS_FLAG_INIT_VALUES;
    }
    /** \copydoc ValueSet::initialized */
    static bool initialized() {
      return (static_flags & BaseClass::VS_FLAG_INIT_VALUES);
    }
    /** \copydoc ValueSet::initValues */
    static void initValues(const bool noDefaults=false,
			   const bool force=false,
			   const bool noChildren=false) {
      if ((!force) && initialized()) return;
      initMembers(true);
      if (!noDefaults)
	init_value_map(values, initialized());
      T::_initValues();
      DO_VALUE_SET_CHILD_CLASSES(initValues, (noDefaults, force));
      if (!preinit_values.empty()) {
	copy_value_map(values, preinit_values, "initValues: ",
		       false, true, true, false, true);
      }
      static_flags |= BaseClass::VS_FLAG_INIT_VALUES;
    }

    /** \copydoc ValueSet::initMemberPointers */
    static void initMemberPointers(const bool noChildren=false) {
      if (static_flags & BaseClass::VS_FLAG_INIT_POINTERS) return;
      BaseClass::initDefaults();
      T::_initMemberPointers();
      DO_VALUE_SET_CHILD_CLASSES(initMemberPointers, ());
      static_flags |= BaseClass::VS_FLAG_INIT_POINTERS;
    }
    /** \copydoc ValueSetBase::initStaticMembers */
    static void initStaticMembers(const bool noChildren = false) {
      initMemberPointers(true);
      ParentClass::initStaticMembers(noChildren);
    }

    /**
       Disabled method to create an instance of the class.
     */
    static ValueSet_t* newInstance(ValueSet_t* par = nullptr) {
      UNUSED(par);
      ERROR_VALUE_SET("Cannot create instances of ValueSetStatic subclasses");
      return nullptr;
    }
      
    /**
       Get the map containing pointers to the static class members.
       \return Map between member names and pointers to static class
         members.
     */
    static std::map<std::string, double*> getPointerMap() {
      return get_pointer_map(values);
    }

    /** Get the iterator pointing to the start of the value set */
    static iterator begin() {
      ENSURE_VALUE_POINTERS;
      return values.begin();
    }
    /** Get the iterator pointing to the end of the value set */
    static iterator end() {
      ENSURE_VALUE_POINTERS;
      return values.end();
    }
    /** Get the constant iterator pointing to the start of the value set */
    // Inspection utilities
    /** \copydoc ValueSetClass_t::max_value_width */
    static std::size_t max_value_width(bool noChildren = false) {
      static std::size_t out = 0;
      static std::size_t outBase = 0;
      if (outBase == 0) {
	outBase = BaseClass::value_width_value_map(values);
      }
      if (noChildren) return outBase;
      if (out == 0) {
	out = outBase;
	std::size_t iout = 0;
#define DO_CHILD(child, dummy)						\
	iout = child->max_value_width();				\
	if (iout > out) out = iout
	DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      }
      return out;
    }
    /** \copydoc ValueSet::diff */
    static std::string diff(const ValueSet_t& other,
			    std::size_t padKeys=0,
			    std::size_t padVals=0,
			    bool includePrefixes=false,
			    bool noChildren = false) {
      UNUSED(other);
      if (padKeys == 0) {
	padKeys = max_field_width(noChildren);
      }
      if (padVals == 0) {
	padVals = max_value_width(noChildren);
      }
      return T::_diff(padKeys, padVals, includePrefixes, noChildren);
    }
    /** \copydoc ValueSet::print */
    static std::ostream& print(std::ostream &out,
			       const uint tab = 0, std::size_t pad = 0,
			       bool includePrefixes = false,
			       bool includeSkipped = false,
			       const std::vector<std::string>& skip_keys = {},
			       const std::map<std::string, std::string>& key_aliases = {},
			       bool noChildren = false) {
      if (pad == 0) pad = max_field_width(noChildren);
      return T::_print(out, tab, pad, includePrefixes, includeSkipped,
		       skip_keys, key_aliases, noChildren);
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    static void check(const EnumType k, const std::string& context = "") {
      ENSURE_VALUE_POINTERS;
      check_value_map(values, k, context);
    }
    // Value manipulation
    /**
       Add values to the set.
       \param new_values New values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in new_values that are
         not in the set will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in the set that are not in new_values.
       \param no_constants If true, an error will be raised if there are
         constant values in new_values that differ from the values in
	 the set.
       \param no_skipped If true, an error will be raised if there are
         skipped values in new_values.
     */
    static void update_values(const std::map<EnumType, double>& new_values,
			      const std::string& context="",
			      bool allow_extras=false,
			      bool no_missing=false,
			      bool no_constants=false,
			      bool no_skipped=false) {
      ENSURE_VALUE_POINTERS;
      update_value_map(values, new_values,
		       context + "update_values: ",
		       allow_extras, no_missing,
		       no_constants, no_skipped);
    }
    /**
       Add values to the set by reading a parameter file.
       \param filename File containing values to add to the set.
       \param context String providing context for error messages.
       \param allow_extras If true, extra values in the file that are not
         in the set will be allowed.
       \param no_missing If true, an error will be raised if there are
         values in the set that are not in the file.
       \param no_constants If true, an error will be raised if there are
         constant values in the file that differ from the values in the
	 set.
       \param no_skipped If true, an error will be raised if there are
         skipped values in the file.
     */
    static void update_values(const std::string& filename,
			      const std::string& context="",
			      bool allow_extras=false,
			      bool no_missing=false,
			      bool no_constants=false,
			      bool no_skipped=false) {
      ENSURE_VALUE_POINTERS;
      update_value_map(values, filename,
		       context + "update_values[" + filename + "]: ",
		       allow_extras, no_missing,
		       no_constants, no_skipped);
    }
    /** \copydoc ValueSet::setPreInit */
    static void setPreInit(const EnumType& k, const double& v) {
      if (initialized()) return;
      // INFO_VALUE_SET(k, " = ", v);
      preinit_values[k] = v;
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set(const EnumType& k, const double& v) {
      ENSURE_VALUE_POINTERS;
      setPreInit(k, v);
      set_value(values, k, v);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
       \param[in] isGlymaID If true, k is a GlymaID.
     */
    static void set(const std::string& name, const double& v,
		    const bool& isGlymaID = false) {
      if (isGlymaID)
	return set(fromGlymaid(name), v);
      return set(fromNameWithAliases(name), v);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set(const int& k, const double& v) {
      return set(BaseClass::int2key(k), v);
    }
    /**
       Update the value associated with a key in the value set from a map
         containing enzyme activity values.
       \param k Key to update value for.
       \param EnzymeAct Enzyme activities.
     */
    static void setFromEnzymeAct(const EnumType k,
				 const std::map<std::string, double>& EnzymeAct) {
      ENSURE_VALUE_POINTERS;
      set_value_from_EnzymeAct(values, k, EnzymeAct);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value.
     */
    static double get(const EnumType& k) {
      ENSURE_VALUE_POINTERS;
      return get_value_const(values, k);
    }
    /**
       Get the value associated with a key.
       \param name String representation of key to get value for.
       \param[in] isGlymaID If true, k is a GlymaID.
       \returns Value
     */
    static double get(const std::string& name,
		      const bool& isGlymaID = false) {
      if (isGlymaID)
	return get(fromGlymaid(name));
      return get(fromNameWithAliases(name));
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value.
     */
    static double get(const int& k) {
      return get(BaseClass::int2key(k));
    }
    /**
       Add a value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    static void insertOrig(const EnumType k, double* v,
			   const std::string& context="") {
      if (k == EnumClass::NONE || k == EnumClass::MAX)
	return;
      insert_value_orig(values, k, v, context);
    }
    /** \copydoc ValueSetBase::reset */
    static void reset(const bool noChildren = false) {
      ENSURE_VALUE_POINTERS;
      reset_value_map(values);
      ParentClass::reset(noChildren);
    }
    /**
       Get the number of values in the set that would be included in an
         array produced by toArray.
       \returns Value count.
     */
    static std::size_t sizeArray() {
      return T::_sizeArray();
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    static arr toArray() {
      return T::_toArray();
    }
    /**
       Update the values in the set from an array.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    static void fromArray(const arr &vec, const std::size_t offset = 0) {
      T::_fromArray(vec, offset);
    }
    
    /**
       Add a child class to this classes set.
       \tparam C Child class.
     */
    template<typename C>
    static void addChildClass() {
      child_classes.push_back(new ValueSetStaticClassType<C>());
    }

    static std::map<EnumType, double*> values; /**< Values in the set. */
    static std::map<EnumType, double> preinit_values; /**< Value set pre-initialization */

    // Methods that can be overridden by the inheriting class
  protected:
    static void _initMembers() {}
    static void _resetValues() {}
    static void _initValues() {}
    static std::string _diff(std::size_t padKeys=0,
			     std::size_t padVals=0,
			     bool includePrefixes=false,
			     bool noChildren = false) {
      UNUSED(padKeys);
      UNUSED(padVals);
      UNUSED(includePrefixes);
      UNUSED(noChildren);
      return "";
    }
    static std::ostream& _print(std::ostream &out,
				const uint tab = 0, std::size_t pad = 0,
				bool includePrefixes = false,
				bool includeSkipped = false,
				const std::vector<std::string>& skip_keys = {},
				const std::map<std::string, std::string>& key_aliases = {},
				bool noChildren = false) {
      ENSURE_VALUE_POINTERS;
      uint child_tab = tab;
      if (!(noChildren || includePrefixes)) {
	const std::string space(tab * BaseClass::tab_size, ' ');
	out << space << error_prefix() << std::endl;
	child_tab++;
      }
#define DO_CHILD(child, dummy)						\
      child->print(out, child_tab, pad, includePrefixes, includeSkipped,\
		   skip_keys, key_aliases)
      DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      return print_value_map(values, out, child_tab, pad,
			     includePrefixes, includeSkipped,
			     skip_keys, key_aliases);
    }
    static void _initMemberPointers() {}
    static std::size_t _sizeArray() {
      ENSURE_VALUE_POINTERS;
      bool noChildren = false;
      std::size_t out = values.size();
#define DO_CHILD(child, dummy) out += child->sizeArray()
      DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
      return out;
    }
    static arr _toArray() {
      ENSURE_VALUE_POINTERS;
      bool noChildren = false;
      arr out;
#define DO_CHILD(child, dummy)						\
      arr ivec = child->toArray();					\
      out.insert(out.end(), ivec.begin(), ivec.end())
      DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
      value_map_toArray(values, out);
      return out;
    }
    static void _fromArray(const arr &vec, const std::size_t offset = 0) {
      ENSURE_VALUE_POINTERS;
      ENSURE_CHILD_CLASS_POINTERS;
      bool noChildren = false;
      std::size_t ioffset = offset;
#define DO_CHILD(child, dummy)						\
      child->fromArray(vec, ioffset);					\
      ioffset += child->sizeArray()
      DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
      value_map_fromArray(values, vec, ioffset);
    }
    
  };

  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<T, U, ID, PT>::EnumType, double*>
  ValueSetStatic<T, U, ID, PT>::values = {};
  
  template<class T, class U, MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<T, U, ID, PT>::EnumType, double>
  ValueSetStatic<T, U, ID, PT>::preinit_values = {};
  
}
    

#define DECLARE_PARAM_DUMMY_(suffix, mod)				\
    class mod ## suffix :						\
        public suffix ## Base<mod ## suffix, MODULE_ ## mod> {		\
    public:								\
	DECLARE_VALUE_SET_BASE(mod ## suffix, suffix ## Base<mod ## suffix, MODULE_ ## mod>) \
    }
#define DECLARE_PARAM_DUMMY(suffix, mod)				\
    DECLARE_PARAM_DUMMY_(suffix, mod)
#define DECLARE_PARAM_DUMMY_PACKED(args)			\
    DECLARE_PARAM_DUMMY args
#define DECLARE_PARAM_DUMMIES(suffix, ...)				\
    FOR_EACH_WITH_ARGS_PACKED(DECLARE_PARAM_DUMMY_PACKED,		\
			      (suffix), __VA_ARGS__)
#define DECLARE_PARAM_BASE_CLASS(name, pt)				\
    template<typename T, MODULE ID = MODULE_NONE>			\
    class name ## Base : public ValueSet<T, T, ID, PARAM_TYPE_ ## pt> {	\
    public:								\
    DECLARE_VALUE_SET_BASE(name ## Base, ValueSet<T, T, ID, PARAM_TYPE_ ## pt>) \
    name ## Base() :							\
    ValueSet<T, T, ID, PARAM_TYPE_ ## pt>() {}				\
    virtual ~name ## Base() {}						\
    name ## Base(const name ## Base &other) :				\
    ValueSet<T, T, ID, PARAM_TYPE_ ## pt>(other) {}			\
    name ## Base& operator=(const name ## Base& other) {		\
      return *this;							\
    }									\
    }
#define DECLARE_PARAM_BASE_(namespc, name, pt, ...)			\
    namespace ePhotosynthesis {						\
    namespace namespc {					                \
      DECLARE_PARAM_BASE_CLASS(name, pt);				\
      DECLARE_PARAM_DUMMIES(name, __VA_ARGS__);				\
    }									\
    }
#define DECLARE_PARAM_BASE(namespc, name, pt, dummies)			\
    DECLARE_PARAM_BASE_(namespc, name, pt, UNPACK_MACRO dummies)
#define DECLARE_PARAM_BASE_NODUMMIES(namespc, name, pt)			\
    namespace ePhotosynthesis {						\
    namespace namespc {					                \
      DECLARE_PARAM_BASE_CLASS(name, pt);				\
    }									\
    }

#define CASE_MEMBER_NESTED(mod, prefix, fdefault2, prefix2, mod2, fmembers2, X, ...) \
  case (prefix ## mod) : {						\
      try {								\
	DEFER(SWITCH_MEMBER_DEFERED)()(PACK_MACRO(fdefault2),		\
				       prefix2, mod2,			\
				       (fmembers2(mod)),		\
				       PACK_MACRO(X),			\
				       __VA_ARGS__, mod);		\
      } catch (const std::runtime_error& ex) {				\
	ERROR_VALUE_SET_NESTED(ex, ", " +				\
			       utils::enum_key2string(prefix ## mod));	\
      }									\
      break;								\
  }
#define CASE_MEMBER(mod, prefix, X, ...)				\
  case (prefix ## mod) : {						\
      X(__VA_ARGS__, mod);						\
      break;								\
  }
#define CASE_MEMBER_DEFER(mod, prefix, X, ...)				\
  case (prefix ## mod) : {						\
      DEFER(X)()(__VA_ARGS__, mod);					\
      break;								\
  }
#define CASE_MEMBER_PACKED(args)					\
  CASE_MEMBER args
#define CASE_MEMBER_DEFER_PACKED(args)					\
  CASE_MEMBER_DEFER args
#define CASE_MEMBER_NESTED_PACKED(args)					\
  CASE_MEMBER_NESTED args
#define DEFAULT_ERROR(mod)						\
  ERROR_VALUE_SET("Unsupported member: ",				\
		  utils::enum_key2string(mod))
#define SWITCH_MEMBER_(fdefault, mod, case_macro, args, ...)		\
  switch (mod) {							\
    FOR_EACH_WITH_SUFFIX_ARGS_PACKED(case_macro, args, __VA_ARGS__)	\
  default: {								\
      fdefault(mod);							\
  }									\
  }
#define SWITCH_MEMBER_ERROR(prefix, mod, members, ...)	\
    SWITCH_MEMBER_(DEFAULT_ERROR, mod, CASE_MEMBER_PACKED,		\
		   (prefix, __VA_ARGS__), UNPACK_PARENS(members))
#define SWITCH_MEMBER(fdefault, prefix, mod, members, ...)		\
    SWITCH_MEMBER_(PACK_MACRO(fdefault), mod, CASE_MEMBER_PACKED,	\
		   (prefix, __VA_ARGS__), UNPACK_PARENS(members))
		 
#define SWITCH_MEMBER_DEFER(fdefault, prefix, mod, members, ...)	\
    SWITCH_MEMBER_(PACK_MACRO(fdefault), mod, CASE_MEMBER_DEFER_PACKED,	\
		   (prefix, __VA_ARGS__), UNPACK_PARENS(members))
#define SWITCH_MEMBER_NESTED(fdefault, prefix, mod, members, fdefault2, prefix2, mod2, fmembers2, X, ...) \
    SWITCH_MEMBER_(PACK_MACRO(fdefault), mod, CASE_MEMBER_NESTED_PACKED, \
		   (prefix, PACK_MACRO(fdefault2), prefix2, mod2, PACK_MACRO(fmembers2), \
		    PACK_MACRO(X), __VA_ARGS__),			\
		   UNPACK_PARENS(members))

#define SWITCH_MEMBER_DEFERED() SWITCH_MEMBER

#define SWITCH_MOD(...)					\
    SWITCH_MEMBER(DEFAULT_ERROR, MODULE_, __VA_ARGS__)
#define SWITCH_PT(...)						\
    SWITCH_MEMBER(DEFAULT_ERROR, PARAM_TYPE_, __VA_ARGS__)

#define SWITCH_MOD_DEFER(...)					\
    SWITCH_MEMBER_DEFER(DEFAULT_ERROR, MODULE_, __VA_ARGS__)
#define SWITCH_PT_DEFER(...)						\
    SWITCH_MEMBER_DEFER(DEFAULT_ERROR, PARAM_TYPE_, __VA_ARGS__)

#define SWITCH_MOD_DEFERED() SWITCH_MOD
#define SWITCH_PT_DEFERED() SWITCH_PT
#define SWITCH_MOD_DEFER_DEFERED() SWITCH_MOD_DEFER
#define SWITCH_PT_DEFER_DEFERED() SWITCH_PT_DEFER

#define SWITCH_MOD_AND_PT_ERROR(mod, mod_members, pt, pt_fmembers, X, ...) \
    EVAL(SWITCH_MEMBER_NESTED(DEFAULT_ERROR, MODULE_, mod, mod_members,	\
			      DEFAULT_ERROR, PARAM_TYPE_, pt, PACK_MACRO(pt_fmembers), \
			      PACK_MACRO(X), __VA_ARGS__))
#define SWITCH_MOD_AND_PT(mod, mod_members, pt, pt_fmembers, X, ...)	\
    EVAL(SWITCH_MEMBER_NESTED(EMPTY, MODULE_, mod, mod_members,		\
			      EMPTY, PARAM_TYPE_, pt, PACK_MACRO(pt_fmembers), \
			      PACK_MACRO(X), __VA_ARGS__))

#define SWITCH_MOD_ALL(mod, X, ...)				\
  SWITCH_MOD(mod, (MEMBER_NAMES_MODULE), X, __VA_ARGS__)
#define SWITCH_PT_ALL(pt, X, ...)			\
  SWITCH_PT(pt, (MEMBER_NAMES_PARAM), X, __VA_ARGS__)

#define CASE_PAIR__(mod, mod2, prefix, prefix2, var, var2, X, ...)	\
  if (var == CONCATENATE(prefix, mod) && var2 == CONCATENATE(prefix2, mod2)) { \
    X(__VA_ARGS__, mod, mod2);						\
  } else
#define CASE_PAIR_(...) CASE_PAIR__(__VA_ARGS__)
#define CASE_PAIR(pair, ...) CASE_PAIR_(EXPAND pair, __VA_ARGS__)
#define SWITCH_PAIRS_(fdefault, mod, mod2, args, ...)			\
  FOR_EACH_WITH_SUFFIX_ARGS(CASE_PAIR, args, __VA_ARGS__)		\
  {									\
    fdefault(mod, mod2);						\
  }
#define SWITCH_PAIRS_ERROR(prefix, mod, prefix2, mod2, members, ...)	\
  SWITCH_PAIRS_(DEFAULT_ERROR, mod, mod2,				\
		(prefix, prefix2, mod, mod2, __VA_ARGS__),		\
		UNPACK_PARENS(members))
#define SWITCH_PAIRS(fdefault, prefix, mod, prefix2, mod2, members, ...) \
  SWITCH_PAIRS_(PACK_MACRO(fdefault), mod, mod2,			\
		(prefix, prefix2, mod, mod2, __VA_ARGS__),		\
		UNPACK_PARENS(members))
#define SWITCH_MOD_AND_PT_PAIRS(mod, pt, members, ...)			\
  SWITCH_PAIRS(EMPTY, MODULE_, mod, PARAM_TYPE_, pt,			\
	       members, __VA_ARGS__)
  

// Parameters for param_types
// 1. namespace of class
// 2. class suffix
// 3. variable suffix
// 4. variable suffix in module
// 5. variable prefix in module
#define VARS_PARAM_PT_COND(idx) conditions, Condition, 2OUT, _con ## idx, *
#define VARS_PARAM_PT_POOL(idx) pool, Pool, _Pool, _Pool, theVars ## idx->
#define VARS_PARAM_PT_RC(idx) RC, RC, _RC, _RC, theVars ## idx->
#define VARS_PARAM_PT_KE(idx) KE, KE, _KE, _KE, theVars ## idx->
#define VARS_PARAM_PT_VEL_SERIES(idx)			\
    TimeSeries<vel, Vel>, _VEL, _VEL, theVars ## idx->
#define VARS_PARAM_PT_VEL(idx) vel, Vel, _Vel, _Vel, theVars ## idx->
#define VARS_PARAM_PT_MOD(idx) modules, , , , NONE
#define VARS_PARAM_PT(pt, idx) (VARS_PARAM_PT_ ## pt(idx))
#define VARS_PARAM_PT_NAMESPACE_(ns, cs, vs, ms, mp) ns
#define VARS_PARAM_PT_CLASS_SUFFIX_(ns, cs, vs, ms, mp) cs
#define VARS_PARAM_PT_VAR_SUFFIX_(ns, cs, vs, ms, mp) vs
#define VARS_PARAM_PT_MOD_PREFIX_(ns, cs, vs, ms, mp) mp
#define VARS_PARAM_PT_MOD_SUFFIX_(ns, cs, vs, ms, mp) ms
#define VARS_PARAM_PT_NAMESPACE(pt, idx)			\
  EXPAND(VARS_PARAM_PT_NAMESPACE_ VARS_PARAM_PT(pt, idx))
#define VARS_PARAM_PT_CLASS_SUFFIX(pt, idx)		\
  VARS_PARAM_PT_CLASS_SUFFIX_ VARS_PARAM_PT(pt, idx)
#define VARS_PARAM_PT_VAR_SUFFIX(pt, idx)		\
  VARS_PARAM_PT_VAR_SUFFIX_ VARS_PARAM_PT(pt, idx)
#define VARS_PARAM_PT_MOD_PREFIX(pt, idx)		\
  EXPAND(VARS_PARAM_PT_MOD_PREFIX_ VARS_PARAM_PT(pt, idx))
#define VARS_PARAM_PT_MOD_SUFFIX(pt, idx)		\
  VARS_PARAM_PT_MOD_SUFFIX_ VARS_PARAM_PT(pt, idx)
#define VARS_CLASS_VAR_LOCAL(mod, pt)		\
  CONCATENATE(mod, VARS_PARAM_PT_CLASS_SUFFIX(pt,))
#define VARS_CLASS_VAR(mod, pt)			\
  VARS_PARAM_PT_NAMESPACE(pt,)::VARS_CLASS_VAR_LOCAL(mod, pt)
#define VARS_INST_VAR(mod, pt)			\
  CONCATENATE(mod, VARS_PARAM_PT_VAR_SUFFIX(pt,))
#define VARS_INST_VAR_MOD_IDX(mod, pt, idx)				\
  VARS_PARAM_PT_MOD_PREFIX(pt, idx)					\
  CONCATENATE(mod,							\
	      VARS_PARAM_PT_MOD_SUFFIX(pt, idx))
#define VARS_INST_VAR_MOD_BASE_IDX(mod, pt, idx)			\
  CONCATENATE(mod,							\
	      VARS_PARAM_PT_MOD_SUFFIX(pt, idx))
#define VARS_INST_VAR_MOD(mod, pt)					\
  VARS_INST_VAR_MOD_IDX(mod, pt, )
#define VARS_INST_VAR_MOD_BASE(mod, pt)					\
  VARS_INST_VAR_MOD_BASE_IDX(mod, pt, )
#define MOD2PT(mod) PARAM_TYPES_ ## mod
#define MOD2PT_COND(mod)						\
  CALL_MACRO(FIRST_ARG, MOD2PT(mod))
#define MOD2PT_NO_COND(mod)			\
  CALL_MACRO(GET_ARGS_AFTER_1, MOD2PT(mod))
#define MOD2PT_CLASS(mod) MOD, MOD2PT(mod)
#define MOD2PT_DEFERED() MOD2PT
#define MOD2PT_PAIRS(mod, X)						\
    FOR_EACH_WITH_PREFIX_ARGS_COMMA(ADD_PARENS, (mod),			\
       				    X(mod))
#define MOD2PT_PAIRS_DEFERED() MOD2PT_PAIRS
#define ALL_MOD_PT_PAIRS						\
    EVAL(FOR_EACH_GENERIC(MOD2PT_PAIRS_DEFERED,				\
			  CALL_WITH_SUFFIX_ARGS_DEFER,			\
			  SEP_COMMA, (MOD2PT),				\
			  EXPAND VARS_CLASS_MODULES))
#define INST_MOD_PT_PAIRS						\
    EVAL(FOR_EACH_GENERIC(MOD2PT_PAIRS_DEFERED,				\
			  CALL_WITH_SUFFIX_ARGS_DEFER,			\
			  SEP_COMMA, (MOD2PT_COND),			\
			  EXPAND VARS_INST_MODULES)),			\
    EVAL(FOR_EACH_GENERIC(MOD2PT_PAIRS_DEFERED,				\
			  CALL_WITH_SUFFIX_ARGS_DEFER,			\
			  SEP_COMMA, (MOD2PT_NO_COND),			\
			  EXPAND VARS_INST_MODULES, FIBF))

#define VARS_INST_MODULES (BF, FI, PR, PS, RROEA, RedoxReg, RuACT, SUCS, XanCycle)
#define VARS_INST_PARAM_TYPES (COND, POOL, RC, KE, VEL)
#define VARS_INST_CONNECTIONS				\
  BF_FI, EPS_SUCS, FIBF_PSPR, PR_PS, PSPR_SUCS,		\
    RROEA_EPS, RedoxReg_RA, RuACT_EPS, XanCycle_BF

#define VARS_CLASS_MODULES (PACK_MACRO(MEMBER_NAMES_MODULE))
#define VARS_CLASS_PARAM_TYPES PACK_MACRO((MEMBER_NAMES_PARAM))
#define GET_VALUE_SET(mod, pt)			\
  getValueSet(mod, pt, false, {}, std::string(__func__) + ": ")
#define GET_VALUE_SET_CLASS(mod, pt)				\
  getValueSetClass(mod, pt, false, std::string(__func__) + ": ")
#define GET_VALUE_SET_COND(mod, pt)		\
  getValueSet(mod, pt, false, conditions, std::string(__func__) + ": ")


#define VARS_INST_IS_MODULE(var)				\
    VAR_IN_PREFIXED_LIST(var, MODULE_, EXPAND VARS_INST_MODULES)
#define VARS_INST_IS_PARAM_TYPE(var)					\
    VAR_IN_PREFIXED_LIST(var, PARAM_TYPE_, EXPAND VARS_INST_PARAM_TYPES)
#define VARS_IS_SKIPPED(mod, pt)		\
    (mod != MODULE_RROEA && pt == PARAM_TYPE_KE)
#define VARS_CLASS_APPLY_TO_MEMBERS(X)				\
  FOR_EACH(CONCATENATE(X, _PACKED), ALL_MOD_PT_PAIRS);		\
  FOR_EACH(CONCATENATE(X, _CORE), EXPAND VARS_INST_MODULES);	\
  FOR_EACH(CONCATENATE(X, _CONNECTION), VARS_INST_CONNECTIONS)
#define VARS_INST_APPLY_TO_MEMBERS(X)				\
  FOR_EACH(CONCATENATE(X, _PACKED), INST_MOD_PT_PAIRS);		\
  FOR_EACH(CONCATENATE(X, _CORE), EXPAND VARS_INST_MODULES);	\
  FOR_EACH(CONCATENATE(X, _CONNECTION), VARS_INST_CONNECTIONS)

#define VARS_INST_CALL(X, args, mod, pt)	\
    VARS_INST_VAR(mod, pt).X args
#define VARS_INST_CALL_STATIC(X, args, mod)	\
    modules::mod::X args
#define VARS_INST_CALL_VARS(X, args)		\
    X args
#define VARS_CLASS_CALL(X, args, mod, pt)	\
    VARS_CLASS_VAR(mod, pt)::X args
#define VARS_CLASS_CALL_STATIC VARS_INST_CALL_STATIC
#define VARS_CLASS_CALL_VARS VARS_INST_CALL_VARS
#define VARS_RETURN_CALL(X, ...)		\
  return X(__VA_ARGS__)
  
#define VARS_ITER_ALL_BASE(for_instance, include_cond, m, pt, X, ...)	\
  for (std::vector<MODULE>::const_iterator m = ALL_MODULE.begin();	\
       m != ALL_MODULE.end(); m++) {					\
    if (*m == MODULE_ALL) {						\
      X(*m, PARAM_TYPE_VARS, __VA_ARGS__);				\
    } else {								\
      X(*m, PARAM_TYPE_MOD, __VA_ARGS__);				\
      const std::vector<PARAM_TYPE> param_types = getParamTypes(*m, for_instance, include_cond); \
      for (std::vector<PARAM_TYPE>::const_iterator pt = param_types.begin(); \
	   pt != param_types.end(); pt++) {				\
	X(*m, *pt, __VA_ARGS__);					\
      }									\
    }									\
    }
#define VARS_ITER_ALL_CLASS(...) VARS_ITER_ALL_BASE(false, false, __VA_ARGS__)

#define VARS_ITER_ALL_MACRO_BASE(Xget, PT_ARGS, m, pt, X, X_STATIC, X_VARS, ...) \
  for (std::vector<MODULE>::const_iterator m = ALL_MODULE.begin();	\
       m != ALL_MODULE.end(); m++) {					\
    std::vector<PARAM_TYPE>::const_iterator pt;				\
    if (*m == MODULE_ALL) {						\
      auto _tmp = Xget(MODULE_ALL, PARAM_TYPE_VARS);			\
      if (_tmp) {							\
	X_VARS(_tmp, __VA_ARGS__);					\
      }									\
    } else {								\
      ValueSetClass_t* module = getModule(*m, false, std::string(__func__) + ": "); \
      X_STATIC(module, __VA_ARGS__);					\
      const std::vector<PARAM_TYPE> param_types = getParamTypes(*m, EXPAND PT_ARGS); \
      for (std::vector<PARAM_TYPE>::const_iterator pt = param_types.begin(); \
	   pt != param_types.end(); pt++) {				\
	auto _tmp = Xget(*m, *pt);					\
	if (_tmp) {							\
	  X(_tmp, __VA_ARGS__);						\
	}								\
      }									\
    }									\
  }
#define VARS_ITER_MACRO(X, ...)						\
  VARS_ITER_ALL_MACRO_BASE(GET_VALUE_SET, (true, false), m, pt,		\
			   PACK_MACRO(X), PACK_MACRO(X), PACK_MACRO(X),	\
			   __VA_ARGS__)
#define VARS_ITER_MACRO_COND(X, ...)					\
  VARS_ITER_ALL_MACRO_BASE(GET_VALUE_SET_COND,				\
			   (true, true, conditions), m, pt,		\
			   PACK_MACRO(X), PACK_MACRO(X), PACK_MACRO(X),	\
			   __VA_ARGS__)
#define VARS_ITER_MACRO_INST(X, ...)					\
  VARS_ITER_ALL_MACRO_BASE(GET_VALUE_SET, (true, false), m, pt,		\
			   PACK_MACRO(X), EMPTY, PACK_MACRO(X),		\
			   __VA_ARGS__)
#define VARS_ITER_MACRO_CLASS(X, ...)					\
  VARS_ITER_ALL_MACRO_BASE(GET_VALUE_SET_CLASS, (false, false), m, pt,	\
			   PACK_MACRO(X), PACK_MACRO(X), PACK_MACRO(X),	\
			   __VA_ARGS__)
