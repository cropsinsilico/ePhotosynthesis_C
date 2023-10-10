#pragma once
#include <boost/python.hpp>

#include "Variables.hpp"
#include "drivers/drivers.hpp"

namespace ePhotosynthesis {
namespace python {
    struct DriverWrap : drivers::Driver, boost::python::wrapper<drivers::Driver> {
        void setup() {
            this->get_override("setup")();
        }
        void getResults() {
            this->get_override("getResults")();
        }
        std::vector<double> MB(realtype t, N_Vector u) {
            return this->get_override("MB")(t, u);
        }
    };
    void exportModules();
    void exportDrivers();
}
}