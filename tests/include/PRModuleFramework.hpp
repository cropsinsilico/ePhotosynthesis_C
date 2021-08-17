#include "VariableFramework.hpp"
#include "modules/PR.hpp"


class PRModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        PR::setPS_connect(false);
        PR::setPS_RuBP(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->PRRatio = ratio;
    }

    void Rate(const double t, const PRCondition* PR_con, Variables *theVars) {
        PR::_Rate(t, PR_con, theVars);
    }
    PRCondition* MB_con(const double t, const PRCondition* PR_con, Variables* theVars) {
        return PR::_MB_con(t, PR_con, theVars);
    }
    arr MB(const double t, const PRCondition* PR_con, Variables* theVars) {
        return PR::_MB(t, PR_con, theVars);
    }

};
