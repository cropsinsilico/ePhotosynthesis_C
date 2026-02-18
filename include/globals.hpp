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
#include <map>

namespace ePhotosynthesis {

// some ungrouped functions
/**
  Set some initial global variables

  @param t The current timestamp
  @param[in,out] theVars The global variables
  */
void Condition(double t, Variables *theVars);

/**
  Generate output

  @param t The current timestamp
  @param[in,out] theVars The global variables
  */
void GenOut(double t, Variables *theVars);

/**
  Set some initial global variables

  @param[in,out] theVars The global variables
  */
void IniModelCom(Variables *theVars);

/**
  Set parameters

  @param[in,out] theVars The global variables
  */
void ParamSet(Variables *theVars);

/**
  Set some initial global variables

  @param[in,out] theVars The global variables
  */
void SYSInitial(Variables *theVars);

double TargetFunVal(const Variables *theVars);

void makeFluxTR(Variables *theVars);

/**
   Write a map to file as a table.
   \tparam T Type of data in the columns.
   \param[in] filename The name of the file to save the table to.
   \param[in] mapper The map that should be written to the table.
   \param[in] delimiter The string that should be used to separate columns.
 */
template<typename T>
EPHOTO_API void writeTable(const std::string &filename,
                           const std::map<std::string, std::vector<T> > &mapper,
                           const std::string& delimiter = ",") {
  std::ofstream out;
  out.open(filename);
  // Header
  size_t N = 0;
  std::string msg_prefix = "writeTable[" + filename + "]: ";
  for (typename std::map<std::string, std::vector<T> >::const_iterator it = mapper.begin();
       it != mapper.end(); it++) {
    if (it == mapper.begin()) {
      N = it->second.size();
    } else {
      out << delimiter;
      if (it->second.size() != N) {
        throw std::runtime_error(msg_prefix + "Column \"" + it->first +
                                 "\" contains " + std::to_string(it->second.size()) +
                                 " elements, but there were " + std::to_string(N) +
                                 " elements in the first column.");
      }
    }
    out << it->first;
  }
  out << std::endl;
  // Body
  for (size_t i = 0; i < N; i++) {
    for (typename std::map<std::string, std::vector<T> >::const_iterator it = mapper.begin();
         it != mapper.end(); it++) {
      if (it != mapper.begin()) {
        out << delimiter;
      }
      out << it->second[i];
    }
    out << std::endl;
  }
  out.close();
}
  
/**
  Utility function to read a table from a text file and convert it to a map. The input text file
  must be formatted with the parameter names on the first line and values on subsequent lines
  with whitespace separations between names/values on each line.

  @param filename The name of the file to open and read.
  @param[in, out] mapper The std::map to put the contents of the file into
  @exception std::runtime_error If the opening of the file fails.
  */
EPHOTO_API void readTable(const std::string &filename,
                          std::map<std::string, std::vector<std::string> > &mapper);

/**
  Overloaded utility function to read a text file and convert it to a map. The input text file
  must be formatted with key/value pairs, one pair per line, and white space between
  the key and value. This overloaded version works where the values are std::string type.

  @param filename The name of the file to open and read.
  @param[in, out] mapper The std::map to put the contents of the file into
  @exception std::runtime_error If the opening of the file fails.
  */
EPHOTO_API void readFile(const std::string &filename,
                         std::map<std::string, std::string> &mapper);

/**
  Overloaded utility function to read a text file and convert it to a map. The input text file
  must be formatted with key/value pairs, one pair per line, and white space between
  the key and value. This overloaded version works where the values are double type.

  @param filename The name of the file to open and read.
  @param[in, out] mapper The std::map to put the contents of the file into
  @param[in] enzymes If True, the inputs are scales like enzymes.
  @exception std::runtime_error If the opening of the file fails.
  */
EPHOTO_API void readFile(const std::string &filename, std::map<std::string, double> &mapper, bool enzymes=false);

}  // namespace ePhotosynthesis
