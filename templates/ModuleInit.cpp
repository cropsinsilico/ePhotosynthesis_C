#include "modules/ModuleTemplate.hpp"

// update this with the number of data elements in the container
const size_t ModuleContainer::count = XX;

ModuleContainer* ModuleTemplate::_init(Variables *theVars) {
    ModuleContainer* ctnr = new ModuleContainer();
    // put any initialization code here
    return ctnr;
}