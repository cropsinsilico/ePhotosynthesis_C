#include "VariableFramework.hpp"
#include "drivers/RAC4leafMeta_Driver.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

namespace ePhotosynthesis {
namespace test {
class RAC4leafMetaDriverTest : public VariableFramework {
protected:
    void TearDown() override {
        if (driver != nullptr)
            delete driver;
        driver = nullptr;
    }

    RAC4leafMetaCondition* ini() {
        return driver->RAC4leafMeta_Ini();
    }

    arr MB(realtype t, N_Vector u) {
        return driver->MB(t, u);
    }

    void setIntermediate(arr data) {
        driver->intermediateRes = &data[0];
    }
    arr getResultsVal() {
        return driver->results;
    }
    RAC4leafMetaDriver* driver;
};

namespace {
TEST_F(RAC4leafMetaDriverTest, IniTest) {
    RAC4leafMetaCondition::reset();
    driver = new RAC4leafMetaDriver(theVars, 0., 1., 5., 1500, 1e-6, 1e-5);
    RAC4leafMetaCondition* rac = ini();
    arr res = rac->toArray();
    arr comp = zeros(RAC4leafMetaCondition::size());

    DIFF(res, comp)
    delete rac;
}

TEST_F(RAC4leafMetaDriverTest, setupTest) {
    driver = new RAC4leafMetaDriver(theVars, 0., 1., 5., 1500, 1e-6, 1e-5);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
}

TEST_F(RAC4leafMetaDriverTest, getResultsTest) {
    driver = new RAC4leafMetaDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    setIntermediate(driver->constraints);
    driver->getResults();
    arr res = getResultsVal();
    EXPECT_NE(0, res.size());
}

TEST_F(RAC4leafMetaDriverTest, MBTest) {
    driver = new RAC4leafMetaDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    N_Vector y = N_VNew_Serial(driver->constraints.size());
    for (size_t i = 0; i < driver->constraints.size(); i++)
        NV_Ith_S(y, i) = driver->constraints[i];
    arr res = MB(1.5, y);
    EXPECT_NE(0, res.size());
    N_VDestroy(y);
}

}
}
}
