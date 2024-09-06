#include "enums/enums_utils.hpp"
#include <istream>
#include <iostream>
#include <algorithm>

std::string ePhotosynthesis::utils::str_toupper(const std::string& inStr) {
  std::string outStr(inStr);
  std::transform(inStr.begin(), inStr.end(), outStr.begin(),
		 [](unsigned char c) { return std::toupper(c); });
  return outStr;
}

std::string ePhotosynthesis::utils::str_tolower(const std::string& inStr) {
  std::string outStr(inStr);
  std::transform(inStr.begin(), inStr.end(), outStr.begin(),
		 [](unsigned char c) { return std::tolower(c); });
  return outStr;
}

template<> const std::map<const EMPTY_ENUM, const std::string>& get_enum_names<enum EMPTY_ENUM>() {
  static const std::map<const EMPTY_ENUM, const std::string> map;
  return map;
};

template<> const std::map<const EMPTY_ENUM, const double>& get_enum_defaults<enum EMPTY_ENUM>() {
  static const std::map<const EMPTY_ENUM, const double> map;
  return map;
};
