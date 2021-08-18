#include "BFModuleFramework.hpp"
#include "FIModuleFramework.hpp"
#include "modules/FIBF.hpp"

namespace ePhotosynthesis {
namespace test {

class FIBFModuleTest : public virtual FIModuleTest, public virtual BFModuleTest {
protected:
    void SetUp() override {
        BFModuleTest::SetUp();
        FIModuleTest::SetUp();
    }

    conditions::FIBFCondition* MB_con(const double t, const conditions::FIBFCondition* FIBF_con,
                                      Variables* theVars) {
        return modules::FIBF::_MB_con(t, FIBF_con, theVars);
    }
    arr MB(const double t, const conditions::FIBFCondition* FIBF_con, Variables* theVars) {
        return modules::FIBF::_MB(t, FIBF_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
