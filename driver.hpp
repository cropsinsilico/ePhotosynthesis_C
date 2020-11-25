#pragma once
#include <vector>

#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>

struct UserData {
  std::vector<realtype> coeffs;
};

inline UserData *alloc_user_data() {
  UserData *data;
  data = new UserData();
  return data;
}
