#include "EPSModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(EPSModuleTest, InitTest) {
    EPSCondition* epsc = EPS::init(CMModuleTest::theVars);
    EXPECT_TRUE(epsc->CM_con->PS_PR_con->PR_con->getPS_RuBP());
}

TEST_F(EPSModuleTest, MBTest) {
    CMModuleTest::theVars->FIBF_PSPR_com = true;
    Variables* orig = new Variables(CMModuleTest::theVars);
    BF::setPS_connect(true);

    EPSCondition* epsc = EPS::init(CMModuleTest::theVars);
    arr res = MB(1.5, epsc, CMModuleTest::theVars);
    arr comp = zeros(EPSCondition::size());
    DIFF(res, comp)

    delete epsc;
    orig->useC3 = true;
    epsc = EPS::init(orig);
    arr res1 = MB(1.5, epsc, orig);
    comp = zeros(EPSCondition::size());
    DIFF(res1, comp)

    delete epsc;
    delete orig;
}
}
