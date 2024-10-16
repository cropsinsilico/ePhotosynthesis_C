#include "enums/enums_utils.hpp"
#include <istream>
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::MODULE& x) {
  out << ePhotosynthesis::get_enum_names<ePhotosynthesis::MODULE>().find(x)->second;
  return out;
}
std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::PARAM_TYPE& x) {
  out << ePhotosynthesis::get_enum_names<ePhotosynthesis::PARAM_TYPE>().find(x)->second;
  return out;
}

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
