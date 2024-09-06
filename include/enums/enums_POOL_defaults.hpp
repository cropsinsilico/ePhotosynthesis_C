// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include <map>
#include "enums/enums_utils.hpp"


const std::map<const BF_POOL, const double>& BF_POOL_defaults();
template<> const std::map<const BF_POOL, const double>& get_enum_defaults<enum BF_POOL>() {
  return BF_POOL_defaults();
};

const std::map<const FI_POOL, const double>& FI_POOL_defaults();
template<> const std::map<const FI_POOL, const double>& get_enum_defaults<enum FI_POOL>() {
  return FI_POOL_defaults();
};

const std::map<const RROEA_POOL, const double>& RROEA_POOL_defaults();
template<> const std::map<const RROEA_POOL, const double>& get_enum_defaults<enum RROEA_POOL>() {
  return RROEA_POOL_defaults();
};

const std::map<const RuACT_POOL, const double>& RuACT_POOL_defaults();
template<> const std::map<const RuACT_POOL, const double>& get_enum_defaults<enum RuACT_POOL>() {
  return RuACT_POOL_defaults();
};

const std::map<const SUCS_POOL, const double>& SUCS_POOL_defaults();
template<> const std::map<const SUCS_POOL, const double>& get_enum_defaults<enum SUCS_POOL>() {
  return SUCS_POOL_defaults();
};

