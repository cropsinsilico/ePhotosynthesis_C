#include "RAC4leafMetaFramework.hpp"
#include "modules/Leaf_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::vel;

namespace {
TEST_F(RAC4leafMetaModuleTest, InitTest) {
    RAC4leafMetaCondition* rc = RAC4leafMeta::init(theVars);

    EXPECT_NE(rc->Leaf_con->Ci, 0.);
    delete rc;
}

TEST_F(RAC4leafMetaModuleTest, MBTest) {
    const EnzymeVel ev = theVars->Enzyme_Vel;
    RAC4leafMetaCondition* rc = RAC4leafMeta::init(theVars);
    arr res = MB(1.5, rc, theVars);

    EXPECT_NE(theVars->Enzyme_Vel.v1, ev.v1);
    EXPECT_NE(theVars->Enzyme_Vel.vSuc1, ev.vSuc1);

    DIFF(res, zeros(res.size()))
    delete rc;
}

}
