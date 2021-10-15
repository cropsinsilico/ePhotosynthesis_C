#include "Variables.hpp"
#include "globals.hpp"
#include "modules/AE.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

AECondition* AE::_MB_con(const double t, const AECondition* AE_con, Variables *theVars) {


    //arr LM_v = RAC4LeafMetaVel(t, LM_con, myVars);
    Rate(t, AE_con, theVars);

    AECondition* dydt = new AECondition();

    dydt->Mchl_ActATPsynthase = theVars->AE_Vel.vATPsynthase_Act_Mchl;
    dydt->Mchl_ActGAPDH = theVars->AE_Vel.vGAPDH_Act_Mchl;
    dydt->Mchl_ActNADPMDH = theVars->AE_Vel.vNADPMDH_Act;
    dydt->Bchl_ActATPsynthase = theVars->AE_Vel.vATPsynthase_Act_Bchl;
    dydt->Bchl_ActPEPC = theVars->AE_Vel.vPEPC_Act;
    dydt->Bchl_ActGAPDH = theVars->AE_Vel.vGAPDH_Act_Bchl;
    dydt->Bchl_ActFBPase = theVars->AE_Vel.vFBPase_Act;
    dydt->Bchl_ActSBPase = theVars->AE_Vel.vSBPase_Act;
    dydt->Bchl_ActPRK = theVars->AE_Vel.vPRK_Act;
    dydt->Bchl_ActRubisco = theVars->AE_Vel.vRubisco_Act;
    dydt->Bchl_ActRCA = theVars->AE_Vel.vRCA_Act;

    return dydt;
}
