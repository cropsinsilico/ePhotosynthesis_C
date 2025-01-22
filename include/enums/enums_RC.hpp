// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include "enums/enums_helpers.hpp"

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type : int {
    MEMBERS_BFRC
  };
  typedef typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type ENUM_BF_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_BF, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_BF_RC& x);
  template<>
  struct enum_helper_inv<ENUM_BF_RC> {
    typedef ENUM_BF_RC type;
    static const MODULE module = MODULE_BF;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_BF, PARAM_TYPE_RC> BF;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type : int {
    MEMBERS_FIBFRC
  };
  typedef typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type ENUM_FIBF_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_FIBF_RC& x);
  template<>
  struct enum_helper_inv<ENUM_FIBF_RC> {
    typedef ENUM_FIBF_RC type;
    static const MODULE module = MODULE_FIBF;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_FIBF, PARAM_TYPE_RC> FIBF;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type : int {
    MEMBERS_FIRC
  };
  typedef typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type ENUM_FI_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_FI, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_FI_RC& x);
  template<>
  struct enum_helper_inv<ENUM_FI_RC> {
    typedef ENUM_FI_RC type;
    static const MODULE module = MODULE_FI;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_FI, PARAM_TYPE_RC> FI;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type : int {
    MEMBERS_PRRC
  };
  typedef typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type ENUM_PR_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_PR, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_PR_RC& x);
  template<>
  struct enum_helper_inv<ENUM_PR_RC> {
    typedef ENUM_PR_RC type;
    static const MODULE module = MODULE_PR;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_PR, PARAM_TYPE_RC> PR;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type : int {
    MEMBERS_PSRC
  };
  typedef typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type ENUM_PS_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_PS, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_PS_RC& x);
  template<>
  struct enum_helper_inv<ENUM_PS_RC> {
    typedef ENUM_PS_RC type;
    static const MODULE module = MODULE_PS;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_PS, PARAM_TYPE_RC> PS;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type : int {
    MEMBERS_RROEARC
  };
  typedef typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type ENUM_RROEA_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_RROEA_RC& x);
  template<>
  struct enum_helper_inv<ENUM_RROEA_RC> {
    typedef ENUM_RROEA_RC type;
    static const MODULE module = MODULE_RROEA;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_RROEA, PARAM_TYPE_RC> RROEA;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type : int {
    MEMBERS_RedoxRegRC
  };
  typedef typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type ENUM_RedoxReg_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_RedoxReg_RC& x);
  template<>
  struct enum_helper_inv<ENUM_RedoxReg_RC> {
    typedef ENUM_RedoxReg_RC type;
    static const MODULE module = MODULE_RedoxReg;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_RC> RedoxReg;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type : int {
    MEMBERS_RuACTRC
  };
  typedef typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type ENUM_RuACT_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_RuACT_RC& x);
  template<>
  struct enum_helper_inv<ENUM_RuACT_RC> {
    typedef ENUM_RuACT_RC type;
    static const MODULE module = MODULE_RuACT;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_RuACT, PARAM_TYPE_RC> RuACT;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type : int {
    MEMBERS_SUCSRC
  };
  typedef typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type ENUM_SUCS_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_SUCS_RC& x);
  template<>
  struct enum_helper_inv<ENUM_SUCS_RC> {
    typedef ENUM_SUCS_RC type;
    static const MODULE module = MODULE_SUCS;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_SUCS, PARAM_TYPE_RC> SUCS;
  }
}

namespace ePhotosynthesis {
  #ifndef EPHOTO_USE_SCOPED_ENUM
  template<>
  enum ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type : int {
    MEMBERS_XanCycleRC
  };
  typedef typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type ENUM_XanCycle_RC;
  #endif  // EPHOTO_USE_SCOPED_ENUM
  template<>
  const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::all;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::names;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::defaults;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::defaults_C3;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::glymaids;
  template<>
  const std::map<std::string, typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::aliases;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::docs;
  template<>
  std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::value_flags;
  template<>
  const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::Type, int> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::static_value_flags;
  template<>
  std::string ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC>::error_prefix();
  std::ostream& operator<<(std::ostream& out, const ENUM_XanCycle_RC& x);
  template<>
  struct enum_helper_inv<ENUM_XanCycle_RC> {
    typedef ENUM_XanCycle_RC type;
    static const MODULE module = MODULE_XanCycle;
    static const PARAM_TYPE param_type = PARAM_TYPE_RC;
  };
  namespace RC {
    typedef ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_RC> XanCycle;
  }
}


