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

#define MEMBERS_PS				\
  KA231,					\
    KE11,					\
    KE12,					\
    KE13,					\
    KE16,					\
    KE21,					\
    KE22,					\
    KE23,					\
    KE25,					\
    KE4,					\
    KE5,					\
    KE6,					\
    KE7,					\
    KE8,					\
    KE9,					\
    KI11,					\
    KI12,					\
    KI13,					\
    KI131,					\
    KI132,					\
    KI133,					\
    KI134,					\
    KI135,					\
    KI14,					\
    KI15,					\
    KI231,					\
    KI61,					\
    KI62,					\
    KI9,					\
    KM10,					\
    KM101,					\
    KM102,					\
    KM103,					\
    KM13,					\
    KM131,					\
    KM132,					\
    KM161,					\
    KM162,					\
    KM163,					\
    KM21,					\
    KM22,					\
    KM23,					\
    KM231,					\
    KM232,					\
    KM233,					\
    KM234,					\
    KM241,					\
    KM311,					\
    KM313,					\
    KM31a,					\
    KM32,					\
    KM32b,					\
    KM33,					\
    KM51,					\
    KM52,					\
    KM53,					\
    KM61,					\
    KM81,					\
    KM82,					\
    KM9,					\
    KVmo,					\
    PS_C_CA,					\
    PS_C_CP,					\
    PS_PEXT,					\
    V24,					\
    V31,					\
    V32,					\
    V33,					\
    V2,						\
    V3,						\
    V6,						\
    V9,						\
    V13,					\
    V16,					\
    V23,					\
    V1,						\
    V5,						\
    V7,						\
    V8,						\
    V10,					\
    PS_C_CN,					\
    KA232,					\
    KA233,					\
    KI23,					\
    KM312,					\
    KE10,					\
    KM11,					\
    KM12,					\
    KM71,					\
    KM72,					\
    KM73,					\
    KM74,					\
    Vfactor1,					\
    Vfactor2,					\
    Vfactor3,					\
    Vfactor5,					\
    Vfactor7,					\
    Vfactor13,					\
    Vfactor23,					\
    Vf_T3,					\
    Vf_T2,					\
    Vf_T1,					\
    Vf_T6,					\
    Vf_T5,					\
    Vf_T9,					\
    Vf_T13,					\
    Vf_T23,					\
    PsV1,					\
    _NADPH,					\
    PiTc,					\
    V1Reg,					\
    Theta,					\
    beta,					\
    Jmax,					\
    KE1Ratio,					\
    KE2Ratio,					\
    PsV1_0,					\
    PsV2_0,					\
    PsV3_0,					\
    PsV5_0,					\
    PsV6_0,					\
    PsV7_0,					\
    PsV8_0,					\
    PsV9_0,					\
    PsV10_0,					\
    PsV13_0,					\
    PsV16,					\
    PsV23_0,					\
    PsV31,					\
    PsV32,					\
    PsV33,					\
    Ru_Act,					\
    PsV2,					\
    PsV3,					\
    PsV5,					\
    PsV6,					\
    PsV7,					\
    PsV8,					\
    PsV9,					\
    PsV10,					\
    PsV13,					\
    PsV23,					\
    I2,						\
    J,						\
    KE57,					\
    Km8p5p,					\
    Km5p5p,					\
    KE810,					\
    Km5gap,					\
    Km8f6p,					\
    Km8s7p,					\
    Km8gap,					\
    MaxCoeff,					\
    Q10_1,					\
    Q10_2,					\
    Q10_3,					\
    Q10_5,					\
    Q10_6,					\
    Q10_7,					\
    Q10_8,					\
    Q10_9,					\
    Q10_10,					\
    Q10_13,					\
    Q10_23,					\
    R,						\
    c_c,					\
    dHa_c,					\
    c_o,					\
    dHa_o,					\
    RegFactor,					\
    SC,						\
    SC1,					\
    STOM1,					\
    STOM2
#define MEMBERS_PS_CONSTANT			\
    Q10_1,					\
    Q10_2,					\
    Q10_3,					\
    Q10_5,					\
    Q10_6,					\
    Q10_7,					\
    Q10_8,					\
    Q10_9,					\
    Q10_10,					\
    Q10_13,					\
    Q10_23,					\
    R,						\
    c_c,					\
    dHa_c,					\
    c_o,					\
    dHa_o,					\
    RegFactor,					\
    SC,						\
    SC1,					\
    STOM1,					\
    STOM2
#define MEMBERS_PS_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_PS_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class to for PS related functions and common variables
 */
class PS : public ModuleBase<PS, conditions::PSCondition, MODULE_PS> {
public:
    DECLARE_VALUE_SET_STATIC(PS, ModuleBase<PS, conditions::PSCondition, MODULE_PS>)
    /**
      Set the initial parameters.

      \param param The initial parameters.
      */
    static void setParam(const arr &param) {
        Param = param;
    }

    /**
      Get the current size of the PS TimeSeries.

      \returns The current size of the TimeSeries.
      */
    static std::size_t getN() {return N;}
private:
    friend ModuleBase;
    friend class modules::PS_PR;
#ifdef TESTING
    friend class test::PSModuleTest;
#endif
    /**
      Function to calculate the dy/dt values for the PSCondition at the given time stamp.

      \param t The current timestamp
      \param PSs PSCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::PSCondition* const PSs, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the PSCondition at the given time stamp.

      \param t The current timestamp
      \param PSs PSCondition object giving the input parameters
      \param theVars The global variables
      \return A PSCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::PSCondition* _MB_con(const double t, const conditions::PSCondition* const PSs,
                                            Variables *theVars);

    /**
      Function to set the initial state of the PSCondition class.

      \param theVars Pointer to the global variables
      \return A PSCondition object with values set based on the input
      */
    static conditions::PSCondition* _init(Variables *theVars);

    /**
      Calculate the Rates of PS based on the input PSCondition.

      \param t The current timestamp
      \param PSs PSCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::PSCondition* const PSs, Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
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

    static arr Param;
    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the PS TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(PS);

}  // namespace modules
}  // namespace ePhotosynthesis
