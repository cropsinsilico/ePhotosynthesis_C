#include "PS_PRModuleFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(PS_PRModuleTest, InitTest) {
    PSModuleTest::theVars->PR_PS_com = true;
    PS_PRCondition* psprc = PS_PR::init(PSModuleTest::theVars);
    EXPECT_TRUE(psprc->PR_con->getPS_RuBP());
    delete psprc;
}

TEST_F(PS_PRModuleTest, MBTest) {
    PS_PRCondition* psprc = PS_PR::init(PSModuleTest::theVars);
    arr res = MB(1.5, psprc, PSModuleTest::theVars);
    arr comp = zeros(PS_PRCondition::size());
    DIFF(res, comp)

    delete psprc;
}
}
