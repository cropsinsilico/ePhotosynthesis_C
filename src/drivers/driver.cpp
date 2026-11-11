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

#include "drivers/driver.hpp"
#include "Variables.hpp"
#include <sundials/sundials_math.h>
#include <cvode/cvode.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <sunnonlinsol/sunnonlinsol_newton.h>
#include "drivers/CVodeMem.hpp"
#include <cmath>
#include <iomanip>

namespace {
// CVODE borrows the solvers and vector: release it before its dependencies.
struct SolverResources {
    ePhotosynthesis::drivers::CVodeMem* memory = nullptr;
    N_Vector y = nullptr;
    SUNMatrix matrix = nullptr;
    SUNNonlinearSolver nonlinear = nullptr;
    SUNLinearSolver linear = nullptr;
    ~SolverResources() {
        if (memory) memory->cvode_mem_free();
        if (nonlinear) SUNNonlinSolFree(nonlinear);
        if (linear) SUNLinSolFree(linear);
        if (matrix) SUNMatDestroy(matrix);
        if (y) N_VDestroy(y);
    }
};
}

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

bool Driver::showWarnings = false;

Driver::Driver(Variables *theVars, const double startTime, const double stepSize, const double endTime,
	       const int maxSubsteps, const double atol, const double rtol,
	       const bool showWarn) {
#ifdef SUNDIALS_CONTEXT_REQUIRED
  _context = theVars->context_ptr();
#endif // SUNDIALS_CONTEXT_REQUIRED
  inputVars = theVars;
  start = startTime;
  step = stepSize;
  initialStep = stepSize;
  endtime = endTime;
  maxSubSteps = maxSubsteps;
  showWarnings = showWarn;
  abstol = atol;
  reltol = rtol;
  maxStep = 20. * step;
  data = nullptr;
  cvode_mem = nullptr;
  intermediateRes = nullptr;
  if (inputVars->useC3 && inputVars->EnzymeAct.empty())
      throw std::runtime_error("EnzymeAct must be set if useC3 is True (automatically set for EPS driver)");
}

arr Driver::run() {
    if (!std::isfinite(runtimeLimitSeconds) || runtimeLimitSeconds <= 0.0) {
        throw std::invalid_argument("runtimeLimitSeconds must be finite and positive");
    }
    if (!std::isfinite(start) || !std::isfinite(endtime) || endtime <= start ||
        !std::isfinite(initialStep) || initialStep <= 0.0) {
        throw std::invalid_argument("run requires finite start < endtime and a positive step");
    }
    if (!std::isfinite(abstol) || abstol < 0.0 || !std::isfinite(reltol) || reltol < 0.0 ||
        (abstol == 0.0 && reltol == 0.0)) {
        throw std::invalid_argument("ODE tolerances must be finite, nonnegative, and not both zero");
    }
    runStarted = std::chrono::steady_clock::now();
    lastRunStatus = RunStatus::notRun;
    finalState.clear();
    usedInitialState = false;
    finalStateReusable = false;
    rejectedStateTrials = 0;
    nonfiniteRhsTrials = 0;
    firstRejectedState.clear();
    // Keep the caller's pointer valid even when retries or exceptions use a copy.
    struct InputPointerGuard {
        Variables* original;
        ~InputPointerGuard() { Driver::inputVars = original; }
    } inputGuard{inputVars};
    std::unique_ptr<Variables> originalVars(inputVars->deepcopy());
    std::unique_ptr<Variables> retryVars;
    step = initialStep;
    uint count = 0;
    int max_count = 10; 
    while (count < max_count){
        if (runtimeExpired()) break;
        if (count > 0) {
            retryVars.reset(originalVars->deepcopy());
            inputVars = retryVars.get();
        }
        maxStep = 20. * step;
        SolverResources resources;

        setup();

        sunindextype N =  static_cast<long>(constraints.size());
        N_Vector y;
#ifdef SUNDIALS_CONTEXT_REQUIRED
        y = N_VNew_Serial(N, context());
#else // SUNDIALS_CONTEXT_REQUIRED
        y = N_VNew_Serial(N);
#endif // SUNDIALS_CONTEXT_REQUIRED
        resources.y = y;
        if (!y) throw std::runtime_error("N_VNew_Serial failed");
	sunrealtype* y_ptr = N_VGetArrayPointer(y);

        for (std::size_t i = 0; i < constraints.size(); i++)
            y_ptr[i] = constraints[i];
        realtype t0 = start;

        resources.memory = &CVodeMem::create();
        resources.memory->cvode_mem_init(this, t0, y);

#ifdef SUNDIALS_CONTEXT_REQUIRED
        SUNMatrix A = SUNDenseMatrix(N, N, context());
        resources.matrix = A;
        if (!A) throw std::runtime_error("SUNDenseMatrix failed");
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y, context());
        resources.nonlinear = NLS;
        if (!NLS) throw std::runtime_error("SUNNonlinSol_Newton failed");
        SUNLinearSolver LS = SUNLinSol_Dense(y, A, context());
#else // SUNDIALS_CONTEXT_REQUIRED
        SUNMatrix A = SUNDenseMatrix(N, N);
        resources.matrix = A;
        if (!A) throw std::runtime_error("SUNDenseMatrix failed");
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y);
        resources.nonlinear = NLS;
        if (!NLS) throw std::runtime_error("SUNNonlinSol_Newton failed");
        SUNLinearSolver LS = SUNLinSol_Dense(y, A);
#endif // SUNDIALS_CONTEXT_REQUIRED

        resources.linear = LS;
        if (!LS) throw std::runtime_error("SUNLinSol_Dense failed");
        if (CVodeSetNonlinearSolver(cvode_mem, NLS) != CV_SUCCESS) {
            throw std::runtime_error("CVodeSetNonlinearSolver failed");
        }
        if (CVodeSetLinearSolver(cvode_mem, LS, A) != CV_SUCCESS) {
            throw std::runtime_error("CVodeSetLinearSolver failed");
        }

        configureSolver(y);
        if (const char* reason = stateError(y)) {
            throw std::runtime_error(std::string("invalid initial ODE state: ") + reason);
        }

        usedInitialState = false;
        if (count == 0 && initialState.size() == constraints.size()) {
            std::copy(initialState.begin(), initialState.end(), y_ptr);
            if (stateError(y)) {
                std::copy(constraints.begin(), constraints.end(), y_ptr);
            } else {
                // CVodeInit already copied the default state. ReInit loads the
                // seed and retains tolerances, constraints, and solver options.
                if (CVodeReInit(cvode_mem, start, y) != CV_SUCCESS) {
                    throw std::runtime_error("CVodeReInit for initial state failed");
                }
                usedInitialState = true;
            }
        }

        realtype t = start;
        bool runOK = true;
        realtype tout = std::min(start + step, endtime);
        // Store all output points from the final 100 seconds. The steady-state
        // metric is the absolute least-squares slope of each output variable.
        const double steadyStateWindow = 100.0;
        const double windowStart = std::max(start, endtime - steadyStateWindow);
        std::vector<double> steadyStateTimes;
        std::vector<std::vector<double>> steadyStateData;
        std::vector<double> difference; // absolute gradients, retained name for downstream code
        while (t < endtime) {
            const realtype previousTime = t;
            int flag = CV_RHSFUNC_FAIL;
            if (!runtimeExpired()) {
                flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
            }
            const char* invalidState = stateError(y);
            if (flag != CV_SUCCESS || runtimeExpired() || t <= previousTime || invalidState) {
                long int internalSteps = 0;
                realtype internalStep = 0.0;
                CVodeGetNumSteps(cvode_mem, &internalSteps);
                CVodeGetCurrentStep(cvode_mem, &internalStep);
                const double elapsed = std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - runStarted).count();
                const char* reason = lastRunStatus == RunStatus::timedOut ? "timeout" :
                    (invalidState ? "invalid_state" : (flag != CV_SUCCESS ? "solver_failure" : "no_progress"));
                std::cerr << std::setprecision(17) << "ePhoto integration failed: attempt=" << count + 1
                          << " status=" << reason
                          << " cvode_flag=" << flag << " target_t=" << tout
                          << " reached_t=" << t << " internal_steps=" << internalSteps
                          << " internal_step=" << internalStep
                          << " elapsed_seconds=" << elapsed
                          << " CO2_in=" << originalVars->CO2_in
                          << " light=" << originalVars->TestLi
                          << " temperature=" << inputVars->Tp
                          << " rejected_state_trials=" << rejectedStateTrials
                          << " nonfinite_rhs_trials=" << nonfiniteRhsTrials
                          << " state_error=" << (invalidState ? invalidState : "none")
                          << " first_rejected_state=" << (firstRejectedState.empty() ? "none" : firstRejectedState)
                          << std::endl;
                if (invalidState) {
                    std::cerr << "invalid_accepted_state_begin\n";
                    writeState(std::cerr, y);
                    std::cerr << "invalid_accepted_state_end\n";
                }
                runOK = false;
                break;
            }
            tout = std::min(t + step, endtime);

//this print out all metabolites concentration for each time step
//be careful of adpative time step
           if(inputVars->saveMetabolite){
             std::cout << "t = " << t <<std::endl; 
             GenOut(t,inputVars);
             auto& metabolites = inputVars->CO2A;
             std::vector<double> lastData = metabolites.getLastData();
             std::cout << "This step data:\n";
             for (double val : lastData) {
                 std::cout << val << " ";
             }
             std::cout << std::endl;
           }
// Collect every output point in the final steadyStateWindow seconds.
          if (t >= windowStart - 1e-9 && t <= endtime + 1e-9) {
            GenOut(t, inputVars);
            auto& metabolites = inputVars->CO2A;
            const std::vector<double> currentData = metabolites.getLastData();

            if (!steadyStateData.empty() &&
                currentData.size() != steadyStateData.front().size()) {
              throw std::runtime_error("inconsistent metabolite data size");
            }

            steadyStateTimes.push_back(static_cast<double>(t));
            steadyStateData.push_back(currentData);
          }
        }//end integration loop
        if (runOK && runtimeExpired()) runOK = false;

        if (runOK) {
          // Calculate an ordinary least-squares gradient for each variable:
          // slope = sum((t - mean(t)) * (y - mean(y))) /
          //         sum((t - mean(t))^2)
          if (steadyStateData.size() < 2) {
            throw std::runtime_error(
                "fewer than two data points in the steady-state window");
          }

          const size_t nVariables = steadyStateData.front().size();
          if (nVariables < 52) {
            std::cout << "steady-state data has size of: "
                      << nVariables << std::endl;
            throw std::runtime_error("invalid data size");
          }

          double meanTime = 0.0;
          for (double sampleTime : steadyStateTimes) {
            meanTime += sampleTime;
          }
          meanTime /= static_cast<double>(steadyStateTimes.size());

          double timeVarianceSum = 0.0;
          for (double sampleTime : steadyStateTimes) {
            const double dt = sampleTime - meanTime;
            timeVarianceSum += dt * dt;
          }
          if (timeVarianceSum <= 0.0) {
            throw std::runtime_error(
                "steady-state data points do not span a nonzero time interval");
          }

          difference.assign(nVariables, 0.0);
          for (size_t variable = 0; variable < nVariables; ++variable) {
            double meanValue = 0.0;
            for (const auto& sample : steadyStateData) {
              meanValue += sample[variable];
            }
            meanValue /= static_cast<double>(steadyStateData.size());

            double covarianceSum = 0.0;
            for (size_t sample = 0; sample < steadyStateData.size(); ++sample) {
              covarianceSum +=
                  (steadyStateTimes[sample] - meanTime) *
                  (steadyStateData[sample][variable] - meanValue);
            }

            // Steady state depends on the magnitude, not the sign, of the trend.
            difference[variable] =
                std::abs(covarianceSum / timeVarianceSum);
          }
        }

// Threshold value for checking steady-state metabolite
        double threshold = 1e-4;

// Check if some elements are larger than the threshold
// This is no longer used. Instead we use a penalty on the non-steady-state
// metabolites to make our obj function smoother
// Keep this here for diagnosis purpose only
        bool allSmallerThanThreshold = true;
        if(difference.size()>52){ 
          for (int index=7; index<51; ++index) {
            if (difference[index] > threshold) {
                allSmallerThanThreshold = false;
	        if(inputVars->saveMetabolite){
                  std::cout << "\nWarning: metabolites' steady-state check failed\n";
                }
                break;
            }
          }
        }

        if (runOK) {
            intermediateRes = N_VGetArrayPointer(y);
            time = t;
            getResults();
            if (runtimeExpired()) runOK = false;
        }

        intermediateRes = nullptr;
//if condition 1: ODE runs AND
//condition 2: all metabolites are converged, which is as a penalty
        if (runOK){
//save steady-state metabolite to a text file
//during optimization, this should be turned off to save time
//turn it on for analysis purpose
	  if(inputVars->saveMetabolite){
            GenOut(time,inputVars);
            auto& metabolites = inputVars->CO2A;
            auto lastData = metabolites.getLastData();
            std::string filename = "last_data.txt";
            saveLastDataToFile(lastData, filename);
          }
// Extract subvector
// we only need the difference for metabolites
// difference contains one absolute fitted gradient per output variable
          std::vector<double> sub_vector(difference.begin() + 7,
                                         difference.begin() + 51);
          double penalty = smoothPenalty(sub_vector, threshold);
// the 6th is the CO2 assimilation. See src/GenOut.cpp
          double penalty_assim = 1.0 - exp(-1000.0 * (std::max(difference[6] - threshold,0.0))); 
//return results of assimilation and others
//since we maximize assimilation, the penalty (positive) is deducted
//the penalty takes value from 0 to 1
//the multiplier on the penalty is determined from my own tests and subject to changes 
//If it's too large, the optimizer may not converge
//If it's too small, the optimizer may converge by allowing penalties on some metabolites
//Neither is what we want

          double multiplier = 8.0;
          
	  if(inputVars->saveMetabolite){
            std::cout << "results[0] "<<results[0]<<" Penalty "<<penalty<<
            " multiplier "<<multiplier<<" assim_penalty "<<penalty_assim<<std::endl;
          }
//when record = false, the penalty is always zero despite being calculated
//when runBioCro = true, don't substract the penalties
          results[3] = penalty;
          if(inputVars->runBioCro==false){
            results[0] = results[0] - multiplier * penalty;
          }
          finalStateReusable = reusableFinalState(y);
          if (runtimeExpired()) break;
          finalState.assign(y_ptr, y_ptr + constraints.size());
          lastRunStatus = RunStatus::success;
          return results;
        }

        if (lastRunStatus == RunStatus::timedOut) break;
        count++;
        step = initialStep / (count + 1);
        if (count < max_count) {
            std::cerr << "ePhoto retrying from original inputs: attempt=" << count + 1
                      << " output_step=" << step << " max_internal_step=" << 20. * step
                      << std::endl;
        }
    }
    finalStateReusable = false;
    if (lastRunStatus != RunStatus::timedOut) {
        lastRunStatus = RunStatus::retriesExhausted;
    }
    std::cerr << std::setprecision(17) << "ePhoto returned fallback: status="
              << (lastRunStatus == RunStatus::timedOut ? "timeout" : "retries_exhausted")
              << " CO2_in=" << originalVars->CO2_in
              << " light=" << originalVars->TestLi << " temperature=" << inputVars->Tp
              << " elapsed_seconds=" << std::chrono::duration<double>(
                     std::chrono::steady_clock::now() - runStarted).count()
              << std::endl;
    // Match the BioCro [A, Vc, PR, penalty] contract, with an explicit failure penalty.
    results.assign(failureResultSize(), 0.5);
    results[3] = 1.0;
    return results;
}

bool Driver::runtimeExpired() {
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - runStarted).count()
        >= runtimeLimitSeconds) {
        lastRunStatus = RunStatus::timedOut;
        return true;
    }
    return false;
}

Driver::~Driver() {
    CVodeMem *cmem = nullptr;
    cmem = &CVodeMem::create();
    cmem->cvode_mem_free();
    cvode_mem = nullptr;
#ifdef SUNDIALS_CONTEXT_REQUIRED
    if (_context.use_count() == 1)
        SUNContext_Free(_context.get());
    _context.reset();
#endif // SUNDIALS_CONTEXT_REQUIRED
}

const char* Driver::stateError(N_Vector state) const {
    const auto values = N_VGetArrayPointer(state);
    for (sunindextype i = 0; i < NV_LENGTH_S(state); ++i) {
        if (!std::isfinite(values[i])) return "nonfinite_state";
    }
    return nullptr;
}

void Driver::writeState(std::ostream& out, N_Vector state) const {
    const auto values = N_VGetArrayPointer(state);
    for (sunindextype i = 0; i < NV_LENGTH_S(state); ++i) out << "state[" << i << "]=" << values[i] << '\n';
}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    CalcData *data = static_cast<CalcData*>(user_data);
    Driver* driver = data->drv;
    // Positive asks CVODE to recover from an inadmissible Newton trial by
    // reducing the step and retrying. Unrecoverable failures return negative.
    if (driver->runtimeExpired()) return -1;
    if (const char* reason = driver->stateError(u)) {
        ++driver->rejectedStateTrials;
        if (driver->firstRejectedState.empty()) driver->firstRejectedState = reason;
        return 1;
    }
    arr ddxdt = driver->MB(t, u);
    if (driver->runtimeExpired()) return -1;
    if (ddxdt.size() != static_cast<std::size_t>(NV_LENGTH_S(u_dot))) {
        if (driver->firstRejectedState.empty()) driver->firstRejectedState = "rhs_dimension_mismatch";
        return -1;
    }
    for (double value : ddxdt) {
        if (!std::isfinite(value)) {
            ++driver->nonfiniteRhsTrials;
            return 1;
        }
    }
    std::copy(ddxdt.begin(), ddxdt.end(), N_VGetArrayPointer(u_dot));
    return 0;
}

void Driver::saveLastDataToFile(const std::vector<double> &lastData, const std::string &filename) {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (double val : lastData) {
        outFile << val << " ";
    }

    outFile << std::endl;
    outFile.close();
}

// Function to calculate the smooth penalty
double Driver::smoothPenalty(const std::vector<double>& x, double threshold) {
    double penalty = 0.0;
    double alpha = 1000.0;  // Parameter to control the sharpness of the penalty

    for (double xi : x) {
        if (xi > threshold) {
            penalty += 1.0 - exp(-alpha * (xi - threshold));
        }
    }

    return penalty;
}
