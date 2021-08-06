#include <random>
#include "VariableFramework.hpp"
#include "conditions/PS_PRCondition.hpp"

namespace {
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

    PSCondition psc;
    psc.HexP = 1.5e-4;
    PRCondition prc;
    prc.SERc = 2.3e-3;

    PS_PRCondition pspr2(&psc, &prc);
    EXPECT_DOUBLE_EQ(1.5e-4, pspr2.PS_con->HexP);
    EXPECT_DOUBLE_EQ(2.3e-3, pspr2.PR_con->SERc);
}
}
