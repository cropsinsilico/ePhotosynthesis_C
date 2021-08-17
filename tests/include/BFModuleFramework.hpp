#include "VariableFramework.hpp"
#include "modules/BF.hpp"


class BFModuleTest : public virtual VariableFramework {
protected:
    void SetUp() override {
        BF::setFI_connect(false);
        BF::setPS_connect(false);
        BF::setRROEA_connect(false);
        VariableFramework::SetUp();
        theVars->CO2_in = 500.;
        theVars->TestLi = 14.;
        theVars->TestATPCost = 4.5;
        theVars->EnzymeAct = Emap;
        std::vector<double> ratio(50, 1.5);
        theVars->BFRatio = ratio;
    }
    void Rate(const double t, const BFCondition* BF_con, Variables *theVars) {
        BF::_Rate(t, BF_con, theVars);
    }
    BFCondition* MB_con(const double t, const BFCondition* BF_con, Variables* theVars) {
        return BF::_MB_con(t, BF_con, theVars);
    }
    arr MB(const double t, const BFCondition* BF_con, Variables* theVars) {
        return BF::_MB(t, BF_con, theVars);
    }

};
