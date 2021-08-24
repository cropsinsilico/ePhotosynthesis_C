#include "conditions/DependentCondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& DependentCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "DependentCondition" << std::endl;
    A_con->_print(out, tab + 1);
    B_con->_print(out, tab + 1);
    return out;
}
