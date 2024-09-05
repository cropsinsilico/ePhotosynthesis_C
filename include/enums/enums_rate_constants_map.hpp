#pragma once

#include <map>
#include <string>

#include "enums/enums_rate_constants.hpp"


const std::map<const RuACT_rate_constants, const std::string>& RuACT_rate_constants_map();
const std::map<const RedoxReg_rate_constants, const std::string>& RedoxReg_rate_constants_map();
const std::map<const PR_rate_constants, const std::string>& PR_rate_constants_map();
const std::map<const BF_rate_constants, const std::string>& BF_rate_constants_map();
const std::map<const RROEA_rate_constants, const std::string>& RROEA_rate_constants_map();
const std::map<const SUCS_rate_constants, const std::string>& SUCS_rate_constants_map();
const std::map<const XanCycle_rate_constants, const std::string>& XanCycle_rate_constants_map();
const std::map<const FI_rate_constants, const std::string>& FI_rate_constants_map();
const std::map<const PS_rate_constants, const std::string>& PS_rate_constants_map();
