#pragma once

enum BF_COND {
  BF_COND_NONE        ,
  BF_COND_ISPHr       ,  //!< The reduced ion sulfer protein (ISPH)
  BF_COND_cytc1       ,  //!< The oxidized state of cytc1
  BF_COND_ISPo        ,  //!< The oxidized ion sulfer protein (ISP)
  BF_COND_ISPoQH2     ,  //!< The complex of oxidized ion sulfer protein and reduced quinone
  BF_COND_QHsemi      ,  //!< Semiquinone
  BF_COND_cytbL       ,  //!< The oxidized cytbL
  BF_COND_Qi          ,  //!< The binding Quinone
  BF_COND_Q           ,  //!< Quinone
  BF_COND_cytbH       ,  //!< The oxidized form of cytbH
  BF_COND_Qn          ,  //!< Q-
  BF_COND_Qr          ,  //!< Q2-
  BF_COND_QH2         ,  //!< QH2
  BF_COND_cytc2       ,  //!< oxidized cytc2
  BF_COND_P700        ,  //!< The reduced state of P700, including both P700 and excited P700
  BF_COND_ADP         ,  //!< ADP in stroma
  BF_COND__Pi         ,  //!< Phosphate in stroma
  BF_COND_ATP         ,  //!< ATP in stroma
  BF_COND_Ks          ,  //!< K ions in stroma
  BF_COND_Mgs         ,  //!< Mg ions in stroma
  BF_COND_Cls         ,  //!< Cl ions in stroma
  BF_COND_Aip         ,  //!< The number of photons in peripheral antenna
  BF_COND_U           ,  //!< The number of photons in core antenna
  BF_COND_An          ,  //!< The reduced electron acceptor in PSI
  BF_COND_Fdn         ,  //!< The reduced ferrodoxin
  BF_COND_BFHs        ,  //!< The total concentration of proton and protonated buffer species in stroma, put in unit: mmol l-1
  BF_COND_BFHl        ,  //!< The total concentration of proton and protonated buffer species in lumen, unit: mmol l-1
  BF_COND_PHs         ,  //!< The PH value of the stroma
  BF_COND_PHl         ,  //!< The PH value of the lumen
  BF_COND_NADPH       ,  //!< The NADPH concentration in stroma, Unit: mmol l-1;
  BF_COND_MAX         ,
};

enum FI_COND {
  FI_COND_NONE           ,
  FI_COND_A              ,  //!< The concentration of excitons in the peripheral antenna
  FI_COND_U              ,  //!< The concentration fo excitons in the core antenna
  FI_COND_P680ePheo      ,  //!< The concentration of the P680Pheo
  FI_COND_P680pPheon     ,  //!< The concentration for the P680+ Pheo-
  FI_COND_P680pPheo      ,  //!< The concentration of P680+ Pheo
  FI_COND_P680Pheon      ,  //!< The concentration of P680Pheo-
  FI_COND_Yz             ,  //!< The concentration of reduced tyrosine
  FI_COND_S1T            ,  //!< The concentration of S1 in combination with reduced tyrosine
  FI_COND_S2T            ,  //!< The concentration of S2 in combination with reduced tyrosine
  FI_COND_S3T            ,  //!< The concentration of S3 in combination with reduced tyrosine
  FI_COND_S0T            ,  //!< The concentration of S0 in combination with reduced tyrosine
  FI_COND_S1Tp           ,  //!< The concentration of S1 in combination with oxidized tyrosine
  FI_COND_S2Tp           ,  //!< The concentration of S2 in combination with oxidized tyrosine
  FI_COND_S3Tp           ,  //!< The concentration of S3 in combination with oxidized tyrosine
  FI_COND_S0Tp           ,  //!< The concentration of S0 in combination with oxidized tyrosine
  FI_COND_QAQB           ,  //!< The concentration of [QAQB]
  FI_COND_QAnQB          ,  //!< The concentration of [QA-QB]
  FI_COND_QAQBn          ,  //!< The concentration of [QAQB-]
  FI_COND_QAnQBn         ,  //!< The concentration of [QA-QB-]
  FI_COND_QAQB2n         ,  //!< The concentration of [QAQB2-]
  FI_COND_QAnQB2n        ,  //!< The concentration of [QA-QB2-]
  FI_COND_PQn            ,  //!< The concentration of reduced PQ, i.e. PQH2;
  FI_COND_MAX            ,
};

enum PR_COND {
  PR_COND_NONE      ,
  PR_COND_GCEA      ,  //!< Glycerate in chloroplast; derived based on V113
  PR_COND_GCA       ,  //!< Derived from radioactive labelling experiment; assume equal concenatration inside and outshide chloroplast
  PR_COND_PGCA      ,  //!< Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;
  PR_COND_GCAc      ,  //!< See the note for GCA.
  PR_COND_GOAc      ,  //!< Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
  PR_COND_SERc      ,  //!< Serine in cytosol; 7.5 original value
  PR_COND_GLYc      ,  //!< Glycine in cytosol; 1.8 original vlaue
  PR_COND_HPRc      ,  //!< HydroxylPyruvate; derived from equation 123;
  PR_COND_GCEAc     ,  //!< Glycerate in cytosol; assume at equilibrium with GCEA initially.
  PR_COND_RuBP      ,  //!< RuBP concentration
  PR_COND_MAX       ,
};

enum PS_COND {
  PS_COND_NONE     ,
  PS_COND_RuBP     ,
  PS_COND_PGA      ,
  PS_COND_DPGA     ,
  PS_COND_T3P      ,
  PS_COND_ADPG     ,  //!< (0.0 with C3)
  PS_COND_FBP      ,
  PS_COND_E4P      ,
  PS_COND_S7P      ,
  PS_COND_SBP      ,
  PS_COND_ATP      ,
  PS_COND_HexP     ,
  PS_COND_PenP     ,
  PS_COND_MAX      ,
};

enum RROEA_COND {
  RROEA_COND_NONE      ,
  RROEA_COND_MDH       ,  //!< The initial concentration of actove MDH
  RROEA_COND_Thio      ,  //!< The initial concentration of reduced thioredoxin
  RROEA_COND_Fd        ,  //!< The initial concentration of reduced ferrodoxin
  RROEA_COND_RuACT     ,  //!< The initial concentration of active Rubisco activase
  RROEA_COND_MAX       ,
};

enum RedoxReg_COND {
  RedoxReg_COND_NONE          ,
  RedoxReg_COND_ThioRatio     ,
  RedoxReg_COND_FBPase        ,
  RedoxReg_COND_SBPase        ,
  RedoxReg_COND_PRK           ,
  RedoxReg_COND_ATPase        ,
  RedoxReg_COND_MAX           ,
};

enum RuACT_COND {
  RuACT_COND_NONE     ,
  RuACT_COND_ER       ,  //!< The concentration of inactive ER
  RuACT_COND_Eaf      ,  //!< The total concentration of E, EC, AND ECM
  RuACT_COND_ECMR     ,  //!< The concentration of ECMR
  RuACT_COND_RuBP     ,  //!< The concentration of ECMR
  RuACT_COND_MAX      ,
};

enum SUCS_COND {
  SUCS_COND_NONE       ,
  SUCS_COND_T3Pc       ,
  SUCS_COND_FBPc       ,
  SUCS_COND_HexPc      ,
  SUCS_COND_F26BPc     ,
  SUCS_COND_UDPGc      ,
  SUCS_COND_SUCP       ,
  SUCS_COND_SUC        ,
  SUCS_COND_PGAc       ,
  SUCS_COND_MAX        ,
};

enum XanCycle_COND {
  XanCycle_COND_NONE     ,
  XanCycle_COND_Vx      ,  //!< * 0.37
  XanCycle_COND_Ax      ,  //!< * 0.37
  XanCycle_COND_Zx      ,  //!< * 0.37
  XanCycle_COND_ABA     ,
  XanCycle_COND_MAX     ,
};

