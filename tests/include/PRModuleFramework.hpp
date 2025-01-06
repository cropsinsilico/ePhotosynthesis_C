#include "VariableFramework.hpp"
#include "modules/PR.hpp"

namespace ePhotosynthesis {
namespace test {

class PRModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::PR::setPS_connect(false);
        modules::PR::setPS_RuBP(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->PRRatio = std::vector<double>(theVars->PRRatio.size(), 1.5);
    }

    void Rate(const double t, const conditions::PRCondition* PR_con, Variables *theVars) {
        modules::PR::_Rate(t, PR_con, theVars);
    }
    conditions::PRCondition* MB_con(const double t, const conditions::PRCondition* PR_con,
                                    Variables* theVars) {
        return modules::PR::_MB_con(t, PR_con, theVars);
    }
    arr MB(const double t, const conditions::PRCondition* PR_con, Variables* theVars) {
        return modules::PR::_MB(t, PR_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
