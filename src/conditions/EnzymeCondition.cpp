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

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
const std::size_t EnzymeCondition::count = 87;

EnzymeCondition::EnzymeCondition(const EnzymeCondition* const other) {
    MC_HCO3 = other->MC_HCO3;
    MC_OAA = other->MC_OAA;
    MC_PEP = other->MC_PEP;
    MC_Malate = other->MC_Malate;
    MC_Pyruvate = other->MC_Pyruvate;
    MC_PGA = other->MC_PGA;
    MC_FBP = other->MC_FBP;
    MC_UDPG = other->MC_UDPG;
    MC_SUCP = other->MC_SUCP;
    MC_SUC = other->MC_SUC;
    MC_F26BP = other->MC_F26BP;
    MC_ATP = other->MC_ATP;
    MC_T3P = other->MC_T3P;
    MC_HexP = other->MC_HexP;
    MC_Sucrose = other->MC_Sucrose;
    Mchl_OAA = other->Mchl_OAA;
    Mchl_Malate = other->Mchl_Malate;
    Mchl_PEP = other->Mchl_PEP;
    Mchl_Pyruvate = other->Mchl_Pyruvate;
    Mchl_NADPH = other->Mchl_NADPH;
    Mchl_ATP = other->Mchl_ATP;
    Mchl_PGA = other->Mchl_PGA;
    Mchl_DPGA = other->Mchl_DPGA;
    Mchl_T3P = other->Mchl_T3P;
    BSC_T3P = other->BSC_T3P;
    BSC_PGA = other->BSC_PGA;
    BSC_Malate = other->BSC_Malate;
    BSC_Pyruvate = other->BSC_Pyruvate;
    BSC_CO2 = other->BSC_CO2;
    Bchl_CO2 = other->Bchl_CO2;
    Bchl_RuBP = other->Bchl_RuBP;
    Bchl_PGA = other->Bchl_PGA;
    Bchl_DPGA = other->Bchl_DPGA;
    Bchl_ATP = other->Bchl_ATP;
    Bchl_NADPH = other->Bchl_NADPH;
    Bchl_SBP = other->Bchl_SBP;
    Bchl_S7P = other->Bchl_S7P;
    Bchl_FBP = other->Bchl_FBP;
    Bchl_E4P = other->Bchl_E4P;
    Bchl_Starch = other->Bchl_Starch;
    Bchl_Rubisco = other->Bchl_Rubisco;
    Bchl_T3P = other->Bchl_T3P;
    Bchl_HexP = other->Bchl_HexP;
    Bchl_Pent = other->Bchl_Pent;
    Bchl_Malate = other->Bchl_Malate;
    Bchl_Pyruvate = other->Bchl_Pyruvate;
    Bchl_PGCA = other->Bchl_PGCA;
    Bchl_GCA = other->Bchl_GCA;
    Bchl_GCEA = other->Bchl_GCEA;
    Bper_GCA = other->Bper_GCA;
    Bper_GOA = other->Bper_GOA;
    Bper_GLY = other->Bper_GLY;
    Bper_SER = other->Bper_SER;
    Bper_HPR = other->Bper_HPR;
    Bper_GCEA = other->Bper_GCEA;
    MC_CO2 = other->MC_CO2;
    Bchl_PPi = other->Bchl_PPi;
    Bchl_ADPG = other->Bchl_ADPG;
    MC_Glu = other->MC_Glu;
    MC_OxoG = other->MC_OxoG;
    MC_Asp = other->MC_Asp;
    MC_Ala = other->MC_Ala;
    BSC_OxoG = other->BSC_OxoG;
    BSC_Glu = other->BSC_Glu;
    BSC_Asp = other->BSC_Asp;
    BSC_Ala = other->BSC_Ala;
    BSC_OAA = other->BSC_OAA;
    BSC_PEP = other->BSC_PEP;
    BSC_ATP = other->BSC_ATP;
    Bchl_OAA = other->Bchl_OAA;
    MC_O2 = other->MC_O2;
    Mchl_O2 = other->Mchl_O2;
    BSC_O2 = other->BSC_O2;
    Bchl_O2 = other->Bchl_O2;
    Bchl_PEP = other->Bchl_PEP;
    Mchl_GCEA = other->Mchl_GCEA;
    Bmito_OAA = other->Bmito_OAA;
    Bmito_MAL = other->Bmito_MAL;
    Bmito_PYR = other->Bmito_PYR;
    Bmito_CO2 = other->Bmito_CO2;
    Bmito_NADH = other->Bmito_NADH;
    Bchl_Asp = other->Bchl_Asp;
    Bchl_Ala = other->Bchl_Ala;
    Mchl_Asp = other->Mchl_Asp;
    Mchl_Ala = other->Mchl_Ala;
    E_PPDK_Mchl = other->E_PPDK_Mchl;
    EP_PPDK_Mchl = other->EP_PPDK_Mchl;
}

EnzymeCondition::EnzymeCondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}

void EnzymeCondition::_fromArray(const arr &vec, const std::size_t offset) {
    std::size_t counter = 0;
    MC_HCO3 = vec[offset + counter++];
    MC_OAA = vec[offset + counter++];
    MC_PEP = vec[offset + counter++];
    MC_Malate = vec[offset + counter++];
    MC_Pyruvate = vec[offset + counter++];
    MC_PGA = vec[offset + counter++];
    MC_FBP = vec[offset + counter++];
    MC_UDPG = vec[offset + counter++];
    MC_SUCP = vec[offset + counter++];
    MC_SUC = vec[offset + counter++];
    MC_F26BP = vec[offset + counter++];
    MC_ATP = vec[offset + counter++];
    MC_T3P = vec[offset + counter++];
    MC_HexP = vec[offset + counter++];
    MC_Sucrose = vec[offset + counter++];
    Mchl_OAA = vec[offset + counter++];
    Mchl_Malate = vec[offset + counter++];
    Mchl_PEP = vec[offset + counter++];
    Mchl_Pyruvate = vec[offset + counter++];
    Mchl_NADPH = vec[offset + counter++];
    Mchl_ATP = vec[offset + counter++];
    Mchl_PGA = vec[offset + counter++];
    Mchl_DPGA = vec[offset + counter++];
    Mchl_T3P = vec[offset + counter++];
    BSC_T3P = vec[offset + counter++];
    BSC_PGA = vec[offset + counter++];
    BSC_Malate = vec[offset + counter++];
    BSC_Pyruvate = vec[offset + counter++];
    BSC_CO2 = vec[offset + counter++];
    Bchl_CO2 = vec[offset + counter++];
    Bchl_RuBP = vec[offset + counter++];
    Bchl_PGA = vec[offset + counter++];
    Bchl_DPGA = vec[offset + counter++];
    Bchl_ATP = vec[offset + counter++];
    Bchl_NADPH = vec[offset + counter++];
    Bchl_SBP = vec[offset + counter++];
    Bchl_S7P = vec[offset + counter++];
    Bchl_FBP = vec[offset + counter++];
    Bchl_E4P = vec[offset + counter++];
    Bchl_Starch = vec[offset + counter++];
    Bchl_Rubisco = vec[offset + counter++];
    Bchl_T3P = vec[offset + counter++];
    Bchl_HexP = vec[offset + counter++];
    Bchl_Pent = vec[offset + counter++];
    Bchl_Malate = vec[offset + counter++];
    Bchl_Pyruvate = vec[offset + counter++];
    Bchl_PGCA = vec[offset + counter++];
    Bchl_GCA = vec[offset + counter++];
    Bchl_GCEA = vec[offset + counter++];
    Bper_GCA = vec[offset + counter++];
    Bper_GOA = vec[offset + counter++];
    Bper_GLY = vec[offset + counter++];
    Bper_SER = vec[offset + counter++];
    Bper_HPR = vec[offset + counter++];
    Bper_GCEA = vec[offset + counter++];
    MC_CO2 = vec[offset + counter++];
    Bchl_PPi = vec[offset + counter++];
    Bchl_ADPG = vec[offset + counter++];
    MC_Glu = vec[offset + counter++];
    MC_OxoG = vec[offset + counter++];
    MC_Asp = vec[offset + counter++];
    MC_Ala = vec[offset + counter++];
    BSC_OxoG = vec[offset + counter++];
    BSC_Glu = vec[offset + counter++];
    BSC_Asp = vec[offset + counter++];
    BSC_Ala = vec[offset + counter++];
    BSC_OAA = vec[offset + counter++];
    BSC_PEP = vec[offset + counter++];
    BSC_ATP = vec[offset + counter++];
    Bchl_OAA = vec[offset + counter++];
    MC_O2 = vec[offset + counter++];
    Mchl_O2 = vec[offset + counter++];
    BSC_O2 = vec[offset + counter++];
    Bchl_O2 = vec[offset + counter++];
    Bchl_PEP = vec[offset + counter++];
    Mchl_GCEA = vec[offset + counter++];
    Bmito_OAA = vec[offset + counter++];
    Bmito_MAL = vec[offset + counter++];
    Bmito_PYR = vec[offset + counter++];
    Bmito_CO2 = vec[offset + counter++];
    Bmito_NADH = vec[offset + counter++];
    Bchl_Asp = vec[offset + counter++];
    Bchl_Ala = vec[offset + counter++];
    Mchl_Asp = vec[offset + counter++];
    Mchl_Ala = vec[offset + counter++];
    E_PPDK_Mchl = vec[offset + counter++];
    EP_PPDK_Mchl = vec[offset + counter++];
}

arr EnzymeCondition::_toArray() const {
    return {MC_HCO3, MC_OAA, MC_PEP, MC_Malate, MC_Pyruvate,
                MC_PGA, MC_FBP, MC_UDPG, MC_SUCP, MC_SUC,
                MC_F26BP, MC_ATP, MC_T3P, MC_HexP, MC_Sucrose,
                Mchl_OAA, Mchl_Malate, Mchl_PEP, Mchl_Pyruvate,
                Mchl_NADPH, Mchl_ATP, Mchl_PGA, Mchl_DPGA,
                Mchl_T3P, BSC_T3P, BSC_PGA, BSC_Malate,
                BSC_Pyruvate, BSC_CO2, Bchl_CO2, Bchl_RuBP,
                Bchl_PGA, Bchl_DPGA, Bchl_ATP, Bchl_NADPH,
                Bchl_SBP, Bchl_S7P, Bchl_FBP, Bchl_E4P,
                Bchl_Starch, Bchl_Rubisco, Bchl_T3P, Bchl_HexP,
                Bchl_Pent, Bchl_Malate, Bchl_Pyruvate, Bchl_PGCA,
                Bchl_GCA, Bchl_GCEA, Bper_GCA, Bper_GOA, Bper_GLY,
                Bper_SER, Bper_HPR, Bper_GCEA, MC_CO2, Bchl_PPi,
                Bchl_ADPG, MC_Glu, MC_OxoG, MC_Asp, MC_Ala,
                BSC_OxoG, BSC_Glu, BSC_Asp, BSC_Ala, BSC_OAA,
                BSC_PEP, BSC_ATP, Bchl_OAA, MC_O2, Mchl_O2,
                BSC_O2, Bchl_O2, Bchl_PEP, Mchl_GCEA, Bmito_OAA,
                Bmito_MAL, Bmito_PYR, Bmito_CO2, Bmito_NADH,
                Bchl_Asp, Bchl_Ala, Mchl_Asp, Mchl_Ala, E_PPDK_Mchl,
                EP_PPDK_Mchl};
}
