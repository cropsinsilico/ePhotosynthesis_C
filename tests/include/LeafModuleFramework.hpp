#include "VariableFramework.hpp"
#include "modules/Leaf_C4.hpp"

namespace ePhotosynthesis {
namespace test {

class LeafModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->Tp = 28.;
        theVars->BallBerryInterceptC4 = 2.5;
        theVars->WeatherRH = 45.;
        theVars->BallBerrySlopeC4 =1.2;
        theVars->TestATPCost = 4.5;
        std::vector<double> ratio(50, 1.5);
    }
    void Rate(const double t, const conditions::LeafCondition* Leaf_con, Variables *theVars) {
        modules::Leaf::_Rate(t, Leaf_con, theVars);
    }
    conditions::LeafCondition* MB_con(const double t, const conditions::LeafCondition* Leaf_con,
                                    Variables* theVars) {
        return modules::Leaf::_MB_con(t, Leaf_con, theVars);
    }
    arr MB(const double t, const conditions::LeafCondition* Leaf_con, Variables* theVars) {
        return modules::Leaf::_MB(t, Leaf_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
