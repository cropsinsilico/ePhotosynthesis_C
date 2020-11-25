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

#include "PS.hpp"
#include "PR.hpp"

class PS_PRCon {
public:
    PS_PRCon() {}
    PS_PRCon(const PS_PRCon &other) {
        PS_con = other.PS_con;
        PR_con = other.PR_con;
    }
    PS_PRCon(const PSCon &sother, const PRCon &rother) {
        PS_con = sother;
        PR_con = rother;
    }
    PS_PRCon(const std::vector<double> &vec, const size_t offset = 0){
        fromArray(vec, offset);
    }
    void fromArray(const std::vector<double> &vec, const size_t offset = 0) {
        PS_con.RuBP = vec[offset];
        PS_con.PGA = vec[offset + 1];
        PS_con.DPGA = vec[offset + 2];
        PS_con.T3P = vec[offset + 3];
        PS_con.FBP = vec[offset + 4];
        PS_con.E4P = vec[offset + 5];
        PS_con.S7P = vec[offset + 6];
        PS_con.SBP = vec[offset + 7];
        PS_con.ATP = vec[offset + 8];
        PS_con.NADPH = vec[offset + 9];
        PS_con.CO2 = vec[offset + 10];
        PS_con.O2 = vec[offset + 11];
        PS_con.HexP = vec[offset + 12];
        PS_con.PenP = vec[offset + 13];
        PR_con.GCEA = vec[offset + 14];
        PR_con.GCA = vec[offset + 15];
        PR_con.PGCA = vec[offset + 16];
        PR_con.GCAc = vec[offset + 17];
        PR_con.GOAc = vec[offset + 18];
        PR_con.SERc = vec[offset + 19];
        PR_con.GLYc = vec[offset + 20];
        PR_con.HPRc = vec[offset + 21];
        PR_con.GCEAc = vec[offset + 22];
        PS_con.ADPG = vec[offset + 23];
        PR_con.RUBP = PS_con.RuBP;
        PR_con.CO2 = PS_con.CO2;
        PR_con.O2 = PS_con.O2;
        PR_con.PGA = PS_con.PGA;
    }
    arr toArray() {
        arr outvec = {PS_con.RuBP,
                                      PS_con.PGA,
                                      PS_con.DPGA,
                                      PS_con.T3P,
                                      PS_con.FBP,
                                      PS_con.E4P,
                                      PS_con.S7P,
                                      PS_con.SBP,
                                      PS_con.ATP,
                                      PS_con.NADPH,
                                      PS_con.CO2,
                                      PS_con.O2,
                                      PS_con.HexP,
                                      PS_con.PenP,
                                      PR_con.GCEA,
                                      PR_con.GCA,
                                      PR_con.PGCA,
                                      PR_con.GCAc,
                                      PR_con.GOAc,
                                      PR_con.SERc,
                                      PR_con.GLYc,
                                      PR_con.HPRc,
                                      PR_con.GCEAc,
                                      PS_con.ADPG};

        return outvec;
    }

    size_t size() {
        return 24;//PS_con.size() + PR_con.size();
    }
    PSCon PS_con;
    PRCon PR_con;
};

PS_PRCon PS_PRIni(Variables *myVars);

arr PS_PRmb(double t, PS_PRCon &PS_PRs, Variables *myVars);
