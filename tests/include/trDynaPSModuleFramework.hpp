#include "DynaPSModuleFramework.hpp"
#include "RROEAModuleFramework.hpp"
#include "modules/trDynaPS.hpp"

class trDynaPSModuleTest : public virtual RROEAModuleTest, public virtual DynaPSModuleTest {
protected:
    void SetUp() override {
        DynaPSModuleTest::SetUp();
        RROEAModuleTest::SetUp();
    }

    trDynaPSCondition* MB_con(const double t, const trDynaPSCondition* trDynaPS_con, Variables* theVars) {
        return trDynaPS::_MB_con(t, trDynaPS_con, theVars);
    }
    arr MB(const double t, const trDynaPSCondition* trDynaPS_con, Variables* theVars) {
        return trDynaPS::_MB(t, trDynaPS_con, theVars);
    }

};
