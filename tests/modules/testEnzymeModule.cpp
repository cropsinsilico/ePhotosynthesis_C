#include "EnzymeModuleFramework.hpp"
#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(EnzymeModuleTest, InitTest) {
    EnzymeCondition* ec = Enzyme::init(theVars);

    EXPECT_NE(ec->MC_HCO3, 0.);
    delete ec;
}

}
