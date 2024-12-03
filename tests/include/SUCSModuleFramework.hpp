#include "VariableFramework.hpp"
#include "modules/SUCS.hpp"

namespace ePhotosynthesis {
namespace test {

class SUCSModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::SUCS::setSUCS2PS_Pic(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->VfactorCp = std::vector<double>(theVars->VfactorCp.size(), 1.5);
        theVars->SUCRatio = std::vector<double>(theVars->SUCRatio.size(), 1.5);
    }

    void Rate(const double t, const conditions::SUCSCondition* SUCS_con, Variables *theVars) {
        modules::SUCS::_Rate(t, SUCS_con, theVars);
    }
    conditions::SUCSCondition* MB_con(const double t, const conditions::SUCSCondition* SUCS_con,
                                      Variables* theVars) {
        return modules::SUCS::_MB_con(t, SUCS_con, theVars);
    }
    arr MB(const double t, const conditions::SUCSCondition* SUCS_con, Variables* theVars) {
        return modules::SUCS::_MB(t, SUCS_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
