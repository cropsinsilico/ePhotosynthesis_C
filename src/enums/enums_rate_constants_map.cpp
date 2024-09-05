#include "enums/enums_rate_constants_map.hpp"

#include "enums/enums_utils.hpp"


const std::map<const RuACT_rate_constants, const std::string>& RuACT_rate_constants_map() {
  static const std::map<const RuACT_rate_constants, const std::string> map {
    {RuACT_k1 , "k1" },
    {RuACT_kn1, "kn1"},
    {RuACT_km1, "km1"},
    {RuACT_Ke2, "Ke2"},
    {RuACT_Ke3, "Ke3"},
    {RuACT_k6 , "k6" },
    {RuACT_kc , "kc" },
    {RuACT_ko , "ko" },
    {RuACT_kr , "kr" },
    {RuACT_ET , "ET" },
    {RuACT_Rac, "Rac"},
    {RuACT_C  , "C"  },
    {RuACT_O  , "O"  },
    {RuACT_M  , "M"  },
  };
  return map;
};
template<> const std::map<const RuACT_rate_constants, const std::string>& get_enum_map<enum RuACT_rate_constants>() {
  return RuACT_rate_constants_map();
};
template<> MODULE get_enum_module<enum RuACT_rate_constants>() {
  return MODULE_RuACT;
}

const std::map<const RedoxReg_rate_constants, const std::string>& RedoxReg_rate_constants_map() {
  static const std::map<const RedoxReg_rate_constants, const std::string> map {
    {RedoxReg_Fd_Thio_ET    , "Fd_Thio_ET"    },
    {RedoxReg_ThioT         , "ThioT"         },
    {RedoxReg_Thio_Oxidation, "Thio_Oxidation"},
    {RedoxReg_Thion0        , "Thion0"        },
  };
  return map;
};
template<> const std::map<const RedoxReg_rate_constants, const std::string>& get_enum_map<enum RedoxReg_rate_constants>() {
  return RedoxReg_rate_constants_map();
};
template<> MODULE get_enum_module<enum RedoxReg_rate_constants>() {
  return MODULE_RedoxReg;
}

const std::map<const PR_rate_constants, const std::string>& PR_rate_constants_map() {
  static const std::map<const PR_rate_constants, const std::string> map {
    {PR_V111      , "V111"      },
    {PR_V112      , "V112"      },
    {PR_V113      , "V113"      },
    {PR_V121      , "V121"      },
    {PR_V122      , "V122"      },
    {PR_V123      , "V123"      },
    {PR_V124      , "V124"      },
    {PR_V131      , "V131"      },
    {PR_NADHc     , "NADHc"     },
    {PR_NADc      , "NADc"      },
    {PR_GLUc      , "GLUc"      },
    {PR_KGc       , "KGc"       },
    {PR_ADP       , "ADP"       },
    {PR_ATP       , "ATP"       },
    {PR_PGA       , "PGA"       },
    {PR_KO        , "KO"        },
    {PR_KC        , "KC"        },
    {PR_KR        , "KR"        },
    {PR_KM112     , "KM112"     },
    {PR_KI1122    , "KI1122"    },
    {PR_KI1121    , "KI1121"    },
    {PR_KM1131    , "KM1131"    },
    {PR_KM1132    , "KM1132"    },
    {PR_KI113     , "KI113"     },
    {PR_KE113     , "KE113"     },
    {PR_KM121     , "KM121"     },
    {PR_KM1221    , "KM1221"    },
    {PR_KM1222    , "KM1222"    },
    {PR_KI1221    , "KI1221"    },
    {PR_KE122     , "KE122"     },
    {PR_KM123     , "KM123"     },
    {PR_KI123     , "KI123"     },
    {PR_KE123     , "KE123"     },
    {PR_KM1241    , "KM1241"    },
    {PR_KM1242    , "KM1242"    },
    {PR_KE124     , "KE124"     },
    {PR_KM1311    , "KM1311"    },
    {PR_KI1311    , "KI1311"    },
    {PR_V1T       , "V1T"       },
    {PR_KM1011    , "KM1011"    },
    {PR_KI1011    , "KI1011"    },
    {PR_V2T       , "V2T"       },
    {PR_KM1012    , "KM1012"    },
    {PR_KI1012    , "KI1012"    },
    {PR_Vfactor112, "Vfactor112"},
    {PR_Vfactor113, "Vfactor113"},
    {PR_Vfactor121, "Vfactor121"},
    {PR_Vfactor122, "Vfactor122"},
    {PR_Vfactor123, "Vfactor123"},
    {PR_Vfactor124, "Vfactor124"},
    {PR_Vfactor131, "Vfactor131"},
    {PR_Vf_T131   , "Vf_T131"   },
    {PR_Vf_T113   , "Vf_T113"   },
    {PR_Vf_T123   , "Vf_T123"   },
    {PR_Vf_T121   , "Vf_T121"   },
    {PR_Vf_T122   , "Vf_T122"   },
    {PR_Vf_T112   , "Vf_T112"   },
  };
  return map;
};
template<> const std::map<const PR_rate_constants, const std::string>& get_enum_map<enum PR_rate_constants>() {
  return PR_rate_constants_map();
};
template<> MODULE get_enum_module<enum PR_rate_constants>() {
  return MODULE_PR;
}

const std::map<const BF_rate_constants, const std::string>& BF_rate_constants_map() {
  static const std::map<const BF_rate_constants, const std::string> map {
    {BF_K1      , "K1"      },
    {BF_K2      , "K2"      },
    {BF_K3      , "K3"      },
    {BF_K4      , "K4"      },
    {BF_K5      , "K5"      },
    {BF_K6      , "K6"      },
    {BF_K7      , "K7"      },
    {BF_K8      , "K8"      },
    {BF_K9      , "K9"      },
    {BF_K10     , "K10"     },
    {BF_Vmax11  , "Vmax11"  },
    {BF_Kqi     , "Kqi"     },
    {BF_PK      , "PK"      },
    {BF_PMg     , "PMg"     },
    {BF_PCl     , "PCl"     },
    {BF_Kau     , "Kau"     },
    {BF_Kua     , "Kua"     },
    {BF_Kf      , "Kf"      },
    {BF_Kd      , "Kd"      },
    {BF_K15     , "K15"     },
    {BF_K16     , "K16"     },
    {BF_Em_ISP  , "Em_ISP"  },
    {BF_Em_Cytf , "Em_Cytf" },
    {BF_Em_PG   , "Em_PG"   },
    {BF_MemCap  , "MemCap"  },
    {BF_RVA     , "RVA"     },
    {BF_KBs     , "KBs"     },
    {BF_KBl     , "KBl"     },
    {BF_KM1ATP  , "KM1ATP"  },
    {BF_KM1ADP  , "KM1ADP"  },
    {BF_KM1PI   , "KM1PI"   },
    {BF_KM2NADP , "KM2NADP" },
    {BF_KM2NADPH, "KM2NADPH"},
    {BF_V2M     , "V2M"     },
    {BF_KE2     , "KE2"     },
    {BF_kA_d    , "kA_d"    },
    {BF_kA_f    , "kA_f"    },
    {BF_kA_U    , "kA_U"    },
    {BF_kU_A    , "kU_A"    },
    {BF_kU_d    , "kU_d"    },
    {BF_kU_f    , "kU_f"    },
    {BF_k1      , "k1"      },
    {BF_k_r1    , "k_r1"    },
    {BF_kz      , "kz"      },
    {BF_k12     , "k12"     },
    {BF_k23     , "k23"     },
    {BF_k30     , "k30"     },
  };
  return map;
};
template<> const std::map<const BF_rate_constants, const std::string>& get_enum_map<enum BF_rate_constants>() {
  return BF_rate_constants_map();
};
template<> MODULE get_enum_module<enum BF_rate_constants>() {
  return MODULE_BF;
}

const std::map<const RROEA_rate_constants, const std::string>& RROEA_rate_constants_map() {
  static const std::map<const RROEA_rate_constants, const std::string> map {
    {RROEA_ke2GAPDH   , "ke2GAPDH"   },
    {RROEA_ke2MDH     , "ke2MDH"     },
    {RROEA_ke2FBPase  , "ke2FBPase"  },
    {RROEA_ke2SBPase  , "ke2SBPase"  },
    {RROEA_ke2PRK     , "ke2PRK"     },
    {RROEA_ke2RubACT  , "ke2RubACT"  },
    {RROEA_ke2Fd      , "ke2Fd"      },
    {RROEA_keFd2Thio  , "keFd2Thio"  },
    {RROEA_keFd2Calvin, "keFd2Calvin"},
    {RROEA_ke2ATPGPP  , "ke2ATPGPP"  },
    {RROEA_KEe2FBPase , "KEe2FBPase" },
    {RROEA_KEe2SBPase , "KEe2SBPase" },
    {RROEA_KEe2PRK    , "KEe2PRK"    },
    {RROEA_KEe2ATPase , "KEe2ATPase" },
    {RROEA_KEe2RuACT  , "KEe2RuACT"  },
    {RROEA_KEe2GAPDH  , "KEe2GAPDH"  },
    {RROEA_KEe2MDH    , "KEe2MDH"    },
    {RROEA_KEe2ATPGPP , "KEe2ATPGPP" },
    {RROEA_KEeFd2Thio , "KEeFd2Thio" },
    {RROEA_SA_GAPDH   , "SA_GAPDH"   },
    {RROEA_SA_MDH     , "SA_MDH"     },
    {RROEA_SA_PRK     , "SA_PRK"     },
    {RROEA_SA_FBPase  , "SA_FBPase"  },
    {RROEA_SA_SBPase  , "SA_SBPase"  },
    {RROEA_SA_ATPGPP  , "SA_ATPGPP"  },
    {RROEA_SA_ATPase  , "SA_ATPase"  },
    {RROEA_mw_GAPDH   , "mw_GAPDH"   },
    {RROEA_mw_MDH     , "mw_MDH"     },
    {RROEA_mw_PRK     , "mw_PRK"     },
    {RROEA_mw_FBPase  , "mw_FBPase"  },
    {RROEA_mw_SBPase  , "mw_SBPase"  },
    {RROEA_mw_ATPGPP  , "mw_ATPGPP"  },
    {RROEA_mw_ATPase  , "mw_ATPase"  },
    {RROEA_FdT        , "FdT"        },
    {RROEA_ThioT      , "ThioT"      },
    {RROEA_RuACTT     , "RuACTT"     },
  };
  return map;
};
template<> const std::map<const RROEA_rate_constants, const std::string>& get_enum_map<enum RROEA_rate_constants>() {
  return RROEA_rate_constants_map();
};
template<> MODULE get_enum_module<enum RROEA_rate_constants>() {
  return MODULE_RROEA;
}

const std::map<const SUCS_rate_constants, const std::string>& SUCS_rate_constants_map() {
  static const std::map<const SUCS_rate_constants, const std::string> map {
    {SUCS_V51     , "V51"     },
    {SUCS_V52     , "V52"     },
    {SUCS_V55     , "V55"     },
    {SUCS_V56     , "V56"     },
    {SUCS_V57     , "V57"     },
    {SUCS_V58     , "V58"     },
    {SUCS_V59     , "V59"     },
    {SUCS_V62     , "V62"     },
    {SUCS_Vdhap_in, "Vdhap_in"},
    {SUCS_Vgap_in , "Vgap_in" },
    {SUCS_Vpga_in , "Vpga_in" },
    {SUCS_ATc     , "ATc"     },
    {SUCS_UTc     , "UTc"     },
    {SUCS_PTc     , "PTc"     },
    {SUCS_KE501   , "KE501"   },
    {SUCS_Km511   , "Km511"   },
    {SUCS_Km512   , "Km512"   },
    {SUCS_Km513   , "Km513"   },
    {SUCS_KE51    , "KE51"    },
    {SUCS_Km521   , "Km521"   },
    {SUCS_KI521   , "KI521"   },
    {SUCS_KI522   , "KI522"   },
    {SUCS_KI523   , "KI523"   },
    {SUCS_KE52    , "KE52"    },
    {SUCS_KE531   , "KE531"   },
    {SUCS_KE541   , "KE541"   },
    {SUCS_Km551   , "Km551"   },
    {SUCS_Km552   , "Km552"   },
    {SUCS_Km553   , "Km553"   },
    {SUCS_Km554   , "Km554"   },
    {SUCS_KE55    , "KE55"    },
    {SUCS_Km561   , "Km561"   },
    {SUCS_Km562   , "Km562"   },
    {SUCS_KI561   , "KI561"   },
    {SUCS_KI562   , "KI562"   },
    {SUCS_KI563   , "KI563"   },
    {SUCS_KI564   , "KI564"   },
    {SUCS_KI565   , "KI565"   },
    {SUCS_KE56    , "KE56"    },
    {SUCS_Km571   , "Km571"   },
    {SUCS_Ki572   , "Ki572"   },
    {SUCS_KE57    , "KE57"    },
    {SUCS_Km581   , "Km581"   },
    {SUCS_KI581   , "KI581"   },
    {SUCS_KI582   , "KI582"   },
    {SUCS_Km591   , "Km591"   },
    {SUCS_Km593   , "Km593"   },
    {SUCS_KI591   , "KI591"   },
    {SUCS_KE59    , "KE59"    },
    {SUCS_KE61    , "KE61"    },
    {SUCS_Km621   , "Km621"   },
  };
  return map;
};
template<> const std::map<const SUCS_rate_constants, const std::string>& get_enum_map<enum SUCS_rate_constants>() {
  return SUCS_rate_constants_map();
};
template<> MODULE get_enum_module<enum SUCS_rate_constants>() {
  return MODULE_SUCS;
}

const std::map<const XanCycle_rate_constants, const std::string>& XanCycle_rate_constants_map() {
  static const std::map<const XanCycle_rate_constants, const std::string> map {
    {XanCycle_kva, "kva"},
    {XanCycle_kaz, "kaz"},
    {XanCycle_kza, "kza"},
    {XanCycle_kav, "kav"},
  };
  return map;
};
template<> const std::map<const XanCycle_rate_constants, const std::string>& get_enum_map<enum XanCycle_rate_constants>() {
  return XanCycle_rate_constants_map();
};
template<> MODULE get_enum_module<enum XanCycle_rate_constants>() {
  return MODULE_XanCycle;
}

const std::map<const FI_rate_constants, const std::string>& FI_rate_constants_map() {
  static const std::map<const FI_rate_constants, const std::string> map {
    {FI_kA_d    , "kA_d"    },
    {FI_kA_f    , "kA_f"    },
    {FI_kA_U    , "kA_U"    },
    {FI_kU_A    , "kU_A"    },
    {FI_kU_d    , "kU_d"    },
    {FI_kU_f    , "kU_f"    },
    {FI_k1      , "k1"      },
    {FI_k_r1    , "k_r1"    },
    {FI_kz      , "kz"      },
    {FI_k12     , "k12"     },
    {FI_k23     , "k23"     },
    {FI_k30     , "k30"     },
    {FI_k01     , "k01"     },
    {FI_k2      , "k2"      },
    {FI_kAB1    , "kAB1"    },
    {FI_kBA1    , "kBA1"    },
    {FI_kAB2    , "kAB2"    },
    {FI_kBA2    , "kBA2"    },
    {FI_k3      , "k3"      },
    {FI_k_r3    , "k_r3"    },
    {FI_k_pq_oxy, "k_pq_oxy"},
    {FI_QBt     , "QBt"     },
    {FI_PQT     , "PQT"     },
  };
  return map;
};
template<> const std::map<const FI_rate_constants, const std::string>& get_enum_map<enum FI_rate_constants>() {
  return FI_rate_constants_map();
};
template<> MODULE get_enum_module<enum FI_rate_constants>() {
  return MODULE_FI;
}

const std::map<const PS_rate_constants, const std::string>& PS_rate_constants_map() {
  static const std::map<const PS_rate_constants, const std::string> map {
    {PS_CP       , "CP"       },
    {PS_CA       , "CA"       },
    {PS_CN       , "CN"       },
    {PS_PEXT     , "PEXT"     },
    {PS_V1       , "V1"       },
    {PS_V2       , "V2"       },
    {PS_V3       , "V3"       },
    {PS_V5       , "V5"       },
    {PS_V6       , "V6"       },
    {PS_V7       , "V7"       },
    {PS_V8       , "V8"       },
    {PS_V9       , "V9"       },
    {PS_V13      , "V13"      },
    {PS_V16      , "V16"      },
    {PS_V23      , "V23"      },
    {PS_V24      , "V24"      },
    {PS_V31      , "V31"      },
    {PS_V32      , "V32"      },
    {PS_V33      , "V33"      },
    {PS_KM11_A   , "KM11_A"   },
    {PS_KM12_A   , "KM12_A"   },
    {PS_KM13     , "KM13"     },
    {PS_KI11     , "KI11"     },
    {PS_KI12     , "KI12"     },
    {PS_KI13     , "KI13"     },
    {PS_KI14     , "KI14"     },
    {PS_KI15     , "KI15"     },
    {PS_KM21     , "KM21"     },
    {PS_KM22     , "KM22"     },
    {PS_KM23     , "KM23"     },
    {PS_KM31a    , "KM31a"    },
    {PS_KM32b    , "KM32b"    },
    {PS_KE4      , "KE4"      },
    {PS_KM51     , "KM51"     },
    {PS_KM52     , "KM52"     },
    {PS_KM53     , "KM53"     },
    {PS_KE5      , "KE5"      },
    {PS_KM61     , "KM61"     },
    {PS_KI61     , "KI61"     },
    {PS_KI62     , "KI62"     },
    {PS_KE6      , "KE6"      },
    {PS_KM71     , "KM71"     },
    {PS_KM72     , "KM72"     },
    {PS_KM73     , "KM73"     },
    {PS_KM74     , "KM74"     },
    {PS_KE7      , "KE7"      },
    {PS_KM81     , "KM81"     },
    {PS_KM82     , "KM82"     },
    {PS_KE8      , "KE8"      },
    {PS_KM9      , "KM9"      },
    {PS_KI9      , "KI9"      },
    {PS_KE9      , "KE9"      },
    {PS_KM10     , "KM10"     },
    {PS_KM101    , "KM101"    },
    {PS_KM102    , "KM102"    },
    {PS_KM103    , "KM103"    },
    {PS_KE10     , "KE10"     },
    {PS_KE11     , "KE11"     },
    {PS_KE12     , "KE12"     },
    {PS_KM131    , "KM131"    },
    {PS_KM132    , "KM132"    },
    {PS_KI131    , "KI131"    },
    {PS_KI132    , "KI132"    },
    {PS_KI133    , "KI133"    },
    {PS_KI134    , "KI134"    },
    {PS_KI135    , "KI135"    },
    {PS_KE13     , "KE13"     },
    {PS_KM161    , "KM161"    },
    {PS_KM162    , "KM162"    },
    {PS_KM163    , "KM163"    },
    {PS_KE16     , "KE16"     },
    {PS_KE21     , "KE21"     },
    {PS_KE22     , "KE22"     },
    {PS_KM311    , "KM311"    },
    {PS_KM312    , "KM312"    },
    {PS_KM313    , "KM313"    },
    {PS_KM32     , "KM32"     },
    {PS_KM33     , "KM33"     },
    {PS_KM231    , "KM231"    },
    {PS_KM232    , "KM232"    },
    {PS_KM233    , "KM233"    },
    {PS_KM234    , "KM234"    },
    {PS_KE23     , "KE23"     },
    {PS_KA231    , "KA231"    },
    {PS_KI231    , "KI231"    },
    {PS_KVmo     , "KVmo"     },
    {PS_KM241    , "KM241"    },
    {PS_KE25     , "KE25"     },
    {PS_KE57     , "KE57"     },
    {PS_Km8p5p   , "Km8p5p"   },
    {PS_Km5p5p   , "Km5p5p"   },
    {PS_KE810    , "KE810"    },
    {PS_Km5gap   , "Km5gap"   },
    {PS_Km8f6p   , "Km8f6p"   },
    {PS_Km8s7p   , "Km8s7p"   },
    {PS_Km8gap   , "Km8gap"   },
    {PS_MaxCoeff , "MaxCoeff" },
    {PS_Vfactor1 , "Vfactor1" },
    {PS_Vfactor2 , "Vfactor2" },
    {PS_Vfactor3 , "Vfactor3" },
    {PS_Vfactor5 , "Vfactor5" },
    {PS_Vfactor7 , "Vfactor7" },
    {PS_Vfactor13, "Vfactor13"},
    {PS_Vfactor23, "Vfactor23"},
    {PS_Vf_T3    , "Vf_T3"    },
    {PS_Vf_T2    , "Vf_T2"    },
    {PS_Vf_T1    , "Vf_T1"    },
    {PS_Vf_T6    , "Vf_T6"    },
    {PS_Vf_T5    , "Vf_T5"    },
    {PS_Vf_T9    , "Vf_T9"    },
    {PS_Vf_T13   , "Vf_T13"   },
    {PS_Vf_T23   , "Vf_T23"   },
  };
  return map;
};
template<> const std::map<const PS_rate_constants, const std::string>& get_enum_map<enum PS_rate_constants>() {
  return PS_rate_constants_map();
};
template<> MODULE get_enum_module<enum PS_rate_constants>() {
  return MODULE_PS;
}
