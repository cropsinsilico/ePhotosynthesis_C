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

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class PSModuleTest;
}
#endif
namespace modules {
class PS_PR;
/**
 Class to for PS related functions and common variables
 */
class PS : public ModuleBase<PS, conditions::PSCondition> {
public:
    static void setParam(const arr &param) {
        Param = param;
    }

    static std::size_t getN() {return N;}
private:
    friend ModuleBase;
    friend class modules::PS_PR;
#ifdef TESTING
    friend class test::PSModuleTest;
#endif
    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr _MB(const double t, const conditions::PSCondition* const PSs, Variables *theVars);
    static conditions::PSCondition* _MB_con(const double t, const conditions::PSCondition* const PSs,
                                            Variables *theVars);

    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A PSCon object with values set base on the input
      */
    static conditions::PSCondition* _init(Variables *theVars);

    /**
      Calculate the Rates of PS based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      */
    static void _Rate(const double t, const conditions::PSCondition* const PSs, Variables *theVars);

    static void _reset();
    SET_GET(PiTc)
    SET_GET(V1)
    SET_GET(KM12)
    SET_GET(KM11)
    SET_GET(V3)
    SET_GET(V6)
    SET_GET(V9)
    SET_GET(V13)
    SET_GET(V16)
    SET_GET(V23)
    SET_GET(Theta)
    SET_GET(beta)
    SET_GET(Jmax)

    SET_GET(KA231)
    SET_GET(KE11)
    SET_GET(KE12)
    SET_GET(KE13)
    SET_GET(KE16)
    SET_GET(KE21)
    SET_GET(KE22)
    SET_GET(KE23)
    SET_GET(KE25)
    SET_GET(KE4)
    SET_GET(KE5)
    SET_GET(KE6)
    SET_GET(KE7)
    SET_GET(KE8)
    SET_GET(KE9)
    SET_GET(KI11)
    SET_GET(KI12)
    SET_GET(KI13)
    SET_GET(KI131)
    SET_GET(KI132)
    SET_GET(KI133)
    SET_GET(KI134)
    SET_GET(KI135)
    SET_GET(KI14)
    SET_GET(KI15)
    SET_GET(KI231)
    SET_GET(KI61)
    SET_GET(KI62)
    SET_GET(KI9)
    SET_GET(KM10)
    SET_GET(KM101)
    SET_GET(KM102)
    SET_GET(KM103)
    SET_GET(KM13)
    SET_GET(KM131)
    SET_GET(KM132)
    SET_GET(KM161)
    SET_GET(KM162)
    SET_GET(KM163)
    SET_GET(KM21)
    SET_GET(KM22)
    SET_GET(KM23)
    SET_GET(KM231)
    SET_GET(KM232)
    SET_GET(KM233)
    SET_GET(KM234)
    SET_GET(KM241)
    SET_GET(KM311)
    SET_GET(KM313)
    SET_GET(KM31a)
    SET_GET(KM32)
    SET_GET(KM32b)
    SET_GET(KM33)
    SET_GET(KM51)
    SET_GET(KM52)
    SET_GET(KM53)
    SET_GET(KM61)
    SET_GET(KM81)
    SET_GET(KM82)
    SET_GET(KM9)
    SET_GET(KVmo)
    SET_GET(PS_C_CA)
    SET_GET(PS_C_CP)
    SET_GET(PS_C_CN)
    SET_GET(PS_PEXT)
    SET_GET(V24)
    SET_GET(V31)
    SET_GET(V32)
    SET_GET(V33)
    SET_GET(V2)
    SET_GET(V5)
    SET_GET(V7)
    SET_GET(V8)
    SET_GET(V10)
    SET_GET(V1Reg)
    SET_GET(KA232)
    SET_GET(KA233)
    SET_GET(KI23)
    SET_GET(KM312)
    SET_GET(KE10)
    SET_GET(KM71)
    SET_GET(KM72)
    SET_GET(KM73)
    SET_GET(KM74)
    SET_GET(Vfactor1)
    SET_GET(Vfactor2)
    SET_GET(Vfactor3)
    SET_GET(Vfactor5)
    SET_GET(Vfactor7)
    SET_GET(Vfactor13)
    SET_GET(Vfactor23)
    SET_GET(Vf_T3)
    SET_GET(Vf_T2)
    SET_GET(Vf_T1)
    SET_GET(Vf_T6)
    SET_GET(Vf_T5)
    SET_GET(Vf_T9)
    SET_GET(Vf_T13)
    SET_GET(Vf_T23)
    SET_GET(PsV1)
    SET_GET_BOOL_MODULE(C3, conditions::PS)
    SET_GET(_NADPH)

    static double KE1Ratio;
    static double KE2Ratio;
    static double PsV1_0;
    static double PsV2_0;
    static double PsV3_0;
    static double PsV5_0;
    static double PsV6_0;
    static double PsV7_0;
    static double PsV8_0;
    static double PsV9_0;
    static double PsV10_0;
    static double PsV13_0;
    static double PsV16;
    static double PsV23_0;
    static double PsV31;
    static double PsV32;
    static double PsV33;
    static double Ru_Act;
    static double PsV2;
    static double PsV3;
    static double PsV5;
    static double PsV6;
    static double PsV7;
    static double PsV8;
    static double PsV9;
    static double PsV10;
    static double PsV13;
    static double PsV23;
    static double I2;
    static double J;
    static double KE57;
    static double Km8p5p;
    static double Km5p5p;
    static double KE810;
    static double Km5gap;
    static double Km8f6p;
    static double Km8s7p;
    static double Km8gap;
    static double MaxCoeff;

    static arr Param;
    static double TIME;
    static std::size_t N;
};

}  // namespace modules
}  // namespace ePhotosynthesis
