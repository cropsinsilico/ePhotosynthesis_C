#include "FIBFModuleFramework.hpp"
#include "CMModuleFramework.hpp"
#include "modules/EPS.hpp"
#include "conditions/EPSCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class EPSConditionTest : public virtual FIBFConditionTest, public virtual CMConditionTest {
protected:
    void SetUp() override {
	CMConditionTest::SetUp();
	FIBFConditionTest::SetUp();
	conditions::EPSCondition::reset();
    }
};

class EPSModuleTest : public virtual FIBFModuleTest, public virtual CMModuleTest {
protected:
    void SetUp() override {
        CMModuleTest::SetUp();
        FIBFModuleTest::SetUp();
	modules::EPS::reset();
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
