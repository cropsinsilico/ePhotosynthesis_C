#include <random>
#include "VariableFramework.hpp"
#include "conditions/XanCycleCondition.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class XanCycleConditionTest : public VariableFramework {

};

TEST_F(XanCycleConditionTest, InitializeTest) {
    XanCycleCondition* xc = new XanCycleCondition();
    xc->ABA = 0.85;
    XanCycleCondition* xc2 = new XanCycleCondition(xc);
    EXPECT_DOUBLE_EQ(xc->ABA, xc2->ABA);

    xc->clear();
    EXPECT_NE(xc->ABA, xc2->ABA);
    delete xc;
    delete xc2;
}

TEST_F(XanCycleConditionTest, ArrayTest) {
    arr input = get_random(XanCycleCondition::size());
    XanCycleCondition xc(input);
    arr output = xc.toArray();
    COMPARE(input, output)
}

}
