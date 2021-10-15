#include <math.h>
#include "Variables.hpp"
#include "modules/AE.hpp"
#include "conditions/AECondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double AACCC = 0.05;
const double tao_ActATPsynthase = 0.5 * 60;
const double tao_ActGAPDH = 1 * 60 / 10;
const double tao_ActFBPase = 1.878 * 60;//1.878*60;
const double tao_ActSBPase = 3.963 * 60;//3.963*60;
const double tao_ActPRK = 1 * 60 / 10;
const double KaRac = 12.4;//mg m-2
const double Tao_MDH = 1.;
const double Tao_PEPC = 2.;

void AE::_Rate(const double t, const conditions::AECondition *const AE_con, Variables *theVars) {
    const double tao_ActNADPMDH = 0.965 * 60 * Tao_MDH;//0.5*60*4*myVars.Tao_MDH;
    const double tao_ActPEPC = 60 * Tao_PEPC;
    const double tao_ActRubisco = theVars->taoRub * 60;//2.5;//KTaoRac/Rac; // s

    const double I = theVars->TestLi * CONVERT / 1000 * 0.85;
    const double ActATPsyn0 = std::min(0.0017 * I * 1000 / 0.85 + AACCC, 1.);
    const double ActNADPMDH0 = std::min(0.0017 * I * 1000 / 0.85 + 0.05, 1.);
    const double ActGAPDH0 = std::min(0.0017 * I * 1000 / 0.85 + AACCC, 1.);
    const double ActPEPC0 = std::min(0.0017 * I * 1000 / 0.85 + 0.05, 1.);
    const double ActFBPase0 = std::min(0.0017 * I * 1000 / 0.85 + AACCC, 1.);
    const double ActSBPase0 = std::min(0.0017 * I * 1000 / 0.85 + AACCC, 1.);
    const double ActPRK0 = std::min(0.0017 * I * 1000 / 0.85 / + AACCC, 1.);
    const double vATPsynthase_Act_Mchl = (ActATPsyn0 - AE_con->Mchl_ActATPsynthase) * 1 / tao_ActATPsynthase;//*Vm_ATPM/kcat_ATPsyn;
    const double vNADPMDH_Act = (ActNADPMDH0 - AE_con->Mchl_ActNADPMDH) * 1 / tao_ActNADPMDH;//*Vm_3/kcat_NADPMDH;
    const double vGAPDH_Act_Mchl = (ActGAPDH0 - AE_con->Mchl_ActGAPDH) * 1 / tao_ActGAPDH;
    const double vATPsynthase_Act_Bchl = (ActATPsyn0 - AE_con->Bchl_ActATPsynthase) * 1 / tao_ActATPsynthase;//;
    const double vPEPC_Act = (ActPEPC0 - AE_con->Bchl_ActPEPC) * 1 / tao_ActPEPC;

    double ActRca0 = 0.0017 * I * 1000 / 0.85 + 0.06;
    if (ActRca0 > 1)
        ActRca0 = 1;
    const double tao_ActRca = 0.7594 * 60 / 10;
    const double vGAPDH_Act_Bchl = (ActGAPDH0 - AE_con->Bchl_ActGAPDH) * 1 / tao_ActGAPDH;
    const double vFBPase_Act = (ActFBPase0 - AE_con->Bchl_ActFBPase) * 1 / tao_ActFBPase;
    const double vSBPase_Act = (ActSBPase0 - AE_con->Bchl_ActSBPase) * 1 / tao_ActSBPase;
    const double vPRK_Act = (ActPRK0 - AE_con->Bchl_ActPRK) * 1 / tao_ActPRK;
    const double vRCA_Act = (ActRca0 - AE_con->Bchl_ActRCA) * 1 / tao_ActRca;

    const double Rac = 216.9 / tao_ActRubisco * 60;////216.9 min mg m-2
    const double ActRubisco0 = Rac * AE_con->Bchl_ActRCA / (KaRac + Rac * AE_con->Bchl_ActRCA);
    const double vRubisco_Act = (ActRubisco0 - AE_con->Bchl_ActRubisco) * 1 / tao_ActRubisco;//;

    theVars->AE_Vel.vATPsynthase_Act_Mchl = vATPsynthase_Act_Mchl;
    theVars->AE_Vel.vNADPMDH_Act = vNADPMDH_Act;
    theVars->AE_Vel.vGAPDH_Act_Mchl = vGAPDH_Act_Mchl;
    theVars->AE_Vel.vATPsynthase_Act_Bchl = vATPsynthase_Act_Bchl;
    theVars->AE_Vel.vPEPC_Act = vPEPC_Act;
    theVars->AE_Vel.ActRubisco0 = ActRubisco0;
    theVars->AE_Vel.vRubisco_Act = vRubisco_Act;
    theVars->AE_Vel.vGAPDH_Act_Bchl = vGAPDH_Act_Bchl;
    theVars->AE_Vel.vFBPase_Act = vFBPase_Act;
    theVars->AE_Vel.vSBPase_Act = vSBPase_Act;
    theVars->AE_Vel.vPRK_Act = vPRK_Act;
    theVars->AE_Vel.vRCA_Act = vRCA_Act;
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
