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

#include "con/RedoxRegCon.hpp"

/**
 Class for RedoxReg related calculations and common variables
 */
class RedoxReg{
public:
    /**
      Initialize the variables

      @param theVars The global variables
      @return A RedoxRegCon object for input into calculations
      */
    static RedoxRegCon* RedoxReg_Ini(Variables *theVars);

    /**
      Calculate the Rates of RedoxReg based on the inputs

      @param t The current timestamp
      @param RedoxReg_Con RedoxRegCon object giving the input parameters
      @param theVars The global variables
      */
    static void RedoxReg_Rate(const double t, const RedoxRegCon* RedoxReg_Con, Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param RedoxReg_Con BFCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr RedoxReg_Mb(const double t, const RedoxRegCon* RedoxReg_Con, Variables *theVars);

    static int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);
private:
    static double RedoxReg_VMAX13;
    static double RedoxReg_VMAX16;
    static double RedoxReg_VMAX6;
    static double RedoxReg_VMAX9;
};
