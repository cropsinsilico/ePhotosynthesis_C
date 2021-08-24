#include "modules/DependentModule.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DependentCondition* DependentModule::_init(Variables *theVars) {
    ACondition* A_con = AModule::init(theVars);
    BCondition* B_con = BModule::init(theVars);
    DependentCondition* ctnr = new DependentCondition(A_con, B_con);
    // put any initialization code here
    return ctnr;
}

void DependentModule::_reset() {
    AModule::_reset();
    BModule::_reset();
    conditions::DependentCondition::reset();
}
