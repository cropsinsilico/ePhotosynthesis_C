#include "RAModuleFramework.hpp"
#include "XanCycleModuleFramework.hpp"
#include "modules/DynaPS.hpp"

namespace ePhotosynthesis {
namespace test {

class DynaPSModuleTest : public virtual RAModuleTest, public virtual XanCycleModuleTest {
protected:
    void SetUp() override {
        XanCycleModuleTest::SetUp();
        RAModuleTest::SetUp();
    }

    conditions::DynaPSCondition* MB_con(const double t,
                                        const conditions::DynaPSCondition* DynaPS_con,
                                        Variables* theVars) {
        return modules::DynaPS::_MB_con(t, DynaPS_con, theVars);
    }
    arr MB(const double t, const conditions::DynaPSCondition* DynaPS_con, Variables* theVars) {
        return modules::DynaPS::_MB(t, DynaPS_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
