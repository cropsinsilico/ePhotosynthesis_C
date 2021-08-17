#include "VariableFramework.hpp"
#include "modules/FI.hpp"


class FIModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        FI::setBF_connect(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->FIRatio = ratio;
    }

    void Rate(const double t, const FICondition* FI_con, Variables *theVars) {
        FI::_Rate(t, FI_con, theVars);
    }
    FICondition* MB_con(const double t, const FICondition* FI_con, Variables* theVars) {
        return FI::_MB_con(t, FI_con, theVars);
    }
    arr MB(const double t, const FICondition* FI_con, Variables* theVars) {
        return FI::_MB(t, FI_con, theVars);
    }

};
