// Include guard here, just in case
// #pragma once
#include <cstdlib>
#include "ValueSet.hpp"
#include "Variables.hpp"

using namespace ePhotosynthesis;

//////////////////////////////////////////////////////////
// ValueSet_t
//////////////////////////////////////////////////////////
bool ValueSet_t::equals(const ValueSet_t& b0, const bool) const {
  if(typeid(*this) != typeid(b0)) {
    std::cerr << "typeids not equal in ValueSet_t" << std::endl;
    return false;
  }
  return true;
}
bool ValueSet_t::operator==(const ValueSet_t& b) const {
  return equals(b);
}
bool ValueSet_t::operator!=(const ValueSet_t& b) const {
  return !(this->operator==(b));
}

#define ADD_METHOD(name, retT, args, retV, suffix)                      \
  retT ValueSet_t::name VS_ARGS_T_NODEF(args) suffix {                  \
    FOR_EACH(UNUSED, EXPAND VS_ARGS(args));                             \
    IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
      }
#define ADD_METHOD_BOTH(name, retT, args, retV, suffix)                 \
  ADD_METHOD(name, PACK_MACRO(retT), args, retV, suffix)                \
  retT ValueSet_t::_static_ ## name VS_ARGS_T_NODEF(args) {             \
    FOR_EACH(UNUSED, EXPAND VS_ARGS(args));                             \
    IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
  }
#define ADD_METHOD_STATIC(name, retT, args, retV, suffix)               \
  retT ValueSet_t::_virtual_ ## name VS_ARGS_T_NODEF(args) suffix {     \
    FOR_EACH(UNUSED, EXPAND VS_ARGS(args));                             \
    IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
  }

// Methods specific to classes
ADD_METHODS_VALUE_SET_T_INST_ONLY(ADD_METHOD)
ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(ADD_METHOD_STATIC)
    
// Methods static in ValueSetStatic and not static in ValueSet
ValueSet_t* ValueSet_t::getValueSet() {
  return const_cast<ValueSet_t*>(const_cast<const ValueSet_t*>(this)->getValueSet());
}
std::vector<ValueSet_t**>& ValueSet_t::getChildren() {
  return const_cast<std::vector<ValueSet_t**>&>(const_cast<const ValueSet_t*>(this)->getChildren());
}
ADD_METHODS_VALUE_SET_T_BOTH(ADD_METHOD_BOTH)

// Methods static in both classes
ADD_METHODS_VALUE_SET_T_STATIC(ADD_METHOD_STATIC)
    
#undef ADD_METHOD_BOTH
#undef ADD_METHOD
#undef ADD_METHOD_STATIC


//////////////////////////////////////////////////////////
// ValueSetClass_t 
//////////////////////////////////////////////////////////
#define ADD_METHOD_BOTH(name, ...)
#define ADD_METHOD(name, retT, args, retV, suffix)	\
  retT ValueSetClass_t::name VS_ARGS_T_NODEF(args) suffix {             \
    FOR_EACH(UNUSED, EVAL(EXPAND VS_ARGS(args)));                       \
    IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
  }
#define DISABLE_METHOD(name, retT, args, retV, suffix)          \
  retT ValueSetClass_t::name VS_ARGS_T_NODEF(args) suffix {     \
    FOR_EACH(UNUSED, EVAL(EXPAND VS_ARGS(args)));               \
    throw std::runtime_error(std::string(__func__) +		\
                             ": Not a static method");                  \
    IF_NOT_EMPTY(retV, static PACK_MACRO(retT) tmp_ = retV; return tmp_;) \
  }
#define ADD_METHOD_STATIC(name, ...)
    
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

//////////////////////////////////////////////////////////
// ValueSetClassType
//////////////////////////////////////////////////////////
#define ADD_METHOD(name, retT, args, retV, suffix)                      \
  template<typename T>                                                  \
  retT ValueSetClassType<T>::                                           \
  name VS_ARGS_T_NODEF(args) suffix {                                   \
    return T::name VS_ARGS(args);                                       \
  }									\
  template<typename T>                                                  \
  STUB_VIRTUAL_VALUE_SET_DEFINE(ValueSetClassType<T>,                   \
                                name, retT, args, retV, suffix)
#define ADD_METHOD_BOTH(name, retT, args, retV, suffix)                 \
  template<typename T>                                                  \
  retT ValueSetClassType<T>::                                           \
  name VS_ARGS_T_NODEF(args) suffix {                                   \
    return T::_static_ ## name VS_ARGS(args);                           \
  }

// // Methods specific to classes
// ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(ADD_METHOD)
    
// // Methods virtual in ValueSet & static in ValueSetStatic
// ADD_METHODS_VALUE_SET_T_BOTH(ADD_METHOD_BOTH)

// // Methods static in both classes
// ADD_METHODS_VALUE_SET_T_STATIC(ADD_METHOD)

#undef ADD_METHOD_BOTH
#undef ADD_METHOD


//////////////////////////////////////////////////////////
// ValueSetStaticClassType
//////////////////////////////////////////////////////////
#define ADD_METHOD(name, retT, args, retV, suffix)              \
  template<typename T>                                          \
  retT ValueSetStaticClassType<T>::                             \
  name VS_ARGS_T(args) suffix {                                 \
    return T::name VS_ARGS(args);                               \
  }
#define DISABLE_METHOD(name, retT, args, retV, suffix)                  \
  template<typename T>                                                  \
  retT ValueSetStaticClassType<T>::                                     \
  name VS_ARGS_T(args) suffix {                                         \
    FOR_EACH(UNUSED, EXPAND VS_ARGS(args));                             \
    throw std::runtime_error(std::string(__func__) +			\
                             ": Invalid for static value set");         \
    return retV;							\
  }

// // Methods specific to classes
// ADD_METHODS_VALUE_SET_T_INST_ONLY_STATIC(DISABLE_METHOD)
// ADD_METHODS_VALUE_SET_T_STATIC_ONLY(ADD_METHOD)

#undef DISABLE_METHOD
#undef ADD_METHOD

//////////////////////////////////////////////////////////
// ValueSetWrapperBase
//////////////////////////////////////////////////////////
template<typename T>
ValueSetWrapperBase<T>::ValueSetWrapperBase(T* ptr) :
ptr_(ptr) {
}
template<typename T>
ValueSetWrapperBase<T>::ValueSetWrapperBase(ValueSetWrapperBase&& other) :
  ptr_(nullptr) {
  std::swap(other.ptr_, ptr_);
}
template<typename T>
ValueSetWrapperBase<T>::~ValueSetWrapperBase() {
  if (ptr_) {
    delete ptr_;
    ptr_ = nullptr;
  }
}
template<typename T>
ValueSetWrapperBase<T>& ValueSetWrapperBase<T>::operator=(ValueSetWrapperBase&& other) {
  if (this != &other) {
    this->~ValueSetWrapperBase();
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
  }
  return *this;
}
template<typename T>
T* ValueSetWrapperBase<T>::operator->() {
  if (ptr_ == nullptr)
    throw std::runtime_error("T pointer is null");
  return ptr_;
}
template<typename T>
const T* ValueSetWrapperBase<T>::operator->() const {
  if (ptr_ == nullptr)
    throw std::runtime_error("T pointer is null");
  return ptr_;
}
template<typename T>
T& ValueSetWrapperBase<T>::operator*() {
  return *ptr_;
}
template<typename T>
const T& ValueSetWrapperBase<T>::operator*() const {
  return *ptr_;
}
template<typename T>
ValueSetWrapperBase<T>::operator bool() const {
  return (bool)(ptr_);
}


//////////////////////////////////////////////////////////
// ValueSetBase
//////////////////////////////////////////////////////////

#define TEMPLATE template<class T, class U, MODULE ID, PARAM_TYPE PT>
#define CLASS ValueSetBase<T, U, ID, PT>

TEMPLATE
ValueSetClass_t* CLASS::getValueSetClass() {
  static ValueSetStaticClassType<T> out;
  return &out;
}
TEMPLATE
typename CLASS::EnumType CLASS::int2key(const int& k) {
  if (k < 0)
    ERROR_VALUE_SET("Cannot convert a negative value to an enum");
  if (k > ValueSetEnum<ID, PT>::all.size())
    ERROR_VALUE_SET("Integer (", k,
                    ") exceeds maximum value in enum (",
                    std::to_string(ValueSetEnum<ID, PT>::all.size()),
                    ")");
  return ValueSetEnum<ID, PT>::all[k - 1]; // NONE not in all
}

TEMPLATE
int CLASS::key2int(const typename CLASS::EnumType& k) {
  return static_cast<int>(k);
}

/*
TEMPLATE
template<typename V>
std::map<typename CLASS::EnumType, V> CLASS::int2key(const std::map<int, V>& map) {
  std::map<typename CLASS::EnumType, V> out;
  for (typename std::map<int, V>::const_iterator it = map.begin();
       it != map.end(); it++)
    out.insert({int2key(it->first), it->second});
  return out;
}
*/

TEMPLATE
bool CLASS::inArrays(const typename CLASS::EnumType k) {
  return (!(CLASS::EnumBaseClass::isNonvector(k) ||
            CLASS::EnumBaseClass::isSkipped(k)));
}

/*
TEMPLATE
template<typename K, typename V>
std::size_t CLASS::field_width_value_map(const std::map<K, V>& vals,
                                         bool includePrefixes) {
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
    pad += utils::get_enum_names<PARAM_TYPE>().find(param_type)->second.size();
    pad += utils::get_enum_names<MODULE>().find(module)->second.size();
  }
  return pad;
}

TEMPLATE
template<typename K, typename V>
std::size_t CLASS::value_width_value_map(const std::map<K, V>& vals) {
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
*/

TEMPLATE
std::size_t CLASS::max_field_width(bool noChildren) {
  static std::size_t out = 0;
  static std::size_t outBase = 0;
  if (outBase == 0) {
    outBase = field_width_value_map(defaults);
  }
  if (noChildren) return outBase;
  if (out == 0) {
    out = outBase;
    std::size_t iout = 0;
#define DO_CHILD(child, dummy)						\
    iout = child->max_field_width();                                    \
    if (iout > out) out = iout
    DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  }
  return out;
}

TEMPLATE
std::size_t CLASS::max_default_value_width(bool noChildren) {
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
    iout = child->max_default_value_width();                            \
    if (iout > out) out = iout
    DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  }
  return out;
}

TEMPLATE
std::ostream& CLASS::print_value(const double& val, std::ostream &out,
                                 bool show_pointers) {
  if (show_pointers)
    out << &val;
  else
    out << val;
  return out;
}
TEMPLATE
std::ostream& CLASS::print_value(const double* val, std::ostream &out,
                                 bool show_pointers) {
  if (show_pointers)
    out << val;
  else
    out << *val;
  return out;
}

/*
TEMPLATE
template<typename V>
std::string CLASS::string_value(V const val,
                                bool show_pointers) {
  std::ostringstream out;
  print_value(val, out, show_pointers);
  return std::move(out).str();
}
*/

TEMPLATE
std::string CLASS::getTabs(const uint tab) {
  return std::string(tab * CLASS::tab_size, ' ');
}

/*
TEMPLATE
template<typename V>
std::ostream& CLASS::print_value(const typename CLASS::EnumType& key,
                                 V const & val,
                                 std::ostream &out,
                                 const uint tab,
                                 std::size_t pad,
                                 bool includePrefixes,
                                 bool includeSkipped,
                                 const std::vector<std::string>& skip_keys,
                                 const std::map<std::string, std::string>& key_aliases,
                                 bool show_pointers,
                                 std::string iname) {
  if ((!includeSkipped) && (CLASS::EnumBaseClass::isSkipped(key) ||
                            CLASS::EnumBaseClass::isConstant(key)))
    return out;
  const std::string space = getTabs(tab);
  if (iname.empty()) {
    if (includePrefixes)
      iname = getNameWithPrefix(key);
    else
      iname = getName(key);
  } else if (includePrefixes) {
    iname = utils::enum_key2string(module) + "::" +
      utils::enum_key2string(param_type) + "::" + iname;
  }
  std::map<std::string, std::string>::const_iterator it_alias = key_aliases.find(iname);
  if (!utils::contains(skip_keys, iname)) {
    out << space << std::setw(pad + CLASS::tab_size) << std::left;
    if (it_alias != key_aliases.end())
      iname = it_alias->second;
    out << iname;
    print_value(val, out, show_pointers);
    out << std::endl;
  }
  return out;
}

TEMPLATE
template<typename V>
std::ostream& CLASS::print_value(const std::string& name,
                                 V const & val,
                                 std::ostream &out,
                                 const uint tab,
                                 std::size_t pad,
                                 bool includePrefixes,
                                 bool includeSkipped,
                                 const std::vector<std::string>& skip_keys,
                                 const std::map<std::string, std::string>& key_aliases,
                                 bool show_pointers,
                                 std::string iname) {
  typename CLASS::EnumType key;
  if (iname.empty())
    key = fromNameWithAliases(name);
  else
    key = int2key(0);
  if (pad == 0) pad = max_field_width();
  return print_value(key, val, out, tab, pad,
                     includePrefixes, includeSkipped,
                     skip_keys, key_aliases, show_pointers, iname);
}

TEMPLATE
template<typename V>
std::ostream& CLASS::print_value_map(const std::map<typename CLASS::EnumType, V>& vals,
                                     std::ostream &out,
                                     const uint tab,
                                     std::size_t pad,
                                     bool includePrefixes,
                                     bool includeSkipped,
                                     const std::vector<std::string>& skip_keys,
                                     const std::map<std::string, std::string>& key_aliases,
                                     bool show_pointers) {
  check_value_map(vals, "print_value_map: ");
  if (pad == 0)
    pad = field_width_value_map(vals, includePrefixes);
  for (typename std::map<typename CLASS::EnumType, V>::const_iterator it = vals.begin();
       it != vals.end(); it++) {
    print_value(it->first, it->second,
                out, tab, pad, includePrefixes, includeSkipped,
                skip_keys, key_aliases, show_pointers);
  }
  return out;
}
TEMPLATE
template<typename V>
std::string CLASS::to_string_with_precision(V const a_value,
                                            const int n) {
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return std::move(out).str();
}
*/

TEMPLATE
bool CLASS::valuesEqual(const double& a, const double& b) {
  return utils::values_equal(a, b);
}
TEMPLATE
bool CLASS::valuesEqual(const double* a, const double* b) {
  return valuesEqual(*a, *b);
}
TEMPLATE
bool CLASS::valuesEqual(const double& a, const double* b) {
  return valuesEqual(a, *b);
}
TEMPLATE
bool CLASS::valuesEqual(const double* a, const double& b) {
  return valuesEqual(*a, b);
}
TEMPLATE
bool CLASS::valuesEqual(const typename CLASS::ValueType& a, const typename CLASS::ValueType& b) {
  return valuesEqual(a.x, b.x);
}
TEMPLATE
bool CLASS::valuesEqual(const typename CLASS::ValueType& a, const double& b) {
  return valuesEqual(a.x, b);
}
TEMPLATE
bool CLASS::valuesEqual(const typename CLASS::ValueType& a, const double* b) {
  return valuesEqual(a.x, *b);
}
TEMPLATE
bool CLASS::valuesEqual(const double& a, const typename CLASS::ValueType& b) {
  return valuesEqual(a, b.x);
}
TEMPLATE
bool CLASS::valuesEqual(const double* a, const typename CLASS::ValueType& b) {
  return valuesEqual(*a, b.x);
}

TEMPLATE
std::string CLASS::valueString(const typename CLASS::ValueType& a) {
  return valueString(a.x);
}
TEMPLATE
std::string CLASS::valueString(const double& a) {
  return std::to_string(a);
}
TEMPLATE
std::string CLASS::valueString(const double* a) {
  if (!a)
    return "NULL";
  return std::to_string(*a);
}

TEMPLATE
std::string CLASS::valuePointerString(const double& a) {
  return "N/A";
}
TEMPLATE
std::string CLASS::valuePointerString(const double* a) {
  std::ostringstream oss;
  oss << a;
  return oss.str();
}
TEMPLATE
std::string CLASS::valuePointerString(const typename CLASS::ValueType& a) {
  return valuePointerString(a.x);
}

TEMPLATE
void CLASS::valueSet(double& dst, const double& src,
                     const std::string&) {
  dst = src;
}
TEMPLATE
void CLASS::valueSet(double& dst, const double* src,
                     const std::string& context) {
  if (!src)
    ERROR_VALUE_SET(context, "source pointer null");
  dst = src[0];
}
TEMPLATE
void CLASS::valueSet(double* dst, const double& src,
                     const std::string& context) {
  if (!dst)
    ERROR_VALUE_SET(context, "destination pointer null");
  dst[0] = src;
}
TEMPLATE
void CLASS::valueSet(double* dst, const double* src,
                     const std::string& context) {
  if (!dst)
    ERROR_VALUE_SET(context, "pointer null");
  if (!src)
    ERROR_VALUE_SET(context, "source pointer null");
  dst[0] = src[0];
}
/*
TEMPLATE
template<typename V>
void CLASS::valueSet(typename CLASS::ValueType& dst, V const & src,
                     const std::string&) {
  dst = src;
}
*/

TEMPLATE
double CLASS::valueGet(const double* x) { return *x; }
TEMPLATE
double CLASS::valueGet(const double& x) { return x; }
TEMPLATE
double CLASS::valueGet(const typename CLASS::ValueType& x) { return valueGet(x.x); }

TEMPLATE
void CLASS::insertValue(std::map<typename CLASS::EnumType, double*>& valsDst,
                        const typename CLASS::EnumType& k, const double* src,
                        const std::string& context) {
  if (!src)
    ERROR_VALUE_SET(context, "source pointer null");
  valsDst[k] = const_cast<double*>(src);
}
TEMPLATE
void CLASS::insertValue(std::map<typename CLASS::EnumType, double>& valsDst,
                        const typename CLASS::EnumType& k, const double& src,
                        const std::string&) {
  valsDst[k] = src;
}
TEMPLATE
void CLASS::insertValue(std::map<typename CLASS::EnumType, double>& valsDst,
                        const typename CLASS::EnumType& k, const double* src,
                        const std::string& context) {
  if (!src)
    ERROR_VALUE_SET(context, "source pointer null");
  valsDst[k] = src[0];
}
TEMPLATE
void CLASS::insertValue(std::map<typename CLASS::EnumType, double*>&,
                        const typename CLASS::EnumType& k, const double&,
                        const std::string& context) {
  ERROR_VALUE_SET(context, "No value for \'",
                  getName(k), "\'");
}
/*
TEMPLATE
template<typename V>
void CLASS::insertValue(std::map<typename CLASS::EnumType, typename CLASS::ValueType>& valsDst,
                        const typename CLASS::EnumType& k, V const & src,
                        const std::string&) {
  valsDst[k] = src;
}
*/

TEMPLATE
void CLASS::compareValues(const typename CLASS::EnumType k,
                          const double& a, const double& b,
                          const std::string& context) {
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
/*
TEMPLATE
template<typename V>
void CLASS::check_value_map(const std::map<typename CLASS::EnumType, V>& vals,
                            const std::string& context) {
  if (vals.empty() && !defaults.empty()) {
    ERROR_VALUE_SET(context, "Value map empty");
  }
}
TEMPLATE
template<typename V>
void CLASS::check_value_map(const std::map<typename CLASS::EnumType, V>& vals,
                            const typename CLASS::EnumType k,
                            const std::string& context) {
  typename std::map<typename CLASS::EnumType, V>::const_iterator it = vals.find(k);
  if (it == vals.end()) {
    std::string msg = "\'" +
      getName(k) +
      "\' not present";
    if (!context.empty())
      msg = context + ": " + msg;
    ERROR_VALUE_SET(msg);
  }
}

TEMPLATE
template<typename V>
void CLASS::remove_skipped(std::map<typename CLASS::EnumType, V>& vals,
                           const std::string& context) {
  for (typename std::map<typename CLASS::EnumType, int>::const_iterator s = CLASS::EnumBaseClass::value_flags.begin();
       s != CLASS::EnumBaseClass::value_flags.end(); s++) {
    if (!(s->second & VALUE_FLAG_SKIPPED))
      continue;
    typename std::map<typename CLASS::EnumType, V>::iterator it = vals.find(s->first);
    if (it != vals.end()) {
      DEBUG_VALUE_SET(context, "erasing ", s->first);
      vals.erase(it);
    }
  }
}
*/
    
TEMPLATE
std::map<std::string, double*> CLASS::get_pointer_map(std::map<typename CLASS::EnumType, double*> vals) {
  std::map<std::string, double*> out;
  for (typename std::map<typename CLASS::EnumType, double*>::iterator it = vals.begin();
       it != vals.end(); it++) {
    out[getName(it->first)] = it->second;
  }
  return out;
}

/*
TEMPLATE
template<typename K, typename VSrc, typename VDst>
std::string CLASS::diff_value_maps(const std::map<K, VDst>& valsDst,
                                   const std::map<K, VSrc>& valsSrc,
                                   std::size_t padKeys,
                                   std::size_t padVals,
                                   bool includePrefixes) {
  std::map<K, std::string> valStrDst, valStrSrc;
#define GET_VAL(x) to_string_with_precision(utils::extract_value(x))
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
    out << std::setw(padKeys + CLASS::tab_size) << std::left;
    if (includePrefixes)
      out << getNameWithPrefix(itSrc->first);
    else
      out << getName(itSrc->first);
    out << std::setw(padVals + CLASS::tab_size) << std::left << itDst->second;
    out << std::setw(padVals + CLASS::tab_size) << std::left << itSrc->second << std::endl;
  }
#undef GET_VAL
  return std::move(out.str());
}

TEMPLATE
template<typename VSrc, typename VDst>
bool CLASS::compare_value_maps(const std::map<typename CLASS::EnumType, VDst>& valsDst,
                               const std::map<typename CLASS::EnumType, VSrc>& valsSrc,
                               const std::string& context) {
  for (typename std::map<typename CLASS::EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
       itSrc != valsSrc.end(); itSrc++) {
    typename std::map<typename CLASS::EnumType, VDst>::const_iterator itDst = valsDst.find(itSrc->first);
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
    for (typename std::map<typename CLASS::EnumType, VDst>::const_iterator itDst = valsDst.begin();
         itDst != valsDst.end(); itDst++) {
      typename std::map<typename CLASS::EnumType, VSrc>::const_iterator itSrc = valsSrc.find(itDst->first);
      if (itSrc == valsSrc.end()) {
        INFO_VALUE_SET(context, itDst->first,
                       " value not present in b");
        return false;
      }
    }
  }
  return true;
}

TEMPLATE
template<typename VSrc, typename VDst>
bool CLASS::compare_value_maps(const std::map<int, VDst>& valsDst,
                               const std::map<int, VSrc>& valsSrc,
                               const std::string& context) {
  std::map<typename CLASS::EnumType, VDst> kvalsDst = int2key(valsDst);
  std::map<typename CLASS::EnumType, VSrc> kvalsSrc = int2key(valsSrc);
  return compare_value_maps(kvalsDst, kvalsSrc, context);
}

TEMPLATE
template<typename VSrc, typename VDst>
void CLASS::check_value_maps(std::map<typename CLASS::EnumType, VDst>& valsDst,
                             const std::map<typename CLASS::EnumType, VSrc>& valsSrc,
                             const std::string& context,
                             bool ignore_extras,
                             bool ignore_missing,
                             bool ignore_constants) {
  for (typename std::map<typename CLASS::EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
       itSrc != valsSrc.end(); itSrc++) {
    typename std::map<typename CLASS::EnumType, VDst>::const_iterator itDst = valsDst.find(itSrc->first);
    if (itDst == valsDst.end()) {
      if (ignore_extras)
        continue;
      ERROR_VALUE_SET(context, getName(itSrc->first),
                      " is not present in the destination");
    }
    if ((!ignore_constants) &&
        CLASS::EnumBaseClass::isConstant(itSrc->first) &&
        (!valuesEqual(itDst->second, itSrc->second))) {
      std::string oldval = valueString(itDst->second);
      std::string newval = valueString(itSrc->second);
      CLASS::EnumBaseClass::checkNotConstant(itSrc->first, context +
                                             "[" + oldval + ", " + newval +
                                             "]: ");
    }
  }
  if (valsDst.size() != valsSrc.size() && !ignore_missing) {
    for (typename std::map<typename CLASS::EnumType, VDst>::const_iterator itDst = valsDst.begin();
         itDst != valsDst.end(); itDst++) {
      typename std::map<typename CLASS::EnumType, VSrc>::const_iterator itSrc = valsSrc.find(itDst->first);
      if (itSrc == valsSrc.end()) {
        if (ignore_missing)
          continue;
        ERROR_VALUE_SET(context, getName(itDst->first),
                        " is not present in the source");
      }
    }
  }
}

TEMPLATE
template<typename VSrc, typename VDst>
void CLASS::copy_value_map(std::map<typename CLASS::EnumType, VDst>& valsDst,
                           const std::map<typename CLASS::EnumType, VSrc>& valsSrc,
                           const std::string& context,
                           bool ignore_extras,
                           bool ignore_missing,
                           bool ignore_constants,
                           bool ignore_skipped,
                           bool ignore_initonce) {
  bool check = false;
  if (valsDst.size() != valsSrc.size())
    check = true;
  if (check)
    check_value_maps(valsDst, valsSrc, context + "copy_value_map: ",
                     ignore_extras, ignore_missing, ignore_constants);
  for (typename std::map<typename CLASS::EnumType, VSrc>::const_iterator itSrc = valsSrc.begin();
       itSrc != valsSrc.end(); itSrc++) {
    typename std::map<typename CLASS::EnumType, VDst>::iterator itDst = valsDst.find(itSrc->first);
    if (itDst == valsDst.end()) {
      if (ignore_extras)
        continue;
      ERROR_VALUE_SET(context, getName(itSrc->first),
                      " is not present in the destination");
    }
    if (((!ignore_skipped) && CLASS::EnumBaseClass::isSkipped(itSrc->first)) ||
        ((!ignore_initonce) && CLASS::EnumBaseClass::isInitonce(itSrc->first))) {
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

TEMPLATE
template<typename V>
void CLASS::copy_value_map(std::map<typename CLASS::EnumType, V>& vals,
                           const std::string& filename,
                           const std::string& context,
                           bool ignore_extras,
                           bool ignore_missing,
                           bool ignore_constants,
                           bool ignore_skipped,
                           bool ignore_initonce) {
  std::map<typename CLASS::EnumType, double> new_values = utils::readEnumFile<typename CLASS::EnumType, double>(filename);
  copy_value_map(vals, new_values, context + "[" + filename + "]: ",
                 ignore_extras, ignore_missing,
                 ignore_constants, ignore_skipped,
                 ignore_initonce);
}

TEMPLATE
template<typename VSrc, typename VDst>
void CLASS::update_value_map(std::map<typename CLASS::EnumType, VDst>& valsDst,
                             const std::map<typename CLASS::EnumType, VSrc>& valsSrc,
                             const std::string& context,
                             bool allow_extras,
                             bool no_missing,
                             bool no_constants,
                             bool no_skipped) {
  copy_value_map(valsDst, valsSrc, context,
                 allow_extras, !no_missing,
                 !no_constants, !no_skipped);
}

TEMPLATE
template<typename V>
void CLASS::update_value_map(std::map<typename CLASS::EnumType, V>& vals,
                             const std::string& filename,
                             const std::string& context,
                             bool allow_extras,
                             bool no_missing,
                             bool no_constants,
                             bool no_skipped) {
  copy_value_map(vals, filename, context,
                 allow_extras, !no_missing,
                 !no_constants, !no_skipped);
}
TEMPLATE
template<typename V>
std::map<int, double> CLASS::export_value_map(const std::map<typename CLASS::EnumType, V>& vals,
                                              const std::string& context) {
  std::map<int, double> out;
  for (typename std::map<typename CLASS::EnumType, V>::const_iterator it = vals.begin();
       it != vals.end(); it++) {
    out[key2int(it->first)] = valueGet(it->second);
  }
  return out;
}
*/
TEMPLATE // template<typename V>
void CLASS::import_value_map(std::map<typename CLASS::EnumType, double*>& dst,
                             const std::map<int, double>& src,
                             const std::string& context) {
  std::map<typename CLASS::EnumType, double> ksrc = BaseClass::int2key(src);
  copy_value_map(dst, ksrc, context,
                 false, false, true, true, true);
}

TEMPLATE
void CLASS::import_value_map(std::map<typename CLASS::EnumType, double>& dst,
                             const std::map<int, double>& src,
                             const std::string& context) {
  std::map<typename CLASS::EnumType, double> ksrc = BaseClass::int2key(src);
  dst.clear();
  dst.insert(ksrc.begin(), ksrc.end());
}

/*
TEMPLATE
template<typename V>
void CLASS::init_value_map(std::map<typename CLASS::EnumType, V>& vals,
                           const bool isinit) {
  // checkDefaults("init_value_map: ");
  initDefaults();
  copy_value_map(vals, defaults, "init_value_map: ",
                 true, false, !isinit, true, !isinit);
}
*/

TEMPLATE
void CLASS::checkDefaults(const std::string& context) {
  // initDefaults();
  if (defaults.empty() && !CLASS::EnumBaseClass::defaults.empty())
    ERROR_VALUE_SET(context,
                    "defaults not initialized, call ",
                    "initDefaults first");
}

TEMPLATE
MODULE CLASS::get_module() { return module; }
TEMPLATE
PARAM_TYPE CLASS::get_param_type() { return param_type; }
    
TEMPLATE
const std::vector<PARAM_TYPE>& CLASS::get_parameter_types() {
  static std::vector<PARAM_TYPE> out = {};
  return out;
}
    
TEMPLATE
void CLASS::initDefaults() {
  initDefaults(static_flags & VS_FLAG_DEFAULTS_C3);
}

TEMPLATE
void CLASS::initDefaults(const bool useC3,
                         const std::string& filename,
                         const bool force,
                         const bool noChildren) {
  if (!(static_flags & VS_FLAG_INIT_DEFAULTS)) {
    defaults.insert(CLASS::EnumBaseClass::defaults.begin(),
                    CLASS::EnumBaseClass::defaults.end());
    if (useC3) {
      utils::copyInto(defaults, CLASS::EnumBaseClass::defaults_C3);
      static_flags |= VS_FLAG_DEFAULTS_C3;
    } else {
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
      copy_value_map(defaults, CLASS::EnumBaseClass::defaults_C3,
                     "initDefaults: ",
                     false, true, true, true, true);
      static_flags |= VS_FLAG_DEFAULTS_C3;
    } else {
      copy_value_map(defaults, CLASS::EnumBaseClass::defaults,
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

TEMPLATE
std::string CLASS::getName(const int& k) {
  return getName(int2key(k));
}
TEMPLATE
std::string CLASS::getName(const std::string& k) {
  return k;
}

// TEMPLATE
// template<typename K>
// std::string CLASS::getNameWithPrefix(const K& k) {
//   std::ostringstream out;
//   out << utils::enum_key2string(module) << "::" <<
//     utils::enum_key2string(param_type) << "::" <<
//     getName(k);
//   return std::move(out).str();
// }

TEMPLATE
std::string CLASS::getAliasedName(const std::string& name) {
  typename CLASS::EnumType k = fromNameWithAliases(name);
  return getName(k);
}
    
TEMPLATE
typename CLASS::EnumType CLASS::fromNameWithAliases(const std::string& name,
                                                    bool isGlymaID) {
  if (isGlymaID)
    return CLASS::EnumBaseClass::fromGlymaid(name);
  // TODO: Check for module/param_type prefix
  if (CLASS::EnumBaseClass::aliases.size()) {
    typename std::map<std::string, typename CLASS::EnumType>::const_iterator it = CLASS::EnumBaseClass::aliases.find(name);
    if (it != CLASS::EnumBaseClass::aliases.end())
      return it->second;
  }
  try {
    return CLASS::EnumBaseClass::fromName(name);
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
            return CLASS::EnumBaseClass::fromName(name_alt);
          }
        }
      }
    }
    throw;
  }
}
    
TEMPLATE
int CLASS::fromNameWithAliasesInt(const std::string& name,
                                  bool isGlymaID) {
  return static_cast<int>(fromNameWithAliases(name, isGlymaID));
}

TEMPLATE
bool CLASS::has(const std::string& name,
                const bool& isGlymaID) {
  try {
    if (isGlymaID)
      CLASS::EnumBaseClass::fromGlymaid(name);
    else
      fromNameWithAliases(name);
    return true;
  } catch (...) {
    return false;
  }
}

TEMPLATE
bool CLASS::hasGlymaID(const std::string& name) {
  try {
    CLASS::EnumBaseClass::fromGlymaid(name);
    return true;
  } catch (...) {
    return false;
  }
}

TEMPLATE
void CLASS::setDefault(const typename CLASS::EnumType& k, const double& v,
                       const bool dontPreserve) {
  checkDefaults("setDefault: ");
  defaults[k] = v;
  if (!dontPreserve)
    static_flags |= VS_FLAG_DEFAULTS_EDITED;
}

TEMPLATE
void CLASS::setDefault(const std::string& k, const double& v,
                       const bool& isGlymaID,
                       const bool dontPreserve) {
  if (isGlymaID)
    return setDefault(CLASS::EnumBaseClass::fromGlymaid(k),
                      v, dontPreserve);
  return setDefault(fromNameWithAliases(k), v, dontPreserve);
}

TEMPLATE
void CLASS::setDefault(const int& k, const double& v,
                       const bool dontPreserve) {
  return setDefault(int2key(k), v, dontPreserve);
}

TEMPLATE
double CLASS::getDefault(const typename CLASS::EnumType& x) {
  checkDefaults("getDefault: ");
  typename std::map<typename CLASS::EnumType, double>::const_iterator it;
  it = defaults.find(x);
  if (it == defaults.end()) {
    ERROR_VALUE_SET("Could not locate Default for '", x, "'");
  }
  return it->second;
}

TEMPLATE
double CLASS::getDefault(const std::string& x,
                         const bool& isGlymaID) {
  if (isGlymaID)
    return getDefault(CLASS::EnumBaseClass::fromGlymaid(x));
  return getDefault(fromNameWithAliases(x));
}

TEMPLATE
double CLASS::getDefault(const int& k) {
  return getDefault(int2key(k));
}

TEMPLATE
double CLASS::getDefault(const typename CLASS::EnumType& x, const double& defaultV) {
  checkDefaults("getDefault[optional]: ");
  typename std::map<typename CLASS::EnumType, double>::const_iterator it;
  it = defaults.find(x);
  if (it == defaults.end()) {
    return defaultV;
  }
  return it->second;
}

TEMPLATE
std::string CLASS::getDocs(const std::string& name,
                           const bool& isGlymaID) {
  if (isGlymaID)
    return CLASS::EnumBaseClass::getDocs(CLASS::EnumBaseClass::fromGlymaid(name), "");
  return CLASS::EnumBaseClass::getDocs(fromNameWithAliases(name), "");
}

TEMPLATE
std::string CLASS::getDocs(const int& k) {
  return CLASS::EnumBaseClass::getDocs(BaseClass::int2key(k), "");
}

/*
TEMPLATE
template<typename V>
typename CLASS::ValueType CLASS::get_value(std::map<typename CLASS::EnumType, V>& vals,
                                           const typename CLASS::EnumType k,
                                           const std::string& context) {
  check_value_map(vals, k, context);
  return ValueType(k, vals.find(k)->second,
                   context + "get_value[V]: "
                   + getName(k));
}
*/

TEMPLATE
const double& CLASS::get_value_const(const std::map<typename CLASS::EnumType, double>& vals,
                                     const typename CLASS::EnumType k,
                                     const std::string& context) {
  check_value_map(vals, k, context);
  DEBUG_VALUE_SET(context, "[CONST DOUBLE]: ", k, " = ",
                  vals.find(k)->second);
  return vals.find(k)->second;
}

TEMPLATE
const double& CLASS::get_value_const(const std::map<typename CLASS::EnumType, double*>& vals,
                                     const typename CLASS::EnumType k,
                                     const std::string& context) {
  check_value_map(vals, k, context);
  DEBUG_VALUE_SET(context, "[CONST DOUBLE*]: ", k, " = ",
                  *(vals.find(k)->second));
  return *(vals.find(k)->second);
}
    
TEMPLATE
const double& CLASS::get_value_const(const std::map<typename CLASS::EnumType, typename CLASS::ValueType>& vals,
                                     const typename CLASS::EnumType k,
                                     const std::string& context) {
  check_value_map(vals, k, context);
  DEBUG_VALUE_SET(context, "[CONST VALUE]: ", k, " = ",
                  vals.find(k)->second.get());
  return vals.find(k)->second.getRef();
}
    
TEMPLATE
void CLASS::set_value(std::map<typename CLASS::EnumType, double>& vals,
                      const typename CLASS::EnumType k, const double v,
                      const std::string& context) {
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
  CLASS::EnumBaseClass::checkNotConstant(k, context);
  vals[k] = v;
#ifdef VERBOSE_VALUE_SET_DEBUG
  DEBUG_VALUE_SET_END(" [AFTER = ", vals[k], "]");
#endif // VERBOSE_VALUE_SET_DEBUG
}
    
TEMPLATE void CLASS::set_value(std::map<typename CLASS::EnumType, double*>& vals,
                               const typename CLASS::EnumType k, const double v,
                               const std::string& context) {
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

TEMPLATE
void CLASS::set_value(std::map<typename CLASS::EnumType, typename CLASS::ValueType>& vals,
                      const typename CLASS::EnumType k, const double v,
                      const std::string& context) {
  DEBUG_VALUE_SET_NOEND(context, "[VALUE] ", k, "[",
                        std::to_string(k), "]: ");
  typename std::map<typename CLASS::EnumType, typename CLASS::ValueType>::iterator it = vals.find(k);
#ifdef VERBOSE_VALUE_SET_DEBUG
  if (it == vals.end()) {
    DEBUG_VALUE_SET_RAW("[NULL] 0");
  } else {
    DEBUG_VALUE_SET_RAW("[", it->second.getPtr(), "] ", it->second.get());
  }
  DEBUG_VALUE_SET_RAW(" -> ", v);
#endif // VERBOSE_VALUE_SET_DEBUG
  check_value_map(vals, k, context);
  it->second.set(v);
  DEBUG_VALUE_SET_END(" [AFTER = ", it->second.get(), "]");
}

/*
TEMPLATE
template<typename V>
void CLASS::set_value_from_EnzymeAct(std::map<typename CLASS::EnumType, V>& vals,
                                     const typename CLASS::EnumType k,
                                     const std::map<std::string, double>& EnzymeAct) {
  set_value(vals, k, EnzymeAct.at(getName(k)),
            "set_value_from_EnzymeAct: ");
}
*/
    
TEMPLATE double*& CLASS::get_value_orig(std::map<typename CLASS::EnumType, double*>& vals,
                                        const typename CLASS::EnumType k,
                                        const std::string& context) {
  CLASS::EnumBaseClass::checkNotConstant(k, context);
  check_value_map(vals, k, context);
  DEBUG_VALUE_SET(context, k);
  return vals.find(k)->second;
}
    
TEMPLATE
void CLASS::insert_value_orig(std::map<typename CLASS::EnumType, double*>& vals,
                              const typename CLASS::EnumType k, double* v,
                              const std::string& context) {
  if (vals.find(k) == vals.end()) {
    vals.emplace(k, &(v[0]));
    if (CLASS::EnumBaseClass::isInitonce(k)) {
      // TODO: this check may not be necessary if the init once
      //   variables are reinitialized after a reset
      typename std::map<typename CLASS::EnumType, double>::const_iterator it_C3 = CLASS::EnumBaseClass::defaults_C3.find(k);
      if (it_C3 != CLASS::EnumBaseClass::defaults_C3.end())
        compareValues(k, CLASS::EnumBaseClass::defaults.find(k)->second,
                      it_C3->second);
      checkDefaults("insert_value_orig: ");
      v[0] = defaults.find(k)->second;
      DEBUG_VALUE_SET(context, "initialized ", k, " = ",
                      vals[k], " [", v, "]");
    }
    DEBUG_VALUE_SET(context, "inserted ", k, " = ",
                    vals[k], " [", v, "]");
  }
}

TEMPLATE
void CLASS::set_value_orig(std::map<typename CLASS::EnumType, double*>& vals,
                           const typename CLASS::EnumType k, double* v) {
  get_value_orig(vals, k, "set_orig") = v;
}

/*
TEMPLATE
template<typename V>
void CLASS::reset_value_map(std::map<typename CLASS::EnumType, V>& vals) {
  DEBUG_VALUE_SET_NOARGS;
  for (typename std::map<typename CLASS::EnumType, V>::iterator it = vals.begin();
       it != vals.end(); it++) {
    if (!CLASS::EnumBaseClass::isConstant(it->first)) {
      if (CLASS::EnumBaseClass::isResetone(it->first)) {
        set_value(vals, it->first, 1.0, "reset_value_map: ");
      } else {
        set_value(vals, it->first, 0.0, "reset_value_map: ");
      }
    }
  }
}

TEMPLATE
template<typename V>
void CLASS::value_map_toArray(const std::map<typename CLASS::EnumType, V>& vals,
                              arr& out) {
  size_t i = 0;
  out.resize(out.size() + vals.size());
  for (typename std::map<typename CLASS::EnumType, V>::const_iterator it = vals.begin();
       it != vals.end(); it++) {
    if (inArrays(it->first)) {
      CLASS::EnumBaseClass::checkNotConstant(it->first);
      out[i] = get_value_const(vals, it->first,
                               "value_map_toArray: ");
      i++;
    }
  }
}

TEMPLATE
template<typename V>
void CLASS::value_map_fromArray(std::map<typename CLASS::EnumType, V>& vals,
                                const arr &vec,
                                const std::size_t offset) {
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
  for (typename std::map<typename CLASS::EnumType, V>::iterator it = vals.begin();
       it != vals.end(); it++) {
    if (inArrays(it->first)) {
      // CLASS::EnumBaseClass::checkNotConstant(it->first, "value_map_fromArray: ");
      set_value(vals, it->first, vec[offset + i],
                "value_map_fromArray: ");
      i++;
    }
  }
}
*/
    
TEMPLATE std::size_t CLASS::memberCount(const bool noChildren) {
  std::size_t out = CLASS::EnumBaseClass::defaults.size() - (CLASS::EnumBaseClass::countSkipped() + CLASS::EnumBaseClass::countNonvector());
#define DO_CHILD(child, dummy) out += child->memberCount()
  DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  return out;
}

TEMPLATE std::string CLASS::memberState(const bool noChildren) {
  std::string out = CLASS::error_prefix() +
    ", NCHILD = " + std::to_string(child_classes.size()) +
    ", COUNT = " + std::to_string(memberCount()) +
    ", SKIPPED = " + utils::to_string(CLASS::EnumBaseClass::listSkipped()) +
    ", NONVECT = " + utils::to_string(CLASS::EnumBaseClass::listNonvector());
  if (noChildren)
    return out;
  for (std::vector<ValueSetClassWrapper>::const_iterator it = child_classes.begin();
       it != child_classes.end(); it++)
    out += "\n\t" + (*it)->memberState();
  return out;
}

/*
TEMPLATE
template<typename C>
void CLASS::addChildClass() {
  child_classes.emplace_back(new ValueSetClassType<C>());
}
*/

TEMPLATE void CLASS::initChildClasses(const bool noChildren) {
  if (static_flags & VS_FLAG_INIT_CHILDREN) return;
  T::_initChildClasses();
  DO_VALUE_SET_CHILD_CLASSES(initChildClasses, ());
  static_flags |= VS_FLAG_INIT_CHILDREN;
}

TEMPLATE void CLASS::initStaticMembers(const bool noChildren) {
  if (static_flags & VS_FLAG_INIT_STATIC) return;
  T::_initStaticMembers();
  initChildClasses(true); // Children will be called in initStaticMembers
  DO_VALUE_SET_CHILD_CLASSES(initStaticMembers, ());
  static_flags |= VS_FLAG_INIT_STATIC;
}

TEMPLATE bool CLASS::selected() {
  return (static_flags & VS_FLAG_SELECTED);
}

TEMPLATE void CLASS::select(const bool x, const bool noChildren) {
  if (x) {
    static_flags |= VS_FLAG_SELECTED;
  } else {
    static_flags &= ~VS_FLAG_SELECTED;
  }
  T::_select(x, noChildren);
  DO_VALUE_SET_CHILD_CLASSES(select, (x));
}

TEMPLATE void CLASS::checkSelected() {
  if (!(static_flags & VS_FLAG_SELECTED))
    ERROR_VALUE_SET("Value set has not been selected");
}

TEMPLATE bool CLASS::hasChildren() {
  return (child_classes.size() > 0);
}
TEMPLATE bool CLASS::usesC3() {
  return (static_flags & VS_FLAG_DEFAULTS_C3);
}
TEMPLATE void CLASS::enableC3(const bool x,
                       const bool noChildren) {
  initDefaults(x, "", true);
  if (x)
    static_flags |= VS_FLAG_DEFAULTS_C3;
  else
    static_flags &= ~VS_FLAG_DEFAULTS_C3;
  T::_enableC3(x, noChildren);
  DO_VALUE_SET_CHILD_CLASSES(enableC3, (x));
}

TEMPLATE void CLASS::reset(const bool noChildren) {
  int flags0 = static_flags;
  CLASS::EnumBaseClass::clearSkipped();
  T::_reset(noChildren);
  static_flags = 0;
  if (flags0 & VS_FLAG_INIT_CHILDREN)
    static_flags |= VS_FLAG_INIT_CHILDREN;
  if (flags0 & VS_FLAG_INIT_POINTERS)
    static_flags |= VS_FLAG_INIT_POINTERS;
  defaults.clear();
  DO_VALUE_SET_CHILD_CLASSES(reset, ());
}
TEMPLATE std::size_t CLASS::size() {
  return T::_size();
}

#undef TEMPLATE
#undef CLASS


//////////////////////////////////////////////////////////
// ValueSet
//////////////////////////////////////////////////////////

#define TEMPLATE template<class T, class U, MODULE ID, PARAM_TYPE PT>
#define CLASS ValueSet<T, U, ID, PT>

TEMPLATE
CLASS::ValueSet() : values() {}
TEMPLATE
CLASS::ValueSet(const CLASS& other) : ValueSet() {
  copyValueSet(other);
}
TEMPLATE
CLASS::~ValueSet() {}
TEMPLATE
void CLASS::copyValueSet(const CLASS& other) {
  int flags0 = flags;
  flags = other.flags;
  preinit_values = other.preinit_values;
  if (!(flags0 & BaseClass::VS_FLAG_INIT_POINTERS))
    flags &= ~BaseClass::VS_FLAG_INIT_POINTERS;
  if (!(flags0 & BaseClass::VS_FLAG_INIT_CHILDREN))
    flags &= ~BaseClass::VS_FLAG_INIT_CHILDREN;
}
TEMPLATE
void CLASS::copyInstance(const T& other, const bool noChildren) {
  initMembers();
  static_cast<T*>(this)->_copyMembers(other);
  copyChildren(other, noChildren);
}
TEMPLATE
void CLASS::copyChildren(const T& other, const bool noChildren) {
  if (children.size() != other.children.size()) {
    ERROR_VALUE_SET("copyMembers: "
                    "This instance has ", children.size(),
                    " children, but the one being copied has ",
                    other.children.size());
  }
  DO_VALUE_SET_CHILDREN(copyMembers, (**(other.children[iChild])));
}
TEMPLATE
T& CLASS::operator=(const T& other) {
  copyMembers(other);
  return *dynamic_cast<T*>(this);
}
TEMPLATE
CLASS::ValueSet(const arr &vec, const std::size_t offset) : ValueSet() {}

TEMPLATE
const ValueSet_t* CLASS::getValueSet() const {
  return this;
}

TEMPLATE
std::map<int, double> CLASS::getValueMap(const bool preinit) const {
  if (preinit)
    return BaseClass::export_value_map(preinit_values, CONTEXT_VALUE_SET);
  return BaseClass::export_value_map(values, CONTEXT_VALUE_SET);
}
TEMPLATE
void CLASS::setValueMap(const std::map<int, double>& map,
                        const bool setinit,
                        const bool preinit) {
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

TEMPLATE
void CLASS::copyMembers(const ValueSet_t& other0,
                        const bool noChildren) {
  const T& other = castValueSet<T>(other0);
  copyValueSet(other);
  // TODO: Re-enable this only if more efficient than direct copy
  // BaseClass::copy_value_map(values, other.values, "copyMembers[vals]: ", true);
  copyInstance(other, noChildren);
}
TEMPLATE
void CLASS::initMembers(const bool noChildren) {
  ParentClass::initStaticMembers(true);
  initMemberPointers(true);
  static_cast<T*>(this)->_initMembers();
  initChildren(true);
  DO_VALUE_SET_CHILDREN(initMembers, ());
}
TEMPLATE
void CLASS::resetValues(const bool noChildren) {
  ENSURE_VALUE_POINTERS;
  BaseClass::reset_value_map(values);
  static_cast<T*>(this)->_resetValues();
  DO_VALUE_SET_CHILDREN(resetValues, ());
  flags &= ~BaseClass::VS_FLAG_INIT_VALUES;
}
TEMPLATE
bool CLASS::initialized() const {
  return (flags & BaseClass::VS_FLAG_INIT_VALUES);
}
TEMPLATE
void CLASS::initValues(const bool noDefaults,
                       const bool force,
                       const bool noChildren) {
  if ((!force) && initialized()) return;
  initMembers(true);
  if (!noDefaults)
    BaseClass::init_value_map(values, initialized());
  static_cast<T*>(this)->_initValues();
  DO_VALUE_SET_CHILDREN(initValues, (noDefaults, force));
  if (!preinit_values.empty()) {
    BaseClass::copy_value_map(values, preinit_values, "initValues: ",
                              false, true, true, false, true);
  }
  flags |= BaseClass::VS_FLAG_INIT_VALUES;
  if (BaseClass::usesC3())
    flags |= BaseClass::VS_FLAG_DEFAULTS_C3;
  else
    flags &= ~BaseClass::VS_FLAG_DEFAULTS_C3;
}
TEMPLATE
void CLASS::resetInstance(const bool noChildren) {
  T::reset(true);
  int preserve_flags = 0;
  if (flags & BaseClass::VS_FLAG_INIT_POINTERS)
    preserve_flags |= BaseClass::VS_FLAG_INIT_POINTERS;
  if (flags & BaseClass::VS_FLAG_INIT_CHILDREN)
    preserve_flags |= BaseClass::VS_FLAG_INIT_CHILDREN;
  flags = preserve_flags;
  DO_VALUE_SET_CHILDREN(resetInstance, ());
}
TEMPLATE
void CLASS::initMemberPointers(const bool noChildren) {
  if (flags & BaseClass::VS_FLAG_INIT_POINTERS) return;
  static_cast<T*>(this)->_initMemberPointers();
  DO_VALUE_SET_CHILDREN(initMemberPointers, ());
  flags |= BaseClass::VS_FLAG_INIT_POINTERS;
}
TEMPLATE
void CLASS::initChildren(const bool noChildren) {
  if (flags & BaseClass::VS_FLAG_INIT_CHILDREN) return;
  static_cast<T*>(this)->_initChildren();
  DO_VALUE_SET_CHILDREN(initChildren, ());
  flags |= BaseClass::VS_FLAG_INIT_CHILDREN;
}
TEMPLATE
void CLASS::createChildren(const bool noChildren) {
  BaseClass::initChildClasses(true);
  initChildren(true);
  static_cast<T*>(this)->_createChildren();
  // if (child_classes.size() != children.size())
  //        ERROR_VALUE_SET("Number of child classes (",
  //                        child_classes.size(),
  // 			") does not match the number of child pointers (",
  //                        children.size(), ")");
  // std::vector<ValueSetClassWrapper>::iterator itCls = child_classes.begin();
  // for (std::vector<ValueSet_t**>::iterator it = children.begin();
  // 	   it != children.end(); it++, itCls++) {
  // 	if (**it) continue;
  // 	(*it)[0] = (*itCls)->newInstance(static_cast<ValueSet_t*>(this));
  // 	// (*it)[0]->setParent(static_cast<ValueSet_t*>(this));
  // }
  DO_VALUE_SET_CHILDREN(createChildren, ());
}
TEMPLATE
const std::vector<ValueSet_t**>& CLASS::getChildren() const {
  return children;
}
TEMPLATE
ValueSet_t* CLASS::newInstance(ValueSet_t* par) {
  return T::_newInstance(par);
}
TEMPLATE
void CLASS::setParent(U* par) {
  UNUSED(par);
}
TEMPLATE
void CLASS::setParent(ValueSet_t*) {
  // if (typeid(*par) != typeid(U))
  //   ERROR_VALUE_SET("Type IDs do not match");
  // static_cast<T*>(this)->setParent(static_cast<U*>(par));
}

TEMPLATE
std::map<std::string, double*> CLASS::getPointerMap() {
  initMemberPointers();
  return BaseClass::get_pointer_map(values);
}

TEMPLATE
typename CLASS::iterator CLASS::begin() {
  ENSURE_VALUE_POINTERS;
  return values.begin();
}
TEMPLATE
typename CLASS::iterator CLASS::end() {
  ENSURE_VALUE_POINTERS;
  return values.end();
}
TEMPLATE
typename CLASS::const_iterator CLASS::begin() const {
  ENSURE_VALUE_POINTERS_CONST;
  return values.begin();
}
TEMPLATE
typename CLASS::const_iterator CLASS::end() const {
  ENSURE_VALUE_POINTERS_CONST;
  return values.end();
}
TEMPLATE
std::size_t CLASS::max_value_width(bool noChildren) const {
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
TEMPLATE
std::string CLASS::diff(const ValueSet_t& other,
                        std::size_t padKeys,
                        std::size_t padVals,
                        bool includePrefixes,
                        bool noChildren) const {
  if (padKeys == 0) {
    padKeys = BaseClass::max_field_width(noChildren);
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
TEMPLATE
std::ostream& CLASS::print(std::ostream &out,
                           const uint tab, std::size_t pad,
                           bool includePrefixes,
                           bool includeSkipped,
                           const std::vector<std::string>& skip_keys,
                           const std::map<std::string, std::string>& key_aliases,
                           bool noChildren) const {
  if (pad == 0) pad = BaseClass::max_field_width(noChildren);
  return static_cast<const T*>(this)->_print(out, tab, pad,
                                             includePrefixes,
                                             includeSkipped,
                                             skip_keys,
                                             key_aliases,
                                             noChildren);
}
/*
TEMPLATE
std::ostream& operator<<(std::ostream& out, const CLASS& x) {
  return x.print(out);
}
TEMPLATE
std::ostream& operator<<(std::ostream& out, const CLASS* x) {
  if (!x) {
    out << "0x0" << std::endl;
    return out;
  }
  return x->print(out);
}
*/
TEMPLATE
bool CLASS::equals(const ValueSet_t& b0,
                   const bool noChildren) const {
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
TEMPLATE
bool CLASS::operator==(const T& other) const {
  return static_cast<const T*>(this)->equals(static_cast<const ValueSet_t&>(other));
}
TEMPLATE
bool CLASS::operator!=(const T& other) const {
  return !(operator==(other));
}
TEMPLATE
void CLASS::check(const EnumType k, const std::string& context) const {
  ENSURE_VALUE_POINTERS_CONST;
  BaseClass::check_value_map(values, k, context);
}
TEMPLATE
void CLASS::update_values(const std::map<EnumType, double>& new_values,
                          const std::string& context,
                          bool allow_extras,
                          bool no_missing,
                          bool no_constants,
                          bool no_skipped) {
  ENSURE_VALUE_POINTERS;
  BaseClass::update_value_map(values, new_values,
                              context + "update_values: ",
                              allow_extras, no_missing,
                              no_constants, no_skipped);
}
TEMPLATE
void CLASS::update_values(const std::string& filename,
                          const std::string& context,
                          bool allow_extras,
                          bool no_missing,
                          bool no_constants,
                          bool no_skipped) {
  ENSURE_VALUE_POINTERS;
  BaseClass::update_value_map(values, filename,
                              context + "update_values[" + filename + "]: ",
                              allow_extras, no_missing,
                              no_constants, no_skipped);
}
TEMPLATE
typename CLASS::ValueType CLASS::operator[](const typename CLASS::EnumType k) {
  ENSURE_VALUE_POINTERS;
  return BaseClass::get_value(values, k);
}

TEMPLATE
const double& CLASS::operator[](const typename CLASS::EnumType k) const {
  ENSURE_VALUE_POINTERS_CONST;
  return BaseClass::get_value_const(values, k);
}
TEMPLATE
void CLASS::setPreInit(const typename CLASS::EnumType& k,
                       const double& v) {
  if (initialized()) return;
  // INFO_VALUE_SET(k, " = ", v);
  preinit_values[k] = v;
}
TEMPLATE
void CLASS::set(const typename CLASS::EnumType& k, const double& v) {
  ENSURE_VALUE_POINTERS;
  setPreInit(k, v);
  BaseClass::set_value(values, k, v);
}
TEMPLATE
void CLASS::set(const std::string& name, const double& v,
                const bool& isGlymaID) {
  if (isGlymaID)
    return set(EnumBaseClass::fromGlymaid(name), v);
  return set(BaseClass::fromNameWithAliases(name), v);
}
TEMPLATE
void CLASS::set(const int& k, const double& v) {
  return set(BaseClass::int2key(k), v);
}
TEMPLATE
void CLASS::setFromEnzymeAct(const typename CLASS::EnumType k,
                             const std::map<std::string, double>& EnzymeAct) {
  ENSURE_VALUE_POINTERS;
  BaseClass::set_value_from_EnzymeAct(values, k, EnzymeAct);
}
TEMPLATE
typename CLASS::ValueType CLASS::get(const typename CLASS::EnumType& k) {
  ENSURE_VALUE_POINTERS;
  return BaseClass::get_value(values, k);
}
TEMPLATE
double CLASS::get(const typename CLASS::EnumType& k) const {
  ENSURE_VALUE_POINTERS_CONST;
  return BaseClass::get_value_const(values, k);
}
TEMPLATE
double CLASS::get(const std::string& name,
                  const bool& isGlymaID) const {
  if (isGlymaID)
    return get(EnumBaseClass::fromGlymaid(name));
  return get(BaseClass::fromNameWithAliases(name));
}
TEMPLATE
double CLASS::get(const int& k) const {
  return get(BaseClass::int2key(k));
}
TEMPLATE
void CLASS::insertOrig(const typename CLASS::EnumType k, double* v,
                       const std::string& context) {
  if (k == EnumClass::NONE || k == EnumClass::MAX)
    return;
  BaseClass::insert_value_orig(values, k, v, context);
}
TEMPLATE
std::size_t CLASS::sizeArray() const {
  // TODO: include children here instead of in overridden method?
  return static_cast<const T*>(this)->_sizeArray();
}
TEMPLATE
arr CLASS::toArray() const {
  // TODO: include children here instead of in overridden method?
  return static_cast<const T*>(this)->_toArray();
}
TEMPLATE
void CLASS::fromArray(const arr &vec, const std::size_t offset) {
  // TODO: include children here instead of in overridden method?
  static_cast<T*>(this)->_fromArray(vec, offset);
}
TEMPLATE
void CLASS::fromArray(realtype *x) {
  arr vec(BaseClass::size());
  for (std::size_t i = 0; i < BaseClass::size(); i++)
    vec[i] = x[i];
  fromArray(vec);
}

TEMPLATE
void CLASS::addChild(ValueSet_t** C) {
  children.push_back(C);
}

TEMPLATE
std::string CLASS::_diff(const T& other,
                         std::size_t padKeys,
                         std::size_t padVals,
                         bool includePrefixes,
                         bool noChildren) const {
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
TEMPLATE
std::ostream& CLASS::_print(std::ostream &out,
                            const uint tab, std::size_t pad,
                            bool includePrefixes,
                            bool includeSkipped,
                            const std::vector<std::string>& skip_keys,
                            const std::map<std::string, std::string>& key_aliases,
                            bool noChildren) const {
  ENSURE_VALUE_POINTERS_CONST;
  uint child_tab = tab;
  if (!(noChildren || includePrefixes)) {
    const std::string space = BaseClass::getTabs(tab);
    out << space << error_prefix() << std::endl;
    child_tab++;
  }
#define DO_CHILD(child, dummy)						\
  child->print(out, child_tab, pad, includePrefixes, includeSkipped,    \
               skip_keys, key_aliases)
  DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  return BaseClass::print_value_map(values, out, child_tab, pad,
                                    includePrefixes, includeSkipped,
                                    skip_keys, key_aliases);
}
TEMPLATE
std::size_t CLASS::_sizeArray() const {
  ENSURE_VALUE_POINTERS_CONST;
  bool noChildren = false;
  std::size_t out = values.size();
#define DO_CHILD(child, dummy) out += child->sizeArray()
  DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  return out;
}
TEMPLATE
arr CLASS::_toArray() const {
  ENSURE_VALUE_POINTERS_CONST;
  bool noChildren = false;
  arr out;
#define DO_CHILD(child, dummy)						\
  arr ivec = child->toArray();                                          \
  out.insert(out.end(), ivec.begin(), ivec.end())
  DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  BaseClass::value_map_toArray(values, out);
  return out;
}
TEMPLATE
void CLASS::_fromArray(const arr &vec, const std::size_t offset) {
  ENSURE_VALUE_POINTERS;
  ENSURE_CHILD_POINTERS;
  bool noChildren = false;
  std::size_t ioffset = offset;
#define DO_CHILD(child, dummy)						\
  child->fromArray(vec, ioffset);					\
  ioffset += child->sizeArray()
  DO_VALUE_SET_CHILDREN_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
  BaseClass::value_map_fromArray(values, vec, ioffset);
}

#undef TEMPLATE
#undef CLASS


//////////////////////////////////////////////////////////
// ValueSetStatic
//////////////////////////////////////////////////////////

#define TEMPLATE template<class T, class U, MODULE ID, PARAM_TYPE PT>
#define CLASS ValueSetStatic<T, U, ID, PT>

TEMPLATE const ValueSet_t* CLASS::getValueSet() {
  return BaseClass::getValueSetClass();
}
    
TEMPLATE std::map<int, double> CLASS::getValueMap(const bool preinit) {
  if (preinit)
    return BaseClass::export_value_map(preinit_values, CONTEXT_VALUE_SET);
  return BaseClass::export_value_map(values, CONTEXT_VALUE_SET);
}
TEMPLATE void CLASS::setValueMap(const std::map<int, double>& map,
                               const bool setinit,
                               const bool preinit) {
  if (preinit) {
    if (setinit)
      ERROR_VALUE_SET("preinit and setinit cannot both be true");
    BaseClass::import_value_map(preinit_values, map, CONTEXT_VALUE_SET);
    return;
  }
  BaseClass::import_value_map(values, map, CONTEXT_VALUE_SET);
  if (setinit)
    BaseClass::static_flags |= BaseClass::VS_FLAG_INIT_VALUES;
}
    
TEMPLATE void CLASS::initMembers(const bool noChildren) {
  initStaticMembers(true);
  // initMemberPointers(true); // Called by initStaticMembers
  T::_initMembers();
  DO_VALUE_SET_CHILD_CLASSES(initMembers, ());
}
TEMPLATE void CLASS::resetValues(const bool noChildren) {
  ENSURE_VALUE_POINTERS;
  BaseClass::reset_value_map(values);
  T::_resetValues();
  DO_VALUE_SET_CHILD_CLASSES(resetValues, ());
  BaseClass::static_flags &= ~BaseClass::VS_FLAG_INIT_VALUES;
}
TEMPLATE bool CLASS::initialized() {
  return (BaseClass::static_flags & BaseClass::VS_FLAG_INIT_VALUES);
}
TEMPLATE void CLASS::initValues(const bool noDefaults,
                              const bool force,
                              const bool noChildren) {
  if ((!force) && initialized()) return;
  initMembers(true);
  if (!noDefaults)
    BaseClass::init_value_map(values, initialized());
  T::_initValues();
  // if (!noChildren) {
  //   ERROR_VALUE_SET("recursion for child classes disabled.");
  // }
  DO_VALUE_SET_CHILD_CLASSES(initValues, (noDefaults, force));
  if (!preinit_values.empty()) {
    BaseClass::copy_value_map(values, preinit_values, "initValues: ",
                              false, true, true, false, true);
  }
  BaseClass::static_flags |= BaseClass::VS_FLAG_INIT_VALUES;
}

TEMPLATE void CLASS::initMemberPointers(const bool noChildren) {
  if (BaseClass::static_flags & BaseClass::VS_FLAG_INIT_POINTERS)
    return;
  BaseClass::initDefaults();
  T::_initMemberPointers();
  DO_VALUE_SET_CHILD_CLASSES(initMemberPointers, ());
  BaseClass::static_flags |= BaseClass::VS_FLAG_INIT_POINTERS;
}
TEMPLATE void CLASS::initStaticMembers(const bool noChildren) {
  initMemberPointers(true);
  ParentClass::initStaticMembers(noChildren);
}

TEMPLATE ValueSet_t* CLASS::newInstance(ValueSet_t* par) {
  UNUSED(par);
  ERROR_VALUE_SET("Cannot create instances of ValueSetStatic subclasses");
  return nullptr;
}
      
TEMPLATE std::map<std::string, double*> CLASS::getPointerMap() {
  return BaseClass::get_pointer_map(values);
}

TEMPLATE typename CLASS::iterator CLASS::begin() {
  ENSURE_VALUE_POINTERS;
  return values.begin();
}
TEMPLATE typename CLASS::iterator CLASS::end() {
  ENSURE_VALUE_POINTERS;
  return values.end();
}
TEMPLATE std::size_t CLASS::max_value_width(bool noChildren) {
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
    iout = child->max_value_width();                                    \
    if (iout > out) out = iout
    DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  }
  return out;
}
TEMPLATE std::string CLASS::diff(const ValueSet_t& other,
                               std::size_t padKeys,
                               std::size_t padVals,
                               bool includePrefixes,
                               bool noChildren) {
  UNUSED(other);
  if (padKeys == 0) {
    padKeys = BaseClass::max_field_width(noChildren);
  }
  if (padVals == 0) {
    padVals = max_value_width(noChildren);
  }
  return T::_diff(padKeys, padVals, includePrefixes, noChildren);
}
TEMPLATE std::ostream& CLASS::print(std::ostream &out,
                                  const uint tab, std::size_t pad,
                                  bool includePrefixes,
                                  bool includeSkipped,
                                  const std::vector<std::string>& skip_keys,
                                  const std::map<std::string, std::string>& key_aliases,
                                  bool noChildren) {
  if (pad == 0) pad = BaseClass::max_field_width(noChildren);
  return T::_print(out, tab, pad, includePrefixes, includeSkipped,
                   skip_keys, key_aliases, noChildren);
}
TEMPLATE void CLASS::check(const typename CLASS::EnumType k, const std::string& context) {
  ENSURE_VALUE_POINTERS;
  BaseClass::check_value_map(values, k, context);
}
TEMPLATE void CLASS::update_values(const std::map<typename CLASS::EnumType, double>& new_values,
                                 const std::string& context,
                                 bool allow_extras,
                                 bool no_missing,
                                 bool no_constants,
                                 bool no_skipped) {
  ENSURE_VALUE_POINTERS;
  BaseClass::update_value_map(values, new_values,
                              context + "update_values: ",
                              allow_extras, no_missing,
                              no_constants, no_skipped);
}
TEMPLATE void CLASS::update_values(const std::string& filename,
                                 const std::string& context,
                                 bool allow_extras,
                                 bool no_missing,
                                 bool no_constants,
                                 bool no_skipped) {
  ENSURE_VALUE_POINTERS;
  BaseClass::update_value_map(values, filename,
                              context + "update_values[" + filename + "]: ",
                              allow_extras, no_missing,
                              no_constants, no_skipped);
}
TEMPLATE void CLASS::setPreInit(const typename CLASS::EnumType& k, const double& v) {
  if (initialized()) return;
  // INFO_VALUE_SET(k, " = ", v);
  preinit_values[k] = v;
}
TEMPLATE void CLASS::set(const typename CLASS::EnumType& k, const double& v) {
  ENSURE_VALUE_POINTERS;
  setPreInit(k, v);
  BaseClass::set_value(values, k, v);
}
TEMPLATE void CLASS::set(const std::string& name, const double& v,
                       const bool& isGlymaID) {
  if (isGlymaID)
    return set(EnumBaseClass::fromGlymaid(name), v);
  return set(BaseClass::fromNameWithAliases(name), v);
}
TEMPLATE void CLASS::set(const int& k, const double& v) {
  return set(BaseClass::int2key(k), v);
}
TEMPLATE void CLASS::setFromEnzymeAct(const typename CLASS::EnumType k,
                                    const std::map<std::string, double>& EnzymeAct) {
  ENSURE_VALUE_POINTERS;
  BaseClass::set_value_from_EnzymeAct(values, k, EnzymeAct);
}
TEMPLATE double CLASS::get(const typename CLASS::EnumType& k) {
  ENSURE_VALUE_POINTERS;
  return BaseClass::get_value_const(values, k);
}
TEMPLATE double CLASS::get(const std::string& name,
                         const bool& isGlymaID) {
  if (isGlymaID)
    return get(EnumBaseClass::fromGlymaid(name));
  return get(BaseClass::fromNameWithAliases(name));
}
TEMPLATE double CLASS::get(const int& k) {
  return get(BaseClass::int2key(k));
}
TEMPLATE void CLASS::insertOrig(const typename CLASS::EnumType k, double* v,
                              const std::string& context) {
  if (k == EnumClass::NONE || k == EnumClass::MAX)
    return;
  BaseClass::insert_value_orig(values, k, v, context);
}
TEMPLATE void CLASS::reset(const bool noChildren) {
  ENSURE_VALUE_POINTERS;
  BaseClass::reset_value_map(values);
  ParentClass::reset(noChildren);
}
TEMPLATE std::size_t CLASS::sizeArray() {
  return T::_sizeArray();
}
TEMPLATE arr CLASS::toArray() {
  return T::_toArray();
}
TEMPLATE void CLASS::fromArray(const arr &vec, const std::size_t offset) {
  T::_fromArray(vec, offset);
}
    
TEMPLATE std::ostream& CLASS::_print(std::ostream &out,
                                   const uint tab, std::size_t pad,
                                   bool includePrefixes,
                                   bool includeSkipped,
                                   const std::vector<std::string>& skip_keys,
                                   const std::map<std::string, std::string>& key_aliases,
                                   bool noChildren) {
  ENSURE_VALUE_POINTERS;
  uint child_tab = tab;
  if (!(noChildren || includePrefixes)) {
    const std::string space = BaseClass::getTabs(tab);
    out << space << error_prefix() << std::endl;
    child_tab++;
  }
#define DO_CHILD(child, dummy)						\
  child->print(out, child_tab, pad, includePrefixes, includeSkipped,    \
               skip_keys, key_aliases)
  DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  return BaseClass::print_value_map(values, out, child_tab, pad,
                                    includePrefixes, includeSkipped,
                                    skip_keys, key_aliases);
}
TEMPLATE std::size_t CLASS::_sizeArray() {
  ENSURE_VALUE_POINTERS;
  bool noChildren = false;
  std::size_t out = values.size();
#define DO_CHILD(child, dummy) out += child->sizeArray()
  DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
  return out;
}
TEMPLATE arr CLASS::_toArray() {
  ENSURE_VALUE_POINTERS;
  bool noChildren = false;
  arr out;
#define DO_CHILD(child, dummy)						\
  arr ivec = child->toArray();                                          \
  out.insert(out.end(), ivec.begin(), ivec.end())
  DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, const_iterator, );
#undef DO_CHILD
  BaseClass::value_map_toArray(values, out);
  return out;
}
TEMPLATE void CLASS::_fromArray(const arr &vec, const std::size_t offset) {
  ENSURE_VALUE_POINTERS;
  ENSURE_CHILD_CLASS_POINTERS;
  bool noChildren = false;
  std::size_t ioffset = offset;
#define DO_CHILD(child, dummy)						\
  child->fromArray(vec, ioffset);					\
  ioffset += child->sizeArray()
  DO_VALUE_SET_CHILD_CLASSES_MACRO(DO_CHILD, iterator, );
#undef DO_CHILD
  BaseClass::value_map_fromArray(values, vec, ioffset);
}

#undef TEMPLATE
#undef CLASS


//////////////////////////////////////////////////////////
// Explicit template instantiation
//////////////////////////////////////////////////////////

#include "modules/ModuleAll.hpp"
#include "Variables.hpp"

#define INST_MOD_PT_PAIRS_NO_COND                                       \
    EVAL(FOR_EACH_GENERIC(MOD2PT_PAIRS_DEFERED,				\
			  CALL_WITH_SUFFIX_ARGS_DEFER,			\
			  SEP_COMMA, (MOD2PT_NO_COND),			\
			  EXPAND VARS_INST_MODULES, FIBF))

namespace ePhotosynthesis {
  template class ValueSetWrapperBase<ValueSet_t>;
  template class ValueSetWrapperBase<ValueSetClass_t>;
  template class ValueSetBase<Variables, Variables, MODULE_ALL, PARAM_TYPE_VARS>;
  template class ValueSet<Variables, Variables, MODULE_ALL, PARAM_TYPE_VARS>;

#define CLASS_VARS(cls)                                                 \
  cls<Variables, Variables, MODULE_ALL, PARAM_TYPE_VARS>
#define CLASS_INST(cls, U, m, pt)                                       \
  cls<VARS_CLASS_VAR(m, pt), U, MODULE_ ## m, PARAM_TYPE_ ## pt>
#define EXPLICIT_TEMPLATE_INSTANTIATION_BASE(cls, U, m, pt)     \
  template class CLASS_INST(cls, U, m, pt)
#define CALL_FUNCTIONS_MULT_(what, CLASS, ...)  \
  
#define CALL_FUNCTIONS_MULT(what, args, x)
#define FUNCTIONS_MULT(METHOD, CLASS, args)    \
  UNPACK_MACRO(METHOD)(EXPAND CLASS, EXPAND args)
#define PARAM_OPTIONS_V(...)                   \
  (double), (double*), (Value<__VA_ARGS__>)
#define PARAM_OPTIONS_K(...)                                    \
  (int), (typename __VA_ARGS__::EnumType), (std::string)
#define PARAM_OPTIONS_PAIR(...)                                       \
  COMBO_ARGS((PARAM_OPTIONS_V(__VA_ARGS__)),                          \
             (PARAM_OPTIONS_V(__VA_ARGS__)))
#define PARAM_OPTIONS_TRIPLE(...)                                    \
  COMBO_ARGS((PARAM_OPTIONS_K(__VA_ARGS__)),                         \
             (PARAM_OPTIONS_V(__VA_ARGS__)),                         \
             (PARAM_OPTIONS_V(__VA_ARGS__)))

#define TEST_METHOD1_(var1)                      \
  TEST_METHOD1[var1]
#define TEST_METHOD2_(var1, var2)                    \
  TEST_METHOD2[var1, var2]
#define TEST_METHOD3_(var1, var2, var3)              \
  TEST_METHOD3[var1, var2, var3]
  
#define TEST_COMBO1(...)                                                \
  FOR_EACH_GENERIC_NESTED_PACKED_NO_ARGS(TEST_METHOD1_, SEP_SEMICOLON,  \
                                         ((a), (b<__VA_ARGS__>)))
#define TEST_COMBO2(...)                                                \
  FOR_EACH_GENERIC_NESTED_PACKED_NO_ARGS(TEST_METHOD2_, SEP_SEMICOLON,  \
                                         ((a), (b<__VA_ARGS__>)),       \
                                         ((x), (y<__VA_ARGS__>)))
#define TEST_COMBO3(...)                                                \
  FOR_EACH_GENERIC_NESTED_PACKED_NO_ARGS(TEST_METHOD3_, SEP_SEMICOLON,  \
                                         ((a), (b<__VA_ARGS__>)),       \
                                         ((x), (y<__VA_ARGS__>)),       \
                                         ((m), (n<__VA_ARGS__>)))

  // HERE1 = TEST_COMBO1(FakeClass<A, B, C>);
  // HERE2 = TEST_COMBO2(FakeClass<A, B, C>);
  // HERE3 = TEST_COMBO3(FakeClass<A, B, C>);


#define INSTANTIATE_FUNCTIONS_SING(METHOD, CLASS, ...)                  \
  FOR_EACH_GENERIC_NESTED_PACKED(METHOD, SEP_SEMICOLON, (CLASS),        \
                                 __VA_ARGS__)
#define INSTANTIATE_FUNCTIONS_MULT(METHOD, CLASS, ...)                  \
  FOR_EACH_GENERIC_NESTED_PACKED(METHOD, SEP_SEMICOLON, (CLASS),        \
                                 __VA_ARGS__)

#define INSTANTIATE_FUNCTIONS(CLASS, SUFFIX)
  /*
#define INSTANTIATE_FUNCTIONS(CLASS, SUFFIX)                            \
  INSTANTIATE_FUNCTIONS_SING(CONCATENATE(FUNCTIONS_VAL_, SUFFIX),       \
                             CLASS, (PARAM_OPTIONS_V CLASS));           \
  INSTANTIATE_FUNCTIONS_SING(CONCATENATE(FUNCTIONS_KEY_, SUFFIX),       \
                             CLASS, (PARAM_OPTIONS_K CLASS));           \
  INSTANTIATE_FUNCTIONS_MULT(CONCATENATE(FUNCTIONS_PAIR_, SUFFIX),      \
                             CLASS, (PARAM_OPTIONS_V CLASS),            \
                             (PARAM_OPTIONS_V CLASS));                  \
  INSTANTIATE_FUNCTIONS_MULT(CONCATENATE(FUNCTIONS_TRIPLE_, SUFFIX),    \
                             CLASS, (PARAM_OPTIONS_K CLASS),            \
                             (PARAM_OPTIONS_V CLASS),                   \
                             (PARAM_OPTIONS_V CLASS))
#define INSTANTIATE_FUNCTIONS_DEFERED() INSTANTIATE_FUNCTIONS


#define FUNCTIONS_KEY_VSB(CLASS, K)                     \
  FUNCTIONS_KEY_VSB_(PACK_MACRO(PACK_MACRO CLASS), PACK_MACRO(K))
#define FUNCTIONS_KEY_VSB_(CLASS, K)             \
  template std::string                           \
  CLASS::getNameWithPrefix<K>(const K& k)
#define FUNCTIONS_VAL_VSB(CLASS, V)                                     \
  FUNCTIONS_VAL_VSB_(PACK_MACRO(PACK_MACRO CLASS),                      \
                     PACK_MACRO(V))
#define FUNCTIONS_VAL_VSB_(CLASS, V)                                    \
  template std::map<typename CLASS::EnumType, V>                        \
  CLASS::int2key<V>(const std::map<int, V>& map);                       \
  template std::string                                                  \
  CLASS::string_value<V>(V const val, bool show_pointers);              \
  template std::ostream&                                                \
  CLASS::print_value<V>(const typename CLASS::EnumType&,                \
                        V const &, std::ostream&, const uint,           \
                        std::size_t, bool, bool,                        \
                        const std::vector<std::string>&,                \
                        const std::map<std::string, std::string>&,      \
                        bool, std::string);                             \
  template std::ostream&                                                \
  CLASS::print_value<V>(const std::string& name,                        \
                        V const & val,                                  \
                        std::ostream &out,                              \
                        const uint tab,                                 \
                        std::size_t pad,                                \
                        bool includePrefixes,                           \
                        bool includeSkipped,                            \
                        const std::vector<std::string>& skip_keys,      \
                        const std::map<std::string, std::string>& key_aliases, \
                        bool show_pointers,                             \
                        std::string iname);                             \
  template std::ostream&                                                \
  CLASS::print_value_map<V>(const std::map<typename CLASS::EnumType, V>& vals, \
                            std::ostream &out,                          \
                            const uint tab,                             \
                            std::size_t pad,                            \
                            bool includePrefixes,                       \
                            bool includeSkipped,                        \
                            const std::vector<std::string>& skip_keys,  \
                            const std::map<std::string, std::string>& key_aliases, \
                            bool show_pointers);                        \
  template std::string                                                  \
  CLASS::to_string_with_precision<V>(V const a_value,                   \
                                     const int n);                      \
  template void                                                         \
  CLASS::valueSet<V>(typename CLASS::ValueType& dst, V const & src, const std::string&); \
  template void                                                         \
  CLASS::insertValue<V>(std::map<typename CLASS::EnumType, typename CLASS::ValueType>& valsDst, \
                        const typename CLASS::EnumType& k, V const & src, \
                        const std::string&);                            \
  template void                                                         \
  CLASS::check_value_map<V>(const std::map<typename CLASS::EnumType, V>& vals, \
                            const std::string& context);                \
  template void                                                         \
  CLASS::check_value_map<V>(const std::map<typename CLASS::EnumType, V>& vals, \
                            const typename CLASS::EnumType k,           \
                            const std::string& context);                \
  template void                                                         \
  CLASS::remove_skipped<V>(std::map<typename CLASS::EnumType, V>& vals, \
                           const std::string& context);                 \
  template void                                                         \
  CLASS::copy_value_map<V>(std::map<typename CLASS::EnumType, V>& vals, \
                           const std::string& filename,                 \
                           const std::string& context,                  \
                           bool ignore_extras,                          \
                           bool ignore_missing,                         \
                           bool ignore_constants,                       \
                           bool ignore_skipped,                         \
                           bool ignore_initonce);                       \
  template void                                                         \
  CLASS::update_value_map<V>(std::map<typename CLASS::EnumType, V>& vals, \
                             const std::string& filename,               \
                             const std::string& context,                \
                             bool allow_extras,                         \
                             bool no_missing,                           \
                             bool no_constants,                         \
                             bool no_skipped);                          \
  template std::map<int, double>                                        \
  CLASS::export_value_map<V>(const std::map<typename CLASS::EnumType, V>& vals, \
                             const std::string& context);               \
  template void                                                         \
  CLASS::init_value_map<V>(std::map<typename CLASS::EnumType, V>& vals,  \
                           const bool isinit);                          \
  template void                                                         \
  CLASS::set_value_from_EnzymeAct<V>(std::map<typename CLASS::EnumType, V>& vals, \
                                     const typename CLASS::EnumType k,  \
                                     const std::map<std::string, double>& EnzymeAct); \
  template void                                                         \
  CLASS::reset_value_map(std::map<typename CLASS::EnumType, V>& vals);  \
  template void                                                         \
  CLASS::value_map_toArray(const std::map<typename CLASS::EnumType, V>& vals, \
                           arr& out);                                   \
  template void                                                         \
  CLASS::value_map_fromArray(std::map<typename CLASS::EnumType, V>& vals, \
                             const arr &vec,                            \
                             const std::size_t offset)
#define FUNCTIONS_PAIR_VSB(CLASS, VSrc, VDst)                           \
  FUNCTIONS_PAIR_VSB_(PACK_MACRO(PACK_MACRO CLASS),                     \
                      PACK_MACRO(VSrc),                                 \
                      PACK_MACRO(VDst))
#define FUNCTIONS_PAIR_VSB_(CLASS, VSrc, VDst)                          \
  template bool                                                         \
  CLASS::compare_value_maps<VSrc, VDst>(const std::map<typename CLASS::EnumType, VDst>& valsDst, \
                                        const std::map<typename CLASS::EnumType, VSrc>& valsSrc, \
                                        const std::string& context);    \
  template bool                                                         \
  CLASS::compare_value_maps<VSrc, VDst>(const std::map<int, VDst>& valsDst, \
                                        const std::map<int, VSrc>& valsSrc, \
                                        const std::string& context);    \
  template void                                                         \
  CLASS::check_value_maps<VSrc, VDst>(std::map<typename CLASS::EnumType, VDst>& valsDst, \
                                      const std::map<typename CLASS::EnumType, VSrc>& valsSrc, \
                                      const std::string& context,       \
                                      bool ignore_extras,               \
                                      bool ignore_missing,              \
                                      bool ignore_constants);           \
  template void                                                         \
  CLASS::copy_value_map<VSrc, VDst>(std::map<typename CLASS::EnumType, VDst>& valsDst, \
                                    const std::map<typename CLASS::EnumType, VSrc>& valsSrc, \
                                    const std::string& context,         \
                                    bool ignore_extras,                 \
                                    bool ignore_missing,                \
                                    bool ignore_constants,              \
                                    bool ignore_skipped,                \
                                    bool ignore_initonce);              \
  template void                                                         \
  CLASS::update_value_map<VSrc, VDst>(std::map<typename CLASS::EnumType, VDst>& valsDst, \
                                      const std::map<typename CLASS::EnumType, VSrc>& valsSrc, \
                                      const std::string& context,       \
                                      bool allow_extras,                \
                                      bool no_missing,                  \
                                      bool no_constants,                \
                                      bool no_skipped)
#define FUNCTIONS_TRIPLE_VSB(CLASS, K, VSrc, VDst)                       \
  FUNCTIONS_TRIPLE_VSB_(PACK_MACRO(PACK_MACRO CLASS),                   \
                        PACK_MACRO(K),                                  \
                        PACK_MACRO(VSrc),                               \
                        PACK_MACRO(VDst))
#define FUNCTIONS_TRIPLE_VSB_(CLASS, K, VSrc, VDst)                      \
  template std::string                                                  \
  CLASS::diff_value_maps<K, VSrc, VDst>(const std::map<K, VDst>& valsDst, \
                                        const std::map<K, VSrc>& valsSrc, \
                                        std::size_t padKeys,            \
                                        std::size_t padVals,            \
                                        bool includePrefixes)
#define FUNCTIONS_CHILDREN_VSB(CLASS, C)        \
  template void CLASS::addChildClass<C>()
  
#define FUNCTIONS_KEY_VS(CLASS, K)
#define FUNCTIONS_VAL_VS(CLASS, V)
#define FUNCTIONS_PAIR_VS(CLASS, VSrc, VDst)
#define FUNCTIONS_TRIPLE_VS(CLASS, K, VSrc, VDst)
#define FUNCTIONS_CHILDREN_VS(CLASS, C)
  
#define FUNCTIONS_KEY_VSS(CLASS, K)
#define FUNCTIONS_VAL_VSS(CLASS, V)
#define FUNCTIONS_PAIR_VSS(CLASS, VSrc, VDst)
#define FUNCTIONS_TRIPLE_VSS(CLASS, K, VSrc, VDst)
#define FUNCTIONS_CHILDREN_VSS(CLASS, C)
  */

#define VSB_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt)                   \
  EXPLICIT_TEMPLATE_INSTANTIATION_BASE(ValueSetBase, U, m, pt);         \
  INSTANTIATE_FUNCTIONS((CLASS_INST(ValueSetBase, U, m, pt)), VSB)

  /*
  template std::ostream& operator<< <VARS_CLASS_VAR(m, pt), U,          \
    MODULE_ ## m, PARAM_TYPE_ ## pt>                                    \
    (std::ostream& out, const CLASS_INST(ValueSetBase, U, m, pt)& x);   \
  template std::ostream& operator<< <VARS_CLASS_VAR(m, pt), U,          \
    MODULE_ ## m, PARAM_TYPE_ ## pt>                                    \
    (std::ostream& out, const CLASS_INST(ValueSetBase, U, m, pt)* x)
  */

  // FOR_EACH(FUNCTIONS_CHILDREN_VSB, CLASS_INST(ValueSetBase, U, m, pt), \
  //          CHILDREN_ ## m)
#define VS_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt)                   \
  EXPLICIT_TEMPLATE_INSTANTIATION_BASE(ValueSet, U, m, pt);            \
  INSTANTIATE_FUNCTIONS((CLASS_INST(ValueSet, U, m, pt)), VS)
#define VSS_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt)                   \
  EXPLICIT_TEMPLATE_INSTANTIATION_BASE(ValueSetStatic, U, m, pt);       \
  INSTANTIATE_FUNCTIONS((CLASS_INST(ValueSetStatic, U, m, pt)), VSS)
#define EXPLICIT_TEMPLATE_INSTANTIATION_INST(U, m, pt)  \
  VSB_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt);        \
  VS_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt)
#define EXPLICIT_TEMPLATE_INSTANTIATION_STAT(U, m, pt)  \
  VSB_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt);        \
  VSS_EXPLICIT_TEMPLATE_INSTANTIATION(U, m, pt)
  
#define EXPLICIT_TEMPLATE_INSTANTIATION_GENE(m, pt)                     \
  EXPLICIT_TEMPLATE_INSTANTIATION_INST(VARS_CLASS_VAR(m, pt), m, pt)
#define EXPLICIT_TEMPLATE_INSTANTIATION_COND(m)                         \
  EXPLICIT_TEMPLATE_INSTANTIATION_INST(VARS_CLASS_VAR(PARENT_ ## m, COND), \
                                       m, COND)
#define EXPLICIT_TEMPLATE_INSTANTIATION_COND_DEFERED()                  \
  EXPLICIT_TEMPLATE_INSTANTIATION_COND
#define EXPLICIT_TEMPLATE_INSTANTIATION_MOD(m)                          \
  EXPLICIT_TEMPLATE_INSTANTIATION_STAT(VARS_CLASS_VAR(m, COND), m, MOD)
#define EXPLICIT_TEMPLATE_INSTANTIATION_MOD_DEFERED()   \
  EXPLICIT_TEMPLATE_INSTANTIATION_MOD
  
#define EXPLICIT_TEMPLATE_INSTANTIATION_PAIR(pair)   \
  EXPLICIT_TEMPLATE_INSTANTIATION_GENE pair
#define EXPLICIT_TEMPLATE_INSTANTIATION_PAIR_DEFERED() \
  EXPLICIT_TEMPLATE_INSTANTIATION_PAIR

  INSTANTIATE_FUNCTIONS((CLASS_VARS(ValueSetBase)), VSB);
  INSTANTIATE_FUNCTIONS((CLASS_VARS(ValueSetBase)), VS);

  EVAL(FOR_EACH_GENERIC(EXPLICIT_TEMPLATE_INSTANTIATION_COND_DEFERED,
                        CALL_WITH_EMPTY_ARGS_DEFER, SEP_SEMICOLON, (),
                        MEMBER_NAMES_MODULE));
  EVAL(FOR_EACH_GENERIC(EXPLICIT_TEMPLATE_INSTANTIATION_MOD_DEFERED,
                        CALL_WITH_EMPTY_ARGS_DEFER, SEP_SEMICOLON, (),
                        MEMBER_NAMES_MODULE));
  EVAL(FOR_EACH_GENERIC(EXPLICIT_TEMPLATE_INSTANTIATION_PAIR_DEFERED,
                        CALL_WITH_EMPTY_ARGS_DEFER, SEP_SEMICOLON, (),
                        INST_MOD_PT_PAIRS_NO_COND));
  // FOR_EACH(EXPLICIT_TEMPLATE_INSTANTIATION_MOD,
  //          MEMBER_NAMES_MODULE);
  // FOR_EACH(EXPLICIT_TEMPLATE_INSTANTIATION_PAIR,
  //          INST_MOD_PT_PAIRS_NO_COND);


#undef CLASS_VARS
#undef CLASS_INST
#undef EXPLICIT_TEMPLATE_INSTANTIATION_BASE
#undef FUNCTIONS_MULT
#undef PARAM_OPTIONS_V
#undef PARAM_OPTIONS_K
#undef COMBINE_OPTIONS
#undef PARAM_OPTIONS_PAIR
#undef PARAM_OPTIONS_TRIPLE
#undef INSTANTIATE_FUNCTIONS_SING
#undef INSTANTIATE_FUNCTIONS_MULT
#undef INSTANTIATE_FUNCTIONS
#undef FUNCTIONS_KEY_VSB
#undef FUNCTIONS_VAL_VSB
#undef FUNCTIONS_PAIR_VSB
#undef FUNCTIONS_TRIPLE_VSB
#undef FUNCTIONS_CHILDREN_VSB
#undef FUNCTIONS_KEY_VS
#undef FUNCTIONS_VAL_VS
#undef FUNCTIONS_PAIR_VS
#undef FUNCTIONS_TRIPLE_VS
#undef FUNCTIONS_CHILDREN_VS
#undef FUNCTIONS_KEY_VSS
#undef FUNCTIONS_VAL_VSS
#undef FUNCTIONS_PAIR_VSS
#undef FUNCTIONS_TRIPLE_VSS
#undef FUNCTIONS_CHILDREN_VSS
#undef VSB_EXPLICIT_TEMPLATE_INSTANTIATION
#undef VS_EXPLICIT_TEMPLATE_INSTANTIATION
#undef VSS_EXPLICIT_TEMPLATE_INSTANTIATION
#undef EXPLICIT_TEMPLATE_INSTANTIATION_INST
#undef EXPLICIT_TEMPLATE_INSTANTIATION_STAT
#undef EXPLICIT_TEMPLATE_INSTANTIATION_GENE
#undef EXPLICIT_TEMPLATE_INSTANTIATION_COND
#undef EXPLICIT_TEMPLATE_INSTANTIATION_MOD
#undef EXPLICIT_TEMPLATE_INSTANTIATION_PAIR

  
}
