//This code is generated by generate_enum.py and should not be 
//modified directly
#include "enums/enums_COND_defaults.hpp"

const std::map<const BF_COND, const double>& BF_COND_defaults() {
  static const std::map<const BF_COND, const double> map {
    {BF_COND_ISPHr  , 0.0    },
    {BF_COND_cytc1  , 1.0    },
    {BF_COND_ISPo   , 1.0    },
    {BF_COND_ISPoQH2, 0.0    },
    {BF_COND_QHsemi , 0.0    },
    {BF_COND_cytbL  , 1.0    },
    {BF_COND_Qi     , 0.0    },
    {BF_COND_Q      , 1.0    },
    {BF_COND_cytbH  , 1.0    },
    {BF_COND_Qn     , 0.0    },
    {BF_COND_Qr     , 0.0    },
    {BF_COND_QH2    , 5.0    },
    {BF_COND_cytc2  , 1.0    },
    {BF_COND_P700   , 0.5    },
    {BF_COND_ADP    , 0.82   },
    {BF_COND_ATP    , 0.68   },
    {BF_COND_Ks     , 10.0   },
    {BF_COND_Mgs    , 5.0    },
    {BF_COND_Cls    , 1.0    },
    {BF_COND_Aip    , 0.0    },
    {BF_COND_U      , 0.0    },
    {BF_COND_An     , 0.0    },
    {BF_COND_Fdn    , 0.3    },
    {BF_COND_BFHs   , 19.0001},
    {BF_COND_BFHl   , 19.0001},
    {BF_COND_PHs    , 7.0    },
    {BF_COND_PHl    , 7.0    },
    {BF_COND_NADPH  , 0.21   },
  };
  return map;
};

template<> const std::map<const BF_COND, const std::string>& get_enum_defaults<enum BF_COND>() {
  return BF_COND_defaults();
};

const std::map<const FI_COND, const double>& FI_COND_defaults() {
  static const std::map<const FI_COND, const double> map {
    {FI_COND_A         , 0  },
    {FI_COND_U         , 0  },
    {FI_COND_P680ePheo , 1  },
    {FI_COND_P680pPheon, 0  },
    {FI_COND_P680pPheo , 0  },
    {FI_COND_P680Pheon , 0  },
    {FI_COND_Yz        , 1  },
    {FI_COND_S1T       , 0.8},
    {FI_COND_S2T       , 0  },
    {FI_COND_S3T       , 0  },
    {FI_COND_S0T       , 0.2},
    {FI_COND_S1Tp      , 0  },
    {FI_COND_S2Tp      , 0  },
    {FI_COND_S3Tp      , 0  },
    {FI_COND_S0Tp      , 0  },
    {FI_COND_QAQB      , 1  },
    {FI_COND_QAnQB     , 0  },
    {FI_COND_QAQBn     , 0  },
    {FI_COND_QAnQBn    , 0  },
    {FI_COND_QAQB2n    , 0  },
    {FI_COND_QAnQB2n   , 0  },
    {FI_COND_PQn       , 5  },
  };
  return map;
};

template<> const std::map<const FI_COND, const std::string>& get_enum_defaults<enum FI_COND>() {
  return FI_COND_defaults();
};

const std::map<const PR_COND, const double>& PR_COND_defaults() {
  static const std::map<const PR_COND, const double> map {
    {PR_COND_GCEA , 0.1812},
    {PR_COND_GCA  , 0.36  },
    {PR_COND_PGCA , 0.0029},
    {PR_COND_GCAc , 0.36  },
    {PR_COND_GOAc , 0.028 },
    {PR_COND_SERc , 7.5   },
    {PR_COND_GLYc , 1.8   },
    {PR_COND_HPRc , 0.0035},
    {PR_COND_GCEAc, 0.1812},
    {PR_COND_RuBP , 2.0   },
  };
  return map;
};

template<> const std::map<const PR_COND, const std::string>& get_enum_defaults<enum PR_COND>() {
  return PR_COND_defaults();
};

const std::map<const PS_COND, const double>& PS_COND_defaults() {
  static const std::map<const PS_COND, const double> map {
    {PS_COND_RuBP, 2.000 },
    {PS_COND_PGA , 2.400 },
    {PS_COND_DPGA, 0.0011},
    {PS_COND_T3P , 0.5   },
    {PS_COND_ADPG, 0.005 },
    {PS_COND_FBP , 0.670 },
    {PS_COND_E4P , 0.050 },
    {PS_COND_S7P , 2.000 },
    {PS_COND_SBP , 0.300 },
    {PS_COND_ATP , 0.68  },
    {PS_COND_HexP, 2.2   },
    {PS_COND_PenP, 0.25  },
  };
  return map;
};

template<> const std::map<const PS_COND, const std::string>& get_enum_defaults<enum PS_COND>() {
  return PS_COND_defaults();
};

const std::map<const RROEA_COND, const double>& RROEA_COND_defaults() {
  static const std::map<const RROEA_COND, const double> map {
    {RROEA_COND_MDH  , 0.0   },
    {RROEA_COND_Thio , 0.081 },
    {RROEA_COND_Fd   , 0.081 },
    {RROEA_COND_RuACT, 0.0056},
  };
  return map;
};

template<> const std::map<const RROEA_COND, const std::string>& get_enum_defaults<enum RROEA_COND>() {
  return RROEA_COND_defaults();
};

const std::map<const RedoxReg_COND, const double>& RedoxReg_COND_defaults() {
  static const std::map<const RedoxReg_COND, const double> map {
    {RedoxReg_COND_ThioRatio, 1000},
    {RedoxReg_COND_FBPase   , 6   },
    {RedoxReg_COND_SBPase   , 9   },
    {RedoxReg_COND_PRK      , 13  },
    {RedoxReg_COND_ATPase   , 16  },
  };
  return map;
};

template<> const std::map<const RedoxReg_COND, const std::string>& get_enum_defaults<enum RedoxReg_COND>() {
  return RedoxReg_COND_defaults();
};

const std::map<const RuACT_COND, const double>& RuACT_COND_defaults() {
  static const std::map<const RuACT_COND, const double> map {
    {RuACT_COND_ER  , =},
    {RuACT_COND_Eaf , =},
    {RuACT_COND_ECMR, =},
    {RuACT_COND_RuBP, =},
  };
  return map;
};

template<> const std::map<const RuACT_COND, const std::string>& get_enum_defaults<enum RuACT_COND>() {
  return RuACT_COND_defaults();
};

const std::map<const SUCS_COND, const double>& SUCS_COND_defaults() {
  static const std::map<const SUCS_COND, const double> map {
    {SUCS_COND_T3Pc  , 2.0   },
    {SUCS_COND_FBPc  , 2.0   },
    {SUCS_COND_HexPc , 5.8   },
    {SUCS_COND_F26BPc, 7.8e-6},
    {SUCS_COND_UDPGc , 0.57  },
    {SUCS_COND_SUCP  , 0.0   },
    {SUCS_COND_SUC   , 0.0   },
    {SUCS_COND_PGAc  , 0.5   },
  };
  return map;
};

template<> const std::map<const SUCS_COND, const std::string>& get_enum_defaults<enum SUCS_COND>() {
  return SUCS_COND_defaults();
};

const std::map<const XanCycle_COND, const double>& XanCycle_COND_defaults() {
  static const std::map<const XanCycle_COND, const double> map {
    {XanCycle_COND_Vx , 160.0},
    {XanCycle_COND_Ax , 10.0 },
    {XanCycle_COND_Zx , 5.0  },
    {XanCycle_COND_ABA, 1.0  },
  };
  return map;
};

template<> const std::map<const XanCycle_COND, const std::string>& get_enum_defaults<enum XanCycle_COND>() {
  return XanCycle_COND_defaults();
};

