#include <random>
#include "RedoxRegModuleFramework.hpp"

namespace {

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::test;

TEST_F(RedoxRegConditionTest, InitializerTest) {
    RedoxRegCondition RedoxReg;
    RedoxReg.Thion = 3.5;
    RedoxReg.RA_con->RuACT_con->ER = 0.0045;

    RedoxRegCondition RedoxReg1(&RedoxReg);
    EXPECT_DOUBLE_EQ(RedoxReg.Thion, RedoxReg1.Thion);
    EXPECT_DOUBLE_EQ(RedoxReg.RA_con->RuACT_con->ER, RedoxReg1.RA_con->RuACT_con->ER);
    EXPECT_NE(RedoxReg.RA_con, RedoxReg1.RA_con);

    RedoxRegCondition RedoxReg2;
    RedoxReg2 = RedoxReg;
    EXPECT_DOUBLE_EQ(RedoxReg.Thion, RedoxReg2.Thion);
    EXPECT_DOUBLE_EQ(RedoxReg.RA_con->RuACT_con->ER, RedoxReg2.RA_con->RuACT_con->ER);
    EXPECT_NE((void*)(RedoxReg.RA_con), (void*)(RedoxReg2.RA_con));
    EXPECT_NE(RedoxReg.RA_con, RedoxReg2.RA_con);
    

    RACondition* rc = new RACondition();
    rc->RuACT_con->ER = 0.0054;
    const double thio = 4.25e-7;
    RedoxRegCondition* rr = new RedoxRegCondition(rc, thio);
    EXPECT_DOUBLE_EQ(rc->RuACT_con->ER, rr->RA_con->RuACT_con->ER);
    EXPECT_DOUBLE_EQ(thio, rr->Thion);
    EXPECT_EQ(rr->RA_con, rc);
    RedoxRegCondition* rr2 = new RedoxRegCondition(rr->RA_con, thio);
    EXPECT_DOUBLE_EQ(rc->RuACT_con->ER, rr2->RA_con->RuACT_con->ER);
    EXPECT_DOUBLE_EQ(thio, rr2->Thion);
    EXPECT_NE(rr2->RA_con, rr->RA_con);

    delete rr;
    delete rr2;

}



TEST_F(RedoxRegConditionTest, ArrayTest) {
    arr input = get_random(RedoxRegCondition::size());
    RedoxRegCondition pc(input);
    arr output = pc.toArray();
    COMPARE(input, output)
}

TEST_F(RedoxRegConditionTest, OutputTest) {
  RedoxRegCondition RedoxReg;
  std::cout << RedoxReg << std::endl;
}
  
}
