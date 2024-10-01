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

#define CHECK_RATIO_IDX(act, exp, key)					\
  if (it->first == key && act != exp) {					\
    throw std::runtime_error("CHECK[" + utils::enum_key2string(key)	\
			     + "]: expected = " + std::to_string(exp)	\
			     + ", actual = " + std::to_string(act));	\
  }

namespace ePhotosynthesis {

#ifdef CHECK_VALUE_SET_ALTS
#define DEBUG_VALUE_SET_MSG(x)			\
  std::cerr << x
#define DEBUG_VALUE_SET_(...)			\
  FOR_EACH(DEBUG_VALUE_SET_MSG, __VA_ARGS__)
#define DEBUG_VALUE_SET(...)						\
  DEBUG_VALUE_SET_(error_prefix(), ": ", __func__, ": ",		\
		   __VA_ARGS__, std::endl)
#define DEBUG_VALUE_SET_NOARGS						\
  DEBUG_VALUE_SET_(error_prefix(), ": ", __func__, std::endl)
#define DEBUG_VALUE_SET_RAW(...)		\
  DEBUG_VALUE_SET_(__VA_ARGS__, "")
#define DEBUG_VALUE_SET_END(...)		\
  DEBUG_VALUE_SET_(__VA_ARGS__, std::endl)
#define DEBUG_VALUE_SET_NOEND(...)			\
  DEBUG_VALUE_SET_(error_prefix(), ": ", __func__, ": ", __VA_ARGS__)
#else
#define DEBUG_VALUE_SET(...)
#define DEBUG_VALUE_SET_NOARGS
#define DEBUG_VALUE_SET_RAW(...)
#define DEBUG_VALUE_SET_END(...)
#define DEBUG_VALUE_SET_NOEND(...)
#endif // CHECK_VALUE_SET_ALTS
  
#define DECLARE_VALUE_SET_MEMBER_RECORD			\
  static const StaticMemberClass _adjustments
  /*
#define TYPED_VALUE_SET_MEMBERS(name, ...)		\
  FOR_EACH_WITH_ARGS_COMMA(JOIN, (name::EnumClass::), __VA_ARGS__)
  */
#define DEFINE_VALUE_SET_MEMBER_RECORD(prefix, name)			\
  const typename prefix name::StaticMemberClass prefix name::_adjustments = \
    prefix name::StaticMemberClass()
#define DEFINE_VALUE_SET_MEMBER_RECORD_HEADER(prefix, name)

  // Non-static value set
#define INIT_VALUE_SET_MEMBER(name)				\
  ValueSetClass::insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_MEMBER: ")
#define INIT_VALUE_SET_MEMBERS_(...)		\
  FOR_EACH(INIT_VALUE_SET_MEMBER, __VA_ARGS__)
#define INIT_VALUE_SET_MEMBERS(name)					\
  INIT_VALUE_SET_MEMBERS_(MEMBERS_ ## name)

#define DECLARE_VALUE_SET_CORE(name)				\
  typedef _valueSetStaticMember<name> StaticMemberClass;	\
  DECLARE_VALUE_SET_MEMBER_RECORD;
#define DEFINE_VALUE_SET_CORE(mod, name)
  /*
#define DECLARE_VALUE_SET_CORE(name)			\
  name(const name& other);				\
  name(const arr &vec, const std::size_t offset);
#define DEFINE_VALUE_SET_CORE(name)				\
  name::name(const name& other) {				\
    initMembers();						\
    copyMembers(other);						\
  }								\
  name::name(const arr &vec, const std::size_t offset) {	\
    initMembers();						\
    fromArray(vec, offset);					\
  }
  */
#ifdef CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_INIT_MEMBERS(name)		\
  void initMembers() override {				\
    initMemberPointers();				\
  }
#else // CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_INIT_MEMBERS(name)	\
  void initMembers() override {}
#endif // CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_MEMBERS_(...)				\
  FOR_EACH(DECLARE_VALUE_SET_MEMBER, __VA_ARGS__)
#define DECLARE_VALUE_SET_MEMBERS(name)				\
  DECLARE_VALUE_SET_MEMBERS_(MEMBERS_ ## name);			\
  void initMemberPointers() override {				\
    if (!values.empty()) return;				\
    INIT_VALUE_SET_MEMBERS(name);				\
  }								\
  std::map<std::string, double*> getPointerMap() override {	\
    initMemberPointers();					\
    return get_pointer_map(values);				\
  }								\
  DECLARE_VALUE_SET_INIT_MEMBERS(name)				\
  DECLARE_VALUE_SET_CORE(name)
#define DECLARE_VALUE_SET(name, ...)		\
  DECLARE_VALUE_SET_BASE(name, __VA_ARGS__)	\
  using __VA_ARGS__::memberCount;		\
  DECLARE_VALUE_SET_MEMBERS(name)
#define DECLARE_VALUE_SET_COMPOSITE_ADD_CHILD(child)	\
    out += child::memberCount()
#define DECLARE_VALUE_SET_COMPOSITE_ADD_CHILDREN(...)	\
  FOR_EACH(DECLARE_VALUE_SET_COMPOSITE_ADD_CHILD, __VA_ARGS__, _EmptyMacroType)
#define DECLARE_VALUE_SET_COMPOSITE(name, children, ...)	\
  DECLARE_VALUE_SET_BASE(name, __VA_ARGS__)			\
  DECLARE_VALUE_SET_MEMBERS(name)				\
  static std::size_t memberCount() {				\
    std::size_t out = ParentClass::memberCount();		\
    DECLARE_VALUE_SET_COMPOSITE_ADD_CHILDREN children;		\
    return out;							\
  }
  
#define DEFINE_VALUE_SET_MEMBERS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_NS(mod, name)			\
  DEFINE_VALUE_SET_MEMBERS(mod, name)			\
  DEFINE_VALUE_SET_CORE(mod, name)
#define DEFINE_VALUE_SET(name)			\
  DEFINE_VALUE_SET_NS(, name)
#define DEFINE_VALUE_SET_HEADER(name)
#define DEFINE_VALUE_SET_SRC(name)  // Only used by conditions
  
  // Static value set
#define INIT_VALUE_SET_STATIC_MEMBER(name)				\
  ValueSetClass::insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_STATIC_MEMBER: ")
#define INIT_VALUE_SET_STATIC_MEMBERS_(...)		\
  FOR_EACH(INIT_VALUE_SET_STATIC_MEMBER, __VA_ARGS__)
#define INIT_VALUE_SET_STATIC_MEMBERS(name)		\
  INIT_VALUE_SET_STATIC_MEMBERS_(MEMBERS_ ## name)

#define DEFINE_VALUE_SET_STATIC_MODULE_CORE0(mod, name)	\
  double mod name::TIME = 0.;				\
  std::size_t mod name::N = 1;
#define DEFINE_VALUE_SET_STATIC_MODULE_CORE(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_(name, ...)	\
  FOR_EACH_WITH_ARGS(DEFINE_VALUE_SET_STATIC_MEMBER,	\
		     (name), __VA_ARGS__)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_NS(mod, name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_(name, MEMBERS_ ## name)	\
  DEFINE_VALUE_SET_STATIC_MODULE_CORE(mod, name)		\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS(name)			\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NS(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_(name, MEMBERS_ ## name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE(name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC(name)		\
  DEFINE_VALUE_SET_STATIC_NS(, name)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE(name)	\
  DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_NS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD_HEADER(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER(name)		\
  DEFINE_VALUE_SET_STATIC_HEADER_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE(name)	\
  DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(, name)
  
#ifdef CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_STATIC_INIT_MEMBERS(name)	\
  static void initMembers() {				\
    initMemberPointers();				\
  }
#else // CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_STATIC_INIT_MEMBERS(name)	\
  static void initMembers() {}
#endif // CHECK_VALUE_SET_ALTS
#define DECLARE_VALUE_SET_STATIC_MEMBERS_(...)			\
  FOR_EACH(DECLARE_VALUE_SET_STATIC_MEMBER, __VA_ARGS__)
#define DECLARE_VALUE_SET_STATIC_MEMBERS(name)			\
  public:							\
  static void initMemberPointers() {				\
    if (!values.empty())					\
      return;							\
    INIT_VALUE_SET_STATIC_MEMBERS(name);			\
  }								\
  static std::map<std::string, double*> getPointerMap() {	\
    initMemberPointers();					\
    return get_pointer_map(values);				\
  }								\
  DECLARE_VALUE_SET_STATIC_INIT_MEMBERS(name)			\
  static void initStaticMembers() {				\
    initMembers();						\
  }								\
  static void initMembersState(bool force=false) {		\
    if (!(force || values.empty()))				\
      return;							\
    initMembers();						\
    ValueSetClass::initMembersState();				\
  }								\
  static void initValues(const bool useC3) {			\
    initMembersState();						\
    ParentClass::initValues(useC3);				\
  }								\
  private:							\
  DECLARE_VALUE_SET_STATIC_MEMBERS_(MEMBERS_ ## name);		\
  public:
#define DECLARE_VALUE_SET_STATIC_CORE(name, ...)		\
  DECLARE_VALUE_SET_STATIC_BASE(name, __VA_ARGS__)		\
  typedef _valueSetStaticMember<name> StaticMemberClass;	\
  friend Value<name>;						\
  friend _valueSetStaticMember<name>;				\
  DECLARE_VALUE_SET_MEMBER_RECORD;
#define DECLARE_VALUE_SET_STATIC_MODULE_CORE0(name, ...)		\
  static double TIME;   /* The timestamp of the most recent call to _Rate */ \
  static std::size_t N; /* The current size of the module TimeSeries */
#define DECLARE_VALUE_SET_STATIC_MODULE_CORE(name, ...)
#define DECLARE_VALUE_SET_STATIC(name, ...)			\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)		\
  DECLARE_VALUE_SET_STATIC_MODULE_CORE(name, __VA_ARGS__)	\
  DECLARE_VALUE_SET_STATIC_MEMBERS(name)
#define DECLARE_VALUE_SET_STATIC_COMPOSITE(name, ...)	\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)	\
  DECLARE_VALUE_SET_STATIC_MEMBERS(name)

#define DECLARE_CONDITION_BASE(name)					\
  public:								\
  /** Copy constructor that makes a deep copy of the given object */	\
  /** @param other The object to copy */				\
  name ## Condition(const name ## Condition* const other);		\
  /** Constructor to create an object from the input vector, starting */ \
  /**   at the given offset.*/						\
  /** @param vec Vector to create the object from */			\
  /** @param offset The index in vec to start creating the object from */ \
  name ## Condition(const arr &vec, const std::size_t offset = 0);	\
  ~name ## Condition() override {					\
    _clear();								\
  }									\
  /** \copydoc ConditionBase::_print */					\
  std::ostream& _print(std::ostream &out, const uint tab = 0) const;	\
private:								\
 friend ConditionBase;							\
 friend class modules::name;						\
 /** \copydoc ConditionBase::_fromArray */				\
 void _fromArray(const arr &vec, const std::size_t offset = 0) override; \
 /** \copydoc ConditionBase::_toArray */				\
 arr _toArray() const override;						\
 /** \copydoc ConditionBase::_size */					\
 static std::size_t _size();						\
 /** \copydoc ConditionBase::_clear */					\
 void _clear() override;						\
 /** \copydoc ConditionBase::_reset */					\
 static void _reset();
  
#define DECLARE_CONDITION(name, parent)					\
  DECLARE_VALUE_SET(name ## Condition,					\
		    ConditionBase<name ## Condition, parent,		\
		    MODULE_ ## name>)					\
  DECLARE_CONDITION_BASE(name)						\
  name ## Condition(parent* par = nullptr) {				\
    setParent(par);							\
    initMembers();							\
  }									\
  /** The Maximum size of the serialized vector */			\
  static const std::size_t count;
  
#define INIT_CONDITION_CHILD(name)					\
  name ## _con(new name ## Condition(this))
#define INIT_CONDITION_CHILDREN_(...) // TODO
#define INIT_CONDITION_CHILDREN(...)					\
  INIT_CONDITION_CHILDREN_(__VA_ARGS__)
#define DECLARE_CONDITION_COMPOSITE(name, parent)			\
  DECLARE_VALUE_SET_COMPOSITE(name ## Condition, (CHILDREN_ ## name),	\
			      ConditionBase<name ## Condition, parent,	\
			      MODULE_ ## name>)				\
  DECLARE_CONDITION_BASE(name)						\
  name ## Condition(parent* par = nullptr) :				\
  INIT_CONDITION_CHILDREN(CHILDREN_ ## name) {				\
    setParent(par);							\
    initMembers();							\
  }
#define DEFINE_CONDITION_BASE(name)					\
  name ## Condition::name ## Condition(const arr &vec,			\
				       const std::size_t offset) {	\
    initMembers();							\
    fromArray(vec, offset);						\
  }
#define DEFINE_CONDITION(name)			\
  void name ## Condition::_clear() {}
  
#define CLEAR_CONDITION_CHILD(name)		\
  if (name ## _con != nullptr) {		\
    delete name ## _con;			\
    name ## _con = nullptr;			\
  }
#define CLEAR_CONDITION_CHILDREN_(...) // TODO
#define CLEAR_CONDITION_CHILDREN(...)		\
  CLEAR_CONDITION_CHILDREN_(__VA_ARGS__)
#define DEFINE_CONDITION_COMPOSITE(name)	\
  void name ## Condition::_clear() {		\
    CLEAR_CONDITION_CHILDREN(CHILDREN_ ## name)	\
  }

#define DEFINE_MODULE_COMPOSITE(name, children)	\
  void name::_reset() {				\
    RESET_CHILDREN children;			\
    conditions::name ## Condition::reset();	\
  }

#ifdef CHECK_VALUE_SET_ALTS
#define CALL_STATIC(method, ...)		\
  return method(alts, __VA_ARGS__)
#define CALL_STATIC_CONST(method, ...)		\
  CALL_STATIC(method, __VA_ARGS__)
#define CALL_STATIC_NOARGS(method)		\
  return method(alts)
#define CALL_STATIC_NOARGS_CONST(method)	\
  CALL_STATIC_NOARGS(method)
#define CALL_STATIC_METHOD(method)		\
  return alts.method()
#define CALL_STATIC_METHOD_CONST(method)	\
  CALL_STATIC_METHOD(method)
#else // CHECK_VALUE_SET_ALTS
#define CALL_STATIC(method, ...)		\
  initMemberPointers();				\
  return method(values, __VA_ARGS__)
#define CALL_STATIC_CONST(method, ...)		\
  return method(values, __VA_ARGS__)
#define CALL_STATIC_NOARGS(method)		\
  initMemberPointers();				\
  return method(values)
#define CALL_STATIC_NOARGS_CONST(method)	\
  return method(values)
#define CALL_STATIC_METHOD(method)		\
  initMemberPointers();				\
  return values.method()
#define CALL_STATIC_METHOD_CONST(method)	\
  return values.method()
#endif // CHECK_VALUE_SET_ALTS

  template<typename T>
  class _valueSetStaticMember {
  public:
    _valueSetStaticMember() {
      T::initStaticMembers();
    }
  };

#define VALUE_X_PTR 1
  template<typename T>
  class Value {
  public:
#ifdef VALUE_X_PTR
    Value(typename T::EnumType k0, double& v,
	  const std::string& context="") : x(&v), k(k0), ctx(context) {}
    Value(typename T::EnumType k0, double* v,
	  const std::string& context="") : x(v), k(k0), ctx(context) {}
    double* x = nullptr;
#else // VALUE_X_PTR
    Value(typename T::EnumType k0=T::EnumClass::NONE,
	  const std::string& context="") : x(0.0), k(k0), ctx(context) {}
    Value(typename T::EnumType k0, const double& v,
	  const std::string& context="") : x(v), k(k0), ctx(context) {
      init();
    }
    void init(const std::string& context="") {
      T::insertOrig(k, &x, context + "Value::init: ");
    }
    double x = 0.0;
#endif // VALUE_X_PTR
    typename T::EnumType k;
    std::string ctx;
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
    BINARY_OP(bool, ==)
    BINARY_OP(bool, !=)
    ARITH_OP2(+)
    ARITH_OP2(-)
    ARITH_OP2(*)
    ARITH_OP2(/)
    ARITH_OP2(%)
#undef CONV_OP
#undef ARITH_OP2
#undef BINARY_OP
#undef ASSIGN_OP
  };

  /**
     Template class to provide core methods for manipulating sets of
     values used by a model run that can be set via input files.

     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<MODULE ID, PARAM_TYPE PT>
  class ValueSetBase : public ValueSetEnum<ID, PT> {
  public:
    typedef ValueSetBase<ID, PT> BaseClass; /**< Specialized value set base class */
    typedef ValueSetEnum<ID, PT> EnumClass; /**< Enumerator class specifying keys for values in the set */
    typedef typename EnumClass::Type EnumType; /**< Enumerator type specifying keys for values in the set */
    typedef Value<BaseClass> ValueType; /**< Class for storing values */
#ifdef CHECK_VALUE_SET_ALTS    
    typedef typename std::map<EnumType, double>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
#else // CHECK_VALUE_SET_ALTS
    typedef typename std::map<EnumType, double*>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double*>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
#endif // CHECK_VALUE_SET_ALTS
    INHERIT_METHOD_ENUM(EnumClass);

    /**
       Determine if a key is included in arrays.
       \param k Key to check.
       \returns true if the key is included in arrays, false otherwise.
     */
    static bool inArrays(const EnumType k) {
      return (!(EnumClass::isNonvector(k) || EnumClass::isSkipped(k)));
    }

    // Inspection utilities
    /**
       Display the values in a value map.
       \param vals Value map to display.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_value_map(const std::map<EnumType, double*>& vals,
					 std::ostream &out,
					 const uint tab=0,
					 bool show_pointers=false) {
      const std::string space(tab * 4, ' ');
      for (typename std::map<EnumType, double*>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	out << space << getName(it->first) << " = ";
	if (show_pointers) {
	  out << it->second;
	} else {
	  out << *(it->second);
	}
	out << std::endl;
      }
      return out;
    }
    /**
       Display the values in a map.
       \param vals Value map to display.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_value_map(const std::map<EnumType, double>& vals,
					 std::ostream &out,
					 const uint tab = 0) {
      return EnumClass::print_map(vals, out, tab);
    }
    template <typename T>
    static std::string to_string_with_precision(const T a_value,
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
    static bool valuesEqual(const double* a, const double* b) {
      return valuesEqual(*a, *b);
    }
    static bool valuesEqual(const double& a, const double* b) {
      return valuesEqual(a, *b);
    }
    static bool valuesEqual(const double* a, const double& b) {
      return valuesEqual(*a, b);
    }
    static std::string valueString(const double& a) {
      return std::to_string(a);
    }
    static std::string valueString(const double* a) {
      if (!a)
	return "NULL";
      return std::to_string(*a);
    }
    static std::string valuePointerString(const double& a) {
      return "N/A";
    }
    static std::string valuePointerString(const double* a) {
      std::ostringstream oss;
      oss << a;
      return oss.str();
    }
    static void valueSet(double& dst, const double& src,
			 const std::string& context = "") {
      UNUSED(context);
      dst = src;
    }
    static void valueSet(double& dst, const double* src,
			 const std::string& context = "") {
      if (!src)
	throw std::runtime_error(error_prefix() + context +
				 "source pointer null");
      dst = src[0];
    }
    static void valueSet(double* dst, const double& src,
			 const std::string& context = "") {
      if (!dst)
	throw std::runtime_error(error_prefix() + context +
				 "destination pointer null");
      dst[0] = src;
    }
    static void valueSet(double* dst, const double* src,
			 const std::string& context = "") {
      if (!dst)
	throw std::runtime_error(error_prefix() + context +
				 "pointer null");
      if (!src)
	throw std::runtime_error(error_prefix() + context +
				 "source pointer null");
      dst[0] = src[0];
    }
    static void insertValue(std::map<EnumType, double*>& valsDst,
			    const EnumType& k, const double* src,
			    const std::string& context = "") {
      if (!src)
	throw std::runtime_error(error_prefix() + context +
				 "source pointer null");
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
	throw std::runtime_error(error_prefix() + context +
				 "source pointer null");
      valsDst[k] = src[0];
    }
    static void insertValue(std::map<EnumType, double*>&,
			    const EnumType& k, const double&,
			    const std::string& context = "") {
      throw std::runtime_error(error_prefix() + context +
			       "No value for \'" + getName(k) + "\'");
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
	throw std::runtime_error(error_prefix() + msg);
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
	throw std::runtime_error(error_prefix() + msg);
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
      for (typename std::vector<EnumType>::const_iterator s = EnumClass::skipped.begin();
	   s != EnumClass::skipped.end(); s++) {
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
	  throw std::runtime_error(error_prefix() + context +
				   getName(itSrc->first) +
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
	    throw std::runtime_error(error_prefix() + context +
				     getName(itDst->first) +
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
     */
    template<typename VSrc, typename VDst>
    static void copy_value_map(std::map<EnumType, VDst>& valsDst,
			       const std::map<EnumType, VSrc>& valsSrc,
			       const std::string& context="",
			       bool ignore_extras=false,
			       bool ignore_missing=false,
			       bool ignore_constants=false,
			       bool ignore_skipped=false) {
#ifdef CHECK_VALUE_SET_ALTS
      bool check = true;
#else // CHECK_VALUE_SET_ALTS
      bool check = false;
#endif // CHECK_VALUE_SET_ALTS
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
	  throw std::runtime_error(error_prefix() + context +
				   getName(itSrc->first) +
				   " is not present in the destination");
	}
	if (((!ignore_skipped) && isSkipped(itSrc->first))
	    || isInitonce(itSrc->first))
	  continue;
#ifdef CHECK_VALUE_SET_ALTS
	std::string srcval = valueString(itSrc->second);
	std::string dstval = valueString(itDst->second);
	std::string dstptr = valuePointerString(itDst->second);
#endif // CHECK_VALUE_SET_ALTS
	valueSet(itDst->second, itSrc->second);
#ifdef CHECK_VALUE_SET_ALTS
	DEBUG_VALUE_SET(context, itSrc->first, ": [", dstptr, "] ",
			dstval, " -> ", srcval, " [AFTER = ",
			valueString(itDst->second), "]");
#endif // CHECK_VALUE_SET_ALTS
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
     */
    template<typename V>
    static void copy_value_map(std::map<EnumType, V>& vals,
			       const std::string& filename,
			       const std::string& context="",
			       bool ignore_extras=false,
			       bool ignore_missing=false,
			       bool ignore_constants=false,
			       bool ignore_skipped=false) {
      std::map<EnumType, double> new_values = utils::readEnumFile<EnumType, double>(filename);
      copy_value_map(vals, new_values, context + "[" + filename + "]: ",
		     ignore_extras, ignore_missing,
		     ignore_constants, ignore_skipped);
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
    /**
       Re-initialize the values in a value map to the default values,
         minus any values that are skipped.
       \tparam V Value map type.
       \param vals Value map to initialize.
       \param useC3 If true, default values for a C3 will be used
     */
    template<typename V>
    static void init_value_map(std::map<EnumType, V>& vals,
			       const bool useC3) {
      if (useC3) {
	copy_value_map(vals, defaults_C3, "init_value_map[C3]: ",
		       true, false, true, true);
      } else {
	copy_value_map(vals, defaults, "init_value_map: ",
		       true, false, true, true);
      }
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
    template<typename T>
    static ValueType get_value(std::map<EnumType, T>& vals,
			       const EnumType k,
			       const std::string& context="[]: ") {
      check_value_map(vals, k, context);
      return ValueType(k, vals.find(k)->second,
		       context + "get_value[T]: "
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
#ifdef CHECK_VALUE_SET_ALTS
      DEBUG_VALUE_SET_NOEND(context, "[DOUBLE] ", k, "[",
			    std::to_string(k), "]: ");
      if (vals.find(k) == vals.end()) {
	DEBUG_VALUE_SET_RAW("NULL");
      } else {
	DEBUG_VALUE_SET_RAW(vals[k]);
      }
      DEBUG_VALUE_SET_RAW(" -> ", v);
#endif // CHECK_VALUE_SET_ALTS
      checkNotConstant(k, context);
      vals[k] = v;
      DEBUG_VALUE_SET_END(" [AFTER = ", vals[k], "]");
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
#ifdef CHECK_VALUE_SET_ALTS
      if (vals.find(k) == vals.end()) {
	DEBUG_VALUE_SET_RAW("[NULL] 0");
      } else {
	DEBUG_VALUE_SET_RAW("[", vals[k], "] ", *(vals[k]));
      }
      DEBUG_VALUE_SET_RAW(" -> ", v);
#endif // CHECK_VALUE_SET_ALTS
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
       Get the alternate value for the value associated with a
         key from a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Alternate value.
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
	  compareValues(k, defaults.find(k)->second, defaults_C3.find(k)->second);
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
       Check that the value for a specified key in a value map matches
         the value in an alternate map.
       \param vals Value map.
       \param alts Alternate value map.
       \param k Key to check.
       \param context String providing context for error messages.
     */
    static void check_value_alt(const std::map<EnumType, double*>& vals,
				const std::map<EnumType, double>& alts,
				const EnumType k,
				const std::string& context="") {
      typename std::map<EnumType, double>::const_iterator itp = alts.find(k);
      if (itp == alts.end()) {
	throw std::runtime_error(error_prefix() + context +
				 "check_value_alt: Key \'" +
				 getName(k) +
				 "\' is missing from alts");
      }
      typename std::map<EnumType, double*>::const_iterator itv = vals.find(k);
      if (itv == vals.end()) {
	throw std::runtime_error(error_prefix() + context +
				 "check_value_alt: Key \'" +
				 getName(k) +
				 "\' is missing from values");
      }
      compareValues(k, *(itv->second), itp->second,
		    context +
		    "Comparing \'" + getName(k) +
		    "\' between old and new tracking methods");
    }
    /**
       Check that the values in a value map match the values in an
         original value map.
       \param vals Value map.
       \param alts Alternate value map.
       \param context String providing context for error messages.
     */
    static void check_value_alts(const std::map<EnumType, double*>& vals,
				 const std::map<EnumType, double>& alts,
				 const std::string& context="") {
      for (typename std::map<EnumType, double*>::const_iterator itv = vals.begin();
	   itv != vals.end(); itv++) {
	check_value_alt(vals, alts, itv->first, context);
      }
      for (typename std::map<EnumType, double>::const_iterator itp = alts.begin();
	   itp != alts.end(); itp++) {
	typename std::map<EnumType, double*>::const_iterator itv = vals.find(itp->first);
	if (itv == vals.end()) {
	  check_value_alt(vals, alts, itp->first, context);
	}
      }
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
	if (!EnumClass::isConstant(it->first)) {
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
       \returns Values array.
     */
    template<typename V>
    static arr value_map_toArray(const std::map<EnumType, V>& vals) {
      arr out;
      size_t i = 0;
      out.resize(vals.size());
      for (typename std::map<EnumType, V>::const_iterator it = vals.begin();
	   it != vals.end(); it++) {
	if (inArrays(it->first)) {
	  checkNotConstant(it->first);
	  out[i] = get_value_const(vals, it->first,
				   "value_map_toArray: ");
	  i++;
	}
      }
      return out;
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
      size_t i = 0;
      size_t nexp = memberCount();
      size_t nact = vec.size() - offset;
      if (vals.size() != defaults.size())
	throw std::runtime_error(error_prefix() +
				 std::to_string(vals.size()) +
				 " provided, but " +
				 std::to_string(defaults.size()) +
				 " expected");
      if (nexp > nact)
	throw std::runtime_error(error_prefix() +
				 std::to_string(nexp) +
				 " expected, but only " +
				 std::to_string(nact) +
				 " provided (vec.size() = " +
				 std::to_string(vec.size()) +
				 ", offset = "+
				 std::to_string(offset) + ")");
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
       \return Member counts.
     */
    static std::size_t memberCount() {
      std::size_t out = defaults.size() - (skipped.size() + nonvector.size());
      return out;
    }
  };
  
  /**
     Template class to provide methods for manipulating a set of values
     used by a model run that can be set via input files.

     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<MODULE ID, PARAM_TYPE PT>
  class ValueSet : public ValueSetBase<ID, PT> {
  public:
    typedef ValueSet<ID, PT> ValueSetClass;
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<ID, PT>)

    /**
       Default constructor. Values will be initialized with default
         values, minus any values that are skipped.
     */
    ValueSet() :
#ifdef CHECK_VALUE_SET_ALTS
      alts(),
#endif // CHECK_VALUE_SET_ALTS
      values() {}
    /**
       Copy constructor. Values will be updated from the values in the
         copied set.
       \param other Value set to copy values from.
     */
    ValueSet(const ValueSet& other) : ValueSet() {}
    /**
       Assignment operator. Values will be updated from the values in the
         copied set.
       \param other Value set to copy values from.
       \returns Updated value set.
     */
    ValueSet& operator=(const ValueSet &other) {
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
       Copy values from another value set to this one.
       \param other Value set to copy values from.
     */
    virtual void copyMembers(const ValueSet& other) {
      initMembersState();
#ifdef CHECK_VALUE_SET_ALTS
      copy_value_map(alts, other.alts, "copyMembers[alts]: ", true);
#else // CHECK_VALUE_SET_ALTS
      // copy_value_map(values, other.values, "copyMembers[vals]: ", true);
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Initialize the members in the instance. This should be overriden
         by child classes with a method that adds pointers to values.
     */
    virtual void initMembers() {}
    /**
       Initialize the static values in the instance.
     */
    static void initStaticMembers() {}
    /**
       Initialize the members in the instance for the current state.
       \param force If true, the state will be updated reguardless of
         if the class has be updated since the last time the state was
	 updated.
     */
    virtual void initMembersState(bool force=false) {
      if (!(force || values.empty()))
	return;
      initMembers();
#ifdef CHECK_VALUE_SET_ALTS
      alts.insert(defaults.begin(), defaults.end());
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Re-initialize the values to the default values, minus any values
         that are skipped.
       \param useC3 If true, default values for a C3 will be used
     */
    virtual void initValues(const bool useC3) {
      initMembersState(true);
#ifdef CHECK_VALUE_SET_ALTS
      init_value_map(alts, useC3);
#endif // CHECK_VALUE_SET_ALTS
    }

    /**
       Copy values from member pointers into alternate value map.
     */
    virtual void updateAlts() {
#ifdef CHECK_VALUE_SET_ALTS
      copy_value_map(alts, values, "updateAlts: ",
		     false, false, false, false);
#endif // CHECK_VALUE_SET_ALTS
    }

    /**
       Initialize the map containing pointers to instance members.
     */
    virtual void initMemberPointers() {}
      
    /**
       Get the map containing pointers to the instance members.
       \return Map between member names and pointers to instance
         members.
     */
    virtual std::map<std::string, double*> getPointerMap() {
      std::map<std::string, double*> out;
      return out;
    }

    /** Get the iterator pointing to the start of the value set */
    iterator begin() { CALL_STATIC_METHOD(begin); }
    /** Get the iterator pointing to the end of the value set */
    iterator end() { CALL_STATIC_METHOD(end); }
    /** Get the constant iterator pointing to the start of the value set */
    const_iterator begin() const { CALL_STATIC_METHOD_CONST(begin); }
    /** Get the constant iterator pointing to the end of the value set */
    const_iterator end() const { CALL_STATIC_METHOD_CONST(end); }
    // Inspection utilities
    /**
       Display the values in the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    virtual std::ostream& print(std::ostream &out, const uint tab = 0) const {
      CALL_STATIC_CONST(print_value_map, out, tab);
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    void check(const EnumType k, const std::string& context = "") const {
      CALL_STATIC_CONST(check_value_map, k, context);
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
      CALL_STATIC(update_value_map, new_values,
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
      CALL_STATIC(update_value_map, filename,
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
      CALL_STATIC(get_value, k);
    }
    /**
       Get the constant reference for the value associated with a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    const double& operator[](const EnumType k) const {
      CALL_STATIC_CONST(get_value_const, k);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    virtual void set(const EnumType k, const double v) {
      CALL_STATIC(set_value, k, v);
    }
    /**
       Update the value associated with a key in the value set from a map
         containing enzyme activity values.
       \param k Key to update value for.
       \param EnzymeAct Enzyme activities.
     */
    virtual void setFromEnzymeAct(const EnumType k,
				  const std::map<std::string, double>& EnzymeAct) {
      CALL_STATIC(set_value_from_EnzymeAct, k, EnzymeAct);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value
     */
    virtual ValueType get(const EnumType k) {
      CALL_STATIC(get_value, k);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value
     */
    virtual double get(const EnumType k) const {
      CALL_STATIC_CONST(get_value_const, k);
    }
    /**
       Add a value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    virtual void insertOrig(const EnumType k, double* v,
			    const std::string& context="") {
      if (k == EnumClass::NONE || k == EnumClass::MAX)
	return;
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end())
	alts[k] = *v;
#endif // CHECK_VALUE_SET_ALTS
      insert_value_orig(values, k, v, context);
    }
    /**
       Check if the value using the original code matches the value
         calculated using only dynamic value methods.
       \param k Key to check.
       \param context String providing context for error messages.
     */
    virtual void checkAlt(const EnumType k,
			  const std::string& context="") const {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alt(values, alts, k, context);
#else // CHECK_VALUE_SET_ALTS
      UNUSED(k);
      throw std::runtime_error(error_prefix() + context + "Alternates not enabled");
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Check if the values using the original code match the values
         calculated using only dynamic value methods.
       \param context String providing context for error messages.
     */
    virtual void checkAlts(const std::string& context="") const {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alts(values, alts, context);
#else // CHECK_VALUE_SET_ALTS
      throw std::runtime_error(error_prefix() + context + "Alternates not enabled");
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Reset the static class members.
     */
    static void reset() {
      clearSkipped();
    }
    /**
       Reset all of the values to 0;
     */
    virtual void resetValues() {
      CALL_STATIC_NOARGS(reset_value_map);
    }
    /**
       Get the number of values in the set that would be included in an
         array produced by toArray.
       \returns Value count.
     */
    virtual std::size_t sizeArray() const {
      CALL_STATIC_METHOD_CONST(size);
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    virtual arr toArray() const {
      CALL_STATIC_NOARGS_CONST(value_map_toArray);
    }
    /**
       Update the values in the set from an array.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    virtual void fromArray(const arr &vec, const std::size_t offset = 0) {
      CALL_STATIC(value_map_fromArray, vec, offset);
    }
#ifdef CHECK_VALUE_SET_ALTS
    std::map<EnumType, double> alts; /**< Alternate copy of values in the set calculated using only the updated dynamic values */
#endif // CHECK_VALUE_SET_ALTS
    std::map<EnumType, double*> values; /**< Values in the set. */
  };
  
  /**
     Template class to provide methods for manipulating a set of static
     class values used by a model run that can be set via input files.

     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<MODULE ID, PARAM_TYPE PT>
  class ValueSetStatic : public ValueSetBase<ID, PT> {
  public:
    typedef ValueSetStatic<ID, PT> ValueSetClass;
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<ID, PT>)
    
    /**
       Initialize the values in the instance. This should be overriden
         by child classes with a method that adds pointers to values.
     */
    static void initMembers() {}
    /**
       Initialize the static values in the instance.
     */
    static void initStaticMembers() {}
    /**
       Initialize the members in the instance for the current state.
       \param force If true, the state will be updated reguardless of
         if the class has be updated since the last time the state was
	 updated.
     */
    static void initMembersState(bool force=false) {
      if (!(force || values.empty()))
	return;
      initMembers();
#ifdef CHECK_VALUE_SET_ALTS
      alts.insert(defaults.begin(), defaults.end());
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Re-initialize the values to the default values, minus any values
         that are skipped.
       \param useC3 If true, default values for a C3 will be used
     */
    static void initValues(const bool useC3) {
      initMembersState();
#ifdef CHECK_VALUE_SET_ALTS
      init_value_map(alts, useC3);
#endif // CHECK_VALUE_SET_ALTS
    }

    /**
       Copy values from member pointers into alternate value map.
     */
    static void updateAlts() {
#ifdef CHECK_VALUE_SET_ALTS
      copy_value_map(alts, values, "updateAlts: ",
		     false, false, false, false);
#endif // CHECK_VALUE_SET_ALTS
    }
    
    /**
       Initialize the map containing pointers to instance members.
     */
    static void initMemberPointers() {}
      
    /**
       Get the map containing pointers to the static class members.
       \return Map between member names and pointers to static class
         members.
     */
    static std::map<std::string, double*> getPointerMap() {
      std::map<std::string, double*> out;
      return out;
    }

    /** Get the iterator pointing to the start of the value set */
    static iterator begin() { CALL_STATIC_METHOD(begin); }
    /** Get the iterator pointing to the end of the value set */
    static iterator end() { CALL_STATIC_METHOD(end); }
    /** Get the constant iterator pointing to the start of the value set */
    // Inspection utilities
    /**
       Display the values in the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print(std::ostream &out, const uint tab = 0) {
      CALL_STATIC(print_value_map, out, tab);
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    static void check(const EnumType k, const std::string& context = "") {
      CALL_STATIC(check_value_map, k, context);
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
      CALL_STATIC(update_value_map, new_values,
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
      CALL_STATIC(update_value_map, filename,
		  context + "update_values[" + filename + "]: ",
		  allow_extras, no_missing,
		  no_constants, no_skipped);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set(const EnumType k, const double v) {
      CALL_STATIC(set_value, k, v);
    }
    /**
       Update the value associated with a key in the value set from a map
         containing enzyme activity values.
       \param k Key to update value for.
       \param EnzymeAct Enzyme activities.
     */
    static void setFromEnzymeAct(const EnumType k,
				 const std::map<std::string, double>& EnzymeAct) {
      CALL_STATIC(set_value_from_EnzymeAct, k, EnzymeAct);
    }
    /**
       Get the value associated with a key.
       \param k Key to get value for.
       \returns Value.
     */
    static double get(const EnumType k) {
      CALL_STATIC(get_value_const, k);
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
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end()) {
	alts[k] = *v;
      }
#endif // CHECK_VALUE_SET_ALTS
      insert_value_orig(values, k, v, context);
    }
    /**
       Check if the value using the original code matches the value
         calculated using only dynamic value methods.
       \param k Key to check.
       \param context String providing context for error messages.
     */
    static void checkAlt(const EnumType k,
			 const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alt(values, alts, k, context);
#else // CHECK_VALUE_SET_ALTS
      UNUSED(k);
      throw std::runtime_error(error_prefix() + context + "Alternates not enabled");
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Check if the values using the original code match the values
         calculated using only dynamic value methods.
       \param context String providing context for error messages.
     */
    static void checkAlts(const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alts(values, alts, context);
#else // CHECK_VALUE_SET_ALTS
      throw std::runtime_error(error_prefix() + context + "Alternates not enabled");
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Reset the static class members to 0.
     */
    static void reset() {
      clearSkipped();
      CALL_STATIC_NOARGS(reset_value_map);
    }
    /**
       Get the number of values in the set that would be included in an
         array produced by toArray.
       \returns Value count.
     */
    virtual std::size_t sizeArray() const {
      CALL_STATIC_METHOD(size);
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    static arr toArray() {
      CALL_STATIC_NOARGS(value_map_toArray);
    }
    /**
       Update the values in the set from an array.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    static void fromArray(const arr &vec, const std::size_t offset = 0) {
      CALL_STATIC(fromArray, vec, offset);
    }
#ifdef CHECK_VALUE_SET_ALTS
    static std::map<EnumType, double> alts; /**< Alternate copy of values in the set calculated using only the updated dynamic values */
#endif // CHECK_VALUE_SET_ALTS
    static std::map<EnumType, double*> values; /**< Values in the set. */
  };

#ifdef CHECK_VALUE_SET_ALTS
  template<MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<ID, PT>::EnumType, double>
  ValueSetStatic<ID, PT>::alts = {};
#endif // CHECK_VALUE_SET_ALTS

  template<MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<ID, PT>::EnumType, double*>
  ValueSetStatic<ID, PT>::values = {};
  
}
    
#undef CALL_STATIC
