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

double TargetFunVal(Variables *theVars);

void makeFluxTR(Variables *theVars);

/**
  Overloaded utility function to read a text file and convert it to a map. The input text file
  must be formatted with key/value pairs, one pair per line, and white space between
  the key and value. This overloaded version works where the values are std::string type.

  @param filename The name of the file to open and read.
  @param[in, out] mapper The std::map to put the contents of the file into
  @exception std::runtime_error If the opening of the file fails.
  */
void readFile(const std::string &filename, std::map<std::string, std::string> &mapper);

/**
  Overloaded utility function to read a text file and convert it to a map. The input text file
  must be formatted with key/value pairs, one pair per line, and white space between
  the key and value. This overloaded version works where the values are double type.

  @param filename The name of the file to open and read.
  @param[in, out] mapper The std::map to put the contents of the file into
  @exception std::runtime_error If the opening of the file fails.
  */
void readFile(const std::string &filename, std::map<std::string, double> &mapper);

}  // namespace ePhotosynthesis
