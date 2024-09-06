#pragma once

enum BF_RC {
  BF_RC_NONE         ,
  BF_RC_K1           ,  //!< The rate constant for formation of ISP.QH2 complex; unit:  per second
  BF_RC_K2           ,  //!< The rate constant for ISP.QH2-->QH(semi) + ISPH(red) ; unit:  per second
  BF_RC_K3           ,  //!< The rate constant for QH. + cytbL --> Q + cytbL- + H+ Unit: s-1
  BF_RC_K4           ,  //!< The rate constant for cytbL- + cytbH --> cytbL + cytbH- Unit: s-1
  BF_RC_K5           ,  //!< The rate constant for CytbH- + Q --> cytbH + Q- Unit: s-1
  BF_RC_K6           ,  //!< The rate constant  for CytbH- + Q- --> cytbH + Q2- Unit: s-1
  BF_RC_K7           ,  //!< The rate constant for Q binding to Qi site; which assumed half time as 200 us, following Croft's website Unit: s-1
  BF_RC_K8           ,  //!< The rate constant for ISPH + CytC1 --> ISPH(ox) + CytC1+ Unit: s-1
  BF_RC_K9           ,  //!< The rate constant for the electron transport from cytc1 to cytc2 Unit: s-1
  BF_RC_K10          ,  //!< The rate constant for the electron transport from cytc2 to P700 Unit: s-1
  BF_RC_Vmax11       ,  //!< The maximum rate of ATP synthesis Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
  BF_RC_Kqi          ,  //!< The rate constant for uptake of two protons from the stroma to Q2- s-1
  BF_RC_PK           ,  //!< The permeability constant for K Unit: cm s-1
  BF_RC_PMg          ,  //!< The permeability constant for Mg Unit: cm s-1
  BF_RC_PCl          ,  //!< The permeability constant for Cl Unit: cm s-1
  BF_RC_Kau          ,  //!< The rate constant for exciton transfer from perpheral antenna to core antenna, see FI Unit: s-1
  BF_RC_Kua          ,  //!< The rate constant for exciton transfer from core antenna to peripheral antenna, SEE FI Unit: s-1
  BF_RC_Kf           ,  //!< The rate constant for fluorescence emission, see the note in FI Unit: s-1
  BF_RC_Kd           ,  //!< The rate constant for heat dissipation; see the note for FI Unit: s-1
  BF_RC_K15          ,  //!< The rate constant for primary charge separation in PSI Unit: s-1
  BF_RC_K16          ,  //!< The rate constant for electron tranfer from electron acceptor of PSI to Fd Unit: s-1
  BF_RC_Em_ISP       ,  //!< ISPHr + cytc1 --> ISPHox + cytc1-
  BF_RC_Em_Cytf      ,  //!< ISPHr + cytc1 --> ISPHox + cytc1-
  BF_RC_Em_PG        ,  //!< cytc1- + cytc2 --> cytc1 + cytc2-
  BF_RC_MemCap       ,  //!< The membrane capacity
  BF_RC_RVA          ,  //!< The ratio of lumen volume to thylakoid membrane area
  BF_RC_KBs          ,  //!< The buffer equilibrium constant in stroma
  BF_RC_KBl          ,  //!< The buffer equilibrium constant in lumen
  BF_RC_KM1ATP       ,  //!< The michaelis menton constant for ATP for ATP synthesis
  BF_RC_KM1ADP       ,  //!< The michaelis menton constant for ATP for ADP synthesis
  BF_RC_KM1PI        ,  //!< The michaelis menton constant for ATP for PI synthesis
  BF_RC_KM2NADP      ,  //!< The michaelis menten constant for NADP Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
  BF_RC_KM2NADPH     ,  //!< The michaelis menten constant for NADPH Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
  BF_RC_V2M          ,  //!< The maximum rate of NADPH formation Unit: mmol l-1 s-1; The unit for the reactions occurrs in stroma is mmol l-1 s-1
  BF_RC_KE2          ,  //!< Equilibrium constant
  BF_RC_kA_d         ,  //!< The total amount of cytbH or cytbL; Unit: micromole m-2 leaf area
  BF_RC_kA_f         ,  //!< The total amount of cytc; Unit: micromole m-2 leaf area
  BF_RC_kA_U         ,  //!< The total concentration of K in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of K, and Mg and Cl as well, is constant.
  BF_RC_kU_A         ,  //!< The total concentration of Mg in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Mg, and K and Cl as well, is constant.
  BF_RC_kU_d         ,  //!< The total concentration of Cl in both stroma and lumen. Unit: mmol l-1. In this model, it was assumed that the total concentration of Cl in both stroma and lumen is constant.
  BF_RC_kU_f         ,  //!< The total concentration of Ferrodoxin
  BF_RC_k1           ,  //!< The total concentration of the primary electron acceptor of PSI; Unit: micromole m-2 leaf area
  BF_RC_k_r1         ,  //!< The total concentration of plastoquinone in thylakoid membrane. ; Unit: micromole m-2 leaf area
  BF_RC_kz           ,  //!< The total concentration of buffer in stroma; unit: mmol per liter
  BF_RC_k12          ,  //!< The total concentration of buffer in lumen; unit: mmol per liter
  BF_RC_k23          ,  //!< The total number of P700; unit: micromole m-2 leaf area
  BF_RC_k30          ,  //!< The total concentration of NADPH in stroma; 1 is an guessed value;
  BF_RC_MAX          ,
};

enum FI_RC {
  FI_RC_NONE         ,
  FI_RC_kA_d         ,  //!< The rate constant of heat dissipation from peripheral antenna Lazar (1999), 0.25~1 *10^(9)
  FI_RC_kA_f         ,  //!< The rate constant of fluorescence emission from peripheral antenna Lazar 1999, with a lifetime of 5 ns at closed reaction center
  FI_RC_kA_U         ,  //!< The rate constant of exciton transfer from periphral antenna to core antenna Reference needed, a guess
  FI_RC_kU_A         ,  //!< The rate constant of exciton transfer from core antenna to peripheral antenna Reference needed, a guess
  FI_RC_kU_d         ,  //!< The rate constant of heat emission from core antenna
  FI_RC_kU_f         ,  //!< The rate constant of fluorescence emission from core antenna
  FI_RC_k1           ,  //!< The rate constant of primary charge separation for open reaction center
  FI_RC_k_r1         ,  //!< The rate constant of charge recombination for open reactoin center
  FI_RC_kz           ,  //!< The rate constant of the Tyrosine oxidation Lazar (1999); 3.8~50 * 10^6
  FI_RC_k12          ,  //!< The rate constant of the S1 to S2 transition Lazar (1999); 0.667~33.3 * 10^3
  FI_RC_k23          ,  //!< The rate constant of the S2 to S3 transition Lazar (1999); 0.667~33.3 * 10^3
  FI_RC_k30          ,  //!< The rate constant of the S3 to S0 transition Lazar (1999); 0.667~33.3 * 10^3
  FI_RC_k01          ,  //!< The rate constant of the S0 to S1 transition Lazar (1999); 0.667~33.3 * 10^3
  FI_RC_k2           ,  //!< The rate constant of the QA reduction by Pheo- Lazar (1999); 2~2.3 * 10^9
  FI_RC_kAB1         ,  //!< The rate constant of QAQB-->QAQB- Lazar (1999); 2.5~5 * 10^3
  FI_RC_kBA1         ,  //!< The rate constant of the QAQB- -->QAQB Lazar (1999)
  FI_RC_kAB2         ,  //!< The rate constant of the QAQB- --> QAQB2- Lazar (1999); 1.25~3.33 * 10^3
  FI_RC_kBA2         ,  //!< The rate constant of the QAQB2- --> QAQB- Lazar (1999), or same as 330016] depend on the equilibium constant
  FI_RC_k3           ,  //!< The rate constant of the exchange of PQ and QBH2 Lazar (1999),0.12~1 for the fast PQ pool, or 3~8 for the slow recycling PQ pool
  FI_RC_k_r3         ,  //!< The rate constant of the exchange of QB and PQH2 Lazar (1999), since the equilibrium constant is 1 (205 in Lazar, 1999)
  FI_RC_k_pq_oxy     ,  //!< The rate constant of the PQH2 oxidation Lazar (1999),50~500
  FI_RC_QBt          ,  //!< The total concentration of Qb site;
  FI_RC_PQT          ,  //!< The total concentration of PQ;
  FI_RC_MAX          ,
};

enum PR_RC {
  PR_RC_NONE           ,
  PR_RC_V111           ,  //!< Reaction 111: RUBP+O2<-->PGlycolate + PGA
  PR_RC_V112           ,  //!< Reaction 112: PGlycolate-->Pi+Glycolate;
  PR_RC_V113           ,  //!< Reaction 113: Gcea+ATP<-->ADP + PGA
  PR_RC_V121           ,  //!< Reaction 121: Glycolate +O2<-->H2O2+Glyoxylate
  PR_RC_V122           ,  //!< Reaction 122: Glyoxylate + Serine<--> Hydoxypyruvate + Glycine;
  PR_RC_V123           ,  //!< Reaction 123: HydroxylPyruvate + NAD <--> NADH + Glycerate
  PR_RC_V124           ,  //!< Reaction 124: Glyoxylate + Glu  <--> KG + Glycine;
  PR_RC_V131           ,  //!< Reaction 131: NAD+Glycine <--> CO2+ NADH + NH3
  PR_RC_NADHc          ,  //!< NADH concentration in cytosol
  PR_RC_NADc           ,  //!< NAD concentration in cytosol
  PR_RC_GLUc           ,  //!< GLU concentration in cytosol
  PR_RC_KGc            ,  //!< KG concentration in cytosol
  PR_RC_ADP            ,  //!< ADP concentration in chloroplast
  PR_RC_ATP            ,  //!< ATP concentration in chloroplast
  PR_RC_PGA            ,  //!< PGA in chloroplast (2.4 with C3)
  PR_RC_KO             ,  //!< Michaelis constant for O2; ignored w/ PS_PR
  PR_RC_KC             ,  //!< Michaelis constant for CO2; ignored w/ PS_PR
  PR_RC_KR             ,  //!< Michaelis constant for RUBP
  PR_RC_KM112          ,  //!< Km112 for PGlycolate;
  PR_RC_KI1122         ,  //!< Inhibition constant for Glycolate;
  PR_RC_KI1121         ,  //!< The competitive Pi inhibition for PGlycolate
  PR_RC_KM1131         ,  //!< Km for ATP;
  PR_RC_KM1132         ,  //!< Km for Gcea;
  PR_RC_KI113          ,  //!< Competitive inhibition for ATP; in original paper it is 0.36;
  PR_RC_KE113          ,  //!< New: Kleczkowski et al . 1985 Archives of Biochemistry and Biophysics  300, as default
  PR_RC_KM121          ,  //!< Km for Glycolate
  PR_RC_KM1221         ,  //!< Michaelis constant for glyoxylate;
  PR_RC_KM1222         ,  //!< Michaelis constant for serinie;
  PR_RC_KI1221         ,  //!< Inhibition constant for Glycine;
  PR_RC_KE122          ,  //!< New: Guynn, R.W.; Arch. Biochem. Biophys.; 218, 14 (1982).; 0.24. At 25 degree.
  PR_RC_KM123          ,  //!< Michaelis constant for hydroxylpyruvate;
  PR_RC_KI123          ,  //!< Inhibition constant for hydroxypyruvate;
  PR_RC_KE123          ,  //!< Equilibrium constant for hydroxypyruvate;
  PR_RC_KM1241         ,  //!< Michaelis constant for glyoxylate
  PR_RC_KM1242         ,  //!< Michaelis constant for Glu
  PR_RC_KE124          ,  //!< New: Cooper, A.J.L.; Meister, A.; Biochemistry; 11, 661 (1972).; K' 607.
  PR_RC_KM1311         ,  //!< Michaelis constant for Glycine;
  PR_RC_KI1311         ,  //!< Inhibition constant for Serine
  PR_RC_V1T            ,  //!< Multiplied by CE
  PR_RC_KM1011         ,  //!< Michaelis constant for glycerate
  PR_RC_KI1011         ,  //!< Inhibition constant for glycerate
  PR_RC_V2T            ,  //!< Multiplied by CE, 6 for C3
  PR_RC_KM1012         ,  //!< Michaelis constant for glycolate
  PR_RC_KI1012         ,  //!< Inhibition constant for glycolate
  PR_RC_Vfactor112     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor113     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor121     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor122     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor123     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor124     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vfactor131     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PR_RC_Vf_T131        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_Vf_T113        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_Vf_T123        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_Vf_T121        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_Vf_T122        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_Vf_T112        ,  //!< VfactorT used with GRNT & Tp > 25
  PR_RC_MAX            ,
};

enum PS_RC {
  PS_RC_NONE          ,
  PS_RC_CP            ,  //!< Global constant for the total phosphate
  PS_RC_CA            ,  //!< Global constant for the total adenylates
  PS_RC_CN            ,  //!< Global constant for the cytosolic Phosphate concentration;
  PS_RC_PEXT          ,  //!< Global constant for the cytosolic Phosphate concentration;
  PS_RC_V1            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V2            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V3            ,  //!< 1.57*SC  (Harris & Koniger, 1997)
  PS_RC_V5            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V6            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V7            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V8            ,  //!< (Harris & Koniger, 1997)
  PS_RC_V9            ,  //!< 0.17*SC*FC (Harris & Koniger, 1997) *3.
  PS_RC_V13           ,  //!< (Harris & Koniger, 1997)
  PS_RC_V16           ,  //!< (Aflalo & Shavit, 1983, Davenport & McLeod, 1986)
  PS_RC_V23           ,
  PS_RC_V24           ,
  PS_RC_V31           ,  //!< 3.73 / 3 for C3 (Lilley, Chon, Mosbach & Heldt, 1977b)
  PS_RC_V32           ,  //!< 3.73 / 3 for C3 (Lilley, Chon, Mosbach & Heldt, 1977b)
  PS_RC_V33           ,  //!< WY 2018103; 3.73 / 3 for C3 (Lilley, Chon, Mosbach & Heldt, 1977b)
  PS_RC_KM11_A        ,  //!< Factor for KM11 calc
  PS_RC_KM12_A        ,  //!< Factor for KM12 calc
  PS_RC_KM13          ,  //!< RuBP 1 RuBP+CO2->2PGA
  PS_RC_KI11          ,  //!< PGA
  PS_RC_KI12          ,  //!< FBP
  PS_RC_KI13          ,  //!< SBP
  PS_RC_KI14          ,  //!< Pi
  PS_RC_KI15          ,  //!< NADPH
  PS_RC_KM21          ,  //!< PGA 2 PGA+ATP <-> ADP + DPGA
  PS_RC_KM22          ,  //!< ATP 2 PGA+ATP <-> ADP + DPGA
  PS_RC_KM23          ,  //!< ADP
  PS_RC_KM31a         ,  //!< BPGA 3 DPGA+NADPH <->GAP + OP+NADP
  PS_RC_KM32b         ,  //!< NADPH 3 DPGA+NADPH <->GAP + OP+NADP
  PS_RC_KE4           ,  //!< Using the value from Patterson
  PS_RC_KM51          ,  //!< GAP 5 GAP+DHAP <->FBP
  PS_RC_KM52          ,  //!< DHAP 5 GAP+DHAP <->FBP
  PS_RC_KM53          ,  //!< FBP 5 GAP+DHAP <->FBP
  PS_RC_KE5           ,  //!< Defult: 7.1
  PS_RC_KM61          ,  //!< FBP 6 FBP<->F6P+OP
  PS_RC_KI61          ,  //!< F6P
  PS_RC_KI62          ,  //!< Pi
  PS_RC_KE6           ,  //!< The equilibrium constant for this reaction  New: mM Laisk or Bassham and Krause 1969 BBA
  PS_RC_KM71          ,  //!< Xu5P 7 F6P+GAP<->E4P+Xu5P
  PS_RC_KM72          ,  //!< E4P 7 F6P+GAP<->E4P+Xu5P
  PS_RC_KM73          ,  //!< F6P This value was based on estimate
  PS_RC_KM74          ,  //!< Estimate for GAP ORIGINAL 0.1
  PS_RC_KE7           ,  //!< The equilibrium constant for this reaction  New: Laisk Bassham and Krause 1969 BBA
  PS_RC_KM81          ,  //!< DHAP
  PS_RC_KM82          ,  //!< E4P estimate
  PS_RC_KE8           ,  //!< The equilibrium constant for this reaction  New: mM-1  Laisk  Bassham and Krause 1969 BBA. Default: 1.107
  PS_RC_KM9           ,  //!< SBP 9 SBP<->S7P+OP
  PS_RC_KI9           ,  //!< The inibintion constant for Pi;
  PS_RC_KE9           ,  //!< The equilibrium constant of this reaction  New:  mM      Laisk  Bassham and Krause 1969 BBA
  PS_RC_KM10          ,  //!< R5P 10 S7P+GAP<->Ri5P+Xu5P
  PS_RC_KM101         ,  //!< Xu5P
  PS_RC_KM102         ,  //!< Estimate for GAP
  PS_RC_KM103         ,  //!< Estimate for S7P  New
  PS_RC_KE10          ,  //!< The equilibrium constant for this reaction  New: From Laisk or Bassham and Krause 1969 BBA
  PS_RC_KE11          ,  //!< Equilibrium Constant 11 Ri5P<-->Ru5P
  PS_RC_KE12          ,  //!< Equilibrium Constant 12 Xu5P<-->Ru5P
  PS_RC_KM131         ,  //!< Ru5P 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KM132         ,  //!< ATP 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KI131         ,  //!< PGA 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KI132         ,  //!< RuBP 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KI133         ,  //!< Pi 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KI134         ,  //!< ADP 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KI135         ,  //!< ADP 13 Ru5P+ATP<->RuBP+ADP
  PS_RC_KE13          ,  //!< The equilibrium constant for this reaction  New: From Laisk or Bassham and Krause 1969 BBA
  PS_RC_KM161         ,  //!< ADP 16 ADP+Pi<->ATP
  PS_RC_KM162         ,  //!< Pi 16 ADP+Pi<-> ATP
  PS_RC_KM163         ,  //!< ATP 16  ADP+Pi<-> ATP  New: Based on Laisk
  PS_RC_KE16          ,  //!< The equilibrium constant for this reaction  New: From Laisk or Bassham and Krause 1969 BBA
  PS_RC_KE21          ,  //!< Equilibrium constant 21 F6P<->G6P
  PS_RC_KE22          ,  //!< Equilibrium constant 22 G6P<->G1P
  PS_RC_KM311         ,  //!< DHAP 31 DHAPi<->DHAPo
  PS_RC_KM312         ,  //!< Pi 31 DHAPi<->DHAPo
  PS_RC_KM313         ,  //!< Pext 31 DHAPi<->DHAPo
  PS_RC_KM32          ,  //!< PGA 32 PGAi<->PGAo
  PS_RC_KM33          ,  //!< GAP 33 GAPi<->GAPo
  PS_RC_KM231         ,  //!< G1P 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989
  PS_RC_KM232         ,  //!< ATP 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989
  PS_RC_KM233         ,  //!< ADPG 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989
  PS_RC_KM234         ,  //!< PPi 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989
  PS_RC_KE23          ,
  PS_RC_KA231         ,  //!< PGA 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989
  PS_RC_KI231         ,  //!< Pi 23 G1P+ATP<->ADPG + PPi  Laisk et al 1989 WY201803
  PS_RC_KVmo          ,  //!< The minimum maximum velocity  Laisk et al 1989
  PS_RC_KM241         ,
  PS_RC_KE25          ,
  PS_RC_KE57          ,
  PS_RC_Km8p5p        ,
  PS_RC_Km5p5p        ,
  PS_RC_KE810         ,
  PS_RC_Km5gap        ,
  PS_RC_Km8f6p        ,
  PS_RC_Km8s7p        ,
  PS_RC_Km8gap        ,
  PS_RC_MaxCoeff      ,
  PS_RC_Vfactor1      ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor2      ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor3      ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor5      ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor7      ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor13     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vfactor23     ,  //!< VfactorCp used with GRNC and CO2 > 0
  PS_RC_Vf_T3         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T2         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T1         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T6         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T5         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T9         ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T13        ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_Vf_T23        ,  //!< VfactorT used with GRNT & Tp > 25
  PS_RC_MAX           ,
};

enum RROEA_RC {
  RROEA_RC_NONE            ,
  RROEA_RC_ke2GAPDH        ,  //!< The rate constant of electron transfer to GAPDH. From literature.
  RROEA_RC_ke2MDH          ,  //!< The rate constant of electront transfer to MDH, this rate is totally ASSUMED.
  RROEA_RC_ke2FBPase       ,  //!< The rate constant of electron transfer from thioredoxin to FBPase.
  RROEA_RC_ke2SBPase       ,  //!< The rate constant of electron tranfer from thioredoxin to SBPase
  RROEA_RC_ke2PRK          ,  //!< The rate constant of electron transfer from thioredoxin to PRK, Phosphoribulase kinase
  RROEA_RC_ke2RubACT       ,  //!< The rate constant of electron transfer from thioredoxin to Rubisco activase
  RROEA_RC_ke2Fd           ,  //!< The rate constant of electron transfer to fe
  RROEA_RC_keFd2Thio       ,  //!< The rate constant of electron transfer from fd to thio
  RROEA_RC_keFd2Calvin     ,  //!< The rate constant of electron transfer from fd to Calvin cycle
  RROEA_RC_ke2ATPGPP       ,  //!< The rate constant of electron transfer to ATPGPP
  RROEA_RC_KEe2FBPase      ,
  RROEA_RC_KEe2SBPase      ,
  RROEA_RC_KEe2PRK         ,
  RROEA_RC_KEe2ATPase      ,
  RROEA_RC_KEe2RuACT       ,
  RROEA_RC_KEe2GAPDH       ,
  RROEA_RC_KEe2MDH         ,
  RROEA_RC_KEe2ATPGPP      ,
  RROEA_RC_KEeFd2Thio      ,
  RROEA_RC_SA_GAPDH        ,
  RROEA_RC_SA_MDH          ,
  RROEA_RC_SA_PRK          ,
  RROEA_RC_SA_FBPase       ,
  RROEA_RC_SA_SBPase       ,
  RROEA_RC_SA_ATPGPP       ,
  RROEA_RC_SA_ATPase       ,
  RROEA_RC_mw_GAPDH        ,
  RROEA_RC_mw_MDH          ,
  RROEA_RC_mw_PRK          ,
  RROEA_RC_mw_FBPase       ,
  RROEA_RC_mw_SBPase       ,
  RROEA_RC_mw_ATPGPP       ,
  RROEA_RC_mw_ATPase       ,
  RROEA_RC_FdT             ,  //!< Not used with RROEA_EPS
  RROEA_RC_ThioT           ,
  RROEA_RC_RuACTT          ,
  RROEA_RC_MAX             ,
};

enum RedoxReg_RC {
  RedoxReg_RC_NONE               ,
  RedoxReg_RC_Fd_Thio_ET         ,
  RedoxReg_RC_ThioT              ,
  RedoxReg_RC_Thio_Oxidation     ,
  RedoxReg_RC_Thion0             ,  //!< This is a wild guess
  RedoxReg_RC_MAX                ,
};

enum RuACT_RC {
  RuACT_RC_NONE     ,
  RuACT_RC_k1      ,  //!< The rate constant of the activation of the Rubisco bound with RuBP. This step is associated with the ARubisco theVars->activase content or activity;
  RuACT_RC_kn1     ,  //!< The rate constant of E inactivation by binding of RuBP;
  RuACT_RC_km1     ,  //!< The michaelis menton constant for RuBP with E.
  RuACT_RC_Ke2     ,  //!< Data from Mate et al 1996. Unit: micormolar;
  RuACT_RC_Ke3     ,  //!< Data from Mate et al 1996. Unit: micormolar;
  RuACT_RC_k6      ,  //!< micromolar per meter square per second, transfered to unit
  RuACT_RC_kc      ,  //!< Michaelis menton constant for CO2
  RuACT_RC_ko      ,  //!< Michaelis menton constant for O2
  RuACT_RC_kr      ,  //!< The apparaent michaelis menton constant for RuBP
  RuACT_RC_ET      ,  //!< The total concentraiton of E, ER, EC, ECM, ECMR , mM;
  RuACT_RC_Rac     ,  //!< The concentration of the activase, mM
  RuACT_RC_C       ,  //!< mM
  RuACT_RC_O       ,  //!< mM
  RuACT_RC_M       ,
  RuACT_RC_MAX     ,
};

enum SUCS_RC {
  SUCS_RC_NONE         ,
  SUCS_RC_V51          ,  //!< DHAP+GAP --FBP; default 0.5
  SUCS_RC_V52          ,  //!< FBP --F6P + Pi
  SUCS_RC_V55          ,  //!< G1P+UTP --OPOP+UDPG
  SUCS_RC_V56          ,  //!< UDPG+F6P--SUCP + UDP
  SUCS_RC_V57          ,  //!< SUCP--Pi + SUC; 0.27 DEFALT
  SUCS_RC_V58          ,  //!< F26BP--F6P + Pi
  SUCS_RC_V59          ,  //!< F6P + ATP --ADP + F26BP; default 0.03
  SUCS_RC_V62          ,  //!< SUC Sink        0.9 works.
  SUCS_RC_Vdhap_in     ,  //!< DHAP export from chloroplast
  SUCS_RC_Vgap_in      ,  //!< GAP export from chloroplast
  SUCS_RC_Vpga_in      ,  //!< PGA export from chloropalst
  SUCS_RC_ATc          ,  //!< mM
  SUCS_RC_UTc          ,  //!< mM
  SUCS_RC_PTc          ,
  SUCS_RC_KE501        ,  //!< 0.05 for C3
  SUCS_RC_Km511        ,
  SUCS_RC_Km512        ,
  SUCS_RC_Km513        ,
  SUCS_RC_KE51         ,
  SUCS_RC_Km521        ,
  SUCS_RC_KI521        ,
  SUCS_RC_KI522        ,
  SUCS_RC_KI523        ,
  SUCS_RC_KE52         ,
  SUCS_RC_KE531        ,
  SUCS_RC_KE541        ,
  SUCS_RC_Km551        ,
  SUCS_RC_Km552        ,
  SUCS_RC_Km553        ,
  SUCS_RC_Km554        ,
  SUCS_RC_KE55         ,
  SUCS_RC_Km561        ,
  SUCS_RC_Km562        ,
  SUCS_RC_KI561        ,
  SUCS_RC_KI562        ,
  SUCS_RC_KI563        ,
  SUCS_RC_KI564        ,
  SUCS_RC_KI565        ,
  SUCS_RC_KE56         ,
  SUCS_RC_Km571        ,
  SUCS_RC_Ki572        ,
  SUCS_RC_KE57         ,
  SUCS_RC_Km581        ,
  SUCS_RC_KI581        ,
  SUCS_RC_KI582        ,
  SUCS_RC_Km591        ,
  SUCS_RC_Km593        ,
  SUCS_RC_KI591        ,
  SUCS_RC_KE59         ,
  SUCS_RC_KE61         ,
  SUCS_RC_Km621        ,
  SUCS_RC_MAX          ,
};

enum XanCycle_RC {
  XanCycle_RC_NONE     ,
  XanCycle_RC_kva     ,  //!< Ruth Frommolt et a; 2001; Planta
  XanCycle_RC_kaz     ,  //!< Ruth Frommolt et a; 2001; Planta
  XanCycle_RC_kza     ,  //!< Ruth Frommolt et a; 2001; Planta
  XanCycle_RC_kav     ,  //!< Ruth Frommolt et a; 2001; Planta. This is not given in the paper. Therefore, teh value is really an educated guess.
  XanCycle_RC_MAX     ,
};

