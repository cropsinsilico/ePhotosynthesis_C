#pragma once

#include <map>
#include <string>
#include "definitions.hpp"
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <sstream>

#include "enums/enums.hpp"

const boost::regex token("\\s+");

EPHOTO_API std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::MODULE& x);
EPHOTO_API std::ostream& operator<<(std::ostream& out, const ePhotosynthesis::PARAM_TYPE& x);

namespace ePhotosynthesis {
  namespace utils {

    ///////////////////////////
    // String utilities
    ///////////////////////////
    EPHOTO_API std::string str_toupper(const std::string& inStr);
    EPHOTO_API std::string str_tolower(const std::string& inStr);
    EPHOTO_API std::vector<std::string> str_split(const std::string& inStr,
                                                  const std::string& sep);
    EPHOTO_API bool str_startswith(std::string const& value,
                                   std::string const& prefix);
    EPHOTO_API bool str_endswith(std::string const& value,
                                 std::string const& suffix);

    EPHOTO_API std::string str_getTabs(const uint tab = 0);
  
    template<typename T>
    std::ostream& print_string(const std::vector<T>& x,
                               std::ostream& out,
                               const unsigned int tab=0) {
      out << str_getTabs(tab) << "[";
      std::size_t i = 0;
      for (typename std::vector<T>::const_iterator it = x.begin();
           it != x.end(); it++, i++) {
        if (i > 0) out << ", ";
        out << *it;
      }
      out << "]";
      return out;
    }
    template<typename T1, typename T2>
    std::ostream& print_string(const std::map<T1, T2>& x,
                               std::ostream& out,
                               const unsigned int tab=0) {
      const std::string space = str_getTabs(tab);
      for (typename std::map<T1, T2>::const_iterator it = x.begin();
           it != x.end(); it++) {
        out << space << "  " << it->first << "      " << it->second << std::endl;
      }
      return out;
    }
    
    template<typename T>
    std::string to_string(const T& x,
                          typename std::enable_if<!std::is_same<T, const char[]>::value >::type* = 0) {
      std::ostringstream oss;
      oss << x;
      return oss.str();
    }      
    std::string to_string(const std::string& x);
    std::string to_string(const char* x);
    template<size_t N>
    inline std::string to_string(const char x[N]) {
      return std::string(x);
    }
    template<typename T>
    std::string to_string(const std::vector<T>& x,
                          const unsigned int tab=0) {
      std::ostringstream oss;
      print_string(x, oss, tab);
      return oss.str();
    }
    template<typename T1, typename T2>
    std::string to_string(const std::map<T1, T2>& x,
                          const unsigned int tab=0) {
      std::ostringstream oss;
      print_string(x, oss, tab);
      return oss.str();
    }

    ///////////////////////////
    // Value utilities
    ///////////////////////////
    bool values_equal(const double& a, const double& b);
    double extract_value(const double& x);
    double extract_value(const double* x);
    
    ///////////////////////////
    // Collection utilities
    ///////////////////////////
    template<typename T>
    bool contains(const std::vector<T>& v, const T& x) {
      typename std::vector<T>::const_iterator it;
      for (it = v.begin(); it != v.end(); it++) {
        if ((*(it)) == x) break;
      }
      return (it != v.end());
    }
    template<typename T1, typename T2>
    bool contains(const std::map<T1, T2>& v, const T1& x) {
      typename std::map<T1, T2>::const_iterator it = v.find(x);
      return (it != v.end());
    }
    template<typename T1, typename T2>
    bool contains(const std::map<T1, T2>& v, const T2& x) {
      typename std::map<T1, T2>::const_iterator it;
      for (it = v.begin(); it != v.end(); it++) {
        if (it->second == x) break;
      }
      return (it != v.end());
    }
    template<typename T1, typename T2>
    void copyInto(std::map<T1, T2>& dst, const std::map<T1, T2>& src) {
      typename std::map<T1, T2>::const_iterator it_src;
      typename std::map<T1, T2>::iterator it_dst;
      for (it_src = src.begin(); it_src != src.end(); it_src++) {
        it_dst = dst.find(it_src->first);
        if (it_dst == dst.end())
          dst.insert(*it_src);
        else
          it_dst->second = it_src->second;
      }
    }
    
    ///////////////////////////
    // Enum utilities
    ///////////////////////////
    template<typename T>
    const std::map<T, std::string>& get_enum_names() {
      typedef enum_helper_inv<T> helper;
      typedef ValueSetEnum<helper::module, helper::param_type> VS;
      return VS::names;
    }
    template<>
    EPHOTO_API const std::map<MODULE, std::string>& get_enum_names<MODULE>();
    template<>
    EPHOTO_API const std::map<PARAM_TYPE, std::string>& get_enum_names<PARAM_TYPE>();
    
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
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::vector<T>& x) {
    return utils::print_string(x, out);
  }
  template<typename T1, typename T2>
  std::ostream& operator<<(std::ostream& out, const std::map<T1, T2>& x) {
    return utils::print_string(x, out);
  }
}
