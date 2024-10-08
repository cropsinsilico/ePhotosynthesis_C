#pragma once

#include "DynaPSModuleFramework.hpp"
#include "RROEAModuleFramework.hpp"
#include "modules/trDynaPS.hpp"
#include "conditions/trDynaPSCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::test;
using namespace ePhotosynthesis::conditions;
using namespace ePhotosynthesis::modules;

namespace ePhotosynthesis {
namespace test {

class trDynaPSConditionTest : public virtual RROEAConditionTest, public virtual DynaPSConditionTest {
protected:
    void SetUp() override {
        RROEAConditionTest::SetUp();
        DynaPSConditionTest::SetUp();
        conditions::trDynaPSCondition::reset();
    }
};

class trDynaPSModuleTest : public virtual RROEAModuleTest, public virtual DynaPSModuleTest {
protected:
    void SetUp() override {
        DynaPSModuleTest::SetUp();
        RROEAModuleTest::SetUp();
        modules::trDynaPS::reset();
    }

    conditions::trDynaPSCondition* MB_con(const double t,
                                          const conditions::trDynaPSCondition* trDynaPS_con,
                                          Variables* theVars) {
        return modules::trDynaPS::_MB_con(t, trDynaPS_con, theVars);
    }
    arr MB(const double t, const trDynaPSCondition* trDynaPS_con, Variables* theVars) {
        return trDynaPS::_MB(t, trDynaPS_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
