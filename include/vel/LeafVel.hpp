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
class LeafVel {
public:
    LeafVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFVel object to copy
      */
    LeafVel(const LeafVel &other) {
        NetAssimilation = other.NetAssimilation;
        vCO2b = other.vCO2b;
        vCO2s = other.vCO2s;
        vH2Ob = other.vH2Ob;
        vH2Os = other.vH2Os;
        EnergyBalanceResidual = other.EnergyBalanceResidual;
        vCO2total = other.vCO2total;
        vH2Ototal = other.vH2Ototal;
        vgs = other.vgs;
        vleak = other.vleak;
        Gs = other.Gs;
        Cb = other.Cb;
        Ci = other.Ci;
        Gbw = other.Gbw;
    }

    double NetAssimilation = 0.;
    double vCO2b = 0.;
    double vCO2s = 0.;
    double vH2Ob = 0.;
    double vH2Os = 0.;
    double EnergyBalanceResidual = 0.;
    double vCO2total = 0.;
    double vH2Ototal = 0.;
    double vgs = 0.;
    double vleak = 0.;
    double Gs = 0.;
    double Cb = 0.;
    double Ci = 0.;
    double Gbw = 0.;

    friend std::ostream& operator<<(std::ostream& out, const LeafVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis
