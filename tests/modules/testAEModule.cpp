#include "AEModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(AEModuleTest, InitTest) {
    AECondition* aec = AE::init(theVars);

    EXPECT_NE(aec->Bchl_ActATPsynthase, 0.);
    delete aec;
}

TEST_F(AEModuleTest, RateTest) {
    const AEVel aev = theVars->AE_Vel;
    AECondition* aec = AE::init(theVars);
    Rate(1., aec, theVars);
    EXPECT_NE(theVars->AE_Vel.vATPsynthase_Act_Mchl, aev.vATPsynthase_Act_Mchl);
    EXPECT_NE(theVars->AE_Vel.vPEPC_Act, aev.vPEPC_Act);
    delete aec;
}

TEST_F(AEModuleTest, MBTest) {
    const AEVel aev = theVars->AE_Vel;
    AECondition* aec = AE::init(theVars);
    arr res = MB(1.5, aec, theVars);

    EXPECT_NE(theVars->AE_Vel.vATPsynthase_Act_Mchl, aev.vATPsynthase_Act_Mchl);
    EXPECT_NE(theVars->AE_Vel.vPEPC_Act, aev.vPEPC_Act);

    DIFF(res, zeros(res.size()))
    delete aec;
}

}
