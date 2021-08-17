#include "VariableFramework.hpp"
#include "modules/SUCS.hpp"


class SUCSModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        SUCS::setSUCS2PS_Pic(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->VfactorCp = ratio;
        theVars->SUCRatio = ratio;
    }

    void Rate(const double t, const SUCSCondition* SUCS_con, Variables *theVars) {
        SUCS::_Rate(t, SUCS_con, theVars);
    }
    SUCSCondition* MB_con(const double t, const SUCSCondition* SUCS_con, Variables* theVars) {
        return SUCS::_MB_con(t, SUCS_con, theVars);
    }
    arr MB(const double t, const SUCSCondition* SUCS_con, Variables* theVars) {
        return SUCS::_MB(t, SUCS_con, theVars);
    }

};
