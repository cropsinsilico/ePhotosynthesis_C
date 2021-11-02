#include "VariableFramework.hpp"
#include "modules/RAC4leafMeta.hpp"

namespace ePhotosynthesis {
namespace test {

class RAC4leafMetaModuleTest : public virtual VariableFramework {
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
        theVars->O2_cond = 55.;
        theVars->CI = 100.;
        std::vector<double> ratio(50, 1.5);
    }
    void Rate(const double t, const conditions::RAC4leafMetaCondition* RAC4leafMeta_con, Variables *theVars) {
        modules::RAC4leafMeta::_Rate(t, RAC4leafMeta_con, theVars);
    }
    conditions::RAC4leafMetaCondition* MB_con(const double t, const conditions::RAC4leafMetaCondition* RAC4leafMeta_con,
                                    Variables* theVars) {
        return modules::RAC4leafMeta::_MB_con(t, RAC4leafMeta_con, theVars);
    }
    arr MB(const double t, const conditions::RAC4leafMetaCondition* RAC4leafMeta_con, Variables* theVars) {
        return modules::RAC4leafMeta::_MB(t, RAC4leafMeta_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
