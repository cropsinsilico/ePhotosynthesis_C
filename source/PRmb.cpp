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
\
#include "Variables.hpp"
#include "globals.hpp"

arr PR::PR_Mb(const double t, const PRCon &PR_con, Variables *theVars) {
    Condition(t, theVars);

    PR_Rate(t, PR_con, theVars);

    arr PRdydt = zeros(13);
    PRdydt[0] = theVars->PR_Vel.v1in - theVars->PR_Vel.v113;
    PRdydt[1] = theVars->PR_Vel.v112 - theVars->PR_Vel.v2out;
    PRdydt[2] = theVars->PR_Vel.v111 + theVars->PR_Vel.v113 - 0.5;
    PRdydt[2] = 0;
    PRdydt[3] = theVars->PR_Vel.v111 - theVars->PR_Vel.v112;
    PRdydt[4] = theVars->PR_Vel.v2out - theVars->PR_Vel.v121;
    PRdydt[5] = theVars->PR_Vel.v121 - theVars->PR_Vel.v122 - theVars->PR_Vel.v124;
    PRdydt[6] = theVars->PR_Vel.v131 - theVars->PR_Vel.v122;
    PRdydt[7] = theVars->PR_Vel.v122 + theVars->PR_Vel.v124 - 2 * theVars->PR_Vel.v131;
    PRdydt[8] = theVars->PR_Vel.v122 - theVars->PR_Vel.v123;
    PRdydt[9] = theVars->PR_Vel.v123 - theVars->PR_Vel.v1in;
    PRdydt[10] = 0.3 - theVars->PR_Vel.v111;
    PRdydt[11] = 0;
    PRdydt[12] = 0;

    return PRdydt;
}

std::ostream& operator<<(std::ostream &out, const PRCon &in) {
    out << "PRCon" << std::endl;
    out << "  GCEA= " << in.GCEA<< std::endl;
    out << "  GCA = " << in.GCA << std::endl;
    out << "  PGA = " << in.PGA << std::endl;
    out << "  PGCA= " << in.PGCA<< std::endl;
    out << "  GCAc= " << in.GCAc<< std::endl;
    out << "  GOAc= " << in.GOAc<< std::endl;
    out << "  SERc= " << in.SERc<< std::endl;
    out << "  GLYc= " << in.GLYc<< std::endl;
    out << "  HPRc= " << in.HPRc<< std::endl;
    out << "  GCEAc = " << in.GCEAc << std::endl;
    out << "  RUBP= " << in.RUBP<< std::endl;
    out << "  CO2 = " << in.CO2 << std::endl;
    out << "  O2= " << in.O2<< std::endl;

    return out;
}

std::ostream& operator<<(std::ostream &out, const PRVel &in) {
    out << "PRVel" << std::endl;
    out << "  v111 = " << in.v111 << std::endl;
    out << "  v112 = " << in.v112 << std::endl;
    out << "  v113 = " << in.v113 << std::endl;
    out << "  v121 = " << in.v121 << std::endl;
    out << "  v122 = " << in.v122 << std::endl;
    out << "  v123 = " << in.v123 << std::endl;
    out << "  v124 = " << in.v124 << std::endl;
    out << "  v131 = " << in.v131 << std::endl;
    out << "  v1in = " << in.v1in << std::endl;
    out << "  v2out= " << in.v2out<< std::endl;

    return out;
}
