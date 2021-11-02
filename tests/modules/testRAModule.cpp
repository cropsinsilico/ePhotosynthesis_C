#include "RAModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(RAModuleTest, InitTest) {
    RA::reset();
    EPSModuleTest::theVars->RuACT_EPS_com = true;
    RACondition* rac = RA::init(EPSModuleTest::theVars);
    delete rac;
}

TEST_F(RAModuleTest, MBTest) {
    RA::reset();
    RACondition* rac = RA::init(EPSModuleTest::theVars);
    arr res = MB(1.5, rac, EPSModuleTest::theVars);
    arr comp = zeros(RACondition::size());
    DIFF(res, comp)

    delete rac;
}
}
