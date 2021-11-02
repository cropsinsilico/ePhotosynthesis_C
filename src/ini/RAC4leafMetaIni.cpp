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
#include "modules/RAC4leafMeta.hpp"
#include "modules/Leaf_C4.hpp"
#include "modules/Enzyme_C4.hpp"
#include "modules/AE_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

RAC4leafMetaCondition* RAC4leafMeta::_init(Variables *theVars) {

    LeafCondition* Leaf_con = Leaf::_init(theVars);
    EnzymeCondition* Enzyme_con = Enzyme::_init(theVars);
    AECondition* AE_con = AE::_init(theVars);



    RAC4leafMetaCondition* RAC4LeafMeta_con = new RAC4leafMetaCondition(Leaf_con, Enzyme_con, AE_con);
    //arr RuACTInis = zeros(0);
    //RuACTInis= RuACT_Ini;
    //RAC4LeafMeta_con->print(std::cout);
    return RAC4LeafMeta_con;
}

void RAC4leafMeta::_reset() {
    Leaf::_reset();
    AE::_reset();
    Enzyme::_reset();
    conditions::RAC4leafMetaCondition::reset();
}
