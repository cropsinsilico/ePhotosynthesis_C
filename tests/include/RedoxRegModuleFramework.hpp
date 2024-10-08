#include "VariableFramework.hpp"
#include "modules/RedoxReg.hpp"
#include "conditions/RedoxRegCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class RedoxRegConditionTest : public VariableFramework {
protected:
    void SetUp() override {
        conditions::RedoxRegCondition::reset();
        VariableFramework::SetUp();
    }
};

class RedoxRegModuleTest : public VariableFramework {
protected:
    void SetUp() override {
        modules::RedoxReg::reset();
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
    }

    void Rate(const double t, const conditions::RedoxRegCondition* RedoxReg_con,
              Variables *theVars) {
        modules::RedoxReg::_Rate(t, RedoxReg_con, theVars);
    }
    conditions::RedoxRegCondition* MB_con(const double t,
                                          const conditions::RedoxRegCondition* RedoxReg_con,
                                          Variables* theVars) {
        return modules::RedoxReg::_MB_con(t, RedoxReg_con, theVars);
    }
    arr MB(const double t, const conditions::RedoxRegCondition* RedoxReg_con, Variables* theVars) {
        return modules::RedoxReg::_MB(t, RedoxReg_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
