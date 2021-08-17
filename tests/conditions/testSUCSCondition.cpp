#include <random>
#include "VariableFramework.hpp"
#include "conditions/SUCSCondition.hpp"

namespace {

class SUCSConditionTest : public VariableFramework {

};

TEST_F(SUCSConditionTest, InitializeTest) {
    SUCSCondition* sc = new SUCSCondition();
    sc->T3Pc = 0.85;
    SUCSCondition* sc2 = new SUCSCondition(sc);
    EXPECT_DOUBLE_EQ(sc->T3Pc, sc2->T3Pc);

    sc->clear();
    EXPECT_NE(sc->T3Pc, sc2->T3Pc);
    delete sc;
    delete sc2;
}

TEST_F(SUCSConditionTest, ArrayTest) {
    arr input = get_random(SUCSCondition::size());
    SUCSCondition sc(input);
    arr output = sc.toArray();
    COMPARE(input, output)
}

}
