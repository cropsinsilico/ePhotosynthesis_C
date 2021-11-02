#include "VariableFramework.hpp"
#include "conditions/Leaf_C4Condition.hpp"

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class LeafConditionTest : public VariableFramework {
};

TEST_F(LeafConditionTest, InitializeTest) {
    LeafCondition* lc = new LeafCondition();
    lc->Ci = 2.795;
    LeafCondition* lc2 = new LeafCondition(lc);
    EXPECT_DOUBLE_EQ(lc->Ci, lc2->Ci);

    lc->clear();
    delete lc;
    delete lc2;
}

TEST_F(LeafConditionTest, ArrayTest) {
    arr input = get_random(LeafCondition::size());
    LeafCondition lc(input);
    arr output = lc.toArray();
    COMPARE(input, output)
}
}
