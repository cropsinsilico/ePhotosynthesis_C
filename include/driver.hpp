#pragma once

/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include <vector>

#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>
#include "globals.hpp"
struct Variables;
class Driver;
class CVodeMem;
struct UserData {
  std::vector<realtype> coeffs;
};

inline UserData *alloc_user_data() {
  UserData *data;
  data = new UserData();
  return data;
}

struct CalcData {
    Driver *drv;
};

inline CalcData *alloc_calc_data() {
    CalcData *data;
    data = new CalcData();
    return data;
}

class Driver {
public:
    Driver(Variables *theVars, const double start, const double step, const double endtime,
           const int maxSubSteps, const double atol, const double rtol) {
        this->theVars = theVars;
        this->start = start;
        this->step = step;
        this->endtime = endtime;
        this->maxSubSteps = maxSubSteps;
        this->abstol = atol;
        this->reltol = rtol;
        data = nullptr;
    }
    virtual void setup() = 0;
    arr run();
    virtual void getResults() = 0;
    static int calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
    virtual arr MB(realtype t, N_Vector u) = 0;
    virtual ~Driver();
    static Variables *theVars;
    arr constraints;
protected:
    friend CVodeMem;
    realtype abstol;
    realtype reltol;
    double start, step, endtime;
    int maxSubSteps;
    realtype *intermediateRes;
    arr results;
    realtype time;
    CalcData* data;
    void *cvode_mem;
};
