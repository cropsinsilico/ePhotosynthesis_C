#include <random>
#include "PRModuleFramework.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(PRConditionTest, C3Test) {
    EXPECT_FALSE(PRCondition::getPS_RuBP());
    PRCondition::setPS_RuBP(true);
    EXPECT_TRUE(PRCondition::getPS_RuBP());

    EXPECT_FALSE(PRCondition::getPS_connect());
    PRCondition::setPS_connect(true);
    EXPECT_TRUE(PRCondition::getPS_connect());

}

TEST_F(PRConditionTest, SizeTest) {
    size_t size = PRCondition::size();
    PRCondition::setPS_RuBP(true);
    size_t newsize = PRCondition::size();
    EXPECT_GT(size, newsize);
}

TEST_F(PRConditionTest, InitializerTest) {
    PRCondition pr;
    pr.GCEA = 3.5;
    pr.RuBP = 0.0045;

    PRCondition pr1(&pr);
    EXPECT_NE(pr.GCEA, 0.);
    EXPECT_DOUBLE_EQ(pr.GCEA, pr1.GCEA);
    EXPECT_DOUBLE_EQ(pr.RuBP, pr1.RuBP);

    PRCondition::setPS_RuBP(true);
    PRCondition pr2(&pr);
    EXPECT_NE(pr.GCEA, 0.);
    EXPECT_NE(pr.RuBP, pr2.RuBP);
    EXPECT_DOUBLE_EQ(pr.GCEA, pr2.GCEA);

}

TEST_F(PRConditionTest, ArrayTest) {
    arr input = get_random(PRCondition::size());
    PRCondition pc(input);
    arr output = pc.toArray();
    COMPARE(input, output)

    PRCondition::setPS_RuBP(true);
    input = get_random(PRCondition::size());
    EXPECT_LT(input.size(), output.size());
    PRCondition pc1(input);
    output = pc1.toArray();
    COMPARE(input, output)
}
}
