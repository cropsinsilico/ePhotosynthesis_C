#include <random>
#include "RROEAModuleFramework.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(RROEAConditionTest, InitializeTest) {
    RROEACondition* rrc = new RROEACondition();
    rrc->PRK = 0.85;
    RROEACondition* rrc2 = new RROEACondition(rrc);
    EXPECT_DOUBLE_EQ(rrc->PRK, rrc2->PRK);
    delete rrc;
    delete rrc2;
}

TEST_F(RROEAConditionTest, ArrayTest) {
    arr input = get_random(RROEACondition::size());
    RROEACondition rrc(input);
    arr output = rrc.toArray();
    COMPARE(input, output)
}

}
