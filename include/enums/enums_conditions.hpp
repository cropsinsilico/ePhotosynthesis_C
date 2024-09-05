#pragma once

enum PR_conditions {
  PR_conditions_NONE     ,
  PR_GCEA      ,  //!< Glycerate in chloroplast; derived based on V113
  PR_GCA       ,  //!< Derived from radioactive labelling experiment; assume equal concenatration inside and outshide chloroplast
  PR_PGCA      ,  //!< Phosphoglycolate in chloroplast derived based on the Km112; orignal value is : 0.0029;
  PR_GCAc      ,  //!< See the note for GCA.
  PR_GOAc      ,  //!< Glyoxylate in cytosol; 0.028; EXPERIMENTAL DATA;
  PR_SERc      ,  //!< Serine in cytosol; 7.5 original value
  PR_GLYc      ,  //!< Glycine in cytosol; 1.8 original vlaue
  PR_HPRc      ,  //!< HydroxylPyruvate; derived from equation 123;
  PR_GCEAc     ,  //!< Glycerate in cytosol; assume at equilibrium with GCEA initially.
  PR_RuBP      ,  //!< RuBP concentration
  PR_conditions_MAX     ,
};

enum PS_conditions {
  PS_conditions_NONE     ,
  PS_RuBP     ,
  PS_PGA      ,
  PS_DPGA     ,
  PS_T3P      ,
  PS_ADPG     ,  //!< (0.0 with C3)
  PS_FBP      ,
  PS_E4P      ,
  PS_S7P      ,
  PS_SBP      ,
  PS_ATP      ,
  PS_HexP     ,
  PS_PenP     ,
  PS_conditions_MAX     ,
};

enum RedoxReg_conditions {
  RedoxReg_conditions_NONE     ,
  RedoxReg_ThioRatio     ,
  RedoxReg_FBPase        ,
  RedoxReg_SBPase        ,
  RedoxReg_PRK           ,
  RedoxReg_ATPase        ,
  RedoxReg_conditions_MAX     ,
};

enum FI_conditions {
  FI_conditions_NONE     ,
  FI_A              ,  //!< The concentration of excitons in the peripheral antenna
  FI_U              ,  //!< The concentration fo excitons in the core antenna
  FI_P680ePheo      ,  //!< The concentration of the P680Pheo
  FI_P680pPheon     ,  //!< The concentration for the P680+ Pheo-
  FI_P680pPheo      ,  //!< The concentration of P680+ Pheo
  FI_P680Pheon      ,  //!< The concentration of P680Pheo-
  FI_Yz             ,  //!< The concentration of reduced tyrosine
  FI_S1T            ,  //!< The concentration of S1 in combination with reduced tyrosine
  FI_S2T            ,  //!< The concentration of S2 in combination with reduced tyrosine
  FI_S3T            ,  //!< The concentration of S3 in combination with reduced tyrosine
  FI_S0T            ,  //!< The concentration of S0 in combination with reduced tyrosine
  FI_S1Tp           ,  //!< The concentration of S1 in combination with oxidized tyrosine
  FI_S2Tp           ,  //!< The concentration of S2 in combination with oxidized tyrosine
  FI_S3Tp           ,  //!< The concentration of S3 in combination with oxidized tyrosine
  FI_S0Tp           ,  //!< The concentration of S0 in combination with oxidized tyrosine
  FI_QAQB           ,  //!< The concentration of [QAQB]
  FI_QAnQB          ,  //!< The concentration of [QA-QB]
  FI_QAQBn          ,  //!< The concentration of [QAQB-]
  FI_QAnQBn         ,  //!< The concentration of [QA-QB-]
  FI_QAQB2n         ,  //!< The concentration of [QAQB2-]
  FI_QAnQB2n        ,  //!< The concentration of [QA-QB2-]
  FI_PQn            ,  //!< The concentration of reduced PQ, i.e. PQH2;
  FI_conditions_MAX     ,
};

enum RuACT_conditions {
  RuACT_conditions_NONE     ,
  RuACT_ER       ,  //!< The concentration of inactive ER
  RuACT_Eaf      ,  //!< The total concentration of E, EC, AND ECM
  RuACT_ECMR     ,  //!< The concentration of ECMR
  RuACT_RuBP     ,  //!< The concentration of ECMR
  RuACT_conditions_MAX     ,
};

enum XanCycle_conditions {
  XanCycle_conditions_NONE     ,
  XanCycle_Vx      ,  //!< * 0.37
  XanCycle_Ax      ,  //!< * 0.37
  XanCycle_Zx      ,  //!< * 0.37
  XanCycle_ABA     ,
  XanCycle_conditions_MAX     ,
};

enum SUCS_conditions {
  SUCS_conditions_NONE     ,
  SUCS_T3Pc       ,
  SUCS_FBPc       ,
  SUCS_HexPc      ,
  SUCS_F26BPc     ,
  SUCS_UDPGc      ,
  SUCS_SUCP       ,
  SUCS_SUC        ,
  SUCS_PGAc       ,
  SUCS_conditions_MAX     ,
};

enum BF_conditions {
  BF_conditions_NONE     ,
  BF_ISPHr       ,  //!< The reduced ion sulfer protein (ISPH)
  BF_cytc1       ,  //!< The oxidized state of cytc1
  BF_ISPo        ,  //!< The oxidized ion sulfer protein (ISP)
  BF_ISPoQH2     ,  //!< The complex of oxidized ion sulfer protein and reduced quinone
  BF_QHsemi      ,  //!< Semiquinone
  BF_cytbL       ,  //!< The oxidized cytbL
  BF_Qi          ,  //!< The binding Quinone
  BF_Q           ,  //!< Quinone
  BF_cytbH       ,  //!< The oxidized form of cytbH
  BF_Qn          ,  //!< Q-
  BF_Qr          ,  //!< Q2-
  BF_QH2         ,  //!< QH2
  BF_cytc2       ,  //!< oxidized cytc2
  BF_P700        ,  //!< The reduced state of P700, including both P700 and excited P700
  BF_ADP         ,  //!< ADP in stroma
  BF__Pi         ,  //!< Phosphate in stroma
  BF_ATP         ,  //!< ATP in stroma
  BF_Ks          ,  //!< K ions in stroma
  BF_Mgs         ,  //!< Mg ions in stroma
  BF_Cls         ,  //!< Cl ions in stroma
  BF_Aip         ,  //!< The number of photons in peripheral antenna
  BF_U           ,  //!< The number of photons in core antenna
  BF_An          ,  //!< The reduced electron acceptor in PSI
  BF_Fdn         ,  //!< The reduced ferrodoxin
  BF_BFHs        ,  //!< The total concentration of proton and protonated buffer species in stroma, put in unit: mmol l-1
  BF_BFHl        ,  //!< The total concentration of proton and protonated buffer species in lumen, unit: mmol l-1
  BF_PHs         ,  //!< The PH value of the stroma
  BF_PHl         ,  //!< The PH value of the lumen
  BF_NADPH       ,  //!< The NADPH concentration in stroma, Unit: mmol l-1;
  BF_conditions_MAX     ,
};

enum RROEA_conditions {
  RROEA_conditions_NONE     ,
  RROEA_MDH       ,  //!< The initial concentration of actove MDH
  RROEA_Thio      ,  //!< The initial concentration of reduced thioredoxin
  RROEA_Fd        ,  //!< The initial concentration of reduced ferrodoxin
  RROEA_RuACT     ,  //!< The initial concentration of active Rubisco activase
  RROEA_conditions_MAX     ,
};

