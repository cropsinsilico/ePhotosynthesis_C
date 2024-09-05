#include "VariableFramework.hpp"
#include "modules/BF.hpp"

namespace ePhotosynthesis {
namespace test {

class BFModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::BF::setFI_connect(false);
        modules::BF::setPS_connect(false);
        modules::BF::setRROEA_connect(false);
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
