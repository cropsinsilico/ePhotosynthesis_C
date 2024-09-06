#pragma once

#include <map>
#include <string>

#include "enums/enums_COND.hpp"


const std::map<const BF_COND, const std::string>& BF_COND_map();
const std::map<const FI_COND, const std::string>& FI_COND_map();
const std::map<const PR_COND, const std::string>& PR_COND_map();
const std::map<const PS_COND, const std::string>& PS_COND_map();
const std::map<const RROEA_COND, const std::string>& RROEA_COND_map();
const std::map<const RedoxReg_COND, const std::string>& RedoxReg_COND_map();
const std::map<const RuACT_COND, const std::string>& RuACT_COND_map();
const std::map<const SUCS_COND, const std::string>& SUCS_COND_map();
const std::map<const XanCycle_COND, const std::string>& XanCycle_COND_map();
