#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>

#include "drivers/CVodeMem.hpp"
#include "drivers/driver.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;

void CVodeMem::cvode_mem_init(Driver* driver, realtype t0, N_Vector y) {
    if (!initialized) {
        data = alloc_calc_data();
#ifdef SUNDIALS_CONTEXT_REQUIRED
        _cvode_mem = CVodeCreate(CV_BDF, driver->context[0]);
#else // SUNDIALS_CONTEXT_REQUIRED
	_cvode_mem = CVodeCreate(CV_BDF);
#endif // SUNDIALS_CONTEXT_REQUIRED
        if (CVodeInit(_cvode_mem, driver->calculate, t0, y) != CV_SUCCESS) {
            std::cout << "CVodeInit failed" << std::endl;
            throw std::runtime_error("CVodeInit failed");
        }

        if (CVodeSStolerances(_cvode_mem, driver->reltol, driver->abstol) != CV_SUCCESS) {
            std::cout << "CVodeSStolerances failed" << std::endl;
            throw std::runtime_error("CVodeSStolerances failed");
        }

        if (CVodeSetUserData(_cvode_mem, data) != CV_SUCCESS) {
            std::cout << "CVodeSetUserData failed" << std::endl;
            throw std::runtime_error("CVodeSetUserData failed");
        }
        if (CVodeSetMaxStep(_cvode_mem, driver->maxStep) != CV_SUCCESS) {
            std::cout << "CVodeSetMaxStep failed" << std::endl;
            throw std::runtime_error("CVodeSetMaxStep failed");
        }
        if (!driver->showWarnings) {
            if (CVodeSetMaxHnilWarns(_cvode_mem, -1) != CV_SUCCESS) {
                std::cout << "CVodeSetMaxHnilWarns failed" << std::endl;
                throw std::runtime_error("CVodeSetMaxHnilWarns failed");
            }
            if (CVodeSetNoInactiveRootWarn(_cvode_mem) != CV_SUCCESS) {
                std::cout << "CVodeSetNoInactiveRootWarn failed" << std::endl;
                throw std::runtime_error("CVodeSetNoInactiveRootWarn failed");
            }
        }
        if (CVodeSetMaxNumSteps(_cvode_mem, driver->maxSubSteps) != CV_SUCCESS) {
            std::cout << "CVodeSetMaxNumSteps failed" << std::endl;
            throw std::runtime_error("CVodeSetMaxNumSteps failed");
        }
        initialized = true;
    } else {
        if (CVodeReInit(_cvode_mem, t0, y) != CV_SUCCESS) {
            std::cout << "CVodeReInit failed" << std::endl;
            throw std::runtime_error("CVodeReInit failed");
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
