#include "RuACTModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(RuACTModuleTest, InitTest) {

    RuACTCondition* RuACTc = RuACT::init(theVars);

    delete RuACTc;
}

TEST_F(RuACTModuleTest, RateTest) {
    const RuACTVel RuACTv = theVars->RuACT_Vel;
    RuACTCondition* RuACTc = RuACT::init(theVars);

    Rate(1., RuACTc, theVars);
    EXPECT_NE(RuACTv.v1, theVars->RuACT_Vel.v1);
    EXPECT_NE(RuACTv.v7, theVars->RuACT_Vel.v7);

    delete RuACTc;
}

TEST_F(RuACTModuleTest, RateTest2) {
    const RuACTVel RuACTv = theVars->RuACT_Vel;
    RuACTCondition* RuACTc = RuACT::init(theVars);
    theVars->record = true;
    Rate(1., RuACTc, theVars);
    Rate(1., RuACTc, theVars);
    Rate(2., RuACTc, theVars);
    EXPECT_NE(RuACTv.v1, theVars->RuACT_Vel.v1);
    EXPECT_NE(RuACTv.v7, theVars->RuACT_Vel.v7);
    EXPECT_EQ(2, theVars->RuACT_VEL.size());

    delete RuACTc;
}

TEST_F(RuACTModuleTest, MBTest) {
    const RuACTVel RuACTv = theVars->RuACT_Vel;
    RuACTCondition* RuACTc = RuACT::init(theVars);
    arr res = MB(1.5, RuACTc, theVars);

    EXPECT_NE(RuACTv.v1, theVars->RuACT_Vel.v1);
    EXPECT_NE(RuACTv.v7, theVars->RuACT_Vel.v7);

    DIFF(res, zeros(res.size()))
    delete RuACTc;
}
}
