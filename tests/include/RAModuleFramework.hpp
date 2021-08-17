#include "EPSModuleFramework.hpp"
#include "RuACTModuleFramework.hpp"
#include "modules/RA.hpp"

class RAModuleTest : public virtual EPSModuleTest, public virtual RuACTModuleTest {
protected:
    void SetUp() override {
        EPSModuleTest::SetUp();
        RuACTModuleTest::SetUp();
    }

    RACondition* MB_con(const double t, const RACondition* RA_con, Variables* theVars) {
        return RA::_MB_con(t, RA_con, theVars);
    }
    arr MB(const double t, const RACondition* RA_con, Variables* theVars) {
        return RA::_MB(t, RA_con, theVars);
    }

};
