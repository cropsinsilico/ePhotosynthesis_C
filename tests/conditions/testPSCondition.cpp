#include <random>
#include "VariableFramework.hpp"
#include "conditions/PSCondition.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class PSConditionTest : public VariableFramework {
protected:
    void SetUp() override {
        PSCondition::setC3(false);
        VariableFramework::SetUp();
    }
};

TEST_F(PSConditionTest, C3Test) {
    EXPECT_FALSE(PSCondition::getC3());
    PSCondition::setC3(true);
    EXPECT_TRUE(PSCondition::getC3());
}

TEST_F(PSConditionTest, SizeTest) {
    size_t size = PSCondition::size();
    PSCondition::setC3(true);
    size_t newsize = PSCondition::size();
    EXPECT_GT(size, newsize);
}
TEST_F(PSConditionTest, InitializerTest) {
    PSCondition pc;
    pc.ADPG = 3.5;
    pc.PenP = 0.0045;

    PSCondition pc1(&pc);
    EXPECT_NE(pc.ADPG, 0.);
    EXPECT_DOUBLE_EQ(pc.ADPG, pc1.ADPG);
    EXPECT_DOUBLE_EQ(pc.PenP, pc1.PenP);

    PSCondition::setC3(true);
    PSCondition pc2(&pc);
    EXPECT_NE(pc.ADPG, 0.);
    EXPECT_NE(pc.ADPG, pc2.ADPG);
    EXPECT_DOUBLE_EQ(pc.PenP, pc2.PenP);
    pc.clear();
}

TEST_F(PSConditionTest, ArrayTest) {
    arr input = get_random(PSCondition::size());
    PSCondition pc(input);
    arr output = pc.toArray();
    COMPARE(input, output)

    PSCondition::setC3(true);
    input = get_random(PSCondition::size());
    EXPECT_LT(input.size(), output.size());
    PSCondition pc1(input);
    output = pc1.toArray();
    COMPARE(input, output)
}
}
