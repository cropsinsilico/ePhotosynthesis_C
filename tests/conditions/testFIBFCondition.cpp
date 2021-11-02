#include "VariableFramework.hpp"
#include "conditions/FIBFCondition.hpp"


using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

namespace {
class FIBFConditionTest : public VariableFramework {
    void SetUp() override {
        conditions::FICondition::setBF_connect(false);
        conditions::BFCondition::setPS_connect(false);
        conditions::BFCondition::setRROEA_connect(false);
        conditions::BFCondition::setFI_connect(false);
        VariableFramework::SetUp();
    }
};

TEST_F(FIBFConditionTest, InitializeTest) {
    FIBFCondition::reset();
    FIBFCondition fc;
    fc.kd = 10.9;
    fc.FI_con->QAnQB = 52.8;
    FIBFCondition fc1(&fc);

    EXPECT_DOUBLE_EQ(fc.kd, fc1.kd);
    EXPECT_DOUBLE_EQ(fc.FI_con->QAnQB, fc1.FI_con->QAnQB);

    FICondition* fic = new FICondition();
    fic->P680Pheon = 162.4;

    BFCondition* bfc = new BFCondition();
    bfc->An = 0.558;

    FIBFCondition* fc2 = new FIBFCondition(bfc, fic);
    EXPECT_DOUBLE_EQ(fc2->FI_con->P680Pheon, 162.4);
    EXPECT_DOUBLE_EQ(fc2->BF_con->An, 0.558);

    FIBFCondition* fc3 = new FIBFCondition(fc2->BF_con, fc2->FI_con);
    EXPECT_DOUBLE_EQ(fc2->FI_con->P680Pheon, fc3->FI_con->P680Pheon);
    EXPECT_DOUBLE_EQ(fc2->BF_con->An, fc3->BF_con->An);
    EXPECT_NE(fc2->FI_con, fc3->FI_con);
    EXPECT_NE(fc2->BF_con, fc3->BF_con);
    delete fc2;
    delete fc3;

}

TEST_F(FIBFConditionTest, ArrayTest) {
    FIBFCondition::reset();
    arr input = get_random(FIBFCondition::size());
    FIBFCondition fib(input);
    arr output = fib.toArray();
    COMPARE(input, output)
}
}
