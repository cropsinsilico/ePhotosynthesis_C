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
#include "conditions/PSCondition.hpp"
#include "definitions.hpp"
class PR;
class RuACT;
class SUCS;
/**
 Class to for PS related functions and common variables
 */
class PS : public ModuleBase<PS, PSCondition> {
public:
    static void setParam(const arr &param) {
        Param = param;
    }
    static double _NADPH;

    static void setC3(const bool val) {
        useC3 = val;
        PSCondition::setC3(val);
    }
    static size_t getN() {return N;}
    SET_GET(PiTc)
private:
    friend ModuleBase;
    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const PSCondition* PSs, Variables *theVars);
    static PSCondition* _MB_con(const double t, const PSCondition* PSs, Variables *theVars);

    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A PSCon object with values set base on the input
      */
    static PSCondition* _init(Variables *theVars);

    /**
      Calculate the Rates of PS based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const PSCondition* PSs, Variables *theVars);

    friend PR;
    friend RuACT;
    friend SUCS;
    static double KA231;
    static double KE11;
    static double KE12;
    static double KE13;
    static double KE16;
    static double KE21;
    static double KE22;
    static double KE23;
    static double KE25;
    static double KE4;
    static double KE5;
    static double KE6;
    static double KE7;
    static double KE8;
    static double KE9;
    static double KI11;
    static double KI12;
    static double KI13;
    static double KI131;
    static double KI132;
    static double KI133;
    static double KI134;
    static double KI135;
    static double KI14;
    static double KI15;
    static double KI231;
    static double KI61;
    static double KI62;
    static double KI9;
    static double KM10;
    static double KM101;
    static double KM102;
    static double KM103;
    static double KM13;
    static double KM131;
    static double KM132;
    static double KM161;
    static double KM162;
    static double KM163;
    static double KM21;
    static double KM22;
    static double KM23;
    static double KM231;
    static double KM232;
    static double KM233;
    static double KM234;
    static double KM241;
    static double KM311;
    static double KM313;
    static double KM31a;
    static double KM32;
    static double KM32b;
    static double KM33;
    static double KM51;
    static double KM52;
    static double KM53;
    static double KM61;
    static double KM81;
    static double KM82;
    static double KM9;
    static double KVmo;
    static double PS_C_CA;
    static double PS_C_CP;
    static double PS_C_CN;
    static double PS_PEXT;
    static double V24;
    static double V31;
    static double V32;
    static double V33;
    static double V5;
    static double V7;
    static double V8;
    static double V10;
    static double KA232;
    static double KA233;
    static double KI23;
    static double KM312;
    static double KE10;
    static double KM11;
    static double KM12;
    static double KM71;
    static double KM72;
    static double KM73;
    static double KM74;
    static double Vfactor1;
    static double Vfactor2;
    static double Vfactor3;
    static double Vfactor5;
    static double Vfactor7;
    static double Vfactor13;
    static double Vfactor23;
    static double Vf_T3;
    static double Vf_T2;
    static double Vf_T1;
    static double Vf_T6;
    static double Vf_T5;
    static double Vf_T9;
    static double Vf_T13;
    static double Vf_T23;
    static double PsV1;
    static arr Param;
    static bool useC3;
    static double TIME;
    static size_t N;
};
