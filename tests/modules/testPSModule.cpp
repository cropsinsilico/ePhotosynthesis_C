#include "PSModuleFramework.hpp"

namespace {
TEST_F(PSModuleTest, InitTest) {

    PSCondition* PSc = PS::init(theVars);

    theVars->useC3 = true;
    theVars->EnzymeAct = Emap;
    theVars->GP = 1;
    theVars->Tp = 27.9;
    theVars->GRNC = 1;
    theVars->GRNT = 1;
    PSCondition* PSc2 = PS::init(theVars);

    delete PSc;
    delete PSc2;
}

TEST_F(PSModuleTest, RateTest) {
    const PSVel PSv = theVars->PS_Vel;
    PSCondition* PSc = PS::init(theVars);

    Rate(1., PSc, theVars);

    EXPECT_NE(theVars->PS_Vel.v2, PSv.v2);
    EXPECT_NE(theVars->PS_Vel.v6, PSv.v6);
    EXPECT_NE(theVars->PS_Vel.v32, PSv.v32);

    delete PSc;
}

TEST_F(PSModuleTest, RateTest2) {
    const PSVel PSv = theVars->PS_Vel;

    PSCondition* PSc = PS::init(theVars);
    theVars->record = true;
    Rate(1., PSc, theVars);
    Rate(1., PSc, theVars);
    Rate(2., PSc, theVars);
    EXPECT_NE(theVars->PS_Vel.v2, PSv.v2);
    EXPECT_NE(theVars->PS_Vel.v6, PSv.v6);
    EXPECT_NE(theVars->PS_Vel.v32, PSv.v32);

    EXPECT_EQ(2, theVars->PS_VEL.size());
    delete PSc;
}

TEST_F(PSModuleTest, MBTest) {
    const PSVel PSv = theVars->PS_Vel;
    PSCondition* PSc = PS::init(theVars);
    arr res = MB(1.5, PSc, theVars);

    EXPECT_NE(theVars->PS_Vel.v2, PSv.v2);
    EXPECT_NE(theVars->PS_Vel.v6, PSv.v6);
    EXPECT_NE(theVars->PS_Vel.v32, PSv.v32);

    DIFF(res, zeros(res.size()))
    delete PSc;
}
}
