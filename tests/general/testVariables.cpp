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

TEST_F(VariableFramework, TestDeepcopy) {
    BFVel bfv;
    theVars->BF_VEL.insert(2, 0.5, bfv);
    theVars->BF_FI_com = true;
    Variables* tempVar = theVars->deepcopy();
    EXPECT_EQ(tempVar->BF_VEL.size(), theVars->BF_VEL.size());
    EXPECT_EQ(theVars->BF_VEL.size(), 1);
    EXPECT_EQ(tempVar->BF_FI_com, theVars->BF_FI_com);
    EXPECT_TRUE(theVars->BF_FI_com);

    Variables* temp2 = tempVar->deepcopy();
    EXPECT_EQ(tempVar->BF_VEL.size(), temp2->BF_VEL.size());
    EXPECT_EQ(temp2->BF_VEL.size(), 1);
    EXPECT_EQ(tempVar->BF_FI_com, temp2->BF_FI_com);
    EXPECT_TRUE(temp2->BF_FI_com);

    delete tempVar;
    delete temp2;
}
TEST_F(VariableFramework, TestOutput) {
    std::cout << theVars << std::endl;
}

TEST_F(VariableFramework, TestParseVar) {
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    std::string k, name;
#define TEST_VAR(src, dst)                                      \
    {                                                           \
      mod = MODULE_NONE;                                        \
      pt = PARAM_TYPE_NONE;                                     \
      k = #src;                                                 \
      name = "";                                                \
      name = Variables::parseVar(k, mod, pt);                   \
      EXPECT_EQ(name, #dst);                                    \
      EXPECT_EQ(mod, MODULE_ALL);                               \
      EXPECT_EQ(pt, PARAM_TYPE_VARS);                           \
    }
    TEST_VAR(Tp, Tp);
    TEST_VAR(WeatherTemperature, Tp);
    TEST_VAR(TestLi, TestLi);
    TEST_VAR(PAR, TestLi);
    TEST_VAR(Radiation_PAR, TestLi);
    TEST_VAR(CO2, CO2_in);
    TEST_VAR(Air_CO2, CO2_in);
#undef TEST_VAR
}
TEST_F(VariableFramework, TestReadDefaults) {
    std::map<std::string, std::string> inputs;
    TempParamFile tmp("ALL::VARS::AVR");
    double AVR_DEFAULT0 = Variables::getDefault(MODULE_ALL,
                                                PARAM_TYPE_VARS,
                                                "AVR");
    EXPECT_NE(AVR_DEFAULT0, tmp.value);
    Variables::readDefaults(tmp.filename, inputs);
    double AVR_DEFAULT1 = Variables::getDefault(MODULE_ALL,
                                                PARAM_TYPE_VARS,
                                                "AVR");
    EXPECT_NE(AVR_DEFAULT1, AVR_DEFAULT0);
    EXPECT_EQ(AVR_DEFAULT1, tmp.value);
    Variables* var1 = new Variables();
    EXPECT_EQ(var1->AVR, AVR_DEFAULT1);
    delete var1;
    var1 = nullptr;

    TempParamFile tmp2("ALL::VARS::AVR", MODULE_NONE,
                       PARAM_TYPE_NONE, 33);
    EXPECT_NE(tmp2.value, tmp.value);
    Variables::readDefaults(tmp2.filename, inputs);
    double AVR_DEFAULT2 = Variables::getDefault(MODULE_ALL,
                                                PARAM_TYPE_VARS,
                                                "AVR");
    EXPECT_EQ(AVR_DEFAULT2, AVR_DEFAULT1);

    TempParamFile tmp3("INVALID_VAR32");
    Variables::readDefaults(tmp3.filename);
    
    Variables::setDefault(MODULE_ALL, PARAM_TYPE_VARS,
                          "AVR", AVR_DEFAULT0);
}
TEST_F(VariableFramework, TestReadParam) {
    std::map<std::string, std::string> inputs;
    Variables tempVar(theVars);
    std::vector<std::string> names({"ALL::VARS::AVR", "Tp"});
    TempParamFile tmp(names);
    tempVar.readParam(tmp.filename, inputs);
    EXPECT_TRUE(tempVar.has("Tp"));
    EXPECT_TRUE(tempVar.hasVar(MODULE_ALL, PARAM_TYPE_VARS, "Tp"));
    EXPECT_TRUE(tempVar.hasVar("Tp"));
    EXPECT_EQ(tempVar.AVR, tmp.value);
    EXPECT_EQ(tempVar.Tp, tmp.value);

    TempParamFile tmp2(names, MODULE_NONE, PARAM_TYPE_NONE, 33);
    EXPECT_NE(tmp2.value, tmp.value);
    tempVar.readParam(tmp2.filename, inputs);
    EXPECT_EQ(tempVar.AVR, tmp.value);
    EXPECT_EQ(tempVar.Tp, tmp.value);

    TempParamFile tmp3("INVALID_VAR32");
    tempVar.readParam(tmp3.filename);
}
TEST_F(VariableFramework, TestReadEnzymeAct) {
    // TODO
}
TEST_F(VariableFramework, TestReadGRN) {
    // TODO
}

}
