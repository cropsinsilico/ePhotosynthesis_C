#include "VariableFramework.hpp"
#include "drivers/CM_Driver.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

namespace ePhotosynthesis {
namespace test {
class CMDriverTest : public VariableFramework {
protected:
    void TearDown() override {
        if (driver != nullptr)
            delete driver;
        driver = nullptr;
    }
    CMCondition* ini() {
        return driver->CM_Ini();
    }
    arr MB(realtype t, N_Vector u) {
        return driver->MB(t, u);
    }
    void setIntermediate(arr data) {
        driver->intermediateRes = &data[0];
        //for (size_t i = 0; i < data.size(); i++)
        //driver->intermediateRes[i] = data[i];
        //driver->intermediateRes = N_VGetArrayPointer(nv);
    }
    arr getResultsVal() {
        return driver->results;
    }
    CMDriver* driver;
};

namespace {
TEST_F(CMDriverTest, IniTest) {
    driver = new CMDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    CMCondition* cmc = ini();
    arr res = cmc->toArray();
    arr comp = zeros(CMCondition::size());

    DIFF(res, comp)
    delete cmc;
}

TEST_F(CMDriverTest, setupTest) {
    driver = new CMDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());

}

TEST_F(CMDriverTest, getResultsTest) {
    driver = new CMDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    setIntermediate(driver->constraints);
    driver->getResults();
    arr res = getResultsVal();
    EXPECT_NE(0, res.size());
}

TEST_F(CMDriverTest, MBTest) {
    driver = new CMDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(driver->constraints.size()), context);
    for (size_t i = 0; i < driver->constraints.size(); i++)
        NV_Ith_S(y, i) = driver->constraints[i];
    arr res = MB(1.5, y);
    EXPECT_NE(0, res.size());
    N_VDestroy(y);
}
}

}  // namespace test
}  // namespace ePhotosynthesis
