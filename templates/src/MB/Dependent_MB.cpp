#include "Variables.hpp"
#include "modules/DependentModule.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

DependentCondition* DependentModule::_MB_con(const double t, const DependentCondition* const Dep_con, Variables* theVars) {
    ACondition* A_con = Dep_con->A_con;
    BCondition* B_con = Dep_con->B_con;
    // do any initial work here

    // call the differential calculator for the children
    ACondition* A_mb = AModule::MB_con(t, A_con, theVars);
    BCondition* B_mb = BModule::MB_con(t, B_con, theVars);

    // Create the differential results
    DependentCondition* dydt = new DependentCondition(A_mb, B_mb);

    // calculate the differentials
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif

    return dydt;
}

arr DependentModule::_MB(const double t, const DependentCondition* const Dep_con,
 Variables *theVars) {
    DependentCondition* dydt = _MB_con(t, Dep_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}