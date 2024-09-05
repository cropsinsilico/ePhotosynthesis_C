#include "VariableFramework.hpp"
#include "drivers/trDynaPS_Driver.hpp"
#include "modules/PR.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::modules;

namespace ePhotosynthesis {
namespace test {
class trDynaPSDriverTest : public VariableFramework {
protected:
    void TearDown() override {
        if (driver != nullptr)
            delete driver;
        driver = nullptr;
    }
    trDynaPSCondition* ini() {
        return driver->trDynaPS_Ini();
    }
    arr MB(realtype t, N_Vector& u) {
        return driver->MB(t, u);
    }
    void setIntermediate(arr& data) {
        driver->intermediateRes = &data[0];
        //for (size_t i = 0; i < data.size(); i++)
        //driver->intermediateRes[i] = data[i];
        //driver->intermediateRes = N_VGetArrayPointer(nv);
    }
    arr getResultsVal() {
        return driver->results;
    }
    trDynaPSDriver* driver = nullptr;
};

namespace {
TEST_F(trDynaPSDriverTest, IniTest) {
    driver = new trDynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 18, 1.3);
    trDynaPSCondition* trc = ini();
    arr res = trc->toArray();
    arr comp = zeros(trDynaPSCondition::size());

    DIFF(res, comp)
    delete trc;
}

TEST_F(trDynaPSDriverTest, setupTest) {
    std::vector<size_t> paras = {100, 150, 175, 225, 250, 300, 307};
    for (auto i : paras) {
        driver = new trDynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, i, 1.3);
        EXPECT_EQ(0, driver->constraints.size());
        driver->setup();
        EXPECT_NE(0, driver->constraints.size());
    }
}

TEST_F(trDynaPSDriverTest, getResultsTest) {
    driver = new trDynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 18, 1.3);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    setIntermediate(driver->constraints);
    driver->getResults();
    arr res = getResultsVal();
    EXPECT_NE(0, res.size());
}

TEST_F(trDynaPSDriverTest, MBTest) {
    driver = new trDynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 18, 1.3);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(driver->constraints.size()), context);
    sunrealtype* y_ptr = N_VGetArrayPointer(y);
    for (size_t i = 0; i < driver->constraints.size(); i++)
      y_ptr[i] = driver->constraints[i];
    arr res = MB(1.5, y);
    EXPECT_NE(0, res.size());
    N_VDestroy(y);
}

TEST_F(trDynaPSDriverTest, FullTest) {
    theVars->CO2_in = 500.;
    theVars->TestLi = 1.4;
    theVars->TestATPCost = 4.5;
    theVars->EnzymeAct = Emap;
    theVars->RUBISCOMETHOD = 1;
    Variables* orig = new Variables(theVars);
    PR::setRUBISCOTOTAL(3);
    driver = new trDynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 1, 1);
    arr res;
    EXPECT_THROW(res = driver->run(), std::runtime_error);
    delete driver;
    orig->CO2_in = 400.;
    orig->TestLi = 800.;
    orig->TestATPCost = 0.;
    driver = new trDynaPSDriver(orig, 0., 1., 100., 2500, 1e-4, 1e-4, 1, 1);
    res = driver->run();
    arr comp = zeros(res.size());
    DIFF(comp, res)
}
}

}  // namespace test
}  // namespace ePhotosynthesis
