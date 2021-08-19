#include "EPSModuleFramework.hpp"
#include "RuACTModuleFramework.hpp"
#include "modules/RA.hpp"

namespace ePhotosynthesis {
namespace test {

class RAModuleTest : public virtual EPSModuleTest, public virtual RuACTModuleTest {
protected:
    void SetUp() override {
        EPSModuleTest::SetUp();
        RuACTModuleTest::SetUp();
    }

    conditions::RACondition* MB_con(const double t, const conditions::RACondition* RA_con,
                                    Variables* theVars) {
        return modules::RA::_MB_con(t, RA_con, theVars);
    }
    arr MB(const double t, const conditions::RACondition* RA_con, Variables* theVars) {
        return modules::RA::_MB(t, RA_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
