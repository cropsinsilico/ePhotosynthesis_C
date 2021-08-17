#include "VariableFramework.hpp"
#include "modules/XanCycle.hpp"

class XanCycleModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        XanCycle::setXanCycle2FIBF_Xstate(0.5);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(5, 1.5);
        theVars->XanRatio = ratio;
    }

    void Rate(const double t, const XanCycleCondition* XanCycle_con, Variables *theVars) {
        XanCycle::_Rate(t, XanCycle_con, theVars);
    }
    XanCycleCondition* MB_con(const double t, const XanCycleCondition* XanCycle_con, Variables* theVars) {
        return XanCycle::_MB_con(t, XanCycle_con, theVars);
    }
    arr MB(const double t, const XanCycleCondition* XanCycle_con, Variables* theVars) {
        return XanCycle::_MB(t, XanCycle_con, theVars);
    }

};
