#pragma once

enum XanCycle_init {
  XanCycle_Vx      ,
  XanCycle_Ax      ,
  XanCycle_Zx      ,
  XanCycle_ABA     ,
};

enum RedoxReg_init {
  RedoxReg_ThioRatio     ,
  RedoxReg_FBPase        ,
  RedoxReg_SBPase        ,
  RedoxReg_PRK           ,
  RedoxReg_ATPase        ,
};

enum SUCS_init {
  SUCS_T3Pc       ,
  SUCS_FBPc       ,
  SUCS_HexPc      ,
  SUCS_F26BPc     ,
  SUCS_UDPGc      ,
  SUCS_SUCP       ,
  SUCS_SUC        ,
  SUCS_PGAc       ,
};

enum FI_init {
  FI_A              ,
  FI_U              ,
  FI_P680ePheo      ,
  FI_P680pPheon     ,
  FI_P680pPheo      ,
  FI_P680Pheon      ,
  FI_Yz             ,
  FI_S1T            ,
  FI_S2T            ,
  FI_S3T            ,
  FI_S0T            ,
  FI_S1Tp           ,
  FI_S2Tp           ,
  FI_S3Tp           ,
  FI_S0Tp           ,
  FI_QAQB           ,
  FI_QAnQB          ,
  FI_QAQBn          ,
  FI_QAnQBn         ,
  FI_QAQB2n         ,
  FI_QAnQB2n        ,
  FI_PQn            ,
};

enum RROEA_init {
  RROEA_MDH       ,
  RROEA_Thio      ,
  RROEA_Fd        ,
  RROEA_RuACT     ,
};

enum PR_init {
  PR_GCEA      ,
  PR_GCA       ,
  PR_PGCA      ,
  PR_GCAc      ,
  PR_GOAc      ,
  PR_SERc      ,
  PR_GLYc      ,
  PR_HPRc      ,
  PR_GCEAc     ,
  PR_RuBP      ,
};

enum PS_init {
  PS_RuBP     ,
  PS_PGA      ,
  PS_DPGA     ,
  PS_T3P      ,
  PS_ADPG     ,
  PS_FBP      ,
  PS_E4P      ,
  PS_S7P      ,
  PS_SBP      ,
  PS_ATP      ,
  PS_HexP     ,
  PS_PenP     ,
};

enum RuACT_init {
  RuACT_ER       ,
  RuACT_Eaf      ,
  RuACT_ECMR     ,
  RuACT_RuBP     ,
};

enum BF_init {
  BF_ISPHr       ,
  BF_cytc1       ,
  BF_ISPo        ,
  BF_ISPoQH2     ,
  BF_QHsemi      ,
  BF_cytbL       ,
  BF_Qi          ,
  BF_Q           ,
  BF_cytbH       ,
  BF_Qn          ,
  BF_Qr          ,
  BF_QH2         ,
  BF_cytc2       ,
  BF_P700        ,
  BF_ADP         ,
  BF__Pi         ,
  BF_ATP         ,
  BF_Ks          ,
  BF_Mgs         ,
  BF_Cls         ,
  BF_Aip         ,
  BF_U           ,
  BF_An          ,
  BF_Fdn         ,
  BF_BFHs        ,
  BF_BFHl        ,
  BF_PHs         ,
  BF_PHl         ,
  BF_NADPH       ,
};

