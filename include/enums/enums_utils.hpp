#pragma once

#include "ePhotosynthesis_export.h"

#include "enums/enums_constants_map.hpp"
#include "enums/enums_init_map.hpp"

namespace ePhotosynthesis {
  namespace utils {

    EPHOTO_API std::string str_toupper(const std::string& inStr);
    EPHOTO_API std::string str_tolower(const std::string& inStr);
    template<typename T1>
    bool enum_key_search(const std::map<const T1, const std::string> map,
			 const T1& key,
			 std::string& val) {
      for (typename std::map<const T1, const std::string>::const_iterator it = map.cbegin();
	   it != map.cend(); it++) {
	if (it->first == key) {
	  val = it->second;
	  return true;
	}
      }
      return false;
    }
			 
    template<typename T1>
    bool enum_value_search(const std::map<const T1, const std::string> map,
			   const std::string& val,
			   T1& key, bool allow_anycase=false,
			   std::string prefix="",
			   std::string suffix="") {
      for (typename std::map<const T1, const std::string>::const_iterator it = map.cbegin();
	   it != map.cend(); it++) {
	if ((it->second == val) ||
	    (allow_anycase && (str_toupper(val) == str_toupper(it->second))) ||
	    ((!prefix.empty()) && (val == (prefix + it->second))) ||
	    ((!suffix.empty()) && (val == (it->second + suffix)))) {
	  key = it->first;
	  return true;
	}
      }
      return false;
    }
    template<typename T1, typename T2>
    T1 max_enum_value(const std::map<const T1, const T2> map) {
      return map.crbegin()->first;
    }


    template<typename T, typename Tflag>
    std::string strBitFlags(const T& flag,
			    const std::map<const Tflag, const std::string>& map,
			    const std::string& prefix="",
			    const std::string& suffix="\n") {
      std::string out;
      for (typename std::map<const Tflag, const std::string>::const_iterator it = map.begin();
	   it != map.end(); it++) {
	if (flag & it->first)
	  out += prefix + it->second + suffix;
      }
      return out;
    }
  }
}
