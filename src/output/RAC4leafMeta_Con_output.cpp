#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis::conditions;

std::ostream& RAC4leafMetaCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "RAC4leafMetaConsition" << std::endl;
    Leaf_con->_print(out, tab + 1);
    Enzyme_con->_print(out, tab + 1);
    AE_con->_print(out, tab + 1);
    return out;
}
