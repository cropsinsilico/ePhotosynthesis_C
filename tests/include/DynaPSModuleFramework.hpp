#include "RAModuleFramework.hpp"
#include "XanCycleModuleFramework.hpp"
#include "modules/DynaPS.hpp"
#include "conditions/DynaPSCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class DynaPSConditionTest : public virtual RAConditionTest, public virtual XanCycleConditionTest {
protected:
    void SetUp() override {
	RAConditionTest::SetUp();
	XanCycleConditionTest::SetUp();
	conditions::DynaPSCondition::reset();
    }
};

class DynaPSModuleTest : public virtual RAModuleTest, public virtual XanCycleModuleTest {
protected:
    void SetUp() override {
        XanCycleModuleTest::SetUp();
        RAModuleTest::SetUp();
	modules::DynaPS::reset();
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
