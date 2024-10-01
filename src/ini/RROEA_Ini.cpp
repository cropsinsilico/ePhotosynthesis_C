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

#include "Variables.hpp"
#include "modules/RROEA.hpp"
#include "modules/PS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double RROEA::TIME = 0.;
std::size_t RROEA::N = 1;
const std::size_t RROEACondition::count = 10;

DEFINE_VALUE_SET_STATIC(RROEA);
DEFINE_VALUE_SET(RROEACondition);
DEFINE_VALUE_SET_NS(RC::, RROEARC);
DEFINE_VALUE_SET_NS(KE::, RROEAKE);
DEFINE_VALUE_SET_NS(pool::, RROEAPool);

RROEACondition* RROEA::_init(Variables *theVars) {

    double Coeff = 1.;
    theVars->RROEA_RC.Coeff = Coeff;
    
    // The rate constant used in the model
    theVars->RROEA_RC.ke2GAPDH = 22. / 60. * Coeff;    // The rate constant of electron transfer to GAPDH. From literature.
    theVars->RROEA_RC.ke2MDH = 20. / 60. * Coeff;      // The rate constant of electront transfer to MDH, this rate is totally ASSUMED.
    theVars->RROEA_RC.ke2FBPase = 1.38 / 60. * Coeff; // The rate constant of electron transfer from thioredoxin to FBPase.
    theVars->RROEA_RC.ke2SBPase = 1.65 / 60. * Coeff; // The rate constant of electron tranfer from thioredoxin to SBPase
    theVars->RROEA_RC.ke2PRK = 59.8 / 60. * Coeff;    // The rate constant of electron transfer from thioredoxin to PRK, Phosphoribulase kinase
    theVars->RROEA_RC.ke2RubACT = 6.35 / 60. * Coeff; // The rate constant of electron transfer from thioredoxin to Rubisco activase
    theVars->RROEA_RC.ke2Fd = 18.5 * Coeff;          // The rate constant of electron transfer to fe
    // Jmax of 180 micro mole per meter square per second.
    theVars->RROEA_RC.keFd2Thio = 10. * Coeff;        // The rate constant of electron transfer from fd to thio
    theVars->RROEA_RC.keFd2Calvin = 7. * Coeff;       // The rate constant of electron transfer from fd to Calvin cycle
    // Of course, this is a big assumption where the electron is transfered to NADPH
    // then to the Calvin cycle. This rate is much higher than the electron transfer to differnet
    // enzymes.

    theVars->RROEA_RC.ke2ATPGPP = 6.3 / 60 * Coeff;     // The rate constant of electron transfer to ATPGPP
    theVars->RROEA_RC.ke2ATPase = 1.0;                  // The rate constant of electron transfer to ATPase

    // Here is all the equilibriun constants for the different reactions in photosystem
    theVars->RROEA_KE.KEe2FBPase = 0.311167869;
    theVars->RROEA_KE.KEe2SBPase = 0.459194309;
    theVars->RROEA_KE.KEe2PRK = 0.677638775;
    theVars->RROEA_KE.KEe2ATPase = 1.0; // 2.177727336 was set, but unused, 1.0 is used in RROEA_Rate
    theVars->RROEA_KE.KEe2RuACT = 0.677638775;
    theVars->RROEA_KE.KEe2GAPDH = 0.044461692;
    theVars->RROEA_KE.KEe2MDH = 0.044461692;
    theVars->RROEA_KE.KEe2ATPGPP = 1.;
    theVars->RROEA_KE.KEeFd2Thio = 24776.;

    // The following calculate the total concentration of different enzymes.
    const double MDH_Vmax = 2;       // This value is assumed and there is no literature about it. Need to be
    // fixed.
    theVars->RROEA_Pool.MDH_Vmax = MDH_Vmax;

    double SC;
    double fc16;
    double FC;
    if (!theVars->RROEA_EPS_com) {

        FC = 1.;
        fc16 = 1.;
        SC = 1.;
        RROEA::FC = FC;
        RROEA::fc16 = fc16;
        RROEA::SC = SC;

        PS::setV3(5.04 * SC);       // (Harris & Koniger, 1997) 3 GAP dehydragenase DPGA+NADPH <->GAP + OP+NADP
        PS::setV6(1.155 * SC);      // (Harris & Koniger, 1997) 6 FBPase FBP<->F6P+OP    1.155
        PS::setV9(0.168 * SC * FC); // (Harris & Koniger, 1997) 9 SBPase SBP<->S7P+OP    0.168 as original value; 0.4168 was its value.
        PS::setV13(8.0094 * SC);    // (Harris & Koniger, 1997) 13 Ribulosebiphosphate kinase Ru5P+ATP<->RuBP+ADP
        PS::setV16(3. * SC * fc16);  // (Aflalo & Shavit, 1983, Davenport & McLeod, 1986) 16 ATP synthase ADP+Pi<->ATP    1.47
        PS::setV23(1.68 * SC * FC); // (Latzko, Steup & Schachtele, 1981) 23 ADP-glucose pyrophosphorylase and ADPG+Gn<->G(n+1)+ADP 0.18
    }


    const double NA = 100.;

    const double SA_GAPDH = 620.;
    const double SA_MDH = 184.;
    const double SA_PRK = 410.;
    const double SA_FBPase = 119.;
    const double SA_SBPase = 70.;
    const double SA_ATPGPP = 10.;

    const double SA_ATPase = NA;

    const double mw_GAPDH = 147000.;
    const double mw_MDH = 87000.;
    const double mw_PRK = 40000.;

    const double mw_FBPase = 195000.;
    const double mw_SBPase = 66000.;
    const double mw_ATPGPP = 210000.;
    const double mw_ATPase = 500000.;

    double FdT = 0.081;

    theVars->RROEA_Pool.SA_GAPDH = SA_GAPDH;
    theVars->RROEA_Pool.SA_MDH = SA_MDH;
    theVars->RROEA_Pool.SA_PRK = SA_PRK;
    theVars->RROEA_Pool.SA_FBPase = SA_FBPase;
    theVars->RROEA_Pool.SA_SBPase = SA_SBPase;
    theVars->RROEA_Pool.SA_ATPGPP = SA_ATPGPP;
    theVars->RROEA_Pool.SA_ATPase = SA_ATPase;
    theVars->RROEA_Pool.mw_GAPDH = mw_GAPDH;
    theVars->RROEA_Pool.mw_MDH = mw_MDH;
    theVars->RROEA_Pool.mw_PRK = mw_PRK;
    theVars->RROEA_Pool.mw_FBPase = mw_FBPase;
    theVars->RROEA_Pool.mw_SBPase = mw_SBPase;
    theVars->RROEA_Pool.mw_ATPGPP = mw_ATPGPP;
    theVars->RROEA_Pool.mw_ATPase = mw_ATPase;

    if (theVars->RROEA_EPS_com)
        FdT = theVars->BF_Pool.kU_f;

    theVars->RROEA_Pool.GAPDH = PS::getV3() * 1000. * 60. / SA_GAPDH / mw_GAPDH;
    theVars->RROEA_Pool.FBPase = PS::getV6() * 1000. * 60. / SA_FBPase / mw_FBPase;
    theVars->RROEA_Pool.SBPase = PS::getV9() * 1000. * 60. / SA_SBPase / mw_SBPase;
    theVars->RROEA_Pool.PRK = PS::getV13() * 1000. * 60. / SA_PRK / mw_PRK;
    theVars->RROEA_Pool.ATPase = PS::getV16() * 1000. * 60. / SA_ATPase / mw_ATPase;
    theVars->RROEA_Pool.ATPGPP = PS::getV23() * 1000. * 60. / SA_ATPGPP / mw_ATPGPP;
    theVars->RROEA_Pool.MDH = MDH_Vmax * 1000. * 60. / SA_MDH / mw_MDH;
    theVars->RROEA_Pool.ThioT = 0.081;
    theVars->RROEA_Pool.FdT = FdT;
    theVars->RROEA_Pool.RuACTT = 0.0056;


    Coeff = 0.3;

    double Fd = 0.081 * Coeff; // The initial concentraiton of reduced fd

    RROEACondition* RROEA_con = new RROEACondition();
    RROEA_con->Coeff = Coeff;
    RROEA_con->GAPDH = theVars->RROEA_Pool.GAPDH * Coeff;   // The initial concentration of active GAPDH
    RROEA_con->FBPase = theVars->RROEA_Pool.FBPase * Coeff; // The initial concentration of active FBPase
    RROEA_con->SBPase = theVars->RROEA_Pool.SBPase * Coeff; // The initial concentration of active SBPase
    RROEA_con->PRK = theVars->RROEA_Pool.PRK * Coeff;       // The initial concentration of actove PRK
    RROEA_con->ATPase = theVars->RROEA_Pool.ATPase * Coeff; // The initial concentration of actove ATPase
    RROEA_con->ATPGPP = theVars->RROEA_Pool.ATPGPP * Coeff; // The initial concentration of actove ATPGPP
    RROEA_con->MDH = 0.;                                     // The initial concentration of actove MDH
    RROEA_con->Thio = 0.081 * Coeff;                        // The initial concentration of reduced thioredoxin
    RROEA_con->Fd = Fd;                                     // The initial concentration of reduced ferrodoxin
    RROEA_con->RuACT = 0.0056 * Coeff;                      // The initial concentration of active Rubisco activase

    // Here defines the information for transfer between models

    return RROEA_con;
}

RROEACondition* RROEA::_initAlt(Variables *theVars, RROEACondition* RROEA_con) {
#ifdef CHECK_VALUE_SET_ALTS
    theVars->initParamStatic<RROEA>();
    theVars->initParam(theVars->RROEA_RC);
    theVars->initParam(theVars->RROEA_KE);
    theVars->initParam(theVars->RROEA_Pool);
    theVars->initParam(*RROEA_con);
  
    for (typename RC::RROEARC::iterator it = theVars->RROEA_RC.begin();
         it != theVars->RROEA_RC.end(); it++) {
      if (it->first == RC::RROEA::Coeff)
        continue;
      if (it->first != RC::RROEA::ke2Fd &&
          it->first != RC::RROEA::keFd2Thio &&
          it->first != RC::RROEA::keFd2Calvin &&
          it->first != RC::RROEA::ke2ATPase)
        it->second /= 60.0;
      if (it->first != RC::RROEA::ke2ATPase)
        it->second *= theVars->RROEA_RC[RC::RROEA::Coeff];
    }

    if (!theVars->RROEA_EPS_com) {
      PS::set(MOD::PS::V3, 5.04 * RROEA::get(MOD::RROEA::SC));
      PS::set(MOD::PS::V6, 1.155 * RROEA::get(MOD::RROEA::SC));
      PS::set(MOD::PS::V9, 0.168 * RROEA::get(MOD::RROEA::SC) *
              RROEA::get(MOD::RROEA::FC));
      PS::set(MOD::PS::V13, 8.0094 * RROEA::get(MOD::RROEA::SC));
      PS::set(MOD::PS::V16, 3. * RROEA::get(MOD::RROEA::SC) *
              RROEA::get(MOD::RROEA::fc16));
      PS::set(MOD::PS::V23, 1.68 * RROEA::get(MOD::RROEA::SC) *
              RROEA::get(MOD::RROEA::FC));
    }

    if (theVars->RROEA_EPS_com)
      theVars->RROEA_Pool[POOL::RROEA::FdT] = theVars->BF_Pool[POOL::BF::kU_f];
#define UPDATE_POOL(name, Vmax)                                         \
    theVars->RROEA_Pool[POOL::RROEA::name] = PS::get(MOD::PS::Vmax) *   \
      1000. * 60. /                                                     \
      theVars->RROEA_Pool[POOL::RROEA::SA_ ## name] /                   \
      theVars->RROEA_Pool[POOL::RROEA::mw_ ## name]
    UPDATE_POOL(GAPDH, V3);
    UPDATE_POOL(FBPase, V6);
    UPDATE_POOL(SBPase, V9);
    UPDATE_POOL(PRK, V13);
    UPDATE_POOL(ATPase, V16);
    UPDATE_POOL(ATPGPP, V23);
#undef UPDATE_POOL
    theVars->RROEA_Pool[POOL::RROEA::MDH] =
      theVars->RROEA_Pool[POOL::RROEA::MDH_Vmax] * 1000. * 60. /
      theVars->RROEA_Pool[POOL::RROEA::SA_MDH] /
      theVars->RROEA_Pool[POOL::RROEA::mw_MDH];

#define UPDATE_COND(name)                                       \
    RROEA_con->set(COND::RROEA::name,                           \
                   RROEA_con->get(COND::RROEA::Coeff) *         \
                   theVars->RROEA_Pool[POOL::RROEA::name])
    UPDATE_COND(GAPDH);
    UPDATE_COND(FBPase);
    UPDATE_COND(SBPase);
    UPDATE_COND(PRK);
    UPDATE_COND(ATPase);
    UPDATE_COND(ATPGPP);
#undef UPDATE_COND
    (*RROEA_con)[COND::RROEA::Thio] *= RROEA_con->get(COND::RROEA::Coeff);
    (*RROEA_con)[COND::RROEA::Fd] *= RROEA_con->get(COND::RROEA::Coeff);
    (*RROEA_con)[COND::RROEA::RuACT] *= RROEA_con->get(COND::RROEA::Coeff);

    theVars->RROEA_RC.checkAlts("RROEA::_init::RROEA_RC: ");
    theVars->RROEA_KE.checkAlts("RROEA::_init::RROEA_KE: ");
    theVars->RROEA_Pool.checkAlts("RROEA::_init::RROEA_Pool: ");
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
#endif // CHECK_VALUE_SET_ALTS
    return RROEA_con;
}

void RROEA::_updateAlts(Variables *theVars, RROEACondition* RROEA_con) {
#ifdef CHECK_VALUE_SET_ALTS
    RROEA::updateAlts();
    theVars->RROEA_RC.updateAlts();
    theVars->RROEA_Pool.updateAlts();
    theVars->RROEA_KE.updateAlts();
    RROEA_con->updateAlts();
#else // CHECK_VALUE_SET_ALTS
    UNUSED(theVars);
    UNUSED(RROEA_con);
#endif // CHECK_VALUE_SET_ALTS
}
