#include "vel/LeafVel.hpp"

namespace ePhotosynthesis {
namespace vel {
std::ostream& operator<<(std::ostream &out, const LeafVel &in) {
    out << "LeafVel" << std::endl;
    out << "  NetAssimilation = " << in.NetAssimilation << std::endl;
    out << "  vCO2b = " << in.vCO2b << std::endl;
    out << "  vCO2s = " << in.vCO2s << std::endl;
    out << "  vH2Ob = " << in.vH2Ob << std::endl;
    out << "  vH2Os = " << in.vH2Os << std::endl;
    out << "  EnergyBalanceResidual = " << in.EnergyBalanceResidual << std::endl;
    out << "  vCO2total = " << in.vCO2total << std::endl;
    out << "  vH2Ototal = " << in.vH2Ototal << std::endl;
    out << "  vgs = " << in.vgs << std::endl;
    out << "  vleak = " << in.vleak << std::endl;
    out << "  Gs = " << in.Gs << std::endl;
    out << "  Cb = " << in.Cb << std::endl;
    out << "  Ci = " << in.Ci << std::endl;
    out << "  Gbw = " << in.Gbw << std::endl;
    return out;
}

}
}
