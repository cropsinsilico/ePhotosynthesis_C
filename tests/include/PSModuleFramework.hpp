#include "VariableFramework.hpp"
#include "modules/PS.hpp"


class PSModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        PS::setC3(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->PSRatio = ratio;
        theVars->VfactorCp = ratio;
        theVars->VfactorT = ratio;
    }

    void Rate(const double t, const PSCondition* PS_con, Variables *theVars) {
        PS::_Rate(t, PS_con, theVars);
    }
    PSCondition* MB_con(const double t, const PSCondition* PS_con, Variables* theVars) {
        return PS::_MB_con(t, PS_con, theVars);
    }
    arr MB(const double t, const PSCondition* PS_con, Variables* theVars) {
        return PS::_MB(t, PS_con, theVars);
    }

};
