#include "BFModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(BFModuleTest, InitTest) {

    BFCondition* bfc = BF::init(theVars);

    Variables* theVars2 = new Variables();
    theVars2->CO2_in = 500.;
    theVars2->TestLi = 14.;
    theVars2->TestATPCost = 4.5;
    theVars2->useC3 = true;
    theVars2->EnzymeAct = Emap;

    BFCondition* bfc2 = BF::init(theVars2);

    EXPECT_NE(theVars->BF_Pool.kA_d, theVars2->BF_Pool.kA_d);
    EXPECT_NE(theVars->BF_Pool.kU_A, theVars2->BF_Pool.kU_A);
    EXPECT_NE(theVars->BF_Pool.kU_f, theVars2->BF_Pool.kU_f);
    EXPECT_NE(theVars->BF_Pool.k12, theVars2->BF_Pool.k12);
    EXPECT_NE(theVars->BF_Pool.k30, theVars2->BF_Pool.k30);
    EXPECT_NE(theVars->BF_Pool.kU_d, theVars2->BF_Pool.kU_d);
    delete bfc;
    delete bfc2;
    delete theVars2;
}

TEST_F(BFModuleTest, RateTest) {
    const BFVel bfv = theVars->BF_Vel;
    BFCondition* bfc = BF::init(theVars);

    Rate(1., bfc, theVars);
    EXPECT_NE(bfv.Vbf8, theVars->BF_Vel.Vbf8);
    EXPECT_NE(bfv.Vinc, theVars->BF_Vel.Vinc);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);
    EXPECT_NE(bfv.Vbf4, theVars->BF_Vel.Vbf4);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);

    delete bfc;
}

TEST_F(BFModuleTest, RateTest2) {
    const BFVel bfv = theVars->BF_Vel;
    BFCondition* bfc = BF::init(theVars);
    theVars->record = true;
    Rate(1., bfc, theVars);
    Rate(1., bfc, theVars);
    Rate(2., bfc, theVars);
    EXPECT_NE(bfv.Vbf8, theVars->BF_Vel.Vbf8);
    EXPECT_NE(bfv.Vinc, theVars->BF_Vel.Vinc);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);
    EXPECT_NE(bfv.Vbf4, theVars->BF_Vel.Vbf4);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);
    EXPECT_EQ(2, theVars->BF_VEL.size());
    DIFF(theVars->BF2OUT, zeros(theVars->BF2OUT.size()));
    delete bfc;
}

TEST_F(BFModuleTest, MBTest) {
    const BFVel bfv = theVars->BF_Vel;
    BFCondition* bfc = BF::init(theVars);
    arr res = MB(1.5, bfc, theVars);

    EXPECT_NE(bfv.Vbf8, theVars->BF_Vel.Vbf8);
    EXPECT_NE(bfv.Vinc, theVars->BF_Vel.Vinc);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);
    EXPECT_NE(bfv.Vbf4, theVars->BF_Vel.Vbf4);
    EXPECT_NE(bfv.VgPQH2, theVars->BF_Vel.VgPQH2);

    DIFF(res, zeros(res.size()))
    delete bfc;
}
}
