#include "FIModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(FIModuleTest, InitTest) {

    FICondition* FIc = FI::init(theVars);

    Variables* theVars2 = new Variables();
    theVars2->CO2_in = 500.;
    theVars2->TestLi = 14.;
    theVars2->TestATPCost = 4.5;
    theVars2->useC3 = true;
    theVars2->EnzymeAct = Emap;

    FICondition* FIc2 = FI::init(theVars2);

    EXPECT_NE(theVars->FI_RC.k3, theVars2->FI_RC.k3);
    EXPECT_NE(theVars->FI_RC.kU_d, theVars2->FI_RC.kU_d);
    EXPECT_NE(theVars->FI_RC.kz, theVars2->FI_RC.kz);
    delete FIc;
    delete FIc2;
    delete theVars2;
}

TEST_F(FIModuleTest, RateTest) {
    const FIVel FIv = theVars->FI_Vel;
    FICondition* FIc = FI::init(theVars);

    Rate(1., FIc, theVars);

    EXPECT_NE(FIv.v_pq_ox, theVars->FI_Vel.v_pq_ox);
    EXPECT_NE(FIv.v_r3, theVars->FI_Vel.v_r3);

    delete FIc;
}

TEST_F(FIModuleTest, RateTest2) {
    const FIVel FIv = theVars->FI_Vel;
    FICondition* FIc = FI::init(theVars);
    theVars->record = true;
    Rate(1., FIc, theVars);
    Rate(1., FIc, theVars);
    Rate(2., FIc, theVars);
    EXPECT_NE(FIv.v_pq_ox, theVars->FI_Vel.v_pq_ox);
    EXPECT_NE(FIv.v_r3, theVars->FI_Vel.v_r3);

    EXPECT_EQ(2, theVars->FI_VEL.size());
    delete FIc;
}

TEST_F(FIModuleTest, MBTest) {
    const FIVel FIv = theVars->FI_Vel;
    FICondition* FIc = FI::init(theVars);
    arr res = MB(1.5, FIc, theVars);

    EXPECT_NE(FIv.v_pq_ox, theVars->FI_Vel.v_pq_ox);
    EXPECT_NE(FIv.v_r3, theVars->FI_Vel.v_r3);

    DIFF(res, zeros(res.size()))
    delete FIc;
}
}
