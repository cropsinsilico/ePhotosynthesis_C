#include "VariableFramework.hpp"
#include "conditions/AE_C4Condition.hpp"

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class AEConditionTest : public VariableFramework {
};

TEST_F(AEConditionTest, InitializeTest) {
    AECondition* aec = new AECondition();
    aec->Bchl_ActATPsynthase = 1.345;
    AECondition* aec2 = new AECondition(aec);
    EXPECT_DOUBLE_EQ(aec->Bchl_ActATPsynthase, aec2->Bchl_ActATPsynthase);

    aec->clear();
    delete aec;
    delete aec2;
}

TEST_F(AEConditionTest, ArrayTest) {
    arr input = get_random(AECondition::size());
    AECondition aec(input);
    arr output = aec.toArray();
    COMPARE(input, output)
}
}
