#include "globals.hpp"
#include "VariableFramework.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::ElementsAre;

namespace {

class ConditionTest : public VariableFramework {

};

TEST_F(ConditionTest, TestUseC3) {
    theVars->useC3 = true;
    theVars->CO2_in = 1.0;
    Condition(0, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (1./3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);

    theVars->CO2_in = 125.;
    theVars->TestLi = 425;
    Condition(5, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (125./3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, 425*.7225);

}

TEST_F(ConditionTest, TestGeneral) {
    theVars->CO2_in = 1.0;
    theVars->TestLi = 1.0;
    Condition(0, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (.7/3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, .7225);

    theVars->CO2_in = 125.;
    theVars->TestLi = 425;
    Condition(5, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (125.*.7/3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, 425*.7225);

}

class ParamSetTest : public VariableFramework {

};

TEST_F(ParamSetTest, TestGeneral) {
    const std::vector<double> values = {1.75, 6.9};
    for (auto val : values) {
        theVars->PS12ratio = val;
        ParamSet(theVars);
        ASSERT_THAT(theVars->BF_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->FI_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->RuACT_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->XanCycle_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->RROEA_Param, ElementsAre(0., val));
    }
}
class TargetFunValTest : public VariableFramework {

};

TEST_F(TargetFunValTest, TestGeneral) {
    theVars->record = false;
    PSVel psv, psv1;
    psv.v1 = 1.5;
    psv.v5 = 2.6;
    psv1.v1 = 1.5;
    psv1.v5 = 2.6;
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PS_VEL.insert(1, 0.5, psv1);
    PRVel prv, prv1;
    prv.v111 = 1.5;
    prv.v113 = 2.6;
    prv1.v111 = 1.5;
    prv1.v113 = 2.6;
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PS_VEL.insert(1, 0.5, psv1);
    theVars->PR_VEL.insert(0, 0., prv);
    theVars->PR_VEL.insert(1, 0.5, prv1);
    EXPECT_DOUBLE_EQ(0., TargetFunVal(theVars));
    for (size_t i = 0; i < theVars->PS_VEL.size(); i++) {
        EXPECT_DOUBLE_EQ(1.5, theVars->PS_VEL[i].v1);
        EXPECT_DOUBLE_EQ(2.6, theVars->PS_VEL[i].v5);
        EXPECT_DOUBLE_EQ(0., theVars->PS_VEL[i].v6);
    }
    for (size_t i = 0; i < theVars->PR_VEL.size(); i++) {
        EXPECT_DOUBLE_EQ(1.5, theVars->PR_VEL[i].v111);
        EXPECT_DOUBLE_EQ(2.6, theVars->PR_VEL[i].v113);
        EXPECT_DOUBLE_EQ(0., theVars->PR_VEL[i].v123);
    }
    theVars->record = true;
    EXPECT_NE(0., TargetFunVal(theVars));
    for (size_t i = 0; i < theVars->PS_VEL.size(); i++) {
        EXPECT_NE(1.5, theVars->PS_VEL[i].v1);
        EXPECT_NE(2.6, theVars->PS_VEL[i].v5);
        EXPECT_DOUBLE_EQ(0., theVars->PS_VEL[i].v6);
    }
    for (size_t i = 0; i < theVars->PR_VEL.size(); i++) {
        EXPECT_NE(1.5, theVars->PR_VEL[i].v111);
        EXPECT_NE(2.6, theVars->PR_VEL[i].v113);
        EXPECT_DOUBLE_EQ(0., theVars->PR_VEL[i].v123);
    }
}

class GenOutTest : public VariableFramework {

};

TEST_F(GenOutTest, GenOut) {
    theVars->record = false;
    GenOut(0, theVars);
    EXPECT_EQ(0, theVars->CO2A.size());
    theVars->record = true;
    GenOut(1, theVars);
    EXPECT_EQ(1, theVars->CO2A.size());
    theVars->FIBF_PSPR_com = true;
    theVars->RuACT_EPS_com = true;
    GenOut(2, theVars);
    EXPECT_EQ(1, theVars->CO2A.size());
}

TEST_F(GenOutTest, makeFluxTR) {
    EXPECT_EQ(0, theVars->FluxTR.size());
    RuACTVel ruv;
    PSVel psv;
    PRVel prv;
    SUCSVel suv;
    BFVel bfv;
    FIVel fiv;
    XanCycleVel xcv;
    RROEAVel rrv;
    theVars->RuACT_VEL.insert(0, 0., ruv);
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PR_VEL.insert(0, 0., prv);
    theVars->SUCS_VEL.insert(0, 0., suv);
    theVars->BF_VEL.insert(0, 0., bfv);
    theVars->FI_VEL.insert(0, 0., fiv);
    theVars->XanCycle_VEL.insert(0, 0., xcv);
    theVars->RROEA_VEL.insert(0, 0., rrv);
    makeFluxTR(theVars);
    EXPECT_NE(0, theVars->FluxTR.size());
}
}
