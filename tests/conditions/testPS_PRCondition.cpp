#include <random>
#include "VariableFramework.hpp"
#include "conditions/PS_PRCondition.hpp"

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class PS_PRConditionTest : public VariableFramework {
protected:
    void SetUp() override {
        PRCondition::setPS_RuBP(false);
        PRCondition::setPS_connect(false);
        PSCondition::setC3(false);
        VariableFramework::SetUp();
    }
};

TEST_F(PS_PRConditionTest, ConstructorTest) {
    PS_PRCondition pspr;
    pspr.PR_con->HPRc = 2.5;
    pspr.PS_con->E4P = 0.445;

    PS_PRCondition pspr1(&pspr);
    EXPECT_DOUBLE_EQ(pspr.PR_con->HPRc, pspr1.PR_con->HPRc);
    EXPECT_DOUBLE_EQ(pspr.PS_con->E4P, pspr1.PS_con->E4P);

    PSCondition* psc = new PSCondition();
    psc->HexP = 1.5e-4;
    PRCondition* prc = new PRCondition();
    prc->SERc = 2.3e-3;

    PS_PRCondition* pspr2 = new PS_PRCondition(psc, prc);
    EXPECT_DOUBLE_EQ(1.5e-4, pspr2->PS_con->HexP);
    EXPECT_DOUBLE_EQ(2.3e-3, pspr2->PR_con->SERc);

    PS_PRCondition* pspr3 = new PS_PRCondition(pspr2->PS_con, pspr2->PR_con);
    EXPECT_DOUBLE_EQ(pspr3->PS_con->HexP, pspr2->PS_con->HexP);
    EXPECT_DOUBLE_EQ(pspr3->PR_con->SERc, pspr2->PR_con->SERc);
    EXPECT_NE(pspr2->PS_con, pspr3->PS_con);
    EXPECT_NE(pspr2->PR_con, pspr3->PR_con);
    delete pspr2;
    delete pspr3;
}

TEST_F(PS_PRConditionTest, ArrayTest) {
    arr input = get_random(PS_PRCondition::size());
    PS_PRCondition pspr(input);
    arr output = pspr.toArray();
    COMPARE(input, output)
}
}
