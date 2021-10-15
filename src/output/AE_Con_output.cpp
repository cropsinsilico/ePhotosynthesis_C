#include "conditions/AECondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& AECondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "AECondition" << std::endl;
    out << space << "  Mchl_ActATPsynthase = " << Mchl_ActATPsynthase << std::endl;
    out << space << "  Mchl_ActGAPDH = " << Mchl_ActGAPDH << std::endl;
    out << space << "  Mchl_ActNADPMDH = " << Mchl_ActNADPMDH << std::endl;
    out << space << "  Bchl_ActATPsynthase = " << Bchl_ActATPsynthase << std::endl;
    out << space << "  Bchl_ActPEPC = " << Bchl_ActPEPC << std::endl;
    out << space << "  Bchl_ActGAPDH = " << Bchl_ActGAPDH << std::endl;
    out << space << "  Bchl_ActFBPase = " << Bchl_ActFBPase << std::endl;
    out << space << "  Bchl_ActSBPase = " << Bchl_ActSBPase << std::endl;
    out << space << "  Bchl_ActPRK = " << Bchl_ActPRK << std::endl;
    out << space << "  Bchl_ActRubisco = " << Bchl_ActRubisco << std::endl;
    out << space << "  Bchl_ActRCA = " << Bchl_ActRCA << std::endl;
    return out;
}
