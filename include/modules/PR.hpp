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

#include "ModuleBase.hpp"
#include "conditions/PRCondition.hpp"

/**
  Class for grouping PR related functions and common variables
  */
class PR : public ModuleBase<PR, PRCondition> {
private:
    friend ModuleBase;
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A PRCon object with values set base on the input
      */
    static PRCondition* _init(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param PR_con PRCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const PRCondition* PR_con, Variables *theVars);
    static PRCondition* _MB_con(const double t, const PRCondition* const PR_con, Variables *theVars);
    /**
      Calculate the Rates of PR based on the inputs

      @param PR_con PRCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const PRCondition* PR_con, Variables *theVars);

    static double KC;
    static double KE113;
    static double KE122;
    static double KE123;
    static double KE124;
    static double KGc;
    static double KI1011;
    static double KI1012;
    static double KI1121;
    static double KI1122;
    static double KI113;
    static double KI1221;
    static double KI123;
    static double KI1311;
    static double KM1011;
    static double KM1012;
    static double KM112;
    static double KM1131;
    static double KM1132;
    static double KM121;
    static double KM1221;
    static double KM1222;
    static double KM123;
    static double KM1241;
    static double KM1242;
    static double KM1311;
    static double KO;
    static double KR;
    static double NADHc;
    static double NADc;
    static double PR_ADP;
    static double PR_ATP;
    static double V111;
    static double V112;
    static double V113;
    static double V121;
    static double V122;
    static double V123;
    static double V124;
    static double V131;
    static double V2T;
    static double Vfactor112;
    static double Vfactor113;
    static double Vfactor121;
    static double Vfactor122;
    static double Vfactor123;
    static double Vfactor124;
    static double Vfactor131;
    static double Vf_T131;
    static double Vf_T113;
    static double Vf_T123;
    static double Vf_T121;
    static double Vf_T122;
    static double Vf_T112;
};
