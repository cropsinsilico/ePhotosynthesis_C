#include "PSModuleFramework.hpp"
#include "PRModuleFramework.hpp"
#include "modules/PS_PR.hpp"

namespace ePhotosynthesis {
namespace test {

class PS_PRModuleTest : public virtual PSModuleTest, public virtual PRModuleTest {
protected:
    void SetUp() override {
        PSModuleTest::SetUp();
        PRModuleTest::SetUp();
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
