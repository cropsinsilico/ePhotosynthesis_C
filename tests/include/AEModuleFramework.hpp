#include "VariableFramework.hpp"
#include "modules/AE_C4.hpp"

namespace ePhotosynthesis {
namespace test {

class AEModuleTest : public virtual VariableFramework {
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
    void Rate(const double t, const conditions::AECondition* AE_con, Variables *theVars) {
        modules::AE::_Rate(t, AE_con, theVars);
    }
    conditions::AECondition* MB_con(const double t, const conditions::AECondition* AE_con,
                                    Variables* theVars) {
        return modules::AE::_MB_con(t, AE_con, theVars);
    }
    arr MB(const double t, const conditions::AECondition* AE_con, Variables* theVars) {
        return modules::AE::_MB(t, AE_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
