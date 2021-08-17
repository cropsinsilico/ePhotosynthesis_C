#include "RAModuleFramework.hpp"
#include "XanCycleModuleFramework.hpp"
#include "modules/DynaPS.hpp"

class DynaPSModuleTest : public virtual RAModuleTest, public virtual XanCycleModuleTest {
protected:
    void SetUp() override {
        XanCycleModuleTest::SetUp();
        RAModuleTest::SetUp();
    }

    DynaPSCondition* MB_con(const double t, const DynaPSCondition* DynaPS_con, Variables* theVars) {
        return DynaPS::_MB_con(t, DynaPS_con, theVars);
    }
    arr MB(const double t, const DynaPSCondition* DynaPS_con, Variables* theVars) {
        return DynaPS::_MB(t, DynaPS_con, theVars);
    }

};
