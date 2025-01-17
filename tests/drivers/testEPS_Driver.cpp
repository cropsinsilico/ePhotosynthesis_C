#include "VariableFramework.hpp"
#include "drivers/EPS_Driver.hpp"

using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::drivers;

namespace ePhotosynthesis {
namespace test {
class EPSDriverTest : public VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        theVars->EnzymeAct = Emap;
    }

    void TearDown() override {
        if (driver != nullptr)
            delete driver;
        driver = nullptr;
    }
    EPSCondition* ini() {
        return driver->EPS_Init();
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
    EPSDriver* driver;
};

namespace {
TEST_F(EPSDriverTest, IniTest) {
    driver = new EPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 15, 1.5, 27.3);
    EPSCondition* ec = ini();
    arr res = ec->toArray();
    arr comp = zeros(EPSCondition::size());

    DIFF(res, comp)
    delete ec;
}

TEST_F(EPSDriverTest, setupTest) {
    driver = new EPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 15, 1.5, 27.3);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());

}

TEST_F(EPSDriverTest, getResultsTest) {
    driver = new EPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 15, 1.5, 27.3);
    EXPECT_EQ(0, driver->constraints.size());
    driver->setup();
    EXPECT_NE(0, driver->constraints.size());
    setIntermediate(driver->constraints);
    driver->getResults();
    arr res = getResultsVal();
    EXPECT_NE(0, res.size());
}

TEST_F(EPSDriverTest, MBTest) {
    driver = new EPSDriver(theVars, 0., 1., 100., 2500, 1e-4, 1e-4, 15, 1.5, 27.3);
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
