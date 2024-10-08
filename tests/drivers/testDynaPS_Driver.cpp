#include "VariableFramework.hpp"
#include "drivers/DynaPS_Driver.hpp"
#include "modules/DynaPS.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

namespace ePhotosynthesis {
namespace test {
class DynaPSDriverTest : public VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        modules::DynaPS::reset();
    }
    void TearDown() override {
        if (driver != nullptr)
            delete driver;
        driver = nullptr;
    }
    DynaPSCondition* ini() {
        return driver->DynaPS_Ini();
    }
    arr MB(realtype t, N_Vector& u) {
        return driver->MB(t, u);
    }
    void setIntermediate(arr& data) {
        driver->intermediateRes = &data[0];
    }
    arr getResultsVal() {
        return driver->results;
    }
    DynaPSDriver* driver;
};

namespace {
TEST_F(DynaPSDriverTest, IniTest) {
    driver = new DynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 1, 1.2);
    DynaPSCondition* dc = ini();
    arr res = dc->toArray();
    arr comp = zeros(DynaPSCondition::size());

    DIFF(res, comp)
    delete dc;
}

TEST_F(DynaPSDriverTest, setupTest) {
    std::vector<size_t> paras = {100, 150, 175, 225, 250, 300, 307};
    for (auto i : paras) {
        driver = new DynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, i, 1.2);
        EXPECT_EQ(0, driver->constraints.size());
        driver->setup();
        EXPECT_NE(0, driver->constraints.size());
        delete driver;
        driver = nullptr;
    }
}

TEST_F(DynaPSDriverTest, getResultsTest) {
    driver = new DynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 1, 1.2);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    setIntermediate(driver->constraints);
    driver->getResults();
    arr res = getResultsVal();
    EXPECT_NE(0, res.size());
}

TEST_F(DynaPSDriverTest, MBTest) {
    driver = new DynaPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 1, 1.2);
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
}

}  // namespace test
}  // namespace ePhotosynthesis
