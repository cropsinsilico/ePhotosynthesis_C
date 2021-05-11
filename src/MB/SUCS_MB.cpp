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

#include "globals.hpp"
#include "Variables.hpp"
#include "modules/SUCS.hpp"

arr SUCS::SUCS_Mb(const double t, const SUCSCon* SUCS_Con, Variables *theVars) {
    Condition(t, theVars);

    theVars->SUCS_Param[0] = theVars->GLight;

    //arr SUCS_Vel = zeros(15);
    SUCS_Rate(t, SUCS_Con, theVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation//
    ////////////////////////////////////////////////////////////////
    //std::cout << theVars->SUCS_Vel;
    // The Major Variables
    arr SUCS_mb = zeros(12);
    if (theVars->useC3) {
        SUCS_mb[0] = theVars->PS_Vel.v31 + theVars->PS_Vel.v33 - 2 * theVars->SUCS_Vel.v51; // T3Pc
    } else {
        SUCS_mb[0] = theVars->SUCS_Vel.vdhap_in + theVars->SUCS_Vel.vgap_in - 2 * theVars->SUCS_Vel.v51; // T3Pc
    }
    SUCS_mb[1] = theVars->SUCS_Vel.v51 - theVars->SUCS_Vel.v52; // FBPc
    SUCS_mb[2] = theVars->SUCS_Vel.v52 - theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v59 + theVars->SUCS_Vel.v58 - theVars->SUCS_Vel.v56;// HexPc
    SUCS_mb[3] = theVars->SUCS_Vel.v59 - theVars->SUCS_Vel.v58; // F26BPc
    SUCS_mb[4] = 0; //   vatpf - theVars->SUCS_Vel.v59 - v60;   // ATPc
    SUCS_mb[5] = 0; // ADPc
    SUCS_mb[7] = theVars->SUCS_Vel.v55 - theVars->SUCS_Vel.v56; // UDPGc
    SUCS_mb[8] = 0; //  v60 - v55  ;   // UTPc
    SUCS_mb[9] = theVars->SUCS_Vel.v56 - theVars->SUCS_Vel.v57; // SUCP
    SUCS_mb[10] = theVars->SUCS_Vel.v57 - theVars->SUCS_Vel.v62; // SUC
    if (theVars->useC3) {
        SUCS_mb[11] = theVars->PS_Vel.v32 - theVars->SUCS_Vel.vpga_use;
    } else {
        SUCS_mb[11] = theVars->SUCS_Vel.vpga_in - theVars->SUCS_Vel.vpga_use; // pgaC
    }
    return SUCS_mb;
}

std::ostream& operator<<(std::ostream &out, const SUCSCon &in) {
    out << "SUCSCon" << std::endl;
    out << "  T3Pc= " << in.T3Pc<< std::endl;
    out << "  FBPc= " << in.FBPc<< std::endl;
    out << "  HexPc = " << in.HexPc << std::endl;
    out << "  F26BPc= " << in.F26BPc<< std::endl;
    out << "  ATPc= " << in.ATPc<< std::endl;
    out << "  ADPc= " << in.ADPc<< std::endl;
    out << "  OPOPc = " << in.OPOPc << std::endl;
    out << "  UDPGc = " << in.UDPGc << std::endl;
    out << "  UTPc= " << in.UTPc<< std::endl;
    out << "  SUCP= " << in.SUCP<< std::endl;
    out << "  SUC = " << in.SUC << std::endl;
    out << "  PGAc= " << in.PGAc<< std::endl;

    return out;
}

std::ostream& operator<<(std::ostream &out, const SUCSVel &in) {
    out << "SUCSVel" << std::endl;
    out << "  v51= " << in.v51<< std::endl;
    out << "  v52= " << in.v52<< std::endl;
    out << "  v55= " << in.v55<< std::endl;
    out << "  v56= " << in.v56<< std::endl;
    out << "  v57= " << in.v57<< std::endl;
    out << "  v58= " << in.v58<< std::endl;
    out << "  v59= " << in.v59<< std::endl;
    out << "  v60= " << in.v60<< std::endl;
    out << "  v61= " << in.v61<< std::endl;
    out << "  v62= " << in.v62<< std::endl;
    out << "  vdhap_in = " << in.vdhap_in << std::endl;
    out << "  vgap_in =" << in.vgap_in<< std::endl;
    out << "  vpga_in =" << in.vpga_in<< std::endl;
    out << "  vpga_use = " << in.vpga_use << std::endl;
    out << "  vatpf= " << in.vatpf<< std::endl;
    return out;
}
