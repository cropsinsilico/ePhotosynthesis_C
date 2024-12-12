// Include guard here, just in case
// #pragma once
#include <cstdlib>
#include "ValueSet.hpp"
#include "Variables.hpp"

using namespace ePhotosynthesis;

void cleanup_value_sets() {
  Variables::cleanupValueSets();
}
void register_value_set_cleanup() {
  static bool cleanup_value_sets_registered = false;
  if (!cleanup_value_sets_registered) {
    std::atexit(cleanup_value_sets);
    cleanup_value_sets_registered = true;
  }
}

/* ValueSet_t */
bool ValueSet_t::equals(const ValueSet_t& b0,
			const bool noChildren) const {
  UNUSED(noChildren);
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


/* ValueSetClass_t */
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

/* ValueSetClassType */
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


/* ValueSetStaticClassType */
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

// #define VS_EXPLICIT_TEMPLATE_INSTANTIATION_(m, pt)   \
//   template class ValueSetClassType<VARS_CLASS_VAR(m, pt)>;
//   // template class ValueSetStaticClassType<VARS_CLASS_VAR(m, pt)>;
// #define VS_EXPLICIT_TEMPLATE_INSTANTIATION(pair) \
//     VS_EXPLICIT_TEMPLATE_INSTANTIATION_ pair
// FOR_EACH(VS_EXPLICIT_TEMPLATE_INSTANTIATION, ALL_MOD_PT_PAIRS)
