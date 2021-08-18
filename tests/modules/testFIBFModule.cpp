#include "FIBFModuleFramework.hpp"
#include "modules/BF.hpp"
#include "modules/FI.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(FIBFModuleTest, InitTest) {
    FIBFCondition* fibfc = FIBF::init(BFModuleTest::theVars);
    EXPECT_TRUE(fibfc == nullptr);
}

TEST_F(FIBFModuleTest, MBTest) {
    FI::setBF_connect(true);
    BFCondition* bfc = BF::init(BFModuleTest::theVars);
    FICondition* fic = FI::init(BFModuleTest::theVars);
    FIBFCondition* fibfc = new FIBFCondition(bfc, fic);
    arr res = MB(1.5, fibfc, BFModuleTest::theVars);
    arr comp = zeros(FIBFCondition::size());
    DIFF(res, comp)
    delete fibfc;
}
}
