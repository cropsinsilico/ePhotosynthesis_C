#include "VariableFramework.hpp"
#include "modules/RuACT.hpp"
#include "conditions/RuACTCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class RuACTConditionTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        conditions::RuACTCondition::reset();
        VariableFramework::SetUp();
    }
};

class RuACTModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::RuACT::reset();
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->RuACTRatio = std::vector<double>(theVars->RuACTRatio.size(), 1.5);
    }

    void Rate(const double t, const conditions::RuACTCondition* RuACT_con, Variables *theVars) {
        modules::RuACT::_Rate(t, RuACT_con, theVars);
    }
    conditions::RuACTCondition* MB_con(const double t, const conditions::RuACTCondition* RuACT_con,
                                       Variables* theVars) {
        return modules::RuACT::_MB_con(t, RuACT_con, theVars);
    }
    arr MB(const double t, const conditions::RuACTCondition* RuACT_con, Variables* theVars) {
        return modules::RuACT::_MB(t, RuACT_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
