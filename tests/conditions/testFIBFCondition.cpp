#include <random>
#include "VariableFramework.hpp"
#include "conditions/FIBFCondition.hpp"


namespace {
class FIBFConditionTest : public VariableFramework {
    void SetUp() override {
        FICondition::setBF_connect(false);
        BFCondition::setPS_connect(false);
        BFCondition::setRROEA_connect(false);
        BFCondition::setFI_connect(false);
        VariableFramework::SetUp();
    }
};

TEST_F(FIBFConditionTest, InitializeTest) {
    FIBFCondition fc;
    fc.kd = 10.9;
    fc.FI_con->QAnQB = 52.8;
    FIBFCondition fc1(&fc);

    EXPECT_DOUBLE_EQ(fc.kd, fc1.kd);
    EXPECT_DOUBLE_EQ(fc.FI_con->QAnQB, fc1.FI_con->QAnQB);

    FICondition fic;
    fic.P680Pheon = 162.4;

    BFCondition bfc;
    bfc.An = 0.558;

    FIBFCondition fc2(&bfc, &fic);
    EXPECT_DOUBLE_EQ(fc2.FI_con->P680Pheon, 162.4);
    EXPECT_DOUBLE_EQ(fc2.BF_con->An, 0.558);
}

TEST_F(FIBFConditionTest, ArrayTest) {
    arr input = get_random(FIBFCondition::size());
    FIBFCondition fib(input);
    arr output = fib.toArray();
    COMPARE(input, output)
}
}
