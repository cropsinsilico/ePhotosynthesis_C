#include "Variables.hpp"
#include "modules/AE.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double AE::TIME = 0.;
std::size_t AE::N = 1;

AECondition* AE::_init(Variables *theVars) {

    AECondition* AE_con = new AECondition();

    //Initial value

    AE_con->Mchl_ActATPsynthase = 0.;
    AE_con->Mchl_ActGAPDH = 0.;
    AE_con->Mchl_ActNADPMDH = 0.;
    AE_con->Bchl_ActATPsynthase = 3.;
    AE_con->Bchl_ActPEPC = 0.05;
    AE_con->Bchl_ActGAPDH = 0.3;
    AE_con->Bchl_ActFBPase = 0.3;
    AE_con->Bchl_ActSBPase = 0.3;
    AE_con->Bchl_ActPRK = 0.3;
    AE_con->Bchl_ActRubisco = 0.1;
    AE_con->Bchl_ActRCA = 0.05;

    return AE_con;
}

void AE::_reset() {

}
