// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include "enums/enums_helpers.hpp"

namespace ePhotosynthesis {
  #ifdef EPHOTO_USE_SCOPED_ENUM
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Key to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const ENUM_ALL_VARS& x) {
    out << ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_ALL_VARS, T>& x) {
    ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_ALL_VARS>& x) {
    ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::print_vector(x, out);
    return out;
  }
  #else // EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type : int {
    MEMBERS_ALLVARS
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, std::string> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, double> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, double> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, std::string> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, std::string> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, int> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, int> ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::static_value_flags;
  
  namespace VARS {
    typedef ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS> ALL;
  }
  
}
