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

#define DECLARE_VALUE_SET_MEMBER_RECORD			\
  static const StaticMemberClass _adjustments
#define TYPED_VALUE_SET_MEMBERS(name, ...)		\
  FOR_EACH_WITH_ARGS_COMMA(JOIN, (name::EnumClass::), __VA_ARGS__)
#define DEFINE_VALUE_SET_MEMBER_RECORD(prefix, name)			\
  const typename prefix name::StaticMemberClass prefix name::_adjustments = \
    prefix name::StaticMemberClass({TYPED_VALUE_SET_MEMBERS(prefix name, MEMBERS_ ## name)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _CONSTANT)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _SKIPPED)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _NOT_IN_ARRAY)})
#define DEFINE_VALUE_SET_MEMBER_RECORD_SINGLE(prefix, name)		\
  const typename prefix name::StaticMemberClass prefix name::_adjustments = \
    prefix name::StaticMemberClass({prefix name::EnumClass::MEMBERS_ ## name}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _CONSTANT)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _SKIPPED)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _NOT_IN_ARRAY)})
#define DEFINE_VALUE_SET_MEMBER_RECORD_NONE(prefix, name)		\
  const typename prefix name::StaticMemberClass prefix name::_adjustments = \
    prefix name::StaticMemberClass({},					\
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _CONSTANT)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _SKIPPED)}, \
				   {TYPED_VALUE_SET_MEMBERS(prefix name, NONE, MEMBERS_ ## name ## _NOT_IN_ARRAY)})

  // Non-static value set
#define INIT_VALUE_SET_MEMBER(name)				\
  name = 0.0;							\
  insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_MEMBER: ")
#define INIT_VALUE_SET_MEMBERS_(...)		\
  FOR_EACH(INIT_VALUE_SET_MEMBER, __VA_ARGS__)
#define INIT_VALUE_SET_MEMBERS(name)					\
  INIT_VALUE_SET_MEMBERS_(MEMBERS_ ## name)
#define INIT_VALUE_SET_MEMBERS_SINGLE(name)	\
  INIT_VALUE_SET_MEMBER(MEMBERS_ ## name)
#define INIT_VALUE_SET_MEMBERS_NONE(name)

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
#define DECLARE_VALUE_SET_MEMBERS_(...)				\
  FOR_EACH(DECLARE_VALUE_SET_MEMBER, __VA_ARGS__)
#define DECLARE_VALUE_SET_MEMBERS(name)			\
  DECLARE_VALUE_SET_MEMBERS_(MEMBERS_ ## name);		\
  void initMembers() override {				\
    INIT_VALUE_SET_MEMBERS(name);			\
    ParentClass::initMembers();				\
  }							\
  DECLARE_VALUE_SET_CORE(name)
#define DECLARE_VALUE_SET_MEMBERS_SINGLE(name)		\
  DECLARE_VALUE_SET_MEMBER(MEMBERS_ ## name);		\
  void initMembers() override {				\
    INIT_VALUE_SET_MEMBERS_SINGLE(name);		\
    ParentClass::initMembers();				\
  }							\
  DECLARE_VALUE_SET_CORE(name)
#define DECLARE_VALUE_SET_MEMBERS_NONE(name)		\
  void initMembers() override {				\
    INIT_VALUE_SET_MEMBERS_NONE(name)			\
    ParentClass::initMembers();				\
  }							\
  DECLARE_VALUE_SET_CORE(name)
#define DECLARE_VALUE_SET(name, ...)		\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)	\
  DECLARE_VALUE_SET_MEMBERS(name)
#define DECLARE_VALUE_SET_SINGLE(name, ...)	\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)	\
  DECLARE_VALUE_SET_MEMBERS_SINGLE(name)
#define DECLARE_VALUE_SET_NONE(name, ...)	\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)	\
  DECLARE_VALUE_SET_MEMBERS_NONE(name)
  
#define DEFINE_VALUE_SET_MEMBERS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_MEMBERS_SINGLE(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD_SINGLE(mod, name)
#define DEFINE_VALUE_SET_MEMBERS_NONE(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD_NONE(mod, name)
#define DEFINE_VALUE_SET_NS(mod, name)			\
  DEFINE_VALUE_SET_MEMBERS(mod, name)			\
  DEFINE_VALUE_SET_CORE(mod, name)
#define DEFINE_VALUE_SET(name)			\
  DEFINE_VALUE_SET_NS(, name)
#define DEFINE_VALUE_SET_SINGLE_NS(mod, name)		\
  DEFINE_VALUE_SET_MEMBERS_SINGLE(mod, name)		\
  DEFINE_VALUE_SET_CORE(mod, name)
#define DEFINE_VALUE_SET_SINGLE(name)		\
  DEFINE_VALUE_SET_SINGLE_NS(, name)
#define DEFINE_VALUE_SET_NONE_NS(mod, name)		\
  DEFINE_VALUE_SET_MEMBERS_NONE(mod, name)		\
  DEFINE_VALUE_SET_CORE(mod, name)
#define DEFINE_VALUE_SET_NONE(name)		\
  DEFINE_VALUE_SET_NONE_NS(, name)
#define DEFINE_VALUE_SET_HEADER(name)
#define DEFINE_VALUE_SET_HEADER_SINGLE(name)
#define DEFINE_VALUE_SET_HEADER_NONE(name)
#define DEFINE_VALUE_SET_SRC(name)  // Only used by conditions
  
  // Static value set
#define INIT_VALUE_SET_STATIC_MEMBER(name)				\
  name = 0.0;								\
  insertOrig(EnumClass::name, &name, "INIT_VALUE_SET_STATIC_MEMBER: ")
#define INIT_VALUE_SET_STATIC_MEMBERS_(...)		\
  FOR_EACH(INIT_VALUE_SET_STATIC_MEMBER, __VA_ARGS__)
#define INIT_VALUE_SET_STATIC_MEMBERS(name)		\
  INIT_VALUE_SET_STATIC_MEMBERS_(MEMBERS_ ## name)
#define INIT_VALUE_SET_STATIC_MEMBERS_SINGLE(name)	\
  INIT_VALUE_SET_STATIC_MEMBER(MEMBERS_ ## name)
#define INIT_VALUE_SET_STATIC_MEMBERS_NONE(name)

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
#define DEFINE_VALUE_SET_STATIC_MEMBERS_SINGLE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBER(name, MEMBERS_ ## name);	\
  DEFINE_VALUE_SET_STATIC_MODULE_CORE(mod, name)		\
  DEFINE_VALUE_SET_MEMBER_RECORD_SINGLE(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_SINGLE(name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_SINGLE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_NONE_NS(mod, name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD_NONE(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_NONE(name)		\
  DEFINE_VALUE_SET_STATIC_MODULE_CORE(mod, name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NONE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_(name, MEMBERS_ ## name)	\
  DEFINE_VALUE_SET_MEMBER_RECORD(mod, name)
#define DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE(name)		\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC(name)		\
  DEFINE_VALUE_SET_STATIC_NS(, name)
#define DEFINE_VALUE_SET_STATIC_SINGLE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_SINGLE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_SINGLE(name)	\
  DEFINE_VALUE_SET_STATIC_SINGLE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_NONE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_NONE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_NONE(name)	\
  DEFINE_VALUE_SET_STATIC_NONE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(mod, name)	\
  DEFINE_VALUE_SET_STATIC_MEMBERS_COMPOSITE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_COMPOSITE(name)	\
  DEFINE_VALUE_SET_STATIC_COMPOSITE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER(name)		\
  DEFINE_VALUE_SET_STATIC_HEADER_NS(, )
#define DEFINE_VALUE_SET_STATIC_HEADER_SINGLE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_SINGLE(name)	\
  DEFINE_VALUE_SET_STATIC_HEADER_SINGLE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_NONE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_NONE(name)	\
  DEFINE_VALUE_SET_STATIC_HEADER_NONE_NS(, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(mod, name)
#define DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE(name)	\
  DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE_NS(, name)
  
#define DECLARE_VALUE_SET_STATIC_MEMBERS_(...)			\
  FOR_EACH(DECLARE_VALUE_SET_STATIC_MEMBER, __VA_ARGS__)
#define DECLARE_VALUE_SET_STATIC_MEMBERS(name)		\
  public:						\
  static void initMembers() {				\
    INIT_VALUE_SET_STATIC_MEMBERS(name);		\
    ParentClass::initMembers();				\
  }							\
  private:						\
  DECLARE_VALUE_SET_STATIC_MEMBERS_(MEMBERS_ ## name);	\
  public:
#define DECLARE_VALUE_SET_STATIC_MEMBERS_SINGLE(name)	\
  public:						\
  static void initMembers() {				\
    INIT_VALUE_SET_STATIC_MEMBERS_SINGLE(name);		\
    ParentClass::initMembers();				\
  }							\
  private:						\
  DECLARE_VALUE_SET_STATIC_MEMBER(MEMBERS_ ## name);	\
  public:
#define DECLARE_VALUE_SET_STATIC_MEMBERS_NONE(name)	\
  public:						\
  static void initMembers() {				\
    INIT_VALUE_SET_STATIC_MEMBERS_NONE(name)		\
    ParentClass::initMembers();				\
  }
#define DECLARE_VALUE_SET_STATIC_CORE(name, ...)		\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)			\
  typedef _valueSetStaticMember<name> StaticMemberClass;	\
  friend Value<name>;						\
  friend _valueSetStaticMember<name>;				\
  DECLARE_VALUE_SET_MEMBER_RECORD;
#define DECLARE_VALUE_SET_STATIC(name, ...)		\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)	\
  DECLARE_VALUE_SET_STATIC_MEMBERS(name)
#define DECLARE_VALUE_SET_STATIC_SINGLE(name, ...)	\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)	\
  DECLARE_VALUE_SET_STATIC_MEMBERS_SINGLE(name)
#define DECLARE_VALUE_SET_STATIC_NONE(name, ...)	\
  DECLARE_VALUE_SET_STATIC_CORE(name, __VA_ARGS__)	\
  DECLARE_VALUE_SET_STATIC_MEMBERS_NONE(name)

#ifdef CHECK_VALUE_SET_ALTS
#define CALL_STATIC(method, ...)		\
  method(alts, __VA_ARGS__)
#define CALL_STATIC_NOARGS(method)		\
  method(alts)
#define CALL_STATIC_METHOD(method)		\
  alts.method()
#define CALL_STATIC_WITH_ALL(method, ...)	\
  method(alts, __VA_ARGS__)
#else // CHECK_VALUE_SET_ALTS
#define CALL_STATIC(method, ...)		\
  method(values, __VA_ARGS__)
#define CALL_STATIC_NOARGS(method)		\
  method(values)
#define CALL_STATIC_METHOD(method)		\
  values.method()
#define CALL_STATIC_WITH_ALL(method, ...)	\
  method(values, values_all, __VA_ARGS__)
#endif // CHECK_VALUE_SET_ALTS
  
#ifdef CHECK_VALUE_SET_ALTS
#define SET_VALUE(module, pt, var, k, v)	\
  var.k = v;					\
  var[pt::module::k] = v
#define MULT_VALUE(module, pt, var, k, v)	\
  var.k = var.k * v;				\
  var[pt::module::k] = var[pt::module::k] * v
#define DIVD_VALUE(module, pt, var, k, v)	\
  var.k = var.k / v;				\
  var[pt::module::k] = var[pt::module::k] / v
#define SET_VALUE_PTR(module, pt, var, k, v)	\
  var->k = v;					\
  (*var)[pt::module::k] = v
#define MULT_VALUE_PTR(module, pt, var, k, v)		\
  var->k = var-k * v;					\
  (*var)[pt::module::k] = (*var)[pt::module::k] * v
#define DIVD_VALUE_PTR(module, pt, var, k, v)		\
  var->k = var->k / v;					\
  (*var)[pt::module::k] = (*var)[pt::module::k] / v
#define SET_VALUE_STATIC(cls, k, v)		\
  cls::k = v;					\
  cls::set(cls::EnumClass::k, v)
#else // CHECK_VALUE_SET_ALTS
#define SET_VALUE(module, pt, var, k, v)	\
  var[pt::module::k] = v
#define MULT_VALUE(module, pt, var, k, v)	\
  var[pt::module::k] = var[pt::module::k] * v
#define DIVD_VALUE(module, pt, var, k, v)	\
  var[pt::module::k] = var[pt::module::k] / v
#define SET_VALUE_PTR(module, pt, var, k, v)	\
  (*var)[pt::module::k] = v
#define MULT_VALUE_PTR(module, pt, var, k, v)	\
  (*var)[pt::module::k] = (*var)[pt::module::k] * v
#define DIVD_VALUE_PTR(module, pt, var, k, v)		\
  (*var)[pt::module::k] = (*var)[pt::module::k] / v
#endif // CHECK_VALUE_SET_ALTS
#define SET_VALUE_VARS(module, pt, k, v)			\
  SET_VALUE(module, pt, theVars->module ## _ ## pt, k, v)
#define MULT_VALUE_VARS(module, pt, k, v)			\
  MULT_VALUE(module, pt, theVars->module ## _ ## pt, k, v)

  template<typename T>
  class _valueSetStaticMember {
  public:
    _valueSetStaticMember(typename T::EnumType k, double* val,
			  const std::string& context="_valueSetStaticMember: ") {
      T::insertOrig(k, val, context);
    }
    _valueSetStaticMember(typename T::EnumType k, const double* val,
			  const std::string& context="_valueSetStaticMember: ") {
      T::insertConstOrig(k, val, context);
    }
    _valueSetStaticMember(const std::vector<typename T::EnumType> order,
			  const std::vector<typename T::EnumType> constant,
			  const std::vector<typename T::EnumType> skipped,
			  const std::vector<typename T::EnumType> not_in_array) :
      order_(order.begin(), order.end()),
      constant_(constant.begin(), constant.end()),
      skipped_(skipped.begin(), skipped.end()),
      not_in_array_(not_in_array.begin(), not_in_array.end()) {
      T::initStaticMembers();
      typename std::vector<typename T::EnumType>::const_iterator it;
#define INSERT(a, b)					\
      a.clear();					\
      for (it = b.begin(); it != b.end(); it++) {	\
	if (((*it) == T::EnumClass::NONE) ||		\
	    ((*it) == T::EnumClass::MAX)) {		\
	  continue;					\
	}						\
	a.push_back(*it);				\
      }
      INSERT(T::constant_values(), constant);
      INSERT(T::skipped_values(), skipped);
      INSERT(T::non_array_values(), not_in_array);
#undef INSERT
      // T::constant_values().clear();
      // T::constant_values().insert(T::constant_values().begin(),
      // 				  constant.begin(), constant.end());
      // T::skipped_values().clear();
      // T::skipped_values().insert(T::skipped_values().begin(),
      // 				 skipped.begin(), skipped.end());
      // T::non_array_values().clear();
      // T::non_array_values().insert(T::non_array_values().begin(),
      // 				   not_in_array.begin(),
      // 				   not_in_array.end());
    }
    std::vector<typename T::EnumType> order_;
    std::vector<typename T::EnumType> constant_;
    std::vector<typename T::EnumType> skipped_;
    std::vector<typename T::EnumType> not_in_array_;
  };

  template<typename T>
  class Value {
  public:
    Value(typename T::EnumType k0=T::EnumClass::NONE) : k(k0), x(0.0) {}
    Value(typename T::EnumType k0, const double& v) : k(k0), x(v) {
      init();
    }
    typename T::EnumType k;
    double x = 0.0;
    void init(const std::string& context="") {
      T::insertOrig(k, &x, context + "Value::init: ");
    }
#define CONV_OP(op)				\
    operator op() const {			\
      return op(x);				\
    }
#define ASSIGN_OP(op)				\
    template<typename T2>			\
    Value& operator op(const Value<T2>& b) {	\
      x op b.x;					\
      return *this;				\
    }						\
    template<typename T2>			\
    Value& operator op(const T2& b) {		\
      x op b;					\
      return *this;				\
    }
#define BINARY_OP(ret, op)					\
    template<typename T2>					\
    friend ret operator op(Value<T> a, const Value<T2>& b) {	\
      return (a.x op b.x);					\
    }								\
    template<typename T2>					\
    friend ret operator op(Value<T> a, const T2& b) {		\
      return (a.x op b);					\
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
  class ValueSetBase {
  public:
    typedef ValueSetBase<ID, PT> BaseClass; /**< Specialized value set base class */
    typedef ValueSetEnum<ID, PT> EnumClass; /**< Enumerator class specifying keys for values in the set */
    typedef typename EnumClass::Type EnumType; /**< Enumerator type specifying keys for values in the set */
    typedef Value<BaseClass> ValueType; /**< Class for storing values */
#ifdef CHECK_VALUE_SET_ALTS    
    typedef typename std::map<EnumType, double>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
#else // CHECK_VALUE_SET_ALTS
    typedef typename std::map<EnumType, double&>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double&>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
#endif // CHECK_VALUE_SET_ALTS
    static const MODULE module; /**< Enum corresponding to the module that the values will be used by */
    static const PARAM_TYPE param_type; /**< Enum corresponding to the module component that the values will be used by */

    /**
       Get an error prefix for the class.
       \returns Error prefix.
     */
    static std::string error_prefix() {
      std::string out = utils::enum_key2string(param_type) + "[" +
	utils::enum_key2string(module) + "]: ";
      return out;
    }
    /**
       Get the default value for a key in the set.
       \returns Default value.
     */
    static const double& default_value(const EnumType k) {
      const std::map<EnumType, double>& defaults = default_values();
      typename std::map<EnumType, double>::const_iterator it = defaults.find(k);
      if (it == defaults.end())
	throw std::runtime_error(error_prefix() +
				 "Could not locate default for \'"
				 + utils::enum_key2string(k)
				 + "\'");
      return it->second;
    }
    /**
       Get the default value for a key in the set for C3.
       \returns Default value.
     */
    static const double& default_value_C3(const EnumType k) {
      const std::map<EnumType, double>& defaults = default_values_C3();
      typename std::map<EnumType, double>::const_iterator it = defaults.find(k);
      if (it == defaults.end())
	throw std::runtime_error(error_prefix() +
				 "Could not locate C3 default for \'"
				 + utils::enum_key2string(k)
				 + "\'");
      return it->second;
    }
    /**
       Get the full set of default values associated with the set.
       \returns Mapping between set keys and default values.
     */
    static const std::map<EnumType, double>& default_values() {
      return get_enum_defaults<EnumType>();
    }
    /**
       Get the full set of default values associated with the set for C3.
       \returns Mapping between set keys and default values.
     */
    static const std::map<EnumType, double>& default_values_C3() {
      return get_enum_defaults_C3<EnumType>();
    }
    // Constant value manipulation
    /**
       Get the keys for values in the set that are constant.
       \return Constant value keys.
     */
    static std::vector<EnumType>& constant_values() {
      return constant_enum_param<EnumType>();
    }
    /**
       Add a key to the set of keys that are constant.
       \param k Key to make constant.
     */
    static void add_constant(const EnumType k) {
      if (k == EnumClass::NONE)
	return;
      add_constant_enum_param(k);
    }
    /**
       Add a set of keys to the constant set of keys.
       \param vals Vector of keys to add.
     */
    static void add_constants(const std::vector<EnumType>& vals) {
      for (typename std::vector<EnumType>::const_iterator it = vals.begin();
	   it != vals.end(); it++)
	add_constant(*it);
    }
    // static const std::vector<EnumType> constant_values; /**< Values in the set that are constant */
    /**
       Determine if a key is constant.
       \param k Key to check.
       \returns true if the key is constant, false otherwise.
     */
    static bool isConstant(const EnumType k) {
      for (typename std::vector<EnumType>::iterator it = constant_values().begin();
	   it != constant_values().end(); it++) {
        if (*it == k)
          return true;
      }
      return false;
    }
    /**
       Throw an error if a key is constant.
       \param k Key to check.
       \context String to prefix error message with.
     */
    static void checkConstant(const EnumType k, const std::string& context = "") {
      if (isConstant(k)) {
	std::string msg = "\'" + utils::enum_key2string(k) + "\' constant";
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(error_prefix() + msg);
      }
    }
    // Skipped value manipulation
    /**
       Get the keys for values in the set that are currently skipped
         (unused).
       \return Skipped value keys.
     */
    static std::vector<EnumType>& skipped_values() {
      return skipped_enum_param<EnumType>();
    }
    /**
       Add a key to the set of keys skipped (unused) by the set. The key
         will continue to be skipped until reset or unskip is called.
       \param k Key to skip.
     */
    static void skip(const EnumType k) {
      if (k == EnumClass::NONE)
	return;
      skip_enum_param(k);
    }
    /**
       Remove a key from the set of keys skipped (unused) by the set.
       \param k Key to no-longer.
     */
    static void unskip(const EnumType k) {
      if (k == EnumClass::NONE)
	return;
      skip_enum_param(k, true);
    }
    /**
       Determine if a key is currently skipped (unused) by the set.
       \param k Key to check.
       \returns true if the key is skipped, false otherwise.
     */
    static bool isSkipped(const EnumType k) {
      for (typename std::vector<EnumType>::iterator it = skipped_values().begin();
	   it != skipped_values().end(); it++) {
        if (*it == k)
          return true;
      }
      return false;
    }
    /**
       Remove all keys from the list of keys skipped by the set.
     */
    static void reset_skipped() {
      skipped_enum_param<EnumType>(true);
    }
    /**
       Throw an error if a key is currently being skipped by the set.
       \param k Key to check.
       \context String to prefix error message with.
     */
    static void checkSkipped(const EnumType k, const std::string& context = "") {
      if (isSkipped(k)) {
	std::string msg = "\'" + utils::enum_key2string(k) + "\' skipped";
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(error_prefix() + msg);
      }
    }
    
    // Non-array value manipulation
    /**
       Get the keys for values in the set that are included in arrays.
       \return Non-array value keys.
     */
    static std::vector<EnumType>& non_array_values() {
      return non_array_enum_param<EnumType>();
    }
    /**
       Add a key to the set of keys that are not included in arrays.
       \param k Key to add to list of non-array keys.
     */
    static void add_non_array(const EnumType k) {
      if (k == EnumClass::NONE)
	return;
      add_non_array_enum_param(k);
    }
    /**
       Determine if a key is included in arrays.
       \param k Key to check.
       \returns true if the key is included in arrays, false otherwise.
     */
    static bool inArrays(const EnumType k) {
      for (typename std::vector<EnumType>::iterator it = non_array_values().begin();
	   it != non_array_values().end(); it++) {
        if (*it == k)
          return false;
      }
      return true;
    }
    
    // Inspection utilities
    /**
       Display the values in a value map.
       \param vals Value map to display.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_value_map(const std::map<EnumType, TORIG>& vals,
					 std::ostream &out,
					 const uint tab = 0) {
      const std::string space(tab * 4, ' ');
      for (typename std::map<EnumType, TORIG>::const_iterator it = vals.begin();
	   it != vals.end(); it++)
	out << space << utils::enum_key2string(it->first) << " = " <<
	  DREF_ORIG(it->second) << std::endl;
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
      const std::string space(tab * 4, ' ');
      for (typename std::map<EnumType, double>::const_iterator it = vals.begin();
	   it != vals.end(); it++)
	out << space << utils::enum_key2string(it->first) << " = " <<
	  it->second << std::endl;
      return out;
    }
    /**
       Display the keys in a vector.
       \param vals Vector of values to print.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_value_vector(const std::vector<EnumType>& vals,
					    std::ostream &out,
					    const uint tab = 0) {
      const std::string space(tab * 4, ' ');
      out << space << "[";
      for (typename std::vector<EnumType>::const_iterator it = vals.begin();
	   it != vals.end(); it++)
	out << utils::enum_key2string(*it) << ", ";
      out << "]" << std::endl;
      return out;
    }
    /**
       Convert a value map to a string.
       \param vals Value map to convert to a string.
       \param tab Number of tabs to prefix each line in the output with.
       \returns String.
     */
    static std::string string_value_map(const std::map<EnumType, double>& vals,
					const uint tab = 0) {
      std::ostringstream oss;
      print_value_map(vals, oss, tab);
      return oss.str();
    }
    /**
       Convert a vector of keys to a string.
       \param vals Vector of values to convert to a string.
       \param tab Number of tabs to prefix each line in the output with.
       \returns String.
     */
    static std::string string_value_vector(const std::vector<EnumType>& vals,
					   const uint tab = 0) {
      std::ostringstream oss;
      print_value_vector(vals, oss, tab);
      return oss.str();
    }
    /**
       Display the default values for the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_defaults(std::ostream &out, const uint tab = 0) {
      return print_value_map(default_values(), out, tab);
    }
    /**
       Display the keys currently skipped by the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_skipped(std::ostream &out, const uint tab = 0) {
      return print_value_vector(skipped_values(), out, tab);
    }
    /**
       Display the keys that are not part of arrays.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_non_array(std::ostream &out, const uint tab = 0) {
      return print_value_vector(non_array_values(), out, tab);
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
      double relative_max = std::max(std::fabs(a), std::fabs(b));
      double relative = std::fabs(a - b);
      if (relative_max > COMPARE_RELATIVE_EPSILON)
	relative = relative / relative_max;
      double absolute = std::fabs(a - b);
      return ((relative <= COMPARE_RELATIVE_EPSILON) &&
	      (absolute <= COMPARE_ABSOLUTE_EPSILON));
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
	  utils::enum_key2string(k) +
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
	  utils::enum_key2string(k) +
	  "\' not present";
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(error_prefix() + msg);
      }
    }
    // Value manipulation
    /**
       Add values to a value map.
       \param vals Map to add values to.
       \param new_values New values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
       \param ignore_constants If true, allow constant values to be
         updated.
     */
    static void update_value_map(std::map<EnumType, double>& vals,
				 const std::map<EnumType, double>& new_values,
				 bool clear_existing = false,
				 bool ignore_constants = false) {
      if (constant_values().size() == 0) {
	if (clear_existing)
	  vals.clear();
      } else {
	if (clear_existing) {
	  std::vector<EnumType> to_erase;
	  for (typename std::map<EnumType, double>::const_iterator it = vals.begin();
	       it != vals.end(); it++) {
	    if (!isConstant(it->first))
	      to_erase.push_back(it->first);
	  }
	  for (typename std::vector<EnumType>::const_iterator it = to_erase.begin();
	       it != to_erase.end(); it++) {
	    std::cerr << error_prefix() << "update_value_map: erasing " <<
	      utils::enum_key2string(*it) << std::endl;
	    vals.erase(vals.find(*it));
	  }
	}
	if (!ignore_constants) {
	  for (typename std::map<EnumType, double>::const_iterator it = new_values.begin();
	       it != new_values.end(); it++) {
	    typename std::map<EnumType, double>::const_iterator it_old = vals.find(it->first);
	    if (it_old == vals.end() ||
		!(valuesEqual(it_old->second, it->second))) {
	      std::string oldval = "NULL";
	      std::string newval = std::to_string(it->second);
	      if (it_old != vals.end())
		oldval = std::to_string(it_old->second);
	      checkConstant(it->first, "update_value_map[alts][" + oldval
			    + ", " + newval + "]: ");
	    }
	  }
	}
      }
      for (typename std::map<EnumType, double>::const_iterator it = new_values.begin();
	   it != new_values.end(); it++) {
	if (isSkipped(it->first) || isConstant(it->first))
	  continue;
	typename std::map<EnumType, double>::iterator it_old = vals.find(it->first);
	// if (it_old == vals.end())
	//   throw std::runtime_error(error_prefix() +
	// 			   "No alt for \'" +
	// 			   utils::enum_key2string(it->first) +
	// 			   "\'");
	std::cerr << error_prefix() << "update_value_map[alts][" <<
	  utils::enum_key2string(it->first) << "]: ";
	if (it_old == vals.end()) {
	  std::cerr << "NULL";
	  vals.emplace(it->first, it->second);
	} else {
	  std::cerr << it_old->second;
	  it_old->second = it->second;
	}
	std::cerr << " -> " << it->second <<
	  " [AFTER = " << it_old->second <<
	  "]" << std::endl;
      }
      for (typename std::vector<EnumType>::const_iterator s = skipped_values().begin();
	   s != skipped_values().end(); s++) {
	typename std::map<EnumType, double>::iterator it = vals.find(*s);
	if (it != vals.end()) {
	  std::cerr << error_prefix() << "update_value_map[alts]: erasing " <<
	    utils::enum_key2string(*s) << std::endl;
	  vals.erase(it);
	}
      }
    }
    /**
       Add values to a value map.
       \param vals Map to add values to.
       \param new_values New values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
       \param ignore_constants If true, allow constant values to be
         updated.
     */
    static void update_value_map(std::map<EnumType, TORIG>& vals,
				 const std::map<EnumType, double>& new_values,
				 bool clear_existing = false,
				 bool ignore_constants = false) {
      if (clear_existing) {
	std::vector<EnumType> to_erase;
	for (typename std::map<EnumType, TORIG>::const_iterator it = vals.begin();
	     it != vals.end(); it++) {
	  if (isSkipped(it->first) ||
	      (new_values.find(it->first) == new_values.end() && !isConstant(it->first)))
	    to_erase.push_back(it->first);
	}
	for (typename std::vector<EnumType>::const_iterator it = to_erase.begin();
	     it != to_erase.end(); it++) {
	  std::cerr << error_prefix() << "update_value_map[values]: erasing " <<
	    utils::enum_key2string(*it) << std::endl;
	  vals.erase(vals.find(*it));
	}
      }
      if (!ignore_constants) {
	for (typename std::map<EnumType, double>::const_iterator it = new_values.begin();
	     it != new_values.end(); it++) {
	  typename std::map<EnumType, TORIG>::const_iterator it_old = vals.find(it->first);
	  if (it_old == vals.end() ||
	      !(valuesEqual(DREF_ORIG(it_old->second), it->second))) {
	    std::string oldval = "NULL";
	    std::string newval = std::to_string(it->second);
	    if (it_old != vals.end())
	      oldval = std::to_string(DREF_ORIG(it_old->second));
	    checkConstant(it->first, "update_value_map[values][" + oldval
			  + ", " + newval + "]: ");
	  }
	}
      }
      for (typename std::map<EnumType, double>::const_iterator it = new_values.begin();
	   it != new_values.end(); it++) {
	if (isSkipped(it->first) || isConstant(it->first))
	  continue;
	typename std::map<EnumType, TORIG>::iterator it_old = vals.find(it->first);
	if (it_old == vals.end())
	  throw std::runtime_error(error_prefix() +
				   "update_value_map[values][constants]: No " TORIG_STRING " for \'" +
				   utils::enum_key2string(it->first) +
				   "\'");
	std::cerr << error_prefix() << "update_value_map[values][" <<
	  utils::enum_key2string(it->first) << "]: " <<
	  PTR_ORIG(it_old->second) << ", ";
	std::cerr << DREF_ORIG(it_old->second) << " -> " << it->second;
	DREF_ORIG(it_old->second) = it->second;
	std::cerr << " [AFTER = " << DREF_ORIG(it_old->second) <<
	  "]" << std::endl;
      }
    }
    /**
       Add values to a value map.
       \param vals Map to add values to.
       \param new_values New values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
       \param ignore_constants If true, allow constant values to be
         updated.
     */
    static void update_value_map(std::map<EnumType, TORIG>& vals,
				 const std::map<EnumType, TORIG>& new_values,
				 bool clear_existing = false,
				 bool ignore_constants = false) {
      if (clear_existing) {
	std::vector<EnumType> to_erase;
	for (typename std::map<EnumType, TORIG>::const_iterator it = vals.begin();
	     it != vals.end(); it++) {
	  if (isSkipped(it->first) ||
	      (new_values.find(it->first) == new_values.end() && !isConstant(it->first)))
	    to_erase.push_back(it->first);
	}
	for (typename std::vector<EnumType>::const_iterator it = to_erase.begin();
	     it != to_erase.end(); it++) {
	  std::cerr << error_prefix() << "update_value_map[values]: erasing " <<
	    utils::enum_key2string(*it) << std::endl;
	  vals.erase(vals.find(*it));
	}
      }
      if (!ignore_constants) {
	for (typename std::map<EnumType, TORIG>::const_iterator it = new_values.begin();
	     it != new_values.end(); it++) {
	  typename std::map<EnumType, TORIG>::const_iterator it_old = vals.find(it->first);
	  if (it_old == vals.end() ||
	      !(valuesEqual(DREF_ORIG(it_old->second),
			    DREF_ORIG(it->second)))) {
	    std::string oldval = "NULL";
	    std::string newval = std::to_string(DREF_ORIG(it->second));
	    if (it_old != vals.end())
	      oldval = std::to_string(DREF_ORIG(it_old->second));
	    checkConstant(it->first, "update_value_map[values][" + oldval
			  + ", " + newval + "]: ");
	  }
	}
      }
      for (typename std::map<EnumType, TORIG>::const_iterator it = new_values.begin();
	   it != new_values.end(); it++) {
	if (isSkipped(it->first) || isConstant(it->first))
	  continue;
	typename std::map<EnumType, TORIG>::iterator it_old = vals.find(it->first);
	if (it_old == vals.end())
	  throw std::runtime_error(error_prefix() +
				   "update_value_map[values][" TORIG_STRING "s]: No " TORIG_STRING " for \'" +
				   utils::enum_key2string(it->first) +
				   "\'");
	std::cerr << error_prefix() << "update_value_map[values][" <<
	  utils::enum_key2string(it->first) << "]: " <<
	  PTR_ORIG(it_old->second) << ", ";
	std::cerr << DREF_ORIG(it_old->second) << " -> " << DREF_ORIG(it->second);
	DREF_ORIG(it_old->second) = DREF_ORIG(it->second);
	std::cerr << " [AFTER = " << DREF_ORIG(it_old->second) <<
	  "]" << std::endl;
      }
    }
    /**
       Add values to a value map by reading a parameter file.
       \param vals Map to add values to.
       \param filename File containing values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    static void update_value_map(std::map<EnumType, double>& vals,
				 const std::string& filename,
				 bool clear_existing = false) {
      std::map<EnumType, double> new_values = utils::readEnumFile<EnumType, double>(filename);
      update_value_map(vals, new_values, clear_existing, true);
    }
    /**
       Add values to a value map by reading a parameter file.
       \param vals Map to add values to.
       \param filename File containing values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    static void update_value_map(std::map<EnumType, TORIG>& vals,
				 std::map<EnumType, TORIG>& vals_all,
				 const std::string& filename,
				 bool clear_existing = false) {
      std::map<EnumType, double> new_values = utils::readEnumFile<EnumType, double>(filename);
      update_value_map(vals, vals_all, new_values, clear_existing, true);
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
	update_value_map(vals, default_values_C3(), false, true);
      } else {
	update_value_map(vals, default_values(), false, true);
      }
    }
    static void init_value_map_orig(std::map<EnumType, TORIG>& vals,
				    const std::map<EnumType, TORIG>& new_vals) {
      // vals.insert(new_vals.begin(), new_vals.end());
      for (typename std::map<EnumType, TORIG>::const_iterator it_new = new_vals.begin();
	   it_new != new_vals.end(); it_new++) {
	typename std::map<EnumType, TORIG>::iterator it_old = vals.find(it_new->first);
	if (it_old == vals.end()) {
	  std::cerr << error_prefix() << "init_value_map_orig: emplace " <<
	    utils::enum_key2string(it_new->first) << std::endl;
	  vals.emplace(it_new->first, it_new->second);
	}
      }
      for (typename std::vector<EnumType>::const_iterator s = skipped_values().begin();
	   s != skipped_values().end(); s++) {
	typename std::map<EnumType, TORIG>::iterator it = vals.find(*s);
	if (it != vals.end()) {
	  std::cerr << error_prefix() << "init_value_map_orig[values]: erasing " <<
	    utils::enum_key2string(*s) << std::endl;
	  vals.erase(it);
	}
      }
    }
    /**
       Get the reference for the value associated with a key from a value
         map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Value reference.
     */
    static double& get_value(std::map<EnumType, double>& vals,
			     const EnumType k,
			     const std::string& context="[]") {
      checkSkipped(k, context);
      checkConstant(k, context);
      check_value_map(vals, k, context);
      if (context.size()) {
	std::cerr << error_prefix() << context << "get_value[double]: " <<
	  utils::enum_key2string(k) << std::endl;
      }
      return vals.find(k)->second;
    }
    /**
       Get the reference for the value associated with a key from a value
         map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Value reference.
     */
    static double& get_value(std::map<EnumType, TORIG>& vals,
			     const EnumType k,
			     const std::string& context="[]") {
      checkSkipped(k, context);
      checkConstant(k, context);
      check_value_map(vals, k, context);
      if (context.size()) {
	std::cerr << error_prefix() << context << "get_value[TORIG]: " <<
	  utils::enum_key2string(k) << std::endl;
      }
      return DREF_ORIG(vals.find(k)->second);
    }
    /**
       Get the constant reference for the value associated with a key from
         a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Constant value reference.
     */
    static const double& get_value(const std::map<EnumType, double>& vals,
				   const EnumType k,
				   const std::string& context="[]") {
      checkSkipped(k, context);
      check_value_map(vals, k, context);
      if (context.size()) {
	std::cerr << error_prefix() << context << "get_value[const double]: " <<
	  utils::enum_key2string(k) << std::endl;
      }
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
    static const double& get_value(const std::map<EnumType, TORIG>& vals,
				   const EnumType k,
				   const std::string& context="[]") {
      checkSkipped(k, context);
      check_value_map(vals, k, context);
      if (context.size()) {
	std::cerr << error_prefix() << context << "get_value[const TORIG]: " <<
	  utils::enum_key2string(k) << std::endl;
      }
      return DREF_ORIG(vals.find(k)->second);
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set_value(std::map<EnumType, double>& vals,
			  const EnumType k, const double v) {
      checkSkipped(k, "set_value: ");
      checkConstant(k, "set_value: ");
      std::cerr << error_prefix() << "set_value[double]: " <<
	utils::enum_key2string(k) << ": ";
      if (vals.find(k) == vals.end())
	std::cerr << "NULL";
      else
	std::cerr << vals[k];
      std::cerr << " -> " << v;
      vals[k] = v;
      std::cerr << " [AFTER = " << vals[k] << "]" << std::endl;
      // get_value(vals, k, "set") = v;
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set_value(std::map<EnumType, TORIG>& vals,
			  const EnumType k, const double v) {
      std::cerr << error_prefix() << "set_value[TORIG]: " <<
	utils::enum_key2string(k) << ": ";
      if (vals.find(k) == vals.end())
	std::cerr << "NULL";
      else
	std::cerr << DREF_ORIG(vals[k]);
      std::cerr << " -> " << v;
      get_value(vals, k, "set") = v;
      std::cerr << " [AFTER = " << DREF_ORIG(vals[k]) << std::endl;
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
      set_value(vals, k, EnzymeAct.at(utils::enum_key2string(k)));
    }
    /**
       Get the alternate value for the value associated with a
         key from a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Alternate value.
     */
    static TORIG& get_value_orig(std::map<EnumType, TORIG>& vals,
				 const EnumType k,
				 const std::string& context="[]") {
      checkSkipped(k, context);
      checkConstant(k, context);
      check_value_map(vals, k, context);
      std::cerr << error_prefix() << context << "get_value_orig: " <<
	utils::enum_key2string(k) << std::endl;
      return vals.find(k)->second;
    }
    /**
       Get the constant alternate value for the value associated with a
         key from a value map.
       \param vals Value map.
       \param k Key to get value for.
       \param context String providing context for error messages.
       \returns Constant alternate value.
     */
    static TORIG_CONST& get_value_orig(const std::map<EnumType, TORIG>& vals,
				       const EnumType k,
				       const std::string& context="[]") {
      checkSkipped(k, context);
      check_value_map(vals, k, context);
      std::cerr << error_prefix() << context << "get_value_orig[const]: " <<
	utils::enum_key2string(k) << std::endl;
      return vals.find(k)->second;
    }
    /**
       Add a reference to an original value for the given key to a value
         map.
       \param vals Value map to update.
       \param vals_all Value map of all values to update.
       \param k Key to update value for.
       \param v New value for key.
       \param context String providing context for error messages.
     */
    static void insert_value_orig(std::map<EnumType, TORIG>& vals,
				  std::map<EnumType, TORIG>& vals_all,
				  const EnumType k, double* v,
				  const std::string& context="") {
      if (vals_all.find(k) == vals_all.end()) {
	std::cerr << error_prefix() << "insert_value_orig: emplace " <<
	  utils::enum_key2string(k) << std::endl;
	vals.emplace(k, ADDR_ORIG(v[0]));
	vals_all.emplace(k, ADDR_ORIG(v[0]));
	std::cerr << error_prefix() << context << "inserted " <<
	  utils::enum_key2string(k) << " [" << v << "]" << std::endl;
      } else {
	std::cerr << error_prefix() << context <<
	  utils::enum_key2string(k) << " already inserted" << std::endl;
      }
    }
    /**
       Add a reference to an original value for the given key to a value
         map.
       \param vals Value map to update.
       \param vals_all Value map of all values to update.
       \param k Key to update value for.
       \param v New value for key.
       \param context String providing context for error messages.
     */
    static void insert_const_value_orig(std::map<EnumType, TORIG>& vals,
					std::map<EnumType, TORIG>& vals_all,
					const EnumType k, const double* v,
					const std::string& context="") {
      if (vals_all.find(k) == vals_all.end()) {
	std::cerr << error_prefix() << "insert_const_value_orig: emplace " <<
	  utils::enum_key2string(k) << std::endl;
	vals.emplace(k, ADDR_ORIG(const_cast<double*>(v)[0]));
	vals_all.emplace(k, ADDR_ORIG(const_cast<double*>(v)[0]));
	add_constant(k);
	std::cerr << error_prefix() << context << "inserted " <<
	  utils::enum_key2string(k) << " [" << v << "]" << std::endl;
      } else {
	std::cerr << error_prefix() << context <<
	  utils::enum_key2string(k) << " already inserted" << std::endl;
      }
    }
    /**
       Update the value associated with a key in a value map.
       \param vals Value map to update.
       \param k Key to update value for.
       \param v New value for key.
     */
    static void set_value_orig(std::map<EnumType, TORIG>& vals,
			       const EnumType k, TORIG v) {
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
    static void check_value_alt(const std::map<EnumType, TORIG>& vals,
				const std::map<EnumType, double>& alts,
				const EnumType k,
				const std::string& context="") {
      typename std::map<EnumType, double>::const_iterator itp = alts.find(k);
      if (itp == alts.end()) {
	throw std::runtime_error(error_prefix() + context +
				 "check_value_alt: Key \'" +
				 utils::enum_key2string(k) +
				 "\' is missing from alts");
      }
      typename std::map<EnumType, TORIG>::const_iterator itv = vals.find(k);
      if (itv == vals.end()) {
	throw std::runtime_error(error_prefix() + context +
				 "check_value_alt: Key \'" +
				 utils::enum_key2string(k) +
				 "\' is missing from values");
      }
      compareValues(k, DREF_ORIG(itv->second), itp->second,
		    context +
		    "Comparing \'" + utils::enum_key2string(k) +
		    "\' between old and new tracking methods");
    }
    /**
       Check that the values in a value map match the values in an
         original value map.
       \param vals Value map.
       \param alts Alternate value map.
       \param context String providing context for error messages.
     */
    static void check_value_alts(const std::map<EnumType, TORIG>& vals,
				 const std::map<EnumType, double>& alts,
				 const std::string& context="") {
      for (typename std::map<EnumType, TORIG>::const_iterator itv = vals.begin();
	   itv != vals.end(); itv++) {
	check_value_alt(vals, alts, itv->first, context);
      }
      for (typename std::map<EnumType, double>::const_iterator itp = alts.begin();
	   itp != alts.end(); itp++) {
	typename std::map<EnumType, TORIG>::const_iterator itv = vals.find(itp->first);
	if (itv == vals.end()) {
	  throw std::runtime_error(error_prefix() + context +
				   "check_value_alts: Key \'" +
				   utils::enum_key2string(itp->first) +
				   "\' is missing from values");
	}
      }
    }
    /**
       Reset all of the values in a value map that are not constant to 0.
       \param vals Value map to reset.
     */
    static void reset_value_orig_map(std::map<EnumType, TORIG>& vals) {
      std::cerr << error_prefix() << "reset_value_orig_map" << std::endl;
      for (typename std::map<EnumType, TORIG>::iterator it = vals.begin();
	   it != vals.end(); it++)
	if (!isConstant(it->first))
	  set_value(vals, it->first, default_value(it->first));
    }
    /**
       Reset all of the values in a value map that are not constant to 0.
       \param vals Value map to reset.
     */
    static void reset_value_map(std::map<EnumType, double>& vals) {
      std::cerr << error_prefix() << "reset_value_map" << std::endl;
      for (typename std::map<EnumType, double>::iterator it = vals.begin();
	   it != vals.end(); it++)
	if (!isConstant(it->first))
	  set_value(vals, it->first, default_value(it->first));
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
	   it != vals.end(); it++, i++) {
	if (inArrays(it->first))
	  out[i] = get_value(vals, it->first);
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
      size_t nexp = vals.size() - non_array_values().size();
      size_t nact = vec.size() - offset;
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
	  checkConstant(it->first, "value_map_fromArray: ");
	  set_value(vals, it->first, vec[offset + i]);
	  i++;
	}
      }
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
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<ID, PT>)

    /**
       Default constructor. Values will be initialized with default
         values, minus any values that are skipped.
     */
    ValueSet() :
#ifdef CHECK_VALUE_SET_ALTS
      alts(),
#endif // CHECK_VALUE_SET_ALTS
      values(), values_all() {}
    /**
       Copy constructor. Values will be updated from the values in the
         copied set.
     */
    ValueSet(const ValueSet& other) : ValueSet() {}
    /**
       Copy values from an array.
     */
    ValueSet(const arr &vec, const std::size_t offset) : ValueSet() {}
    /**
       Copy values from another value set to this one.
       \param other Value set to copy values from.
     */
    virtual void copyMembers(const ValueSet& other) {
      init_value_map_orig(values, values_all);
      print_skipped(std::cerr);
      std::cerr << "ValueSet::copy" << std::endl;
      update_value_map(values, other.values, true);
#ifdef CHECK_VALUE_SET_ALTS
      update_value_map(alts, other.alts, true);
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Initialize the values in the instance. This should be overriden
         by child classes with a method that adds pointers to values.
     */
    virtual void initMembers() {
      init_value_map_orig(values, values_all);
    }
    /**
       Initialize the static values in the instance.
     */
    static void initStaticMembers() {}
    /**
       Re-initialize the values to the default values, minus any values
         that are skipped.
       \param useC3 If true, default values for a C3 will be used
     */
    void initValues(const bool useC3) {
      init_value_map_orig(values, values_all);
      init_value_map(values, useC3);
#ifdef CHECK_VALUE_SET_ALTS
      init_value_map(alts, useC3);
#endif // CHECK_VALUE_SET_ALTS
    }
    /**
       Get the current number of values in the set.
       \returns Number of values in the set.
     */
    virtual std::size_t size_values() const {
      return CALL_STATIC_METHOD(size);
    }
    /** Get the iterator pointing to the start of the value set */
    iterator begin() { return CALL_STATIC_METHOD(begin); }
    /** Get the iterator pointing to the end of the value set */
    iterator end() { return CALL_STATIC_METHOD(end); }
    /** Get the constant iterator pointing to the start of the value set */
    const_iterator begin() const { return CALL_STATIC_METHOD(begin); }
    /** Get the constant iterator pointing to the end of the value set */
    const_iterator end() const { return CALL_STATIC_METHOD(end); }
    // Inspection utilities
    /**
       Display the values in the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    virtual std::ostream& print(std::ostream &out, const uint tab = 0) const {
      return CALL_STATIC(print_value_map, out, tab);
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    void check(const EnumType k, const std::string& context = "") const {
      CALL_STATIC(check_value_map, k, context);
    }
    // Value manipulation
    /**
       Add values to the set.
       \param new_values New values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    void update_values(const std::map<EnumType, double>& new_values,
		       bool clear_existing = false) {
      CALL_STATIC(update_value_map, new_values, clear_existing);
    }
    /**
       Add values to the set by reading a parameter file.
       \param filename File containing values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    void update_values(const std::string& filename,
		       bool clear_existing = false) {
      CALL_STATIC(update_value_map, filename, clear_existing);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    double& operator[](const EnumType k) {
      return CALL_STATIC(get_value, k);
    }
    /**
       Get the constant reference for the value associated with a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    const double& operator[](const EnumType k) const {
      return CALL_STATIC(get_value, k);
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
      return CALL_STATIC(set_value_from_EnzymeAct, k, EnzymeAct);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    virtual double get(const EnumType k) const {
      return CALL_STATIC(get_value, k);
    }
    /**
       Add a value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    virtual void insertOrig(const EnumType k, double* v,
			    const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end())
	alts[k] = *v;
#endif // CHECK_VALUE_SET_ALTS
      insert_value_orig(values, values_all, k, v, context);
    }
    /**
       Add a constant value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    virtual void insertConstOrig(const EnumType k, const double* v,
				 const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end())
	alts[k] = *v;
#endif // CHECK_VALUE_SET_ALTS
      insert_const_value_orig(values, values_all, k, v, context);
    }
    /**
       Update the value reference associated with a key.
       \param k Key to update value for.
       \param v New value reference for key.
     */
    virtual void setOrig(const EnumType k, TORIG v) {
      set_value_orig(values, k, v);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    virtual TORIG getOrig(const EnumType k) {
      return get_value_orig(values, k);
    }
    /**
       Get the const reference for the value reference associated with
         a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    virtual TORIG_CONST getOrig(const EnumType k) const {
      return get_value_orig(values, k);
    }
    /**
       Check if the value using the original code matches the value
         calculated using only dynamic value methods.
       \param k Key to check.
     */
    virtual void checkAlt(const EnumType k) const {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alt(values, alts, k);
#else // CHECK_VALUE_SET_ALTS
      UNUSED(k);
      throw std::runtime_error(error_prefix() + "Alternates not enabled");
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
       Reset all of the values to 0;
     */
    virtual void resetValues() {
      CALL_STATIC_NOARGS(reset_value_map);
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    virtual arr toArray() const {
      return CALL_STATIC_NOARGS(value_map_toArray);
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
    std::map<EnumType, TORIG> values; /**< Values in the set. */
    std::map<EnumType, TORIG> values_all; /**< Values in the set including those that are being skipped. */
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
    INHERIT_METHODS_VALUE_SET_BASE(ValueSetBase<ID, PT>)
    
    /**
       Initialize the values in the instance. This should be overriden
         by child classes with a method that adds pointers to values.
     */
    static void initMembers() {
      init_value_map_orig(values, values_all);
    }
    /**
       Initialize the static values in the instance.
     */
    static void initStaticMembers() {
      initMembers();
    }
    /**
       Re-initialize the values to the default values, minus any values
         that are skipped.
       \param useC3 If true, default values for a C3 will be used
     */
    static void initValues(const bool useC3) {
      std::cerr << error_prefix() << "Before init_value_map_orig" << std::endl;
      init_value_map_orig(values, values_all);
      std::cerr << error_prefix() << "Before init_value_map[values]" << std::endl;
      init_value_map(values, useC3);
#ifdef CHECK_VALUE_SET_ALTS
      std::cerr << error_prefix() << "Before init_value_map[alts]" << std::endl;
      init_value_map(alts, useC3);
#endif // CHECK_VALUE_SET_ALTS
      std::cerr << error_prefix() << "initValues done" << std::endl;
    }
    /**
       Get the current number of values in the set.
       \returns Number of values in the set.
     */
    static std::size_t size_values() {
      return CALL_STATIC_METHOD(size);
    }
    /** Get the iterator pointing to the start of the value set */
    static iterator begin() { return CALL_STATIC_METHOD(begin); }
    /** Get the iterator pointing to the end of the value set */
    static iterator end() { return CALL_STATIC_METHOD(end); }
    /** Get the constant iterator pointing to the start of the value set */
    // Inspection utilities
    /**
       Display the values in the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print(std::ostream &out, const uint tab = 0) {
      return CALL_STATIC(print_value_map, out, tab);
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
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    static void update_values(const std::map<EnumType, double>& new_values,
			      bool clear_existing = false) {
      CALL_STATIC(update_value_map, new_values, clear_existing);
    }
    /**
       Add values to the set by reading a parameter file.
       \param filename File containing values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    static void update_values(const std::string& filename,
			      bool clear_existing = false) {
      CALL_STATIC(update_value_map, filename, clear_existing);
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
      return CALL_STATIC(set_value_from_EnzymeAct, k, EnzymeAct);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    static double get(const EnumType k) {
      return CALL_STATIC(get_value, k);
    }
    /**
       Add a value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    static void insertOrig(const EnumType k, double* v,
			   const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end())
	alts[k] = *v;
#endif // CHECK_VALUE_SET_ALTS
      insert_value_orig(values, values_all, k, v, context);
    }
    /**
       Add a constant value reference for the given key.
       \param k Key to update value for.
       \param v New value pointer for key.
       \param context String providing context for error messages.
     */
    static void insertConstOrig(const EnumType k, const double* v,
				const std::string& context="") {
#ifdef CHECK_VALUE_SET_ALTS
      if (alts.find(k) == alts.end())
	alts[k] = *v;
#endif // CHECK_VALUE_SET_ALTS
      insert_const_value_orig(values, values_all, k, v, context);
    }
    /**
       Update the value reference associated with a key.
       \param k Key to update value for.
       \param v New value reference for key.
     */
    static void setOrig(const EnumType k, TORIG v) {
      set_value_orig(values, k, v);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    static TORIG getOrig(const EnumType k) {
      return get_value_orig(values, k);
    }
    /**
       Get the const reference for the value reference associated with
         a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    static TORIG_CONST getOrigConst(const EnumType k) {
      return get_value_orig(const_cast<const std::map<EnumType, TORIG>&>(values), k);
    }
    /**
       Check if the value using the original code matches the value
         calculated using only dynamic value methods.
       \param k Key to check.
     */
    static void checkAlt(const EnumType k) {
#ifdef CHECK_VALUE_SET_ALTS
      check_value_alt(values, alts, k);
#else // CHECK_VALUE_SET_ALTS
      UNUSED(k);
      throw std::runtime_error(error_prefix() + "Alternates not enabled");
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
       Reset all of the values to 0;
     */
    static void resetValues() {
      CALL_STATIC_NOARGS(reset_value_map);
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    static arr toArray() {
      return CALL_STATIC_NOARGS(value_map_toArray);
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
    static std::map<EnumType, TORIG> values; /**< Values in the set. */
    static std::map<EnumType, TORIG> values_all; /**< Values in the set including those that are being skipped. */
  };

#ifdef CHECK_VALUE_SET_ALTS
  template<MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<ID, PT>::EnumType, double>
  ValueSetStatic<ID, PT>::alts = {};
#endif // CHECK_VALUE_SET_ALTS

  template<MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<ID, PT>::EnumType, TORIG>
  ValueSetStatic<ID, PT>::values = {};
  
  template<MODULE ID, PARAM_TYPE PT>
  std::map<typename ValueSetStatic<ID, PT>::EnumType, TORIG>
  ValueSetStatic<ID, PT>::values_all = {};

  template<MODULE ID, PARAM_TYPE PT>
  const MODULE ValueSetBase<ID, PT>::module = ID;
  
  template<MODULE ID, PARAM_TYPE PT>
  const PARAM_TYPE ValueSetBase<ID, PT>::param_type = PT;

}
    
#undef CALL_STATIC
