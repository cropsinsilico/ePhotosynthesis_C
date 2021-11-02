#include "VariableFramework.hpp"
#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

namespace {
class RAC4leafMetaConditionTest : public VariableFramework {
};

TEST_F(RAC4leafMetaConditionTest, InitializeTest) {
    RAC4leafMetaCondition rlc;
    rlc.Leaf_con->Cb = 22.397;
    rlc.AE_con->Mchl_ActATPsynthase = 0.225;
    RAC4leafMetaCondition rlc2(&rlc);
    EXPECT_DOUBLE_EQ(rlc.Leaf_con->Cb, rlc2.Leaf_con->Cb);
    EXPECT_DOUBLE_EQ(rlc.AE_con->Mchl_ActATPsynthase, rlc2.AE_con->Mchl_ActATPsynthase);

    LeafCondition* lc = new LeafCondition();
    lc->Eb = 0.00335;
    AECondition* aec = new AECondition();
    aec->Bchl_ActPRK = 2.00006;
    EnzymeCondition* enc = new EnzymeCondition();
    enc->BSC_Malate = 22200.9987;

    RAC4leafMetaCondition* rc = new RAC4leafMetaCondition(lc, enc, aec);
    EXPECT_DOUBLE_EQ(rc->Leaf_con->Eb, 0.00335);
    EXPECT_DOUBLE_EQ(rc->AE_con->Bchl_ActPRK, 2.00006);
    EXPECT_DOUBLE_EQ(rc->Enzyme_con->BSC_Malate, 22200.9987);

    RAC4leafMetaCondition* rc2 = new RAC4leafMetaCondition(rc->Leaf_con, rc->Enzyme_con, rc->AE_con);
    EXPECT_DOUBLE_EQ(rc->Leaf_con->Eb, rc2->Leaf_con->Eb);
    EXPECT_DOUBLE_EQ(rc->AE_con->Bchl_ActPRK, rc2->AE_con->Bchl_ActPRK);
    EXPECT_DOUBLE_EQ(rc->Enzyme_con->BSC_Malate, rc2->Enzyme_con->BSC_Malate);
    EXPECT_NE(rc->Leaf_con, rc2->Leaf_con);
    EXPECT_NE(rc->Enzyme_con, rc2->Enzyme_con);
    EXPECT_NE(rc->AE_con, rc2->AE_con);
    delete rc;
    delete rc2;
}

TEST_F(RAC4leafMetaConditionTest, ArrayTest) {
    RAC4leafMetaCondition::reset();
    arr input = get_random(RAC4leafMetaCondition::size());
    RAC4leafMetaCondition rac(input);
    arr output = rac.toArray();
    COMPARE(input, output)
}
}
