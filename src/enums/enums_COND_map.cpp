#include "enums/enums_COND_map.hpp"

#include "enums/enums_utils.hpp"


const std::map<const BF_COND, const std::string>& BF_COND_map() {
  static const std::map<const BF_COND, const std::string> map {
    {BF_COND_ISPHr  , "ISPHr"  },
    {BF_COND_cytc1  , "cytc1"  },
    {BF_COND_ISPo   , "ISPo"   },
    {BF_COND_ISPoQH2, "ISPoQH2"},
    {BF_COND_QHsemi , "QHsemi" },
    {BF_COND_cytbL  , "cytbL"  },
    {BF_COND_Qi     , "Qi"     },
    {BF_COND_Q      , "Q"      },
    {BF_COND_cytbH  , "cytbH"  },
    {BF_COND_Qn     , "Qn"     },
    {BF_COND_Qr     , "Qr"     },
    {BF_COND_QH2    , "QH2"    },
    {BF_COND_cytc2  , "cytc2"  },
    {BF_COND_P700   , "P700"   },
    {BF_COND_ADP    , "ADP"    },
    {BF_COND_ATP    , "ATP"    },
    {BF_COND_Ks     , "Ks"     },
    {BF_COND_Mgs    , "Mgs"    },
    {BF_COND_Cls    , "Cls"    },
    {BF_COND_Aip    , "Aip"    },
    {BF_COND_U      , "U"      },
    {BF_COND_An     , "An"     },
    {BF_COND_Fdn    , "Fdn"    },
    {BF_COND_BFHs   , "BFHs"   },
    {BF_COND_BFHl   , "BFHl"   },
    {BF_COND_PHs    , "PHs"    },
    {BF_COND_PHl    , "PHl"    },
    {BF_COND_NADPH  , "NADPH"  },
  };
  return map;
};
template<> const std::map<const BF_COND, const std::string>& get_enum_map<enum BF_COND>() {
  return BF_COND_map();
};
template<> MODULE get_enum_module<enum BF_COND>() {
  return MODULE_BF;
}

const std::map<const FI_COND, const std::string>& FI_COND_map() {
  static const std::map<const FI_COND, const std::string> map {
    {FI_COND_A         , "A"         },
    {FI_COND_U         , "U"         },
    {FI_COND_P680ePheo , "P680ePheo" },
    {FI_COND_P680pPheon, "P680pPheon"},
    {FI_COND_P680pPheo , "P680pPheo" },
    {FI_COND_P680Pheon , "P680Pheon" },
    {FI_COND_Yz        , "Yz"        },
    {FI_COND_S1T       , "S1T"       },
    {FI_COND_S2T       , "S2T"       },
    {FI_COND_S3T       , "S3T"       },
    {FI_COND_S0T       , "S0T"       },
    {FI_COND_S1Tp      , "S1Tp"      },
    {FI_COND_S2Tp      , "S2Tp"      },
    {FI_COND_S3Tp      , "S3Tp"      },
    {FI_COND_S0Tp      , "S0Tp"      },
    {FI_COND_QAQB      , "QAQB"      },
    {FI_COND_QAnQB     , "QAnQB"     },
    {FI_COND_QAQBn     , "QAQBn"     },
    {FI_COND_QAnQBn    , "QAnQBn"    },
    {FI_COND_QAQB2n    , "QAQB2n"    },
    {FI_COND_QAnQB2n   , "QAnQB2n"   },
    {FI_COND_PQn       , "PQn"       },
  };
  return map;
};
template<> const std::map<const FI_COND, const std::string>& get_enum_map<enum FI_COND>() {
  return FI_COND_map();
};
template<> MODULE get_enum_module<enum FI_COND>() {
  return MODULE_FI;
}

const std::map<const PR_COND, const std::string>& PR_COND_map() {
  static const std::map<const PR_COND, const std::string> map {
    {PR_COND_GCEA , "GCEA" },
    {PR_COND_GCA  , "GCA"  },
    {PR_COND_PGCA , "PGCA" },
    {PR_COND_GCAc , "GCAc" },
    {PR_COND_GOAc , "GOAc" },
    {PR_COND_SERc , "SERc" },
    {PR_COND_GLYc , "GLYc" },
    {PR_COND_HPRc , "HPRc" },
    {PR_COND_GCEAc, "GCEAc"},
    {PR_COND_RuBP , "RuBP" },
  };
  return map;
};
template<> const std::map<const PR_COND, const std::string>& get_enum_map<enum PR_COND>() {
  return PR_COND_map();
};
template<> MODULE get_enum_module<enum PR_COND>() {
  return MODULE_PR;
}

const std::map<const PS_COND, const std::string>& PS_COND_map() {
  static const std::map<const PS_COND, const std::string> map {
    {PS_COND_RuBP, "RuBP"},
    {PS_COND_PGA , "PGA" },
    {PS_COND_DPGA, "DPGA"},
    {PS_COND_T3P , "T3P" },
    {PS_COND_ADPG, "ADPG"},
    {PS_COND_FBP , "FBP" },
    {PS_COND_E4P , "E4P" },
    {PS_COND_S7P , "S7P" },
    {PS_COND_SBP , "SBP" },
    {PS_COND_ATP , "ATP" },
    {PS_COND_HexP, "HexP"},
    {PS_COND_PenP, "PenP"},
  };
  return map;
};
template<> const std::map<const PS_COND, const std::string>& get_enum_map<enum PS_COND>() {
  return PS_COND_map();
};
template<> MODULE get_enum_module<enum PS_COND>() {
  return MODULE_PS;
}

const std::map<const RROEA_COND, const std::string>& RROEA_COND_map() {
  static const std::map<const RROEA_COND, const std::string> map {
    {RROEA_COND_MDH  , "MDH"  },
    {RROEA_COND_Thio , "Thio" },
    {RROEA_COND_Fd   , "Fd"   },
    {RROEA_COND_RuACT, "RuACT"},
  };
  return map;
};
template<> const std::map<const RROEA_COND, const std::string>& get_enum_map<enum RROEA_COND>() {
  return RROEA_COND_map();
};
template<> MODULE get_enum_module<enum RROEA_COND>() {
  return MODULE_RROEA;
}

const std::map<const RedoxReg_COND, const std::string>& RedoxReg_COND_map() {
  static const std::map<const RedoxReg_COND, const std::string> map {
    {RedoxReg_COND_ThioRatio, "ThioRatio"},
    {RedoxReg_COND_FBPase   , "FBPase"   },
    {RedoxReg_COND_SBPase   , "SBPase"   },
    {RedoxReg_COND_PRK      , "PRK"      },
    {RedoxReg_COND_ATPase   , "ATPase"   },
  };
  return map;
};
template<> const std::map<const RedoxReg_COND, const std::string>& get_enum_map<enum RedoxReg_COND>() {
  return RedoxReg_COND_map();
};
template<> MODULE get_enum_module<enum RedoxReg_COND>() {
  return MODULE_RedoxReg;
}

const std::map<const RuACT_COND, const std::string>& RuACT_COND_map() {
  static const std::map<const RuACT_COND, const std::string> map {
    {RuACT_COND_ER  , "ER"  },
    {RuACT_COND_Eaf , "Eaf" },
    {RuACT_COND_ECMR, "ECMR"},
    {RuACT_COND_RuBP, "RuBP"},
  };
  return map;
};
template<> const std::map<const RuACT_COND, const std::string>& get_enum_map<enum RuACT_COND>() {
  return RuACT_COND_map();
};
template<> MODULE get_enum_module<enum RuACT_COND>() {
  return MODULE_RuACT;
}

const std::map<const SUCS_COND, const std::string>& SUCS_COND_map() {
  static const std::map<const SUCS_COND, const std::string> map {
    {SUCS_COND_T3Pc  , "T3Pc"  },
    {SUCS_COND_FBPc  , "FBPc"  },
    {SUCS_COND_HexPc , "HexPc" },
    {SUCS_COND_F26BPc, "F26BPc"},
    {SUCS_COND_UDPGc , "UDPGc" },
    {SUCS_COND_SUCP  , "SUCP"  },
    {SUCS_COND_SUC   , "SUC"   },
    {SUCS_COND_PGAc  , "PGAc"  },
  };
  return map;
};
template<> const std::map<const SUCS_COND, const std::string>& get_enum_map<enum SUCS_COND>() {
  return SUCS_COND_map();
};
template<> MODULE get_enum_module<enum SUCS_COND>() {
  return MODULE_SUCS;
}

const std::map<const XanCycle_COND, const std::string>& XanCycle_COND_map() {
  static const std::map<const XanCycle_COND, const std::string> map {
    {XanCycle_COND_Vx , "Vx" },
    {XanCycle_COND_Ax , "Ax" },
    {XanCycle_COND_Zx , "Zx" },
    {XanCycle_COND_ABA, "ABA"},
  };
  return map;
};
template<> const std::map<const XanCycle_COND, const std::string>& get_enum_map<enum XanCycle_COND>() {
  return XanCycle_COND_map();
};
template<> MODULE get_enum_module<enum XanCycle_COND>() {
  return MODULE_XanCycle;
}

