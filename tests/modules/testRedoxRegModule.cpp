#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */
#include "RedoxRegModuleFramework.hpp"

namespace {
TEST_F(RedoxRegModuleTest, InitTest) {

    /*RedoxRegCondition* RedoxRegc = RedoxReg::init(theVars);

    Variables* theVars2 = new Variables();
    theVars2->CO2_in = 500.;
    theVars2->TestLi = 14.;
    theVars2->TestATPCost = 4.5;
    theVars2->useC3 = true;
    theVars2->EnzymeAct = Emap;

    RedoxRegCondition* RedoxRegc2 = RedoxReg::init(theVars2);

    EXPECT_NE(theVars->RedoxReg_MP.size(), 0);
    for (size_t x = 0; x < theVars->RedoxReg_MP.size(); x++) {
        EXPECT_NE(theVars->RedoxReg_MP[x].size(), 0);
        for (size_t y = 0; y < theVars->RedoxReg_MP[x].size(); x++) {
            EXPECT_NE(theVars->RedoxReg_MP[x][y], 0.);
        }
    }
    delete RedoxRegc;
    delete RedoxRegc2;
    delete theVars2;
    */
}

TEST_F(RedoxRegModuleTest, RateTest) {
    const RedoxRegVel RedoxRegv = theVars->RedoxReg_Vel;
    RedoxRegCondition* RedoxRegc = RedoxReg::init(theVars);

    Rate(1., RedoxRegc, theVars);

    delete RedoxRegc;
}

TEST_F(RedoxRegModuleTest, RateTest2) {
    /*const RedoxRegVel RedoxRegv = theVars->RedoxReg_Vel;
    RedoxRegCondition* RedoxRegc = RedoxReg::init(theVars);
    RedoxReg::settrDynaPS2RedReg_cal(true);
    theVars->record = true;
    Rate(1., RedoxRegc, theVars);
    Rate(1., RedoxRegc, theVars);
    Rate(2., RedoxRegc, theVars);
    EXPECT_EQ(2, theVars->RedoxReg_VEL.size());
    delete RedoxRegc;*/
}

TEST_F(RedoxRegModuleTest, MBTest) {
    /*const RedoxRegVel RedoxRegv = theVars->RedoxReg_Vel;
    RedoxRegCondition* RedoxRegc = RedoxReg::init(theVars);
    arr res = MB(1.5, RedoxRegc, theVars);

    DIFF(res, zeros(res.size()))
    delete RedoxRegc;*/
}

TEST_F(RedoxRegModuleTest, FPercentTest) {

}
}
