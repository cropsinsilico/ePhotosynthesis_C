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
