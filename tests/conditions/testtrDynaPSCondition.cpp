#include <random>
#include "trDynaPSModuleFramework.hpp"
#include <nvector/nvector_serial.h>

namespace {
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(trDynaPSConditionTest, ConstructorTest) {
    trDynaPSCondition trDynaPS;
    trDynaPS.DynaPS_con->RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc = 2.5;
    trDynaPS.RROEA_con->ATPGPP = 0.445;

    trDynaPSCondition trDynaPS1(&trDynaPS);
    EXPECT_DOUBLE_EQ(trDynaPS.DynaPS_con->RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc, trDynaPS1.DynaPS_con->RA_con->EPS_con->CM_con->PS_PR_con->PR_con->HPRc);
    EXPECT_DOUBLE_EQ(trDynaPS.RROEA_con->ATPGPP, trDynaPS1.RROEA_con->ATPGPP);

    DynaPSCondition* dc = new DynaPSCondition();
    RROEACondition* rc = new RROEACondition();

    trDynaPSCondition* trDynaPS2 = new trDynaPSCondition(dc, rc);

    trDynaPSCondition* trDynaPS3 = new trDynaPSCondition(trDynaPS2->DynaPS_con, trDynaPS2->RROEA_con);
    EXPECT_NE(trDynaPS2->DynaPS_con, trDynaPS3->DynaPS_con);
    EXPECT_NE(trDynaPS2->RROEA_con, trDynaPS3->RROEA_con);
    delete trDynaPS2;
    delete trDynaPS3;
}

TEST_F(trDynaPSConditionTest, ArrayTest) {
    arr input = get_random(trDynaPSCondition::size());
    trDynaPSCondition trDynaPS(input);
    arr output = trDynaPS.toArray();
    COMPARE(input, output)

#ifdef SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(input.size()), context);
#else // SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(input.size());
#endif // SUNDIALS_CONTEXT_REQUIRED
    sunrealtype* y_ptr = N_VGetArrayPointer(y);
    for (size_t i = 0; i < input.size(); i++)
        y_ptr[i] = input[i];

    trDynaPSCondition trDynaPS2(N_VGetArrayPointer(y));
    arr output2 = trDynaPS2.toArray();
    COMPARE(input, output2)
    N_VDestroy(y);
}
TEST_F(trDynaPSConditionTest, OutputTest) {
    trDynaPSCondition trDynaPS;
    std::cout << trDynaPS << std::endl;
}
}
