#include <random>
#include "VariableFramework.hpp"
#include "conditions/RACondition.hpp"
#include <nvector/nvector_serial.h>

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class RAConditionTest : public VariableFramework {
};

TEST_F(RAConditionTest, ConstructorTest) {
    RACondition RA;
    RA.EPS_con->CM_con->PS_PR_con->PR_con->HPRc = 2.5;
    RA.RuACT_con->ER = 0.445;

    RACondition RA1(&RA);
    EXPECT_DOUBLE_EQ(RA.EPS_con->CM_con->PS_PR_con->PR_con->HPRc, RA1.EPS_con->CM_con->PS_PR_con->PR_con->HPRc);
    EXPECT_DOUBLE_EQ(RA.RuACT_con->ER, RA1.RuACT_con->ER);

    EPSCondition* ec = new EPSCondition();
    RuACTCondition* rc = new RuACTCondition();

    RACondition* RA2 = new RACondition(ec, rc);

    RACondition* RA3 = new RACondition(RA2->EPS_con, RA2->RuACT_con);
    EXPECT_NE(RA2->EPS_con, RA3->EPS_con);
    EXPECT_NE(RA2->RuACT_con, RA3->RuACT_con);
    delete RA2;
    delete RA3;
}

TEST_F(RAConditionTest, ArrayTest) {
    arr input = get_random(RACondition::size());
    RACondition RA(input);
    arr output = RA.toArray();
    COMPARE(input, output)
}
}
