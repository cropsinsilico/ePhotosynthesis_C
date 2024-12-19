//#include <random>
#include "BFModuleFramework.hpp"

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(BFConditionTest, ConnectTest) {
    EXPECT_FALSE(BFCondition::getPS_connect());
    EXPECT_FALSE(BFCondition::getRROEA_connect());
    EXPECT_FALSE(BFCondition::getFI_connect());
    BFCondition::setPS_connect(true);
    BFCondition::setRROEA_connect(true);
    BFCondition::setFI_connect(true);

    EXPECT_TRUE(BFCondition::getPS_connect());
    EXPECT_TRUE(BFCondition::getRROEA_connect());
    EXPECT_TRUE(BFCondition::getFI_connect());
}

TEST_F(BFConditionTest, SizeTest) {
    size_t size = BFCondition::size();

    BFCondition::setPS_connect(true);
    size_t newsize = BFCondition::size();
    EXPECT_GT(size, newsize);
    size = newsize;
    BFCondition::setRROEA_connect(true);
    newsize = BFCondition::size();
    EXPECT_GT(size, newsize);
    size = newsize;
    BFCondition::setFI_connect(true);
    newsize = BFCondition::size();
    EXPECT_GT(size, newsize);
}

TEST_F(BFConditionTest, InitializerTest) {
    BFCondition *bfc = new BFCondition();
    try {
        bfc->Q = 2.5;
        bfc->cytbH = 99.6;
        bfc->ATP = 16.3;
        bfc->Fdn = 55.3;
        BFCondition bfc1(bfc);
        EXPECT_DOUBLE_EQ(bfc->Q, bfc1.Q);
        EXPECT_DOUBLE_EQ(bfc->cytbH, bfc1.cytbH);
        EXPECT_DOUBLE_EQ(bfc->ATP, bfc1.ATP);
        EXPECT_DOUBLE_EQ(bfc->Fdn, bfc1.Fdn);

        BFCondition::setPS_connect(true);
        BFCondition bfc2(bfc);
        EXPECT_DOUBLE_EQ(bfc->Q, bfc2.Q);
        EXPECT_DOUBLE_EQ(bfc->cytbH, bfc2.cytbH);
        EXPECT_NE(bfc->ATP, bfc2.ATP);
        EXPECT_DOUBLE_EQ(bfc->Fdn, bfc2.Fdn);

        BFCondition::setFI_connect(true);
        BFCondition bfc3(bfc);
        EXPECT_NE(bfc->Q, bfc3.Q);
        EXPECT_DOUBLE_EQ(bfc->cytbH, bfc3.cytbH);
        EXPECT_NE(bfc->ATP, bfc3.ATP);
        EXPECT_DOUBLE_EQ(bfc->Fdn, bfc3.Fdn);

        BFCondition::setRROEA_connect(true);
        BFCondition bfc4(bfc);
        EXPECT_NE(bfc->Q, bfc4.Q);
        EXPECT_DOUBLE_EQ(bfc->cytbH, bfc4.cytbH);
        EXPECT_NE(bfc->ATP, bfc4.ATP);
        EXPECT_NE(bfc->Fdn, bfc4.Fdn);

	// Value from default parameter files
	bfc4.initValues();
	EXPECT_NE(bfc->cytbH, bfc4.cytbH);
	EXPECT_DOUBLE_EQ(bfc4.cytbH, 1.0);
	
        delete bfc;
    }
    catch(...) {
        delete bfc;
    }

}

TEST_F(BFConditionTest, ArrayTest) {
    arr input = get_random(BFCondition::size());
    BFCondition bfc(input);

    arr output = bfc.toArray();
    COMPARE(input, output)
    BFCondition::setFI_connect(true);
    input = get_random(BFCondition::size());
    EXPECT_LT(input.size(), output.size());
    bfc.fromArray(input);
    output = bfc.toArray();
    COMPARE(input, output)
    BFCondition::setRROEA_connect(true);
    input = get_random(BFCondition::size());
    EXPECT_LT(input.size(), output.size());
    bfc.fromArray(input);
    output = bfc.toArray();
    COMPARE(input, output)
    BFCondition::setPS_connect(true);
    input = get_random(BFCondition::size());
    EXPECT_LT(input.size(), output.size());
    bfc.fromArray(input);
    output = bfc.toArray();
    COMPARE(input, output)

}
}
