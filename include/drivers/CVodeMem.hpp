#pragma once
#include <cvode/cvode_direct.h>

namespace ePhotosynthesis {
namespace drivers {

class Driver;
class CalcData;

class CVodeMem {
public:
    /**
      This is a singleton class the initializes an interacts with the ODE solver. This class sets all
      the memory allocations and stepping parameters.
      */
    static CVodeMem& create() {
        static CVodeMem instance;
        return instance;
    }
    ~CVodeMem();
    CVodeMem(const CVodeMem&) = delete;
    CVodeMem(CVodeMem&&) = delete;
    CVodeMem& operator=(const CVodeMem&) = delete;
    CVodeMem& operator=(CVodeMem&&) = delete;

    /**
      Initialize the ODE solver using the given Driver instance.

      \param driver Instance of Driver, containing the setpping and solving parameters.
      \param t0 The first timestamp to run the calculations at.
      \param y Specialized data vector containing the inputs for the solver.
      */
    void cvode_mem_init(Driver* driver, realtype t0, N_Vector y);
private:
    CVodeMem() {
        initialized = false;
        data = nullptr;
        _cvode_mem = nullptr;
    }
    bool initialized;
    void* _cvode_mem;
    CalcData* data;
};

}  // namespace drivers
}  // namespace ePhotosynthesis
