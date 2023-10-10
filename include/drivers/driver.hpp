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

namespace ePhotosynthesis {
class Variables;
namespace drivers {

class Driver;
class CVodeMem;

/**
  Data structure for passing parameters to the ODE solver
  */
struct UserData {
  std::vector<realtype> coeffs;
};

/**
  Create an instance of UserData

  \returns New instance of the UserData structure.
  */
inline UserData *alloc_user_data() {
  return new UserData();
}

/**
  Structure for passing a driver to the ODE routines
  */
struct CalcData {
    Driver *drv;
};

/**
  Create an instance of CalcData.

  \returns New instance of the CalcData structure.
  */
inline CalcData *alloc_calc_data() {
    return new CalcData();
}

/**
  Base class for all drivers. These classes handle taking the input data, feeding it to the ODE solver,
  iterating until the solver reaches its stopping time or fails, and returning the results.
  */
class Driver {
public:
    /**
      Generate the initial parameter Condition class for the solver to use.
      */
    virtual void setup() = 0;

    /**
      Run the ODE solver and return the results.

      \return `std::vector<double>` containg the results of the computations.
      \exception std::runtime_error General failure.
      */
    arr run();

    /**
      Runs the solver one more time on the intermediate results to get the solution at the end time
      stamp.
      */
    virtual void getResults() = 0;

    /**
      Wrapper for the underlying Module MB function. Takes the inputs and converts them to the
      appropriate Condition class.

      \param t The current timestamp.
      \param u A specialized data vector containgin the inputs which are converted to a Condition class.
      \returns dy/dt values for the step.
      */
    virtual arr MB(realtype t, N_Vector u) = 0;
    virtual ~Driver();
    static Variables *inputVars;  // the instance of Variables to use for all calculations.
    arr constraints;   // serialized version of the Condition class being used.

protected:
    friend CVodeMem;
    /**
      This sets up initial values for the class.

      \param[in, out] theVars Instance of the Variables class, which holds global scope variables and
             intermediate results.
      \param startTime The timestamp to start the solver at. Typically this is 0.
      \param stepSize The optimal step size for the solver to use.
      \param endTime The timestamp to end the calculations.
      \param maxSubsteps The maximum number of sub-iterations to make between steps.
      \param atol The absolute tolerance the ODE solver should use.
      \param rtol The relative tolerance the ODE solver should use.
      \param showWarn Whether or not to send solver warnings to stdout. If `true` then all
             warning messages are sent. If `false`, the default, warning messages are not sent,
             reducing clutter in production output.
      */
    Driver(Variables *theVars, const double startTime, const double stepSize, const double endTime,
           const int maxSubsteps, const double atol, const double rtol,
           const bool showWarn = false) {
        this->inputVars = theVars;
        this->start = startTime;
        this->step = stepSize;
        initialStep = stepSize;
        this->endtime = endTime;
        this->maxSubSteps = maxSubsteps;
        this->showWarnings = showWarn;
        abstol = atol;
        reltol = rtol;
        maxStep = 20. * step;
        data = nullptr;
        origVars = nullptr;
    }
    /**
      Does the computations and generates the results for each step or sub-step in the solver.
      A pointer to this function is passed to the ODE solver. The API of the function cannot change
      as it is dictated by the solver.

      \param t The current timestamp.
      \param u The inputs
      \param u_dot[out] Pointer to the resulting dy/dt values.
      \param user_data Any user supplied data.
      */
    static int calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data);

    realtype abstol;            // absolute tolerance
    realtype reltol;            // relative tolerance
    double start, step, endtime; // time stuff
    double initialStep;         // used to store the initial step size
    int maxSubSteps;
    realtype *intermediateRes;
    arr results;
    realtype time;
    CalcData* data;
    double maxStep;
    void *cvode_mem;
    static bool showWarnings;
private:
    Driver() {}
    Variables* origVars;
};

}  // namespace drivers
}  // namespace ePhotosynthesis
