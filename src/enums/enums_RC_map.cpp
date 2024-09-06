#include "enums/enums_RC_map.hpp"

#include "enums/enums_utils.hpp"


const std::map<const BF_RC, const std::string>& BF_RC_map() {
  static const std::map<const BF_RC, const std::string> map {
    {BF_RC_K1      , "K1"      },
    {BF_RC_K2      , "K2"      },
    {BF_RC_K3      , "K3"      },
    {BF_RC_K4      , "K4"      },
    {BF_RC_K5      , "K5"      },
    {BF_RC_K6      , "K6"      },
    {BF_RC_K7      , "K7"      },
    {BF_RC_K8      , "K8"      },
    {BF_RC_K9      , "K9"      },
    {BF_RC_K10     , "K10"     },
    {BF_RC_Vmax11  , "Vmax11"  },
    {BF_RC_Kqi     , "Kqi"     },
    {BF_RC_PK      , "PK"      },
    {BF_RC_PMg     , "PMg"     },
    {BF_RC_PCl     , "PCl"     },
    {BF_RC_Kau     , "Kau"     },
    {BF_RC_Kua     , "Kua"     },
    {BF_RC_Kf      , "Kf"      },
    {BF_RC_Kd      , "Kd"      },
    {BF_RC_K15     , "K15"     },
    {BF_RC_K16     , "K16"     },
    {BF_RC_Em_ISP  , "Em_ISP"  },
    {BF_RC_Em_Cytf , "Em_Cytf" },
    {BF_RC_Em_PG   , "Em_PG"   },
    {BF_RC_MemCap  , "MemCap"  },
    {BF_RC_RVA     , "RVA"     },
    {BF_RC_KBs     , "KBs"     },
    {BF_RC_KBl     , "KBl"     },
    {BF_RC_KM1ATP  , "KM1ATP"  },
    {BF_RC_KM1ADP  , "KM1ADP"  },
    {BF_RC_KM1PI   , "KM1PI"   },
    {BF_RC_KM2NADP , "KM2NADP" },
    {BF_RC_KM2NADPH, "KM2NADPH"},
    {BF_RC_V2M     , "V2M"     },
    {BF_RC_KE2     , "KE2"     },
    {BF_RC_kA_d    , "kA_d"    },
    {BF_RC_kA_f    , "kA_f"    },
    {BF_RC_kA_U    , "kA_U"    },
    {BF_RC_kU_A    , "kU_A"    },
    {BF_RC_kU_d    , "kU_d"    },
    {BF_RC_kU_f    , "kU_f"    },
    {BF_RC_k1      , "k1"      },
    {BF_RC_k_r1    , "k_r1"    },
    {BF_RC_kz      , "kz"      },
    {BF_RC_k12     , "k12"     },
    {BF_RC_k23     , "k23"     },
    {BF_RC_k30     , "k30"     },
  };
  return map;
};
template<> const std::map<const BF_RC, const std::string>& get_enum_map<enum BF_RC>() {
  return BF_RC_map();
};
template<> MODULE get_enum_module<enum BF_RC>() {
  return MODULE_BF;
}

const std::map<const FI_RC, const std::string>& FI_RC_map() {
  static const std::map<const FI_RC, const std::string> map {
    {FI_RC_kA_d    , "kA_d"    },
    {FI_RC_kA_f    , "kA_f"    },
    {FI_RC_kA_U    , "kA_U"    },
    {FI_RC_kU_A    , "kU_A"    },
    {FI_RC_kU_d    , "kU_d"    },
    {FI_RC_kU_f    , "kU_f"    },
    {FI_RC_k1      , "k1"      },
    {FI_RC_k_r1    , "k_r1"    },
    {FI_RC_kz      , "kz"      },
    {FI_RC_k12     , "k12"     },
    {FI_RC_k23     , "k23"     },
    {FI_RC_k30     , "k30"     },
    {FI_RC_k01     , "k01"     },
    {FI_RC_k2      , "k2"      },
    {FI_RC_kAB1    , "kAB1"    },
    {FI_RC_kBA1    , "kBA1"    },
    {FI_RC_kAB2    , "kAB2"    },
    {FI_RC_kBA2    , "kBA2"    },
    {FI_RC_k3      , "k3"      },
    {FI_RC_k_r3    , "k_r3"    },
    {FI_RC_k_pq_oxy, "k_pq_oxy"},
    {FI_RC_QBt     , "QBt"     },
    {FI_RC_PQT     , "PQT"     },
  };
  return map;
};
template<> const std::map<const FI_RC, const std::string>& get_enum_map<enum FI_RC>() {
  return FI_RC_map();
};
template<> MODULE get_enum_module<enum FI_RC>() {
  return MODULE_FI;
}

const std::map<const PR_RC, const std::string>& PR_RC_map() {
  static const std::map<const PR_RC, const std::string> map {
    {PR_RC_V111      , "V111"      },
    {PR_RC_V112      , "V112"      },
    {PR_RC_V113      , "V113"      },
    {PR_RC_V121      , "V121"      },
    {PR_RC_V122      , "V122"      },
    {PR_RC_V123      , "V123"      },
    {PR_RC_V124      , "V124"      },
    {PR_RC_V131      , "V131"      },
    {PR_RC_NADHc     , "NADHc"     },
    {PR_RC_NADc      , "NADc"      },
    {PR_RC_GLUc      , "GLUc"      },
    {PR_RC_KGc       , "KGc"       },
    {PR_RC_ADP       , "ADP"       },
    {PR_RC_ATP       , "ATP"       },
    {PR_RC_PGA       , "PGA"       },
    {PR_RC_KO        , "KO"        },
    {PR_RC_KC        , "KC"        },
    {PR_RC_KR        , "KR"        },
    {PR_RC_KM112     , "KM112"     },
    {PR_RC_KI1122    , "KI1122"    },
    {PR_RC_KI1121    , "KI1121"    },
    {PR_RC_KM1131    , "KM1131"    },
    {PR_RC_KM1132    , "KM1132"    },
    {PR_RC_KI113     , "KI113"     },
    {PR_RC_KE113     , "KE113"     },
    {PR_RC_KM121     , "KM121"     },
    {PR_RC_KM1221    , "KM1221"    },
    {PR_RC_KM1222    , "KM1222"    },
    {PR_RC_KI1221    , "KI1221"    },
    {PR_RC_KE122     , "KE122"     },
    {PR_RC_KM123     , "KM123"     },
    {PR_RC_KI123     , "KI123"     },
    {PR_RC_KE123     , "KE123"     },
    {PR_RC_KM1241    , "KM1241"    },
    {PR_RC_KM1242    , "KM1242"    },
    {PR_RC_KE124     , "KE124"     },
    {PR_RC_KM1311    , "KM1311"    },
    {PR_RC_KI1311    , "KI1311"    },
    {PR_RC_V1T       , "V1T"       },
    {PR_RC_KM1011    , "KM1011"    },
    {PR_RC_KI1011    , "KI1011"    },
    {PR_RC_V2T       , "V2T"       },
    {PR_RC_KM1012    , "KM1012"    },
    {PR_RC_KI1012    , "KI1012"    },
    {PR_RC_Vfactor112, "Vfactor112"},
    {PR_RC_Vfactor113, "Vfactor113"},
    {PR_RC_Vfactor121, "Vfactor121"},
    {PR_RC_Vfactor122, "Vfactor122"},
    {PR_RC_Vfactor123, "Vfactor123"},
    {PR_RC_Vfactor124, "Vfactor124"},
    {PR_RC_Vfactor131, "Vfactor131"},
    {PR_RC_Vf_T131   , "Vf_T131"   },
    {PR_RC_Vf_T113   , "Vf_T113"   },
    {PR_RC_Vf_T123   , "Vf_T123"   },
    {PR_RC_Vf_T121   , "Vf_T121"   },
    {PR_RC_Vf_T122   , "Vf_T122"   },
    {PR_RC_Vf_T112   , "Vf_T112"   },
  };
  return map;
};
template<> const std::map<const PR_RC, const std::string>& get_enum_map<enum PR_RC>() {
  return PR_RC_map();
};
template<> MODULE get_enum_module<enum PR_RC>() {
  return MODULE_PR;
}

const std::map<const PS_RC, const std::string>& PS_RC_map() {
  static const std::map<const PS_RC, const std::string> map {
    {PS_RC_CP       , "CP"       },
    {PS_RC_CA       , "CA"       },
    {PS_RC_CN       , "CN"       },
    {PS_RC_PEXT     , "PEXT"     },
    {PS_RC_V1       , "V1"       },
    {PS_RC_V2       , "V2"       },
    {PS_RC_V3       , "V3"       },
    {PS_RC_V5       , "V5"       },
    {PS_RC_V6       , "V6"       },
    {PS_RC_V7       , "V7"       },
    {PS_RC_V8       , "V8"       },
    {PS_RC_V9       , "V9"       },
    {PS_RC_V13      , "V13"      },
    {PS_RC_V16      , "V16"      },
    {PS_RC_V23      , "V23"      },
    {PS_RC_V24      , "V24"      },
    {PS_RC_V31      , "V31"      },
    {PS_RC_V32      , "V32"      },
    {PS_RC_V33      , "V33"      },
    {PS_RC_KM11_A   , "KM11_A"   },
    {PS_RC_KM12_A   , "KM12_A"   },
    {PS_RC_KM13     , "KM13"     },
    {PS_RC_KI11     , "KI11"     },
    {PS_RC_KI12     , "KI12"     },
    {PS_RC_KI13     , "KI13"     },
    {PS_RC_KI14     , "KI14"     },
    {PS_RC_KI15     , "KI15"     },
    {PS_RC_KM21     , "KM21"     },
    {PS_RC_KM22     , "KM22"     },
    {PS_RC_KM23     , "KM23"     },
    {PS_RC_KM31a    , "KM31a"    },
    {PS_RC_KM32b    , "KM32b"    },
    {PS_RC_KE4      , "KE4"      },
    {PS_RC_KM51     , "KM51"     },
    {PS_RC_KM52     , "KM52"     },
    {PS_RC_KM53     , "KM53"     },
    {PS_RC_KE5      , "KE5"      },
    {PS_RC_KM61     , "KM61"     },
    {PS_RC_KI61     , "KI61"     },
    {PS_RC_KI62     , "KI62"     },
    {PS_RC_KE6      , "KE6"      },
    {PS_RC_KM71     , "KM71"     },
    {PS_RC_KM72     , "KM72"     },
    {PS_RC_KM73     , "KM73"     },
    {PS_RC_KM74     , "KM74"     },
    {PS_RC_KE7      , "KE7"      },
    {PS_RC_KM81     , "KM81"     },
    {PS_RC_KM82     , "KM82"     },
    {PS_RC_KE8      , "KE8"      },
    {PS_RC_KM9      , "KM9"      },
    {PS_RC_KI9      , "KI9"      },
    {PS_RC_KE9      , "KE9"      },
    {PS_RC_KM10     , "KM10"     },
    {PS_RC_KM101    , "KM101"    },
    {PS_RC_KM102    , "KM102"    },
    {PS_RC_KM103    , "KM103"    },
    {PS_RC_KE10     , "KE10"     },
    {PS_RC_KE11     , "KE11"     },
    {PS_RC_KE12     , "KE12"     },
    {PS_RC_KM131    , "KM131"    },
    {PS_RC_KM132    , "KM132"    },
    {PS_RC_KI131    , "KI131"    },
    {PS_RC_KI132    , "KI132"    },
    {PS_RC_KI133    , "KI133"    },
    {PS_RC_KI134    , "KI134"    },
    {PS_RC_KI135    , "KI135"    },
    {PS_RC_KE13     , "KE13"     },
    {PS_RC_KM161    , "KM161"    },
    {PS_RC_KM162    , "KM162"    },
    {PS_RC_KM163    , "KM163"    },
    {PS_RC_KE16     , "KE16"     },
    {PS_RC_KE21     , "KE21"     },
    {PS_RC_KE22     , "KE22"     },
    {PS_RC_KM311    , "KM311"    },
    {PS_RC_KM312    , "KM312"    },
    {PS_RC_KM313    , "KM313"    },
    {PS_RC_KM32     , "KM32"     },
    {PS_RC_KM33     , "KM33"     },
    {PS_RC_KM231    , "KM231"    },
    {PS_RC_KM232    , "KM232"    },
    {PS_RC_KM233    , "KM233"    },
    {PS_RC_KM234    , "KM234"    },
    {PS_RC_KE23     , "KE23"     },
    {PS_RC_KA231    , "KA231"    },
    {PS_RC_KI231    , "KI231"    },
    {PS_RC_KVmo     , "KVmo"     },
    {PS_RC_KM241    , "KM241"    },
    {PS_RC_KE25     , "KE25"     },
    {PS_RC_KE57     , "KE57"     },
    {PS_RC_Km8p5p   , "Km8p5p"   },
    {PS_RC_Km5p5p   , "Km5p5p"   },
    {PS_RC_KE810    , "KE810"    },
    {PS_RC_Km5gap   , "Km5gap"   },
    {PS_RC_Km8f6p   , "Km8f6p"   },
    {PS_RC_Km8s7p   , "Km8s7p"   },
    {PS_RC_Km8gap   , "Km8gap"   },
    {PS_RC_MaxCoeff , "MaxCoeff" },
    {PS_RC_Vfactor1 , "Vfactor1" },
    {PS_RC_Vfactor2 , "Vfactor2" },
    {PS_RC_Vfactor3 , "Vfactor3" },
    {PS_RC_Vfactor5 , "Vfactor5" },
    {PS_RC_Vfactor7 , "Vfactor7" },
    {PS_RC_Vfactor13, "Vfactor13"},
    {PS_RC_Vfactor23, "Vfactor23"},
    {PS_RC_Vf_T3    , "Vf_T3"    },
    {PS_RC_Vf_T2    , "Vf_T2"    },
    {PS_RC_Vf_T1    , "Vf_T1"    },
    {PS_RC_Vf_T6    , "Vf_T6"    },
    {PS_RC_Vf_T5    , "Vf_T5"    },
    {PS_RC_Vf_T9    , "Vf_T9"    },
    {PS_RC_Vf_T13   , "Vf_T13"   },
    {PS_RC_Vf_T23   , "Vf_T23"   },
  };
  return map;
};
template<> const std::map<const PS_RC, const std::string>& get_enum_map<enum PS_RC>() {
  return PS_RC_map();
};
template<> MODULE get_enum_module<enum PS_RC>() {
  return MODULE_PS;
}

const std::map<const RROEA_RC, const std::string>& RROEA_RC_map() {
  static const std::map<const RROEA_RC, const std::string> map {
    {RROEA_RC_ke2GAPDH   , "ke2GAPDH"   },
    {RROEA_RC_ke2MDH     , "ke2MDH"     },
    {RROEA_RC_ke2FBPase  , "ke2FBPase"  },
    {RROEA_RC_ke2SBPase  , "ke2SBPase"  },
    {RROEA_RC_ke2PRK     , "ke2PRK"     },
    {RROEA_RC_ke2RubACT  , "ke2RubACT"  },
    {RROEA_RC_ke2Fd      , "ke2Fd"      },
    {RROEA_RC_keFd2Thio  , "keFd2Thio"  },
    {RROEA_RC_keFd2Calvin, "keFd2Calvin"},
    {RROEA_RC_ke2ATPGPP  , "ke2ATPGPP"  },
    {RROEA_RC_KEe2FBPase , "KEe2FBPase" },
    {RROEA_RC_KEe2SBPase , "KEe2SBPase" },
    {RROEA_RC_KEe2PRK    , "KEe2PRK"    },
    {RROEA_RC_KEe2ATPase , "KEe2ATPase" },
    {RROEA_RC_KEe2RuACT  , "KEe2RuACT"  },
    {RROEA_RC_KEe2GAPDH  , "KEe2GAPDH"  },
    {RROEA_RC_KEe2MDH    , "KEe2MDH"    },
    {RROEA_RC_KEe2ATPGPP , "KEe2ATPGPP" },
    {RROEA_RC_KEeFd2Thio , "KEeFd2Thio" },
    {RROEA_RC_SA_GAPDH   , "SA_GAPDH"   },
    {RROEA_RC_SA_MDH     , "SA_MDH"     },
    {RROEA_RC_SA_PRK     , "SA_PRK"     },
    {RROEA_RC_SA_FBPase  , "SA_FBPase"  },
    {RROEA_RC_SA_SBPase  , "SA_SBPase"  },
    {RROEA_RC_SA_ATPGPP  , "SA_ATPGPP"  },
    {RROEA_RC_SA_ATPase  , "SA_ATPase"  },
    {RROEA_RC_mw_GAPDH   , "mw_GAPDH"   },
    {RROEA_RC_mw_MDH     , "mw_MDH"     },
    {RROEA_RC_mw_PRK     , "mw_PRK"     },
    {RROEA_RC_mw_FBPase  , "mw_FBPase"  },
    {RROEA_RC_mw_SBPase  , "mw_SBPase"  },
    {RROEA_RC_mw_ATPGPP  , "mw_ATPGPP"  },
    {RROEA_RC_mw_ATPase  , "mw_ATPase"  },
    {RROEA_RC_FdT        , "FdT"        },
    {RROEA_RC_ThioT      , "ThioT"      },
    {RROEA_RC_RuACTT     , "RuACTT"     },
  };
  return map;
};
template<> const std::map<const RROEA_RC, const std::string>& get_enum_map<enum RROEA_RC>() {
  return RROEA_RC_map();
};
template<> MODULE get_enum_module<enum RROEA_RC>() {
  return MODULE_RROEA;
}

const std::map<const RedoxReg_RC, const std::string>& RedoxReg_RC_map() {
  static const std::map<const RedoxReg_RC, const std::string> map {
    {RedoxReg_RC_Fd_Thio_ET    , "Fd_Thio_ET"    },
    {RedoxReg_RC_ThioT         , "ThioT"         },
    {RedoxReg_RC_Thio_Oxidation, "Thio_Oxidation"},
    {RedoxReg_RC_Thion0        , "Thion0"        },
  };
  return map;
};
template<> const std::map<const RedoxReg_RC, const std::string>& get_enum_map<enum RedoxReg_RC>() {
  return RedoxReg_RC_map();
};
template<> MODULE get_enum_module<enum RedoxReg_RC>() {
  return MODULE_RedoxReg;
}

const std::map<const RuACT_RC, const std::string>& RuACT_RC_map() {
  static const std::map<const RuACT_RC, const std::string> map {
    {RuACT_RC_k1 , "k1" },
    {RuACT_RC_kn1, "kn1"},
    {RuACT_RC_km1, "km1"},
    {RuACT_RC_Ke2, "Ke2"},
    {RuACT_RC_Ke3, "Ke3"},
    {RuACT_RC_k6 , "k6" },
    {RuACT_RC_kc , "kc" },
    {RuACT_RC_ko , "ko" },
    {RuACT_RC_kr , "kr" },
    {RuACT_RC_ET , "ET" },
    {RuACT_RC_Rac, "Rac"},
    {RuACT_RC_C  , "C"  },
    {RuACT_RC_O  , "O"  },
    {RuACT_RC_M  , "M"  },
  };
  return map;
};
template<> const std::map<const RuACT_RC, const std::string>& get_enum_map<enum RuACT_RC>() {
  return RuACT_RC_map();
};
template<> MODULE get_enum_module<enum RuACT_RC>() {
  return MODULE_RuACT;
}

const std::map<const SUCS_RC, const std::string>& SUCS_RC_map() {
  static const std::map<const SUCS_RC, const std::string> map {
    {SUCS_RC_V51     , "V51"     },
    {SUCS_RC_V52     , "V52"     },
    {SUCS_RC_V55     , "V55"     },
    {SUCS_RC_V56     , "V56"     },
    {SUCS_RC_V57     , "V57"     },
    {SUCS_RC_V58     , "V58"     },
    {SUCS_RC_V59     , "V59"     },
    {SUCS_RC_V62     , "V62"     },
    {SUCS_RC_Vdhap_in, "Vdhap_in"},
    {SUCS_RC_Vgap_in , "Vgap_in" },
    {SUCS_RC_Vpga_in , "Vpga_in" },
    {SUCS_RC_ATc     , "ATc"     },
    {SUCS_RC_UTc     , "UTc"     },
    {SUCS_RC_PTc     , "PTc"     },
    {SUCS_RC_KE501   , "KE501"   },
    {SUCS_RC_Km511   , "Km511"   },
    {SUCS_RC_Km512   , "Km512"   },
    {SUCS_RC_Km513   , "Km513"   },
    {SUCS_RC_KE51    , "KE51"    },
    {SUCS_RC_Km521   , "Km521"   },
    {SUCS_RC_KI521   , "KI521"   },
    {SUCS_RC_KI522   , "KI522"   },
    {SUCS_RC_KI523   , "KI523"   },
    {SUCS_RC_KE52    , "KE52"    },
    {SUCS_RC_KE531   , "KE531"   },
    {SUCS_RC_KE541   , "KE541"   },
    {SUCS_RC_Km551   , "Km551"   },
    {SUCS_RC_Km552   , "Km552"   },
    {SUCS_RC_Km553   , "Km553"   },
    {SUCS_RC_Km554   , "Km554"   },
    {SUCS_RC_KE55    , "KE55"    },
    {SUCS_RC_Km561   , "Km561"   },
    {SUCS_RC_Km562   , "Km562"   },
    {SUCS_RC_KI561   , "KI561"   },
    {SUCS_RC_KI562   , "KI562"   },
    {SUCS_RC_KI563   , "KI563"   },
    {SUCS_RC_KI564   , "KI564"   },
    {SUCS_RC_KI565   , "KI565"   },
    {SUCS_RC_KE56    , "KE56"    },
    {SUCS_RC_Km571   , "Km571"   },
    {SUCS_RC_Ki572   , "Ki572"   },
    {SUCS_RC_KE57    , "KE57"    },
    {SUCS_RC_Km581   , "Km581"   },
    {SUCS_RC_KI581   , "KI581"   },
    {SUCS_RC_KI582   , "KI582"   },
    {SUCS_RC_Km591   , "Km591"   },
    {SUCS_RC_Km593   , "Km593"   },
    {SUCS_RC_KI591   , "KI591"   },
    {SUCS_RC_KE59    , "KE59"    },
    {SUCS_RC_KE61    , "KE61"    },
    {SUCS_RC_Km621   , "Km621"   },
  };
  return map;
};
template<> const std::map<const SUCS_RC, const std::string>& get_enum_map<enum SUCS_RC>() {
  return SUCS_RC_map();
};
template<> MODULE get_enum_module<enum SUCS_RC>() {
  return MODULE_SUCS;
}

const std::map<const XanCycle_RC, const std::string>& XanCycle_RC_map() {
  static const std::map<const XanCycle_RC, const std::string> map {
    {XanCycle_RC_kva, "kva"},
    {XanCycle_RC_kaz, "kaz"},
    {XanCycle_RC_kza, "kza"},
    {XanCycle_RC_kav, "kav"},
  };
  return map;
};
template<> const std::map<const XanCycle_RC, const std::string>& get_enum_map<enum XanCycle_RC>() {
  return XanCycle_RC_map();
};
template<> MODULE get_enum_module<enum XanCycle_RC>() {
  return MODULE_XanCycle;
}

