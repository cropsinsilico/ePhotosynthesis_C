// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include "enums/enums_helpers.hpp"

namespace ePhotosynthesis {
// [BEGIN] EXPLICIT_SPECIALIZATION_ALL_VARS
#ifndef EPHOTO_USE_SCOPED_ENUM
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
// [END] EXPLICIT_SPECIALIZATION_ALL_VARS
}

// [BEGIN] EXPLICIT_SPECIALIZATION_ALL_VARS_UTILS
#ifdef EPHOTO_USE_SCOPED_ENUM
// Definition of explicitly specialized enum class 
//   utility methods. This causes instantiation of the 
//   specialized class and must come after explicit 
//   specialization of the class (and class members)
/**
  Serialize an enum to an output stream
  \param[in,out] out Output stream
  \param[in] x Key to serialize
  \return Updated stream
*/
std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::ENUM_ALL_VARS& x);
/**
  Serialize an enum to an output stream
  \param[in,out] out Output stream
  \param[in] x Collection to serialize
  \return Updated stream
*/
std::ostream& operator<<(std::ostream& out, const std::map<ePhotosynthesis::ENUM_ALL_VARS, std::string>& x);
/**
  Serialize an enum to an output stream
  \param[in,out] out Output stream
  \param[in] x Collection to serialize
  \return Updated stream
*/
std::ostream& operator<<(std::ostream& out, const std::map<ePhotosynthesis::ENUM_ALL_VARS, double>& x);
/**
  Serialize an enum to an output stream
  \param[in,out] out Output stream
  \param[in] x Collection to serialize
  \return Updated stream
*/
std::ostream& operator<<(std::ostream& out, const std::map<ePhotosynthesis::ENUM_ALL_VARS, int>& x);
/**
  Serialize an enum to an output stream
  \param[in,out] out Output stream
  \param[in] x Collection to serialize
  \return Updated stream
*/
std::ostream& operator<<(std::ostream& out, const std::vector<ePhotosynthesis::ENUM_ALL_VARS>& x);
#endif // EPHOTO_USE_SCOPED_ENUM
// [END] EXPLICIT_SPECIALIZATION_ALL_VARS_UTILS

namespace ePhotosynthesis {
  // [BEGIN] SPECIALIZED_TYPEDEF_ALL_VARS
  namespace VARS {
    typedef ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS> ALL;
  }
  // [END] SPECIALIZED_TYPEDEF_ALL_VARS
  
}

