// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include "enums/enums_helpers.hpp"

namespace ePhotosynthesis {
  // [BEGIN] EXPLICIT_SPECIALIZATION_BF_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_BFVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_BF_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_BF_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_BF_VEL& x) {
    out << ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_BF_VEL, T>& x) {
    ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_BF_VEL>& x) {
    ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_BF_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_BF_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_BF, PARAM_TYPE_VEL> BF;
  }
  // [END] SPECIALIZED_TYPEDEF_BF_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_FIBF_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_FIBFVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_FIBF_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_FIBF_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_FIBF_VEL& x) {
    out << ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_FIBF_VEL, T>& x) {
    ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_FIBF_VEL>& x) {
    ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_FIBF_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_FIBF_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_FIBF, PARAM_TYPE_VEL> FIBF;
  }
  // [END] SPECIALIZED_TYPEDEF_FIBF_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_FI_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_FIVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_FI_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_FI_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_FI_VEL& x) {
    out << ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_FI_VEL, T>& x) {
    ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_FI_VEL>& x) {
    ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_FI_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_FI_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_FI, PARAM_TYPE_VEL> FI;
  }
  // [END] SPECIALIZED_TYPEDEF_FI_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_PR_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_PRVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_PR_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_PR_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_PR_VEL& x) {
    out << ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_PR_VEL, T>& x) {
    ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_PR_VEL>& x) {
    ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_PR_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_PR_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_PR, PARAM_TYPE_VEL> PR;
  }
  // [END] SPECIALIZED_TYPEDEF_PR_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_PS_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_PSVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_PS_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_PS_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_PS_VEL& x) {
    out << ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_PS_VEL, T>& x) {
    ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_PS_VEL>& x) {
    ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_PS_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_PS_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_PS, PARAM_TYPE_VEL> PS;
  }
  // [END] SPECIALIZED_TYPEDEF_PS_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_RROEA_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_RROEAVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_RROEA_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_RROEA_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_RROEA_VEL& x) {
    out << ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_RROEA_VEL, T>& x) {
    ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_RROEA_VEL>& x) {
    ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_RROEA_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_RROEA_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_RROEA, PARAM_TYPE_VEL> RROEA;
  }
  // [END] SPECIALIZED_TYPEDEF_RROEA_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_REDOXREG_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_RedoxRegVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_REDOXREG_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_REDOXREG_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_RedoxReg_VEL& x) {
    out << ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_RedoxReg_VEL, T>& x) {
    ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_RedoxReg_VEL>& x) {
    ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_REDOXREG_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_REDOXREG_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_VEL> RedoxReg;
  }
  // [END] SPECIALIZED_TYPEDEF_REDOXREG_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_RUACT_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_RuACTVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_RUACT_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_RUACT_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_RuACT_VEL& x) {
    out << ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_RuACT_VEL, T>& x) {
    ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_RuACT_VEL>& x) {
    ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_RUACT_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_RUACT_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_RuACT, PARAM_TYPE_VEL> RuACT;
  }
  // [END] SPECIALIZED_TYPEDEF_RUACT_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_SUCS_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_SUCSVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_SUCS_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_SUCS_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_SUCS_VEL& x) {
    out << ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_SUCS_VEL, T>& x) {
    ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_SUCS_VEL>& x) {
    ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_SUCS_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_SUCS_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_SUCS, PARAM_TYPE_VEL> SUCS;
  }
  // [END] SPECIALIZED_TYPEDEF_SUCS_VEL
  
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_XANCYCLE_VEL
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type : int {
    MEMBERS_XanCycleVel
  };
  #endif // EPHOTO_USE_SCOPED_ENUM
  template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::all;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::names;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::defaults;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::defaults_C3;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::glymaids;
  template<> const std::map<std::string, typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::aliases;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::docs;
  template<> std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::value_flags;
  template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::Type, int> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::static_value_flags;
  // [END] EXPLICIT_SPECIALIZATION_XANCYCLE_VEL
  
  // [BEGIN] EXPLICIT_SPECIALIZATION_XANCYCLE_VEL_UTILS
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
  inline std::ostream& operator<<(std::ostream& out, const ENUM_XanCycle_VEL& x) {
    out << ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::getName(x);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::map<ENUM_XanCycle_VEL, T>& x) {
    ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::print_map(x, out);
    return out;
  }
  /**
    Serialize an enum to an output stream
    \param[in,out] out Output stream
    \param[in] x Collection to serialize
    \return Updated stream
  */
  inline std::ostream& operator<<(std::ostream& out, const std::vector<ENUM_XanCycle_VEL>& x) {
    ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL>::print_vector(x, out);
    return out;
  }
  #endif // EPHOTO_USE_SCOPED_ENUM
  // [END] EXPLICIT_SPECIALIZATION_XANCYCLE_VEL_UTILS
  
  // [BEGIN] SPECIALIZED_TYPEDEF_XANCYCLE_VEL
  namespace VEL {
    typedef ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_VEL> XanCycle;
  }
  // [END] SPECIALIZED_TYPEDEF_XANCYCLE_VEL
  
  
}
