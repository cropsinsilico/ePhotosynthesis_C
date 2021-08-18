#include "FIBFModuleFramework.hpp"
#include "CMModuleFramework.hpp"
#include "modules/EPS.hpp"

namespace ePhotosynthesis {
namespace test {

class EPSModuleTest : public virtual FIBFModuleTest, public virtual CMModuleTest {
protected:
    void SetUp() override {
        CMModuleTest::SetUp();
        FIBFModuleTest::SetUp();
    }

    conditions::EPSCondition* MB_con(const double t, const conditions::EPSCondition* EPS_con,
                                     Variables* theVars) {
        return modules::EPS::_MB_con(t, EPS_con, theVars);
    }
    arr MB(const double t, const conditions::EPSCondition* EPS_con, Variables* theVars) {
        return modules::EPS::_MB(t, EPS_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
