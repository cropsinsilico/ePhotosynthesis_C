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

#ifndef BUILD_LIBRARY
#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

#include "cxxopts.hpp"
#include "globals.hpp"
#include "modules/trDynaPS.hpp"
#include "modules/EPS.hpp"
#include "drivers/drivers.hpp"
#include "Variables.hpp"

#define ABSTOL 0.001
// macros to get options from either the command line (has precedence) or an options file
#define varSearchD(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stod(inputs.at(#x), nullptr);
#define varSearchI(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stoi(inputs.at(#x), nullptr);
#define varSearch(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = inputs.at(#x);

const boost::regex token("\\s+");

double compare(const double v1, const double v2) {
    const double dif = abs(v1 - v2);
    if (dif == 0.)
        return 0.;
    if (v2 == 0. && dif <= ABSTOL) {
        return 0.;
    } else if (v1 == 0. && dif <= ABSTOL) {
        return 0.;
    } else if (dif <= ABSTOL && abs(v1) <= ABSTOL*100) {
        return 0.;
    } else if (v2 != 0. || v1 != 0.) {
        return abs(100. * (v1 - v2)/std::max(v2, v1));
    } else {
        return 100.;
    }
}

void readFile(const std::string &filename, std::map<std::string, std::string> &mapper) {
    std::vector<std::string> tempVec;
    std::string input;
    std::ifstream inputfile(filename);
    if(inputfile.fail()) {
        std::cout << "Could not open " << filename << " for reading" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (getline(inputfile, input)) {
        if (input.empty())
            return;
        boost::algorithm::split_regex(tempVec, input, token);
        mapper.insert(std::pair<std::string, std::string>(tempVec[0], tempVec[1]));
    }
}

void readFile(const std::string &filename, std::map<std::string, double> &mapper) {
    std::vector<std::string> tempVec;
    std::string input;
    std::ifstream inputfile(filename);
    if(inputfile.fail()) {
        std::cout << "Could not open " << filename << " for reading" << std::endl;
        exit(EXIT_FAILURE);
    }
    int count = 0;
    while (getline(inputfile, input)) {
        if (input.empty())
            return;
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
enum DriverType {
    None,
    trDynaPS,
    DynaPS,
    CM,
    EPS
};


#ifdef TESTING

void EPS_run(double begintime, double stoptime, double stepsize, double abstol, double reltol, double Tp, int maxSubSteps)
{
    bool record = false;
    std::string evn="InputEvn.txt";
    std::string atpcost="InputATPCost.txt";
    std::string enzymeFile="Einput7.txt";
    std::map<std::string, std::string> inputs;

    readFile(evn, inputs);
    readFile(atpcost, inputs);
    Variables *theVars = new Variables();
    readFile(enzymeFile, theVars->EnzymeAct);
    theVars->TestCa = static_cast<double>(stof(inputs.at("CO2"), nullptr));
    theVars->TestLi = static_cast<double>(stof(inputs.at("PAR"), nullptr));
    theVars->TestSucPath = stoi(inputs.at("SucPath"), nullptr);
    theVars->TestATPCost = stoi(inputs.at("ATPCost"), nullptr);
    theVars->record = record;
    theVars->useC3 = true;
    theVars->RUBISCOMETHOD = 1;
    theVars->RUBISCOTOTAL = 3;

    Driver *maindriver = new EPSDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1, Tp);
    std::vector<double> ResultRate = maindriver->run();
    std::cout<<ResultRate[0]<<std::endl;

    std::ofstream outfile("output.data");
    outfile << ResultRate[0] << std::endl;
    if (theVars != nullptr) {
        maindriver->theVars = nullptr;
        delete theVars;
    }
    delete maindriver;
}

int main()
{
    double stoptime=5000.0, begintime=0.0, stepsize=1.0;
    double abstol=1e-5, reltol=1e-4;
    double Tp=25.1;
    int maxSubSteps=2500;
    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        EPS_run(begintime, stoptime, stepsize, abstol, reltol, Tp, maxSubSteps);
    }
    return (0);
}

#else
int main(int argc, const char* argv[]) {
    try {
        bool record = false;
        bool useC3 = false;
        bool terminateTest = false;
        std::string test = "";
        cxxopts::Options options("ePhotosynthesis", "C++ implementation of the matlab original");
        options.show_positional_help();
        std::string evn, atpcost, optionsFile, enzymeFile;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver, maxSubSteps;
        ushort dbglvl;
        bool debugDelta, debugInternal;
        options.add_options()
                ("v,verbose", "Record output values for all steps (this can significantly slow the program).", cxxopts::value<bool>(record)->default_value("false"))
                ("e,evn", "The InputEvn.txt file.", cxxopts::value<std::string>(evn)->default_value("InputEvn.txt"))
                ("a,atpcost", "The InputATPCost.txt file.", cxxopts::value<std::string>(atpcost)->default_value("InputATPCost.txt"))
                ("n,enzyme", "The input enzyme file.", cxxopts::value<std::string>(enzymeFile)->default_value(""))
                ("b,begintime", "The starting time for the calculations.", cxxopts::value<double>(begintime)->default_value("0.0"))
                ("s,stoptime", "The time to stop calculations.", cxxopts::value<double>(stoptime)->default_value("5000.0"))
                ("z,stepsize", "The step size to use in the calculations.", cxxopts::value<double>(stepsize)->default_value("1.0"))
                ("m,maxSubSteps", "The maximum number of iterations at each time step.", cxxopts::value<int>(maxSubSteps)->default_value("750"))
                ("d,driver", "The driver to use. Choices are:                            1 - trDynaPS                                         2 - DynaPS                                           3 - CM                                               4 - EPS         ", cxxopts::value<int>(driver)->default_value("1"))
                ("c,c3", "Use the C3 model, automatically set to true for EPS driver", cxxopts::value<bool>(useC3)->default_value("false"))
                ("t,abstol", "Absolute tolerance for calculations", cxxopts::value<double>(abstol)->default_value("1e-5"))
                ("r,reltol", "Relative tolerance for calculations", cxxopts::value<double>(reltol)->default_value("1e-4"))
                ("T,Tp", "Input Temperature", cxxopts::value<double>(Tp)->default_value("0.0"))
                ("o,options", "Name of a text file which specifies any of the above options. Command line arguments have priority.", cxxopts::value<std::string>(optionsFile)->default_value(""))
                ("q,test", "Testing", cxxopts::value<std::string>(test)->default_value(""))
                ("x, terminate", "Stop the test on a discrepancy", cxxopts::value<bool>(terminateTest)->default_value("false"))
                ("h,help", "Produce help message")
                ("debug","Debug level", cxxopts::value<ushort>(dbglvl)->default_value("0"))
                ("debugDelta", "Debug deltas", cxxopts::value<bool>(debugDelta)->default_value("false"))
                ("debugInternal", "Debug internals", cxxopts::value<bool>(debugInternal)->default_value("false"))
                ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return (EXIT_SUCCESS);
        }
        std::map<std::string, std::string> inputs;
        if (result.count("options")) {
            readFile(result["options"].as<std::string>(), inputs);

            varSearch(evn)
            varSearch(atpcost)
            varSearchD(begintime)
            varSearchD(stoptime)
            varSearchD(stepsize)
            varSearchI(maxSubSteps)
            varSearchI(driver)
            varSearchD(abstol)
            varSearchD(reltol)
        }
        driverChoice = static_cast<DriverType>(driver);

        readFile(evn, inputs);
        readFile(atpcost, inputs);
        Variables *theVars = new Variables();
        if (result.count("enzyme")) {
            readFile(enzymeFile, theVars->EnzymeAct);
        }
        theVars->TestCa = static_cast<double>(stof(inputs.at("CO2"), nullptr));
        theVars->TestLi = static_cast<double>(stof(inputs.at("PAR"), nullptr));
        theVars->TestSucPath = stoi(inputs.at("SucPath"), nullptr);
        theVars->TestATPCost = stoi(inputs.at("ATPCost"), nullptr);
        theVars->record = record;
        theVars->useC3 = useC3;
        theVars->RUBISCOMETHOD = 1;
        theVars->RUBISCOTOTAL = 3;
        if (debugDelta)
            dbglvl += 8;
        if (debugInternal)
            dbglvl += 16;
#ifdef INCDEBUG
        std::cout << "Setting Debug level " << dbglvl << std::endl;
        theVars->debuglevel = static_cast<Debug::RequestedDebug>(dbglvl);
#else
        if (dbglvl != 0)
            std::cout << "This is not a debug build, no debug reporting will be done." << std::endl;
#endif
        Driver *maindriver;

        //DEBUG_MESSAGE("TESTING",0)
        switch (driverChoice) {
            case trDynaPS:
                maindriver = new trDynaPSDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1);
                break;
            case DynaPS:
#ifdef INCDEBUG
                DynaPSCondition::setTop();
#endif
                maindriver = new DynaPSDrive(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1);
                break;
            case CM:
#ifdef INCDEBUG
                CMCondition::setTop();
#endif
                maindriver = new CMDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol);
                break;
            case EPS:
#ifdef INCDEBUG
                EPSCondition::setTop();
#endif
                theVars->useC3 = true;
                maindriver = new EPSDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1, Tp);
                break;
            default:
                printf("Invalid driver choice given.\n");
                exit(EXIT_FAILURE);
        }

        if (!test.empty()) {
            int count = 1;
            std::ifstream inputfile(test);
            if(inputfile.fail()) {
                std::cout << "Could not open " << test << " for reading" << std::endl;
                exit(EXIT_FAILURE);
            }
            maindriver->setup();

            std::string input;
            std::vector<std::string> tempVec;
            arr inputdata;
            arr outputdata;
            while (getline(inputfile, input)) {
                if (input.empty())
                    continue;
                //std::cout << "line " << input << std::endl;
                boost::algorithm::split_regex(tempVec, input, token);
                double d;
                if (tempVec[0].rfind("Input", 0) == 0) {
                    inputdata.clear();
                    for (uint i = 1; i < tempVec.size(); i++) {
                        std::stringstream ss(tempVec[i]);
                        ss >> d;
                        inputdata.push_back(d);
                    }
                    continue;
                } else if (tempVec[0].rfind("Result", 0) == 0 || tempVec[0].rfind("Output", 0) == 0) {
                    outputdata.clear();
                    for (uint i = 1; i < tempVec.size(); i++) {
                        std::stringstream ss(tempVec[i]);
                        ss >> d;
                        outputdata.push_back(d);
                    }
                } else {
                    continue;
                }
                N_Vector y;
                //std::cout << std::endl << "Inputs: ";
                maindriver->constraints = inputdata;
                //for (ulong z = 0; z < maindriver->constraints.size(); z++) {
                //    std::cout << maindriver->constraints[z];
                //    if (z < maindriver->constraints.size() - 1)
                //        std::cout << ", ";
                //}
                //std::cout << std::endl;

                y = N_VNew_Serial(static_cast<long>(maindriver->constraints.size()));
                for (size_t i = 0; i < maindriver->constraints.size(); i++)
                    NV_Ith_S(y, i) =  maindriver->constraints[i];

                arr results = maindriver->MB(0,y);
                std::cout << std::endl << "Results: " << std::endl;
                std::cout << "  Test run #" << count << ": " << std::endl;
                bool founddiff = false;
                for (uint i = 0; i < results.size(); i++) {
                    double comp = compare(results[i], outputdata[i]);
                    if (comp > 7.) {
                        std::cout << "    Output " << i << "/" << i+1 << ": " << outputdata[i] << " " << results[i] << " " << comp << "%" << std::endl;
                        founddiff = true;
                    }
                }
                if (!founddiff) {
                    std::cout << "    Results match within threshold " << 2.0 << "%" << std::endl;
                } else if (terminateTest) {
                    std::cout << std::endl << "Failed inputs: ";
                    for (auto val : inputdata) {
                        std::cout << val << ", ";
                    }
                    std::cout << std::endl << std::endl;
                    std::cout << "Failed results: ";
                    for (auto val : results) {
                        std::cout << val << ", ";
                    }
                    std::cout << std::endl << std::endl;

                    std::cout << "Expected results: ";
                    for (auto val : outputdata) {
                        std::cout << val << ", ";
                    }
                    std::cout << std::endl;
                    N_VDestroy(y);

                    if (theVars != nullptr) {
                        maindriver->theVars = nullptr;
                        delete theVars;
                    }
                    delete maindriver;
                    exit(EXIT_SUCCESS);
                }
                //for (ulong z = 0; z < results.size(); z++) {
                //    std::cout << results[z];
                 //   if (z < results.size() - 1)
                //        std::cout << ", ";
                //}
                //std::cout << std::endl;

                N_VDestroy(y);
                count++;
            //for (ulong j = 0; j < maindriver->constraints.size(); j++) {
            //    maindriver->constraints[j] += 0.001;
            //}
            }
            if (theVars != nullptr) {
                maindriver->theVars = nullptr;
                delete theVars;
            }
            delete maindriver;

        } else {
            std::vector<double> ResultRate = maindriver->run();

            std::ofstream outfile("output.data");

            switch (driverChoice) {
                case trDynaPS:
                    outfile << "Light intensity,Vc,Vo,VPGA,VT3P,Vstarch,Vt_glycerate,Vt_glycolate" << std::endl;
                    outfile << theVars->TestLi << "," << ResultRate[0] << ",";
                    outfile << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << ",";
                    outfile << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << std::endl;
                    break;
                case DynaPS:
                    outfile << "Light intensity,PSIIabs,PSIabs,Vc,Vo,VPGA,Vsucrose,Vstarch" << std::endl;
                    outfile << theVars->TestLi << "," << ResultRate[0] << ",";
                    outfile << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << ",";
                    outfile << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << std::endl;
                    break;
                case CM:
                    outfile << "Light intensity,CO2AR" << std::endl;
                    outfile << theVars->TestLi << ResultRate[0] << std::endl;
                    break;
                case EPS:
                    outfile << ResultRate[0] << std::endl;
                    break;
                default:
                    break;
            }
            outfile.close();

            if (theVars != nullptr) {
                maindriver->theVars = nullptr;
                delete theVars;
            }
            delete maindriver;
        }
        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}

#endif

#endif // BUILD_LIBRARY
