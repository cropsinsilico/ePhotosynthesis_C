// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#ifdef _MSC_VER
// There is a bug in the MSVC compiler where it does not allow
//   declaration of a specialized class member enum
//   https://developercommunity.visualstudio.com/t/Explicit-specialization-of-member-enumer/10609934
#define EPHOTO_USE_SCOPED_ENUM 1
#endif // _MSC_VER
#ifdef EPHOTO_USE_SCOPED_ENUM
#define SCOPED_ENUM_TYPE(name) name::
#else // EPHOTO_USE_SCOPED_ENUM
#define SCOPED_ENUM_TYPE(name)
#endif // EPHOTO_USE_SCOPED_ENUM
#include "enums/enums_helpers.hpp"

namespace ePhotosynthesis {
  // Unspecialized enum
  template<MODULE M, PARAM_TYPE PT>
  class ValueSetEnum {
  public:
    #ifdef EPHOTO_USE_SCOPED_ENUM
    typedef typename enum_helper<M, PT>::type Type;
    #else  // EPHOTO_USE_SCOPED_ENUM
    enum Type : int;
    #endif  // EPHOTO_USE_SCOPED_ENUM
    static const MODULE module;
    static const PARAM_TYPE param_type;
    static const std::vector<Type> all;  /**< All enum values */
    static const std::map<Type, std::string> names;  /**< Names for values */
    static const std::map<Type, double> defaults;  /**< Defaults for values */
    static const std::map<Type, double> defaults_C3;  /**< Defaults_C3 for values */
    static const std::map<Type, std::string> glymaids;  /**< Glymaids for values */
    static const std::map<std::string, Type> aliases;  /**< Aliases for values */
    static const std::map<Type, std::string> docs;  /**< Docs for values */
    static std::map<Type, int> value_flags;  /**< Value_Flags for values */
    static const std::map<Type, int> static_value_flags;  /**< Static_Value_Flags for values */
    /**
      Get a prefix for errors describing the class
      \return Prefix
    */
    static std::string error_prefix();
    /**
      Get the name value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static std::string getName(const Type& x) {
      typename std::map<Type, std::string>::const_iterator it;
      it = names.find(x);
      if (it == names.end()) {
        throw std::runtime_error("Could not locate Name for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Get the name value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static std::string getName(const Type& x, const std::string& defaultV) {
      typename std::map<Type, std::string>::const_iterator it;
      it = names.find(x);
      if (it == names.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the enum key corresponding to a name values
      \param[in] x Value to get key for
      \return Key
    */
    static Type fromName(const std::string& x) {
      typename std::map<Type, std::string>::const_iterator it;
      for (it = names.begin(); it != names.end(); it++) {
        if (it->second == x) break;
      }
      if (it == names.end()) {
        throw std::runtime_error("Could not locate Name for '" + x + "'");
      }
      return it->first;
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static double getDefault(const Type& x) {
      typename std::map<Type, double>::const_iterator it;
      it = defaults.find(x);
      if (it == defaults.end()) {
        throw std::runtime_error("Could not locate Default for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Get the default value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static double getDefault(const Type& x, const double& defaultV) {
      typename std::map<Type, double>::const_iterator it;
      it = defaults.find(x);
      if (it == defaults.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the defaultc3 value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static double getDefaultC3(const Type& x) {
      typename std::map<Type, double>::const_iterator it;
      it = defaults_C3.find(x);
      if (it == defaults_C3.end()) {
        throw std::runtime_error("Could not locate DefaultC3 for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Get the defaultc3 value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static double getDefaultC3(const Type& x, const double& defaultV) {
      typename std::map<Type, double>::const_iterator it;
      it = defaults_C3.find(x);
      if (it == defaults_C3.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the glymaid value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static std::string getGlymaid(const Type& x) {
      typename std::map<Type, std::string>::const_iterator it;
      it = glymaids.find(x);
      if (it == glymaids.end()) {
        throw std::runtime_error("Could not locate Glymaid for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Get the glymaid value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static std::string getGlymaid(const Type& x, const std::string& defaultV) {
      typename std::map<Type, std::string>::const_iterator it;
      it = glymaids.find(x);
      if (it == glymaids.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the enum key corresponding to a glymaid values
      \param[in] x Value to get key for
      \return Key
    */
    static Type fromGlymaid(const std::string& x) {
      typename std::map<Type, std::string>::const_iterator it;
      for (it = glymaids.begin(); it != glymaids.end(); it++) {
        if (it->second == x) break;
      }
      if (it == glymaids.end()) {
        throw std::runtime_error("Could not locate Glymaid for '" + x + "'");
      }
      return it->first;
    }
    /**
      Get the alias value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static Type getAlias(const std::string& x) {
      typename std::map<std::string, Type>::const_iterator it;
      it = aliases.find(x);
      if (it == aliases.end()) {
        throw std::runtime_error("Could not locate Alias for '" + x + "'");
      }
      return it->second;
    }
    /**
      Get the alias value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static Type getAlias(const std::string& x, const Type& defaultV) {
      typename std::map<std::string, Type>::const_iterator it;
      it = aliases.find(x);
      if (it == aliases.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the docs value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static std::string getDocs(const Type& x) {
      typename std::map<Type, std::string>::const_iterator it;
      it = docs.find(x);
      if (it == docs.end()) {
        throw std::runtime_error("Could not locate Docs for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Get the docs value corresponding to an enum key
      \param[in] x Key to get value for
      \param[in] defaultV Value to return if x is not present
      \return Value
    */
    static std::string getDocs(const Type& x, const std::string& defaultV) {
      typename std::map<Type, std::string>::const_iterator it;
      it = docs.find(x);
      if (it == docs.end()) {
        return defaultV;
      }
      return it->second;
    }
    /**
      Get the enum key corresponding to a docs values
      \param[in] x Value to get key for
      \return Key
    */
    static Type fromDocs(const std::string& x) {
      typename std::map<Type, std::string>::const_iterator it;
      for (it = docs.begin(); it != docs.end(); it++) {
        if (it->second == x) break;
      }
      if (it == docs.end()) {
        throw std::runtime_error("Could not locate Docs for '" + x + "'");
      }
      return it->first;
    }
    /**
      Get the valueflag value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static int getValueFlag(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = value_flags.find(x);
      if (it == value_flags.end()) {
        throw std::runtime_error("Could not locate ValueFlag for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Check if a key is in value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isSkipped(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = value_flags.find(x);
      if (it == value_flags.end()) return false;
      return (it->second & VALUE_FLAG_SKIPPED);
    }
    /**
      Throw an error if a key is not in value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkSkipped(const Type& x, const std::string& context = "") {
      if (!isSkipped(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in value_flags");
      }
    }
    /**
      Throw an error if a key is in value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotSkipped(const Type& x, const std::string& context = "") {
      if (isSkipped(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in value_flags");
      }
    }
    /**
      Get the number of elements in value_flags with VALUE_FLAG_SKIPPED set.
      \return Number of elements with VALUE_FLAG_SKIPPED set.
    */
    static std::size_t countSkipped() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = value_flags.begin(); it != value_flags.end(); it++) {
        if (it->second & VALUE_FLAG_SKIPPED) out++;
      }
      return out;
    }
    /**
      Get the number of elements in value_flags.
      \return Number of elements.
    */
    static std::size_t countValueFlags() {
      std::size_t out;
      out = value_flags.size();
      return out;
    }
    /**
      Get the set of elements in value_flags with VALUE_FLAG_SKIPPED set.
      \return Set of elements.
    */
    static std::vector<Type> listSkipped() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = value_flags.begin(); it != value_flags.end(); it++){
        if (it->second & VALUE_FLAG_SKIPPED) {
          out.push_back(it->first);
        }
      }
      return out;
    }
    /**
      Remove all VALUE_FLAG_SKIPPED flags from value_flags
    */
    static void clearSkipped() {
      typename std::map<Type, int>::iterator it;
      for (it = value_flags.begin(); it != value_flags.end(); it++){
        it->second &= ~VALUE_FLAG_SKIPPED;
      }
    }
    /**
      Remove all entries from value_flags
    */
    static void clearValueFlags() {
      value_flags.clear();
    }
    /**
      Add an element to value_flags if it is not already present
      \param[in] x Key to add
    */
    static void addSkipped(const Type& x) {
      if (!isSkipped(x)) {
        typename std::map<Type, int>::iterator it;
        it = value_flags.find(x);
        if (it == value_flags.end()) {
          value_flags.emplace(x, VALUE_FLAG_NONE);
          it = value_flags.find(x);
        }
        it->second |= VALUE_FLAG_SKIPPED;
      }
    }
    /**
      Add an element to value_flags if it is not already present
      \param[in] x Key to add
      \param y Value to add for x
    */
    static void addValueFlags(const Type& x, const int& y) {
      value_flags.emplace(x, y);
    }
    /**
      Remove an element from value_flags
      \param[in] x Key to remove
    */
    static void removeSkipped(const Type& x) {
      typename std::map<Type, int>::iterator it;
      it = value_flags.find(x);
      if (it != value_flags.end()) {
        it->second &= ~VALUE_FLAG_SKIPPED;
      }
    }
    /**
      Remove an element from value_flags
      \param[in] x Key to remove
    */
    static void removeValueFlags(const Type& x) {
      typename std::map<Type, int>::iterator it;
      it = value_flags.find(x);
      if (it != value_flags.end()) {
        value_flags.erase(it);
      }
    }
    /**
      Add multiple elements to value_flags if they are not already present
      \param[in] x Elements to add
    */
    static void addMultipleValueFlags(const std::map<Type, int>& x) {
      typename std::map<Type, int>::const_iterator it;
      for (it = x.begin(); it != x.end(); it++) {
        addValueFlags(it->first, it->second);
      }
    }
    /**
      Remove multiple elements to value_flags if they are not already present
      \param[in] x Elements to add
    */
    static void removeMultipleValueFlags(const std::map<Type, int>& x) {
      typename std::map<Type, int>::const_iterator it;
      for (it = x.begin(); it != x.end(); it++) {
        removeValueFlags(it->first);
      }
    }
    /**
      Get the staticvalueflag value corresponding to an enum key
      \param[in] x Key to get value for
      \return Value
    */
    static int getStaticValueFlag(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) {
        throw std::runtime_error("Could not locate StaticValueFlag for '" + names.find(x)->second + "'");
      }
      return it->second;
    }
    /**
      Check if a key is in static_value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isConstant(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) return false;
      return (it->second & STATIC_VALUE_FLAG_CONST);
    }
    /**
      Check if a key is in static_value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isCalculated(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) return false;
      return (it->second & STATIC_VALUE_FLAG_CALC);
    }
    /**
      Check if a key is in static_value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isNonvector(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) return false;
      return (it->second & STATIC_VALUE_FLAG_NON_VECTOR);
    }
    /**
      Check if a key is in static_value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isResetone(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) return false;
      return (it->second & STATIC_VALUE_FLAG_RESET_ONE);
    }
    /**
      Check if a key is in static_value_flags
      \param[in] x Key to check
      \return true if x is present, false otherwise
    */
    static bool isInitonce(const Type& x) {
      typename std::map<Type, int>::const_iterator it;
      it = static_value_flags.find(x);
      if (it == static_value_flags.end()) return false;
      return (it->second & STATIC_VALUE_FLAG_INIT_ONCE);
    }
    /**
      Throw an error if a key is not in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkConstant(const Type& x, const std::string& context = "") {
      if (!isConstant(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in static_value_flags");
      }
    }
    /**
      Throw an error if a key is not in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkCalculated(const Type& x, const std::string& context = "") {
      if (!isCalculated(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in static_value_flags");
      }
    }
    /**
      Throw an error if a key is not in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNonvector(const Type& x, const std::string& context = "") {
      if (!isNonvector(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in static_value_flags");
      }
    }
    /**
      Throw an error if a key is not in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkResetone(const Type& x, const std::string& context = "") {
      if (!isResetone(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in static_value_flags");
      }
    }
    /**
      Throw an error if a key is not in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkInitonce(const Type& x, const std::string& context = "") {
      if (!isInitonce(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is not in static_value_flags");
      }
    }
    /**
      Throw an error if a key is in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotConstant(const Type& x, const std::string& context = "") {
      if (isConstant(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in static_value_flags");
      }
    }
    /**
      Throw an error if a key is in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotCalculated(const Type& x, const std::string& context = "") {
      if (isCalculated(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in static_value_flags");
      }
    }
    /**
      Throw an error if a key is in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotNonvector(const Type& x, const std::string& context = "") {
      if (isNonvector(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in static_value_flags");
      }
    }
    /**
      Throw an error if a key is in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotResetone(const Type& x, const std::string& context = "") {
      if (isResetone(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in static_value_flags");
      }
    }
    /**
      Throw an error if a key is in static_value_flags
      \param[in] x Key to check
      \param[in] context String describing context that 
        should be used in the error message
    */
    static void checkNotInitonce(const Type& x, const std::string& context = "") {
      if (isInitonce(x)) {
        throw std::runtime_error(error_prefix() + context + ": '" + names.find(x)->second + "' is in static_value_flags");
      }
    }
    /**
      Get the number of elements in static_value_flags with STATIC_VALUE_FLAG_CONST set.
      \return Number of elements with STATIC_VALUE_FLAG_CONST set.
    */
    static std::size_t countConstant() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++) {
        if (it->second & STATIC_VALUE_FLAG_CONST) out++;
      }
      return out;
    }
    /**
      Get the number of elements in static_value_flags with STATIC_VALUE_FLAG_CALC set.
      \return Number of elements with STATIC_VALUE_FLAG_CALC set.
    */
    static std::size_t countCalculated() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++) {
        if (it->second & STATIC_VALUE_FLAG_CALC) out++;
      }
      return out;
    }
    /**
      Get the number of elements in static_value_flags with STATIC_VALUE_FLAG_NON_VECTOR set.
      \return Number of elements with STATIC_VALUE_FLAG_NON_VECTOR set.
    */
    static std::size_t countNonvector() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++) {
        if (it->second & STATIC_VALUE_FLAG_NON_VECTOR) out++;
      }
      return out;
    }
    /**
      Get the number of elements in static_value_flags with STATIC_VALUE_FLAG_RESET_ONE set.
      \return Number of elements with STATIC_VALUE_FLAG_RESET_ONE set.
    */
    static std::size_t countResetone() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++) {
        if (it->second & STATIC_VALUE_FLAG_RESET_ONE) out++;
      }
      return out;
    }
    /**
      Get the number of elements in static_value_flags with STATIC_VALUE_FLAG_INIT_ONCE set.
      \return Number of elements with STATIC_VALUE_FLAG_INIT_ONCE set.
    */
    static std::size_t countInitonce() {
      std::size_t out;
      out = 0;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++) {
        if (it->second & STATIC_VALUE_FLAG_INIT_ONCE) out++;
      }
      return out;
    }
    /**
      Get the number of elements in static_value_flags.
      \return Number of elements.
    */
    static std::size_t countStaticValueFlags() {
      std::size_t out;
      out = static_value_flags.size();
      return out;
    }
    /**
      Get the set of elements in static_value_flags with STATIC_VALUE_FLAG_CONST set.
      \return Set of elements.
    */
    static std::vector<Type> listConstant() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++){
        if (it->second & STATIC_VALUE_FLAG_CONST) {
          out.push_back(it->first);
        }
      }
      return out;
    }
    /**
      Get the set of elements in static_value_flags with STATIC_VALUE_FLAG_CALC set.
      \return Set of elements.
    */
    static std::vector<Type> listCalculated() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++){
        if (it->second & STATIC_VALUE_FLAG_CALC) {
          out.push_back(it->first);
        }
      }
      return out;
    }
    /**
      Get the set of elements in static_value_flags with STATIC_VALUE_FLAG_NON_VECTOR set.
      \return Set of elements.
    */
    static std::vector<Type> listNonvector() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++){
        if (it->second & STATIC_VALUE_FLAG_NON_VECTOR) {
          out.push_back(it->first);
        }
      }
      return out;
    }
    /**
      Get the set of elements in static_value_flags with STATIC_VALUE_FLAG_RESET_ONE set.
      \return Set of elements.
    */
    static std::vector<Type> listResetone() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++){
        if (it->second & STATIC_VALUE_FLAG_RESET_ONE) {
          out.push_back(it->first);
        }
      }
      return out;
    }
    /**
      Get the set of elements in static_value_flags with STATIC_VALUE_FLAG_INIT_ONCE set.
      \return Set of elements.
    */
    static std::vector<Type> listInitonce() {
      std::vector<Type> out;
      typename std::map<Type, int>::const_iterator it;
      for (it = static_value_flags.begin(); it != static_value_flags.end(); it++){
        if (it->second & STATIC_VALUE_FLAG_INIT_ONCE) {
          out.push_back(it->first);
        }
      }
      return out;
    }
  };
  template<MODULE M, PARAM_TYPE PT>
  const MODULE ValueSetEnum<M, PT>::module = M;
  template<MODULE M, PARAM_TYPE PT>
  const PARAM_TYPE ValueSetEnum<M, PT>::param_type = PT;
  template<MODULE M, PARAM_TYPE PT>
  const std::vector<typename ValueSetEnum<M, PT>::Type> ValueSetEnum<M, PT>::all = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, std::string> ValueSetEnum<M, PT>::names = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, double> ValueSetEnum<M, PT>::defaults = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, double> ValueSetEnum<M, PT>::defaults_C3 = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, std::string> ValueSetEnum<M, PT>::glymaids = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<std::string, typename ValueSetEnum<M, PT>::Type> ValueSetEnum<M, PT>::aliases = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, std::string> ValueSetEnum<M, PT>::docs = {};
  template<MODULE M, PARAM_TYPE PT>
  std::map<typename ValueSetEnum<M, PT>::Type, int> ValueSetEnum<M, PT>::value_flags = {};
  template<MODULE M, PARAM_TYPE PT>
  const std::map<typename ValueSetEnum<M, PT>::Type, int> ValueSetEnum<M, PT>::static_value_flags = {};
  
}
// Global includes
// [BEGIN] HEADERS_GLOBAL
#include "enums/enums_COND.hpp"
#include "enums/enums_POOL.hpp"
#include "enums/enums_KE.hpp"
#include "enums/enums_MOD.hpp"
#include "enums/enums_RC.hpp"
#include "enums/enums_VARS.hpp"
#include "enums/enums_VEL.hpp"
// [END] HEADERS_GLOBAL

