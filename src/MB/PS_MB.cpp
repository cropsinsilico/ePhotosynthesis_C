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

#include "Variables.hpp"
#include "globals.hpp"
#include "modules/PS.hpp"

arr PS::PS_Mb(const double t, const PSCon* PS_con, const arr &Param, Variables *theVars) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifying KM, KI, KE VMAX for different reactions as the regulation//
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Regulations first.
    Condition(t, theVars);

    // Get the rate for the reactions in the photosynthesis sytem
    PS_Rate(t, PS_con, Param, theVars);

    // Get the rate

    // Implement the mass balance equations

    arr PSdydt = zeros(15);
    PSdydt[0] = theVars->PS_Vel.v13 - theVars->PS_Vel.v1;
    PSdydt[1] = 2 * theVars->PS_Vel.v1 - theVars->PS_Vel.v2 - theVars->PS_Vel.v32;
    PSdydt[2] = theVars->PS_Vel.v2 - theVars->PS_Vel.v3;
    PSdydt[3] = theVars->PS_Vel.v3 - 2 * theVars->PS_Vel.v5 - theVars->PS_Vel.v7 - theVars->PS_Vel.v8 - theVars->PS_Vel.v10 - theVars->PS_Vel.v31 - theVars->PS_Vel.v33;
    PSdydt[4] = theVars->PS_Vel.v23 - theVars->PS_Vel.v24;
    PSdydt[5] = theVars->PS_Vel.v5 - theVars->PS_Vel.v6;
    PSdydt[6] = theVars->PS_Vel.v7 - theVars->PS_Vel.v8;
    PSdydt[7] = theVars->PS_Vel.v9 - theVars->PS_Vel.v10;
    PSdydt[8] = theVars->PS_Vel.v8 - theVars->PS_Vel.v9;
    PSdydt[9] = theVars->PS_Vel.v16 - theVars->PS_Vel.v2 - theVars->PS_Vel.v23 - theVars->PS_Vel.v13 - theVars->PS_Vel.v25;
    PSdydt[10] = 0;
    PSdydt[11] = 0;
    PSdydt[12] = 0;
    PSdydt[13] = theVars->PS_Vel.v6 - theVars->PS_Vel.v7 - theVars->PS_Vel.v23 + theVars->PS_Vel.v25;
    PSdydt[14] = theVars->PS_Vel.v7 + theVars->PS_Vel.v10 * 2 - theVars->PS_Vel.v13;

    return PSdydt;
}

std::ostream& operator<<(std::ostream &out, const PSCon &in) {
    out << "PSCon" << std::endl;
    out << "  RuBP= " << in.RuBP<< std::endl;
    out << "  PGA = " << in.PGA << std::endl;
    out << "  DPGA= " << in.DPGA<< std::endl;
    out << "  T3P = " << in.T3P << std::endl;
    out << "  ADPG= " << in.ADPG<< std::endl;
    out << "  FBP = " << in.FBP << std::endl;
    out << "  E4P = " << in.E4P << std::endl;
    out << "  S7P = " << in.S7P << std::endl;
    out << "  SBP = " << in.SBP << std::endl;
    out << "  ATP = " << in.ATP << std::endl;
    out << "  NADPH = " << in.NADPH << std::endl;
    out << "  CO2 = " << in.CO2 << std::endl;
    out << "  O2= " << in.O2<< std::endl;
    out << "  HexP= " << in.HexP<< std::endl;
    out << "  PenP= " << in.PenP<< std::endl;

    return out;
}

std::ostream& operator<<(std::ostream &out, const PSVel &in) {
    out << "PSVel" << std::endl;
    out << "  v1 = " << in.v1 << std::endl;
    out << "  v2 = " << in.v2 << std::endl;
    out << "  v3 = " << in.v3 << std::endl;
    out << "  v4 = " << in.v4 << std::endl;
    out << "  v5 = " << in.v5 << std::endl;
    out << "  v6 = " << in.v6 << std::endl;
    out << "  v7 = " << in.v7 << std::endl;
    out << "  v8 = " << in.v8 << std::endl;
    out << "  v9 = " << in.v9 << std::endl;
    out << "  v10= " << in.v10<< std::endl;
    out << "  v13= " << in.v13<< std::endl;
    out << "  v16= " << in.v16<< std::endl;
    out << "  v23= " << in.v23<< std::endl;
    out << "  v31= " << in.v31<< std::endl;
    out << "  v32= " << in.v32<< std::endl;
    out << "  v33= " << in.v33<< std::endl;
    out << "  Pi = " << in.Pi << std::endl;
    out << "  v24= " << in.v24<< std::endl;
    out << "  v25= " << in.v25<< std::endl;

    return out;
}
