#pragma once
#include <memory>
#include <boost/python.hpp>

#include "Variables.hpp"
#include "drivers/drivers.hpp"

namespace ePhotosynthesis {
namespace python {
#define PYTHON_DRIVER_ARGS                              \
    theVars, startTime, stepSize, endTime,              \
        maxSubsteps, atol, rtol, para, ratio,           \
        showWarn, outVars
#define PYTHON_DRIVER_ARG_DEFS                                  \
    Variables *theVars, const double startTime,                 \
      const double stepSize, const double endTime,              \
      const int maxSubsteps,                                    \
      const double atol, const double rtol,                     \
      const std::size_t para, const double ratio,               \
      const bool showWarn,                                      \
      const boost::python::object& outVars
    template<class T>
    std::shared_ptr<T> Driver_Init(PYTHON_DRIVER_ARG_DEFS);
    template<typename T>
    boost::python::object Driver_Run(T& drv);
#define ADD_DRIVER(mod)                                                 \
    std::shared_ptr<drivers::mod ## Driver>                             \
    mod ## Driver_Init(PYTHON_DRIVER_ARG_DEFS);                         \
    boost::python::object mod ## Driver_Run(drivers::mod ## Driver& drv)
    FOR_EACH(ADD_DRIVER, EXPAND(MEMBERS_DRIVER));
#undef ADD_DRIVER
    void exportModules();
    void exportDrivers();
}
}
