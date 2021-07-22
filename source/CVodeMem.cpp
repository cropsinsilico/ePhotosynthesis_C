#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>

#include "CVodeMem.hpp"
#include "driver.hpp"

void CVodeMem::cvode_mem_init(Driver* driver, realtype t0, N_Vector y) {
    if (!initialized) {
        data = alloc_calc_data();
        _cvode_mem = CVodeCreate(CV_BDF);
        if (CVodeInit(_cvode_mem, driver->calculate, t0, y) != 0) {
            std::cout << "CVodeInit failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (CVodeSStolerances(_cvode_mem, driver->reltol, driver->abstol) != 0) {
            std::cout << "CVodeSStolerances failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (CVodeSetUserData(_cvode_mem, data) != 0) {
            std::cout << "CVodeSetUserData failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (CVodeSetMaxNumSteps(_cvode_mem, driver->maxSubSteps) != 0) {
            std::cout << "CVodeSetMaxNumSteps failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        initialized = true;
    } else {
        if (CVodeReInit(_cvode_mem, t0, y) != 0) {
            std::cout << "CVodeReInit failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    driver->data = data;
    driver->cvode_mem = _cvode_mem;

}

CVodeMem::~CVodeMem() {
    if (initialized) {
        CVodeFree(&_cvode_mem);
        delete data;
    }
}
