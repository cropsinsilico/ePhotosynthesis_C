#include "RAModuleFramework.hpp"

namespace {

TEST_F(RAModuleTest, InitTest) {
    EPSModuleTest::theVars->RuACT_EPS_com = true;
    RACondition* rac = RA::init(EPSModuleTest::theVars);
    delete rac;
}

TEST_F(RAModuleTest, MBTest) {
    RACondition* rac = RA::init(EPSModuleTest::theVars);
    arr res = MB(1.5, rac, EPSModuleTest::theVars);
    arr comp = zeros(RACondition::size());
    DIFF(res, comp)

    delete rac;
}
}
