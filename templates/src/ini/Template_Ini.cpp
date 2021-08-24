#include "modules/TemplateModule.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

TemplateCondition* TemplateModule::_init(Variables *theVars) {
    TemplateCondition* ctnr = new TemplateCondition();
    // put any initialization code here
    return ctnr;
}