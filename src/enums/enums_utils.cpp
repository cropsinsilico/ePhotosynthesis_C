#include "enums/enums_utils.hpp"
#include <istream>
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::MODULE& x) {
  out << ePhotosynthesis::utils::get_enum_names<ePhotosynthesis::MODULE>().find(x)->second;
  return out;
}
std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::PARAM_TYPE& x) {
  out << ePhotosynthesis::utils::get_enum_names<ePhotosynthesis::PARAM_TYPE>().find(x)->second;
  return out;
}

///////////////////////////
// String utilities
///////////////////////////

using namespace ePhotosynthesis;

std::vector<std::string>
utils::str_split(const std::string& inStr,
                                  const std::string& sep) {
  std::vector<std::string> out;
  return out;
}

std::string utils::str_toupper(const std::string& inStr) {
  std::string outStr(inStr);
  std::transform(inStr.begin(), inStr.end(), outStr.begin(),
		 [](unsigned char c) { return std::toupper(c); });
  return outStr;
}

std::string utils::str_tolower(const std::string& inStr) {
  std::string outStr(inStr);
  std::transform(inStr.begin(), inStr.end(), outStr.begin(),
		 [](unsigned char c) { return std::tolower(c); });
  return outStr;
}

std::string utils::str_getTabs(const uint tab) {
  static const uint tab_size = 2;
  return std::string(tab * tab_size, ' ');
}

bool utils::str_startswith(std::string const& value,
                           std::string const& prefix) {
  if (prefix.size() > value.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), value.begin());
}
bool utils::str_endswith(std::string const& value,
                         std::string const& suffix) {
  if (suffix.size() > value.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), value.rbegin());
}
std::string utils::to_string(const std::string& x) {
  return x;
}
std::string utils::to_string(const char* x) {
  return std::string(x);
}

///////////////////////////
// Value utilities
///////////////////////////
bool utils::values_equal(const double& a, const double& b) {
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
double utils::extract_value(const double& x) { return x; }
double utils::extract_value(const double* x) { return *x; }

///////////////////////////
// Enum utilities
///////////////////////////
template<>
const std::map<MODULE, std::string>& utils::get_enum_names<MODULE>() {
#define ADD_MODULE(mod)                         \
  {MODULE_ ## mod, #mod}
  static const std::map<MODULE, std::string> collection = {
    FOR_EACH_COMMA(ADD_MODULE, EXPAND(MEMBER_NAMES_COMPLETE_MODULE))
  };
#undef ADD_MODULE
  return collection;
}
template<>
const std::map<PARAM_TYPE, std::string>& utils::get_enum_names<PARAM_TYPE>() {
#define ADD_PARAM_TYPE(mod)                     \
  {PARAM_TYPE_ ## mod, #mod}
  static const std::map<PARAM_TYPE, std::string> collection = {
    FOR_EACH_COMMA(ADD_PARAM_TYPE, EXPAND(MEMBER_NAMES_COMPLETE_PARAM))
  };
#undef ADD_PARAM_TYPE
  return collection;
}
