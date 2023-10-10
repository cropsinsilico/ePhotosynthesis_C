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
#include "globals.hpp"
#include "modules/FI.hpp"

// This is the routine for calculation of the mass balance equations for the fluorescence induction model
// This routine is composed of two components;
// 1) The initialization of the rates that was transfered from the FI_Rate routine
// 2) The computation of the mass balance equations

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

FICondition* FI::_MB_con(const double t, const FICondition* const FI_Con, Variables *theVars) {

    //////////////////////////////////////////////////////////////////
    //   Calculate the rates first   //
    //////////////////////////////////////////////////////////////////
    if (theVars->useC3) {
        theVars->FI_Param[0] = theVars->lightParam;
    } else {
        Condition(t, theVars);
        theVars->FI_Param[0] = theVars->GLight;
    }

    Rate(t, FI_Con, theVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation //
    ////////////////////////////////////////////////////////////////

    // This page defines the mass balance equation for the system under study
    // One problem need to be taken care of is the variables needed to transfer from FI_CalV to FI_mb
    // The Major Variables
    FICondition* dydt = new FICondition();
    dydt->A = theVars->FI_Vel.Ia - theVars->FI_Vel.vA_f - theVars->FI_Vel.vA_d -
              theVars->FI_Vel.vA_U + theVars->FI_Vel.vU_A; // A 6
    dydt->U = theVars->FI_Vel.Ic + theVars->FI_Vel.vA_U - theVars->FI_Vel.vU_A -
              theVars->FI_Vel.vU_f - theVars->FI_Vel.vU_d - theVars->FI_Vel.v1 +
              theVars->FI_Vel.v_r1 - theVars->FI_Vel.vP680qU; // U
    dydt->P680ePheo = theVars->FI_Vel.vU_P680 + theVars->FI_Vel.v_r1 - theVars->FI_Vel.v1 -
                      theVars->FI_Vel.vP680_d; // P680ePheo QF add
    dydt->P680pPheon = theVars->FI_Vel.v1 - theVars->FI_Vel.v_r1 - theVars->FI_Vel.vz_1 -
                       theVars->FI_Vel.v2_1 + theVars->FI_Vel.vr2_1; // P680pPheon
    dydt->P680pPheo = theVars->FI_Vel.v2_1 - theVars->FI_Vel.vr2_1 - theVars->FI_Vel.vz_2; // P680pPheo
    dydt->P680Pheon = theVars->FI_Vel.vz_1 - theVars->FI_Vel.v2_2 + theVars->FI_Vel.vr2_2; // P680Pheon
    dydt->Yz = theVars->FI_Vel.vS1_S2 + theVars->FI_Vel.vS2_S3 + theVars->FI_Vel.vS3_S0 +
               theVars->FI_Vel.vS0_S1 - theVars->FI_Vel.vz_1 - theVars->FI_Vel.vz_2; // Yz
    dydt->S1T = theVars->FI_Vel.vS0_S1 - theVars->FI_Vel.v1z;  // S1T
    dydt->S2T = theVars->FI_Vel.vS1_S2 - theVars->FI_Vel.v2z;  // S2T
    dydt->S3T = theVars->FI_Vel.vS2_S3 - theVars->FI_Vel.v3z;  // S3T
    dydt->S0T = theVars->FI_Vel.vS3_S0 - theVars->FI_Vel.v0z; // S0T
    dydt->S1Tp = theVars->FI_Vel.v1z - theVars->FI_Vel.vS1_S2; // S1Tp
    dydt->S2Tp = theVars->FI_Vel.v2z - theVars->FI_Vel.vS2_S3; // S2Tp
    dydt->S3Tp = theVars->FI_Vel.v3z - theVars->FI_Vel.vS3_S0; // S3Tp
    dydt->S0Tp = theVars->FI_Vel.v0z - theVars->FI_Vel.vS0_S1; // S0Tp
    dydt->QAQB = theVars->FI_Vel.v3 - theVars->FI_Vel.v_r3 - theVars->FI_Vel.v2_00_1 -
                 theVars->FI_Vel.v2_00_2 + theVars->FI_Vel.vr2_00_1 + theVars->FI_Vel.vr2_00_2; // QAQB
    dydt->QAnQB = theVars->FI_Vel.v2_00_1 + theVars->FI_Vel.v2_00_2 - theVars->FI_Vel.vr2_00_1 -
                  theVars->FI_Vel.vr2_00_2 - theVars->FI_Vel.vAB1 + theVars->FI_Vel.vBA1 + theVars->FI_Vel.v3_n - theVars->FI_Vel.v_r3_n; // QAnQB
    dydt->QAQBn = theVars->FI_Vel.vAB1 - theVars->FI_Vel.vBA1 - theVars->FI_Vel.v2_01_1 -
                  theVars->FI_Vel.v2_01_2 + theVars->FI_Vel.vr2_01_1 + theVars->FI_Vel.vr2_01_2; // QAQBn
    dydt->QAnQBn = theVars->FI_Vel.vBA2 - theVars->FI_Vel.vAB2 + theVars->FI_Vel.v2_01_1 +
                   theVars->FI_Vel.v2_01_2 - theVars->FI_Vel.vr2_01_1 - theVars->FI_Vel.vr2_01_2; // QAnQBn
    dydt->QAQB2n = theVars->FI_Vel.vAB2 - theVars->FI_Vel.vBA2 - theVars->FI_Vel.v3 +
                   theVars->FI_Vel.v_r3 - theVars->FI_Vel.v2_02_1 - theVars->FI_Vel.v2_02_2 +
                   theVars->FI_Vel.vr2_02_1 + theVars->FI_Vel.vr2_02_2; // QAQB2n
    dydt->QAnQB2n = 0. - theVars->FI_Vel.v3_n + theVars->FI_Vel.v_r3_n + theVars->FI_Vel.v2_02_1 +
                    theVars->FI_Vel.v2_02_2 - theVars->FI_Vel.vr2_02_1 - theVars->FI_Vel.vr2_02_2; // QAnQB2n
    if (!FICondition::BF_connect)
        dydt->PQn = theVars->FI_Vel.v3 + theVars->FI_Vel.v3_n - theVars->FI_Vel.v_r3 -
                    theVars->FI_Vel.v_r3_n - theVars->FI_Vel.v_pq_ox; // PQn

#ifdef INCDEBUG
    DEBUG_DELTA(dydt)
#endif
    return dydt;
}

arr FI::_MB(const double t, const FICondition* const FI_Con, Variables *theVars) {
    FICondition *dydt = _MB_con(t, FI_Con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
