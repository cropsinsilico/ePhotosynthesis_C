#include "Variables.hpp"
#include "modules/TemplateModule.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

TemplateCondition* TemplateModule::_MB_con(const double t, const TemplateCondition* const Temp_con, Variables* theVars) {
    // do any initial work here

    // call the rate calculations
    Rate(t, Temp_con, theVars);

    // Create the differential results
    TemplateCondition* dydt = new TemplateCondition();

    // calculate the differentials
#ifdef INCDEBUG
    DEBUG_INTERNAL(dydt)
#endif

    return dydt;
}

arr TemplateModule::_MB(const double t, const TemplateCondition* const Temp_con,
 Variables *theVars) {
    TemplateCondition* dydt = _MB_con(t, Temp_con, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}