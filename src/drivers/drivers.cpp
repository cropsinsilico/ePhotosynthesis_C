#include "drivers/drivers.hpp"
#include "Variables.hpp"


void ePhotosynthesis::drivers::select_driver(const DriverType& driverChoice,
					     const bool useC3, const bool x) {
  std::cerr << "SELECTING DRIVER: " << driverChoice << std::endl;
#define CASE_DRIVER(drv)						\
  ePhotosynthesis::drivers::drv ## Driver::enableC3(useC3);		\
  ePhotosynthesis::drivers::drv ## Driver::select(x);			\
  std::cerr << "DRIVER SELECTED: " << #drv << std::endl
  SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
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
					const bool showWarn) {
  ePhotosynthesis::drivers::Driver* out = nullptr;
#define CASE_DRIVER(drv)						\
  out = new ePhotosynthesis::drivers::drv ## Driver(theVars, begintime, stepsize,	\
						    stoptime, maxSubSteps, abstol, \
						    reltol, para, ratio, showWarn)
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
