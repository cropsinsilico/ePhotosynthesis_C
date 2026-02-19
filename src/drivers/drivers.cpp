#include "drivers/drivers.hpp"
#include "Variables.hpp"


void ePhotosynthesis::drivers::select_driver(const DriverType& driverChoice,
					     const bool useC3, const bool x) {
  if (x) {
    std::cerr << "SELECTING DRIVER: " << driverChoice <<
      " (useC3 = " << useC3 << ")" << std::endl;
    ePhotosynthesis::drivers::select_all_drivers(false);
  }
#define CASE_DRIVER(drv)						\
  ePhotosynthesis::drivers::drv ## Driver::enableC3(useC3);		\
  ePhotosynthesis::drivers::drv ## Driver::select(x);			\
  if (x) {                                                              \
    std::cerr << "DRIVER SELECTED: " << #drv <<                         \
      " (useC3 = " << useC3 << ")" << std::endl;                        \
  }
  SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
}
void ePhotosynthesis::drivers::select_driver(const std::string& driverChoice) {
  if (driverChoice.empty()) {
    select_all_drivers(true);
    return;
  }
  DriverType driverChoiceKey = utils::enum_string2key<DriverType>(driverChoice);
  bool useC3 = (driverChoiceKey == EPS);
  ePhotosynthesis::drivers::select_driver(driverChoiceKey, useC3, true);
}
void ePhotosynthesis::drivers::deselect_driver(const std::string& driverChoice) {
  if (driverChoice.empty()) {
    select_all_drivers(false);
    return;
  }
  DriverType driverChoiceKey = utils::enum_string2key<DriverType>(driverChoice);
  ePhotosynthesis::drivers::select_driver(driverChoiceKey, false, false);
}

void ePhotosynthesis::drivers::select_all_drivers(const bool x) {
#define SELECT_DRIVER(drv)                                      \
  ePhotosynthesis::drivers::drv ## Driver::enableC3(false);     \
  ePhotosynthesis::drivers::drv ## Driver::select(x)
  FOR_EACH(SELECT_DRIVER, EXPAND(MEMBERS_DRIVER));
#undef SELECT_DRIVER
}

ePhotosynthesis::drivers::Driver*
ePhotosynthesis::drivers::create_driver(const DriverType& driverChoice,
					Variables *theVars,
					const double begintime,
					const double stepsize,
					const double stoptime,
					const int maxSubSteps,
					const double abstol,
					const double reltol,
					const std::size_t para,
					const double ratio,
					const bool showWarn,
                                        const std::vector<std::string>& outVars) {
  ePhotosynthesis::drivers::Driver* out = nullptr;
#define CASE_DRIVER(drv)						\
  out = new ePhotosynthesis::drivers::drv ## Driver(theVars, begintime, stepsize,	\
						    stoptime, maxSubSteps, abstol, \
						    reltol, para, ratio, showWarn, outVars)
  SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
  return out;
}

ePhotosynthesis::drivers::Driver*
ePhotosynthesis::drivers::create_driver(const DriverType& driverChoice,
					Variables *theVars,
					const DriverParam& param) {
  ePhotosynthesis::drivers::Driver* out = nullptr;
#define CASE_DRIVER(drv)						\
  out = new ePhotosynthesis::drivers::drv ## Driver(theVars, param)
  SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
  return out;
}
