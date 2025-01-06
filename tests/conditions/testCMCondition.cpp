#include <random>
#include "VariableFramework.hpp"
#include "conditions/CMCondition.hpp"
#include <nvector/nvector_serial.h>

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

class CMConditionTest : public VariableFramework {
};

TEST_F(CMConditionTest, ConstructorTest) {
    CMCondition cm;
    cm.PS_PR_con->PR_con->HPRc = 2.5;
    cm.SUCS_con->HexPc = 0.445;

    CMCondition cm1(&cm);
    EXPECT_DOUBLE_EQ(cm.PS_PR_con->PR_con->HPRc, cm1.PS_PR_con->PR_con->HPRc);
    EXPECT_DOUBLE_EQ(cm.SUCS_con->HexPc, cm1.SUCS_con->HexPc);

    PS_PRCondition* pspr = new PS_PRCondition();
    SUCSCondition* sc = new SUCSCondition();

    CMCondition* cm2 = new CMCondition(pspr, sc);

    CMCondition* cm3 = new CMCondition(cm2->PS_PR_con, cm2->SUCS_con);
    EXPECT_NE(cm2->PS_PR_con, cm3->PS_PR_con);
    EXPECT_NE(cm2->SUCS_con, cm3->SUCS_con);
    delete cm2;
    delete cm3;
}

TEST_F(CMConditionTest, ArrayTest) {
    arr input = get_random(CMCondition::size());
    CMCondition cm(input);
    arr output = cm.toArray();
    COMPARE(input, output)

    // TODO: Check for older versions of Sundials for backwards compat
#ifdef SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(input.size()), context);
#else // SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(input.size());
#endif // SUNDIALS_CONTEXT_REQUIRED
    sunrealtype* y_ptr = N_VGetArrayPointer(y);
    for (size_t i = 0; i < input.size(); i++)
        y_ptr[i] = input[i];
    // N_Vector y = N_VNew_Serial(input.size(), context);
    // for (size_t i = 0; i < input.size(); i++)
    //     NV_Ith_S(y, i) = input[i];

    CMCondition cm2(N_VGetArrayPointer(y));
    arr output2 = cm2.toArray();
    COMPARE(input, output2)
    N_VDestroy(y);
}
}
