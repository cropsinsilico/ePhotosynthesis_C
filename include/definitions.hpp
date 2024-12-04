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

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "ePhotosynthesis_export.h"
#include "macros.hpp"
#include <cmath>
#include <cstdlib>
#include <sundials/sundials_config.h>

#if ((SUNDIALS_VERSION_MAJOR > 6) || (SUNDIALS_VERSION_MAJOR == 6 && SUNDIALS_VERSION_MINOR >= 7))
#include <sundials/sundials_context.h>
#define SUNDIALS_CONTEXT_REQUIRED 1
#endif // 

#ifndef uint
#define uint unsigned int
#endif
#ifndef ushort
#define ushort unsigned short int
#endif

namespace ePhotosynthesis {

// predeclare main global data structure
class Variables;
/**
  The common flat data structure in this code.

  @typedef std::vector<double> arr
  */
typedef std::vector<double> arr;  // Shortcut for std::vector<double>

static std::vector<std::string> glymaID_order {
    "Glyma.19G046800", 
    "Glyma.08G165400",
    "Glyma.08G165500",
    "Glyma.04G015900",
    "Glyma.03G185800", // 5
    "Glyma.10G268500",
    "Glyma.07G142700",
    "Glyma.10G293500",
    "Glyma.11G226900",
    "Glyma.15G136200", // 10
    "Glyma.05G025300",
    "Glyma.01G026700",
    "Glyma.19G089100",
    "Glyma.06G094300",
    "Glyma.10G210700", // 15
    "Glyma.04G011900",
    "Glyma.17G015600",
    "Glyma.19G022900",
    "Glyma.07G028500",
    "Glyma.16G168000", // 20
    "Glyma.19G017200",
    "Glyma.08G044100",
    "Glyma.06G323700",
    "Glyma.10G086600",
    "Glyma.11G169700", // 25
    "Glyma.09G015500",
    "Glyma.15G012500",
    "Glyma.08G097300",
    "Glyma.06G017900",
    "Glyma.08G302600", // 30
    "Glyma.09G255200",
    "Glyma.04G051200",
    "Glyma.13G222300",
    "Glyma.13G204800",
    "Glyma.10G042000", // 35
    "Glyma.01G095900",
    "Glyma.09G024100"};

#ifdef INCDEBUG
namespace Debug {
enum DebugLevel : uint {Top = 0x1, Middle = 0x2, Low = 0x4, Delta=0x8, Internal=0x10};
enum RequestedDebug : uint {None = 0,
                            TopOnly = 1,
                            MiddleOnly = 2,
                            UpperTier = 3,
                            LowOnly = 4,
                            LowTop = 5,
                            LowerTier = 6,
                            All = 7,
                            DeltaOnly = 8,
                            TopDelta = 9,
                            MiddleDelta = 10,
                            UpperDelta = 11,
                            LowDelta = 12,
                            LowTopDelta = 13,
                            LowTierDelta = 14,
                            FullDelta = 15,
                            InternalOnly = 16,
                            TopInternal = 17,
                            MiddleInternal = 18,
                            UpperInternal = 19,
                            LowInternal = 20,
                            LowTopInternal = 21,
                            LowTierInternal = 22,
                            FullInternal= 23,
                            DeltaInternal = 24,
                            TopAll = 25,
                            MiddleAll = 26,
                            UpperAll= 27,
                            LowAll = 28,
                            LowTopAll = 29,
                            LowerTierAll = 30,
                            Full = 31
                           };
}

#define DEBUG_MESSAGE(x) if (x->debugLevel() & theVars->debuglevel) std::cout << (x) << std::endl;
#define DEBUG_INTERNAL(x) if (Debug::Internal & theVars->debuglevel) std::cout << (x) << std::endl;
#define DEBUG_DELTA(x) if (Debug::Delta & theVars->debuglevel) std::cout << "DELTA" << std::endl << (x) << std::endl << "END DELTA" << std::endl;
#else
#define DEBUG_MESSAGE(x)
#define DEBUG_INTERNAL(x)
#define DEBUG_DELTA(x)
#endif

/**
  @def SET_GET(NAME)
  Creates a *static double* private data member and public setter and getter functions for a class
  */
/**
  @def SET_GET_BOOL(NAME)
  Creates a *static boolean* private data member and public setter and getter functions for a class
  */
/**
  @def SET_GET_BOOL_MODULE(NAME)
  Creates a *static boolean* private data member and public setter and getter functions for a module class
  */

//! [SET_GET]
#define SET_GET(NAME) public:						\
  /** Get the value of NAME \returns The current value */		\
  static double get ## NAME() {						\
    initStaticMembers();						\
    return NAME;							\
  }									\
  /** Set the value of NAME \param val The value to set NAME to */	\
  static void set ## NAME(const double val) {				\
    initStaticMembers();						\
    setPreInit(ValueSetClass::NAME, val);				\
    NAME = val;								\
  }
//! [SET_GET]

//! [SET_GET_NOVS]
#define SET_GET_NOVS(NAME) public:\
    /** Get the value of NAME \returns The current value */\
    static double get ## NAME() {				      	\
      return NAME;							\
    }									\
    /** Set the value of NAME \param val The value to set NAME to */	\
    static void set ## NAME(const double val) {			    \
      NAME = val;						    \
    }								    \
    private:\
        EPHOTO_API static double NAME;
//! [SET_GET_NOVS]

//! [SET_GET_BOOL]
#define SET_GET_BOOL(NAME, SKIP) public:				\
    /** Get the value of NAME \returns The current value */\
    static bool get ## NAME() {return NAME;}\
    /** Set the value of NAME \param val The value to set NAME to */\
    static void set ## NAME(const bool val) {			    \
      NAME = val;						    \
      if (val) {						    \
	addSkipped(EnumClass::SKIP);				    \
      } else {							    \
	removeSkipped(EnumClass::SKIP);				    \
      }								    \
    }								    \
    private:\
        EPHOTO_API static bool NAME;
//! [SET_GET_BOOL]
  
//! [SET_GET_BOOL_NOSKIP]
#define SET_GET_BOOL_NOSKIP(NAME) public:			\
    /** Get the value of NAME \returns The current value */\
    static bool get ## NAME() {return NAME;}\
    /** Set the value of NAME \param val The value to set NAME to */\
    static void set ## NAME(const bool val) {NAME = val;}	    \
    private:\
        EPHOTO_API static bool NAME;
//! [SET_GET_BOOL_NOSKIP]

//! [SET_GET_BOOL_MODULE]
#define SET_GET_BOOL_MODULE(NAME, CON) public:\
    /** Get the value of NAME \returns The current value */\
    static bool get ## NAME() {return NAME;}\
    /** Set the value of NAME in this and the CON ## Condition class. \param val The value to set NAME to */\
    static void set ## NAME(const bool val) {\
        NAME = val;\
        CON ## Condition::set ## NAME(val);\
    }\
    private:\
        EPHOTO_API static bool NAME;
//! [SET_GET_BOOL_MODULE]

/**
  Helper function to initialize a vector of 1's of the given length.

  @param length The size of the vector to return
  @return A vector of 1's, with the input size.
  */
inline arr ones(const std::size_t length) { return arr(length, 1.); }

/**
  Helper function to initialize a vector of 0's of the given length.

  @param length The size of the vector to return
  @return A vector of 0's, with the input size.
  */
inline arr zeros(const std::size_t length) { return arr(length, 0.); }

/**
  Implementation of a storage structure for a time series of data.

  This class stores data in a time based series. The entries are indexed
  by the step number and contain a timestamp of the current step. Only one
  datum can be stored for each step, so subsequent additions of data for a
  given step number will overwrite the data.
  */
template <typename T>
class TimeSeries {
public:
    TimeSeries() {}
    ~TimeSeries() {}
    /**
      Insert data into the storage.

      @param step The step number associated with the data
      @param time The time associated with the data (0 time is the beginning of the calculations)
      @tparam input The data to be stored
      */
    void insert(std::size_t step, double time, T &input);

    /**
      Overload of the indexing operator to get the data for a specific step

      @param i The step to return the data for
      @return The data for the given step
      */
    T &operator[](std::size_t i) { return _data[i]; }

    /**
      Get the timestamp for the given step

      @param i The step to get the timestamp for, default is -1, which returns the timestamp of the last step
      @return The timestamp
      */
    double timestamp(const int i = -1) const {
        if (i < 0)
            return _timestamp[current];
        return _timestamp[i];
    }

    /**
      Retrieve the data from the last step

      @return The data from the last step
      */
    T getLastData() const { return _data.back(); }

    /**
      Retrieve the timestamp of the last step

      @return The timestep
      */
    double getLastTime() const { return _timestamp.back(); }

    /**
      Get the number of steps in the series
      */
    std::size_t size() const { return _step.size(); }

    /**
      Write the data to the given stream

      @param[in,out] of The stream to write to
      */
    void write(std::ofstream &of);

    /**
       Comparison operator.
       @param[in] other TimeSeries to compare against this one
       @returns true if other is equivalent to this instance, false
         otherwise.
     */
    bool operator==(const TimeSeries<T>& other) const;
    bool operator!=(const TimeSeries<T>& other) const {
	return (!operator==(other));
    }

private:
    std::size_t current = 0;
    std::vector<T> _data;            // a vector of the accumulated data
    std::vector<int> _step;          // the step number for each entry
    std::vector<double> _timestamp;  // the timestamp for each entry
};

// insert a new data set into the array
// If step already exists then replace the existing entry, otherwise
// add the new data to the vector
template <typename T>
void TimeSeries<T>::insert(std::size_t step, double time, T &input) {
    std::vector<int>::iterator it = std::find(_step.begin(), _step.end(), step);
    T vec(input);
    if (it == _step.end()) {
        _step.push_back(static_cast<int>(step));
        _timestamp.push_back(time);
        _data.push_back(vec);
    } else {
        size_t index = static_cast<size_t>(std::distance(_step.begin(), it));
        _timestamp[index] = time;
        _data[index] = vec;
    }
}

// write the contents of the class to the stream
template <typename T>
void TimeSeries<T>::write(std::ofstream &of) {
    for (std::size_t i = 0; i < _step.size(); i++) {
        of << _timestamp[i];
        for (const auto &e : _data[i])
            of << "," << e;
        of << std::endl;
    }
}

template <typename T>
bool TimeSeries<T>::operator==(const TimeSeries<T>& other) const {
    if (current != other.current) return false;
#define CHECK(name)					\
    if (name.size() != other.name.size())		\
      return false;					\
    for (std::size_t i = 0; i < name.size(); i++) {	\
      if (name[i] != other.name[i]) return false;	\
    }
    CHECK(_data);
    CHECK(_step);
    CHECK(_timestamp);
#undef CHECK
    return true;
}

inline void report(arr values) {
    std::cout << values[0] << "  =  ";
    for (uint i = 1; i < values.size(); i++) {
        std::cout << values[i] << "  ";
    }
    std::cout << std::endl;
}

}  // namespace ePhotosynthesis
