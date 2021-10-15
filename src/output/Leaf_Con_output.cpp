#include "conditions/LeafCondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& LeafCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "LeafCondition" << std::endl;
    out << space << "  Ci = " << Ci << std::endl;
    out << space << "  Cb = " << Cb << std::endl;
    out << space << "  Eb = " << Eb << std::endl;
    out << space << "  Gs = " << Gs << std::endl;
    out << space << "  Tleaf = " << Tleaf << std::endl;
    out << space << "  H2Oou = " << H2Oou << std::endl;
    out << space << "  CO2in = " << CO2in << std::endl;

    return out;
}
