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

#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include <boost/algorithm/string.hpp>

#include "cxxopts.hpp"
#include "globals.hpp"
#include "trDynaPS.hpp"
#include "EPS.hpp"
#include "Variables.hpp"

// macros to get options from either the command line (has precedence) or an options file
#define varSearchD(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stod(inputs.at(#x), nullptr);
#define varSearchI(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stoi(inputs.at(#x), nullptr);
#define varSearch(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = inputs.at(#x);


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
        boost::split(tempVec, input, boost::is_any_of("\t "));
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
    while (getline(inputfile, input)) {
        if (input.empty())
            return;
        boost::split(tempVec, input, boost::is_any_of("\t "));
        double d;
        std::stringstream ss(tempVec[1]);
        ss >> d;
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

int main(int argc, const char* argv[]) {
    try {
        bool record = false;
        bool useC3 = false;
        cxxopts::Options options("ePhotosynthesis", "C++ implementation of the matlab original");
        options.show_positional_help();
        std::string evn, atpcost, optionsFile, enzymeFile;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver, maxSubSteps;
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
                ("h,help", "Produce help message")
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

        Driver *maindriver;

        switch (driverChoice) {
            case trDynaPS:
                maindriver = new trDynaPSDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1);
                break;
            case DynaPS:
                maindriver = new DynaPSDrive(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1);
                break;
            case CM:
                maindriver = new CMDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol);
                break;
            case EPS:
                theVars->useC3 = true;
                maindriver = new EPSDriver(theVars, begintime, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1, Tp);
                break;
            default:
                printf("Invalid driver choice given.\n");
                exit(EXIT_FAILURE);
        }

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

        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}

