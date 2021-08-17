#include "CMModuleFramework.hpp"

namespace {

TEST_F(CMModuleTest, InitTest) {
    CMCondition* cmc = CM::init(SUCSModuleTest::theVars);
    EXPECT_TRUE(cmc->PS_PR_con->PR_con->getPS_RuBP());
}

TEST_F(CMModuleTest, MBTest) {
    CM::setTestSucPath(true);
    Variables* orig = new Variables(SUCSModuleTest::theVars);
    CMCondition* cmc = CM::init(SUCSModuleTest::theVars);
    arr res = MB(1.5, cmc, SUCSModuleTest::theVars);
    arr comp = zeros(CMCondition::size());
    DIFF(res, comp)

    delete cmc;

    orig->useC3 = true;
    arr vfc(50, 1.5);
    arr vft(50, 2.1);
    orig->VfactorCp = vfc;
    orig->VfactorT = vft;
    orig->GRNC = 1;
    orig->GRNT = 1;
    orig->CO2_cond = 150.;
    orig->Tp = 27.2;

    cmc = CM::init(orig);
    arr res1 = MB(1.5, cmc, orig);
    comp = zeros(CMCondition::size());
    DIFF(comp, res1)
    EXPECT_NE(res.size(), res1.size());
    delete cmc;
    delete orig;
}

TEST_F(CMModuleTest, MBTestPRPS) {
    CM::setTestSucPath(true);
    SUCSModuleTest::theVars->PR_PS_com = true;
    Variables* orig = new Variables(SUCSModuleTest::theVars);
    CMCondition* cmc = CM::init(SUCSModuleTest::theVars);
    arr res = MB(1.5, cmc, SUCSModuleTest::theVars);
    arr comp = zeros(CMCondition::size());
    DIFF(res, comp)

    delete cmc;

    orig->useC3 = true;
    arr vfc(50, 1.5);
    arr vft(50, 2.1);
    orig->VfactorCp = vfc;
    orig->VfactorT = vft;
    orig->GRNC = 1;
    orig->GRNT = 1;
    orig->CO2_cond = 150.;
    orig->Tp = 27.2;

    cmc = CM::init(orig);
    arr res1 = MB(1.5, cmc, orig);
    comp = zeros(CMCondition::size());
    DIFF(comp, res1)
    EXPECT_NE(res.size(), res1.size());
    delete cmc;
    delete orig;
}

TEST_F(CMModuleTest, MBTestRUBISCO) {
    CM::setTestSucPath(true);
    SUCSModuleTest::theVars->RUBISCOMETHOD = 2;
    Variables* orig = new Variables(SUCSModuleTest::theVars);
    SUCSModuleTest::theVars->PSPR_SUCS_com = true;
    CMCondition* cmc = CM::init(SUCSModuleTest::theVars);
    arr res = MB(1.5, cmc, SUCSModuleTest::theVars);
    arr comp = zeros(CMCondition::size());
    DIFF(res, comp)

    delete cmc;

    orig->useC3 = true;
    arr vfc(50, 1.5);
    arr vft(50, 2.1);
    orig->VfactorCp = vfc;
    orig->VfactorT = vft;
    orig->GRNC = 1;
    orig->GRNT = 1;
    orig->CO2_cond = 150.;
    orig->Tp = 27.2;

    cmc = CM::init(orig);
    arr res1 = MB(1.5, cmc, orig);
    comp = zeros(CMCondition::size());
    DIFF(comp, res1)
    EXPECT_NE(res.size(), res1.size());
    delete cmc;
    delete orig;
}

}
