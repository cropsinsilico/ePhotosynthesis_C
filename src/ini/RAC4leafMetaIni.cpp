#include "Variables.hpp"
#include "modules/RAC4leafMeta.hpp"
#include "modules/Leaf.hpp"
#include "modules/Enzyme.hpp"
#include "modules/AE.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

RAC4leafMetaCondition* RAC4leafMeta::_init(Variables *theVars) {

    LeafCondition* Leaf_con = Leaf::_init(theVars);
    EnzymeCondition* Enzyme_con = Enzyme::_init(theVars);
    AECondition* AE_con = AE::_init(theVars);

    theVars->TempFactor = Leaf::TempResponseEnzymes(Leaf_con->Tleaf);

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
