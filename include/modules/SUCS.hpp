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
#include "conditions/SUCSCondition.hpp"
#include "definitions.hpp"

namespace ePhotosynthesis {
#ifdef TESTING
namespace test {
class SUCSModuleTest;
}
#endif
namespace modules {
class CM;

#define MEMBERS_SUCS_CONSTANT			\
  KI583,					\
    Vmatpf,					\
    Q10_51,					\
    Q10_52,					\
    Q10_55,					\
    Q10_56,					\
    Q10_57,					\
    Q10_58,					\
    SC,						\
    SC1
#define MEMBERS_SUCS_SKIPPED EMPTY_MEMBER_LIST
#define MEMBERS_SUCS_NOT_IN_ARRAY EMPTY_MEMBER_LIST
  
/**
 Class for SUCS related functions and common variables
 */
class SUCS : public ModuleBase<SUCS, conditions::SUCSCondition, MODULE_SUCS> {
public:
    DECLARE_VALUE_SET_STATIC(SUCS, ModuleBase<SUCS, conditions::SUCSCondition, MODULE_SUCS>)
private:
    friend ModuleBase;
    friend class modules::CM;
#ifdef TESTING
    friend class test::SUCSModuleTest;
#endif
    /**
      Function to set the initial state of the SUCSCondition class.

      \param theVars Pointer to the global variables
      \return A SUCSCondition object with values set based on the input
      */
    static conditions::SUCSCondition* _init(Variables *theVars);

    /**
      Function to calculate the dy/dt values for the SUCSCondition at the given time stamp.

      \param t The current timestamp
      \param SUCS_Con SUCSCondition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const conditions::SUCSCondition* const SUCS_Con, Variables *theVars);

    /**
      Function to calculate the dy/dt values for the SUCSCondition at the given time stamp.

      \param t The current timestamp
      \param SUCS_Con SUCSCondition object giving the input parameters
      \param theVars The global variables
      \return A SUCSCondition instance containing the dy/dt values for this time stamp.
      */
    static conditions::SUCSCondition* _MB_con(const double t,
                                              const conditions::SUCSCondition* const SUCS_Con,
                                              Variables *theVars);
    /**
      Calculate the Rates of SUCS based on the input SUCSCondition.

      \param t The current timestamp
      \param SUCS_Con SUCSCondition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const conditions::SUCSCondition* const SUCS_Con,
                      Variables *theVars);

    /**
      Reset the static member variables to their default values.
      */
    static void _reset();
    SET_GET(KE501)
    SET_GET(KE51)
    SET_GET(KE52)
    SET_GET(KE531)
    SET_GET(KE541)
    SET_GET(KE55)
    SET_GET(KE56)
    SET_GET(KE57)
    SET_GET(KE59)
    SET_GET(KE61)
    SET_GET(KI521)
    SET_GET(KI522)
    SET_GET(KI523)
    SET_GET(KI561)
    SET_GET(KI562)
    SET_GET(KI563)
    SET_GET(KI564)
    SET_GET(KI565)
    SET_GET(KI581)
    SET_GET(KI582)
    SET_GET(KI591)
    SET_GET(Ki572)
    SET_GET(Km511)
    SET_GET(Km512)
    SET_GET(Km513)
    SET_GET(Km521)
    SET_GET(Km551)
    SET_GET(Km552)
    SET_GET(Km553)
    SET_GET(Km554)
    SET_GET(Km561)
    SET_GET(Km562)
    SET_GET(Km571)
    SET_GET(Km581)
    SET_GET(Km591)
    SET_GET(Km593)
    SET_GET(Km621)
    SET_GET(V51)   ///< DHAP+GAP --FBP
    SET_GET(V52)   ///< FBP --F6P + Pi
    SET_GET(V55)   ///< G1P+UTP --OPOP+UDPG
    SET_GET(V56)   ///< UDPG+F6P--SUCP + UDP
    SET_GET(V57)   ///< SUCP--Pi + SUC
    SET_GET(V58)   ///< F26BP--F6P + Pi
    SET_GET(V59)   ///< F6P + ATP --ADP + F26BP
    SET_GET(V60)
    SET_GET(V61)
    SET_GET(V62)   ///< SUC Sink
    SET_GET(Vdhap_in) ///< DHAP export from chloroplast
    SET_GET(Vgap_in)  ///< GAP export from chloroplast
    SET_GET(Vpga_in)  ///< PGA export from chloropalst
    SET_GET(Km592)
    SET_GET(KI592)
    SET_GET(Km601)
    SET_GET(Km602)
    SET_GET(Km603)
    SET_GET(Km604)
    SET_GET(KE60)
    SET_GET(Vfactor51)
    SET_GET(Vfactor52)
    SET_GET(Vfactor56)
    SET_GET(Vfactor57)
    SET_GET(Vfactor59)
    SET_GET(Vf_T51)
    SET_GET(Vf_T52)
    SET_GET(Vf_T56)
    SET_GET(Vf_T57)
    SET_GET(Vf_T59)
    SET_GET(UTPc)
    SET_GET(ATPc)

    SET_GET(SUCS2PS_Pic)

    static double TIME;    // The timestamp of the most recent call to _Rate
    static std::size_t N;  // The current size of the SUCS TimeSeries
};

  DEFINE_VALUE_SET_STATIC_HEADER(SUCS);

}  // namespace modules
}  // namespace ePhotosynthesis
