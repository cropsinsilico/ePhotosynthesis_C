#include "SUCSModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(SUCSModuleTest, InitTest) {

    SUCSCondition* SUCSc = SUCS::init(theVars);

    Variables* theVars2 = new Variables();
    theVars2->CO2_in = 500.;
    theVars2->TestLi = 14.;
    theVars2->TestATPCost = 4.5;
    theVars2->useC3 = true;
    theVars2->EnzymeAct = Emap;

    SUCSCondition* SUCSc2 = SUCS::init(theVars2);

    EXPECT_NE(theVars->SUCS_Pool.ATc, theVars2->SUCS_Pool.ATc);
    delete SUCSc;
    delete SUCSc2;
    delete theVars2;
}

TEST_F(SUCSModuleTest, RateTest) {
    const SUCSVel SUCSv = theVars->SUCS_Vel;
    SUCSCondition* SUCSc = SUCS::init(theVars);

    Rate(1., SUCSc, theVars);
    EXPECT_NE(SUCSv.v51, theVars->SUCS_Vel.v51);
    EXPECT_NE(SUCSv.v59, theVars->SUCS_Vel.v59);

    delete SUCSc;
}

TEST_F(SUCSModuleTest, RateTest2) {
    const SUCSVel SUCSv = theVars->SUCS_Vel;
    SUCSCondition* SUCSc = SUCS::init(theVars);
    theVars->record = true;
    Rate(1., SUCSc, theVars);
    Rate(1., SUCSc, theVars);
    Rate(2., SUCSc, theVars);
    EXPECT_NE(SUCSv.v51, theVars->SUCS_Vel.v51);
    EXPECT_NE(SUCSv.v59, theVars->SUCS_Vel.v59);
    EXPECT_EQ(2, theVars->SUCS_VEL.size());

    delete SUCSc;
}

TEST_F(SUCSModuleTest, MBTest) {
    const SUCSVel SUCSv = theVars->SUCS_Vel;
    SUCSCondition* SUCSc = SUCS::init(theVars);
    arr res = MB(1.5, SUCSc, theVars);

    EXPECT_NE(SUCSv.v51, theVars->SUCS_Vel.v51);
    EXPECT_NE(SUCSv.v59, theVars->SUCS_Vel.v59);

    DIFF(res, zeros(res.size()))
    delete SUCSc;
}
}
