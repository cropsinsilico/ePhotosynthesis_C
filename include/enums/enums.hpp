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
