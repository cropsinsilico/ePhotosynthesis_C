#include "VariableFramework.hpp"
#include "modules/RROEA.hpp"

namespace ePhotosynthesis {
namespace test {

class RROEAModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
    }

    void Rate(const double t, const conditions::RROEACondition* RROEA_con, Variables *theVars) {
        modules::RROEA::_Rate(t, RROEA_con, theVars);
    }
    conditions::RROEACondition* MB_con(const double t, const conditions::RROEACondition* RROEA_con,
                                       Variables* theVars) {
        return modules::RROEA::_MB_con(t, RROEA_con, theVars);
    }
    arr MB(const double t, const conditions::RROEACondition* RROEA_con, Variables* theVars) {
        return modules::RROEA::_MB(t, RROEA_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
