/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include "vel/AE_C4Vel.hpp"

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
