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
#include <cvode/cvode_direct.h>
#include "drivers/CVodeMem.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::drivers;
using namespace ePhotosynthesis::conditions;

bool Driver::showWarnings = false;

arr Driver::run() {
    origVars = new Variables(inputVars);
    uint count = 0;
    int max_count = 10; 
    while (count < max_count){
        maxStep = 20. * step;

        setup();

        sunindextype N =  static_cast<long>(constraints.size());
        N_Vector y;
        y = N_VNew_Serial(N);

        for (std::size_t i = 0; i < constraints.size(); i++)
            NV_Ith_S(y, i) =  constraints[i];
        realtype t0 = start;

        CVodeMem *cmem;
        try {
            cmem = &CVodeMem::create();
            cmem->cvode_mem_init(this, t0, y);
        } catch(...) {
            std::exception_ptr eptr = std::current_exception();
            N_VDestroy(y);
            std::rethrow_exception(eptr);
        }

        data->drv = this;

        SUNMatrix A = SUNDenseMatrix(N, N);
        SUNNonlinearSolver NLS = SUNNonlinSol_Newton(y);
        SUNLinearSolver LS = SUNDenseLinearSolver(y, A);

        try {
            if (CVodeSetNonlinearSolver(cvode_mem, NLS) != CV_SUCCESS) {
                std::cout << "CVodeSetNonlinearSolver failed" << std::endl;
                throw std::runtime_error("CVodeSetNonlinearSolver failed");
            }
            if (CVDlsSetLinearSolver(cvode_mem, LS, A) != CV_SUCCESS) {
                std::cout << "CVDlsSetLinearSolver failed" << std::endl;
                throw std::runtime_error("CVDlsSetLinearSolver failed");
            }
        } catch(...) {
            std::exception_ptr eptr = std::current_exception();
            SUNNonlinSolFree(NLS);
            SUNLinSolFree(LS);
            SUNMatDestroy(A);
            N_VDestroy(y);
            std::rethrow_exception(eptr);
        }

        realtype t = 0;
        bool runOK = true;
        realtype tout = start + step;
        std::vector<double> lastData1,difference; // Declare lastData1 outside the loop
        while (t <= endtime) {
            if (CVode(cvode_mem, tout, y, &t, CV_NORMAL) != CV_SUCCESS) {
                std::cout << "CVode failed at t=" << tout << "  " << t << std::endl;
                runOK = false;
                break;
            }
            tout += step;

//this print out all metabolites concentration for each time step
//be careful of adpative time step
           if(inputVars->saveMetabolite){
             std::cout << "t = " << t <<std::endl; 
             GenOut(t,inputVars);
             TimeSeries<std::vector<double> > metabolites = inputVars->CO2A;
             std::vector<double> lastData = metabolites.getLastData();
             std::cout << "This step data:\n";
             for (double val : lastData) {
                 std::cout << val << " ";
             }
             std::cout << std::endl;
           }
//get the data at endtime-100
          if (std::abs(t - (endtime - 100)) < 1e-6) {
//            std::cout << "t = " << t <<std::endl; 
            GenOut(t,inputVars);
            TimeSeries<std::vector<double> > metabolites = inputVars->CO2A;
            lastData1 = metabolites.getLastData();
//            std::cout << "Last step data:\n";
//            for (double val : lastData1) {
//                std::cout << val << " ";
//            }
//            std::cout << std::endl;
          }
//get the data at endtime and calculate the difference
          if(std::abs(t - endtime) < 1e-6){
//            std::cout << "t = " << t <<std::endl; 
            GenOut(t,inputVars);
            TimeSeries<std::vector<double> > metabolites = inputVars->CO2A;
            auto lastData2 = metabolites.getLastData();
// Ensure difference vector is properly sized
            difference.resize(lastData1.size());
            if(lastData1.size()<52) 
            {
              std::cout << "lastData1 has size of: ";
              std::cout << lastData1.size() <<std::endl;
              throw std::runtime_error("invalid data size"); 
            }
            for (size_t i = 0; i < lastData1.size(); ++i) {
                difference[i] = (lastData2[i] - lastData1[i])/100.;//divide delta t
            }
//            std::cout << "Last step data:\n";
//            for (double val : difference) {
//                std::cout << val << " ";
//            }
//            std::cout << std::endl;
          }
        }//end while
// Threshold value
        double threshold = 1e-4;

// Check if some elements are larger than the threshold
        bool allSmallerThanThreshold = true;
        if(difference.size()>52){ 
          for (int index=7; index<51; ++index) {
            if (difference[index] > threshold) {
                allSmallerThanThreshold = false;
                break;
            }
          }
        }

        if (runOK) {
            intermediateRes = N_VGetArrayPointer(y);
            time = t;
            getResults();
        }

        SUNNonlinSolFree(NLS);
        SUNLinSolFree(LS);
        SUNMatDestroy(A);
        N_VDestroy(y);
//if condition 1: ODE runs AND
//condition 2: all metabolites are converged
        if (runOK){
//save steady-state metabolite to a text file
//during optimization, this should be turned off to save time
//turn it on for analysis purpose
	  if(inputVars->saveMetabolite){
            GenOut(time,inputVars);
            TimeSeries<std::vector<double> > metabolites = inputVars->CO2A;
            auto lastData = metabolites.getLastData();
            std::string filename = "last_data.txt";
            saveLastDataToFile(lastData, filename);
          }
// Extract subvector
// we only need the difference for metabolites
          std::vector<double> sub_vector(difference.begin() + 7,
                                         difference.begin() + 51);
          double penalty = smoothPenalty(sub_vector, threshold);
//return results of assimilation and others
//since we maximize assimilation, the penalty (positive) is deducted
//the penalty takes value from 0 to 1
//the multiplier on the penalty is determined from my own tests and subject to changes 
//If it's too large, the optimizer can not converge
//If it's too small, the optimizer may converge by allowing penalties on some metabolites
//Neither is what we want
          double multiplier = 4.0;
          
	  if(inputVars->saveMetabolite){
            std::cout << "results[0] is "<<results[0]<<" Penalty is "<<penalty<<
            " multiplier is"<<multiplier<<std::endl;
          }
          results[0] = results[0] - multiplier * penalty;
          return results;
        }

// Restore inputVars to its original state
        inputVars = origVars;

        count++;
        step = initialStep / (count + 1);
//        std::cout << "Retrying with smaller step size: " << step << std::endl;
    }
//    throw std::runtime_error("No valid solution found");
//  Create an array of some values
    std::cout << "No solution was found after all adaptive steps." << std::endl;
    arr no_solution_results;
    for (int i = 0; i < 3; ++i) {
//-10 is used as a meaningless value for gross assimilation
        no_solution_results.push_back(0.5);
    }
    return no_solution_results; // Return an array of x if no valid solution found
}

Driver::~Driver() {
    if (origVars != nullptr)
        delete origVars;
}

int Driver::calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    CalcData *data = static_cast<CalcData*>(user_data);
    arr ddxdt = data->drv->MB(t, u);
    for (std::size_t index = 0; index < ddxdt.size(); index++)
        dxdt[index] = ddxdt[index];
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
