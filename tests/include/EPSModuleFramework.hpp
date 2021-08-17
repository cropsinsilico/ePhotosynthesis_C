#include "FIBFModuleFramework.hpp"
#include "CMModuleFramework.hpp"
#include "modules/EPS.hpp"

class EPSModuleTest : public virtual FIBFModuleTest, public virtual CMModuleTest {
protected:
    void SetUp() override {
        CMModuleTest::SetUp();
        FIBFModuleTest::SetUp();
    }

    EPSCondition* MB_con(const double t, const EPSCondition* EPS_con, Variables* theVars) {
        return EPS::_MB_con(t, EPS_con, theVars);
    }
    arr MB(const double t, const EPSCondition* EPS_con, Variables* theVars) {
        return EPS::_MB(t, EPS_con, theVars);
    }

};
