#include "VariableFramework.hpp"
#include "modules/PS.hpp"
#include "conditions/PSCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class PSConditionTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        conditions::PSCondition::reset();
        VariableFramework::SetUp();
    }
};

class PSModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::PS::reset();
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->PSRatio = std::vector<double>(theVars->PSRatio.size(), 1.5);
        theVars->VfactorCp = std::vector<double>(theVars->VfactorCp.size(), 1.5);
        theVars->VfactorT = std::vector<double>(theVars->VfactorT.size(), 1.5);
    }

    void Rate(const double t, const conditions::PSCondition* PS_con, Variables *theVars) {
        modules::PS::_Rate(t, PS_con, theVars);
    }
    conditions::PSCondition* MB_con(const double t, const conditions::PSCondition* PS_con,
                                    Variables* theVars) {
        return modules::PS::_MB_con(t, PS_con, theVars);
    }
    arr MB(const double t, const conditions::PSCondition* PS_con, Variables* theVars) {
        return modules::PS::_MB(t, PS_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
