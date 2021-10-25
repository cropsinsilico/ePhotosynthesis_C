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

#include "conditions/EnzymeCondition.hpp"
using namespace ePhotosynthesis::conditions;

std::ostream& EnzymeCondition::_print(std::ostream &out, const uint tab) const {
    const std::string space(tab * 4, ' ');
    out << space << "EnzymeCondition" << std::endl;
    out << space << "  MC_HCO3 = " << MC_HCO3 << std::endl;
    out << space << "  MC_OAA = " << MC_OAA << std::endl;
    out << space << "  MC_PEP = " << MC_PEP << std::endl;
    out << space << "  MC_Malate = " << MC_Malate << std::endl;
    out << space << "  MC_Pyruvate = " << MC_Pyruvate << std::endl;
    out << space << "  MC_PGA = " << MC_PGA << std::endl;
    out << space << "  MC_FBP = " << MC_FBP << std::endl;
    out << space << "  MC_UDPG = " << MC_UDPG << std::endl;
    out << space << "  MC_SUCP = " << MC_SUCP << std::endl;
    out << space << "  MC_SUC = " << MC_SUC << std::endl;
    out << space << "  MC_F26BP = " << MC_F26BP << std::endl;
    out << space << "  MC_ATP = " << MC_ATP << std::endl;
    out << space << "  MC_T3P = " << MC_T3P << std::endl;
    out << space << "  MC_HexP = " << MC_HexP << std::endl;
    out << space << "  MC_Sucrose = " << MC_Sucrose << std::endl;
    out << space << "  Mchl_OAA = " << Mchl_OAA << std::endl;
    out << space << "  Mchl_Malate = " << Mchl_Malate << std::endl;
    out << space << "  Mchl_PEP = " << Mchl_PEP << std::endl;
    out << space << "  Mchl_Pyruvate = " << Mchl_Pyruvate << std::endl;
    out << space << "  Mchl_NADPH = " << Mchl_NADPH << std::endl;
    out << space << "  Mchl_ATP = " << Mchl_ATP << std::endl;
    out << space << "  Mchl_PGA = " << Mchl_PGA << std::endl;
    out << space << "  Mchl_DPGA = " << Mchl_DPGA << std::endl;
    out << space << "  Mchl_T3P = " << Mchl_T3P << std::endl;
    out << space << "  BSC_T3P = " << BSC_T3P << std::endl;
    out << space << "  BSC_PGA = " << BSC_PGA << std::endl;
    out << space << "  BSC_Malate = " << BSC_Malate << std::endl;
    out << space << "  BSC_Pyruvate = " << BSC_Pyruvate << std::endl;
    out << space << "  BSC_CO2 = " << BSC_CO2 << std::endl;
    out << space << "  Bchl_CO2 = " << Bchl_CO2 << std::endl;
    out << space << "  Bchl_RuBP = " << Bchl_RuBP << std::endl;
    out << space << "  Bchl_PGA = " << Bchl_PGA << std::endl;
    out << space << "  Bchl_DPGA = " << Bchl_DPGA << std::endl;
    out << space << "  Bchl_ATP = " << Bchl_ATP << std::endl;
    out << space << "  Bchl_NADPH = " << Bchl_NADPH << std::endl;
    out << space << "  Bchl_SBP = " << Bchl_SBP << std::endl;
    out << space << "  Bchl_S7P = " << Bchl_S7P << std::endl;
    out << space << "  Bchl_FBP = " << Bchl_FBP << std::endl;
    out << space << "  Bchl_E4P = " << Bchl_E4P << std::endl;
    out << space << "  Bchl_Starch = " << Bchl_Starch << std::endl;
    out << space << "  Bchl_Rubisco = " << Bchl_Rubisco << std::endl;
    out << space << "  Bchl_T3P = " << Bchl_T3P << std::endl;
    out << space << "  Bchl_HexP = " << Bchl_HexP << std::endl;
    out << space << "  Bchl_Pent = " << Bchl_Pent << std::endl;
    out << space << "  Bchl_Malate = " << Bchl_Malate << std::endl;
    out << space << "  Bchl_Pyruvate = " << Bchl_Pyruvate << std::endl;
    out << space << "  Bchl_PGCA = " << Bchl_PGCA << std::endl;
    out << space << "  Bchl_GCA = " << Bchl_GCA << std::endl;
    out << space << "  Bchl_GCEA = " << Bchl_GCEA << std::endl;
    out << space << "  Bper_GCA = " << Bper_GCA << std::endl;
    out << space << "  Bper_GOA = " << Bper_GOA << std::endl;
    out << space << "  Bper_GLY = " << Bper_GLY << std::endl;
    out << space << "  Bper_SER = " << Bper_SER << std::endl;
    out << space << "  Bper_HPR = " << Bper_HPR << std::endl;
    out << space << "  Bper_GCEA = " << Bper_GCEA << std::endl;
    out << space << "  MC_CO2 = " << MC_CO2 << std::endl;
    out << space << "  Bchl_PPi = " << Bchl_PPi << std::endl;
    out << space << "  Bchl_ADPG = " << Bchl_ADPG << std::endl;

    out << space << "  MC_Glu = " << MC_Glu << std::endl;
    out << space << "  MC_OxoG = " << MC_OxoG << std::endl;
    out << space << "  MC_Asp = " << MC_Asp << std::endl;
    out << space << "  MC_Ala = " << MC_Ala << std::endl;

    out << space << "  BSC_OxoG = " << BSC_OxoG << std::endl;
    out << space << "  BSC_Glu = " << BSC_Glu << std::endl;
    out << space << "  BSC_Asp = " << BSC_Asp << std::endl;
    out << space << "  BSC_Ala = " << BSC_Ala << std::endl;
    out << space << "  BSC_OAA = " << BSC_OAA << std::endl;
    out << space << "  BSC_PEP = " << BSC_PEP << std::endl;
    out << space << "  BSC_ATP = " << BSC_ATP << std::endl;
    out << space << "  Bchl_OAA = " << Bchl_OAA << std::endl;
    out << space << "  MC_O2 = " << MC_O2 << std::endl;
    out << space << "  Mchl_O2 = " << Mchl_O2 << std::endl;
    out << space << "  BSC_O2 = " << BSC_O2 << std::endl;
    out << space << "  Bchl_O2 = " << Bchl_O2 << std::endl;
    out << space << "  Bchl_PEP = " << Bchl_PEP << std::endl;
    out << space << "  Mchl_GCEA = " << Mchl_GCEA << std::endl;
    out << space << "  Bmito_OAA = " << Bmito_OAA << std::endl;
    out << space << "  Bmito_MAL = " << Bmito_MAL << std::endl;
    out << space << "  Bmito_PYR = " << Bmito_PYR << std::endl;
    out << space << "  Bmito_CO2 = " << Bmito_CO2 << std::endl;
    out << space << "  Bmito_NADH = " << Bmito_NADH << std::endl;
    out << space << "  Bchl_Asp = " << Bchl_Asp << std::endl;
    out << space << "  Bchl_Ala = " << Bchl_Ala << std::endl;
    out << space << "  Mchl_Asp = " << Mchl_Asp << std::endl;
    out << space << "  Mchl_Ala = " << Mchl_Ala << std::endl;
    out << space << "  E_PPDK_Mchl = " << E_PPDK_Mchl << std::endl;
    out << space << "  EP_PPDK_Mchl = " << EP_PPDK_Mchl << std::endl;

    return out;
}
