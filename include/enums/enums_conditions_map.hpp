#pragma once

#include <map>
#include <string>

#include "enums/enums_conditions.hpp"


const std::map<const PR_conditions, const std::string>& PR_conditions_map();
const std::map<const PS_conditions, const std::string>& PS_conditions_map();
const std::map<const RedoxReg_conditions, const std::string>& RedoxReg_conditions_map();
const std::map<const FI_conditions, const std::string>& FI_conditions_map();
const std::map<const RuACT_conditions, const std::string>& RuACT_conditions_map();
const std::map<const XanCycle_conditions, const std::string>& XanCycle_conditions_map();
const std::map<const SUCS_conditions, const std::string>& SUCS_conditions_map();
const std::map<const BF_conditions, const std::string>& BF_conditions_map();
const std::map<const RROEA_conditions, const std::string>& RROEA_conditions_map();
