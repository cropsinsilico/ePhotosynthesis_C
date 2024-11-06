#include "drivers/CM_Driver.hpp"
#include "drivers/DynaPS_Driver.hpp"
#include "drivers/EPS_Driver.hpp"
#include "drivers/trDynaPS_Driver.hpp"

#define CASE_DRIVER_METHOD(drv, X)					\
  case drv: {								\
    X(drv);								\
    break;								\
  }
#define SWITCH_DRIVER(drv, X)						\
  switch (drv) {							\
    FOR_EACH_WITH_SUFFIX_ARGS(CASE_DRIVER_METHOD, (X), MEMBERS_DRIVER);	\
  default:								\
    std::cerr << __func__ << ": Invalid driver choice given: \"" <<	\
      drv << "\"" << std::endl;						\
    exit(EXIT_FAILURE);							\
  }

namespace ePhotosynthesis {
namespace drivers {
  
  void select_driver(const DriverType& driverChoice,
		     const bool useC3, const bool x = true) {
    std::cerr << "SELECTING DRIVER: " << driverChoice << std::endl;
#define CASE_DRIVER(drv)			\
    drivers::drv ## Driver::enableC3(useC3);	\
    drivers::drv ## Driver::select(x);		\
    std::cerr << "DRIVER SELECTED: " << #drv << std::endl
    SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
  }

  drivers::Driver* create_driver(const DriverType& driverChoice,
				 Variables *theVars,
				 const double begintime,
				 const double stepsize,
				 const double stoptime,
				 const int maxSubSteps,
				 const double abstol,
				 const double reltol,
				 const std::size_t para = 1,
				 const double ratio = 1,
				 const bool showWarn = false) {
    drivers::Driver* out = nullptr;
#define CASE_DRIVER(drv)						\
    out = new drivers::drv ## Driver(theVars, begintime, stepsize,	\
				     stoptime, maxSubSteps, abstol,	\
				     reltol, para, ratio, showWarn)
    SWITCH_DRIVER(driverChoice, CASE_DRIVER);
#undef CASE_DRIVER
    return out;
  }
}
}
