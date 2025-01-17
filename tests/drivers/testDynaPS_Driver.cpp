#include "VariableFramework.hpp"
#include "drivers/DynaPS_Driver.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

namespace ePhotosynthesis {
namespace test {
class DynaPSDriverTest : public VariableFramework {
protected:
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
    // TODO: Check for older versions of Sundials for backwards compat
#ifdef SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(driver->constraints.size()), context);
#else // SUNDIALS_CONTEXT_REQUIRED
    N_Vector y = N_VNew_Serial(driver->constraints.size());
#endif // SUNDIALS_CONTEXT_REQUIRED
    sunrealtype* y_ptr = N_VGetArrayPointer(y);
    for (size_t i = 0; i < driver->constraints.size(); i++)
        y_ptr[i] = driver->constraints[i];
    // N_Vector y = N_VNew_Serial(driver->constraints.size(), context);
    // for (size_t i = 0; i < driver->constraints.size(); i++)
    //     NV_Ith_S(y, i) = driver->constraints[i];
    arr res = MB(1.5, y);
    EXPECT_NE(0, res.size());
    N_VDestroy(y);
}
}

}  // namespace test
}  // namespace ePhotosynthesis
