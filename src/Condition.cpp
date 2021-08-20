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
#include <math.h>
#include "Variables.hpp"
#include "globals.hpp"
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

const boost::regex token("\\s+");

// Function [light] = condition; This function is used to store all the required
// environmental variables, such as light, CO2, O2, humidity as such. This function
// contains two parts. Part a includes the generic (default) conditions and the
// second part contains the detailed conditions for different time period.
using namespace ePhotosynthesis;

void ePhotosynthesis::Condition(double t, Variables *theVars) {
    (void)t;
    // This variable is transfered from PSInitial for modificatin of V16, the rate of ATP synthesis.
    double CO2Temp = theVars->CO2_in;//280;          // CO2 concentation  // ppm
    if (!theVars->useC3)
        CO2Temp *= 0.7;
    const double O2Temp = 0.21;          // O2 concentration  //default is 0.21, i.e. 21//.

    theVars->CO2_cond = CO2Temp / (3. * pow(10., 4.));
    theVars->O2_cond = O2Temp * 1.26;
    const double light = theVars->TestLi * 0.85 * 0.85;// light umol m-2 s-1

    // Here the time dependent variable is regulated.

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //////////   Here define how many interval needed for the experiments  //
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // This is an experimental protocol for doing repeatative experiment

    // double Tinter = theVars->tglobal / NumberInterval;

    // bool FirstMet = false;

    // for (size_t index = 0; index < NumberInterval; index++) {
    //     double b = index * Tinter;
    //     if (t <= b && !FirstMet) {
    //
    //         int modifier = 1 * index;
    //
    //         global BF2XanCycle_pHl;
    //
    //         global Xan2Stom_ABA;
    //
    //
    //         // Light regulation
    //         light =  100 * index;
    //         light = 2000;
    //
    //         // CO2 regulation
    //         doble temp = 280;
    //         //temp = 1000-100 * (index-1);
    //
    //         // O2 regualtion
    //         O2Temp = 0.21 ;
    //
    //         theVars->CO2_cond = temp /(3 * 10 ^ 4);
    //         theVars->O2_cond = O2Temp*1.26;
    //
    //         // Regulation of Vmax
    //         // Global RuACT_RC;
    //         // RuACT_RC(9) = 25 + 25  * index/2;
    //
    //         FirstMet = true;
    //
    //     }
    // }



    // // // PAM MEASUREMENT
    // if (t < 20.) {
    //     light = 7.;
    // } else {
    //     light = 500.;
    // }
    // // // PAM MEASUREMENT
    // double StepL = 20.;
    // if (t < 1.) {
    //     light = 8000.;
    // } else if (t > 1. && t < 20.) {
    //     light = 1.;
    // } else {
    //     int index = floor(t / StepL);
    //     if (t > (StepL * index) && t < (StepL * index + 1)) {
    //         light = 8000.;
    //     } else {
    //         light = 500.;
    //     }
    // }


    // if (t < 200.) {
    //     light = 1000.;
    // } else if (t > 200. && t < 400.) {
    //     light = 100.;
    // } else {
    //     light = 1000.;
    // }

    theVars->GLight = light;
}

void ePhotosynthesis::readFile(const std::string &filename, std::map<std::string, std::string> &mapper) {
    std::vector<std::string> tempVec;
    std::string input;
    std::ifstream inputfile(filename);
    if(inputfile.fail()) {
        std::string errmsg = "Could not open " + filename + " for reading";
        throw std::runtime_error(errmsg);
    }
    while (getline(inputfile, input)) {
        if (input.empty())
            continue;
        boost::algorithm::split_regex(tempVec, input, token);
        mapper.insert(std::pair<std::string, std::string>(tempVec[0], tempVec[1]));
    }
}

void ePhotosynthesis::readFile(const std::string &filename, std::map<std::string, double> &mapper) {
    std::vector<std::string> tempVec;
    std::string input;
    std::ifstream inputfile(filename);
    if(inputfile.fail()) {
        std::string errmsg = "Could not open " + filename + " for reading";
        throw std::runtime_error(errmsg);
    }
    int count = 0;
    while (getline(inputfile, input)) {
        if (input.empty())
            continue;
        boost::algorithm::split_regex(tempVec, input, token);
        double d;
        std::stringstream ss(tempVec[1]);
        ss >> d;
        if (count < 27)
            d /= 30.;
        count++;
        mapper.insert(std::pair<std::string, double>(tempVec[0], d));
    }
}
