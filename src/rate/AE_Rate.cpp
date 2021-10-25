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

#include <math.h>
#include "Variables.hpp"
#include "modules/AE.hpp"
#include "conditions/AECondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double AACCC = 0.05;
const double tao_ActATPsynthase = 0.5 * 60.;
const double tao_ActGAPDH = 1. * 60. / 10.;
const double tao_ActFBPase = 1.878 * 60.;//1.878*60;
const double tao_ActSBPase = 3.963 * 60.;//3.963*60;
const double tao_ActPRK = 1. * 60. / 10.;
const double KaRac = 12.4;//mg m-2
const double Tao_MDH = 1.;
const double Tao_PEPC = 2.;
const double tao_ActNADPMDH = 0.965 * 60. * Tao_MDH;//0.5*60*4*myVars.Tao_MDH;
const double tao_ActPEPC = 60. * Tao_PEPC;
const double tao_ActRca = 0.7594 * 60. / 10.;

void AE::_Rate(const double t, const conditions::AECondition *const AE_con, Variables *theVars) {
    const double tao_ActRubisco = theVars->taoRub * 60.;//2.5;//KTaoRac/Rac; // s

    const double I = theVars->TestLi * CONVERT / 1000. * 0.85;
    const double ActATPsyn0 = std::min(0.0017 * I * 1000. / 0.85 + AACCC, 1.);
    const double ActNADPMDH0 = std::min(0.0017 * I * 1000. / 0.85 + 0.05, 1.);
    const double ActGAPDH0 = std::min(0.0017 * I * 1000. / 0.85 + AACCC, 1.);
    const double ActPEPC0 = std::min(0.0017 * I * 1000. / 0.85 + 0.05, 1.);
    const double ActFBPase0 = std::min(0.0017 * I * 1000. / 0.85 + AACCC, 1.);
    const double ActSBPase0 = std::min(0.0017 * I * 1000. / 0.85 + AACCC, 1.);
    const double ActPRK0 = std::min(0.0017 * I * 1000. / 0.85 / + AACCC, 1.);
    const double ActRca0 = std::min(0.0017 * I * 1000. / 0.85 + 0.06, 1.);

    const double Rac = 216.9 / tao_ActRubisco * 60.;////216.9 min mg m-2

    theVars->AE_Vel.vATPsynthase_Act_Mchl = (ActATPsyn0 - AE_con->Mchl_ActATPsynthase) * 1. /
                                            tao_ActATPsynthase;//*Vm_ATPM/kcat_ATPsyn;
    theVars->AE_Vel.vNADPMDH_Act = (ActNADPMDH0 - AE_con->Mchl_ActNADPMDH) * 1. / tao_ActNADPMDH;//*Vm_3/kcat_NADPMDH;
    theVars->AE_Vel.vGAPDH_Act_Mchl = (ActGAPDH0 - AE_con->Mchl_ActGAPDH) * 1. / tao_ActGAPDH;
    theVars->AE_Vel.vATPsynthase_Act_Bchl = (ActATPsyn0 - AE_con->Bchl_ActATPsynthase) * 1. /
                                            tao_ActATPsynthase;//;
    theVars->AE_Vel.vPEPC_Act = (ActPEPC0 - AE_con->Bchl_ActPEPC) * 1. / tao_ActPEPC;
    theVars->AE_Vel.ActRubisco0 = Rac * AE_con->Bchl_ActRCA / (KaRac + Rac * AE_con->Bchl_ActRCA);
    theVars->AE_Vel.vRubisco_Act = (theVars->AE_Vel.ActRubisco0 - AE_con->Bchl_ActRubisco) * 1. / tao_ActRubisco;//;
    theVars->AE_Vel.vGAPDH_Act_Bchl = (ActGAPDH0 - AE_con->Bchl_ActGAPDH) * 1. / tao_ActGAPDH;
    theVars->AE_Vel.vFBPase_Act = (ActFBPase0 - AE_con->Bchl_ActFBPase) * 1. / tao_ActFBPase;
    theVars->AE_Vel.vSBPase_Act = (ActSBPase0 - AE_con->Bchl_ActSBPase) * 1. / tao_ActSBPase;
    theVars->AE_Vel.vPRK_Act = (ActPRK0 - AE_con->Bchl_ActPRK) * 1. / tao_ActPRK;
    theVars->AE_Vel.vRCA_Act = (ActRca0 - AE_con->Bchl_ActRCA) * 1. / tao_ActRca;
#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->AE_Vel)
#endif
    if (theVars->record) {
        if (t > AE::TIME) {
            AE::N++;
            AE::TIME = t;
        }

        theVars->AE_VEL.insert(AE::N - 1, t, theVars->AE_Vel);
    }

}
