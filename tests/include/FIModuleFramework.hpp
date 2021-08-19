#include "VariableFramework.hpp"
#include "modules/FI.hpp"

namespace ePhotosynthesis {
namespace test {

class FIModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::FI::setBF_connect(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->FIRatio = ratio;
    }

    void Rate(const double t, const conditions::FICondition* FI_con, Variables *theVars) {
        modules::FI::_Rate(t, FI_con, theVars);
    }
    conditions::FICondition* MB_con(const double t, const conditions::FICondition* FI_con,
                                    Variables* theVars) {
        return modules::FI::_MB_con(t, FI_con, theVars);
    }
    arr MB(const double t, const conditions::FICondition* FI_con, Variables* theVars) {
        return modules::FI::_MB(t, FI_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
