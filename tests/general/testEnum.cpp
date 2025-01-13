#include "enums/enums_utils.hpp"
#include "VariableFramework.hpp"


using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;

namespace {

  class EnumTest : public VariableFramework {
  };

  TEST_F(EnumTest, TestSerialization) {
#define CHECK_SERI(x, exp)                      \
    {                                           \
      std::string expected = exp;               \
      std::ostringstream oss;                   \
      oss << x;                                 \
      EXPECT_EQ(oss.str(), expected);           \
    }
    CHECK_SERI(MODULE_BF, "BF");
    CHECK_SERI(PARAM_TYPE_MOD, "MOD");
    CHECK_SERI(Variables::EnumClass::TestLi, "TestLi");
    {
      std::vector<Variables::EnumType> x = {
        Variables::EnumClass::TestLi,
        Variables::EnumClass::alpha2,
        Variables::EnumClass::ADP,
      };
      CHECK_SERI(x, "[TestLi, alpha2, ADP]");
    }
    {
      std::map<Variables::EnumType, int> x = {
        {Variables::EnumClass::ADP, 3},
        {Variables::EnumClass::TestLi, 1},
        {Variables::EnumClass::alpha2, 2}
      };
      CHECK_SERI(x,
                 "  ADP      3\n"
                 "  TestLi      1\n"
                 "  alpha2      2\n");
    }
#undef CHECK_SERI
  }
  
}
