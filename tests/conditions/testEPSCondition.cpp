#include <random>
#include "VariableFramework.hpp"
#include "conditions/EPSCondition.hpp"
#include <nvector/nvector_serial.h>

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class EPSConditionTest : public VariableFramework {
};

TEST_F(EPSConditionTest, ConstructorTest) {
    EPSCondition EPS;
    EPS.CM_con->PS_PR_con->PR_con->HPRc = 2.5;
    EPS.FIBF_con->FI_con->PQn = 0.445;

    EPSCondition EPS1(&EPS);
    EXPECT_DOUBLE_EQ(EPS.CM_con->PS_PR_con->PR_con->HPRc, EPS1.CM_con->PS_PR_con->PR_con->HPRc);
    EXPECT_DOUBLE_EQ(EPS.FIBF_con->FI_con->PQn, EPS1.FIBF_con->FI_con->PQn);

    CMCondition* cm = new CMCondition();
    FIBFCondition* fic = new FIBFCondition();

    EPSCondition* EPS2 = new EPSCondition(fic, cm);

    EPSCondition* EPS3 = new EPSCondition(EPS2->FIBF_con, EPS2->CM_con);
    EXPECT_NE(EPS2->CM_con, EPS3->CM_con);
    EXPECT_NE(EPS2->FIBF_con, EPS3->FIBF_con);
    delete EPS2;
    delete EPS3;
}

TEST_F(EPSConditionTest, ArrayTest) {
    arr input = get_random(EPSCondition::size());
    EPSCondition EPS(input);
    arr output = EPS.toArray();
    COMPARE(input, output)

    N_Vector y = N_VNew_Serial(input.size());
    for (size_t i = 0; i < input.size(); i++)
        NV_Ith_S(y, i) = input[i];

    EPSCondition EPS2(N_VGetArrayPointer(y));
    arr output2 = EPS2.toArray();
    COMPARE(input, output2)
    N_VDestroy(y);
}
}
