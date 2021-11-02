#include "VariableFramework.hpp"
#include "modules/Enzyme_C4.hpp"

namespace ePhotosynthesis {
namespace test {

class EnzymeModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->BFRatio = ratio;
    }
    void Rate(const double t, const conditions::EnzymeCondition* Enzyme_con, Variables *theVars) {
        modules::Enzyme::_Rate(t, Enzyme_con, theVars);
    }
    conditions::EnzymeCondition* MB_con(const double t, const conditions::EnzymeCondition* Enzyme_con,
                                    Variables* theVars) {
        return modules::Enzyme::_MB_con(t, Enzyme_con, theVars);
    }
    arr MB(const double t, const conditions::EnzymeCondition* Enzyme_con, Variables* theVars) {
        return modules::Enzyme::_MB(t, Enzyme_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
