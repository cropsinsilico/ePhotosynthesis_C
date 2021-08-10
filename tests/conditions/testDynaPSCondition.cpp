#include <random>
#include "VariableFramework.hpp"
#include "conditions/DynaPSCondition.hpp"
#include <nvector/nvector_serial.h>

namespace {
class DynaPSConditionTest : public VariableFramework {
};

TEST_F(DynaPSConditionTest, ConstructorTest) {
    DynaPSCondition DynaPS;
    DynaPS.RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc = 2.5;
    DynaPS.XanCycle_con->Ax = 0.445;

    DynaPSCondition DynaPS1(&DynaPS);
    EXPECT_DOUBLE_EQ(DynaPS.RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc, DynaPS1.RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc);
    EXPECT_DOUBLE_EQ(DynaPS.XanCycle_con->Ax, DynaPS1.XanCycle_con->Ax);

    RACondition* rc = new RACondition();
    XanCycleCondition* xc = new XanCycleCondition();

    DynaPSCondition* DynaPS2 = new DynaPSCondition(rc, xc);

    DynaPSCondition* DynaPS3 = new DynaPSCondition(DynaPS2->RA_con, DynaPS2->XanCycle_con);
    EXPECT_NE(DynaPS2->RA_con, DynaPS3->RA_con);
    EXPECT_NE(DynaPS2->XanCycle_con, DynaPS3->XanCycle_con);
    delete DynaPS2;
    delete DynaPS3;
}

TEST_F(DynaPSConditionTest, ArrayTest) {
    arr input = get_random(DynaPSCondition::size());
    DynaPSCondition DynaPS(input);
    arr output = DynaPS.toArray();
    COMPARE(input, output)

    N_Vector y = N_VNew_Serial(input.size());
    for (size_t i = 0; i < input.size(); i++)
        NV_Ith_S(y, i) = input[i];

    DynaPSCondition DynaPS2(N_VGetArrayPointer(y));
    arr output2 = DynaPS2.toArray();
    COMPARE(input, output2)
    N_VDestroy(y);
}
}
