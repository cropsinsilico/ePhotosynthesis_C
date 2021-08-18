#include "VariableFramework.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::vel;

namespace {

TEST_F(VariableFramework, Sizes) {
    EXPECT_EQ(theVars->PR_Param.size(), 2);
    EXPECT_EQ(theVars->PSRatio.size(), 103);
}

TEST_F(VariableFramework, TestCopy) {
    BFVel bfv;
    theVars->BF_VEL.insert(2, 0.5, bfv);
    theVars->BF_FI_com = true;
    Variables tempVar(theVars);
    EXPECT_NE(tempVar.BF_VEL.size(), theVars->BF_VEL.size());
    EXPECT_EQ(theVars->BF_VEL.size(), 1);
    EXPECT_EQ(tempVar.BF_FI_com, theVars->BF_FI_com);
    EXPECT_TRUE(theVars->BF_FI_com);

    Variables temp2(tempVar);
    EXPECT_EQ(tempVar.BF_VEL.size(), temp2.BF_VEL.size());
    EXPECT_EQ(temp2.BF_VEL.size(), 0);
    EXPECT_EQ(tempVar.BF_FI_com, temp2.BF_FI_com);
    EXPECT_TRUE(temp2.BF_FI_com);
}
}
