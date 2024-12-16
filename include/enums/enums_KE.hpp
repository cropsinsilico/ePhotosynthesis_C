// This code is generated by generate_enum.py and should not be
// modified directly
#pragma once

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_BF_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_BF, PARAM_TYPE_KE> {
  typedef ENUM_BF_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_BFKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_BF, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_BF, PARAM_TYPE_KE> BF;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_FIBF_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_FIBF, PARAM_TYPE_KE> {
  typedef ENUM_FIBF_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_FIBFKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_FIBF, PARAM_TYPE_KE> FIBF;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_FI_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_FI, PARAM_TYPE_KE> {
  typedef ENUM_FI_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_FIKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_FI, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_FI, PARAM_TYPE_KE> FI;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_PR_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_PR, PARAM_TYPE_KE> {
  typedef ENUM_PR_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_PRKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PR, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_PR, PARAM_TYPE_KE> PR;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_PS_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_PS, PARAM_TYPE_KE> {
  typedef ENUM_PS_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_PSKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_PS, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_PS, PARAM_TYPE_KE> PS;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_RROEA_KE : int {
    NONE           ,
    KEe2FBPase     ,
    KEe2SBPase     ,
    KEe2PRK        ,
    KEe2ATPase     ,  //!< [CONST] 2.177727336 was set in code, but not used
    KEe2RuACT      ,
    KEe2GAPDH      ,
    KEe2MDH        ,
    KEe2ATPGPP     ,
    KEeFd2Thio     ,
    MAX            ,
};
template<>
struct enum_helper<MODULE_RROEA, PARAM_TYPE_KE> {
  typedef ENUM_RROEA_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type : int {
    NONE           ,
    KEe2FBPase     ,
    KEe2SBPase     ,
    KEe2PRK        ,
    KEe2ATPase     ,  //!< [CONST] 2.177727336 was set in code, but not used
    KEe2RuACT      ,
    KEe2GAPDH      ,
    KEe2MDH        ,
    KEe2ATPGPP     ,
    KEeFd2Thio     ,
    MAX            ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_RROEAKE		\
    NONE           ,		\
    KEe2FBPase     ,		\
    KEe2SBPase     ,		\
    KEe2PRK        ,		\
    KEe2ATPase     ,		\
    KEe2RuACT      ,		\
    KEe2GAPDH      ,		\
    KEe2MDH        ,		\
    KEe2ATPGPP     ,		\
    KEeFd2Thio     ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_RROEA, PARAM_TYPE_KE> RROEA;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_RedoxReg_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_RedoxReg, PARAM_TYPE_KE> {
  typedef ENUM_RedoxReg_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_RedoxRegKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_RedoxReg, PARAM_TYPE_KE> RedoxReg;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_RuACT_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_RuACT, PARAM_TYPE_KE> {
  typedef ENUM_RuACT_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_RuACTKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_RuACT, PARAM_TYPE_KE> RuACT;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_SUCS_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_SUCS, PARAM_TYPE_KE> {
  typedef ENUM_SUCS_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_SUCSKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_SUCS, PARAM_TYPE_KE> SUCS;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_XanCycle_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_XanCycle, PARAM_TYPE_KE> {
  typedef ENUM_XanCycle_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_XanCycleKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_XanCycle, PARAM_TYPE_KE> XanCycle;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_NONE_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_NONE, PARAM_TYPE_KE> {
  typedef ENUM_NONE_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_NONEKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_NONE, PARAM_TYPE_KE> NONE;
}

#ifdef EPHOTO_USE_SCOPED_ENUM
enum class ENUM_MAX_KE : int {
    NONE   ,
    MAX    ,
};
template<>
struct enum_helper<MODULE_MAX, PARAM_TYPE_KE> {
  typedef ENUM_MAX_KE type;
};
#else // EPHOTO_USE_SCOPED_ENUM
template<>
enum ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type : int {
    NONE   ,
    MAX    ,
};
#endif // EPHOTO_USE_SCOPED_ENUM
#define MEMBERS_MAXKE		\
    NONE   ,		\
    MAX
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::all;
template<> const std::map<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::names;
template<> const std::map<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::defaults;
template<> const std::map<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type, double> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::defaults_C3;
template<> const std::map<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type, std::string> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::glymaids;
template<> const std::map<std::string, typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::aliases;
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::constant;
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::calculated;
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::nonvector;
template<> std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::skipped;
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::resetone;
template<> const std::vector<typename ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::Type> ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE>::initonce;

namespace KE {
  typedef ValueSetEnum<MODULE_MAX, PARAM_TYPE_KE> MAX;
}
