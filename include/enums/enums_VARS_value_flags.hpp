// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

namespace ePhotosynthesis {
  template<>
  inline std::map<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type, int>& get_enum_value_flags<typename ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::Type>() {
    return ValueSetEnum<MODULE_ALL, PARAM_TYPE_VARS>::value_flags;
  }
}
