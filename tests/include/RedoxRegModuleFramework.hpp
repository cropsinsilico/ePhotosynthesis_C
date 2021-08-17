#include "VariableFramework.hpp"
#include "modules/RedoxReg.hpp"


class RedoxRegModuleTest : public VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
    }

    void Rate(const double t, const RedoxRegCondition* RedoxReg_con, Variables *theVars) {
        RedoxReg::_Rate(t, RedoxReg_con, theVars);
    }
    RedoxRegCondition* MB_con(const double t, const RedoxRegCondition* RedoxReg_con, Variables* theVars) {
        return RedoxReg::_MB_con(t, RedoxReg_con, theVars);
    }
    arr MB(const double t, const RedoxRegCondition* RedoxReg_con, Variables* theVars) {
        return RedoxReg::_MB(t, RedoxReg_con, theVars);
    }

};
