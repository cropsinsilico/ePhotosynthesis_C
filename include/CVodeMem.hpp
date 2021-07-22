#pragma once
#include <cvode/cvode_direct.h>
class Driver;
class CalcData;

class CVodeMem {
public:
    static CVodeMem& create() {
        static CVodeMem instance;
        return instance;
    }
    ~CVodeMem();
    CVodeMem(const CVodeMem&) = delete;
    CVodeMem(CVodeMem&&) = delete;
    CVodeMem& operator=(const CVodeMem&) = delete;
    CVodeMem& operator=(CVodeMem&&) = delete;

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
