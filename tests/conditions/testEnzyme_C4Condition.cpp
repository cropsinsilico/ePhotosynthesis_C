#include "VariableFramework.hpp"
#include "conditions/Enzyme_C4Condition.hpp"

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class EnzymeConditionTest : public VariableFramework {
};

TEST_F(EnzymeConditionTest, InitializeTest) {
    EnzymeCondition* enc = new EnzymeCondition();
    enc->BSC_Asp = 29.5336;
    EnzymeCondition* enc2 = new EnzymeCondition(enc);
    EXPECT_DOUBLE_EQ(enc->BSC_Asp, enc2->BSC_Asp);

    enc->clear();
    delete enc;
    delete enc2;
}

TEST_F(EnzymeConditionTest, ArrayTest) {
    arr input = get_random(EnzymeCondition::size());
    EnzymeCondition enc(input);
    arr output = enc.toArray();
    COMPARE(input, output)
}
}
