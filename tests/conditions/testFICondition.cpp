#include <random>
#include "FIModuleFramework.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(FIConditionTest, ConnectTest) {
    EXPECT_FALSE(FICondition::getBF_connect());
    FICondition::setBF_connect(true);

    EXPECT_TRUE(FICondition::getBF_connect());
}

TEST_F(FIConditionTest, SizeTest) {
    size_t size = FICondition::size();

    FICondition::setBF_connect(true);
    size_t newsize = FICondition::size();
    EXPECT_GT(size, newsize);
}

TEST_F(FIConditionTest, InitializerTest) {
    FICondition *fic = new FICondition();
    try {
        fic->A = 2.5;
        fic->U = 99.6;
        fic->PQn = 16.3;

        FICondition fic1(fic);
        EXPECT_DOUBLE_EQ(fic->A, fic1.A);
        EXPECT_DOUBLE_EQ(fic->U, fic1.U);
        EXPECT_DOUBLE_EQ(fic->PQn, fic1.PQn);

        FICondition::setBF_connect(true);
        FICondition fic2(fic);
        EXPECT_DOUBLE_EQ(fic->A, fic2.A);
        EXPECT_DOUBLE_EQ(fic->U, fic2.U);
        EXPECT_NE(fic->PQn, fic2.PQn);
        delete fic;
    }
    catch(...) {
        delete fic;
    }
}

TEST_F(FIConditionTest, ArrayTest) {
    arr input = get_random(FICondition::size());
    FICondition fic(input);

    arr output = fic.toArray();
    COMPARE(input, output)
    FICondition::setBF_connect(true);
    input = get_random(FICondition::size());
    EXPECT_LT(input.size(), output.size());
    fic.fromArray(input);
    output = fic.toArray();
    COMPARE(input, output)
}
}
