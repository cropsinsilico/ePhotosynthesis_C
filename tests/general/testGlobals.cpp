#include "globals.hpp"
#include "VariableFramework.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "modules/FIBF.hpp"
#include <stdio.h>
#include <fstream>
#include <map>

using ::testing::ElementsAre;
using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::vel;
using namespace ePhotosynthesis::modules;

namespace {

class ConditionTest : public VariableFramework {

};

TEST_F(ConditionTest, TestUseC3) {
    theVars->useC3 = true;
    theVars->CO2_in = 1.0;
    Condition(0, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (1./3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);

    theVars->CO2_in = 125.;
    theVars->TestLi = 425;
    Condition(5, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (125./3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, 425*.7225);

}

TEST_F(ConditionTest, TestGeneral) {
    theVars->CO2_in = 1.0;
    theVars->TestLi = 1.0;
    Condition(0, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (.7/3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, .7225);

    theVars->CO2_in = 125.;
    theVars->TestLi = 425;
    Condition(5, theVars);
    EXPECT_DOUBLE_EQ(theVars->CO2_cond, (125.*.7/3.)*1.e-4);
    EXPECT_DOUBLE_EQ(theVars->O2_cond, .2646);
    EXPECT_DOUBLE_EQ(theVars->GLight, 425*.7225);

}

TEST_F(ConditionTest, TestReadFile) {
    std::string filename(tmpnam(nullptr));
    std::ofstream temp1;
    temp1.open(filename);
    temp1 << "CO2  150" << std::endl;
    temp1 << "PAR\t850" << std::endl << std::endl;
    temp1 << "SucPath     1" << std::endl;
    temp1.close();

    std::map<std::string, std::string> map1;
    readFile(filename, map1);
    EXPECT_EQ("150", map1.at("CO2"));
    EXPECT_EQ("850", map1.at("PAR"));
    EXPECT_EQ("1", map1.at("SucPath"));
    remove(filename.c_str());
    EXPECT_THROW(readFile(filename, map1), std::runtime_error);

    std::string filename2(tmpnam(nullptr));
    temp1.open(filename2);
    for (int i = 0; i <= 35; i++) {
        temp1 << i << "  " << i << std::endl << std::endl;
    }
    temp1.close();
    std::map<std::string, double> map2;
    readFile(filename2, map2, true);
    for (int i = 0; i < 27; i++) {
        EXPECT_DOUBLE_EQ(map2.at(std::to_string(i)), i/30.);
    }
    for (int i = 27; i <= 35; i++) {
        EXPECT_DOUBLE_EQ(map2.at(std::to_string(i)), i);
    }
    remove(filename2.c_str());
    EXPECT_THROW(readFile(filename2, map2), std::runtime_error);
}
class ParamSetTest : public VariableFramework {

};

TEST_F(ParamSetTest, TestGeneral) {
    const std::vector<double> values = {1.75, 6.9};
    for (auto val : values) {
        theVars->PS12ratio = val;
        ParamSet(theVars);
        ASSERT_THAT(theVars->BF_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->FI_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->RuACT_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->XanCycle_Param, ElementsAre(0., val));
        ASSERT_THAT(theVars->RROEA_Param, ElementsAre(0., val));
    }
}
class TargetFunValTest : public VariableFramework {

};

TEST_F(TargetFunValTest, TestGeneral) {
    theVars->record = false;
    PSVel psv, psv1;
    psv.v1 = 1.5;
    psv.v5 = 2.6;
    psv1.v1 = 1.5;
    psv1.v5 = 2.6;
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PS_VEL.insert(1, 0.5, psv1);
    PRVel prv, prv1;
    prv.v111 = 1.5;
    prv.v113 = 2.6;
    prv1.v111 = 1.5;
    prv1.v113 = 2.6;
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PS_VEL.insert(1, 0.5, psv1);
    theVars->PR_VEL.insert(0, 0., prv);
    theVars->PR_VEL.insert(1, 0.5, prv1);
    EXPECT_DOUBLE_EQ(0., TargetFunVal(theVars));
    for (size_t i = 0; i < theVars->PS_VEL.size(); i++) {
        EXPECT_DOUBLE_EQ(1.5, theVars->PS_VEL[i].v1);
        EXPECT_DOUBLE_EQ(2.6, theVars->PS_VEL[i].v5);
        EXPECT_DOUBLE_EQ(0., theVars->PS_VEL[i].v6);
    }
    for (size_t i = 0; i < theVars->PR_VEL.size(); i++) {
        EXPECT_DOUBLE_EQ(1.5, theVars->PR_VEL[i].v111);
        EXPECT_DOUBLE_EQ(2.6, theVars->PR_VEL[i].v113);
        EXPECT_DOUBLE_EQ(0., theVars->PR_VEL[i].v123);
    }
    theVars->record = true;
    EXPECT_NE(0., TargetFunVal(theVars));
    for (size_t i = 0; i < theVars->PS_VEL.size(); i++) {
        EXPECT_NE(1.5, theVars->PS_VEL[i].v1);
        EXPECT_NE(2.6, theVars->PS_VEL[i].v5);
        EXPECT_DOUBLE_EQ(0., theVars->PS_VEL[i].v6);
    }
    for (size_t i = 0; i < theVars->PR_VEL.size(); i++) {
        EXPECT_NE(1.5, theVars->PR_VEL[i].v111);
        EXPECT_NE(2.6, theVars->PR_VEL[i].v113);
        EXPECT_DOUBLE_EQ(0., theVars->PR_VEL[i].v123);
    }
}

class GenOutTest : public VariableFramework {

};

TEST_F(GenOutTest, GenOut) {
    theVars->record = false;
    GenOut(0, theVars);
    EXPECT_EQ(0, theVars->CO2A.size());
    theVars->record = true;
    GenOut(1, theVars);
    EXPECT_EQ(1, theVars->CO2A.size());
    theVars->FIBF_PSPR_com = true;
    theVars->RuACT_EPS_com = true;
    GenOut(2, theVars);
    EXPECT_EQ(1, theVars->CO2A.size());
}

TEST_F(GenOutTest, makeFluxTR) {
    EXPECT_EQ(0, theVars->FluxTR.size());
    RuACTVel ruv;
    PSVel psv;
    PRVel prv;
    SUCSVel suv;
    BFVel bfv;
    FIVel fiv;
    XanCycleVel xcv;
    RROEAVel rrv;
    theVars->RuACT_VEL.insert(0, 0., ruv);
    theVars->PS_VEL.insert(0, 0., psv);
    theVars->PR_VEL.insert(0, 0., prv);
    theVars->SUCS_VEL.insert(0, 0., suv);
    theVars->BF_VEL.insert(0, 0., bfv);
    theVars->FI_VEL.insert(0, 0., fiv);
    theVars->XanCycle_VEL.insert(0, 0., xcv);
    theVars->RROEA_VEL.insert(0, 0., rrv);
    makeFluxTR(theVars);
    EXPECT_NE(0, theVars->FluxTR.size());
}

class GeneralTest : public VariableFramework {

};

TEST_F(GeneralTest, SYSIniTest) {
    FIBF::setChlPSI(0.);
    FIBF::setChlT(0.);
    FIBF::setChlT2(0.);
    double chlt2 = FIBF::getChlT2();
    double chlt = FIBF::getChlT();
    double chlp = FIBF::getChlPSI();
    double ps12 = theVars->PS12ratio;

    SYSInitial(theVars);
    EXPECT_NE(chlt2, FIBF::getChlT2());
    EXPECT_NE(chlt, FIBF::getChlT());
    EXPECT_NE(chlp, FIBF::getChlPSI());
    EXPECT_NE(ps12, theVars->PS12ratio);
    FIBF::setChlPSI(0.);
    FIBF::setChlT(0.);
    FIBF::setChlT2(0.);
    theVars->PS12ratio = 0.;
}

TEST_F(GeneralTest, IniModelComTest) {
    theVars->PR_PS_com = true;
    theVars->XanCycle_BF_com = true;
    IniModelCom(theVars);
    EXPECT_FALSE(theVars->PR_PS_com);
    EXPECT_FALSE(theVars->XanCycle_BF_com);
}
}

