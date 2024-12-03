#include "PRModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(PRModuleTest, InitTest) {

    PRCondition* PRc = PR::init(theVars);

#ifdef SUNDIALS_CONTEXT_REQUIRED
    Variables* theVars2 = new Variables(&context);
#else // SUNDIALS_CONTEXT_REQUIRED
    Variables* theVars2 = new Variables();
#endif // SUNDIALS_CONTEXT_REQUIRED
    theVars2->CO2_in = 500.;
    theVars2->TestLi = 14.;
    theVars2->TestATPCost = 4.5;
    theVars2->useC3 = true;
    theVars2->EnzymeAct = Emap;
    PRCondition* prc2 = PR::init(theVars2);

    delete PRc;
    delete prc2;
    delete theVars2;
}

TEST_F(PRModuleTest, RateTest) {
    const PRVel PRv = theVars->PR_Vel;
    theVars->RUBISCOMETHOD = 2;
    PRCondition* PRc = PR::init(theVars);

    Rate(1., PRc, theVars);

    EXPECT_NE(theVars->PR_Vel.v112, PRv.v112);
    EXPECT_NE(theVars->PR_Vel.v124, PRv.v124);
    EXPECT_NE(theVars->PR_Vel.v131, PRv.v131);

    delete PRc;
}

TEST_F(PRModuleTest, RateTest2) {
    const PRVel PRv = theVars->PR_Vel;
    theVars->Tp = 27.9;
    theVars->RUBISCOMETHOD = 1;
    PR::setRUBISCOTOTAL(3);
    PRCondition* PRc = PR::init(theVars);
    theVars->record = true;
    Rate(1., PRc, theVars);
    Rate(1., PRc, theVars);
    Rate(2., PRc, theVars);
    EXPECT_NE(theVars->PR_Vel.v112, PRv.v112);
    EXPECT_NE(theVars->PR_Vel.v124, PRv.v124);
    EXPECT_NE(theVars->PR_Vel.v131, PRv.v131);

    EXPECT_EQ(2, theVars->PR_VEL.size());
    delete PRc;
}

TEST_F(PRModuleTest, MBTest) {
    const PRVel PRv = theVars->PR_Vel;
    PRCondition* PRc = PR::init(theVars);
    arr res = MB(1.5, PRc, theVars);

    EXPECT_NE(theVars->PR_Vel.v112, PRv.v112);
    EXPECT_NE(theVars->PR_Vel.v124, PRv.v124);
    EXPECT_NE(theVars->PR_Vel.v131, PRv.v131);

    DIFF(res, zeros(res.size()))
    delete PRc;
}
}
