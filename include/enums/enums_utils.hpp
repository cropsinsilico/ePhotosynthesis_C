#pragma once

#include <map>
#include <string>
#include "definitions.hpp"
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <sstream>

#include "enums/enums.hpp"

const boost::regex token("\\s+");

std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::MODULE& x);
std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::PARAM_TYPE& x);

namespace ePhotosynthesis {
  namespace utils {

    EPHOTO_API std::string str_toupper(const std::string& inStr);
    EPHOTO_API std::string str_tolower(const std::string& inStr);
    EPHOTO_API std::vector<std::string> str_split(const std::string& inStr,
                                                  const std::string& sep);

    template<typename T1>
    bool enum_key_search(const std::map<T1, std::string> map,
			 const T1& key,
			 std::string& val) {
      for (typename std::map<T1, std::string>::const_iterator it = map.cbegin();
	   it != map.cend(); it++) {
	if (it->first == key) {
	  val = it->second;
	  return true;
	}
      }
      return false;
    }
			 
    template<typename T1>
    bool enum_value_search(const std::map<T1, std::string> map,
			   const std::string& val,
			   T1& key, bool allow_anycase=false,
			   std::string prefix="",
			   std::string suffix="") {
      for (typename std::map<T1, std::string>::const_iterator it = map.cbegin();
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
    
    template<typename T>
    T enum_string2key(const std::string& val) {
      const std::map<T, std::string>& map = get_enum_names<T>();
      T key;
      if (!enum_value_search(map, val, key))
	throw std::runtime_error("Could not locate enum for \"" + val
				 + "\"");
      return key;
    }
    template<typename T>
    std::string enum_key2string(const T& key) {
      const std::map<T, std::string>& map = get_enum_names<T>();
      std::string val;
      if (!enum_key_search(map, key, val))
	throw std::runtime_error("Could not locate enum for " +
				 // T::error_prefix() +
				 std::to_string((int)key));
      return val;
    }

    template<typename Tenum, typename Tval>
    std::map<Tenum, Tval> readEnumFile(const std::string &filename) {
      std::map<Tenum, Tval> map;
      std::vector<std::string> tempVec;
      std::string comment = "#";
      std::string input;
      std::ifstream inputfile(filename);
      Tenum key;
      Tval val;
      if(inputfile.fail()) {
        std::string errmsg = "Could not open " + filename + " for reading";
        throw std::runtime_error(errmsg);
      }
      int count = 0;
      while (getline(inputfile, input)) {
        if (input.empty() || (input.rfind(comment, 0) == 0))
	  continue;
        boost::algorithm::split_regex(tempVec, input, token);
	key = enum_string2key<Tenum>(tempVec[0]);
        std::stringstream ss(tempVec[1]);
        ss >> val;
        count++;
        map.insert(std::pair<Tenum, Tval>(key, val));
      }
      return map;
    }
    
    template<typename T1, typename T2>
    T1 max_enum_value(const std::map<T1, T2> map) {
      return map.crbegin()->first;
    }


    template<typename T, typename Tflag>
    std::string strBitFlags(const T& flag,
			    const std::map<Tflag, std::string>& map,
			    const std::string& prefix="",
			    const std::string& suffix="\n") {
      std::string out;
      for (typename std::map<Tflag, std::string>::const_iterator it = map.begin();
	   it != map.end(); it++) {
	if (flag & it->first)
	  out += prefix + it->second + suffix;
      }
      return out;
    }
  }
}
