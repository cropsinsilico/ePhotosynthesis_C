#include "VariableFramework.hpp"
#include "modules/RuACT.hpp"


class RuACTModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        RuACT::setEPS_connect(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->RacRatio = ratio;
    }

    void Rate(const double t, const RuACTCondition* RuACT_con, Variables *theVars) {
        RuACT::_Rate(t, RuACT_con, theVars);
    }
    RuACTCondition* MB_con(const double t, const RuACTCondition* RuACT_con, Variables* theVars) {
        return RuACT::_MB_con(t, RuACT_con, theVars);
    }
    arr MB(const double t, const RuACTCondition* RuACT_con, Variables* theVars) {
        return RuACT::_MB(t, RuACT_con, theVars);
    }

};
