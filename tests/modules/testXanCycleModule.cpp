#include "XanCycleModuleFramework.hpp"


namespace {
TEST_F(XanCycleModuleTest, InitTest) {

    XanCycleCondition* XanCyclec = XanCycle::init(theVars);

    delete XanCyclec;
}

TEST_F(XanCycleModuleTest, RateTest) {
    const XanCycleVel XanCyclev = theVars->XanCycle_Vel;
    XanCycleCondition* XanCyclec = XanCycle::init(theVars);

    Rate(1., XanCyclec, theVars);
    EXPECT_NE(XanCyclev.Vva, theVars->XanCycle_Vel.Vva);
    EXPECT_NE(XanCyclev.Vav, theVars->XanCycle_Vel.Vav);

    delete XanCyclec;
}

TEST_F(XanCycleModuleTest, RateTest2) {
    const XanCycleVel XanCyclev = theVars->XanCycle_Vel;
    XanCycleCondition* XanCyclec = XanCycle::init(theVars);
    theVars->record = true;
    Rate(1., XanCyclec, theVars);
    Rate(1., XanCyclec, theVars);
    Rate(2., XanCyclec, theVars);
    EXPECT_NE(XanCyclev.Vva, theVars->XanCycle_Vel.Vva);
    EXPECT_NE(XanCyclev.Vav, theVars->XanCycle_Vel.Vav);
    EXPECT_EQ(2, theVars->XanCycle_VEL.size());
    delete XanCyclec;
}

TEST_F(XanCycleModuleTest, MBTest) {
    const XanCycleVel XanCyclev = theVars->XanCycle_Vel;
    XanCycleCondition* XanCyclec = XanCycle::init(theVars);
    arr res = MB(1.5, XanCyclec, theVars);

    EXPECT_NE(XanCyclev.Vva, theVars->XanCycle_Vel.Vva);
    EXPECT_NE(XanCyclev.Vav, theVars->XanCycle_Vel.Vav);

    DIFF(res, zeros(res.size()))
    delete XanCyclec;
}
}
