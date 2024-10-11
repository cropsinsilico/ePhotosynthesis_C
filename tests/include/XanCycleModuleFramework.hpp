#pragma once

#include "VariableFramework.hpp"
#include "modules/XanCycle.hpp"
#include "conditions/XanCycleCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class XanCycleConditionTest : public virtual VariableFramework {
protected:
    void SetUp() override {
	conditions::XanCycleCondition::reset();
        VariableFramework::SetUp();
    }
};

class XanCycleModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        modules::XanCycle::reset();
        modules::XanCycle::setXanCycle2FIBF_Xstate(0.5);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        theVars->XanCycleRatio = std::vector<double>(theVars->XanCycleRatio.size(), 1.5);
    }

    void Rate(const double t, const conditions::XanCycleCondition* XanCycle_con,
              Variables *theVars) {
        modules::XanCycle::_Rate(t, XanCycle_con, theVars);
    }
    conditions::XanCycleCondition* MB_con(const double t,
                                          const conditions::XanCycleCondition* XanCycle_con,
                                          Variables* theVars) {
        return modules::XanCycle::_MB_con(t, XanCycle_con, theVars);
    }
    arr MB(const double t, const conditions::XanCycleCondition* XanCycle_con, Variables* theVars) {
        return modules::XanCycle::_MB(t, XanCycle_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
