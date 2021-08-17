#include "PS_PRModuleFramework.hpp"
#include "SUCSModuleFramework.hpp"
#include "modules/CM.hpp"

class CMModuleTest : public virtual PS_PRModuleTest, public virtual SUCSModuleTest {
protected:
    void SetUp() override {
        PS_PRModuleTest::SetUp();
        SUCSModuleTest::SetUp();
        CM::setTestSucPath(false);
    }

    CMCondition* MB_con(const double t, const CMCondition* CM_con, Variables* theVars) {
        return CM::_MB_con(t, CM_con, theVars);
    }
    arr MB(const double t, const CMCondition* CM_con, Variables* theVars) {
        return CM::_MB(t, CM_con, theVars);
    }

};
