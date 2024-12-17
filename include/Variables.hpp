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
#include <map>
#include <memory>
#include "definitions.hpp"
#include "vel/FIVel.hpp"
#include "vel/BFVel.hpp"
#include "vel/PSVel.hpp"
#include "vel/PRVel.hpp"
#include "vel/RROEAVel.hpp"
#include "vel/RuACTVel.hpp"
#include "vel/XanCycleVel.hpp"
#include "vel/RedoxRegVel.hpp"
#include "vel/SUCSVel.hpp"
#include "ke/RROEAKE.hpp"

#include "pool/BFPool.hpp"
#include "pool/FIBFPool.hpp"
#include "pool/FIPool.hpp"
#include "pool/RROEAPool.hpp"
#include "pool/RuACTPool.hpp"
#include "pool/SUCSPool.hpp"

#include "rc/BFRC.hpp"
#include "rc/FIRC.hpp"
#include "rc/RROEARC.hpp"
#include "rc/RuACTRC.hpp"
#include "conditions/PSCondition.hpp"
#include "conditions/PRCondition.hpp"
#include "conditions/SUCSCondition.hpp"
#include "conditions/XanCycleCondition.hpp"

namespace ePhotosynthesis {

#ifdef SUNDIALS_CONTEXT_REQUIRED
extern std::shared_ptr<SUNContext> global_context; //!< Global context

/**
   Initialize the global SUNContext.
*/
void init_global_sundials_context();

/**
   Destroy the global SUNContext.
 */
void cleanup_global_sundials_context();
  
#endif // SUNDIALS_CONTEXT_REQUIRED
    
/**
  Structure to hold global variables
  */
class Variables {
public:
#ifdef SUNDIALS_CONTEXT_REQUIRED

private:
    std::shared_ptr<SUNContext> _context; /**< Sundials context */
    int _context_flags = 0; /**< Flags describing the context */
    /** Flags describing context traits */
    enum CONTEXT_FLAGS : int {
        CONTEXT_FLAG_GLOBAL = 0x00000001, //< The context is global
    };
public:
    /**
       Construct a new variables instance for a given sundials context
       \param[in, out] ctx Sundials context.
    */
    Variables(SUNContext* ctx = nullptr);
    /**
       Get the shared pointer to the Sundials context.
       \return Sundials context shared pointer.
     */
    std::shared_ptr<SUNContext> context_ptr() {
      return _context;
    }
    /**
       Get the Sundials context.
       \return Sundials context.
     */
    SUNContext& context() {
      return *(_context.get());
    }
#else // SUNDIALS_CONTEXT_REQUIRED
    /**
       Construct a new variables instance for a given sundials context
    */
    Variables() {}
#endif // SUNDIALS_CONTEXT_REQUIRED
    /**
       Copy constructor for a pointer to another variables instance.
         Some variables are not included in the default copy (e.g. alfa, 
	 fc, lightParam, CO2A, RedoxReg_MP, module *_Param values, & 
	 rates). To include these, use deepcopy.
       \param[in] other Variables instance to copy.
     */
    Variables(const Variables* other);
    Variables(const Variables& other);
    Variables* deepcopy() const;
    Variables& operator=(const Variables& other);
    friend std::ostream& operator<<(std::ostream &out, const Variables *in);

    bool record = false;
    bool BF_FI_com = false;
    bool EPS_SUCS_com = false;
    bool FIBF_PSPR_com = false;
    bool PR_PS_com = false;
    bool PSPR_SUCS_com = false;
    bool RROEA_EPS_com = false;
    bool RedoxReg_RA_com = false;
    bool RuACT_EPS_com = false;
    bool XanCycle_BF_com = false;
    bool saveMetabolite  = false;
    bool runBioCro       = false; //whether this is coupled run with BioCro

    int GP = 0;
    int GRNC = 0;
    int GRNT = 0;

    int RUBISCOMETHOD = 1;

    const double AVR = 30.;
    const double HPR = 4.66;
    const double O2 = 210.;

    double CO2_cond = 0.;

    double GLight = 0.;
    double O2_cond = 0.;
    double PS12ratio = 0.;
    double ADP = 0.;

    double Pi = 0.;

    double TestATPCost = 0.;
    double CO2_in = 0.;
    double TestLi = 0.;
    double PS2BF_Pi = 0.;
    double PS_PR_Param = 0;

    double Tp = 0;
    double alfa = 0.;
    double fc = 0.;
    double Phi_max = 0.;
    double lightParam = 0.;
    //YH:No longer use these scaling factors in the source code
    //You need to update the text input file if you change Vmax
    //const double alpha1 = 0.87;
    //const double alpha2 = 1.03;
    double sensitivity_sf = 1.0; //a scaling factor for enzymes

    //YH:Q10 values come from my Q10 fitting for LD11
    double Q10_1  = 2.21;
    double Q10_2  = 1.69;
    double Q10_3  = 2.00;
    double Q10_5  = 2.50;
    double Q10_6  = 2.17;
    double Q10_7  = 1.76;
    double Q10_8  = 1.91;
    double Q10_9  = 1.98;
    double Q10_10 = 2.02;
    double Q10_13 = 2.08;
    double Q10_23 = 2.50;
    double PS_scaling_factor = 1.0; //a parameter to scale the total P
    // Parameters
    arr PR_Param = zeros(2);
    arr BF_Param = zeros(2);
    arr FI_PARAM = zeros(2);
    arr FI_Param = zeros(2);
    arr RROEA_Param = zeros(2);
    arr RuACT_Param = zeros(2);
    arr SUCS_Param = zeros(2);
    arr XanCycle_Param = zeros(2);

    // Vel
    vel::BFVel BF_Vel;
    vel::FIVel FI_Vel;
    vel::PRVel PR_Vel;
    vel::PSVel PS_Vel;
    vel::RROEAVel RROEA_Vel;
    vel::RedoxRegVel RedoxReg_Vel;
    vel::RuACTVel RuACT_Vel;
    vel::SUCSVel SUCS_Vel;
    vel::XanCycleVel XanCycle_Vel;

    // Ratio
    arr BFRatio = ones(49);
    arr FIRatio = ones(23);
    arr PRRatio = ones(48);
    arr PSRatio = ones(103);
    arr RacRatio = ones(16);
    arr SUCRatio = ones(66);
    arr XanRatio = ones(4);

    std::map<std::string, double> EnzymeAct;

    arr VfactorCp = zeros(33);
    arr VfactorT = ones(28);

    // Pool
    pool::BFPool BF_Pool;
    pool::FIBFPool FIBF_Pool;
    pool::FIPool FI_Pool;
    pool::RROEAPool RROEA_Pool;
    pool::RuACTPool RuACT_Pool;
    pool::SUCSPool SUCS_Pool;

    // RC
    RC::BFRC BF_RC;
    RC::FIRC FI_RC;
    RC::RROEARC RROEA_RC;
    RC::RuACTRC RuACT_RC;

    // OUT
    arr BF2OUT = zeros(5);
    conditions::PRCondition PR2OUT;
    conditions::PSCondition PS2OUT;
    conditions::SUCSCondition SUCS2OUT;
    conditions::XanCycleCondition XanCycle2OUT;

    // misc
    arr FluxTR = {};
    KE::RROEAKE RROEA_KE;

    TimeSeries<vel::BFVel> BF_VEL = TimeSeries<vel::BFVel>();
    TimeSeries<std::vector<double> > CO2A = TimeSeries<std::vector<double> > ();
    TimeSeries<vel::FIVel> FI_VEL = TimeSeries<vel::FIVel> ();
    TimeSeries<vel::PRVel> PR_VEL = TimeSeries<vel::PRVel> ();
    TimeSeries<vel::PSVel> PS_VEL = TimeSeries<vel::PSVel> ();
    TimeSeries<vel::RROEAVel> RROEA_VEL = TimeSeries<vel::RROEAVel> ();
    std::vector<arr> RedoxReg_MP;
    TimeSeries<vel::RedoxRegVel> RedoxReg_VEL = TimeSeries<vel::RedoxRegVel> ();
    TimeSeries<vel::RuACTVel> RuACT_VEL = TimeSeries<vel::RuACTVel> ();
    TimeSeries<vel::SUCSVel> SUCS_VEL = TimeSeries<vel::SUCSVel> ();
    TimeSeries<vel::XanCycleVel> XanCycle_VEL = TimeSeries<vel::XanCycleVel> ();

    bool useC3 = false;
#ifdef INCDEBUG
    Debug::RequestedDebug debuglevel = Debug::None;
#endif
};

}  // namespace ePhotosynthesis
