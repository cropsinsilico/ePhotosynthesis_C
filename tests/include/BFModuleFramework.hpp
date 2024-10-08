#include "VariableFramework.hpp"
#include "modules/BF.hpp"
#include "conditions/BFCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class BFConditionTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        conditions::BFCondition::reset();
        VariableFramework::SetUp();
    }
};

class BFModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::BF::reset();
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->BFRatio = std::vector<double>(theVars->BFRatio.size(), 1.5);
    }
    void Rate(const double t, const conditions::BFCondition* BF_con, Variables *theVars) {
        modules::BF::_Rate(t, BF_con, theVars);
    }
    conditions::BFCondition* MB_con(const double t, const conditions::BFCondition* BF_con,
                                    Variables* theVars) {
        return modules::BF::_MB_con(t, BF_con, theVars);
    }
    arr MB(const double t, const conditions::BFCondition* BF_con, Variables* theVars) {
        return modules::BF::_MB(t, BF_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
