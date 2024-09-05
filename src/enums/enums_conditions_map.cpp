#include "enums/enums_conditions_map.hpp"

#include "enums/enums_utils.hpp"


const std::map<const PR_conditions, const std::string>& PR_conditions_map() {
  static const std::map<const PR_conditions, const std::string> map {
    {PR_GCEA , "GCEA" },
    {PR_GCA  , "GCA"  },
    {PR_PGCA , "PGCA" },
    {PR_GCAc , "GCAc" },
    {PR_GOAc , "GOAc" },
    {PR_SERc , "SERc" },
    {PR_GLYc , "GLYc" },
    {PR_HPRc , "HPRc" },
    {PR_GCEAc, "GCEAc"},
    {PR_RuBP , "RuBP" },
  };
  return map;
};
template<> const std::map<const PR_conditions, const std::string>& get_enum_map<enum PR_conditions>() {
  return PR_conditions_map();
};

const std::map<const PS_conditions, const std::string>& PS_conditions_map() {
  static const std::map<const PS_conditions, const std::string> map {
    {PS_RuBP, "RuBP"},
    {PS_PGA , "PGA" },
    {PS_DPGA, "DPGA"},
    {PS_T3P , "T3P" },
    {PS_ADPG, "ADPG"},
    {PS_FBP , "FBP" },
    {PS_E4P , "E4P" },
    {PS_S7P , "S7P" },
    {PS_SBP , "SBP" },
    {PS_ATP , "ATP" },
    {PS_HexP, "HexP"},
    {PS_PenP, "PenP"},
  };
  return map;
};
template<> const std::map<const PS_conditions, const std::string>& get_enum_map<enum PS_conditions>() {
  return PS_conditions_map();
};

const std::map<const RedoxReg_conditions, const std::string>& RedoxReg_conditions_map() {
  static const std::map<const RedoxReg_conditions, const std::string> map {
    {RedoxReg_ThioRatio, "ThioRatio"},
    {RedoxReg_FBPase   , "FBPase"   },
    {RedoxReg_SBPase   , "SBPase"   },
    {RedoxReg_PRK      , "PRK"      },
    {RedoxReg_ATPase   , "ATPase"   },
  };
  return map;
};
template<> const std::map<const RedoxReg_conditions, const std::string>& get_enum_map<enum RedoxReg_conditions>() {
  return RedoxReg_conditions_map();
};

const std::map<const FI_conditions, const std::string>& FI_conditions_map() {
  static const std::map<const FI_conditions, const std::string> map {
    {FI_A         , "A"         },
    {FI_U         , "U"         },
    {FI_P680ePheo , "P680ePheo" },
    {FI_P680pPheon, "P680pPheon"},
    {FI_P680pPheo , "P680pPheo" },
    {FI_P680Pheon , "P680Pheon" },
    {FI_Yz        , "Yz"        },
    {FI_S1T       , "S1T"       },
    {FI_S2T       , "S2T"       },
    {FI_S3T       , "S3T"       },
    {FI_S0T       , "S0T"       },
    {FI_S1Tp      , "S1Tp"      },
    {FI_S2Tp      , "S2Tp"      },
    {FI_S3Tp      , "S3Tp"      },
    {FI_S0Tp      , "S0Tp"      },
    {FI_QAQB      , "QAQB"      },
    {FI_QAnQB     , "QAnQB"     },
    {FI_QAQBn     , "QAQBn"     },
    {FI_QAnQBn    , "QAnQBn"    },
    {FI_QAQB2n    , "QAQB2n"    },
    {FI_QAnQB2n   , "QAnQB2n"   },
    {FI_PQn       , "PQn"       },
  };
  return map;
};
template<> const std::map<const FI_conditions, const std::string>& get_enum_map<enum FI_conditions>() {
  return FI_conditions_map();
};

const std::map<const RuACT_conditions, const std::string>& RuACT_conditions_map() {
  static const std::map<const RuACT_conditions, const std::string> map {
    {RuACT_ER  , "ER"  },
    {RuACT_Eaf , "Eaf" },
    {RuACT_ECMR, "ECMR"},
    {RuACT_RuBP, "RuBP"},
  };
  return map;
};
template<> const std::map<const RuACT_conditions, const std::string>& get_enum_map<enum RuACT_conditions>() {
  return RuACT_conditions_map();
};

const std::map<const XanCycle_conditions, const std::string>& XanCycle_conditions_map() {
  static const std::map<const XanCycle_conditions, const std::string> map {
    {XanCycle_Vx , "Vx" },
    {XanCycle_Ax , "Ax" },
    {XanCycle_Zx , "Zx" },
    {XanCycle_ABA, "ABA"},
  };
  return map;
};
template<> const std::map<const XanCycle_conditions, const std::string>& get_enum_map<enum XanCycle_conditions>() {
  return XanCycle_conditions_map();
};

const std::map<const SUCS_conditions, const std::string>& SUCS_conditions_map() {
  static const std::map<const SUCS_conditions, const std::string> map {
    {SUCS_T3Pc  , "T3Pc"  },
    {SUCS_FBPc  , "FBPc"  },
    {SUCS_HexPc , "HexPc" },
    {SUCS_F26BPc, "F26BPc"},
    {SUCS_UDPGc , "UDPGc" },
    {SUCS_SUCP  , "SUCP"  },
    {SUCS_SUC   , "SUC"   },
    {SUCS_PGAc  , "PGAc"  },
  };
  return map;
};
template<> const std::map<const SUCS_conditions, const std::string>& get_enum_map<enum SUCS_conditions>() {
  return SUCS_conditions_map();
};

const std::map<const BF_conditions, const std::string>& BF_conditions_map() {
  static const std::map<const BF_conditions, const std::string> map {
    {BF_ISPHr  , "ISPHr"  },
    {BF_cytc1  , "cytc1"  },
    {BF_ISPo   , "ISPo"   },
    {BF_ISPoQH2, "ISPoQH2"},
    {BF_QHsemi , "QHsemi" },
    {BF_cytbL  , "cytbL"  },
    {BF_Qi     , "Qi"     },
    {BF_Q      , "Q"      },
    {BF_cytbH  , "cytbH"  },
    {BF_Qn     , "Qn"     },
    {BF_Qr     , "Qr"     },
    {BF_QH2    , "QH2"    },
    {BF_cytc2  , "cytc2"  },
    {BF_P700   , "P700"   },
    {BF_ADP    , "ADP"    },
    {BF__Pi    , "_Pi"    },
    {BF_ATP    , "ATP"    },
    {BF_Ks     , "Ks"     },
    {BF_Mgs    , "Mgs"    },
    {BF_Cls    , "Cls"    },
    {BF_Aip    , "Aip"    },
    {BF_U      , "U"      },
    {BF_An     , "An"     },
    {BF_Fdn    , "Fdn"    },
    {BF_BFHs   , "BFHs"   },
    {BF_BFHl   , "BFHl"   },
    {BF_PHs    , "PHs"    },
    {BF_PHl    , "PHl"    },
    {BF_NADPH  , "NADPH"  },
  };
  return map;
};
template<> const std::map<const BF_conditions, const std::string>& get_enum_map<enum BF_conditions>() {
  return BF_conditions_map();
};

const std::map<const RROEA_conditions, const std::string>& RROEA_conditions_map() {
  static const std::map<const RROEA_conditions, const std::string> map {
    {RROEA_MDH  , "MDH"  },
    {RROEA_Thio , "Thio" },
    {RROEA_Fd   , "Fd"   },
    {RROEA_RuACT, "RuACT"},
  };
  return map;
};
template<> const std::map<const RROEA_conditions, const std::string>& get_enum_map<enum RROEA_conditions>() {
  return RROEA_conditions_map();
};

