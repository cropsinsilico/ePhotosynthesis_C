#include <math.h>
#include "globals.hpp"
#include "Variables.hpp"
#include "drivers/RAC4leafMeta_Driver.hpp"
#include "modules/RAC4leafMeta.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

RAC4leafMetaDriver::~RAC4leafMetaDriver() {
    RAC4leafMeta::reset();
}

void RAC4leafMetaDriver::setup() {
    //Enzyme activation settings
    inputVars->EAPPDK = true;////if inputVars->EAPPDK=0 PPDK is fully actived; ////if inputVars->EAPPDK=1 include PPDK activation by PDRP
    inputVars->GsResponse = true; ////if inputVars->GsResponse=0 Ball berry model no time dependent Gs response ; ////if inputVars->GsResponse=1 include Gs response, using inputVars->ki and kd

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inputVars->Para_mata = true; ////if inputVars->Para_mata=true, C4 Metabolic model and Gs model integrated  if inputVars->Para_mata=false Steady-state mdoel and gs model

    inputVars->Wind = 3.5;//m/s //Set as 3.5 to match the bundary layer conductance from licor data
    inputVars->Radiation_PARo = inputVars->TestLi;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inputVars->CI = inputVars->CO2_in * 0.4 / (3 * pow(10, 4));
    inputVars->O2_cond = inputVars->O2_cond * 1.26 / 1000;//inputVars->O2 concentration

    RAC4leafMetaCondition* condition = RAC4leafMeta_Ini();
    constraints = condition->toArray();
    delete condition;
}

void RAC4leafMetaDriver::getResults() {
    RAC4leafMetaCondition* condition = new RAC4leafMetaCondition(intermediateRes);
    arr temp = RAC4leafMeta::MB(time, condition, inputVars);
    results = zeros(1);
    results[0] = inputVars->Leaf_Vel.vCO2b;

    delete condition;
}

RAC4leafMetaCondition* RAC4leafMetaDriver::RAC4leafMeta_Ini() {
    return RAC4leafMeta::init(inputVars);
}

arr RAC4leafMetaDriver::MB(realtype t, N_Vector u) {
    realtype *x = N_VGetArrayPointer(u);

    RAC4leafMetaCondition* condition = new RAC4leafMetaCondition(x);
    arr dydt = RAC4leafMeta::MB(t, condition, inputVars);
    delete condition;
    return dydt;
}
