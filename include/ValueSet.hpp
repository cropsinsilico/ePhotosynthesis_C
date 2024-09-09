#pragma once

/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include "definitions.hpp"
#include "enums/enums_utils.hpp"

namespace ePhotosynthesis {

  // TODO: ConditionBase should be generic
#define COMPARE_COND_VALUES(cond, module, k)		\
  {							\
    double a = cond->k;					\
    double b = cond->get(module ## _COND_ ## k);	\
    cond->compareValues(k, a, b);			\
  }
#define COMPARE_VARS_VALUES(theVars, id, module, k)			\
  {									\
    double a = theVars->module ## _ ## id.k;				\
    double b = theVars->id[MODULE_ ## module]->get(k);			\
    ValueSet::compareValues(k, a, b, #id, #module, #k);			\
  }

#define SET_VALUE(module, var, k, v)		\
  var.k = v;					\
  var[module ## _ ## k] = v

  /**
     Template class to provide methods for manipulating a set of values
     used by a model run that can be set via input files.

     \tparam ID Enum corresponding to the module that the values will be
       used by.
     \tparam PT Enum corresponding to the module component that the values
       will be used by.
   */
  template<MODULE ID, PARAM_TYPE PT>
  class ValueSet {
  public:
    typedef typename MODULE2Enum<ID, PT>::Type EnumType; /**< Enumerator type specifying keys for values in the set */
    typedef typename std::map<EnumType, double>::iterator iterator; /**< Iterator type for values in the set */
    typedef typename std::map<EnumType, double>::const_iterator const_iterator; /**< Constant iterator type for values in the set */
    static const MODULE MODULE = ID; /**< Enum corresponding to the module that the values will be used by */
    static const PARAM_TYPE PARAM_TYPE = PT; /**< Enum corresponding to the module component that the values will be used by */

    /**
       Default constructor. Values will be initialized with default
         values, minus any values that are skipped.
     */
    ValueSet() : values() {
      update_values(default_values());
    }
    /**
       Copy constructor. Values will be updated from the values in the
         copied set.
     */
    ValueSet(const ValueSet& other) :
      values() {
      update_values(other.values);
    }
    /**
       Re-initialize the values to the default values, minus any values
         that are skipped.
     */
    void init() { update_values(default_values()); }
    /**
       Get the current number of values in the set.
       \returns Number of values in the set.
     */
    virtual std::size_t size_values() const {
      return values.size();
    }
    /**
       Get the full set of default values associated with the set.
       \returns Mapping between set keys and default values.
     */
    static const std::map<EnumType, double>& default_values() {
      return get_enum_defaults<EnumType>();
    }
    /** Get the iterator pointing to the start of the value set */
    iterator begin() { return values.begin(); }
    /** Get the iterator pointing to the end of the value set */
    iterator end() { return values.end(); }
    /** Get the constant iterator pointing to the start of the value set */
    const_iterator begin() const { return values.begin(); }
    /** Get the constant iterator pointing to the end of the value set */
    const_iterator end() const { return values.end(); }
    // Skipped value manipulation
    /**
       Get the keys for values in the set that are currently skipped
         (unused).
       \return Skipped value keys.
     */
    static std::vector<EnumType>& skipped_values() {
      return skipped_enum_param<EnumType>();
    }
    /**
       Add a key to the set of keys skipped (unused) by the set. The key
         will continue to be skipped until reset or unskip is called.
       \param k Key to skip.
     */
    static void skip(const EnumType k) {
      skip_enum_param(k);
    }
    /**
       Remove a key from the set of keys skipped (unused) by the set.
       \param k Key to no-longer.
     */
    static void unskip(const EnumType k) {
      skip_enum_param(k, true);
    }
    /**
       Determine if a key is currently skipped (unused) by the set.
       \param k Key to check.
       \returns true if the key is skipped, false otherwise.
     */
    static bool isSkipped(const EnumType k) {
      for (typename std::vector<EnumType>::iterator it = skipped_values().begin();
	   it != skipped_values().end(); it++) {
        if (*it == k)
          return true;
      }
      return false;
    }
    /**
       Remove all keys from the list of keys skipped by the set.
     */
    static void reset_skipped() {
      skipped_enum_param<EnumType>(true);
    }
    /**
       Throw an error if a key is currently being skipped by the set.
       \param k Key to check.
       \context String to prefix error message with.
     */
    static void checkSkipped(const EnumType k, const std::string& context = "") {
      if (isSkipped(k)) {
	std::string msg = "\'" + utils::enum_key2string(k) + "\' skipped";
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(msg);
      }
    }
    // Inspection utilities
    /**
       Display the values in the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    virtual std::ostream& print(std::ostream &out, const uint tab = 0) const {
      const std::string space(tab * 4, ' ');
      for (typename std::map<EnumType, double>::const_iterator it = values.begin();
	   it != values.end(); it++)
	out << space << utils::enum_key2string(it->first) << " = " <<
	  it->second << std::endl;
      return out;
    }
    /**
       Display the default values for the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_defaults(std::ostream &out, const uint tab = 0) {
      const std::string space(tab * 4, ' ');
      for (typename std::map<EnumType, double>::const_iterator it = default_values().begin();
	   it != default_values().end(); it++)
	out << space << utils::enum_key2string(it->first) << " = " <<
	  it->second << std::endl;
      return out;
    }
    /**
       Display the keys currently skipped by the set.
       \param out Output stream.
       \param tab Number of tabs to prefix each line in the output with.
       \returns Output stream.
     */
    static std::ostream& print_skipped(std::ostream &out, const uint tab = 0) {
      const std::string space(tab * 4, ' ');
      out << space << "[";
      for (typename std::vector<EnumType>::const_iterator it = skipped_values().begin();
	   it != skipped_values().end(); it++)
	out << utils::enum_key2string(*it) << ", ";
      out << "]" << std::endl;
      return out;
    }
    /**
       Throw an error if two values are not equivalent.
       \param k Key that values correspond to.
       \param a First value for comparison.
       \param b Second value for comparison.
       \param context String to prefix the error message with.
     */
    void compareValues(const EnumType k, const double& a, const double& b,
		       const std::string& context = "") {
      if (a != b) {
	std::string msg = "Values for " +
	  utils::enum_key2string(MODULE) + " " +
	  utils::enum_key2string(PARAM_TYPE) + " " +
	  utils::enum_key2string(k) +
	  " differ: " +
	  std::to_string(a) + " vs " +
	  std::to_string(b);
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(msg);
      }
    }
    /**
       Throw an error if a key is not present in the set.
       \param k Key to check.
       \param context String to prefix the error message with.
     */
    void check(const EnumType k, const std::string& context = "") const {
      typename std::map<EnumType, double>::const_iterator it = values.find(k);
      if (it == values.end()) {
	std::string msg = "\'" +
	  utils::enum_key2string(k) +
	  "\' not present";
	if (!context.empty())
	  msg = context + ": " + msg;
	throw std::runtime_error(msg);
      }
    }
    // Value manipulation
    /**
       Add values to the set.
       \param new_values New values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    void update_values(const std::map<EnumType, double>& new_values,
		       bool clear_existing = false) {
      if (clear_existing)
	values.clear();
      values.insert(new_values.begin(), new_values.end());
      for (typename std::vector<EnumType>::const_iterator s = skipped_values().begin();
	   s != skipped_values().end(); s++) {
	typename std::map<EnumType, double>::iterator it = values.find(*s);
	if (it != values.end())
	  values.erase(it);
      }
    }
    /**
       Add values to the set by reading a parameter file.
       \param filename File containing values to add to the set.
       \param clear_existing If true, any existing keys will be removed
         from the set. If false, any existing keys will remain in the set
	 unless overwritten by an entry in new_values.
     */
    void update_values(const std::string& filename,
		       bool clear_existing = false) {
      std::map<EnumType, double> new_values = utils::readEnumFile<EnumType, double>(filename);
      update_values(new_values, clear_existing);
    }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    double& operator[](const EnumType k) {
      checkSkipped(static_cast<EnumType>(k), "[]");
      check(k, "[]");
      typename std::map<EnumType, double>::iterator it = values.find(k);
      return it->second;
    }
    /**
       Get the constant reference for the value associated with a key.
       \param k Key to get value for.
       \returns Constant value reference.
     */
    const double& operator[](const EnumType k) const {
      return const_cast<ValueSet*>(this)->operator[](k);
    }
    /**
       Update the value associated with a key.
       \param k Key to update value for.
       \param v New value for key.
     */
    virtual void set(const EnumType k, const double v) { (*this)[k] = v; }
    /**
       Get the reference for the value associated with a key.
       \param k Key to get value for.
       \returns Value reference.
     */
    virtual double get(const EnumType k) const { return (*this)[k]; }
    /**
       Reset all of the values to 0;
     */
    virtual void reset_values() {
      for (typename std::map<EnumType, double>::iterator it = values.begin();
	   it != values.end(); it++)
	it->second = 0.0;
    }
    /**
       Get the values in an array ordering based on key order.
       \returns Values array.
     */
    virtual arr toArray() const {
      arr out;
      size_t i = 0;
      out.resize(values.size());
      for (typename std::map<EnumType, double>::const_iterator it = values.begin();
	   it != values.end(); it++, i++)
	out[i] = it->second;
      return out;
    }
    /**
       Update the values in the set from an array.
       \param vec Array to update values from.
       \param offset Index in vec to start from.
     */
    virtual void fromArray(const arr &vec, const std::size_t offset = 0) {
      size_t i = 0;
      if (size_values() > (vec.size() - offset))
	throw std::runtime_error(std::to_string(size_values()) +
				 " expected, but only " +
				 std::to_string(vec.size() - offset) +
				 " provided (vec.size() = " +
				 std::to_string(vec.size()) +
				 ", offset = "+
				 std::to_string(offset) + ")");
      for (typename std::map<EnumType, double>::iterator it = values.begin();
	   it != values.end(); it++, i++)
	it->second = vec[offset + i];
    }
    std::map<EnumType, double> values; /**< Values in the set */
  };
}
