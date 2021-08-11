#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <cvode/cvode_direct.h>

#include "drivers/CVodeMem.hpp"
#include "drivers/driver.hpp"

void CVodeMem::cvode_mem_init(Driver* driver, realtype t0, N_Vector y) {
    if (!initialized) {
        data = alloc_calc_data();
        _cvode_mem = CVodeCreate(CV_BDF);
        if (CVodeInit(_cvode_mem, driver->calculate, t0, y) != CV_SUCCESS) {
            std::cout << "CVodeInit failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (CVodeSStolerances(_cvode_mem, driver->reltol, driver->abstol) != CV_SUCCESS) {
            std::cout << "CVodeSStolerances failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (CVodeSetUserData(_cvode_mem, data) != CV_SUCCESS) {
            std::cout << "CVodeSetUserData failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (CVodeSetMaxStep(_cvode_mem, driver->maxStep) != CV_SUCCESS) {
            std::cout << "CVodeSetMaxStep failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!driver->showWarnings) {
            if (CVodeSetMaxHnilWarns(_cvode_mem, -1) != CV_SUCCESS) {
                std::cout << "CVodeSetMaxHnilWarns failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (CVodeSetNoInactiveRootWarn(_cvode_mem) != CV_SUCCESS) {
                std::cout << "CVodeSetNoInactiveRootWarn failed" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if (CVodeSetMaxNumSteps(_cvode_mem, driver->maxSubSteps) != CV_SUCCESS) {
            std::cout << "CVodeSetMaxNumSteps failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        initialized = true;
    } else {
        if (CVodeReInit(_cvode_mem, t0, y) != CV_SUCCESS) {
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
