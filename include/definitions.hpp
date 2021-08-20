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

namespace ePhotosynthesis {

// predeclare main global data structure
class Variables;

#define Q10_1 1.93
#define Q10_2 2.
#define Q10_3 2.
#define Q10_5 2.
#define Q10_6 2.
#define Q10_7 2.
#define Q10_8 2.
#define Q10_9 2.
#define Q10_10 2.
#define Q10_13 2.
#define Q10_23 2.
#define Q10_112 1.81
#define Q10_113 2.
#define Q10_121 2.
#define Q10_122 2.01
#define Q10_123 2.
#define Q10_124 2.
#define Q10_131 2.
#define Q10_51 2.
#define Q10_52 1.60
#define Q10_55 2.
#define Q10_56 2.
#define Q10_57 2.
#define Q10_58 2.

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
#endif

#define SET_GET(NAME) public:\
    static double get ## NAME() {return NAME;}\
    static void set ## NAME(const double val) {NAME = val;}\
    private:\
        static double NAME;

#define SET_GET_BOOL(NAME) public:\
    static bool get ## NAME() {return NAME;}\
    static void set ## NAME(const bool val) {NAME = val;}\
    private:\
        static bool NAME;

#define SET_GET_BOOL_MODULE(NAME, CON) public:\
    static bool get ## NAME() {return NAME;}\
    static void set ## NAME(const bool val) {\
        NAME = val;\
        CON ## Condition::set ## NAME(val);\
    }\
    private:\
        static bool NAME;

// rename a common data type to make it easier
typedef std::vector<double> arr;

/**
  Helper function to initialize a vector of 1's of the given length.

  @param length The size of the vector to return
  @return A vector of 1's, with the input size.
  */
inline arr ones(const size_t length) { return arr(length, 1.); }

/**
  Helper function to initialize a vector of 0's of the given length.

  @param length The size of the vector to return
  @return A vector of 0's, with the input size.
  */
inline arr zeros(const size_t length) { return arr(length, 0.); }

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
    void insert(size_t step, double time, T &input);

    /**
      Overload of the indexing operator to get the data for a specific step

      @param i The step to return the data for
      @return The data for the given step
      */
    T &operator[](size_t i) { return _data[i]; }

    /**
      Get the timestamp for the given step

      @param i The step to get the timestamp for, default is -1, which returns the timestamp of the last step
      @return The timestamp
      */
    double timestamp(const int i = -1) {
        if (i < 0)
            return _timestamp[current];
        return _timestamp[i];
    }

    /**
      Retrieve the data from the last step

      @return The data from the last step
      */
    T getLastData() { return _data.back(); }

    /**
      Retrieve the timestamp of the last step

      @return The timestep
      */
    double getLastTime() { return _timestamp.back(); }

    /**
      Get the number of steps in the series
      */
    size_t size() { return _step.size(); }

    /**
      Write the data to the given stream

      @param[in,out] of The stream to write to
      */
    void write(std::ofstream &of);

private:
    size_t current = 0;
    std::vector<T> _data;            // a vector of the accumulated data
    std::vector<int> _step;          // the step number for each entry
    std::vector<double> _timestamp;  // the timestamp for each entry
};

// insert a new data set into the array
// If step already exists then replace the existing entry, otherwise
// add the new data to the vector
template <typename T>
void TimeSeries<T>::insert(size_t step, double time, T &input) {
    std::vector<int>::iterator it = std::find(_step.begin(), _step.end(), step);
    T vec(input);
    if (it == _step.end()) {
        _step.push_back(step);
        _timestamp.push_back(time);
        _data.push_back(vec);
    } else {
        int index = std::distance(_step.begin(), it);
        _timestamp[index] = time;
        _data[index] = vec;
    }
}

// write the contents of the class to the stream
template <typename T>
void TimeSeries<T>::write(std::ofstream &of) {
    for (size_t i = 0; i < _step.size(); i++) {
        of << _timestamp[i];
        for (const auto &e : _data[i])
            of << "," << e;
        of << std::endl;
    }
}


inline void report(arr values) {
    std::cout << values[0] << "  =  ";
    for (uint i = 1; i < values.size(); i++) {
        std::cout << values[i] << "  ";
    }
    std::cout << std::endl;
}

}  // namespace ePhotosynthesis
