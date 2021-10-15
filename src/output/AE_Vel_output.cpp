#include "vel/AEVel.hpp"

namespace ePhotosynthesis {
namespace vel {
std::ostream& operator<<(std::ostream &out, const AEVel &in) {
    out << "AEVel" << std::endl;
    out << "  vATPsynthase_Act_Mchl = " << in.vATPsynthase_Act_Mchl << std::endl;
    out << "  vNADPMDH_Act = " << in.vNADPMDH_Act << std::endl;
    out << "  vGAPDH_Act_Mchl = " << in.vGAPDH_Act_Mchl << std::endl;
    out << "  vATPsynthase_Act_Bchl = " << in.vATPsynthase_Act_Bchl << std::endl;
    out << "  vPEPC_Act = " << in.vPEPC_Act << std::endl;
    out << "  ActRubisco0 = " << in.ActRubisco0 << std::endl;
    out << "  vRubisco_Act = " << in.vRubisco_Act << std::endl;
    out << "  vGAPDH_Act_Bchl = " << in.vGAPDH_Act_Bchl << std::endl;
    out << "  vFBPase_Act = " << in.vFBPase_Act << std::endl;
    out << "  vSBPase_Act = " << in.vSBPase_Act << std::endl;
    out << "  vPRK_Act = " << in.vPRK_Act << std::endl;
    out << "  vRCA_Act = " << in.vRCA_Act << std::endl;

    return out;
}
}
}
