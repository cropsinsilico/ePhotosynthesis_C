#include "trDynaPSModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(trDynaPSModuleTest, InitTest) {
    RROEAModuleTest::theVars->RROEA_EPS_com = true;
    RROEAModuleTest::theVars->XanCycle_BF_com = true;
    trDynaPSCondition* tdc = trDynaPS::init(RROEAModuleTest::theVars);
    delete tdc;
}

TEST_F(trDynaPSModuleTest, MBTest) {
    RROEAModuleTest::theVars->RuACT_EPS_com = true;
    BF::setRROEA_connect(true);
    trDynaPSCondition* tdc = trDynaPS::init(RROEAModuleTest::theVars);
    arr res = MB(1.5, tdc, RROEAModuleTest::theVars);
    arr comp = zeros(trDynaPSCondition::size());
    DIFF(res, comp)

    delete tdc;
}
}
