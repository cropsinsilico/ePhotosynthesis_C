#pragma once

#include "enums/enums_RC.hpp"
#include "enums/enums_COND.hpp"

enum EMPTY_ENUM {};

enum MODULE {
  MODULE_NONE,
  MODULE_BF,
  MODULE_FI,
  MODULE_PR,
  MODULE_PS,
  MODULE_RROEA,
  MODULE_RedoxReg,
  MODULE_RuACT,
  MODULE_SUCS,
  MODULE_XanCycle,
  MODULE_MAX,
};
template<MODULE T>
struct MODULE2Enum_COND {
public:
  typedef enum EMPTY_ENUM Type;
};
template<MODULE T>
struct MODULE2Enum_RC {
public:
  typedef enum EMPTY_ENUM Type;
};
template<>
struct MODULE2Enum_COND<MODULE_BF> {
  typedef enum BF_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_FI> {
  typedef enum FI_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_PR> {
  typedef enum PR_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_PS> {
  typedef enum PS_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_RROEA> {
  typedef enum RROEA_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_RedoxReg> {
  typedef enum RedoxReg_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_RuACT> {
  typedef enum RuACT_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_SUCS> {
  typedef enum SUCS_COND Type;
};
template<>
struct MODULE2Enum_COND<MODULE_XanCycle> {
  typedef enum XanCycle_COND Type;
};
template<>
struct MODULE2Enum_RC<MODULE_BF> {
  typedef enum BF_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_FI> {
  typedef enum FI_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_PR> {
  typedef enum PR_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_PS> {
  typedef enum PS_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_RROEA> {
  typedef enum RROEA_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_RedoxReg> {
  typedef enum RedoxReg_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_RuACT> {
  typedef enum RuACT_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_SUCS> {
  typedef enum SUCS_RC Type;
};
template<>
struct MODULE2Enum_RC<MODULE_XanCycle> {
  typedef enum XanCycle_RC Type;
};
