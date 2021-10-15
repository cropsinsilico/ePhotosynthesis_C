#pragma once

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

#include "definitions.hpp"

namespace ePhotosynthesis {
namespace vel {

/**
 Class for holding the result of the BF_Rate calculations
 */
class AEVel {
public:
    AEVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFVel object to copy
      */
    AEVel(const AEVel &other) {
        vATPsynthase_Act_Mchl = other.vATPsynthase_Act_Mchl;
        vNADPMDH_Act = other.vNADPMDH_Act;
        vGAPDH_Act_Mchl = other.vGAPDH_Act_Mchl;
        vATPsynthase_Act_Bchl = other.vATPsynthase_Act_Bchl;
        vPEPC_Act = other.vPEPC_Act;
        ActRubisco0 = other.ActRubisco0;
        vRubisco_Act = other.vRubisco_Act;
        vGAPDH_Act_Bchl = other.vGAPDH_Act_Bchl;
        vFBPase_Act = other.vFBPase_Act;
        vSBPase_Act = other.vSBPase_Act;
        vPRK_Act = other.vPRK_Act;
        vRCA_Act = other.vRCA_Act;
    }

    double vATPsynthase_Act_Mchl = 0.;
    double vNADPMDH_Act = 0.;
    double vGAPDH_Act_Mchl = 0.;
    double vATPsynthase_Act_Bchl = 0.;
    double vPEPC_Act = 0.;
    double ActRubisco0 = 0.;
    double vRubisco_Act = 0.;
    double vGAPDH_Act_Bchl = 0.;
    double vFBPase_Act = 0.;
    double vSBPase_Act = 0.;
    double vPRK_Act = 0.;
    double vRCA_Act = 0.;

    friend std::ostream& operator<<(std::ostream& out, const AEVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
