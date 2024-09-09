// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#include <map>
#include "enums/enums_POOL.hpp"


inline const std::map<BF_POOL, double>& BF_POOL_defaults() {
  static const std::map<BF_POOL, double> map {
    {BF_POOL_kA_d    , 1.0 },
    {BF_POOL_kA_f    , 1.0 },
    {BF_POOL_kA_U    , 20.0},
    {BF_POOL_kU_A    , 10.0},
    {BF_POOL_kU_d    , 2.0 },
    {BF_POOL_kU_f    , 1.0 },
    {BF_POOL_k1      , 1.0 },
    {BF_POOL_k_r1    , 8.0 },
    {BF_POOL_kz      , 38.0},
    {BF_POOL_k12     , 38.0},
    {BF_POOL_k23     , 1.0 },
    {BF_POOL_k30     , 1.0 },
    {BF_POOL_k01     , 0.0 },
    {BF_POOL_k2      , 0.0 },
    {BF_POOL_kAB1    , 0.0 },
    {BF_POOL_kBA1    , 0.0 },
    {BF_POOL_kAB2    , 0.0 },
    {BF_POOL_kBA2    , 0.0 },
    {BF_POOL_k3      , 0.0 },
    {BF_POOL_k_r3    , 0.0 },
    {BF_POOL_k_pq_oxy, 0.0 },
  };
  return map;
};

template<>
inline const std::map<enum BF_POOL, double>& get_enum_defaults<enum BF_POOL>() {
  return BF_POOL_defaults();
}
inline const std::map<FI_POOL, double>& FI_POOL_defaults() {
  static const std::map<FI_POOL, double> map {
    {FI_POOL_QBt, 1},
    {FI_POOL_PQT, 8},
  };
  return map;
};

template<>
inline const std::map<enum FI_POOL, double>& get_enum_defaults<enum FI_POOL>() {
  return FI_POOL_defaults();
}
inline const std::map<RROEA_POOL, double>& RROEA_POOL_defaults() {
  static const std::map<RROEA_POOL, double> map {
    {RROEA_POOL_GAPDH , 0.0006583},
    {RROEA_POOL_FBPase, 0.0025856},
    {RROEA_POOL_SBPase, 0.0129870},
    {RROEA_POOL_PRK   , 0.0036585},
    {RROEA_POOL_ATPase, 0.0012   },
    {RROEA_POOL_ATPGPP, 0.0285714},
    {RROEA_POOL_MDH   , 0.0037481},
    {RROEA_POOL_ThioT , 0.081    },
    {RROEA_POOL_FdT   , 0.081    },
    {RROEA_POOL_RuACTT, 0.0056   },
  };
  return map;
};

template<>
inline const std::map<enum RROEA_POOL, double>& get_enum_defaults<enum RROEA_POOL>() {
  return RROEA_POOL_defaults();
}
inline const std::map<RuACT_POOL, double>& RuACT_POOL_defaults() {
  static const std::map<RuACT_POOL, double> map {
    {RuACT_POOL_ET , 1.2   },
    {RuACT_POOL_Rac, 0.0056},
    {RuACT_POOL_C  , 0.012 },
    {RuACT_POOL_O  , 0.260 },
    {RuACT_POOL_M  , 5.0   },
  };
  return map;
};

template<>
inline const std::map<enum RuACT_POOL, double>& get_enum_defaults<enum RuACT_POOL>() {
  return RuACT_POOL_defaults();
}
inline const std::map<SUCS_POOL, double>& SUCS_POOL_defaults() {
  static const std::map<SUCS_POOL, double> map {
    {SUCS_POOL_ATc, 1.0 },
    {SUCS_POOL_UTc, 1.5 },
    {SUCS_POOL_PTc, 15.0},
  };
  return map;
};

template<>
inline const std::map<enum SUCS_POOL, double>& get_enum_defaults<enum SUCS_POOL>() {
  return SUCS_POOL_defaults();
}
