#include "PS_PRModuleFramework.hpp"
#include "SUCSModuleFramework.hpp"
#include "modules/CM.hpp"

namespace ePhotosynthesis {
namespace test {

class CMModuleTest : public virtual PS_PRModuleTest, public virtual SUCSModuleTest {
protected:
    void SetUp() override {
        PS_PRModuleTest::SetUp();
        SUCSModuleTest::SetUp();
        modules::CM::setTestSucPath(false);
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
