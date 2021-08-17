#include "RROEAModuleFramework.hpp"

namespace {
TEST_F(RROEAModuleTest, InitTest) {

    RROEACondition* RROEAc = RROEA::init(theVars);

    delete RROEAc;
}

TEST_F(RROEAModuleTest, RateTest) {
    const RROEAVel RROEAv = theVars->RROEA_Vel;
    RROEACondition* RROEAc = RROEA::init(theVars);

    Rate(1., RROEAc, theVars);
    EXPECT_NE(RROEAv.ve2PRK, theVars->RROEA_Vel.ve2PRK);
    EXPECT_NE(RROEAv.veFd2Calvin, theVars->RROEA_Vel.veFd2Calvin);

    delete RROEAc;
}

TEST_F(RROEAModuleTest, RateTest2) {
    const RROEAVel RROEAv = theVars->RROEA_Vel;
    RROEACondition* RROEAc = RROEA::init(theVars);
    theVars->record = true;
    Rate(1., RROEAc, theVars);
    Rate(1., RROEAc, theVars);
    Rate(2., RROEAc, theVars);
    EXPECT_NE(RROEAv.ve2PRK, theVars->RROEA_Vel.ve2PRK);
    EXPECT_NE(RROEAv.veFd2Calvin, theVars->RROEA_Vel.veFd2Calvin);
    EXPECT_EQ(2, theVars->RROEA_VEL.size());
    delete RROEAc;
}

TEST_F(RROEAModuleTest, MBTest) {
    const RROEAVel RROEAv = theVars->RROEA_Vel;
    RROEACondition* RROEAc = RROEA::init(theVars);
    arr res = MB(1.5, RROEAc, theVars);

    EXPECT_NE(RROEAv.ve2PRK, theVars->RROEA_Vel.ve2PRK);
    EXPECT_NE(RROEAv.veFd2Calvin, theVars->RROEA_Vel.veFd2Calvin);

    DIFF(res, zeros(res.size()))
    delete RROEAc;
}
}
