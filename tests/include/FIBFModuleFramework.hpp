#include "BFModuleFramework.hpp"
#include "FIModuleFramework.hpp"
#include "modules/FIBF.hpp"
#include "conditions/FIBFCondition.hpp"

namespace ePhotosynthesis {
namespace test {

class FIBFConditionTest : public virtual FIConditionTest, public virtual BFConditionTest {
protected:
    void SetUp() override {
	FIConditionTest::SetUp();
	BFConditionTest::SetUp();
	conditions::FIBFCondition::reset();
    }
};

class FIBFModuleTest : public virtual FIModuleTest, public virtual BFModuleTest {
protected:
    void SetUp() override {
        BFModuleTest::SetUp();
        FIModuleTest::SetUp();
        modules::FIBF::reset();
    }

    conditions::FIBFCondition* MB_con(const double t, const conditions::FIBFCondition* FIBF_con,
                                      Variables* theVars) {
        return modules::FIBF::_MB_con(t, FIBF_con, theVars);
    }
    arr MB(const double t, const conditions::FIBFCondition* FIBF_con, Variables* theVars) {
        return modules::FIBF::_MB(t, FIBF_con, theVars);
    }

};

}  // namespace test
}  // namspace ePhotosynthesis
