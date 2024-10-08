#include "PSModuleFramework.hpp"
#include "PRModuleFramework.hpp"
#include "modules/PS_PR.hpp"
#include "conditions/PS_PRCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class PS_PRConditionTest : public virtual PSConditionTest, public virtual PRConditionTest {
protected:
    void SetUp() override {
        PRConditionTest::SetUp();
        PSConditionTest::SetUp();
        conditions::PS_PRCondition::reset();
    }
};

class PS_PRModuleTest : public virtual PSModuleTest, public virtual PRModuleTest {
protected:
    void SetUp() override {
        PSModuleTest::SetUp();
        PRModuleTest::SetUp();
        modules::PS_PR::reset();
    }

    conditions::PS_PRCondition* MB_con(const double t, const conditions::PS_PRCondition* PSPR_con,
                                       Variables* theVars) {
        return modules::PS_PR::_MB_con(t, PSPR_con, theVars);
    }
    arr MB(const double t, const conditions::PS_PRCondition* PSPR_con, Variables* theVars) {
        return modules::PS_PR::_MB(t, PSPR_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
