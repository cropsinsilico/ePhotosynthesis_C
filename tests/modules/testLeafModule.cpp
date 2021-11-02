#include "LeafModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(LeafModuleTest, InitTest) {
    LeafCondition* lc = Leaf::init(theVars);

    EXPECT_NE(lc->Ci, 0.);
    delete lc;
}

TEST_F(LeafModuleTest, RateTest) {
    const LeafVel lv = theVars->Leaf_Vel;
    LeafCondition* lc = Leaf::init(theVars);
    Rate(1., lc, theVars);
    EXPECT_NE(theVars->Leaf_Vel.NetAssimilation, lv.NetAssimilation);
    EXPECT_NE(theVars->Leaf_Vel.vCO2total, lv.vCO2total);
    delete lc;
}

TEST_F(LeafModuleTest, MBTest) {
    const LeafVel lv = theVars->Leaf_Vel;
    LeafCondition* lc = Leaf::init(theVars);
    arr res = MB(1.5, lc, theVars);

    EXPECT_NE(theVars->Leaf_Vel.NetAssimilation, lv.NetAssimilation);
    EXPECT_NE(theVars->Leaf_Vel.vCO2total, lv.vCO2total);

    DIFF(res, zeros(res.size()))
    delete lc;
}

}
