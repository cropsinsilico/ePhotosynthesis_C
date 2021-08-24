#include "conditions/TemplateCondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& TemplateCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "TemplateCondition" << std::endl;
    out << space << "  Var1 = " << Var1 << std::endl;

    return out;
}
