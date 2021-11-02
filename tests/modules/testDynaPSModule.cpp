#include "DynaPSModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(DynaPSModuleTest, InitTest) {
    DynaPSCondition* dc = DynaPS::init(RAModuleTest::theVars);
    delete dc;
}

TEST_F(DynaPSModuleTest, MBTest) {
    DynaPS::reset();
    DynaPSCondition* dc = DynaPS::init(RAModuleTest::theVars);
    arr res = MB(1.5, dc, RAModuleTest::theVars);
    arr comp = zeros(DynaPSCondition::size());
    DIFF(res, comp)

    delete dc;
}
}
