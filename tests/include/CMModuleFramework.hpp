#include "PS_PRModuleFramework.hpp"
#include "SUCSModuleFramework.hpp"
#include "modules/CM.hpp"
#include "conditions/CMCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class CMConditionTest : public virtual PS_PRConditionTest, public virtual SUCSConditionTest {
protected:
    void SetUp() override {
	PS_PRConditionTest::SetUp();
	SUCSConditionTest::SetUp();
	conditions::CMCondition::reset();
    }
};

class CMModuleTest : public virtual PS_PRModuleTest, public virtual SUCSModuleTest {
protected:
    void SetUp() override {
        PS_PRModuleTest::SetUp();
        SUCSModuleTest::SetUp();
        modules::CM::reset();
    }

    conditions::CMCondition* MB_con(const double t, const conditions::CMCondition* CM_con,
                                    Variables* theVars) {
        return modules::CM::_MB_con(t, CM_con, theVars);
    }
    arr MB(const double t, const conditions::CMCondition* CM_con, Variables* theVars) {
        return modules::CM::_MB(t, CM_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
