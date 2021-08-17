#include "BFModuleFramework.hpp"
#include "FIModuleFramework.hpp"
#include "modules/FIBF.hpp"

class FIBFModuleTest : public virtual FIModuleTest, public virtual BFModuleTest {
protected:
    void SetUp() override {
        BFModuleTest::SetUp();
        FIModuleTest::SetUp();
    }

    FIBFCondition* MB_con(const double t, const FIBFCondition* FIBF_con, Variables* theVars) {
        return FIBF::_MB_con(t, FIBF_con, theVars);
    }
    arr MB(const double t, const FIBFCondition* FIBF_con, Variables* theVars) {
        return FIBF::_MB(t, FIBF_con, theVars);
    }

};
