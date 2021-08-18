#include <random>
#include "VariableFramework.hpp"
#include "conditions/RuACTCondition.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class RuACTConditionTest : public VariableFramework {
protected:
    void SetUp() override {
        RuACTCondition::setEPS_connect(false);
        VariableFramework::SetUp();
    }
};

TEST_F(RuACTConditionTest, InitializeTest) {
    RuACTCondition* rc = new RuACTCondition();
    rc->RuBP = 0.85;
    RuACTCondition* rc2 = new RuACTCondition(rc);
    EXPECT_DOUBLE_EQ(rc->RuBP, rc2->RuBP);
    RuACTCondition::setEPS_connect(true);
    RuACTCondition* rc3 = new RuACTCondition(rc);
    EXPECT_NE(rc->RuBP, rc3->RuBP);
    delete rc;
    delete rc2;
    delete rc3;
}

TEST_F(RuACTConditionTest, ArrayTest) {
    arr input = get_random(RuACTCondition::size());
    RuACTCondition rc(input);
    arr output = rc.toArray();
    COMPARE(input, output)

    RuACTCondition::setEPS_connect(true);
    arr input2 = get_random(RuACTCondition::size());
    RuACTCondition rc2(input2);
    arr output2 = rc2.toArray();
    COMPARE(input2, output2)
    EXPECT_NE(input.size(), input2.size());

}

}
